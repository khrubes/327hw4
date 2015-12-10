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
        float lastSampleValue;
    
        /* Helper Functions */
        bool hasValidInputsToRunProgram();
        bool isWaveFormSwitch(string argument);
        bool isValidADSREnvelope();
        void setWaveFormType(vector<string>* arguments);
        void initRequiredArgumentsVector();
        void calculateSustainVolume();
    
        /* Getters for generating sample data */
        virtual float getAmplitudeValue(float currentTime);
        float getSampleValue(float currentTime, int sampleNum);
        float getXValue(int sampleNum);
        float getAttackAmplitudeValue(float currentTime);
        float getDecayAmplitudeValue(float currentTime);
        float getReleaseAmplitudeValue(float currentTime);
    
        /* Formulas for generating wave values at a specific time and sample number.*/
        float getSinWaveValue(float currentTime, int sampleNum);
        float getTriangleWaveValue(float currentTime, int sampleNum);
        float getSawtoothWaveValue(float currentTime, int sampleNum);
        float getPulseWaveValue(float currentTime, int sampleNum);
        float getPulseWaveRecurisive(float currentTime, int sampleNum);
    
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
