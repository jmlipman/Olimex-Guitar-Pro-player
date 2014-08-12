#include <avr/io.h>
#include <util/delay.h>
#include "notes.h" //Frequencies
#include "playMax.h" //Amount of notes the player will play
#include "playMelodies.h" //Notes of the melody that will be played
#include "playTempo.h" //Duration of each note


#define SPEED 0.7 //Music speed
#define OFFSET 0 //Notes offset. 0 = normal. 1000 = more high-pitched


int get_loops(double, double);
int get_time_delay(double);
void play_note(double,double);

int main(void)
{

DDRE|=1<<PE4|1<<PE5; //buzzer pings out
DDRA&=~(1<<PA0); // B1 in
PORTA|=1<<PA0; // B1 pull up resistor


while(1){


	while(PINA&1<<PA0); //button up
	while(!(PINA&1<<PA0)); //button down

	//Loop to play all the notes and make the melody
	for(int a=0;a<max_mel;a++){
		play_note(mel[a]+OFFSET,temp[a]);
	}
	
}

}


/**
 * Obtain the amount of loops the loop must do.
 * Depending on the frequency and duration of the note, a certain amount of
 * loops must be done.
 * 
 * @param freq it is wanted to be played.
 * @param tempo (duration) of the note.
 * @return number of loops
*/
int get_loops(double freq, double tempo) {
return ((int) ((262*freq)/340)*tempo*SPEED);
}

/**
 * Time of the delay.
 * 
 * @param frequency
 * @return amount of microseconds the program will wait.
*/
int get_time_delay(double freq){
	return ((1/freq)/2)*1000000;
}

/**
 * This function will play a note given a frequency and the duration itself.
 * 
 * @param frequency it is wanted to be played.
 * @param tempo (duration) of that note.
*/
void play_note(double freq, double tempo)
{
	if(freq!=0) { //Let's play!
		int time = get_time_delay(freq);
		int loops = get_loops(freq,tempo);
		for (int i=0;i<loops; i++){
			PORTE|=1<<PE4; //5V
			for(int j=0; j<time;j++) _delay_us(1);
			PORTE&=~(1<<PE4); //0
			for(int j=0; j<time;j++) _delay_us(1);

		}
	} else { //Silence
		int time = tempo*1000*SPEED;
		for(int j=0; j<time;j++) _delay_ms(1);
	}

}
