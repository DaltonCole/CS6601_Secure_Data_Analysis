# Project 3 - Implement the Paillier Homomorphic PublicKey Encryption Scheme and the Secure Dot Product

* Programmers:
	* Adam Bowers
	* Sammie Bush
	* Dalton Cole

# System Requirements
	* Linux is the expected operating system
		* Can run on Windows, but path information will be different
	* GMP library must be present.
	


# How to Run
	* Usage: 
	    * paillier [p, q, and g file input] [Key output file] [~u file][E(~u) output file] [~v file] [E(~v) output file]

# Output


# Example
	```
	g++ paillier.c -lgmp
	./a.out pqgFile.txt keyOutput.txt vectorU.txt outputVector.txt vectorV.txt outputVectorV dotProduct.txt True
	```
