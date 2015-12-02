#include "SoundFileLogger.hpp"
#include <limits.h>

SoundFileLogger* SoundFileLogger::soundfileLoggerInstance = NULL;
int SoundFileLogger::maxBitDepth;
int SoundFileLogger::sampleRate;
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
    if (SoundFileLogger::sampleRate==0) { /* this is the first soundFile we are logging and have not initialized sampleRate yet*/
        SoundFileLogger::sampleRate = soundFile->getSampleRate();
    } else {
        if (SoundFileLogger::sampleRate != soundFile->getSampleRate()) {
            fprintf(stderr, "Concantenation of soundfiles with different sample rates not supported.");
            exit(0);
        }
    }
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
