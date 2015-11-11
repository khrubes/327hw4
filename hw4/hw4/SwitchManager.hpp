//#ifndef SwitchManager_hpp
//#define SwitchManager_hpp
//#include "all.h"
//#include "SoundProgram.hpp"
//using namespace std;
//
///*
//    This class returns references to #SwitchManager::SwitchFunctions, 
//        which are functions that house the logic for what the switch does and may take in a SoundFile to modify.
// */
//class SwitchManager {
//    private:
//        SoundProgram* soundProgram;
//        bool isValidSwitchWithNoParams(string switchArg);
//        bool isValidSwitchWithParams(string switchArg);
//        void initSwitchArgumentMap(vector<string>* arguments);
//    protected:
//        unordered_map<string, SwitchFunctionLibrary::SwitchFunction> switchFunctionMap;
//        unordered_map<string, string> switchArgumentMap;
//    public:
//        SwitchManager(SoundProgram* soundProgram);
//        /*
//            A SwitchFunction is pointer to a function which implements the functionality of a given switch. 
//                Such SwitchFunctions are managed by the #SwitchManager class
//            @param argument the argument passed in with the switch
//            @return false if there was an error performing the SwitchFunction
//         */
//        typedef bool (*SwitchFunction)(string argument);
//    
//        SwitchFunction getSwitchFunction(string switchArg);
//    
//        /* Switch Function library */
//        const bool hSwitch(string argument);
//        const bool oSwitch(string argument);
//};
//
//#endif /* SwitchFunctionManager_hpp */
