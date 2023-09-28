CC = g++

CFLAGS = -g -Wall

STARGET = server
CTARGET = client


server: $(STARGET).cpp
	$(CC) $(CFLAGS) -o $(STARGET) $(STARGET).cpp

client: $(CTARGET).cpp
	$(CC) $(CFLAGS) -o $(CTARGET) $(CTARGET).cpp

clean:
	rm server client

