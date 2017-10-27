/*
  Usage: Parameter one is p,q,g file input.
        Parameter two is key output file.
        Parameter three is vector u input file.
        Parameter four is vector v input file.
        parameter five is encrypted vector u output file.
        parameter six is encrypted vector v output file.




*/

#include <stdio.h>
#include <string.h>
#include <gmp.h>

typedef struct  //plaintext
{
  mpz_t m;
} plainText_t;

typedef struct //cipher text
{
  mpz_t m;
} cipherText_t;


typedef struct //public key
{
  int bits;
  mpz_t n;
} publicKey_t;

typedef struct //private key
{
  mpz_t lambda;
} privateKey_t;





int main ( int argc, char *argv[] )
{
         pqgFile = argv[1];
         keyOutputFile = argv[2];
         vectorUFile = argv[3];
         vectorVFile = argv[4];
         vectorUOutputFile = argv[5];
         vectorVOutputFile = argv[6];

         publicKey_t public;
         privateKey_t private;


}



void generateKeys(int bits, publicKey_t** public, privatekey_t** private)
{
  int k = bits;
  public.bits = k;





  public = ;
  private = ;
  return;

}

void encryption()
{
  mpz_t random;


  mpz_powm()
  mpz_powm()

  mpz_mul()
  mpz_mod()



}


void decryption()
{

  mpz_powm()
  mpz_sub_ui()
  mpz_div()
  mpz_mul()
  mpz_mod()




}

void dotProduct(vectorU, vectorV)
{


  //encrypt u vector

  //encrypt v vector

  



}
