# Network Applications

Network programming projects including client-server applications, protocols, and distributed systems.

## Featured Applications

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
- **HTTP Servers**: Basic web servers and API endpoints
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
