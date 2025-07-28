#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <glob.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64
#define MAX_JOBS 64
#define MAX_HISTORY 1000

typedef struct {
    pid_t pid;
    char command[MAX_CMD_LEN];
    int job_id;
    int stopped;
    int background;
} job_t;

typedef struct {
    char *args[MAX_ARGS];
    int argc;
    char *input_file;
    char *output_file;
    int append_output;
    int background;
} command_t;

static job_t jobs[MAX_JOBS];
static int next_job_id = 1;
static char **command_history;
static int history_count = 0;
static volatile sig_atomic_t interrupted = 0;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        interrupted = 1;
        printf("\n");
    }
}

void setup_signals(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void add_job(pid_t pid, const char *command, int background) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid == 0) {
            jobs[i].pid = pid;
            strncpy(jobs[i].command, command, MAX_CMD_LEN - 1);
            jobs[i].command[MAX_CMD_LEN - 1] = '\0';
            jobs[i].job_id = next_job_id++;
            jobs[i].stopped = 0;
            jobs[i].background = background;
            if (background) {
                printf("[%d] %d %s\n", jobs[i].job_id, pid, command);
            }
            break;
        }
    }
}

void remove_job(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid == pid) {
            jobs[i].pid = 0;
            break;
        }
    }
}

void check_background_jobs(void) {
    int status;
    pid_t pid;
    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < MAX_JOBS; i++) {
            if (jobs[i].pid == pid) {
                if (WIFEXITED(status)) {
                    printf("[%d] Done %s\n", jobs[i].job_id, jobs[i].command);
                } else if (WIFSIGNALED(status)) {
                    printf("[%d] Terminated %s\n", jobs[i].job_id, jobs[i].command);
                }
                remove_job(pid);
                break;
            }
        }
    }
}

char *expand_tilde(const char *path) {
    if (path[0] != '~') {
        return strdup(path);
    }
    
    struct passwd *pw = getpwuid(getuid());
    if (!pw) {
        return strdup(path);
    }
    
    char *expanded = malloc(strlen(pw->pw_dir) + strlen(path) + 1);
    if (!expanded) {
        return strdup(path);
    }
    
    strcpy(expanded, pw->pw_dir);
    strcat(expanded, path + 1);
    return expanded;
}

char **expand_wildcards(char **args, int *argc) {
    char **new_args = malloc(MAX_ARGS * sizeof(char*));
    int new_argc = 0;
    
    for (int i = 0; i < *argc; i++) {
        if (strchr(args[i], '*') || strchr(args[i], '?')) {
            glob_t glob_result;
            int ret = glob(args[i], GLOB_NOCHECK, NULL, &glob_result);
            
            if (ret == 0) {
                for (size_t j = 0; j < glob_result.gl_pathc && new_argc < MAX_ARGS - 1; j++) {
                    new_args[new_argc++] = strdup(glob_result.gl_pathv[j]);
                }
                globfree(&glob_result);
            } else {
                new_args[new_argc++] = strdup(args[i]);
            }
        } else {
            new_args[new_argc++] = strdup(args[i]);
        }
    }
    
    new_args[new_argc] = NULL;
    *argc = new_argc;
    return new_args;
}

void free_args(char **args) {
    for (int i = 0; args[i]; i++) {
        free(args[i]);
    }
    free(args);
}

int parse_command(char *input, command_t *cmd) {
    cmd->argc = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    cmd->background = 0;
    
    char *token = strtok(input, " \t\n");
    while (token && cmd->argc < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd->input_file = expand_tilde(token);
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd->output_file = expand_tilde(token);
                cmd->append_output = 0;
            }
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd->output_file = expand_tilde(token);
                cmd->append_output = 1;
            }
        } else if (strcmp(token, "&") == 0) {
            cmd->background = 1;
        } else {
            char *expanded = expand_tilde(token);
            cmd->args[cmd->argc++] = expanded;
        }
        token = strtok(NULL, " \t\n");
    }
    
    cmd->args[cmd->argc] = NULL;
    
    char **expanded_args = expand_wildcards(cmd->args, &cmd->argc);
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
        cmd->args[i] = expanded_args[i];
    }
    free(expanded_args);
    
    return cmd->argc > 0;
}

int builtin_cd(char **args) {
    char *path = args[1];
    
    if (!path) {
        struct passwd *pw = getpwuid(getuid());
        path = pw ? pw->pw_dir : "/";
    }
    
    char *expanded_path = expand_tilde(path);
    
    if (chdir(expanded_path) != 0) {
        perror("cd");
        free(expanded_path);
        return 1;
    }
    
    setenv("PWD", expanded_path, 1);
    free(expanded_path);
    return 0;
}

int builtin_pwd(char **args) {
    (void)args;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd))) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror("pwd");
        return 1;
    }
}

int builtin_exit(char **args) {
    int exit_code = 0;
    if (args[1]) {
        exit_code = atoi(args[1]);
    }
    exit(exit_code);
}

int builtin_help(char **args) {
    (void)args;
    printf("Unix Shell - Custom C Shell Implementation\n");
    printf("Built-in commands:\n");
    printf("  cd [dir]     - Change directory\n");
    printf("  pwd          - Print working directory\n");
    printf("  exit [code]  - Exit shell\n");
    printf("  help         - Show this help\n");
    printf("  jobs         - List active jobs\n");
    printf("  history      - Show command history\n");
    printf("  env          - Show environment variables\n");
    printf("  export var=val - Set environment variable\n");
    printf("  unset var    - Unset environment variable\n");
    printf("\nFeatures:\n");
    printf("  - Command history (use up/down arrows)\n");
    printf("  - Tab completion\n");
    printf("  - Wildcard expansion (*, ?)\n");
    printf("  - Tilde expansion (~)\n");
    printf("  - I/O redirection (<, >, >>)\n");
    printf("  - Background processes (&)\n");
    printf("  - Job control\n");
    printf("  - Environment variables\n");
    return 0;
}

int builtin_jobs(char **args) {
    (void)args;
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid != 0) {
            printf("[%d] %s %d %s\n", 
                   jobs[i].job_id,
                   jobs[i].stopped ? "Stopped" : "Running",
                   jobs[i].pid,
                   jobs[i].command);
        }
    }
    return 0;
}

int builtin_history(char **args) {
    (void)args;
#ifdef HAVE_READLINE
    int length = history_length;
    for (int i = 0; i < length; i++) {
        HIST_ENTRY *entry = history_get(i + 1);
        if (entry) {
            printf("%4d  %s\n", i + 1, entry->line);
        }
    }
#else
    for (int i = 0; i < history_count; i++) {
        printf("%4d  %s\n", i + 1, command_history[i]);
    }
#endif
    return 0;
}

int builtin_env(char **args) {
    (void)args;
    extern char **environ;
    for (char **env = environ; *env; env++) {
        printf("%s\n", *env);
    }
    return 0;
}

int builtin_export(char **args) {
    if (!args[1]) {
        builtin_env(args);
        return 0;
    }
    
    char *var = args[1];
    char *value = strchr(var, '=');
    
    if (value) {
        *value = '\0';
        value++;
        if (setenv(var, value, 1) != 0) {
            perror("export");
            return 1;
        }
    } else {
        printf("Usage: export VAR=value\n");
        return 1;
    }
    
    return 0;
}

int builtin_unset(char **args) {
    if (!args[1]) {
        printf("Usage: unset VAR\n");
        return 1;
    }
    
    if (unsetenv(args[1]) != 0) {
        perror("unset");
        return 1;
    }
    
    return 0;
}

typedef struct {
    char *name;
    int (*func)(char **);
} builtin_t;

builtin_t builtins[] = {
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"exit", builtin_exit},
    {"help", builtin_help},
    {"jobs", builtin_jobs},
    {"history", builtin_history},
    {"env", builtin_env},
    {"export", builtin_export},
    {"unset", builtin_unset},
    {NULL, NULL}
};

int execute_builtin(char **args) {
    for (int i = 0; builtins[i].name; i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            return builtins[i].func(args);
        }
    }
    return -1;
}

int setup_redirection(command_t *cmd) {
    if (cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);
        if (fd == -1) {
            perror(cmd->input_file);
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    
    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append_output ? O_APPEND : O_TRUNC;
        int fd = open(cmd->output_file, flags, 0644);
        if (fd == -1) {
            perror(cmd->output_file);
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    
    return 0;
}

int execute_command(command_t *cmd) {
    if (cmd->argc == 0) {
        return 0;
    }
    
    int builtin_result = execute_builtin(cmd->args);
    if (builtin_result != -1) {
        return builtin_result;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        if (setup_redirection(cmd) == -1) {
            exit(1);
        }
        
        execvp(cmd->args[0], cmd->args);
        perror(cmd->args[0]);
        exit(127);
    } else if (pid > 0) {
        char command_str[MAX_CMD_LEN] = "";
        for (int i = 0; i < cmd->argc; i++) {
            strcat(command_str, cmd->args[i]);
            if (i < cmd->argc - 1) strcat(command_str, " ");
        }
        
        if (cmd->background) {
            add_job(pid, command_str, 1);
        } else {
            add_job(pid, command_str, 0);
            int status;
            waitpid(pid, &status, 0);
            remove_job(pid);
            return WEXITSTATUS(status);
        }
    } else {
        perror("fork");
        return 1;
    }
    
    return 0;
}

void execute_pipeline(char *input) {
    char *commands[MAX_ARGS];
    int num_commands = 0;
    
    char *cmd = strtok(input, "|");
    while (cmd && num_commands < MAX_ARGS - 1) {
        while (*cmd == ' ') cmd++;
        commands[num_commands++] = cmd;
        cmd = strtok(NULL, "|");
    }
    
    if (num_commands == 1) {
        command_t parsed_cmd;
        if (parse_command(commands[0], &parsed_cmd)) {
            execute_command(&parsed_cmd);
        }
        return;
    }
    
    int pipes[num_commands - 1][2];
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return;
        }
    }
    
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            command_t parsed_cmd;
            if (parse_command(commands[i], &parsed_cmd)) {
                if (setup_redirection(&parsed_cmd) == -1) {
                    exit(1);
                }
                execvp(parsed_cmd.args[0], parsed_cmd.args);
                perror(parsed_cmd.args[0]);
            }
            exit(127);
        } else if (pid == -1) {
            perror("fork");
        }
    }
    
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

#ifdef HAVE_READLINE
char *command_generator(const char *text, int state) {
    static char *commands[] = {
        "cd", "pwd", "exit", "help", "jobs", "history", "env", "export", "unset",
        "ls", "cat", "grep", "find", "ps", "top", "kill", "echo", "which", "man",
        NULL
    };
    static int list_index, len;
    
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }
    
    while (commands[list_index]) {
        char *name = commands[list_index];
        list_index++;
        
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }
    
    return NULL;
}

char **command_completion(const char *text, int start, int end) {
    (void)end;
    rl_attempted_completion_over = 1;
    
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }
    
    return NULL;
}
#endif

void add_to_history(const char *line) {
#ifdef HAVE_READLINE
    add_history(line);
#else
    if (history_count < MAX_HISTORY) {
        command_history[history_count] = strdup(line);
        history_count++;
    } else {
        free(command_history[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            command_history[i] = command_history[i + 1];
        }
        command_history[MAX_HISTORY - 1] = strdup(line);
    }
#endif
}

void init_shell(void) {
    setup_signals();
    
#ifdef HAVE_READLINE
    rl_attempted_completion_function = command_completion;
    rl_bind_key('\t', rl_complete);
    
    using_history();
    stifle_history(MAX_HISTORY);
    
    char *home = getenv("HOME");
    if (home) {
        char history_file[1024];
        snprintf(history_file, sizeof(history_file), "%s/.unix_shell_history", home);
        read_history(history_file);
    }
#else
    command_history = malloc(MAX_HISTORY * sizeof(char*));
    for (int i = 0; i < MAX_HISTORY; i++) {
        command_history[i] = NULL;
    }
#endif
}

void cleanup_shell(void) {
#ifdef HAVE_READLINE
    char *home = getenv("HOME");
    if (home) {
        char history_file[1024];
        snprintf(history_file, sizeof(history_file), "%s/.unix_shell_history", home);
        write_history(history_file);
    }
    
    clear_history();
#else
    for (int i = 0; i < history_count; i++) {
        free(command_history[i]);
    }
    free(command_history);
#endif
}

void print_prompt(void) {
    char cwd[1024];
    char *home = getenv("HOME");
    char *display_path;
    
    if (getcwd(cwd, sizeof(cwd))) {
        if (home && strncmp(cwd, home, strlen(home)) == 0) {
            display_path = malloc(strlen(cwd) - strlen(home) + 2);
            sprintf(display_path, "~%s", cwd + strlen(home));
        } else {
            display_path = strdup(cwd);
        }
    } else {
        display_path = strdup("unknown");
    }
    
    char *user = getenv("USER");
    if (!user) user = "user";
    
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "localhost");
    }
    
    printf("\033[32m%s@%s\033[0m:\033[34m%s\033[0m$ ", user, hostname, display_path);
    free(display_path);
    fflush(stdout);
}

#ifndef HAVE_READLINE
char *simple_readline(const char *prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    
    char *line = malloc(MAX_CMD_LEN);
    if (!line) return NULL;
    
    if (fgets(line, MAX_CMD_LEN, stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        return line;
    }
    
    free(line);
    return NULL;
}
#endif

int main(void) {
    init_shell();
    
    printf("Unix Shell v1.0 - Custom C Shell Implementation\n");
#ifdef HAVE_READLINE
    printf("Features: readline support, tab completion, command history\n");
#else
    printf("Features: basic command history, job control, piping\n");
#endif
    printf("Type 'help' for available commands or 'exit' to quit.\n\n");
    
    print_prompt();
    
    char *input;
#ifdef HAVE_READLINE
    while ((input = readline("")) != NULL) {
#else
    while ((input = simple_readline("")) != NULL) {
#endif
        if (interrupted) {
            interrupted = 0;
            free(input);
            print_prompt();
            continue;
        }
        
        check_background_jobs();
        
        if (strlen(input) > 0) {
            add_to_history(input);
            
            char *input_copy = strdup(input);
            execute_pipeline(input_copy);
            free(input_copy);
        }
        
        free(input);
        print_prompt();
    }
    
    printf("\nGoodbye!\n");
    cleanup_shell();
    return 0;
}