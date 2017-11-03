/*
  Usage: Parameter one is p,q,g file input.
        Parameter two is key output file.
        Parameter three is vector u input file.
        parameter four is encrypted vector u output file.
        Parameter five is vector v input file.
        parameter six is encrypted vector v output file
		parameter seven is encrypted dot product and dot product file.

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

  if(argc != 8) {
  	printf("Usage: ./a.out pqgFile.txt keyOutput.txt vectorU.txt outputVector.txt vectorV.txt outputVectorV dotProduct.txt\n");
	exit(-1);
  }

	//mpz_t* numbers = readAllFileLines("inputVector.txt",numLines);
  mpz_t* pqgFile = readAllFileLines(argv[1], pqgLines);  //p is index 0, q index 1, g index 2
  mpz_t* vectorU = readAllFileLines(argv[3], numVectorLines);
  mpz_t* vectorV = readAllFileLines(argv[5], numVectorLines);

  // Generate random stae for gmp
  srand(time(NULL));
  gmp_randstate_t r_state;
  unsigned long seed = rand();
  gmp_randinit_default (r_state);
  gmp_randseed_ui(r_state, seed);

  
  // Key Generation, uncomment to see our code, but will replace pqg config used
  /*
  
  mpz_init(p);
  mpz_init(q);
  mpz_init(g);
  mpz_init(lambda);
  mpz_init(n);
  mpz_init(u);

  mp_bitcnt_t k = 1024;
  key_generation(r_state, k);

  mpz_t p,q,g,lambda,n,u;  // n = p * q, g = number of bits for key
  mpz_t decryptionResult;
  */
  
  // To generate Random Number:
  /*
  mpz_t share1;
  mpz_init(share1);
  mpz_t fileBits;
  mpz_init(fileBits);
  generateRandom(fileBits, share1, r_state);
  */

  mpz_t n, lambda,u,decryptionResult;
  mpz_init(lambda);
  mpz_init(n);
  mpz_init(u);
  
//  g = pqgFile[2];

  mpz_mul(n, pqgFile[0], pqgFile[1]);
  
  setPrivateKey(lambda, u, pqgFile[0], pqgFile[1], pqgFile[2], n);
  FILE* lambdaFile = openAndValidateFile(argv[2],"w");
  outputMPZ(lambdaFile,lambda);
  outputMPZ(lambdaFile, u);
  fclose(lambdaFile);

  mpz_t* vectorUEncryption = new mpz_t[numVectorLines];
  for(int i = 0; i<numVectorLines; i++)
  {
	printf("Current Term ");
	gmp_printf("%Zd\n",vectorU[i]);
    encryption(vectorUEncryption[i], vectorU[i], pqgFile[2], n, r_state);
	decryption(decryptionResult,vectorUEncryption[i],lambda,u,n);
	//ecryption(mpz_t& result, mpz_t& cipherText, mpz_t& lambda, mpz_t& u, mpz_t& n)
  }
  outputListMPZ(argv[4],numVectorLines,vectorUEncryption);
  
  mpz_t* vectorVEncryption = new mpz_t[numVectorLines];
  for(int i = 0; i<numVectorLines; i++)
  {
	printf("Current Term ");
	gmp_printf("%Zd\n",vectorV[i]);
    encryption(vectorVEncryption[i], vectorV[i], pqgFile[2], n, r_state);
	decryption(decryptionResult,vectorVEncryption[i],lambda,u,n);
  }
  outputListMPZ(argv[6],numVectorLines,vectorVEncryption);

  //compute secure dot product and unencrypt
  mpz_t* dotProduct = new mpz_t[dotProductComponents];
  computeSecureDotProdcut(dotProduct[0], vectorVEncryption, vectorU,n,numVectorLines);//,lambda,u);
  decryption(dotProduct[1],dotProduct[0],lambda,u,n);
  outputListMPZ(argv[7],dotProductComponents,dotProduct);
  //free memory
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
  mpz_lcm(privateKey_lambda, p, q);


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
  mpz_mod(privateKey_u,l_result,n);
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
  mpz_mod(result, result, n_squared);//mod m^2
  
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
	
	printf("Decrypted to get ");
	gmp_printf("%Zd\n",result);
	mpz_clear(n_squared);
	mpz_clear(c_pow);
}

//math function for l(x) = x-1/n
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



void computeSecureDotProdcut(mpz_t& result, mpz_t* encryptedVector, mpz_t* unencryptedVector, mpz_t& n,int length)//,mpz_t& lambda, mpz_t& u)
{
	mpz_t individualResults,n_squared,decrypt;
	mpz_init(result);
	mpz_init(n_squared);
	mpz_mul(n_squared,n,n);
	mpz_init(individualResults);
    mpz_set_si(result,1);
	for(int i=0; i<length;i++)
	{
		mpz_powm(individualResults,encryptedVector[i],unencryptedVector[i],n_squared);
		//decryption(decrypt,individualResults,lambda,u,n);
		mpz_mul(result,result,individualResults);
	}
	mpz_mod(result,result,n_squared);
	mpz_clear(individualResults);
	return;
}
