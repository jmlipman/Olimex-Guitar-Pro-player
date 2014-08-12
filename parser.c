#include<stdio.h>
#define MAX_LINE 100

int contains(char[], char[]);
char getNote(char[]);
char getOctave(char[]);
double getDuration(char[]);

int main(void){
	FILE *file,*playMax,*playMelodies,*playTempo;
	int insideNote=0,noteCounter=-1;
	int note,octave,duration;
	char line[MAX_LINE];

	int flagSilence;

	char input[10];
	double tmpdouble;

	char cMax[] = "#define max_mel ";
	char cMelodies[] = "int mel[max_mel] = {";
	char cTempo[] = "double temp[max_mel] = {";

	//Preparing the fileMax, playMelodies, playTempo
	playMax=fopen("playMax.h","w");
	playMelodies=fopen("playMelodies.h","w");
	playTempo=fopen("playTempo.h","w");
	fputs(cMax,playMax);
	fputs(cMelodies,playMelodies);
	fputs(cTempo,playTempo);

	file=fopen("xml.xml","r");

	while(fgets(line,MAX_LINE,file)!=NULL)
	{
		//We found a note. Let's turn some flags on.
		if(contains(line,"<note>")){
			//We are inside a note
			insideNote=1;
			//By default, we will write a silence, unless we find a "step"
			flagSilence=1;
			noteCounter++;
			if(noteCounter>0) { //The first time we don't write a "," in the beginning of the file
				fputs(",",playMelodies);
				fputs(",",playTempo);
			}

		}

		//End of the note
		if(contains(line,"</note>") && insideNote){
			insideNote=0;
		}

		//We found the note that will be played
		if(contains(line,"<step>") && insideNote){
			//Let's get the note (A-G)
			input[0]=getElement(line);
			input[1]='\0';
			fputs(&input[0],playMelodies);
			//As we found the step, there is no silence
			flagSilence=0;
		}

		if(contains(line,"<octave>") && insideNote){
			//Let's do the same with the octave
			input[0]=getElement(line);
			input[1]='\0';
			fputs(input,playMelodies);
		}

		if(contains(line,"<duration>") && insideNote){
			//If the flag is still on, there is a silence to be written.
			if(flagSilence){
				fputs("NO",playMelodies);
			}
			//Write the duration
			tmpdouble=getDuration(line);
			sprintf(input,"%.2f",tmpdouble);
			fputs(input,playTempo);
		}
	
	}
	//Amount of notes
	sprintf(input,"%d",noteCounter);

	//Final writtings...
	fputs("};",playMelodies);
	fputs("};",playTempo);
	fputs(input,playMax);

	//Close files
	fclose(file);
	fclose(playMax); fclose(playMelodies); fclose(playTempo);

	return 0;
}

/**
 * Retrieves the note or octave.
 *
 * @param string to be parsed.
 * @return char with found note or octave.
 */ 
char getElement(char str[]){
	int i;
	for(i=0;str[i]!='>' && str[i]!='\0';i++);
	return str[i+1]; //ascii capital letters start
}

/**
 * Retrieves the duration of the note.
 *
 * @param string to be parsed.
 * @return duration (already formatted)
 */ 
double getDuration(char str[]){
	int i,charCounter=0;
	char c[5]; 
	double d=0;

	for(i=0;str[i]!='>' && str[i]!='\0';i++);
	i++;

	for(;str[i]!='<' && str[i]!='\0';i++)
		c[charCounter++]=str[i];

	c[charCounter]='\0';
	
	//Transformation from array to double
	for(i=charCounter-1;i>=0;i--){
		d=d+(c[i]-48)*pow(10,(charCounter-i-1));
	}

	//required format
	return (double) (d/2048);
}

/**
 * This function return 0 or 1 depending on whether string2 is contained on string1
 *
 * @param string1 where string2 will be searched.
 * @param string2 which will be searched in string1.
 * @return 1 or 0.
 */ 
int contains(char str1[], char str2[]){
	int i,j;
	for(i=0;str1[i]!='\0';i++){
		if(str1[i]==str2[0]){
			for(j=1;str1[i+j]==str2[j] && str2[j]!='\0';j++);
			if(str2[j]=='\0') return 1;
		}
	}
	return 0;
}
