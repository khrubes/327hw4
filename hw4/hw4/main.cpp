#include "all.h"
#include "SndMix.hpp"
#include "SoundProgram.hpp"
#include "util.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    try{
        SndMix* sf = new SndMix();
        sf->runProgram(charArrayToStringVector(argc, argv));
    }
    catch (exception& e)
    {
        cout << "An error has occured \" " << e.what() << "\"" << endl;
    }
    return 0;
}
