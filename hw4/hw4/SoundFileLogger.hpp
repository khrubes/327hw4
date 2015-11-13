#ifndef SoundFileLogger_hpp
#define SoundFileLogger_hpp
#include "all.h"
#include "SoundFile.hpp"

/*
    This Singleton class records data about produced #SoundFiles to be used by SndCat and SndMix.
*/
class SoundFileLogger{
public:
    static SoundFileLogger* getInstance(SoundFile* soundFile = NULL);
    int computeMedian(vector<int> values);

private:
    SoundFileLogger(){};
    static SoundFileLogger* soundfileLoggerInstance;
    
    static long sampleRateSum;
    static int numSoundFilesCreated;
    static int maxBitDepth;
};
#endif /* SoundFileLogger_hpp */
