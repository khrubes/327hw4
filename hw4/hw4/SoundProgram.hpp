#ifndef SoundProgram_hpp
#define SoundProgram_hpp
#include "all.h"
#include "SoundFile.hpp"
#include "SoundFileBuilder.hpp"
using namespace std;
/*
    This class is an abstract representation for a Sound Program
*/
class SoundProgram {
    private:
        bool isValidSwitchWithNoParams(string switchArg);
        bool isValidSwitchWithParams(string switchArg);
        void initSwitchArgumentMap(vector<string>* arguments);
        void initSwitchFunctionMap();
    
    protected:
        virtual void runProgram() = 0;
    
        vector<SoundFile*> soundFiles;
        string outPutFileName;
        void initSoundFiles(vector<string> arguments); //sndgen will need to handle logic for multi arguments, make virtual eventually
        /*
            A SwitchFunction is pointer to a function which implements the functionality of a given switch.
            @param argument the argument passed in with the switch
            @return false if there was an error performing the SwitchFunction
         */
        typedef bool (*SwitchFunction)(string argument);
        SwitchFunction getSwitchFunction(string switchArg);
    
        /* Switch Functions*/
        static bool hSwitch(string argument); //override for different programs
    
        unordered_map<string, SwitchFunction> switchFunctionMap;
        unordered_map<string, string> switchArgumentMap;
    
    public:
        SoundProgram();
        SoundProgram(vector<string> arguments);
        void setOutputFileName(string fileName);
    
        SoundFileBuilder* soundFileBuilder;
};

#endif /* SoundProgram_hpp */
