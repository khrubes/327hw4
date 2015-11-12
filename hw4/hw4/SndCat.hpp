#ifndef SndCat_hpp
#define SndCat_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
 This class represents the SndInfo program.
 */
class SndCat : public SoundProgram {
    protected:
        void runProgram();
    public:
        SndCat(vector<string> fileArguments);
};

#endif /* SndCat_hpp */
