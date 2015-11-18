#include "SndCat.hpp"
#include <algorithm>
#include "SoundFileLogger.hpp"
#include "util.hpp"

SndCat::SndCat(vector<string> fileArguments) : SoundProgram(fileArguments){
    initSoundFiles(fileArguments);
    runProgram(fileArguments);
}

string SndCat::getProgramName(){
    return "SndCat";
}

string SndCat::getProgramDescription(){
    return "This program reads all sound files passed as arguments, and writes a single sound file that is the concatenation of the inputs";
}

/*
    This program reads all sound files passed as arguments, and writes a single sound file that is
    the concatenation of the inputs. If no files are passed as arguments, the program reads from standard input.
 
    The number of channels set for the output file will be the median number of channels calculated from the specified input files.
    Missing values will be filled in as 0s.
 */
void SndCat::runProgram(vector<string> fileArguments){
    runSwitches();
    
    SoundFile sndCat;
    int numChannels = getMedianNumChannels(this->soundFiles);
    sndCat.setNumChannels(numChannels);
    sndCat.setNumSamples(SoundFileLogger::getInstance()->totalNumSamples);
    sndCat.setBitDepth(SoundFileLogger::getInstance()->maxBitDepth);
    sndCat.setSampleRate(SoundFileLogger::getInstance()->getAverageSampleRate());
    vector< vector<signed int>> channels( numChannels, vector<signed int>(SoundFileLogger::getInstance()->totalNumSamples, 0) );
    
    int samplesIndex = 0;
    for (auto &soundFile : this->soundFiles)
    {
        for (int channelIndex = 0; channelIndex < min(soundFile->getNumChannels(), numChannels); channelIndex++) {
            for (int sampleIndex =0; sampleIndex < soundFile->getNumSamples(); sampleIndex++) {
                ( channels[channelIndex] )[samplesIndex + sampleIndex] = ( *(soundFile->getChannels() ) )[channelIndex][sampleIndex];
            }
        }
        samplesIndex+= soundFile->getNumSamples(); //next iteration, we will fill in the samples starting at samplesIndex
        if (samplesIndex > SoundFileLogger::getInstance()->totalNumSamples) {
            fprintf(stderr, "Error concantenating SoundFiles");
            break;
        }
    }
    sndCat.setChannels(channels);
    //either set SoundFile to #outputFile or write to standard output
}

