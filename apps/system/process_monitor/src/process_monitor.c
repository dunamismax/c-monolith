#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ncurses.h>
#include <ctype.h>
#include <limits.h>

#define MAX_PROCESSES 2048
#define MAX_COMMAND_LENGTH 256
#define MAX_PATH_LENGTH 512
#define MAX_FILTER_LENGTH 64
#define REFRESH_INTERVAL_MS 1000

typedef enum {
    SORT_PID,
    SORT_CPU,
    SORT_MEMORY,
    SORT_TIME,
    SORT_COMMAND
} sort_mode_t;

typedef enum {
    VIEW_PROCESSES,
    VIEW_TREE,
    VIEW_SYSTEM
} view_mode_t;

typedef struct {
    pid_t pid;
    pid_t ppid;
    char command[MAX_COMMAND_LENGTH];
    char state;
    unsigned long utime;
    unsigned long stime;
    long priority;
    long nice;
    unsigned long vsize;
    long rss;
    unsigned long starttime;
    double cpu_percent;
    double memory_percent;
    int children_count;
    pid_t children[64];
} process_info_t;

typedef struct {
    unsigned long total_memory;
    unsigned long free_memory;
    unsigned long available_memory;
    unsigned long buffers;
    unsigned long cached;
    double cpu_usage;
    double load_avg[3];
    int num_processes;
    int num_threads;
    long uptime;
} system_info_t;

typedef struct {
    process_info_t processes[MAX_PROCESSES];
    int count;
    system_info_t system;
    sort_mode_t sort_mode;
    view_mode_t view_mode;
    bool show_tree;
    bool show_threads;
    char filter[MAX_FILTER_LENGTH];
    int selected_process;
    unsigned long last_total_time;
    struct timeval last_update;
} monitor_state_t;

static volatile bool running = true;
static monitor_state_t state = {0};

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        running = false;
    }
}

bool is_number(const char *str) {
    if (!str || *str == '\0') return false;
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool read_proc_stat(const char *pid_str, process_info_t *proc) {
    char path[MAX_PATH_LENGTH];
    FILE *file;
    
    snprintf(path, sizeof(path), "/proc/%s/stat", pid_str);
    file = fopen(path, "r");
    if (!file) return false;
    
    char comm[MAX_COMMAND_LENGTH];
    int ret = fscanf(file, "%d %s %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %ld %ld %*d %*d %lu %lu %ld",
                     &proc->pid, comm, &proc->state, &proc->ppid,
                     &proc->utime, &proc->stime, &proc->priority, &proc->nice,
                     &proc->starttime, &proc->vsize, &proc->rss);
    
    fclose(file);
    
    if (ret < 11) return false;
    
    strncpy(proc->command, comm, sizeof(proc->command) - 1);
    proc->command[sizeof(proc->command) - 1] = '\0';
    
    if (proc->command[0] == '(' && proc->command[strlen(proc->command) - 1] == ')') {
        proc->command[strlen(proc->command) - 1] = '\0';
        memmove(proc->command, proc->command + 1, strlen(proc->command));
    }
    
    snprintf(path, sizeof(path), "/proc/%s/cmdline", pid_str);
    file = fopen(path, "r");
    if (file) {
        char cmdline[MAX_COMMAND_LENGTH];
        if (fgets(cmdline, sizeof(cmdline), file)) {
            for (size_t i = 0; cmdline[i] && i < sizeof(cmdline) - 1; i++) {
                if (cmdline[i] == '\0') cmdline[i] = ' ';
            }
            if (strlen(cmdline) > 0) {
                strncpy(proc->command, cmdline, sizeof(proc->command) - 1);
                proc->command[sizeof(proc->command) - 1] = '\0';
            }
        }
        fclose(file);
    }
    
    return true;
}

bool read_system_meminfo(system_info_t *sys) {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return false;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line, "MemTotal: %lu kB", &sys->total_memory);
        } else if (strncmp(line, "MemFree:", 8) == 0) {
            sscanf(line, "MemFree: %lu kB", &sys->free_memory);
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %lu kB", &sys->available_memory);
        } else if (strncmp(line, "Buffers:", 8) == 0) {
            sscanf(line, "Buffers: %lu kB", &sys->buffers);
        } else if (strncmp(line, "Cached:", 7) == 0) {
            sscanf(line, "Cached: %lu kB", &sys->cached);
        }
    }
    
    fclose(file);
    return true;
}

bool read_system_loadavg(system_info_t *sys) {
    FILE *file = fopen("/proc/loadavg", "r");
    if (!file) return false;
    
    int ret = fscanf(file, "%lf %lf %lf", &sys->load_avg[0], &sys->load_avg[1], &sys->load_avg[2]);
    fclose(file);
    
    return ret == 3;
}

bool read_system_uptime(system_info_t *sys) {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) return false;
    
    double uptime_seconds;
    int ret = fscanf(file, "%lf", &uptime_seconds);
    fclose(file);
    
    if (ret == 1) {
        sys->uptime = (long)uptime_seconds;
        return true;
    }
    return false;
}

void calculate_cpu_usage(process_info_t *proc, unsigned long total_time_diff) {
    if (total_time_diff > 0) {
        unsigned long proc_time = proc->utime + proc->stime;
        proc->cpu_percent = (double)proc_time / total_time_diff * 100.0;
    } else {
        proc->cpu_percent = 0.0;
    }
}

void calculate_memory_usage(process_info_t *proc, const system_info_t *sys) {
    if (sys->total_memory > 0) {
        proc->memory_percent = (double)(proc->rss * 4) / (sys->total_memory) * 100.0;
    } else {
        proc->memory_percent = 0.0;
    }
}

bool update_processes(monitor_state_t *state) {
    DIR *proc_dir = opendir("/proc");
    if (!proc_dir) return false;
    
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    
    unsigned long current_total_time = 0;
    FILE *stat_file = fopen("/proc/stat", "r");
    if (stat_file) {
        char line[256];
        if (fgets(line, sizeof(line), stat_file)) {
            unsigned long user, nice, system, idle, iowait, irq, softirq, steal = 0;
            if (sscanf(line, "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
                      &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal) == 8) {
                current_total_time = user + nice + system + idle + iowait + irq + softirq + steal;
            }
        }
        fclose(stat_file);
    }
    
    unsigned long total_time_diff = current_total_time - state->last_total_time;
    state->last_total_time = current_total_time;
    
    state->count = 0;
    
    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL && state->count < MAX_PROCESSES) {
        if (!is_number(entry->d_name)) continue;
        
        process_info_t proc = {0};
        if (read_proc_stat(entry->d_name, &proc)) {
            calculate_cpu_usage(&proc, total_time_diff);
            calculate_memory_usage(&proc, &state->system);
            state->processes[state->count] = proc;
            state->count++;
        }
    }
    
    closedir(proc_dir);
    
    for (int i = 0; i < state->count; i++) {
        state->processes[i].children_count = 0;
        for (int j = 0; j < state->count; j++) {
            if (state->processes[j].ppid == state->processes[i].pid) {
                if (state->processes[i].children_count < 64) {
                    state->processes[i].children[state->processes[i].children_count] = state->processes[j].pid;
                    state->processes[i].children_count++;
                }
            }
        }
    }
    
    state->last_update = current_time;
    return true;
}

bool update_system_info(system_info_t *sys) {
    if (!read_system_meminfo(sys)) return false;
    if (!read_system_loadavg(sys)) return false;
    if (!read_system_uptime(sys)) return false;
    
    FILE *file = fopen("/proc/stat", "r");
    if (file) {
        char line[256];
        if (fgets(line, sizeof(line), file)) {
            unsigned long user, nice, system, idle, iowait, irq, softirq, steal = 0;
            if (sscanf(line, "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
                      &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal) == 8) {
                unsigned long total = user + nice + system + idle + iowait + irq + softirq + steal;
                unsigned long active = total - idle;
                sys->cpu_usage = total > 0 ? (double)active / total * 100.0 : 0.0;
            }
        }
        fclose(file);
    }
    
    return true;
}

int compare_processes(const void *a, const void *b) {
    const process_info_t *proc_a = (const process_info_t *)a;
    const process_info_t *proc_b = (const process_info_t *)b;
    
    switch (state.sort_mode) {
        case SORT_PID:
            return proc_a->pid - proc_b->pid;
        case SORT_CPU:
            if (proc_b->cpu_percent > proc_a->cpu_percent) return 1;
            if (proc_b->cpu_percent < proc_a->cpu_percent) return -1;
            return 0;
        case SORT_MEMORY:
            if (proc_b->memory_percent > proc_a->memory_percent) return 1;
            if (proc_b->memory_percent < proc_a->memory_percent) return -1;
            return 0;
        case SORT_TIME:
            return (proc_b->utime + proc_b->stime) - (proc_a->utime + proc_a->stime);
        case SORT_COMMAND:
            return strcmp(proc_a->command, proc_b->command);
        default:
            return 0;
    }
}

void sort_processes(monitor_state_t *state) {
    qsort(state->processes, state->count, sizeof(process_info_t), compare_processes);
}

bool matches_filter(const process_info_t *proc, const char *filter) {
    if (!filter || strlen(filter) == 0) return true;
    
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", proc->pid);
    
    return strstr(proc->command, filter) != NULL || strstr(pid_str, filter) != NULL;
}

void draw_header(void) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    (void)max_y;
    
    attron(A_REVERSE);
    mvprintw(0, 0, "Process Monitor v1.0");
    for (int i = strlen("Process Monitor v1.0"); i < max_x; i++) {
        printw(" ");
    }
    
    char time_str[64];
    time_t now = time(NULL);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&now));
    mvprintw(0, max_x - strlen(time_str) - 1, "%s", time_str);
    attroff(A_REVERSE);
}

void draw_system_info(int start_row) {
    mvprintw(start_row, 0, "System Information:");
    mvprintw(start_row + 1, 2, "Uptime: %ld days, %ld:%02ld:%02ld",
             state.system.uptime / 86400, (state.system.uptime % 86400) / 3600,
             (state.system.uptime % 3600) / 60, state.system.uptime % 60);
    mvprintw(start_row + 2, 2, "Load Average: %.2f %.2f %.2f",
             state.system.load_avg[0], state.system.load_avg[1], state.system.load_avg[2]);
    mvprintw(start_row + 3, 2, "CPU Usage: %.1f%%", state.system.cpu_usage);
    mvprintw(start_row + 4, 2, "Memory: %lu MB total, %lu MB free, %lu MB available",
             state.system.total_memory / 1024, state.system.free_memory / 1024,
             state.system.available_memory / 1024);
    mvprintw(start_row + 5, 2, "Processes: %d", state.count);
}

void draw_process_header(int row) {
    attron(A_REVERSE);
    mvprintw(row, 0, "  PID USER      PR  NI    VIRT    RES    SHR S  %%CPU %%MEM     TIME+ COMMAND");
    attroff(A_REVERSE);
}

void draw_process_line(int row, int index __attribute__((unused)), const process_info_t *proc, bool selected) {
    if (selected) {
        attron(A_REVERSE);
    }
    
    mvprintw(row, 0, "%5d %-8s %3ld %3ld %7lu %6ld %6s %c %5.1f %4.1f %9s %s",
             proc->pid, "user", proc->priority, proc->nice,
             proc->vsize / 1024, proc->rss * 4,
             "0", proc->state, proc->cpu_percent, proc->memory_percent,
             "0:00.00", proc->command);
    
    if (selected) {
        attroff(A_REVERSE);
    }
}

void draw_tree_process(int row, int depth, const process_info_t *proc, bool selected) {
    if (selected) {
        attron(A_REVERSE);
    }
    
    char indent[64] = "";
    for (int i = 0; i < depth && i < 15; i++) {
        strcat(indent, "  ");
    }
    
    mvprintw(row, 0, "%s%5d %5.1f%% %4.1f%% %s",
             indent, proc->pid, proc->cpu_percent, proc->memory_percent, proc->command);
    
    if (selected) {
        attroff(A_REVERSE);
    }
}

void draw_tree_recursive(int *current_row, int max_row, pid_t ppid, int depth) {
    if (*current_row >= max_row) return;
    
    for (int i = 0; i < state.count; i++) {
        if (state.processes[i].ppid == ppid && matches_filter(&state.processes[i], state.filter)) {
            bool selected = (i == state.selected_process);
            draw_tree_process(*current_row, depth, &state.processes[i], selected);
            (*current_row)++;
            
            if (*current_row >= max_row) return;
            
            draw_tree_recursive(current_row, max_row, state.processes[i].pid, depth + 1);
        }
    }
}

void draw_processes(int start_row, int max_rows) {
    if (state.view_mode == VIEW_TREE) {
        mvprintw(start_row, 0, "Process Tree:");
        start_row++;
        max_rows--;
        
        int current_row = start_row;
        draw_tree_recursive(&current_row, start_row + max_rows, 0, 0);
    } else {
        draw_process_header(start_row);
        start_row++;
        max_rows--;
        
        int displayed = 0;
        for (int i = 0; i < state.count && displayed < max_rows; i++) {
            if (matches_filter(&state.processes[i], state.filter)) {
                bool selected = (i == state.selected_process);
                draw_process_line(start_row + displayed, i, &state.processes[i], selected);
                displayed++;
            }
        }
    }
}

void draw_help(int start_row) {
    mvprintw(start_row, 0, "Help:");
    mvprintw(start_row + 1, 2, "q/Q - Quit");
    mvprintw(start_row + 2, 2, "k/K - Kill selected process");
    mvprintw(start_row + 3, 2, "t/T - Toggle tree view");
    mvprintw(start_row + 4, 2, "s/S - Change sort mode");
    mvprintw(start_row + 5, 2, "f/F - Filter processes");
    mvprintw(start_row + 6, 2, "r/R - Refresh");
    mvprintw(start_row + 7, 2, "↑/↓ - Navigate processes");
    mvprintw(start_row + 8, 2, "Space - Pause/Resume");
}

void draw_status_line(void) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    attron(A_REVERSE);
    mvprintw(max_y - 1, 0, "Sort: %s | View: %s | Filter: '%s' | Processes: %d",
             (state.sort_mode == SORT_PID) ? "PID" :
             (state.sort_mode == SORT_CPU) ? "CPU" :
             (state.sort_mode == SORT_MEMORY) ? "Memory" :
             (state.sort_mode == SORT_TIME) ? "Time" : "Command",
             (state.view_mode == VIEW_TREE) ? "Tree" : "List",
             state.filter, state.count);
    
    for (int i = strlen("Sort: ") + 20; i < max_x; i++) {
        printw(" ");
    }
    attroff(A_REVERSE);
}

void redraw_screen(void) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    clear();
    
    draw_header();
    
    if (state.view_mode == VIEW_SYSTEM) {
        draw_system_info(2);
        draw_help(9);
    } else {
        draw_system_info(2);
        int processes_start = 8;
        int processes_height = max_y - processes_start - 2;
        draw_processes(processes_start, processes_height);
    }
    
    draw_status_line();
    refresh();
}

bool kill_process(pid_t pid, int signal) {
    if (kill(pid, signal) == 0) {
        return true;
    }
    return false;
}

void handle_input(void) {
    int ch = getch();
    
    switch (ch) {
        case 'q':
        case 'Q':
        case 27:
            running = false;
            break;
            
        case 'k':
        case 'K':
            if (state.selected_process >= 0 && state.selected_process < state.count) {
                pid_t pid = state.processes[state.selected_process].pid;
                if (kill_process(pid, SIGTERM)) {
                    mvprintw(LINES - 2, 0, "Sent SIGTERM to process %d", pid);
                } else {
                    mvprintw(LINES - 2, 0, "Failed to kill process %d", pid);
                }
                refresh();
                usleep(500000);
            }
            break;
            
        case 't':
        case 'T':
            state.view_mode = (state.view_mode == VIEW_TREE) ? VIEW_PROCESSES : VIEW_TREE;
            break;
            
        case 's':
        case 'S':
            state.sort_mode = (state.sort_mode + 1) % 5;
            sort_processes(&state);
            break;
            
        case 'f':
        case 'F':
            echo();
            mvprintw(LINES - 2, 0, "Filter: ");
            refresh();
            getnstr(state.filter, MAX_FILTER_LENGTH - 1);
            noecho();
            break;
            
        case 'r':
        case 'R':
            update_processes(&state);
            update_system_info(&state.system);
            sort_processes(&state);
            break;
            
        case KEY_UP:
            if (state.selected_process > 0) {
                state.selected_process--;
            }
            break;
            
        case KEY_DOWN:
            if (state.selected_process < state.count - 1) {
                state.selected_process++;
            }
            break;
            
        case ' ':
            break;
            
        case '1':
            state.view_mode = VIEW_PROCESSES;
            break;
        case '2':
            state.view_mode = VIEW_TREE;
            break;
        case '3':
            state.view_mode = VIEW_SYSTEM;
            break;
    }
}

void init_monitor(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    state.sort_mode = SORT_CPU;
    state.view_mode = VIEW_PROCESSES;
    state.selected_process = 0;
    state.filter[0] = '\0';
    
    gettimeofday(&state.last_update, NULL);
}

void cleanup_monitor(void) {
    endwin();
}

int main(void) {
    init_monitor();
    
    while (running) {
        update_system_info(&state.system);
        update_processes(&state);
        sort_processes(&state);
        redraw_screen();
        
        struct timeval start, end;
        gettimeofday(&start, NULL);
        
        long elapsed_ms = 0;
        while (elapsed_ms < REFRESH_INTERVAL_MS && running) {
            handle_input();
            usleep(50000);
            gettimeofday(&end, NULL);
            elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
        }
    }
    
    cleanup_monitor();
    return 0;
}