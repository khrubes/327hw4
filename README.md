# 327hw4

To add a new Switch:
- add a new description in SoundProgram::getSwitchDescription
- ensure getValidSwitches for the SoundProgram you are adding the Switch to includes your switch 

If an input file is not provided, the program will wait until a valid one is inputted line-by-line from the console.


##SoundProgram
The SoundProgram class is an abstract implementation of a SoundProgram. 
To implement a new SoundProgram:

- extend SoundProgram

- @override
-- runProgram
-- getProgramName
-- getProgramDescription

I chose to use an abstract class rather than an interface while there is a lot of shared functionality between the SoundPrograms an abstract class could implement. 
SoundProgram also has a few pure virtual functions, such as :

To add a switch to a SoundProgram:
- @override
vector<string> getValidSwitches(bool withParams)
	 Add your "-switch" to the vector returned by this function. Note this program splits the set returned based on if this switch should expect a parameter. It's best to call superclass version first in your implementation, if you want to support switches implemented by superclasses. 
bool isValidSwitchArgumentPair(string switchArg, string paramValue);
	In this function, define if the "-switch" @param switchArg and the proposed argument @param paramValue are valid together based on the logic of the switch.

- Define a SwitchFunction  
	bool newnameSwitch(string argument);
	and return the function in based off of @param switchArg
	SwitchFunction getSwitchFunction(string switchArg);

- void SoundProgram::initSwitchArgumentMap(vector<string>* arguments)
	Removes strings resembling switches "-*" and their following values "n" from @arguments and adds the pairs to #switchArgumentMap.
    This function also outputs an error message if an invalid switch was passed in.
    @param arguments the arguments passed in from the command line. 

- void SoundProgram::initSoundFiles(vector<string> arguments)
		@param arguments the file arguments to iterate over and store SoundFile object representations of in #soundFiles.
        Switches have already been removed from @arguments in #initSwitchArgumentMap, 
        if the argument cannot be made into a #SoundFile, an error will be printed to stderr.

- void SoundProgram::initSwitchFunctionMap()
    Iterates through the switches provided to the program and maps the appropriate function to them, storing the pairs in #switchFunctionMap



And a few functions that typically require additional implementation from subclassed SoundPrograms

##SndInfo

##SndCat
Note, SndCat currently does not support concantenating Sound Files with different sample rates by interpolating. An error will by printed if files passed to SndCat are 

##SndMix
SndMix actually calls the same runProgram as SndCat, as I designed it to be as similar to SndCat as possible.
##SndGen
	
