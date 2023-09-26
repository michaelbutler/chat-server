import sys
import socket


def client_program():
    HOST, PORT = sys.argv[1], int(sys.argv[2])

    client_socket = socket.socket()
    client_socket.connect((HOST, PORT))

    message = input(" -> ")

    try:
        while message.lower().strip() != 'bye':
            client_socket.send(message.encode())
            data = client_socket.recv(1024).decode()
            print('Received from server: ' + data)
            message = input(" -> ")
    except KeyboardInterrupt:
        print("Ctrl-C caught, exiting.")
    finally:
        client_socket.close()

if __name__ == '__main__':
    client_program()
