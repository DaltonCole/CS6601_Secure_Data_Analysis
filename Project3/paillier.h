#ifndef PAILLIER_H
#define PAILLIER_H



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

#endif


void setPrivateKey(mpz_t& privateKey_lambda, mpz_t& privateKey_u, mpz_t p, mpz_t q, mpz_t g, mpz_t n);
