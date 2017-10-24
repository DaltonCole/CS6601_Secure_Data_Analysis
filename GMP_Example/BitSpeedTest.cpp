//File that takes a filename, secretFileToMake, and encrypted file to make
//generates a random bitset of equal length to file xor it with file
//and output bitset

#include "FileBitOperations.h"    
#include <iostream.h>
#include <time.h>    
using namespace std;

int main(int argc, char* argv[]) {
	int numRuns=100;
	int bytes = argv[0];
	clock_t t1,t2;
	t1=clock();
    mpz_t randNum;
    mpz_init(randNum);
    //initialize r_state
    gmp_randstate_t r_state;
    unsigned long seed = rand();
    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, seed);
	for(int i=0;i<numRuns;i++)
	{
		mpz_urandomb(randNum,r_state,bytes);
	}
    t2=clock();
	float time= (static_cast<float>(t1)-static_cast<float>(t2))/CLOCKS_PER_SEC;
	time= time/numRuns;
	cout<<time<<endl;
    return 0;
}