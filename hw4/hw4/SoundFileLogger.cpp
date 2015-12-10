#include "SoundFileLogger.hpp"
#include <limits.h>

SoundFileLogger* SoundFileLogger::soundfileLoggerInstance = NULL;
int SoundFileLogger::maxBitDepth;
int SoundFileLogger::maxNumChannels;
int SoundFileLogger::sampleRate;
int SoundFileLogger::maxNumSamplesInChannel;
int SoundFileLogger::numRowsForSndCat;

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
    
    if (soundFile->getNumChannels() > SoundFileLogger::maxNumChannels) {
        SoundFileLogger::maxNumChannels = soundFile->getNumChannels();
    }
    
    int numSamplesInChannel = (*(soundFile->getChannels()))[0].size();
    if (SoundFileLogger::maxNumSamplesInChannel < numSamplesInChannel ) {
        SoundFileLogger::maxNumSamplesInChannel = numSamplesInChannel;
    }
    
    if (SoundFileLogger::sampleRate==0) { /* this is the first soundFile we are logging and have not initialized sampleRate yet*/
        SoundFileLogger::sampleRate = soundFile->getSampleRate();
    } else {
        if (SoundFileLogger::sampleRate != soundFile->getSampleRate()) {
            fprintf(stderr, "Concantenation of soundfiles with different sample rates %d and %d not supported.\n", SoundFileLogger::sampleRate, soundFile->getSampleRate());
            exit(0);
        }
    }
    
    SoundFileLogger::numRowsForSndCat += (*(soundFile->getChannels()))[0].size();
    
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
