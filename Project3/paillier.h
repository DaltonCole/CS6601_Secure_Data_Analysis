/*
  Programmed by: Adam Bowers, Sammie Bush, Dalton Cole
  CS6601 Project 3 11/3/2017
  Description: //header for functions in pailler system
*/
#ifndef PAILLIER_H
#define PAILLIER_H

//function to generate lamda and mew given p, q, g, and n
//where p & q are prime and g is a generator for the field
//PRE: p,q, g and are initialized with values specified above
void setPrivateKey(mpz_t& privateKey_lambda, mpz_t& privateKey_u, mpz_t p, mpz_t q, mpz_t& g, mpz_t& n);

//encrypt a message in field n for generator g
//PRE: message, g, and n are initialized message<n, r_state seeded
void encryption(mpz_t& result, mpz_t& message, mpz_t& g, mpz_t& n, gmp_randstate_t& r_state);

//given E(m) compute m and store in result, given private key lamda, mew, and field size n
//PRE: cipherText, lambda, u, and n initialized and they follow properties of pailler encryption(size, prime, etc)
void decryption(mpz_t& result, mpz_t& cipherText, mpz_t& lambda, mpz_t& u, mpz_t& n);

//math function for l(x) = x-1/n
//PRE: encryptedVector, Unencrypted Vector, n initialized, length = num terms allocated for both vectors
// every term in vector<n
void computeSecureDotProdcut(mpz_t& result, mpz_t* encryptedVector, mpz_t* unencryptedVector, mpz_t& n, int length);//,mpz_t& lambda, mpz_t& u);

// compute secure dot product via pailler encryption by using multiplication by a constant and homomorphic addition
//PRE: x and n initialized and x<n
void L_function(mpz_t& result, mpz_t& x, mpz_t& n);

#endif