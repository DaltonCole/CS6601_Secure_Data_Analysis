
// Max line width
const int MAX_LINE_WIDTH=30000;

// Sanitize input file lines
char* deblank(char* input)
{
    char *output=input;
	int jCharacters = strlen(input);
    for (int i = 0, j = 0; i<strlen(input); i++,j++)
    {

        if (input[i]!=' '&&input[i]!='\n'&&input[i]!='\t')
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

// Function that reads in next line of file, and if successful
// imports it into an mpz_t, which is returned
// Assumes that file is already open in r Mode
void readFileLine(FILE* inputFile,mpz_t& nextVal)
{
	char* line = new char[MAX_LINE_WIDTH];
	size_t len = 0;
	mpz_init(nextVal);

	if(fgets(line,MAX_LINE_WIDTH,inputFile)!=NULL)
	{
		line = deblank(line);
		mpz_set_str(nextVal,line,10);
	}
	delete [] line;
	return;
}

// Opens and validates file
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

// Assumes one int on every line of file,
// First reads number of terms then reads in mpz_t  for each line
// and returns resulting array, in addition numLines changed to
// number of terms
mpz_t* readAllFileLines(char* fileName, int& numLines)
{
	FILE* fileToRead = openAndValidateFile(fileName,"r");
	// Count lines
	char* line = NULL;
	numLines = 1;

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
	// Allocate number array and fill with each line
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

// Simply outputs an mpz and newline to given file,
// assumes mpz allocated and file properly allocated
void outputMPZ(FILE* file, mpz_t& term)
{
	mpz_out_str(file,10,term);
	fprintf(file,"\n");
	return;
}

// Outputs list of mpz_t assuming it can open a file
void outputListMPZ(char* fileName, int& length, mpz_t terms[])
{
	FILE* outputFile = openAndValidateFile(fileName, "w");
	for(int i=0;i<length;i++)
	{
		outputMPZ(outputFile,terms[i]);
	}
	return;
}
