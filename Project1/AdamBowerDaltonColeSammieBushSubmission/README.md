# Project 1 - Millionaire Problem using Garbled Circuits
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
	* We provide a h1Circuit generator which takes inputs, generates and then runs the circuit circuit file to solve the millionaire's problem securely
	* To run:
```
python3 h1CircuitGenerator.py /full/path/toGCParser/install/GCParser numberOfBitsForEachPartyInput Party1Input Party2Input
```
		* This will generate the millionaireCircuit.cir Party1InputFile Party2InputFile. Then the program runs GCParser with generated files via the runCircuit script. The results will be piped into ./results/siclientout ./results/siserverout
		* The result files have a variable firstpartygreater which is 1 if first party input is greater than the second party input

# Example
```
python3 h1CircuitGenerator.py /home/user/downloads/GCParser 4 10 11
```