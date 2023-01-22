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

All types of sockets (datagram, stream, etc.) inherit from a common `socket` base class. Most take socket addresses as arguments. All types of socket addresses (IPv4, IPv6, Unix) inherit from a common `socket_addr` base class. Sockets can be moved, but not copied. The libraries implements RAII (resource acquisition is initialization), which is to say that sockets will automatically clean up after themselves when they go out of scope. The library uses exceptions for error handling. Errors resulting from functions that usually set `errno` will throw something that inherits from `errno_error`, an exception class inheriting from `std::runtime_error` that provides access to the `errno` value. Everything else just throws `std::runtime_error`.

Proper documentation will be coming the future. If you have a question in the meantime either: a) open an issue on GitHub (I'll try to respond as soon as a I can) or b) just look at the code (it's fairly straightforward and simple).

If the library is lacking functionality that you would find useful, feel free to open an issue on GitHub, there's a good chance that I'll implement it. If need be, the `socket` class contains a function that returns the underlying file descriptor for the open socket, which can be passed to the raw Linux sockets API.

## License
See [LICENSE.md](LICENSE.md) for details.
