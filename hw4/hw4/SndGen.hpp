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
        long lastSampleValue;
        void initRequiredArgumentsVector();
        long getSampleValue(long currentTime, int iterationNum);
        long getXValue(int iterationNum);
        void calculateSustainVolume();
        long getAmplitudeValue(long currentTime);
        long getAttackAmplitudeValue(long currentTime);
        long getDecayAmplitudeValue(long currentTime);
        long getReleaseAmplitudeValue(long currentTime);
    
        long getSinWaveValue(long currentTime, int iterationNum);
        long getTriangleWaveValue(long currentTime, int iterationNum);
        long getSawtoothWaveValue(long currentTime, int iterationNum);
        long getPulseWaveValue(long currentTime, int iterationNum);
        long getPulseWaveRecurisive(long currentTime, int IterationNum);
    protected:
        string getProgramName();
        string getProgramDescription();
        virtual vector<string> getValidSwitches(bool withParams);
    
    public:
        SndGen();
        virtual string getSwitchDescription(string switchName);
        virtual void runProgram(vector<string> fileArguments);
};

#endif /* SndGen_hpp */
