#include "SndInfo.hpp"

SndInfo::SndInfo() : SoundProgram(){}

/*
  Prints the following for each SoundFile in #soundFiles
    -The file name
    - The file type
    - The sample rate
    - The bit depth
    - The number of channels
    - The number of samples
    - The length of the sound (in seconds)
 */
void SndInfo::runProgram(vector<string> arguments){
    initSwitchArgumentMap(&arguments);
    initSwitchFunctionMap();
    initSoundFiles(arguments);
    runSwitches();
    for (auto &soundFile : this->soundFiles) // access by reference to avoid copying
    {
        this->outputSoundFile(soundFile);
    }
    
}

/*
    Outputs the contents @param soundFile to the console.
 */
void SndInfo::outputSoundFile(SoundFile* soundFile){
    cout << "File name: " << soundFile->getFileName() << endl;
    cout << "File type: " << soundFile->getFileType() << endl;
    cout << "Sample rate: " << soundFile->getSampleRate() << endl;
    cout << "Bit depth: " << soundFile->getBitDepth() << endl;
    cout << "Number of channels: " << soundFile->getNumChannels() << endl;
    cout << "Number of samples: " << soundFile->getNumSamples() << endl;
    long soundLengthInSeconds = (long)(soundFile->getNumSamples() / soundFile->getSampleRate());
    cout << "Sound length in seconds: " << soundLengthInSeconds << endl;
}


string SndInfo::getProgramName(){
    return "SndInfo";
}

string SndInfo::getProgramDescription(){
    return "This program reads all sound files passed as arguments, and for each one, displays the following. \n"
         "The file name\n"
         "The file type\n"
         "The sample rate\n"
         "The bit depth\n"
         "The number of channels\n"
         "The number of samples\n"
         "The length of the sound (in seconds)\n"
         "If no files are passed as arguments, then the program should read from standard input.";
}