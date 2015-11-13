#include "all.h"
#include "SndInfo.hpp"
#include "util.hpp"
using namespace std;

#include <sstream>
#include <vector>


void lineTest()



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
