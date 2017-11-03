#ifndef PAILLIER_H
#define PAILLIER_H

void setPrivateKey(mpz_t& privateKey_lambda, mpz_t& privateKey_u, mpz_t p, mpz_t q, mpz_t& g, mpz_t& n);


void encryption(mpz_t& result, mpz_t& message, mpz_t& g, mpz_t& n, gmp_randstate_t& r_state);


void decryption(mpz_t& result, mpz_t& cipherText, mpz_t& lambda, mpz_t& u, mpz_t& n);

void computeSecureDotProdcut(mpz_t& result, mpz_t* encryptedVector, mpz_t* unencryptedVector, mpz_t& n, int length);//,mpz_t& lambda, mpz_t& u);

void L_function(mpz_t& result, mpz_t& x, mpz_t& n);

#endif