AM_CXXFLAGS = -std=c++20
pkglib_LTLIBRARIES = liblocket.la
liblocket_la_SOURCES =              \
	addrinfo_error.cpp          \
	addrinfo_error.hpp          \
	client_stream_socket.cpp    \
	client_stream_socket.hpp    \
	connected_stream_socket.cpp \
	connected_stream_socket.hpp \
	dgram_socket.cpp            \
	dgram_socket.hpp            \
	errno_error.cpp             \
	errno_error.hpp             \
	inet_4_socket_addr.cpp      \
	inet_4_socket_addr.hpp      \
	inet_6_socket_addr.cpp      \
	inet_6_socket_addr.hpp      \
	inet_socket_addr.cpp        \
	inet_socket_addr.hpp        \
	liblocket.hpp               \
	server_stream_socket.cpp    \
	server_stream_socket.hpp    \
	socket_addr.cpp             \
	socket_addr.hpp             \
	socket.cpp                  \
	socket_error.cpp            \
	socket_error.hpp            \
	socket.hpp                  \
	stream_socket.cpp           \
	stream_socket.hpp           \
	unix_socket_addr.cpp        \
	unix_socket_addr.hpp
liblocket_la_CPPFLAGS = -I$(top_srcdir)/deps/bits-and-bytes/include
