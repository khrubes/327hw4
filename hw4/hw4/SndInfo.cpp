#include "SndInfo.hpp"

SndInfo::SndInfo(vector<string> fileArguments) : SoundProgram(fileArguments){
    runProgram();
}


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
void SndInfo::runProgram(){
    for (auto &soundFile : this->soundFiles) // access by reference to avoid copying
    {
        cout << "File name: " << soundFile->getFileName() << endl;
        cout << "File type: " << soundFile->getFileType() << endl;
        cout << "Sample rate: " << soundFile->getSampleRate() << endl;
        cout << "Bit depth: " << soundFile->getBitRes() << endl;
        cout << "Number of channels: " << soundFile->getNumChannels() << endl;
        cout << "Number of samples: " << soundFile->getNumSamples() << endl; //TODO you don't really need numsamples
        long soundLengthInSeconds = (long)(soundFile->getNumSamples() / soundFile->getSampleRate());
        cout << "Sound length in seconds: " << soundLengthInSeconds << endl;
    }
    
}