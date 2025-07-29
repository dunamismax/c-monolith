# Process Monitor

An htop-inspired system monitoring application with real-time process tracking, written in pure C with ARM64 optimizations.

## Features

- **Real-time Process Tracking**: Live display of all running processes with automatic refresh
- **Process Tree View**: Hierarchical display showing parent-child relationships  
- **System Statistics**: CPU usage, memory consumption, load averages, and uptime
- **Interactive ncurses Interface**: Full-screen terminal UI with keyboard navigation
- **Process Management**: Kill processes with signal handling (SIGTERM)
- **Advanced Sorting**: Sort by PID, CPU usage, memory usage, time, or command name
- **Process Filtering**: Filter processes by name or PID with real-time search
- **Multiple View Modes**: Process list, process tree, and system information views

## Quick Start

```bash
# Build and run
make
make run-process_monitor

# Or build specifically  
make build/release/bin/process_monitor
./build/release/bin/process_monitor
```

### Dependencies
- **ncurses**: Terminal user interface library
- **Linux /proc filesystem**: System and process information source

## Controls

### Navigation
- **↑/↓ Arrow Keys**: Navigate through process list
- **Space**: Pause/Resume automatic refresh
- **r/R**: Manual refresh

### View Modes
- **1**: Process list view (default)
- **2**: Process tree view
- **3**: System information view
- **t/T**: Toggle between list and tree view

### Sorting
- **s/S**: Cycle through sorting modes:
  - PID (Process ID)
  - CPU usage (default)
  - Memory usage
  - Time
  - Command name

### Process Management
- **k/K**: Kill selected process (sends SIGTERM)
- **f/F**: Filter processes by name or PID

### Other
- **q/Q**: Quit application
- **ESC**: Alternative quit

## Interface Layout

```
┌─ Process Monitor v1.0 ──────────────────────────── 14:23:45 ─┐
│                                                              │
│ System Information:                                          │
│   Uptime: 2 days, 14:23:45                                  │
│   Load Average: 1.23 0.98 0.67                              │
│   CPU Usage: 12.5%                                          │
│   Memory: 16384 MB total, 8192 MB free, 10240 MB available  │
│   Processes: 247                                            │
│                                                              │
│   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM   │
│  1234 user       20   0   45678   1234      0 S   2.5  1.2   │
│  5678 user       20   0   98765   4321      0 R  15.3  3.4   │
│                                                              │
├─ Sort: CPU | View: List | Filter: '' | Processes: 247 ──────┤
└──────────────────────────────────────────────────────────────┘
```

## Technical Implementation

### Architecture
- **Data Source**: Linux `/proc` filesystem (`/proc/[pid]/stat`, `/proc/meminfo`, `/proc/loadavg`)
- **UI Framework**: ncurses for full-screen terminal interface
- **Real-time Updates**: CPU percentage calculations based on time differences
- **Memory Management**: Fixed-size arrays with bounds checking

### Performance Features
- **ARM64 Optimized**: Built with Apple Silicon-specific compiler flags
- **Efficient Parsing**: Direct /proc filesystem access without external dependencies
- **Minimal Allocation**: Fixed-size process storage for consistent performance
- **Smart Refresh**: Configurable update intervals (default 1000ms)

## Security & Quality

### Memory Safety
- **Bounds Checking**: All array accesses are bounds-checked with proper validation
- **String Safety**: Safe string operations with null termination guarantees
- **Resource Management**: Proper file handle cleanup and memory management

### Security Features
- **Input Validation**: All user input is sanitized and validated
- **Signal Safety**: Only sends SIGTERM for safe process termination
- **Permission Handling**: Graceful handling of inaccessible process information
- **Buffer Protection**: Stack canaries and overflow detection (debug mode)

## Learning Value

This implementation demonstrates:
- **System Programming**: Direct /proc filesystem interaction and system calls
- **Terminal UI Development**: Professional ncurses interface with real-time updates  
- **Process Management**: Understanding process hierarchies and signal handling
- **Performance Optimization**: Efficient real-time data collection and display
- **Memory-Safe C**: Modern C programming practices with comprehensive error handling

## Comparison to htop

**Similarities**: Real-time monitoring, tree view, process killing, sorting/filtering  
**Differences**: Simplified educational focus, minimal dependencies, ARM64 optimization

This process monitor serves as both a practical system tool and an educational example of system programming in C, demonstrating how to build efficient, interactive terminal applications that interact directly with the operating system.