#! /usr/bin/env python3

import socket

HOST = '127.0.0.1' # Use loopback interface
PORT = 12345

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as socket_:
    socket_.bind((HOST, PORT))
    socket_.listen()
    connection_, address_ = socket_.accept()
    with connection_:
        print("Connected by: ", address_)
        while True:
            data = connection_.recv(1024)
            if not data:
                break
            print("Received '" + repr(data) + "' from client")
            connection_.sendall(data)
