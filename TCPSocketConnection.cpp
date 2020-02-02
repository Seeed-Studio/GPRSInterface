/*
    TCPSocketConnection.cpp

    Copyright (c) 2014 seeed technology inc.
    Website    : www.seeed.cc
    Author     : lawliet zou
    Create Time: Feb 2014
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "TCPSocketConnection.h"

TCPSocketConnection::TCPSocketConnection() {
}

int TCPSocketConnection::connect(const char* host, const int port) {
    if (_sock_fd < 0) {
        _sock_fd = gprs->new_socket();
        if (_sock_fd < 0) {
            return -1;
        }
    }

    if (!gprs->connect(_sock_fd, TCP, host, port)) {
        return false;
    }
    return true;
}

bool TCPSocketConnection::is_connected(void) {
    return gprs->is_connected(_sock_fd);
}

int TCPSocketConnection::send(char* data, int length) {
    int size = gprs->wait_writeable(_sock_fd, length);
    if (size < 0) {
        return -1;
    }
    if (size > length) {
        size = length;
    }
    return gprs->send(_sock_fd, data, size);
}

int TCPSocketConnection::send_all(char* data, int length) {
    return send(data, length);
}

int TCPSocketConnection::receive(char* data, int length) {
    int size = gprs->wait_readable(_sock_fd, DEFAULT_TIMEOUT);
    return gprs->recv(_sock_fd, data, size > length ? length : size);
}

int TCPSocketConnection::receive_all(char* data, int length) {
    return receive(data, length);
}
