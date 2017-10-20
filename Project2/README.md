# Project 2 - Implementing the Threshold based SecureDot Product Protocol using FastGC
* Programmers:
	* Adam Bowers
	* Sammie Bush
	* Dalton Cole

# System Requirements
	* Linux is the expected operating system
		* Can run on Windows, but path information will be different
	* gcparser must be downloaded
		* It can be downloaded directly from:
			* http://mightbeevil.org/gcparser/ 
			* https://github.com/wrmelicher/GCParser
	* Java and Python3 to be installed

# How to Run
	* We provide a h2Circuit generator which takes in a config.json and the GCParser location to generates and then run the circuit file. This does a threshold based secure dot product on the inputs supplied in the config.json.
	
```
To Run:
python3 h2CircuitGenerator.py config.json [GCParser location]
```
		* This will generate secureDotProduct.cir, PartyOneInputFile.cir, and PartyTwoInputFile.cir. Then, the program runs GCParser with generated files via the runCircuit script. The results will be piped into ./results/siclientout ./results/siserverout
		* The result files have a variable greaterOrEqualToT which is 1 if the dot product is greater than t and 0 if the dot product is less than t.

# Examples
#Command:
```
python3 h2CircuitGenerator.py config.json ./
```

#config.json:
```
{
  "party1": "2,4,6",
  "party2": "1,3,8"
}
```
