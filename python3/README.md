# Python3 Chat-Server

Usage:

## Server

```
python3 server.py 127.0.0.1 8081
```

Where `8081` is the port to listen on.

## Client

```
python3 client.py 127.0.0.1 8081
```

To connect to server 127.0.0.1 on port 8081. Then type a string of text and press enter; it will
be sent to the server. Then it will wait for the server to send a string of text back.

## TODO:

- Make the server async and non-blocking
- Allow clients to choose a username and join rooms to send messages
- All clients connected to a room should receive new messages sent to that room.
