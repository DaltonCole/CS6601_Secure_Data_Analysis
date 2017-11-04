# Project 3 - Implement the Paillier Homomorphic PublicKey Encryption Scheme and the Secure Dot Product

* Programmers:
	* Adam Bowers
	* Sammie Bush
	* Dalton Cole

# System Requirements
	* Linux is the expected operating system
		* Can run on Windows, assuming you can compile it
	* GMP library must be present.
	


# How to Run(after compiled)
	* Usage: 
	    * paillier [p, q, and g file input] [Key output file] [~u file] [E(~u) output file] [~v file] [E(~v) output file] [Dot Product file] [Program creates p, q, g (True or False)]

# Output
	* None, necessary information is saved in the argument supplied files

# Example
	```
	g++ paillier.c -lgmp
	./a.out pqgFile.txt keyOutput.txt vectorU.txt outputVector.txt vectorV.txt outputVectorV dotProduct.txt True
	```
	* In the above example
		* pqgFile.txt: The program reads p, q, and g, in that order, from this file. If the last argument is True, this argument is still required, but will not be used
		* keyOUtput.txt: Lambda and mew are stored in this file after creation
		* vectorU.txt: The file containing the U vector. It is expected that each line is a new value in the vector
		* outputVector.txt: Stores the encrypted components of the U vector
		* vectorV.txt: The file containing the V vector. It is expected that each line is a new value in the vector
		* outputVectorV.txt: Stores the encrypted components of the V vector
		* dotProduct.txt: Last line stores the resulting dot product
		* True: The program generates p, q, and g. If this is False, then the pqgFile.txt is used to supply p, q, and g.
