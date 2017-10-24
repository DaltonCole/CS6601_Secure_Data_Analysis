//functions in common in both encrypting and decrypting files

#ifndef FILE_BIT_OPERATIONS_H
#define FILE_BIT_OPERATIONS_H

#include <iostream>
#include <gmpxx.h>
//#include <stdlib.h>
#include <time.h>   
#include <fstream>
#include <stdio.h>
//#include <cstdio>
using namespace std;

const int BLOCKSIZE = 100000;

//function that takes input file pointer and reades either bytes or remaining file bit size, whichever is smaller
//Pre: all parameters properly initialized and allocated before calling
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
    return;
}

//Pre:rand number generator needs to be seeded before calling and
//    all parameters properly initialized and allocated before calling
void generateRandomXor(const mpz_t& number, mpz_t& result,gmp_randstate_t& r_state)
{   
    //make sure its the same size
    mpz_urandomb(result,r_state,mpz_sizeinbase(number,2));
    return;
}

//finally output file to desired file name, buffer assumed to be allocated as blockSize already
//Pre: all parameters properly initialized and allocated before calling
void outputBits(const mpz_t& outputNumber, FILE* outputFile, const int& length, char* buffer)
{
    //export into char*,number of bytes used, order 1 for most significant word first, 
    //export type byte size, endian, nails, gmp to read from
    size_t* size=new size_t;
	mpz_export(buffer,size,1,sizeof(char),0, 0, outputNumber);
    if(size==NULL){cout<<"Size is null"<<endl;}
    else if(*size!=(size_t)length){
        //cout<<"error with export size "<<*size<<" "<<length;
        //need to add leading zeroes
        int diff = (size_t) length-*size;
        char* zeroes = new char[diff];
        fwrite(zeroes,sizeof(char),diff,outputFile);
        delete zeroes;
        //cout<<" "<<diff<<endl;
       /* for(int k=length-1; k-diff>=0;k--){
            buffer[k] = buffer[k-diff];
        }
        for(int k=0;k<diff;k++){
            buffer[k] = 0;
        }*/
    }
    delete size;
    //write input number of bits to file
    fwrite(buffer,sizeof(char),length,outputFile);
    //delete size;
    return;
}

#endif