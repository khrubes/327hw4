#ifndef SndInfo_hpp
#define SndInfo_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
    This class represents the SndInfo program.
 */
class SndInfo : public SoundProgram {
    protected:
        virtual void runProgram(vector<string> fileArguments);
        string getProgramName();
        string getProgramDescription();
    
    public:
        SndInfo(vector<string> fileArguments);
};


#endif /* SndInfo_hpp */
