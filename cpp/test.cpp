
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0

// chat server
int main(int argc , char *argv[])
{
    int master_socket, addrlen, new_socket, client_socket[30];
    // create array of 100 chat strings, max length 1025 each
    char chat_strings[100][1025];
    int num_chat_strings = 0;
    char buffer[1025]; //data buffer of 1K
    char empty_str[] = "-nil-";
    int i;

    //a message
    const char *welcome_message = "Welcome to the Chat Server!\r\n";

    // initialize chat strings buffer
    for (i = 0; i < 100; i++) {
        memcpy(chat_strings[i], empty_str, strlen(empty_str) + 1);
    }

    for (i = 0; i < 100; i++) {
        printf("%d %u, %s\n", i, (unsigned int)strlen(chat_strings[i]), chat_strings[i]);
    }

    return 0;
}
