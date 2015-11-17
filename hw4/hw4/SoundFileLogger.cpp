#include "SoundFileLogger.hpp"
#include <limits.h>

SoundFileLogger* SoundFileLogger::soundfileLoggerInstance = NULL;
long SoundFileLogger::sampleRateSum;
int SoundFileLogger::numSoundFilesCreated;
int SoundFileLogger::maxBitDepth;

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
        if (soundFile->getBitDepth() > SoundFileLogger::maxBitDepth) {
            SoundFileLogger::maxBitDepth = soundFile->getBitDepth();
        }
        SoundFileLogger::sampleRateSum+= soundFile->getSampleRate();
        SoundFileLogger::numSoundFilesCreated++;
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
