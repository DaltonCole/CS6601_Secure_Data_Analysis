/*
  Programmed by: Adam Bowers, Sammie Bush, Dalton Cole
  CS6601 Project 3 11/3/2017
  Description: Use gmp to implement pailler encryption/decryption as well as secure dot product
               via properties of pailler encryption
  Usage: Parameter one is p,q,g file input.
        Parameter two is key output file.
        Parameter three is vector u input file.
        parameter four is encrypted vector u output file.
        Parameter five is vector v input file.
        parameter six is encrypted vector v output file
		    parameter seven is encrypted dot product and dot product file.
        parameter eight is optional, we generate our own keys

Example:
./a.out pqgFile.txt keyOutput.txt vectorU.txt outputVectorU.txt vectorV.txt outputVectorV.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <time.h> // Random number for state
#include "paillier.h"
#include "paillier_io.c"
#include "key_generation.c"


int main ( int argc, char *argv[] )
{
  int numVectorLines;
  int pqgLines = 3;
  int dotProductComponents = 2;
  int numResultLines = 2;

  if(argc < 8) {
  	printf("Usage: ./a.out pqgFile.txt keyOutput.txt vectorU.txt outputVector.txt vectorV.txt outputVectorV dotProduct.txt <True, False: Generate our own Keys>\n");
	  exit(-1);
  }

  // Generate random state for gmp
  srand(time(NULL));              // Seed random number generator
  gmp_randstate_t r_state;        // Create state
  unsigned long seed = rand();
  gmp_randinit_default (r_state);
  gmp_randseed_ui(r_state, seed); // Seed random state
    
  // Declair variables
  mpz_t p;
  mpz_t q;
  mpz_t g;
  mpz_t lambda;
  mpz_t n;
  mpz_t u;
  mpz_t decryptionResult;

  // Initalize variables
  mpz_init(p);
  mpz_init(q);
  mpz_init(g);
  mpz_init(lambda);
  mpz_init(n);
  mpz_init(u);
  mpz_init(decryptionResult);

  // If we are generating keys,
  if(argc == 9 && argv[8][0] == 'T') {
    mp_bitcnt_t k = 1024;
    key_generation(r_state, k);

    mpz_t p,q,g,lambda,n,u;  // n = p * q, g = number of bits for key
    argv[1] = "p_q_g.txt"; // p, q, g file
  }

  /* --- Read in inputs from file --- */
  mpz_t* pqgFile = readAllFileLines(argv[1], pqgLines);  
  // U Vector
  mpz_t* vectorU = readAllFileLines(argv[3], numVectorLines);
  // V Vector
  mpz_t* vectorV = readAllFileLines(argv[5], numVectorLines);

  // Multiply p * q to get n
  mpz_mul(n, pqgFile[0], pqgFile[1]);
  //get and output private key
  setPrivateKey(lambda, u, pqgFile[0], pqgFile[1], pqgFile[2], n);
  FILE* lambdaFile = openAndValidateFile(argv[2],"w");
  outputMPZ(lambdaFile,lambda);
  outputMPZ(lambdaFile, u);
  fclose(lambdaFile);
  //encrypt u vector
  mpz_t* vectorUEncryption = new mpz_t[numVectorLines];
  for(int i = 0; i<numVectorLines; i++)
  {
    encryption(vectorUEncryption[i], vectorU[i], pqgFile[2], n, r_state);
  	decryption(decryptionResult,vectorUEncryption[i],lambda,u,n);
  }
  outputListMPZ(argv[4],numVectorLines,vectorUEncryption);
  //encrypt v vector
  mpz_t* vectorVEncryption = new mpz_t[numVectorLines];
  for(int i = 0; i<numVectorLines; i++)
  {
    encryption(vectorVEncryption[i], vectorV[i], pqgFile[2], n, r_state);
  	decryption(decryptionResult,vectorVEncryption[i],lambda,u,n);
  }
  outputListMPZ(argv[6],numVectorLines,vectorVEncryption);

  // Compute secure dot product and unencrypt
  mpz_t* dotProduct = new mpz_t[dotProductComponents];
  //assume party u will run in model so use v encrypted components and u's unecrypted components
  computeSecureDotProdcut(dotProduct[0], vectorVEncryption, vectorU,n,numVectorLines);//,lambda,u);
  decryption(dotProduct[1],dotProduct[0],lambda,u,n);
  outputListMPZ(argv[7],dotProductComponents,dotProduct);
  
  /* --- Free up memory --- */
  mpz_clear(lambda);
  mpz_clear(u);
  mpz_clear(n);
  mpz_clear(decryptionResult);
  for(int i=0;i<dotProductComponents;i++)
  {
    mpz_clear(dotProduct[i]);
  }
  for(int i=0;i<pqgLines;i++)
  {
	  mpz_clear(pqgFile[i]);
  }
  
  for(int i=0;i<numVectorLines;i++)
  {
	mpz_clear(vectorU[i]);
	mpz_clear(vectorV[i]);
	mpz_clear(vectorUEncryption[i]);
	mpz_clear(vectorVEncryption[i]);
  }
  
  delete[] dotProduct;
  delete[] pqgFile;
  delete[] vectorU;
  delete[] vectorV;
  delete[] vectorUEncryption;
  delete[] vectorVEncryption;
  
  return 0;
}

void setPrivateKey(mpz_t& privateKey_lambda, mpz_t& privateKey_u, mpz_t p, mpz_t q, mpz_t& g, mpz_t& n)
{
  mpz_t l_result;
  mpz_sub_ui(p, p, 1);
  mpz_sub_ui(q, q, 1);
  mpz_lcm(privateKey_lambda, p, q);//lamda = lcm(p-1,q-1)


  mpz_t n_squared;
  mpz_init(n_squared);
  mpz_pow_ui(n_squared, n, 2);
  mpz_powm(privateKey_u, g, privateKey_lambda, n_squared);  // g^lambda MOD n^2
  
  L_function(l_result,privateKey_u,n);
  if(!mpz_invert(l_result,l_result,n))
  {
	printf("Failed to invert L val\n");
	exit(-1);
  }
  mpz_mod(privateKey_u,l_result,n);//mew = L(g^lamda mod n^2)^-1 mod n
  //mpz_div(privateKey_u, privateKey_u, n); // u / n  // Not sure on division. Function chosen in hopes that input is correct. OTherwise, could use mpz_div()

  return;

}

void encryption(mpz_t& result, mpz_t& message, mpz_t& g, mpz_t& n, gmp_randstate_t& r_state)
{
	
  mpz_t randNum,n_squared,g_toX, n_plusOne, gcd,one;
  mpz_init(result);
  mpz_init(n_squared);
  mpz_init(gcd);
  mpz_init(g_toX);
  mpz_init(randNum);
  mpz_init(one);
  mpz_set_si(one,1);

  //gcd r, n must equal one so keep getting random numbers till that happens
  //only using while loop because in large fields this is extremely unlikely 
  //to happen
  do
  {
	generateRandom(n, randNum, r_state);
	mpz_gcd(gcd,randNum,n);
  }while(mpz_cmp(gcd,one)!=0);
  mpz_pow_ui(n_squared, n, 2); // n^2
  mpz_powm(g_toX, g, message, n_squared);//g^x

  mpz_powm(randNum, randNum, n, n_squared);//r^n
  
  mpz_mul(result, randNum, g_toX); // g^x * r^n
  mpz_mod(result, result, n_squared);//mod n^2
  //E(m) = g^m * r^n mod n^2
  mpz_clear(one);
  mpz_clear(gcd);
  mpz_clear(n_squared);
  mpz_clear(randNum);
  mpz_clear(g_toX);


  return;

}


void decryption(mpz_t& result, mpz_t& cipherText, mpz_t& lambda, mpz_t& u, mpz_t& n)
{
	mpz_t n_squared, c_pow,l_result;
	mpz_init(n_squared);
	mpz_init(c_pow);
	mpz_init(result);
		
	mpz_mul(n_squared,n,n);
	//c^lambda mod n^2
	mpz_powm(c_pow,cipherText,lambda,n_squared);
	
	//now get l function val
	L_function(l_result,c_pow,n);
	//L(c)*u
	mpz_mul(result,l_result,u);
	//mod(n)
	mpz_mod(result,result,n);
	
	//printf("Decrypted to get ");
	//gmp_printf("%Zd\n",result);
	mpz_clear(n_squared);
	mpz_clear(c_pow);
}

void L_function(mpz_t& result, mpz_t& x, mpz_t& n)
{
	mpz_t x_subOne;
	mpz_init(result);
	mpz_init(x_subOne);
	//subtract 1 from x
	mpz_sub_ui(x_subOne, x, 1); // u - 1
	//need quotient division
	mpz_tdiv_q(result,x_subOne,n);
	mpz_clear(x_subOne);
}



void computeSecureDotProdcut(mpz_t& result, mpz_t* encryptedVector, mpz_t* unencryptedVector, mpz_t& n,int length)
{
	mpz_t individualResults,n_squared,decrypt;
	mpz_init(result);
	mpz_init(n_squared);
	mpz_mul(n_squared,n,n);
	mpz_init(individualResults);
    mpz_set_si(result,1);
	for(int i=0; i<length;i++)
	{
		//E(m*c) = E(m)^c mod n^2
		mpz_powm(individualResults,encryptedVector[i],unencryptedVector[i],n_squared);
		mpz_mul(result,result,individualResults);
	}
	//E(m1+m2) = E(m1)+E(m2)
	mpz_mod(result,result,n_squared);
	mpz_clear(individualResults);
	return;
}
