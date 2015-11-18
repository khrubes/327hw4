#ifndef SndMix_hpp
#define SndMix_hpp
#include "SndCat.hpp"
using namespace std;

/*
 This class represents the SndCat program.
 */
class SndMix : public SndCat {
    protected:
        string getProgramName();
        string getProgramDescription();
        virtual void initSoundFiles(vector<string> arguments);
        int getMultiplyValueFromSndMixArg(string str);    
    public:
        SndMix(vector<string> fileArguments);
};

#endif /* SndMix_hpp */
