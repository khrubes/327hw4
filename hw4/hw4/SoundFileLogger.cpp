#include "SoundFileLogger.hpp"
#include <limits.h>

// Global static pointer used to ensure a singleton functionality
SoundFileLogger* SoundFileLogger::soundfileLoggerInstance = NULL;

/*
    SoundFileLogger is a singleton class used to track data about #SoundFiles.
    @param soundFile a #SoundFile to record data on, or NULL if this instance is not needed to modify #SoundFile data.
    @return a reference to this SoundFileLogger.
*/
SoundFileLogger* SoundFileLogger::getInstance(SoundFile* soundFile /* default is NULL */) {
    if (!soundfileLoggerInstance){
        soundfileLoggerInstance = new SoundFileLogger;
        soundfileLoggerInstance->maxBitDepth = INT_MIN;
    }
    
    if(soundFile){
        if (soundFile->getBitRes() > maxBitDepth) {
            maxBitDepth = soundFile->getBitRes();
        }
        sampleRateSum+= soundFile->getSampleRate();
        numSoundFilesCreated++;
    }
    
    return soundfileLoggerInstance;
}

/*
    @return the median number of samples from all #SoundFiles belonging to this program.
 */
int SoundFileLogger::computeMedian(vector<int> values){
    sort(values.begin(), values.end());
    return values[(values.size() / 2)];
}
