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

//////////////////////////////////////////////////////////////////////
/// @fn key_generation(gmp_randstate_t& r_state, const mp_bitcnt_t & k)
/// @brief Generates p, q, and g and saves those values to a file
///  called "p_q_g.txt"
/// @pre GMP State has already been generated
/// @param gmp_randstate_t& r_state: The randomly generated state
/// @param mp_bitcnt_t k: The max number of bits for p and q
////////////////////////////////////////////////////////////////////// 
void key_generation(gmp_randstate_t& r_state, const mp_bitcnt_t & k) {
  // Generate p
  mpz_t p;
  mpz_init(p);
  generateRandomPrime_bits(k, p, r_state);
  //gmp_printf("%Zd\n", p); // Example on how to print gmp_ints

  // Generate q
  mpz_t q;
  mpz_init(q);
  generate_next_Prime(q, p);
  //gmp_printf("%Zd\n", q);

  // Define n to be p*q
	mpz_t n;
  mpz_init(n);
  mpz_mul(n, p, q);

  // Generate n^2
  mpz_t n_squared;
  mpz_init(n_squared);
  mpz_mul(n_squared, n, n);

  // Generate g as an element of [0, n^2)
  mpz_t g;
  mpz_init(g);
  generateRandom(n_squared, g, r_state);

  /*
  // Make 1
  mpz_t one;
  mpz_init(one);
  mpz_set_str(one, "1", 10);
  // Make -1
  mpz_t n_one;
  mpz_init(n_one);
  mpz_set_str(n_one, "-1", 10);

  // p-1, q-i
  mpz_t p_minus_1;
  mpz_t q_minus_1;
  mpz_init(p_minus_1);
  mpz_init(q_minus_1);
  mpz_sub(p_minus_1, p, one);
  mpz_sub(q_minus_1, q, one);

  // Define lambda
  mpz_t lambda;
  mpz_init(lambda);
  mpz_lcm(lambda, p_minus_1, q_minus_1);

  // Mew
  mpz_t L;
  mpz_init(L);

  mpz_powm(L, g, lambda, n_squared);
  mpz_sub(L, L, one);
  mpz_cdiv_q(L, L, n);
  mpz_powm(L, L, n_one, n);
  */

  // Save p, q, and g to a file
  FILE *f = fopen("p_q_g.txt", "w");
  if(f == NULL) {
  	printf("ERROR opening p_q_g.txt\n");
  	exit(1);
  }
  mpz_out_str(f, 10, p);
  fprintf(f, "\n");
  mpz_out_str(f, 10, q);
  fprintf(f, "\n");
  mpz_out_str(f, 10, g);
  fprintf(f, "\n");

  fclose(f);

  return;
}