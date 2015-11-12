#ifndef SndCat_hpp
#define SndCat_hpp
#include "SoundProgram.hpp"
using namespace std;

/*
 This class represents the SndCat program.
*/
class SndCat : public SoundProgram {
    protected:
        void runProgram();
        string getProgramName();
        string getProgramDescription();
    
    public:
        SndCat(vector<string> fileArguments);
};

#endif /* SndCat_hpp */
