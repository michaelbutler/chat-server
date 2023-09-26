import sys
import socket


def server_program():
    HOST, PORT = sys.argv[1], int(sys.argv[2])
    server_socket = socket.socket()
    server_socket.bind((HOST, PORT))
    server_socket.listen(2)
    conn, address = server_socket.accept()
    print("Connection from: " + str(address))
    try:
        while True:
            data = conn.recv(1024).decode()
            if not data:
                break
            print("From connected user: " + str(data))
            data = input(' -> ')
            conn.send(data.encode())
    except KeyboardInterrupt:
        print("Ctrl-C caught, exiting.")
    finally:
        conn.close()


if __name__ == '__main__':
    server_program()

