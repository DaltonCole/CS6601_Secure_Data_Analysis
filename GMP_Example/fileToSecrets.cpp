//File that takes a filename, secretFileToMake, and encrypted file to make
//generates a random bitset of equal length to file xor it with file
//and output bitset

#include "FileBitOperations.h"    
#include<ctime>
    
int main(int argc, char* argv[]) {
    
    //just check for existence of args
    if(argc<6){
        cout<<"Enter file for input, output, and secret"<<endl; 
        exit(1);
    }	
	int numTries = 30;
    std::ofstream outputFile("ResultFile.txt",std::ofstream::out);
    for(int i =100000; i<1000000; i+=100000){
		int totalTime =0;
		std:clock_t startTime = std::clock();
		for(int j=0;j<numTries;j++)
		{
			srand(time(NULL));
			//first get input file and its size
			FILE* inputFile = fopen(argv[1],"rb");
			if(inputFile==NULL){ cout<<"Error Opening "<<argv[1]<<endl; exit(1);}
			
			//now get output file
			char* newArray = new char[std::strlen(argv[4])+std::strlen(argv[2])+1];
			std::strcpy(newArray,argv[4]);
			std::strcat(newArray,argv[2]);
			FILE* outputFile = fopen(newArray,"w+b");
			if(outputFile==NULL){ cout<<"Error Opening "<<newArray<<endl; exit(1);}
			delete newArray;
			
			newArray = new char[std::strlen(argv[5])+std::strlen(argv[3])+1];
			std::strcpy(newArray,argv[5]);
			std::strcat(newArray,argv[3]);
			FILE* secretFile = fopen(newArray,"w+b");
			if(secretFile==NULL){ cout<<"Error Opening "<<newArray<<endl; exit(1);} 
			delete newArray;
			FILE* tempFile = fopen("temp","w+b");
			//some initializations to save time
			int bytes = i;
			mpz_t fileBits;
			mpz_init(fileBits);
			mpz_t share1;
			mpz_init(share1);
			mpz_t share2;
			mpz_init(share2);
			mpz_t backToBegin;
			mpz_init(backToBegin);
			char* buffer = new char[i];
			//initialize r_state
			gmp_randstate_t r_state;
			unsigned long seed = rand();
			gmp_randinit_default (r_state);
			gmp_randseed_ui(r_state, seed);
			do
			{
				//first initialize int with input file/////////////////
				readFile(inputFile,fileBits,buffer,bytes);
				//now generate the random number to xor////////////
				generateRandomXor(fileBits,share1,r_state);
				//now xor not gonna bother to write a function for this
				mpz_xor(share2,fileBits,share1);
				mpz_xor(backToBegin,share1,share2);
				//output to files
				outputBits(backToBegin,tempFile,bytes,buffer);
				outputBits(share1, secretFile, bytes, buffer);
				outputBits(share2, outputFile, bytes, buffer);
			
			}while(bytes==i);
			//deallocate memory
			delete buffer;
			mpz_clear(fileBits);
			mpz_clear(share2);
			mpz_clear(share1);
			gmp_randclear(r_state);
			fclose(inputFile);
			fclose(outputFile);
			fclose(secretFile);
			fclose(tempFile);
		}
		std:clock_t endTime = std:clock();
		double totalTime = double(startTime - endTime)/CLOCKS_PER_SEC;
		totalTime = totalTime/numTries;
		outputFile<<i<<" "<<totalTime<<"\n";
	}
	outputFile.close();
    return 0;
}