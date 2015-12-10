#ifndef SndGen_hpp
#define SndGen_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
 This class represents the SndGen program.
 */
class SndGen : public SoundProgram {
    private:
        vector<string> requiredArgumentsVector;
        string waveFormType;
        float lastSampleValue;
        bool hasValidInputsToRunProgram();
        void setWaveFormType(vector<string>* arguments);
        bool isWaveFormSwitch(string argument);
        bool isValidADSREnvelope();
        void initRequiredArgumentsVector();
        void calculateSustainVolume();
        float getSampleValue(float currentTime, int iterationNum);
        virtual float getXValue(int iterationNum);
        virtual float getAmplitudeValue(float currentTime);
        float getAttackAmplitudeValue(float currentTime);
        float getDecayAmplitudeValue(float currentTime);
        float getReleaseAmplitudeValue(float currentTime);
    
        float getSinWaveValue(float currentTime, int iterationNum);
        float getTriangleWaveValue(float currentTime, int iterationNum);
        float getSawtoothWaveValue(float currentTime, int iterationNum);
        float getPulseWaveValue(float currentTime, int iterationNum);
        float getPulseWaveRecurisive(float currentTime, int IterationNum);
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
