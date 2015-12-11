#327 HW 4

##SoundProgram
The SoundProgram class is an abstract implementation of a SoundProgram. 
I chose to use an abstract class rather than an interface while there is a lot of shared functionality between the SoundPrograms an abstract class could implement. 

###SoundProgram also has a few pure virtual functions, such as
- void runProgram()
	Here you implement what the SoundProgram will do with the provided SoundFiles and switch arguments.
- string getProgramName()
	Return the name of the program, to be used in the -h help screen.
- string getProgramDescription()
	Returns a short description of what the program does, to be used in the -h help screen.

###To add a switch to a SoundProgram:
####@override
vector<string> getValidSwitches(bool withParams)
	 Add your "-switch" to the vector returned by this function. Note this program splits the set returned based on if this switch should expect a parameter. It's best to call superclass version first in your implementation, if you want to support switches implemented by superclasses. 
bool isValidSwitchArgumentPair(string switchArg, string paramValue);
	In this function, define if the "-switch" @param switchArg and the proposed argument @param paramValue are valid together based on the logic of the switch.

####Define a SwitchFunction  
	bool newnameSwitch(string argument);
	and return the function in based off of @param switchArg
	SwitchFunction getSwitchFunction(string switchArg);

####A few functions that typically require additional implementation from subclassed SoundPrograms
- void SoundProgram::initSwitchArgumentMap(vector<string>* arguments)
	Removes strings resembling switches "-*" and their following values "n" from @arguments and adds the pairs to #switchArgumentMap.
    This function also outputs an error message if an invalid switch was passed in.
    @param arguments the arguments passed in from the command line. 

- void SoundProgram::initSoundFiles(vector<string> arguments)
		@param arguments the file arguments to iterate over and store SoundFile object representations of in #soundFiles.
        Switches have already been removed from @arguments in #initSwitchArgumentMap. 
        If an input file is not provided, the program will wait until a valid one is inputted line-by-line from the console.
        If the argument cannot be made into a #SoundFile, an error will be printed to stderr.

- void SoundProgram::initSwitchFunctionMap()
    Iterates through the switches provided to the program and maps the appropriate function to them, storing the pairs in #switchFunctionMap'

- void SoundProgram::runSwitches()
	Iterates through switches in #switchArgumentMap and calls the corresponding functions in #switchArgumentMap.

##SndInfo
  Prints the following for each SoundFile in #soundFiles
    -The file name
    - The file type
    - The sample rate
    - The bit depth
    - The number of channels
    - The number of samples
    - The length of the sound (in seconds)


##SndCat
This program reads all sound files passed as arguments, and writes a single sound file that is the concatenation of the inputs. 
Note, SndCat currently does not support concantenating Sound Files with different sample rates by interpolating. An error will by printed if files passed to SndCat are 

##SndMix
This program reads all sound files passed as arguments, and “mixes” them into a single sound file. The program is invoked as 
sndmix [switches] mult1 file1 ...multn filen 
 
where the sample data of filei is multiplied by multi (a real value between -10 and 10)
SndMix actually calls the same runProgram as SndCat, as it was designed to be as similar to SndCat as possible.

##SndGen
	This program produces a sound of a specified frequency and waveform using a simple ADSR envelope.


##SndFx
	"This program produces a sound with a combination of special effects, such as -backwards, -pitchup x, -pitchdown x, -fadeinout, and -echo numtimes."	


##SoundFileBuilder
buildCS229SoundFileFromInput
    Builds a #SoundFile object from input in the form of a CS229 file.
    @param input, the name of the file to create the SoundFile from, or "" by default. If no file name is provided, a SoundFile is built by reading from stdin.
    @param multiplyValue the optional value to multiply SoundFile::channels sample data by.
    @return a pointer to a newly constructed SoundFile, or NULL if there was an error processing the input.

buildSoundFileFromADSREvelope
###Sin Wave
	Sin Waves are produced using
	y = sin (period * x)

###Triangle Wave
 - Trangle Waves are produced using the function 
 	- y = (2/pi) * arcsin( sin ( ((2*pi)/ period ) x ) )

###Sawtooth Wave
  - Sawtooth Waves are produced using the function
    - y = -1 * (2/pi) * arctan( cot ( (x*pi)/ period )  )

###Pulse Wave
	Pulse Waves are produced using the following logic
	  ```C++
	  if ( ( sampleNum / sampleRate )* frequency - floor((1/sampleRate) - frequency) < percent up ) {
        return maxVolume;
      else
        return -1 * maxVolume;
       ```

##SoundFileLogger
    This Singleton class records data about produced #SoundFiles to be used by SndCat and SndMix.
