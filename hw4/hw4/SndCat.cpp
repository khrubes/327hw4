#include "SndCat.hpp"

SndCat::SndCat(vector<string> fileArguments) : SoundProgram(fileArguments){
    runProgram();
}


/*
 This program reads all sound files passed as arguments, and writes a single sound file that is
 the concatenation of the inputs. If no files are passed as arguments, then the program should
 read from standard input. You must support the following switches. 
 
 Note that there are situations that make it hard to concatenate sound files such as ones with different numbers
 of channels. You must choose how to resolve these types of situations. One way here is to
 convert each sound to the maximum number of channels of any sound file, or simply give an
 error to the user.
 
 -o file : specify the output file name; if omitted, write to standard output.
 */
void SndCat::runProgram(){
    //check for swtiches
    
    for (auto &soundFile : this->soundFiles) // access by reference to avoid copying
    {
        //concantinate them into SoundFile
    }
    
    //either set SoundFile to #outputFile or write to standard output
}
