#! /usr/bin/env python3

import socket

HOST = '127.0.0.1' # Use loopback interface
PORT = 12345

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as socket_:
    socket_.connect((HOST, PORT))
    socket_.sendall(b'Hello world!')
    data = socket_.recv(1024)

print('Received', repr(data))
