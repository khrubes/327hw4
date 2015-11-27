#ifndef SndMix_hpp
#define SndMix_hpp
#include "SndCat.hpp"
using namespace std;

/*
 This class represents the SndCat program.
 */
class SndMix : public SndCat {
    protected:
        virtual void initSoundFiles(vector<string> arguments);
        int getMultiplyValueFromSndMixArg(string str);
        string getProgramName();
        string getProgramDescription();
    public:
        SndMix();
};

#endif /* SndMix_hpp */
