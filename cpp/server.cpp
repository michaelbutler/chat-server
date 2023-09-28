/*
* Author's Note:
* Written with help from:
* https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
* 
* This code might indeed not have any C++ specific features and might be C compatible.
*/

//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h> //strlen
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <vector>
#include <map>
#include <string>
#include "util.h"
#include "defines.h"

using namespace std;

/* GLOBAL VARIABLES -- ooof */

// Map of room name => users list (string names CSV)
map<string, string> rooms;

// Map of user names to their socket index number!
map<string, int> users;


class RawMessage {
    public:
        int sender_num; // The socket number who sent this msg
        string msg;
    RawMessage() {}
    RawMessage(int sender_num, string msg)
    {
        this->sender_num = sender_num;
        this->msg = msg;
    }
};

/* Method Definitions */
int handle_message(RawMessage rm, int* csock);


// chat server
int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[MAX_CLIENTS],
    max_clients = MAX_CLIENTS, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;

    // strings of raw text sent to the server by clients
    vector<RawMessage> chat_strings;

    string sbuffer;
    int num_chat_strings = 0;
    char buffer[MAX_RAW_MSG_SIZE]; //data buffer of 1K

    //set of socket descriptors
    fd_set readfds;

    //a message
    const char *welcome_message = "Welcome to the Chat Server!\r\n";

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", 
                   new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            //send new connection greeting message
            if( send(new_socket, welcome_message, strlen(welcome_message), 0) != strlen(welcome_message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        // loop through received messages and add to vector
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buffer, MAX_MSG_SIZE)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
                                (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }

                // Store message in buffer list
                else
                {
                    //set the string terminating NULL byte on the end
                    //of the data read
                    buffer[valread] = '\0';
                    printf("Storing message (%d) in vector\n", (int)strlen(buffer));
                    // +1 to strlen to include the NULL terminator byte
                    sbuffer.assign(buffer, strlen(buffer));
                    clean_sent_string(sbuffer);
                    chat_strings.push_back(RawMessage(i, sbuffer));
                    // memcpy(chat_strings[num_chat_strings], buffer, strlen(buffer) + 1);
                    num_chat_strings++;
                    printf("Stored message (%d) in vector\n", (int)strlen(buffer));
                }
            }
        }

        // handle messages
        for (i = 0; i < num_chat_strings; i++) {
            handle_message(chat_strings[i], client_socket);
        }

        num_chat_strings = 0;
        chat_strings.clear();
    }

    return 0;
}

// csock: array of socket numbers
int handle_message(RawMessage rm, int* csock)
{
    int sd, addrlen;
    char command_type;
    const char* error_msg = "ERROR: Invalid message format.\r\n";
    const char* goodbye_msg = "Goodbye.\r\n";
    char* raw_bytes;
    struct sockaddr_in address;

    addrlen = sizeof(address);

    sd = csock[rm.sender_num];

    if (FALSE == is_valid_message(rm.msg)) {
        printf("Received message is NOT valid. Skipping...\n");
        send(
            sd,
            error_msg,
            strlen(error_msg),
            0);

        return 1;
    }

    raw_bytes = rm.msg.data();
    command_type = raw_bytes[0];

    if (command_type == 'Q') {
        send(
            sd,
            goodbye_msg,
            strlen(goodbye_msg),
            0);
        // Client requested to quit.
        // get his details and print
        getpeername(sd , (struct sockaddr*)&address , \
                    (socklen_t*)&addrlen);
        printf("Host disconnected , ip %s , port %d \n" ,
               inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        //Close the socket and mark as 0 in list for reuse
        close( sd );
        csock[rm.sender_num] = 0;
        return 0;
    }

    // Create Room
    if (command_type == 'C') {
        string room_name = rm.msg.substr(3);
        transform(room_name.begin(), room_name.end(), room_name.begin(), ::tolower);
        cout << "Creating new room: " << room_name << "\n";
        rooms.insert(pair<string, string>(room_name, ""));
    }

    if (command_type == 'U') {
        string username = rm.msg.substr(3);
        // TODO: move to helper function (tolowercase)
        transform(username.begin(), username.end(), username.begin(), ::tolower);
        cout << "Becoming username: " << username << "\n";
        users.insert(pair<string, int>(username, rm.sender_num)); 
    }

    if (command_type == 'M') {
        // send any buffered messages to all connected clients
        // But skip repeating it to the client who SENT the message.
        for (int x = 0; x < MAX_CLIENTS; x++)
        {
            sd = csock[x];
            if (sd != 0 && x != rm.sender_num)
            {
                printf("Sending data to client num. %d\n", x);
                // sends strlen() number of bytes, which does NOT include \0 at the end
                send(
                    sd,
                    rm.msg.data(),
                    strlen(rm.msg.data()),
                    0);
            }
        }
    }

    return 0;
}

