# System Programming Applications

Low-level system interaction projects including shells, system utilities, and OS interaction tools.

## Featured Applications

### Unix Shell Implementation

A complete custom shell demonstrating advanced system programming and process management:

#### **unix_shell** - Custom Unix Shell

- **Purpose**: Full-featured command-line interpreter with modern shell capabilities
- **Features**: Job control, I/O redirection, piping, command history, tab completion
- **Architecture**: Fork/exec process management with signal handling and background jobs
- **Usage**: `./build/release/bin/unix_shell` or `make run-unix_shell`

#### Key Technical Features

- **Process Management**: Complete fork/exec implementation with proper child process handling
- **I/O Redirection**: Full support for input (<), output (>), and append (>>) redirection
- **Pipeline Support**: Multi-command pipelines with proper file descriptor management
- **Job Control**: Background job tracking, signal handling, and process monitoring
- **Built-in Commands**: cd, pwd, exit, help, jobs, history, env, export, unset
- **Advanced Features**: Tilde expansion (~), wildcard globbing (\*, ?), command history
- **Cross-Platform**: Works on both Linux and macOS with optional readline support

### Process Monitor - Real-time System Monitoring

A comprehensive htop-like system monitor with advanced process management capabilities:

#### **process_monitor** - System Resource Monitor

- **Purpose**: Real-time system and process monitoring with interactive terminal interface
- **Features**: Process tree view, CPU/memory monitoring, process killing, filtering and sorting
- **Architecture**: ncurses-based UI with /proc filesystem parsing for real-time data collection
- **Usage**: `./build/release/bin/process_monitor` or `make run-process_monitor`

#### Key Technical Features

- **Real-time Data Collection**: Direct /proc filesystem parsing for process and system information
- **Interactive ncurses Interface**: Full-screen terminal UI with keyboard navigation and controls
- **Multiple View Modes**: Process list, hierarchical tree view, and system information display
- **Process Management**: Signal-based process termination with proper error handling
- **Resource Monitoring**: Live CPU percentage, memory usage, and system statistics
- **Advanced Sorting**: Sort by PID, CPU usage, memory usage, time, or command name
- **Process Filtering**: Filter processes by name or PID with real-time search
- **System Information**: Memory usage, load averages, uptime, and process counts

## Application Types

This directory contains system programming applications such as:

- **Custom Shells**: Command-line interpreters and shell implementations ✅ **IMPLEMENTED**
- **System Monitors**: Process monitoring and system resource tracking ✅ **IMPLEMENTED**
- **File Encryption**: Secure file encryption and decryption tools
- **Memory Managers**: Custom memory allocation and management systems
- **Process Controllers**: Process spawning, management, and IPC tools
- **File System Tools**: Custom file system operations and utilities
- **Device Drivers**: User-space device interaction programs
- **System Daemons**: Background services and system processes

## Characteristics

- Direct operating system API usage
- Process management and inter-process communication
- File system manipulation and monitoring
- Memory management and optimization
- Signal handling and system events
- Platform-specific system calls
- Low-level hardware interaction

## Development Focus

- Understanding of operating system internals
- Safe system call usage with proper error handling
- Process lifecycle management (fork, exec, wait)
- Signal handling and process communication
- File descriptor management and I/O multiplexing
- Memory safety in low-level operations
- Platform abstraction for cross-OS compatibility
- Security considerations for privileged operations
