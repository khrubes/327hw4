//#include "SwitchManager.hpp"
//SwitchManager::SwitchManager(SoundProgram* soundProgram){
//    this->soundProgram = soundProgram;
//}



///*
//    @param outputFileName the output file name
//    @param soundProgram the #SoundProgram to store @outputFileName
//    @return true if @outputFileName is a valid file
//*/
//const bool SwitchManager::oSwitch(string outputFileName){
//    if(!this->soundProgram->soundFileBuilder->isValidFileType(outputFileName, false /*print output*/)){
//        return false;
//    }
//    this->soundProgram->setOutputFileName(outputFileName);
//    return true;    //TODO use for -o
//}

/*
    @param switchArg the switch which determines which function to return
    @return a SwitchFunction, a pointer to a function which implements the given switch
*/
//SwitchManager::SwitchFunction getSwitchFunction(string switchArg){
//    if (switchArg.compare("-h")==0) {
//        return (SwitchManager::SwitchFunction)hSwitch;
//    }else if(switchArg.compare("-o")==0){
//        return (SwitchManager::SwitchFunction)oSwitch;
//    }else{
//        cout << "TODO support more switches" << endl;
//        return NULL;
//    }
//}

//
///*
// Removes strings resembling switches "-*" and their following values "n" from @arguments and adds the pairs to #switchArgumentMap.
// This function also outputs an error message if an invalid switch was passed in.
// @param arguments the arguments passed in from the command line.
// */
//void SwitchManager::initSwitchArgumentMap(vector<string>* arguments){
//    const string switchChar = "-";
//    vector<string>::iterator iter = (*arguments).begin();
//    
//    while(iter < (*arguments).end()){
//        string argument = *iter;
//        size_t foundSwitchChar = argument.find(switchChar);
//        
//        // if the argument resembles a switch (contains "-") and is not a file name
//        if (foundSwitchChar!=string::npos && !(this->soundProgram->soundFileBuilder->isValidFileType(argument, false /*print output*/))) {
//            
//            if ( isValidSwitchWithNoParams(argument) ) {
//                this->switchArgumentMap.insert(make_pair(argument, ""));
//                iter = (*arguments).erase(iter);
//            } else if ( isValidSwitchWithParams(argument) ){
//                //this switch has a parameter, so the next element in @arguments is the value and we need to add it as a value to the hashmap
//                try{
//                    string switchParamArg = *(iter+1);
//                    this->switchArgumentMap.insert(make_pair(argument, switchParamArg));
//                    iter = (*arguments).erase(iter);
//                    iter = (*arguments).erase(iter); //TODO see if there's a better way to remove things from a vector
//                }
//                catch(...){
//                    cout << "No argument provided for " << argument << endl;
//                    exit(1);
//                }
//            }else{
//                cout << "Invalid switch \"" << argument << "\"" << endl;
//                exit(1);
//            }
//        } else {
//            iter++; //in the other case, we are removing elements from @arguments and want to leave iter as is.
//        }
//    }
//}
//
///*
// @param switchArg, the switch argument to test
// @return true if @switchArg is a valid switch that has no parameters following it
// */
//bool SwitchManager::isValidSwitchWithNoParams(string switchArg){
//    vector<string> validNoParamSwitches;
//    validNoParamSwitches.push_back("-h");
//    validNoParamSwitches.push_back("--sine");
//    validNoParamSwitches.push_back("--triangle");
//    validNoParamSwitches.push_back("--sawtooth");
//    validNoParamSwitches.push_back("--pulse");
//    
//    return(find(validNoParamSwitches.begin(), validNoParamSwitches.end(), switchArg) != validNoParamSwitches.end());
//}
//
///*
// @param switchArg, the switch argument to test
// @return true if @switchArg is a valid switch that has a parameter following it.
// */
//bool SwitchManager::isValidSwitchWithParams(string switchArg){
//    vector<string> validParamSwitches;
//    validParamSwitches.push_back("-o");
//    validParamSwitches.push_back("-f");
//    validParamSwitches.push_back("-t");
//    validParamSwitches.push_back("-v");
//    validParamSwitches.push_back("-a");
//    validParamSwitches.push_back("-d");
//    validParamSwitches.push_back("-s");
//    validParamSwitches.push_back("-r");
//    validParamSwitches.push_back("--bits");
//    validParamSwitches.push_back("--sr");
//    validParamSwitches.push_back("--pf");
//    
//    return(find(validParamSwitches.begin(), validParamSwitches.end(), switchArg) != validParamSwitches.end());
//}
