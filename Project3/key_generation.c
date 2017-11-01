// Generates a random number of the same order as "number"
// Result is saved in "result"
// rand number generator needs to be seeded before calling and state saved in "state"
void generateRandom(const mpz_t& number, mpz_t& result, gmp_randstate_t& r_state)
{   
    //make sure its the same size
    mpz_urandomm(result, r_state, number);
    return;
}

void generateRandomPrime_bits(const mp_bitcnt_t& bits, mpz_t& result, gmp_randstate_t& r_state)
{   
    mpz_urandomb(result, r_state, bits);

    // 1500 can be smaller to reduce run time (normal values are between 15 and 50)
    while(mpz_probab_prime_p(result, 1500) < 1) {
    	mpz_urandomb(result, r_state, bits);
    }

    return;
}

void generate_next_Prime(mpz_t& next_prime, const mpz_t& current_prime) {
	mpz_nextprime(next_prime, current_prime);

	return;
}

// Given k, generate a file with p, q and g
void key_generation(gmp_randstate_t& r_state) {
  // Generate p
  mpz_t p;
  mpz_init(p);
  mp_bitcnt_t k = 1024;

  generateRandomPrime_bits(k, p, r_state);
  gmp_printf("%Zd\n", p);

  // Generate q
  mpz_t q;
  mpz_init(q);

  generate_next_Prime(q, p);
  gmp_printf("%Zd\n", q);

  // Make 1
  mpz_t one;
  mpz_init(one);
  mpz_set_str(one, "1", 10);

  // p-1, q-i
  mpz_t p_minus_1;
  mpz_t q_minus_1;
  mpz_init(p_minus_1);
  mpz_init(q_minus_1);
  mpz_sub(p_minus_1, p, one);
  mpz_sub(q_minus_1, q, one);

  // Define n to be p*q
	mpz_t n;
  mpz_init(n);
  mpz_mul(n, p, q);

  // Define lambda
  mpz_t lambda;
  mpz_init(lambda);
  mpz_lcm(lambda, p_minus_1, q_minus_1);

  // Generate n^2
  mpz_t n_squared;
  mpz_init(n_squared);
  mpz_mul(n_squared, n, n);

  // Generate g as an alement of [0, n^2)
  mpz_t g;
  mpz_init(g);
  generateRandom(n_squared, g, r_state);
}

// 