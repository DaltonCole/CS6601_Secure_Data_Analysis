// Generates a random number of the same number of bits as "number"
// Result is saved in "result"
// rand number generator needs to be seeded before calling and state saved in "state"
void generateRandom(const mpz_t& number, mpz_t& result,gmp_randstate_t& r_state)
{   
    //make sure its the same size
    mpz_urandomb(result, r_state, mpz_sizeinbase(number,2));
    return;
}