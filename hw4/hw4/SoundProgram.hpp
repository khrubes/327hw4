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
    protected:
        /* Functions */
        virtual string getProgramName() = 0;
        virtual string getProgramDescription() = 0;
        bool isValidSwitch(string switchArg, bool withParams);
        virtual bool isValidSwitchArgumentPair(string switchArg, string paramValue);
        virtual void initSwitchArgumentMap(vector<string>* arguments);
        void initSwitchFunctionMap();
        virtual void initSoundFiles(vector<string> arguments);
    
        void runSwitches();
        /*
            A SwitchFunction is pointer to a function which implements the functionality of a given switch.
            @param argument the argument passed in with the switch
            @return false if there was an error performing the SwitchFunction
         */
        typedef bool (SoundProgram::*SwitchFunction)(string argument);
        SwitchFunction getSwitchFunction(string switchArg);
    
        /* Switch Functions*/
        bool hSwitch(string argument);
        bool oSwitch(string argument);
        virtual vector<string> getValidSwitches(bool withParams);
    
        /* Member variables */
        unordered_map<string, SwitchFunction> switchFunctionMap;
        unordered_map<string, string> switchArgumentMap;
        vector<SoundFile*> soundFiles;
        string outPutFileName;
        SoundFileBuilder* soundFileBuilder;
    
    public:
        SoundProgram();
        void setOutputFileName(string fileName);
        virtual string getSwitchDescription(string switchName);
        virtual void runProgram(vector<string> fileArguments) = 0;
};

#endif /* SoundProgram_hpp */
