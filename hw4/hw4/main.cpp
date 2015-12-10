#include "SndGen.hpp"
#include "SndInfo.hpp"
#include "SndCat.hpp"
#include "SndMix.hpp"
#include "SndFx.hpp"
#include "SoundProgram.hpp"
#include "util.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    try{
//        string programName = argv[0];
        string programName = "sndfx";
        SoundProgram* program = NULL;
        if (programName.find("sndinfo")!= string::npos) {
            program = new SndInfo();
        }else if (programName.find("sndcat")!= string::npos) {
            program = new SndCat();
        }else if (programName.find("sndmix")!= string::npos) {
            program = new SndMix();
        }else if (programName.find("sndgen")!= string::npos) {
            program = new SndGen();
        }else if (programName.find("sndfx")!= string::npos) {
            program = new SndFx();
        }else {
            fprintf(stderr, "Invalid sound program: %s.\n", programName.c_str());
        }
        program->runProgram(charArrayToStringVector(argc, argv));
    }
    catch (exception& e)
    {
        cout << "An error has occured \" " << e.what() << "\"" << endl;
    }
    return 0;
}
