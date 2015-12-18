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
    //todo update in UML diagram
        float lastAmplitudeValue;
    
        bool isCS229Heading(string line);
        bool shouldIgnoreLine(vector<string> lineVector);
        bool areSoundFileDataValuesInitialzed(int bitRes, int numChannels, int sampleRate);
        bool addStartDataToSoundFile(SoundFile** soundFile, istream& input, unsigned int multiplyValue);
    
        /* Getters for generating sample data */
        float getSampleValue(string waveFormType, float currentTime, float frequency, int sampleNum, int sampleRate, float pf, float maxVolume);
        float getXValue(int sampleNum, int totalNumSamples, float lengthOfSound);
        float getAmplitudeValue(float peakAmplitude, float currentTime, float attackTime, float decayTime, float sustainValue, float releaseTime, float totalSoundLength);
        float getAmplitudeAtPoint(float currentX, float x1, float y1, float x0, float y0);
        float getSlope(float y1, float y0, float x1, float x0);
        float getYIntercept(float slope, float xValue, float yValue);
    
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
