/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//todo look at word size of my 64bit machine and see how blocksize equal to that works(time wise)
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <gmpxx.h>
//#include <stdlib.h>
#include <time.h>   
#include <fstream>
#include <stdio.h>
//#include <cstdio>
using namespace std;

//function that takes input file pointer and reades either blockSize or remaining file bit size, whichever is smaller
void readFile(FILE* inputFile, mpz_t& fileBits, char* buffer,int& bytes){
    if (buffer == NULL) 
	{
		cout<<"Memory error allocating buffer"<<endl;
		exit (2);
	}
	int status = fread (buffer,1,bytes,inputFile);
	if (status == 0)
	{
		cout<<"Reading error"<<endl;
		exit (3);
	}
    else if(status!=bytes)
    {
        bytes = status;
    }
    // importing buffer into mpz intger 'fileBits'
	mpz_import(fileBits,bytes,1,sizeof(buffer[0]),0, 0, buffer);
}

//rand number generator needs to be seeded before calling
void generateRandomXor(const mpz_t& number, mpz_t& result,gmp_randstate_t& r_state)
{   
    //make sure its the same size
    mpz_urandomb(result,r_state,mpz_sizeinbase(number,2));
    return;
}

//finally output file to desired file name, buffer assumed to be allocated as blockSize already
void outputBits(const mpz_t& outputNumber, FILE* outputFile, const int& length, char* buffer)
{
    //export into char*,number of bytes used, order 1 for most significant word first, 
    //export type byte size, endian, nails, gmp to read from
	mpz_export(buffer,NULL,1,sizeof(char),0, 0, outputNumber);
    //write input number of bits to file
    fwrite(buffer,sizeof(char),length,outputFile);
    return;
}
    
int main(int argc, char* argv[]) {
    
    //just check for existence of args
    if(argc<3){
        cout<<"Enter file for input and output"<<endl; 
        exit(1);
    }
    srand(time(NULL));
    //first get input file and its size
    FILE* inputFile = fopen(argv[1],"rb");
    if(inputFile==NULL){ cout<<"Error Opening "<<argv[1]<<endl; exit(1);}
    
    //now get output file
    FILE* outputFile = fopen(argv[2],"w+b");
    if(outputFile==NULL){ cout<<"Error Opening "<<argv[2]<<endl; exit(1);}
    //how many bytes to read at a time
    int blockSize = 32768;
    //some initializations to save time
    int bytes = blockSize;
    mpz_t fileBits;
    mpz_init(fileBits);
    mpz_t share1;
    mpz_init(share1);
    mpz_t share2;
    mpz_init(share2);
    mpz_t backToBegin;
    mpz_init(backToBegin);
    char* buffer = new char[blockSize];
    //initialize r_state
    gmp_randstate_t r_state;
    unsigned long seed = rand();
    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, seed);
    //no output file
    do
    {
        //first initialize int with input file/////////////////
        readFile(inputFile,fileBits,buffer,bytes);
        //now generate the random number to xor////////////
        generateRandomXor(fileBits,share1,r_state);
        //now xor not gonna bother to write a function for this
        mpz_xor(share2,fileBits,share1);
        mpz_xor(backToBegin,share2,share1);
        outputBits(backToBegin, outputFile, bytes, buffer);
    }while(!feof(inputFile)&&bytes==blockSize);
    delete buffer;
    mpz_clear(fileBits);
    mpz_clear(share2);
    mpz_clear(share1);
    mpz_clear(backToBegin);
    gmp_randclear(r_state);
    fflush(outputFile);
    fflush(inputFile);
    fclose(inputFile);
    fclose(outputFile);
    /*
    //free(buffer);
    */
    return 0;
}