#include <string>
#include "util.h"

using namespace std;


// accept A-Z a-z 0-9 and _ for usernames and room names.
int is_valid_entity_name(string name)
{
    char* bytes;

    bytes = name.data();

    for (int i = 0, max = name.length(); i  < max; i++) {
        if (
            (bytes[i] >= 'A' && bytes[i] <= 'Z') ||
            (bytes[i] >= 'a' && bytes[i] <= 'z') ||
            (bytes[i] >= '0' && bytes[i] <= '9') ||
                bytes[i] == '_'
        ) {
            ; // it's valid
        } else {
            return FALSE;
        }
    }

    return TRUE;
}


void debuglog(string log)
{
    if (DEBUG_MODE <= 0) {
        return;
    }
    cout << log << endl;
}
