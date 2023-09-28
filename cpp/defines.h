
#define TRUE 1
#define FALSE 0

#define PORT 8081
#define DEBUG_MODE 1

// max size of a raw message, in bytes, not including \0 termination at the end
// Note: clients are not allowed to send \0 character and server will reject such a msg.
#define MAX_MSG_SIZE 1024
#define MAX_RAW_MSG_SIZE 1025

// max allowed clients connected to server at once
#define MAX_CLIENTS 30

#define SENTINEL_JOIN      'J'
#define SENTINEL_CREATE    'C'
#define SENTINEL_LIST      'L'
#define SENTINEL_USERNAME  'U'
#define SENTINEL_MESSAGE   'M'
#define SENTINEL_QUIT      'Q'
#define SENTINEL_LEAVE     'V'

