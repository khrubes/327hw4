#ifndef SoundFileBuilder_hpp
#define SoundFileBuilder_hpp
#include "all.h"
#include "SoundFile.hpp"
using namespace std;

/*
 This class is responsible for creating SoundFile and WavFile objects for various cases.
 */
class SoundFileBuilder {
    private:
        float lastAmplitudeValue;
    
        bool isCS229Heading(string line);
        bool shouldIgnoreLine(vector<string> lineVector);
        bool areSoundFileDataValuesInitialzed(int bitRes, int numChannels, int sampleRate);
        bool addStartDataToSoundFile(SoundFile** soundFile, istream& input, unsigned int multiplyValue);
    
        /* Getters for generating sample data */
        float getSampleValue(string waveFormType, float currentTime, float frequency, int sampleNum, int sampleRate, float pf, float maxVolume);
        float getXValue(int sampleNum, int totalNumSamples, float lengthOfSound);
        float getAmplitudeValue(float currentTime, float attackTime, float decayTime, float sustainTime, float releaseTime, float totalSoundLength);
    
        /* Formulas for generating wave values at a specific time and sample number.*/
        float getSinWaveValue(float currentTime, float period);
        float getTriangleWaveValue(float currentTime, float period);
        float getSawtoothWaveValue(float currentTime, float period);
        float getPulseWaveValue(float currentTime, int sampleNum, int sampleRate, float frequency, float pf, float maxVolume);

    public:
        SoundFileBuilder();
        SoundFile* buildSoundFileFromInput(string input = "", unsigned int multiplyValue = 1);
        SoundFile* buildSoundFileFromADSREvelope(float attackTime, float decayTime, float sustainTime, float releaseTime, float totalSoundLength, float frequency, float volumePeak, float pf, int bitdepth, int sampleRate, string waveForm);
        bool isValidFileType(string fileName, bool printOutput=true);
};

#endif /* SoundFileBuilder_hpp */
