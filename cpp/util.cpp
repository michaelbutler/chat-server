#include <iostream>
#include <string>
#include <system_error>
#include "defines.h"
#include "util.h"

using namespace std;

const char* wschars = " \t\n\r\f\v";

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
            printf("byte %d is valid\n", (int)bytes[i]);
            ; // it's valid
        } else {
            printf("byte %d is not valid\n", (int)bytes[i]);
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

    // Validate Create & Join Command
    if (bytes[0] == 'J' || bytes[0] == 'C' || bytes[0] == 'U') {
        work_buffer = msg.substr(3);
        if (!is_valid_entity_name(work_buffer)) {
            debuglog("Invalid entity name provided.");
            return FALSE;
        }
    }

    debuglog("Message is valid. Proceeding.");

    return TRUE;
}

// trims the input string (in place, by reference)
void clean_sent_string(string& s)
{
    trim(s);
}

// trim from end of string (right)
string& rtrim(string& s, const char* t)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
string& ltrim(string& s, const char* t)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
string& trim(string& s, const char* t)
{
    return ltrim(rtrim(s, t), t);
}

