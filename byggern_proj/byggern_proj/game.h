/*
 * game.h
 *
 * Created: 16.11.2022 15:43:56
 *  Author: andrea7
 */ 


#ifndef GAME_H_
#define GAME_H_

char* title = "GAMETIME";

uint8_t game_has_started = 0;
uint8_t num_goals = 0;

void print_game(){
	printf("Current goals: %d\n\r", num_goals);
}

void goal(){
	printf("GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!GOAL!");
	num_goals++;
}

void display_game(){
	oled_fill_entire(0x00);
	
	oled_write_string_on_line(title, strlen(title), 0);
	
	// display score
	char* s = "Current score:  x";
	s[strlen(s) - 1] = '0' + num_goals;
	
	oled_write_string_on_line(s, strlen(s), 1);
	
	// display time
}


#endif /* GAME_H_ */