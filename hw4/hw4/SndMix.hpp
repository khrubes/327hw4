#ifndef SndMix_hpp
#define SndMix_hpp
#include "SndCat.hpp"
using namespace std;

/*
 This class represents the SndCat program.
 */
class SndMix : public SndCat {
    protected:
        void runProgram();
        string getProgramName();
        string getProgramDescription();
        virtual void initSoundFiles(vector<string> arguments);
        int getMultiplyValueFromSndMixArg(string str);
    
        /* Switch Functions */
        bool oSwitch(string argument);
    
public:
    SndMix(vector<string> fileArguments);
};

#endif /* SndMix_hpp */
