#include <iostream>
#include <string>
#include "defines.h"
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

// return TRUE (0) if valid or FALSE (0) if not
int is_valid_message(string msg)
{
    char* bytes;
    string work_buffer;

    if (msg.length() < 3) {
        debuglog("Message is under 3 chars long");
        return FALSE;
    }

    bytes = msg.data();

    if (
        bytes[0] != 'J' &&
        bytes[0] != 'C' &&
        bytes[0] != 'L' &&
        bytes[0] != 'U' &&
        bytes[0] != 'M' &&
        bytes[0] != 'Q' &&
        bytes[0] != 'V'
    ) {
        debuglog("Message does not begin with proper char");
        return FALSE;
    }

    // for now just require two zeroes.
    if (bytes[1] != '0' || bytes[2] != '0') {
        debuglog("Message does not have 00 after first char");
        return FALSE;
    }

    // Validate Join Command
    if (bytes[0] == 'J') {
        work_buffer = msg.substr(3);
        if (!is_valid_entity_name(work_buffer)) {
            debuglog("Invalid entity name provided.");
            return FALSE;
        }
    }

    debuglog("Message is valid. Proceeding.");

    return TRUE;
}
