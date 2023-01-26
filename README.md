# locket

## About
Locket (Linux sOCKET) is a C++ wrapper library for the Linux sockets API.

## Installation
Acquire the sources
```
$ git clone https://github.com/jasper1378/locket.git
$ cd locket
```
Build
```
$ make
```
Install
```
$ make install
```

## Usage

The Makefile will install both static and shared versions of the library, use whichever you prefer. Most users will just want to include `locket.hpp`, which itself includes all the other headers. All code in the library is found within the `locket` namespace.

### Sockets

I suggest familiarizing yourself with the underlying Linux sockets API before making use of this library.

Socket classes wraps a system socket handle and maintains its lifetime. When the object goes out of scope, it closes the underlying socket handle. Socket objects are moveable but not copyable. Socket classes also abstracts socket operations such as reading, writing, and connecting. The `locket::socket` class is not intended to be used by itself, rather it serves as an abstract base class for more specific socket types.

All sockets (except for `locket::connected_stream_socket`) may be bound to a specific address.

Datagram sockets are representend by the `locket::dgram_socket` class. Datagram sockets can perform IO in both connected and disconnected states.

A basic stream socket is represented by the `locket::stream_socket` class. The base stream socket class shouldn't be used by itself, rather it serves as a base class for more specialized types of stream sockets.

Client stream sockets are represented by the `locket::client_stream_socket_class`. They must be connected before IO can be performed.

Server stream sockets are represented by the `locket::server_stream_socket` class. Server stream sockets must be bound and listening before they can accept connections. IO is not performed directly through a server stream socket; rather a connected stream socket will be returned after accepting a new connection.

Connected stream sockets are represented by the `locket::connected_stream_socket_class`. They represent a connection created by a server stream socket between itself and a client. Connected stream sockets should only be created by a server stream socket, not as a standalone socket. Connected stream sockets TODO continue from here

### Error Handling

Most functions will throw `std::runtime_error`, `std::invalid_argument`, or another common standard library exception class in case of error. 

Errors resulting from functions that set `errno` (ex. `write()`) will throw `locket::errno_error`, an exception class deriving from `std::runtime_error` who's `what_arg` contains the value of `strerror()`.

Errors resulting from socket-specific functions that set `errno` (ex. `socket()`) will throw `locket::socket_error`, which behaves similarily to `locket::errno_error`. 

Errors resulting from `getaddrinfo()` and `getnameinfo()` will throw `locket::addrinfo_error`, which behaves similarily to `locket::socket_error` but makes use of `gai_strerror()` instead of `strerror()`.

All types of sockets (datagram, stream, etc.) inherit from a common `socket` base class. Most take socket addresses as arguments. All types of socket addresses (IPv4, IPv6, Unix) inherit from a common `socket_addr` base class. Sockets can be moved, but not copied. The libraries implements RAII (resource acquisition is initialization), which is to say that sockets will automatically clean up after themselves when they go out of scope. The library uses exceptions for error handling. Errors resulting from functions that usually set `errno` will throw something that inherits from `errno_error`, an exception class inheriting from `std::runtime_error` that provides access to the `errno` value. Everything else just throws `std::runtime_error`.

## License
See [LICENSE.md](LICENSE.md) for details.
