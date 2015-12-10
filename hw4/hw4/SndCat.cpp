#include "SndCat.hpp"
#include <algorithm>
#include "SoundFileLogger.hpp"
#include "util.hpp"

SndCat::SndCat() : SoundProgram(){}

string SndCat::getProgramName(){
    return "SndCat";
}

string SndCat::getProgramDescription(){
    return "This program reads all sound files passed as arguments, and writes a single sound file that is the concatenation of the inputs";
}

/*
    This program reads all sound files passed as arguments, and writes a single sound file that is
    the concatenation of the inputs. If no files are passed as arguments, the program reads from standard input.
 */
void SndCat::runProgram(vector<string> fileArguments){
    this->initSwitchArgumentMap(&fileArguments);
    this->initSwitchFunctionMap();
    this->initSoundFiles(fileArguments);
    runSwitches();
    
    SoundFile sndCat;
    sndCat.setNumChannels(SoundFileLogger::maxNumChannels);
    sndCat.setNumSamples(SoundFileLogger::maxNumSamplesInChannel * sndCat.getNumChannels());
    sndCat.setBitDepth(SoundFileLogger::getInstance()->maxBitDepth);
    sndCat.setSampleRate(SoundFileLogger::getInstance()->sampleRate);
    concantenateSoundFiles(&sndCat, this->soundFiles, SoundFileLogger::numRowsForSndCat /* num rows for channel */);
    this->outputSoundFile(&sndCat);
}
