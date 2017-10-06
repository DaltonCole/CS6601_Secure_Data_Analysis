#Python file to generate circuit file
#to compute secure dot product given number of terms
#and number of bits of each term

import sys
import os

def main():
    if(len(sys.argv)!= 5):
        print('Error except 4 arguments full path to script running circuit, the number of bits for inputs, party1 input and party2 input')
        return
    numBitsForInput = int(sys.argv[2])
    outFileName = "millionaireCircuit.cir"
    outInputFileName = "partyOneInputFile"
    outInput2FileName = "partyTwoInputFile"

    outText = ""
    #generate input block
    outText = outText + ".input a 1 "+str(numBitsForInput)+"\n"
    outText = outText + ".input b 2 "+str(numBitsForInput)+"\n"
    #then output block
    outText = outText + "\n"
    outText = outText + ".output firstPartyGreater"+"\n"

    #then computation block
    outText = outText + "\n"
    outText = outText + "firstPartyGreater gts a b"+"\n"

    #now write to a file, program will crash if it doesn't open for some reasons
    with open(outFileName,'w') as f:
        f.write(outText)

    outText = ""
    outText += "b "+sys.argv[4] + "\n"

    # now write to a file, program will crash if it doesn't open for some reasons
    with open(outInputFileName, 'w') as f:
        f.write(outText)
    outText = ""
    outText += "a " + sys.argv[3] + "\n"

    # now write to a file, program will crash if it doesn't open for some reasons
    outText = ""
    outText += "a "+sys.argv[3] + "\n"
    with open(outInput2FileName, 'w') as f:
        f.write(outText)
    #now execute the code
    os.system(sys.argv[1]+" ./"+outFileName +" ./"+ outInputFileName +" ./"+ outInput2FileName)


if __name__ == "__main__":
    main()