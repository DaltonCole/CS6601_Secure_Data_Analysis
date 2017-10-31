/*
  Usage: Parameter one is p,q,g file input.
        Parameter two is key output file.
        Parameter three is vector u input file.
        Parameter four is vector v input file.
        parameter five is encrypted vector u output file.
        parameter six is encrypted vector v output file.




*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<gmp.h>

/*
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
*/

//have to assume a max line width for fgets, doubt he'll use more than 300000
const int MAX_LINE_WIDTH=30000;
//function to strip whitespace from a char array
//we use it to sanitize input file lines

char* deblank(char* input)                                
{
    char *output=input;
	int jCharacters = strlen(input);
    for (int i = 0, j = 0; i<strlen(input); i++,j++)
    {
		
        if (input[i]!=' '&&input[i]!='\n'&&input[i]!='\t'&&input[i]!=NULL)                                             
            output[j]=input[i];                                        
        else
		{
            j--;        
			jCharacters--;
		}
    }
	output[jCharacters]='\0';
    return output;                                                          
}

//function that reads in next line of file, and if successful
//imports it into an mpz_t, which is returned
//Assumes that file is already open in r Mode
void readFileLine(FILE* inputFile,mpz_t& nextVal)
{
	char* line = new char[MAX_LINE_WIDTH];
	size_t len = 0;
	mpz_init(nextVal);
	
	if(fgets(line,MAX_LINE_WIDTH,inputFile)!=NULL)//(read = getline(inputFile,line)&line, &len,inputFile)!=-1))
	{
		line = deblank(line);
		printf(line);
		printf("\n");
		mpz_set_str(nextVal,line,10);//(nextVal,length,1,1,0,0,line);//sizeof(int),0,0,line);
		//delete line;
	}
	delete line;
	return;
}

//simply opens and validates file, simple function but saves repeated code
FILE* openAndValidateFile(char* filePath, const char* openMode)
{
	FILE* fileToRead = fopen(filePath,openMode);
	if(fileToRead == NULL)
	{
		printf("Failed to open File ");
		printf("%s",filePath);
		exit(-1);
	}
	return fileToRead;
}
	
//assumes one int on every line of file,
//first reads number of terms then reads in mpz_t  for each line
//and returns resulting array, in addition numLines changed to 
//number of terms
mpz_t* readAllFileLines(char* fileName, int& numLines)
{
	FILE* fileToRead = openAndValidateFile(fileName,"r");
	//now count lines
	char* line = NULL;
	numLines = 0;
	/*
	size_t len = 0;
	ssize_t read;
	if((read = getline(&line, &len,fileToRead)!=-1))
	{
		numLines++;
	}*/
	int ch;
	while(EOF!=(ch=getc(fileToRead)))
	{
		if('\n' == ch)
		{
			numLines++;
		}
	}
	fseek(fileToRead,0,SEEK_SET);
	if(numLines==0)
	{
		fclose(fileToRead);
		return NULL;
	}
	//now allocate number array and fill with each line
	mpz_t* numbers = new mpz_t[numLines];
	int curLine=0;
	while(curLine<numLines)
	{
		readFileLine(fileToRead,numbers[curLine]);
		curLine++;
	}
	fclose(fileToRead);
	return numbers;
}

//simply outputs an mpz and newline to given file,
//assumes mpz allocated and file properly allocated
void outputMPZ(FILE* file, mpz_t& term)
{
	//file*, base, mpz_t
	mpz_out_str(file,10,term);
	fprintf(file,"\n");
	return;
}

//outputs list of mpz_t assuming it can open a file
void outputListMPZ(char* fileName, int& length, mpz_t terms[])
{

	FILE* outputFile = openAndValidateFile(fileName,"w");
	for(int i=0;i<length;i++)
	{
		printf("%d",i);
		printf("\n");
		outputMPZ(outputFile,terms[i]);
	}
	printf("done\n");
	return;
}

int main ( int argc, char *argv[] )
{
	int numLines;
	mpz_t* numbers = readAllFileLines("inputVector.txt",numLines);
	outputListMPZ("outputVector.txt",numLines,numbers);
	for(int i=0;i<numLines;i++)
	{
		mpz_clear(numbers[i]);
	}
	delete numbers;
	//delete numbers;
	//should validate args here
	/*
         pqgFile = argv[1];
         keyOutputFile = argv[2];
         vectorUFile = argv[3];
         vectorVFile = argv[4];
         vectorUOutputFile = argv[5];
         vectorVOutputFile = argv[6];

         publicKey_t public;
         privateKey_t private;
	*/
	
	//be sure to call mpz_clear on all mpz_t and freeing other memory
	return 0;
}
/*


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
*/
