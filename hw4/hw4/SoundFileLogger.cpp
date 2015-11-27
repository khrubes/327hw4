#include "SoundFileLogger.hpp"
#include <limits.h>

SoundFileLogger* SoundFileLogger::soundfileLoggerInstance = NULL;
long SoundFileLogger::sampleRateSum;
int SoundFileLogger::numSoundFilesCreated;
int SoundFileLogger::maxBitDepth;
int SoundFileLogger::totalNumSamples;

/*
    @param soundFile a #SoundFile to record data on, or NULL if this instance is not needed to modify #SoundFile data.
    This method is safe to use if a SoundFileLogger instance has not been created yet.
*/
SoundFileLogger* SoundFileLogger::logInstance(SoundFile* soundFile) {
    if (!soundfileLoggerInstance){
        soundfileLoggerInstance = new SoundFileLogger;
        soundfileLoggerInstance->maxBitDepth = INT_MIN;
    }
    if (soundFile->getBitDepth() > SoundFileLogger::maxBitDepth) {
        SoundFileLogger::maxBitDepth = soundFile->getBitDepth();
    }
    SoundFileLogger::sampleRateSum+= soundFile->getSampleRate();
    SoundFileLogger::numSoundFilesCreated++;
    SoundFileLogger::totalNumSamples+= soundFile->getNumSamples();
    return soundfileLoggerInstance;
}


/*
    SoundFileLogger is a singleton class used to log data about #SoundFiles.
*/
SoundFileLogger* SoundFileLogger::getInstance() {
    if (!soundfileLoggerInstance){
        soundfileLoggerInstance = new SoundFileLogger;
        soundfileLoggerInstance->maxBitDepth = INT_MIN;
    }
    return soundfileLoggerInstance;
}


/*
    Computes the average sample rate of #SoundFiles known to the #SoundFileLogger
*/
int SoundFileLogger::getAverageSampleRate(){
    return (SoundFileLogger::sampleRateSum / SoundFileLogger::numSoundFilesCreated);
}