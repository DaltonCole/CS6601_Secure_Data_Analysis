#Python file to generate circuit file
#to compute secure dot product given number of terms
#and number of bits of each term

def generateMultiplicationforNBitNumbers(numBits):
    text = ""
    for i in range(0,numBits):
        text = text + " result"

def main():
    numBitsForInput = 64
    numTerms = 16
    outText = ""
    #generate input block
    for i in range(0,numTerms):
        outText = outText + ".input party1Term"+str(i)+" 1 "+str(numBitsForInput)+"\n"
        outText = outText + ".input party2Term"+str(i)+" 2 "+str(numBitsForInput)+"\n"
    #then output block
    outText = outText + "\n"
    outText = outText + ".output finalResult"+str(i+1)+"\n"
    outText = outText + "\n"

    #now nest the multiplications
    for i in range(0,numTerms):
        outText += generateMultiplicationforNBitNumbers(numBitsForInput)
        outText += "finalResult"+str(i+1)+" add finalResult"+str(i)+" multiplicationResult\n"



if __name__ == "__main__":
    main()