# C/C++ Server

*Usage:*

```
# compile the server
make all
```

```
# run the server
./server
```

```
# connect to the server with telnet
telnet localhost 8081
```

If you connect to the server with telnet you can send the raw commands as defined below. You will
also get chat messages sent by other users pushed into your telnet session.


# Protocol

Server TCP protocol is as follows:

* Clients must send messages of no more than 1024 bytes each.
* Messages should be of the format:
  * `A00<message>`
  * Where `A` is the command type, `00` is reserved for future use, and `<message>` is the message
  body which can depend on the command.

## Types of Commands

**Set username:**

```
U00user_name
```

Become `user_name` on the server, only if the name isn't taken by someone else.

**Create a chat room:**

```
C00room_name
```

Replace `room_name` with the desired room name (not case sensitive). Room names must be only letters,
numbers, and underscores, no spaces or other chars.

**Join a chat room:**

```
J00room_name
```

Puts you in that room, but only if it exists. See above for allowed room names.

**Send a message:**

```
M00room_name message
```

Sends `message` to `room_name`. You must have joined that room already otherwise it won't work.

