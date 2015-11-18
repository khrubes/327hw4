#include "all.h"
#include "SndMix.hpp"
#include "SoundProgram.hpp"
#include "util.hpp"
using namespace std;

#include <sstream>
#include <vector>


void lineTest(string fileName = ""){
        string line;
        ifstream file(fileName);
        while ( getline (file.is_open() ? file : cin, line) ){
            cout << line << endl;
        }
}



int main(int argc, const char * argv[]) {
    try{
        vector<string> arguments;
        for (int i = 1; i < argc; i++) { // skip over program name
            arguments.push_back(argv[i]);
        }
        SndMix* sf = new SndMix(arguments);
    }
    catch (exception& e)
    {
        cout << "An error has occured \" " << e.what() << "\"" << endl;
    }
    return 0;
}
