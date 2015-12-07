#ifndef SoundFileLogger_hpp
#define SoundFileLogger_hpp
#include "SoundFile.hpp"
using namespace std;

/*
    This Singleton class records data about produced #SoundFiles to be used by SndCat and SndMix.
*/
class SoundFileLogger{
    public:
        static SoundFileLogger* logInstance(SoundFile* soundFile);
        static SoundFileLogger* getInstance();
        static int maxBitDepth;
        static int totalNumSamples;
        static int sampleRate;
    
    private:
        SoundFileLogger(){};
        static SoundFileLogger* soundfileLoggerInstance;
};
#endif /* SoundFileLogger_hpp */
