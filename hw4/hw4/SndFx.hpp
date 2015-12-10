#ifndef SndFx_hpp
#define SndFx_hpp
#include "SndGen.hpp"
#include "SndCat.hpp"
#include "all.h"
using namespace std;

/*
 This class represents the SndFx program.
*/

class SndFx : public SndGen {
    protected:
        string getProgramName();
        string getProgramDescription();
        virtual vector<string> getValidSwitches(bool withParams);
        virtual bool isValidSwitchArgumentPair(string switchArg, string paramValue);
        virtual void initSwitchArgumentMap(vector<string>* arguments);
        virtual SwitchFunction getSwitchFunction(string switchArg);
        virtual float getAmplitudeValue(float currentTime, vector<signed int> currentChannel);
    
        /* Switch Functions */
        bool backwardsSwitch(string argument);
        bool fadeinoutSwitch(string argument);
        bool pitchupSwitch(string argument);
        bool pitchdownSwitch(string argument);
        bool echoSwitch(string argument);
    
    public:
        SndFx();
        virtual string getSwitchDescription(string switchName);
        virtual void runProgram(vector<string> fileArguments);
};

#endif /* SndFx_hpp */
