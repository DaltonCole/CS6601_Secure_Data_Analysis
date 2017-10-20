# Python file to generate circuit file
# to compute secure dot product given number of terms
# and number of bits of each term

import sys
import os
import subprocess
import json
from collections import deque

def generateMultiplicationforNBitNumbers(numBits,var1,var2):
	text = ""
	#not sure how to do this yet but final result should be in variable called
	#multiplicationResult
	for i in range(0,numBits):
		text = text + " result"

def main():
	# Need config.json as a parameter
	if(len(sys.argv)!= 3):
		print('Usage: python3 h2CircuitGenerator.py config.json <GCParser location>')
		return
	print("starting")
	# Json dictionary of {party1: [ints], party2: [ints]}
	data = {}
	with open(sys.argv[1], 'r') as data_file:    
		data = json.load(data_file)

	for key, value in data.items():
		# Convert party1 and party 2 to a list of ints
		data[key] = list(map(int, data[key].split(',')))

	# Find max number of bits
	numBitsForInput = 2
	max_input = 0

	for key, value in data.items():
		for i in value:
			max_input = max(max_input, i)
	while(pow(2, numBitsForInput) - 1 < max_input):
		numBitsForInput += 1
	numBitsForInput *= 2
	numBitsForInput += len(data['party1']) - 1

	generateCircuitForFile(data['threshold'], "secureDotProduct.cir", numBitsForInput, len(data['party1']))

	"""
	# File names
	outFileName = "secureDotProduct.cir"
	outInput1FileName = "partyOneInputFile"
	outInput2FileName = "partyTwoInputFile"

	# Output texts
	outText = ""
	party1outText = ""
	party2outText = ""

	# Generate input block
	input_index = 0;
	for value in data['party1']:
		outText += ".input party1a" + str(input_index) + " 1 " + str(numBitsForInput) + '\n'
		party1outText += "party1a" + str(input_index) + " " + str(value) + '\n'
		input_index += 1
	input_index = 0
	for value in data['party2']:
		outText += ".input party2b" + str(input_index) + " 2 " + str(numBitsForInput) + '\n'
		party2outText += "party2b" + str(input_index) + " " + str(value) + '\n'
		input_index += 1

	# Generate computation block
	temp_text = "\n"

	party_variable_list = deque()
	input_index = 0
	for value1, value2 in zip(data['party1'], data['party2']):
		for i in range(value2):
			party_variable_list.append("party1a" + str(input_index))
		input_index += 1


	index_number = 0
	while party_variable_list:
		if index_number == 0:
			temp_text += 'sum' + str(index_number) + ' add ' + str(party_variable_list.pop()) + ' ' + str(party_variable_list.pop()) + '\n'
		else:
			temp_text += 'sum' + str(index_number) + ' add ' + 'sum' + str(index_number - 1) + ' ' + str(party_variable_list.pop()) + '\n'
		index_number += 1

	# Generate output block
	outText += "\n"
	outText += ".output sum" + str(index_number - 1) + "\n"

	# Add computation block
	outText += temp_text

	with open(outFileName, 'w') as f:
		f.write(outText)

	with open(outInput1FileName, 'w') as f:
		f.write(party2outText)

	with open(outInput2FileName, 'w') as f:
		f.write(party1outText)
	print(" running " +"./runCircuit " + sys.argv[2] + " ./" + outFileName + " ./" + outInput1FileName + " ./" + outInput2FileName)
	os.system("./runCircuit " + sys.argv[2] + " ./" + outFileName + " ./" + outInput1FileName + " ./" + outInput2FileName)
	"""

if __name__ == "__main__":
	main()