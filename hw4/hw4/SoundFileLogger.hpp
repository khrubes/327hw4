#ifndef SoundFileLogger_hpp
#define SoundFileLogger_hpp
#include "all.h"
#include "SoundFile.hpp"
using namespace std;

/*
    This Singleton class records data about produced #SoundFiles to be used by SndCat and SndMix.
*/
class SoundFileLogger{
public:
    static SoundFileLogger* logInstance(SoundFile* soundFile = NULL);
    static SoundFileLogger* getInstance();
    static long sampleRateSum;
    static int numSoundFilesCreated;
    static int maxBitDepth;
    static int totalNumSamples;
    static int getAverageSampleRate();
    
private:
    SoundFileLogger(){};
    static SoundFileLogger* soundfileLoggerInstance;    
};
#endif /* SoundFileLogger_hpp */
