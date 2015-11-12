#include "all.h"
#include "SndInfo.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    try{
        //TODO print error messages to standard error rather than stdout

        vector<string> arguments;
        for (int i = 1; i < argc; i++) { // skip over program name
            arguments.push_back(argv[i]);
        }
        SndInfo* sf = new SndInfo(arguments);
    }
    catch (exception& e)
    {
        cout << "An error has occured \" " << e.what() << "\"" << endl;
    }
    return 0;
}
