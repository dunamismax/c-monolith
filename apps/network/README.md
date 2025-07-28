# Network Applications

Network programming projects including client-server applications, protocols, and distributed systems.

## Featured Applications

### HTTP Web Server

A production-ready HTTP/1.1 web server with modern security features and performance optimizations:

#### **web_server** - Full-Featured HTTP Server

- **Purpose**: Static file serving with HTTP/1.1 protocol implementation
- **Features**: Multi-threaded request handling, MIME type detection, security headers
- **Architecture**: pthread-based concurrent connection handling with signal management
- **Usage**: `./build/release/bin/web_server <document_root> [port]` (default: 8080)
- **Demo**: Includes beautiful dark-themed landing page showcasing the C-monolith project

#### Key Technical Features

- **HTTP/1.1 Protocol**: Complete implementation with proper headers and status codes
- **Multi-threaded Architecture**: Each request handled in separate pthread for high concurrency
- **Security Hardened**: Path traversal protection, security headers (XSS, frame options, content sniffing)
- **MIME Type Detection**: Automatic content-type assignment for various file formats
- **Graceful Shutdown**: Signal handling for clean server termination
- **Access Logging**: Timestamped request logs with client IP tracking

### Real-Time Chat System

A complete client-server chat application demonstrating perfect socket programming and real-time communication:

#### **chat_server** - Multi-Client Chat Server

- **Purpose**: Real-time chat server supporting up to 32 concurrent clients
- **Features**: Thread-safe client management, message broadcasting, graceful shutdown
- **Architecture**: TCP socket server with pthread-based client handling
- **Usage**: `./build/release/bin/chat_server [port]` (default: 8080)

#### **chat_client** - Interactive Chat Client

- **Purpose**: Real-time chat client with interactive messaging
- **Features**: Username validation, real-time message display, command system
- **Architecture**: TCP client with dedicated receive thread
- **Usage**: `./build/release/bin/chat_client [server_ip] [port]`

#### Key Technical Features

- **Perfect Socket Programming**: Proper TCP socket setup, error handling, and cleanup
- **Thread-Safe Broadcasting**: Mutex-protected client list with real-time message distribution
- **Graceful Shutdown**: Signal handling with self-pipe trick for clean server termination
- **Protocol Design**: Custom message protocol with timestamps and message types
- **Concurrent Connections**: Each client handled in separate pthread with shared state management
- **Input Validation**: Comprehensive username and message validation with bounds checking

## Application Types

This directory contains network programming applications such as:

- **Chat Applications**: Real-time messaging clients and servers ✅ **IMPLEMENTED**
- **HTTP Servers**: Static file servers and web applications ✅ **IMPLEMENTED**
- **File Transfer**: Secure file sharing and transfer protocols
- **Network Utilities**: Network diagnostic and monitoring tools
- **Protocol Implementations**: Custom protocols and communication systems
- **Distributed Systems**: Multi-node applications and coordination systems
- **Proxy Servers**: HTTP/HTTPS proxy implementations
- **Network Scanners**: Port scanners and network discovery tools

## Characteristics

- Socket programming with TCP/UDP protocols
- Client-server architecture patterns
- Concurrent connection handling
- Network security and encryption
- Protocol parsing and implementation
- Error handling for network failures
- Cross-platform network compatibility

## Development Focus

- Robust socket programming with proper error handling
- Thread-safe concurrent connection management
- Network protocol implementation and parsing
- Security considerations (authentication, encryption)
- Performance optimization for high-throughput applications
- Graceful handling of network failures and timeouts
- Clean separation of network and application logic
