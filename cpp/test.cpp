
#include <iostream>
#include <string>
#include <vector>

#define TRUE 1
#define FALSE 0

using namespace std;

class RawMessage
{
    public:
        int sender_num; // The socket number who sent this msg
        string msg;
    
    RawMessage()
    {
    }

    RawMessage(int sender_num, string msg)
    {
        this->sender_num = sender_num;
        this->msg = msg;
    }
};

// chat server
int main(int argc , char *argv[])
{
    int x;
    vector<RawMessage> vect;

    for (x = 0; x < 10; x++) {
        vect.push_back(RawMessage(x, "Hello wrold..."));
    }

    for (x = 0; x < 10; x++) {
        cout << "Message: " << vect[x].sender_num << " " << vect[x].msg << endl;
    }

    return 0;
}

