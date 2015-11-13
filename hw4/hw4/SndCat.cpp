#include "SndCat.hpp"

SndCat::SndCat(vector<string> fileArguments) : SoundProgram(fileArguments){
    runProgram();
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
 
 The number of samples set for the output file will be the median number of samples calculated from the specified input files.
 Files with a number of channels or samples below the median will have extra channels inserted with 0 for values.
 */
void SndCat::runProgram(){
    //check for swtiches
    runSwitches();
    
    for (auto &soundFile : this->soundFiles) // access by reference to avoid copying
    {
        //concantinate them into SoundFile
    }
    
    //either set SoundFile to #outputFile or write to standard output
}