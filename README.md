# liblocket

## About

Locket (Linux sOCKET) is a C++ wrapper library for the Linux sockets API.

## Installation

Acquire the sources
```
$ git clone https://github.com/jasper1378/liblocket.git
$ cd liblocket
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

The Makefile will install both static and shared versions of the library, use whichever you prefer. Most users will just want to include `liblocket.hpp`, which itself includes all the other headers. All code in the library is found within the `liblocket` namespace. Function-level detail is not provided in the documentation for two reasons. a) Socket member functions are designed to behave similarly to the underlying API functions with which they share their names. b) I'm lazy.

I recommended having a basic understanding of the Linux sockets API before making use of this library.

### Sockets

Socket classes wraps a system socket handle and maintains its lifetime. When the object goes out of scope, it closes the underlying socket handle. Socket objects are moveable but not copyable. Socket classes also abstracts socket operations such as reading, writing, and connecting. 

The base socket class is represented by the `liblocket::socket` class. It is not intended to be used by itself, rather it serves as an abstract base class for more specific socket types. All sockets (except for `liblocket::connected_stream_socket`) may be bound to a specific address.

Datagram sockets are represented by the `liblocket::dgram_socket` class. Datagram sockets can perform IO in both connected and disconnected states.

A basic stream socket is represented by the `liblocket::stream_socket` class. The base stream socket class shouldn't be used by itself, rather it serves as a base class for more specialized types of stream sockets.

Client stream sockets are represented by the `liblocket::client_stream_socket_class`. They must be connected before IO can be performed.

Server stream sockets are represented by the `liblocket::server_stream_socket` class. Server stream sockets must be bound and listening before they can accept connections. IO is not performed directly through a server stream socket; rather a connected stream socket will be returned after accepting a new connection.

Connected stream sockets are represented by the `liblocket::connected_stream_socket_class`. They represent a connection created by a server stream socket between itself and a client. Connected stream sockets should only be created by a server stream socket, not as a standalone socket.

### Socket Addresses

Socket address classes encapsulates the raw address structures used by the underlying API and makes them easier to work with. They also serve the purpose of separating the type of socket (datagram, stream, etc.) from the domain (unix, inet, etc.). Many socket class constructors and functions take address classes as arguments. Socket address classes also contain functions to simplify the copying of address objects through a polymorphic pointer when the actual type is unknown.

The base address class is represented by the `liblocket::socket_addr` class. It is not intended to be used by itself, rather it serves as an abstract base class for more specific socket types. It also contains an enumeration with values corresponding to the various socket domains (unix, inet4, inet6).

Unix domain socket addresses are represented by the `liblocket::unix_socket_addr` class. This address contains the path of the socket file. It also provides support for abstract unix domain sockets, in which case it contains the name of the socket rather than the path.

Internet domain socket addresses are represented by the `liblocket::inet_socket_addr` class. This class isn't intended to be used by itself, rather it serves as an abstract base class for more specialized internet domain addresses. It contains enumerations for keeping track of IP versions (IPv4 and IPv6) as well as byte order (which most people shouldn't have to worry about).

Internet domain IPv4 and IPv6 socket addresses are represented by the `liblocket::inet4_socket_addr` and `liblocket::inet6_socket_addr` classes respectively. They behave similarly but differ in their internal implementation due to how the underlying API works. These addresses contain ip addresses and port numbers. Their constructors accept both numeric ip addresses as well as textual hostnames.

### Error Handling

Most functions will throw `std::runtime_error`, `std::invalid_argument`, or another common standard library exception class in case of error. 

Errors resulting from functions that set `errno` (ex. `write()`) will throw `liblocket::errno_error`, an exception class deriving from `std::runtime_error` who's `what_arg` contains the value of `strerror()`.

Errors resulting from socket-specific functions that set `errno` (ex. `socket()`) will throw `liblocket::socket_error`, which behaves similarily to `liblocket::errno_error`. 

Errors resulting from `getaddrinfo()` and `getnameinfo()` will throw `liblocket::addrinfo_error`, which behaves similarily to `liblocket::socket_error` but makes use of `gai_strerror()` instead of `strerror()`.

All types of sockets (datagram, stream, etc.) inherit from a common `socket` base class. Most take socket addresses as arguments. All types of socket addresses (IPv4, IPv6, Unix) inherit from a common `socket_addr` base class. Sockets can be moved, but not copied. The libraries implements RAII (resource acquisition is initialization), which is to say that sockets will automatically clean up after themselves when they go out of scope. The library uses exceptions for error handling. Errors resulting from functions that usually set `errno` will throw something that inherits from `errno_error`, an exception class inheriting from `std::runtime_error` that provides access to the `errno` value. Everything else just throws `std::runtime_error`.

## License

See [LICENSE](LICENSE) for details.
