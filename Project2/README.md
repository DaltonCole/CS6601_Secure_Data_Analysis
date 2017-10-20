# Project 2 - Implementing the Threshold based SecureDot Product Protocol using FastGC
* Programmers:
	* Adam Bowers
	* Sammie Bush
	* Dalton Cole

# Requirements
	* Linux is the expected operating system
		* Windows not supported
		
	* gcparser must be downloaded
		* It can be downloaded directly from:
			* http://mightbeevil.org/gcparser/ 
			* https://github.com/wrmelicher/GCParser
			
	* Java and Python3 to be installed
	
	*Threshold should be unsigned int that can fit within 2 * numBitsForInput + ceiling(lg numberOfTerms) bits
	
	*Number of bits for the input must be a positive integer
	
	*Party1 & Party2 each have same set size of unsigned ints all of which can be represented by numBitsForInput
	
	
	

# How to Run
	* We provide a h2Circuit generator which takes in a config.json and the GCParser location to generates and then run the circuit file. This does a threshold based secure dot product on the inputs supplied in the config.json.
	
```
To Run:
python3 h2CircuitGenerator.py [Configuration json file location] [GCParser location]
```
	* This will generate secureDotProduct.cir, PartyOneInputFile.cir, and PartyTwoInputFile.cir. Then, the program runs GCParser with generated files via the runCircuit script. The results will be piped into ./results/siclientout ./results/siserverout
	* The result files have a variable greaterOrEqualToT which is 1 if the dot product is greater than t and 0 if the dot product is less than t.

# Examples
Command:
```
python3 h2CircuitGenerator.py config.json ./
```

config.json:
```
{
  "t": "55340232195358851075",
  "numBitsForInput" :"32",
  "party1": "1,1,1",
  "party2": "4294967295,4294967295,4294967295"
}
```
