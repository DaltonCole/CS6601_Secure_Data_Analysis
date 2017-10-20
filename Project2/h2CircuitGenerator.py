# Python file to generate circuit file and input files
#then execute them given a jsonfile(format specified belo
#and the path to Gcparser on the system


import sys
import os
import subprocess
import json
from collections import deque
from multiplicationCircuitGenerator import generateCircuitForFile

def main():
    # Need config.json as a parameter
    if(len(sys.argv)!= 3):
        print('Usage: python3 h2CircuitGenerator.py config.json <GCParser location>')
        return
    print("starting")
    # Json dictionary of {t:int numBitsForInput: int party1: [ints], party2: [ints]}
    data = {}
    with open(sys.argv[1], 'r') as data_file:
        data = json.load(data_file)

    for key, value in data.items():
        # Convert party1 and party 2 to a list of ints
        data[key] = list(map(int, data[key].split(',')))

    # Find max number of bits

    numBitsForInput = 2
    max_input = 0
    if(len(data['party1'])!=len(data['party2'])):
        print("both parties must have same number of items")
        quit()
    #now make sure properly allocated bits
    for value in data['party1']:
        max_input = max(max_input, value)
    for value in data['party2']:
        max_input = max(max_input, value)
    while(pow(2, numBitsForInput)-1 < max_input):
        numBitsForInput += 1
    if(numBitsForInput>data['numBitsForInput'][0]):
        print("Invalid number of bits allocated for inputs ")
        quit()

    circuitFileName = "secureDotProduct.cir"
    generateCircuitForFile( circuitFileName, data['numBitsForInput'][0], len(data['party1']))

    # File names
    #outFileName = "secureDotProduct.cir"
    outInput1FileName = "partyOneInputFile"
    outInput2FileName = "partyTwoInputFile"

    # Output texts
    #outText = ""
    party1outText = "t "+str(data['t'][0])+"\n"
    party2outText = ""

    # Generate input files for server and client
    input_index = 0
    for value in data['party1']:
        #outText += ".input party1a" + str(input_index) + " 1 " + str(numBitsForInput) + '\n'
        party1outText += "party1Term" + str(input_index) + " " + str(value) + '\n'
        input_index += 1
    input_index = 0
    for value in data['party2']:
        party2outText += "party2Term" + str(input_index) + " " + str(value) + '\n'
        input_index += 1
    with open(outInput1FileName, 'w') as f:
        f.write(party1outText)
    with open(outInput2FileName, 'w') as f:
        f.write(party2outText)
    #now run the generated gc circuit
    print(" running " + "./runCircuit " + sys.argv[
        2] + " " + circuitFileName + " " + outInput1FileName + " " + outInput2FileName)
	#need to set circuit file to executable
    os.system("chmod +x runCircuit")
    os.system("./runCircuit " + sys.argv[2] + " ./" + circuitFileName + " ./" + outInput1FileName + " ./" + outInput2FileName)

if __name__ == "__main__":
    main()