/*
  Usage: Parameter one is p,q,g file input.
        Parameter two is key output file.
        Parameter three is vector u input file.
        parameter four is encrypted vector u output file.
        Parameter five is vector v input file.
        parameter six is encrypted vector v output file.

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
  int privateKeyComponentLines = 2;
  int numResultLines = 2;

  if(argc != 7) {
  	printf("Usage: ./a.out pqgFile.txt keyOutput.txt vectorU.txt outputVector.txt vectorV.txt outputVectorV\n");
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

  // To generate Random Number:
  /*
  mpz_t share1;
  mpz_init(share1);
  mpz_t fileBits;
  mpz_init(fileBits);
  generateRandom(fileBits, share1, r_state);
  */
  
  // Key Generation
  key_generation(r_state);


/*
  outputList(argv[2], privateKeyComponentLines, privateKeyList);
  outputList(argv[4], numVectorLines, vectorU);
  outputList(argv[6], numVectorLines, vectorV);
  outputList(argv[7], numResultLines, Results);
*/




//	outputListMPZ("outputVector.txt",numLines,numbers);
//	for(int i=0;i<numLines;i++)
//	{
//		mpz_clear(numbers[i]);
//	}
//	delete numbers;
  mpz_t p,q,g,lambda,n,u;  // n = p * q, g = number of bits for key



  mpz_init(p);
  mpz_init(q);
  mpz_init(g);
  mpz_init(lambda);
  mpz_init(n);
  mpz_init(u);


//  g = pqgFile[2];

  mpz_mul(n, pqgFile[0], pqgFile[1]);

  setPrivateKey(lambda, u, pqgFile[0], pqgFile[1], pqgFile[2], n);
  FILE* lambdaFile = openAndValidateFile(argv[2],"wb");
  outputMPZ(lambdaFile,lambda);
  outputMPZ(lambdaFile, u);
  fclose(lambdaFile);


	//delete numbers;
	//should validate args here
	/*
         pqgFile = argv[1];
         keyOutputFile = argv[2];
         vectorUFile = argv[3];
         vectorVFile = argv[4];
         vectorUOutputFile = argv[5];
         vectorVOutputFile = argv[6];

         publicKey_t public;
         privateKey_t private;
	*/

	//be sure to call mpz_clear on all mpz_t and freeing other memory
	return 0;
}
/*


void generateKeys(int bits, publicKey_t** public, privatekey_t** private)
{
  int k = bits;
  public.bits = k;





  public = ;
  private = ;
  return;

}
*/

void setPrivateKey(mpz_t& privateKey_lambda, mpz_t& privateKey_u, mpz_t p, mpz_t q, mpz_t g, mpz_t n)
{
  mpz_sub_ui(p, p, 1);
  mpz_sub_ui(q, q, 1);
  mpz_lcm(privateKey_lambda, p, q);


  mpz_t n_squared;
  mpz_init(n_squared);
  mpz_pow_ui(n_squared, n, 2);
  mpz_powm(privateKey_u, g, privateKey_lambda, n_squared);  // g^lambda MOD n^2
  //mpz_sub_ui(privateKey_u, privateKey_u, 1); // u - 1
  //mpz_div(privateKey_u, privateKey_u, n); // u / n  // Not sure on division. Function chosen in hopes that input is correct. OTherwise, could use mpz_div()

  return;

}

void encryption(mpz_t message, mpz_t g, mpz_t n)
{

  //mpz_powm(g, );//g^x
  //r^m
  //mod m^2



}


/*
void encryption(paillier_plaintext_t* res, paillier_pubkey_t* pub, paillier_plaintext_t* pt, paillier_get_rand_t get_rand)
{
  mpz_t r;
  gmp_randstate_t rand;
  mpz_t x;


  //pick random binding factor
  mpz_init(r);
 	init_rand(rand, get_rand, pub->bits / 8 + 1);
	do
		mpz_urandomb(r, rand, pub->bits);
	while( mpz_cmp(r, pub->n) >= 0 );

  //compute ciper text
  if( !res )
  {
    res = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
    mpz_init(res->c);
  }

  mpz_init(x);
	mpz_powm(res->c, pub->n_plusone, pt->m, pub->n_squared);
	mpz_powm(x, r, pub->n, pub->n_squared);

	mpz_mul(res->c, res->c, x);
	mpz_mod(res->c, res->c, pub->n_squared);

	mpz_clear(x);
	mpz_clear(r);
  gmp_randclear(rand);

	return res;





  mpz_t random;


  mpz_powm()
  mpz_powm()

  mpz_mul()
  mpz_mod()


  return;
}


void decryption()
{

  mpz_powm()
  mpz_sub_ui()
  mpz_div()
  mpz_mul()
  mpz_mod()



  return;
}

void dotProduct(vectorU, vectorV)
{


  //encrypt u vector

  //encrypt v vector




  return;
}
*/
