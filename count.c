/*Christopher Luikart
  CSE 5462 Lab 1
  Word Count Program*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

/*Reads up through the next instance of the searched word.
  Returns the number of bytes read
  infile is the read file pointer pointer
  word is the search word
  wordLen is the length of word
  foundWord is a boolean that determines whether an instance was found
  wordOverlap is the distance in characters between the current location of the file pointer
  and the second instance of the first character of word that was read
  */
int readNextInstance(FILE **infile, char *word, int wordLen, int *foundWord, int *wordOverlap){
	int numBytesProcessed = 0;
	char in[1];
	int wordIndex = 0;
	int enableCounting = 0;

	while(!feof(*infile) && !(*foundWord)){
		fread((void *) in, 1, 1, *infile);
		numBytesProcessed++;
		if(*in == (word[wordIndex] & 0xff)){
			wordIndex++;
			if(wordIndex == wordLen){
				*foundWord = 1;
			}
		}else{
			wordIndex = 0;
		}

		if(!enableCounting && wordIndex > 1 && *in == (word[0] & 0xff)){
			enableCounting = 1;
		}

		if(enableCounting){
			(*wordOverlap)++;
		}
	}

	return numBytesProcessed;
}

int main(int argc, char *argv[]){
	//Verify Input args
	if(argc != 4){
		printf("Incorrect Number of arguments\n");
		exit(0);
	}

	char *infile = argv[1];
	char *searchWord = argv[2];
	char *outfile = argv[3];

	//Initialize file size and word count
	int wordCount = 0;
	int fileLen = 0;

	//Open input file in binary
	FILE *readFile = fopen(infile, "rb");

	//Determine word size
	int size = 0;
	while(searchWord[size] != '\0'){
		size++;
	}

	//Initialize boolean instanceFound variable and backtrack distance variable
	int instanceFound;
	int backtrackDistance;
	do{
		//Reset vars
		instanceFound = 0;
		backtrackDistance = 0;
		//Get next instance of search word and update file length
		fileLen += readNextInstance(&readFile, searchWord, size, &instanceFound, &backtrackDistance);
		wordCount += instanceFound; //Update word Count

		//If necessary reset file pointer to passed characeter
		if(backtrackDistance > 0){
			fseek(readFile, -1*backtrackDistance, SEEK_CUR);
			//Adjust file length for double reading
			fileLen -= backtrackDistance;
		}

	}while(instanceFound == 1);

	//Fixes off by 1 err
	fileLen--;

	//Print Stats
	printf("Instances of %s found: %i\n", searchWord, wordCount);
	printf("Length of file: %i Bytes\n", fileLen);

	FILE *out = fopen(outfile, "w");

	fprintf(out, "Instances of %s found: %i\n", searchWord, wordCount);
	fprintf(out, "Length of file: %i Bytes\n", fileLen);

	//Close Files
	fclose(readFile);
	fclose(out);


}