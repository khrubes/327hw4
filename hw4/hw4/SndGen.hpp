#ifndef SndGen_hpp
#define SndGen_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
 This class represents the SndGen program.
 */
class SndGen : public SoundProgram {
    private:
        /* Member Variables */
        vector<string> requiredArgumentsVector;
        string waveFormType;
    
        /* Helper Functions */
        bool hasValidInputsToRunProgram();
        bool isWaveFormSwitch(string argument);
        void setWaveFormType(vector<string>* arguments);
        void initRequiredArgumentsVector();
    //todo update in uml diagram
    protected:
        virtual string getProgramName();
        virtual string getProgramDescription();
        virtual vector<string> getValidSwitches(bool withParams);
        virtual bool isValidSwitchArgumentPair(string switchArg, string paramValue);
        virtual void initSwitchArgumentMap(vector<string>* arguments);
    
    public:
        SndGen();
        virtual string getSwitchDescription(string switchName);
        virtual void runProgram(vector<string> fileArguments);
};

#endif /* SndGen_hpp */
