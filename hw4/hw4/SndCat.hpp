#ifndef SndCat_hpp
#define SndCat_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
 This class represents the SndCat program.
*/
class SndCat : public SoundProgram {
    protected:
        string getProgramName();
        string getProgramDescription();    
    public:
        SndCat();
        virtual void runProgram(vector<string> fileArguments);
};

#endif /* SndCat_hpp */
