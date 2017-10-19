#Python file to generate circuit file
#to compute secure dot product given number of terms
#and number of bits of each term

import math

def generateMultiplicationforNBitNumbers(term1,term2,numBits,maxNeededBits,run):
    text = ""
    #first have to extend to proper number of bits
    #text+= "sextend "+term1+" "+str(maxNeededBits)+"\n"
    #text+= "sextend "+term2+" "+str(maxNeededBits)+"\n"
    text+= run+"formatted"+term2+" sextend "+term2+" "+str(maxNeededBits)+"\n"
    #now go bit by bit to get total
    for i in range(0,numBits):
        curLoop =str(i)
        text = text + run+ "curBit"+curLoop+" select "+ term1+" "+ str(i)+" "+str(i+1)+"\n"
        #now extend for 8 additions
        text+= run+"curBitExt"+curLoop+" sextend "+run+"curBit"+curLoop+" "+str(maxNeededBits)+"\n"
        #now have to add curBit to itself numbits time
        for k in range(0,numBits):
            if (k == 0):
                text += run+"additionResult"+curLoop+chr(ord('a') +k)+" select "+run+"curBitExt"+curLoop+" 0" + " " + str(maxNeededBits) + "\n"
            else:
                text += run+"additionResult"+curLoop+chr(ord('a') +k)+" add " +run+"additionResult"+curLoop+chr(ord('a') +(k-1))\
                        + " "+run+"curBitExt"+curLoop+"\n"
        text+=  run+"additionResult"+curLoop+chr(ord('a') +numBits)+" and "+run+"additionResult"+curLoop+chr(ord('a') +(numBits-1))\
                +" "+run+"formatted"+term2+"\n"
        if(i==0):
            text+= run+"multiplicationResult"+curLoop+" sextend "+run+"additionResult"+curLoop+chr(ord('a') +numBits)\
                   +" "+str(maxNeededBits)+"\n"
        else:
            #shift left by i and
            #we add i zeroes then make sure same length for addition
            text+= run+"multiplicationResult"+curLoop+" add "+run+"multiplicationResult"+str(i-1)+\
                   " "+run + "additionResult" + curLoop + chr(ord('a') + numBits)+"\n"

    return text


def main():
    numBitsForInput = 3
    numTerms = 3
    outText = ""
    maxNeededMultiplicationBits = 2*numBitsForInput
    maxNeededSumBits = maxNeededMultiplicationBits+int(math.ceil(math.log(maxNeededMultiplicationBits,2)))
    #generate input block
    for i in range(0,numTerms):
        outText = outText + ".input party1Term"+str(i)+" 1 "+str(numBitsForInput)+"\n"
        outText = outText + ".input party2Term"+str(i)+" 2 "+str(numBitsForInput)+"\n"
    #then output block
    outText = outText + "\n"
    outText = outText + ".output finalResult"+str(numTerms-1)+"\n"
    outText = outText + "\n"

    #now nest the multiplications
    for i in range(0,numTerms):
        outText += generateMultiplicationforNBitNumbers("party1Term"+str(i),"party2Term"+str(i),
                                                        numBitsForInput,maxNeededMultiplicationBits,chr(ord('a')+i))
        if(i==0):
            outText +="finalResultInter0 select "+chr(ord('a') +i)+"multiplicationResult"+str(numBitsForInput-1)+" 0 "+\
                       str(maxNeededMultiplicationBits) +"\n"
            outText += "finalResult0 sextend "+"finalResultInter0 "+str(maxNeededSumBits)+"\n"
        else:
            outText += "finalResultInter"+str(i)+" select "+chr(ord('a') +i)+"multiplicationResult"+str(numBitsForInput-1)+" 0 "+\
                       str(maxNeededMultiplicationBits) +"\n"
            outText += "finalResultInterExt"+str(i)+" sextend finalResultInter"+str(i)+ " "+str(maxNeededSumBits)+"\n"
            outText += "finalResult"+str(i)+" add finalResult"+str(i-1)+" finalResultInterExt"+str(i)+"\n"

    with open("test.cir", 'w') as f:
        f.write(outText)

if __name__ == "__main__":
    main()