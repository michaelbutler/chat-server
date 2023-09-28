// util.h

#include <string>

using namespace std;

int is_valid_message(string msg);

int is_valid_entity_name(string name);

void debuglog(string log);

void clean_sent_string(string& s);

extern const char* wschars;

string& rtrim(string& s, const char* t = wschars);
string& ltrim(string& s, const char* t = wschars);
string& trim(string& s, const char* t = wschars);
