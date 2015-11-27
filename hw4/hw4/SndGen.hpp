#ifndef SndGen_hpp
#define SndGen_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
 This class represents the SndGen program.
 */
class SndGen : public SoundProgram {
    private:
        int durationInSeconds; //dont need
        int sampleRate; //dont need
        long peakVolume; //probably don't need
        string waveFormType;
    
        long attackSeconds; //probably don't need these, use switchargmap
        long decaySeconds;
        long sustainSeconds;
        long releaseSeconds;
    
        long lastSampleValue;
        long getSampleValue(long currentTime, int iterationNum);
        long getXValue(int iterationNum);
        long calculateSustainVolume(long percentage);
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
    
    public:
        SndGen();
        virtual void runProgram(vector<string> fileArguments);
};

#endif /* SndGen_hpp */
