#Python file to generate circuit file
#to compute secure dot product given number of terms
#and number of bits of each term

import math

#generates line of circuit files for multiplication between term1 and term2
#numbits is number of bits for term1 and term2
# maxneededBits is highest number of bits needed for multiplication between the two
# and run is a unique string identifier for this multiplication to generate unique
#variable names required by gcparser
def generateMultiplicationforNBitNumbers(term1,term2,numBits,maxNeededBits,run):
    text = ""
    #before bit by bit multiplication according to
    #https://en.wikipedia.org/wiki/Binary_multiplier
    for i in range(0,numBits):
        curLoop =str(i)
        #first we get the current bit
        text = text + run+ "curBit"+curLoop+" select "+ term1+" "+ str(i)+" "+str(i+1)+"\n"
        #and extend it to our bit length
        text+= run+"curBitExt"+curLoop+" concat "
        for k in range(0,numBits):
            text+= run+ "curBit"+curLoop+ " "
        text+="\n"
        #then and with other term to get multiplication result for this bit
        text+=  run+"additionResult"+curLoop+chr(ord('a') +numBits)+" and " +run+"curBitExt"+curLoop \
                + " " +term2 + "\n"
        #and finally add it to our running total
        if(i==0):
            text+= run+"multiplicationResult"+curLoop+" concat 0:"+str(maxNeededBits-numBits)+" "+run+"additionResult"+curLoop+chr(ord('a') +numBits)\
                   +"\n"
        else:
            #shift left by i and
            text+= run + "additionResultFinal" + curLoop+chr(ord('a')+numBits) + " concat 0:"+str(maxNeededBits-numBits-i)+" "+ \
                   run + "additionResult" + curLoop + chr(ord('a') + numBits) + " 0:"+str(i) + "\n"
            #we add i zeroes then make sure same length for addition
            text+= run+"multiplicationResult"+curLoop+" add "+run+"multiplicationResult"+str(i-1)+\
                   " "+run + "additionResultFinal" + curLoop+ chr(ord('a') + numBits)+"\n"

    return text

#function that writes out to outputFile the circuit for secure dot product
#with two vectors each with unsigned int input of numBitsForInput and
#having numTerms terms
def generateCircuitForFile(outputFile,numBitsForInput,numTerms):
    outText = ""
    #calculate max possible bits needed for operations
    maxNeededMultiplicationBits = 2*numBitsForInput
    print(maxNeededMultiplicationBits)
    maxNeededSumBits = maxNeededMultiplicationBits+int(math.ceil(math.log(maxNeededMultiplicationBits,2)))
    #now make the circuit file
    with open(outputFile,'w') as f:

        # generate input block
        outText += ".input t 2 "+str(maxNeededSumBits)+"\n"
        for i in range(0,numTerms):
            outText = outText + ".input party1Term"+str(i)+" 2 "+str(numBitsForInput)+"\n"
            outText = outText + ".input party2Term"+str(i)+" 1 "+str(numBitsForInput)+"\n"
        #then output block
        outText = outText + "\n"
        outText = outText + ".output finalResult"+str(numTerms-1)+"\n"
        outText = outText + ".output greaterOrEqualToT"+"\n"
        outText = outText + "\n"
        f.write(outText)
        outText = ""
        #now generate the math
        for i in range(0,numTerms):
            #first we generate the multiplications for current vector element
            outText += generateMultiplicationforNBitNumbers("party1Term"+str(i),"party2Term"+str(i),
                                                            numBitsForInput,maxNeededMultiplicationBits,chr(ord('a')+i))
            #then we add them to the running total
            if(i==0):
                outText +="finalResultInter0 select "+chr(ord('a') +i)+"multiplicationResult"+str(numBitsForInput-1)+" 0 "+\
                           str(maxNeededMultiplicationBits) +"\n"
                outText += "finalResult0 concat 0:"+str(maxNeededSumBits-maxNeededMultiplicationBits)+ " "+\
                            chr(ord('a') +i)+"multiplicationResult"+str(numBitsForInput-1)+"\n"
            else:
                outText += "finalResultInterExt"+str(i)+" concat  0:"+str(maxNeededSumBits-maxNeededMultiplicationBits)+ " "+ \
                           chr(ord('a') + i) + "multiplicationResult" + str(numBitsForInput - 1) + "\n"
                outText += "finalResult"+str(i)+" add finalResult"+str(i-1)+" finalResultInterExt"+str(i)+"\n"
            f.write(outText)
            outText = ""
        outText+= "greaterOrEqualToT gteu finalResult"+str(numTerms-1)+" t"+"\n"
        f.write(outText)
        outText = ""
