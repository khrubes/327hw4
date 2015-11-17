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
    static SoundFileLogger* getInstance(SoundFile* soundFile = NULL);
    static int computeMedian(vector<int> values);
    static long sampleRateSum;
    static int numSoundFilesCreated;
    static int maxBitDepth;
    
private:
    SoundFileLogger(){};
    static SoundFileLogger* soundfileLoggerInstance;
};
#endif /* SoundFileLogger_hpp */
