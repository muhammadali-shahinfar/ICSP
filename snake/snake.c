#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#pragma warning(disable:4996)

//colors
#define RED 12
#define BLUE 3
#define GREEN 10
#define YELLOW 14
#define GRAY 8
#define PINK 13
#define WHITE 15
#define WAIT_TIME_MILI_SEC 100
//directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
// general
#define BOARD_SIZE 40
#define INITIAL_SNAKE_LENGTH 3
#define MINIMUM_SNAKE_LENGTH 2
#define MAX_LEN_SNAKES 30
#define NUMBER_OF_MOUSES 20
//board_characters
#define EMPTY '0'
#define MOUSE 'm'
#define PLAYER1_SNAKE_HEAD '1'
#define PLAYER2_SNAKE_HEAD '2'
#define PLAYER1_SNAKE_BODY 'a'
#define PLAYER2_SNAKE_BODY 'b'
//Bernard, Poison and golden star
#define BERNARD_CLOCK 'c' //on the board character
#define GOLDEN_STAR '*' //on the board character
#define POISON 'x' //on the board character
#define NUMBER_OF_POISONS 5
#define NUMBER_OF_GOLDEN_STARS 3
#define BERNARD_CLOCK_APPEARANCE_CHANCE_PERCENT 20
#define BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC 2000
#define BERNARD_CLOCK_FROZEN_TIME_MILI_SEC 4000
// constant numbers
#define zero 0
#define one 1
#define two 2
#define three 3
#define minus_one -1

CONSOLE_FONT_INFOEX former_cfi;
CONSOLE_CURSOR_INFO former_info;
COORD former_screen_size;

void reset_console() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleDisplayMode(consoleHandle, CONSOLE_WINDOWED_MODE, &former_screen_size);
	SetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
	SetConsoleCursorInfo(consoleHandle, &former_info);
}

void hidecursor(HANDLE consoleHandle)
{
	GetConsoleCursorInfo(consoleHandle, &former_info);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void set_console_font_and_font_size(HANDLE consoleHandle) {
	former_cfi.cbSize = sizeof(former_cfi);
	GetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 20;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Courier");
	SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi);
}

void set_full_screen_mode(HANDLE consoleHandle) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	former_screen_size.X = csbi.dwSize.X; former_screen_size.Y = csbi.dwSize.Y;
	COORD coord;
	SetConsoleDisplayMode(consoleHandle, CONSOLE_FULLSCREEN_MODE, &coord);
}

void init_screen()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	set_full_screen_mode(consoleHandle);
	hidecursor(consoleHandle);
	set_console_font_and_font_size(consoleHandle);

}

void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction) {
	DWORD64 start_time, check_time;
	start_time = GetTickCount64();
	check_time = start_time + WAIT_TIME_MILI_SEC; //GetTickCount returns time in miliseconds
	char key = 0;
	char player1_key_hit = 0;
	char player2_key_hit = 0;

	while (check_time > GetTickCount64()) {
		if (_kbhit()) {
			key = _getch();
			if (key == 0)
				key = _getch();
			if (key == 'w' || key == 'a' || key == 's' || key == 'd')
				player1_key_hit = key;
			if (key == 'i' || key == 'j' || key == 'k' || key == 'l')
				player2_key_hit = key;
		}
	}

	switch (player1_key_hit) {
	case 'w': if (*player1_snake_direction != DOWN) *player1_snake_direction = UP; break;
	case 'a': if (*player1_snake_direction != RIGHT) *player1_snake_direction = LEFT; break;
	case 's': if (*player1_snake_direction != UP) *player1_snake_direction = DOWN; break;
	case 'd': if (*player1_snake_direction != LEFT) *player1_snake_direction = RIGHT; break;
	default: break;
	}

	switch (player2_key_hit) {
	case 'i': if (*player2_snake_direction != DOWN) *player2_snake_direction = UP; break;
	case 'j': if (*player2_snake_direction != RIGHT) *player2_snake_direction = LEFT; break;
	case 'k': if (*player2_snake_direction != UP) *player2_snake_direction = DOWN; break;
	case 'l': if (*player2_snake_direction != LEFT) *player2_snake_direction = RIGHT; break;
	default: break;
	}
}

void draw_point(char point_content) {
	switch (point_content) {
	case PLAYER1_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED); printf("@"); break;
	case PLAYER2_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("@"); break;
	case PLAYER1_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);  printf("o"); break;
	case PLAYER2_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("o"); break;
	case MOUSE: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY); printf("m"); break;
	case GOLDEN_STAR: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW); printf("*"); break;
	case POISON: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN); printf("x"); break;
	case BERNARD_CLOCK: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK); printf("c"); break;
	default: printf(" ");
	}
}

void draw_horizonatal_walls() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	for (int i = 0; i < BOARD_SIZE + 2; ++i)
		printf("-");
	printf("\n");
}

void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]) {
	system("cls");
	draw_horizonatal_walls();
	for (int i = 0; i < BOARD_SIZE; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		printf("|"); // vertical wall 
		for (int j = 0; j < BOARD_SIZE; j++)
			draw_point(board_content[i][j]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		printf("|\n"); // vertical wall
	}
	draw_horizonatal_walls();
}
int board_side_movement(int location, int direction) { // movement of snake in board sides
	switch (direction) {
	case UP: {
		if (location == zero) return BOARD_SIZE - one;
		else return location - one;
		break;
	}
	case DOWN: {
		if (location == BOARD_SIZE - one) return zero;
		else return location + one;
		break;
	}
	case LEFT: {
		if (location == zero) return BOARD_SIZE - one;
		else return location - one;
		break;
	}
	case RIGHT: {
		if (location == BOARD_SIZE - one) return zero;
		else return location + one;
		break;
	}
	}
}
void Mouse_random_location(char Board_content[BOARD_SIZE][BOARD_SIZE]) { // plasement mouse in board
	while (one) {
		int mouse_horizontal = rand() % BOARD_SIZE;
		int mouse_vertical = rand() % BOARD_SIZE;
		if (Board_content[mouse_horizontal][mouse_vertical] == EMPTY) {
			Board_content[mouse_horizontal][mouse_vertical] = MOUSE;
			break;
		}
	}
	return;
}
void player1_snake_down_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) { // run when snake direction is  down && destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != minus_one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - 1]][snake_vertical[i - 1]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = snake_vertical[one];
	snake_horizon[zero] = board_side_movement(snake_horizon[one], DOWN);
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
}
void player2_snake_down_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) { // run when snake direction is  down && destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != -one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = snake_vertical[one];
	snake_horizon[zero] = board_side_movement(snake_horizon[one], DOWN);
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
}
void player1_snake_left_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) { // run when snake direction is  left&& destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != -one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = board_side_movement(snake_vertical[one], LEFT);
	snake_horizon[zero] = snake_horizon[one];
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
}
void player2_snake_left_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) { // run when snake direction is  left&& destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != -one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = board_side_movement(snake_vertical[one], LEFT);
	snake_horizon[zero] = snake_horizon[one];
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
}
void player1_snake_right_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) { // run when snake direction is  right&& destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != -one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = board_side_movement(snake_vertical[one], RIGHT);
	snake_horizon[zero] = snake_horizon[one];
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
}
void player2_snake_right_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) { // run when snake direction is  right && destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != -one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = board_side_movement(snake_vertical[one], RIGHT);
	snake_horizon[zero] = snake_horizon[one];
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
}
void player1_snake_up_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) {  // run when snake direction is  up && destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != minus_one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = snake_vertical[one];
	snake_horizon[zero] = board_side_movement(snake_horizon[one], UP);
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
}
void player2_snake_up_direction_empty(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], char board_content[BOARD_SIZE][BOARD_SIZE]) {  // run when snake direction is  up && destination bloc is empty
	int i;
	for (i = zero; i < MAX_LEN_SNAKES && snake_horizon[i] != minus_one; i++) { // compute snake length
	}
	board_content[snake_horizon[i - one]][snake_vertical[i - one]] = EMPTY;
	while (i>one) {
		snake_horizon[i - one] = snake_horizon[i - two];
		snake_vertical[i - one] = snake_vertical[i - two];
		i--;
	}
	snake_vertical[zero] = snake_vertical[one];
	snake_horizon[zero] = board_side_movement(snake_horizon[one], UP);
	board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
	board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
}
void poison_random_location(char board_content[BOARD_SIZE][BOARD_SIZE]) { // placement poisons in random locations
	while (one) {
		int i = rand() % BOARD_SIZE;
		int j = rand() % BOARD_SIZE;
		if (board_content[i][j] == EMPTY) {
			board_content[i][j] = POISON;
			return;
		}
	}
}
void player1_movemnt(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], int direction, char board_content[BOARD_SIZE][BOARD_SIZE], int *player1_snake_length, int *player2_snake_length) {  // all of movement run here
	switch (direction) {
	case UP: {
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == EMPTY) { // if destination bloc is empty
			player1_snake_up_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == MOUSE) { // if destination bloc is mouse
			if (*(player1_snake_length) == MAX_LEN_SNAKES) player1_snake_up_direction_empty(snake_horizon, snake_vertical, board_content); // if snake length is maximum then program behavior is similar to Empty
			else {
				int i = *player1_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) { // length of snake increase by one
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
				snake_horizon[zero] = board_side_movement(snake_horizon[zero], UP);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
				*(player1_snake_length) = *(player1_snake_length)+one;
			}
			Mouse_random_location(board_content); // replace eaten mouse
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == POISON) {
			*(player1_snake_length) = *(player1_snake_length)-one; // snake length is decreased by one
			if (*(player1_snake_length) < two) { // end program
				system("cls");
				reset_console();
				printf("player 2 is winner\n");
				exit(0); // close program
			}
			board_content[snake_horizon[*(player1_snake_length)]][snake_vertical[*(player1_snake_length)]] = EMPTY;
			snake_horizon[*(player1_snake_length)] = minus_one;
			snake_vertical[*(player1_snake_length)] = minus_one;
			player1_snake_up_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == PLAYER1_SNAKE_BODY) {  // percussion between head of snake1 and body of snake 1
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == PLAYER2_SNAKE_BODY) { // percussion between head of snake1 and body of snake 2
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == PLAYER2_SNAKE_HEAD) { // percussion between head of snake2 and body of snake 2
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	case DOWN: { // algorithm of this bloc is similar to case UP
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == EMPTY) {
			player1_snake_down_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == MOUSE) {
			if (*(player1_snake_length) == MAX_LEN_SNAKES) player1_snake_down_direction_empty(snake_horizon, snake_vertical, board_content);
			else {
				int i = *player1_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
				snake_horizon[zero] = board_side_movement(snake_horizon[zero], DOWN);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
				*(player1_snake_length) = *(player1_snake_length)+one;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == POISON) {
			*(player1_snake_length) = *(player1_snake_length)-one;
			if (*(player1_snake_length) < two) {
				system("cls");
				reset_console();
				printf("player 2 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player1_snake_length)]][snake_vertical[*(player1_snake_length)]] = EMPTY;
			snake_horizon[*(player1_snake_length)] = minus_one;
			snake_vertical[*(player1_snake_length)] = minus_one;
			player1_snake_down_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == PLAYER1_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == PLAYER2_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == PLAYER2_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	case LEFT: { // algorithm of this bloc is similar to case UP
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == EMPTY) {
			player1_snake_left_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == MOUSE) {
			if (*(player1_snake_length) == MAX_LEN_SNAKES) player1_snake_left_direction_empty(snake_horizon, snake_vertical, board_content);
			else {
				int i = *player1_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
				snake_vertical[zero] = board_side_movement(snake_vertical[0], LEFT);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
				*(player1_snake_length) = *(player1_snake_length)+one;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == POISON) {
			*(player1_snake_length) = *(player1_snake_length)-one;
			if (*(player1_snake_length) < two) {
				system("cls");
				reset_console();
				printf("player 2 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player1_snake_length)]][snake_vertical[*(player1_snake_length)]] = EMPTY;
			snake_horizon[*(player1_snake_length)] = minus_one;
			snake_vertical[*(player1_snake_length)] = minus_one;
			player1_snake_left_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == PLAYER1_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == PLAYER2_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == PLAYER2_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	case RIGHT: { // algorithm of this bloc is similar to case UP
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == EMPTY) {
			player1_snake_right_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == MOUSE) {
			if (*(player1_snake_length) == MAX_LEN_SNAKES) player1_snake_right_direction_empty(snake_horizon, snake_vertical, board_content);
			else {
				int i = *player1_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER1_SNAKE_BODY;
				snake_vertical[zero] = board_side_movement(snake_vertical[zero], RIGHT);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER1_SNAKE_HEAD;
				*(player1_snake_length) = *(player1_snake_length)+one;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == POISON) {
			*(player1_snake_length) = *(player1_snake_length)-1;
			if (*(player1_snake_length) < two) {
				system("cls");
				reset_console();
				printf("player 2 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player1_snake_length)]][snake_vertical[*(player1_snake_length)]] = EMPTY;
			snake_horizon[*(player1_snake_length)] = minus_one;
			snake_vertical[*(player1_snake_length)] = minus_one;
			player1_snake_right_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == PLAYER1_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == PLAYER2_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 2 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == PLAYER2_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	}
}
void player2_movemnt(int snake_horizon[MAX_LEN_SNAKES], int snake_vertical[MAX_LEN_SNAKES], int direction, char board_content[BOARD_SIZE][BOARD_SIZE], int *player1_snake_length, int *player2_snake_length) {  // all of player2's movements run here
	switch (direction) {
	case UP: { // algorithm of this bloc is similar to player 1 UP direction
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == EMPTY) {
			player2_snake_up_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == MOUSE) {
			if (*(player2_snake_length) == MAX_LEN_SNAKES) {
				player2_snake_up_direction_empty(snake_horizon, snake_vertical, board_content);
			}
			else {
				int i = *player2_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
				snake_horizon[zero] = board_side_movement(snake_horizon[zero], UP);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
				*(player2_snake_length) = *(player2_snake_length)+1;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == POISON) {
			*(player2_snake_length) = *(player2_snake_length)-one;
			if (*(player2_snake_length) < two) {
				system("cls");
				reset_console();
				printf("player 1 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player2_snake_length)]][snake_vertical[*(player2_snake_length)]] = EMPTY;
			snake_horizon[*(player2_snake_length)] = minus_one;
			snake_vertical[*(player2_snake_length)] = minus_one;
			player2_snake_up_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == PLAYER2_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == PLAYER1_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], UP)][snake_vertical[zero]] == PLAYER1_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player 1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	case DOWN: {// algorithm of this bloc is similar to player 1 UP direction
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == EMPTY) {
			player2_snake_down_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == MOUSE) {
			if (*(player2_snake_length) == MAX_LEN_SNAKES) {
				player2_snake_down_direction_empty(snake_horizon, snake_vertical, board_content);
			}
			else {
				int i = *player2_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
				snake_horizon[zero] = board_side_movement(snake_horizon[zero], DOWN);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
				*(player2_snake_length) = *(player2_snake_length)+one;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == POISON) {
			*(player2_snake_length) = *(player2_snake_length)-one;
			if (*(player2_snake_length) < two) {
				system("cls");
				reset_console();
				printf("player 1 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player2_snake_length)]][snake_vertical[*(player2_snake_length)]] = EMPTY;
			snake_horizon[*(player2_snake_length)] = minus_one;
			snake_vertical[*(player2_snake_length)] = minus_one;
			player2_snake_down_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == PLAYER2_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == PLAYER1_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[board_side_movement(snake_horizon[zero], DOWN)][snake_vertical[zero]] == PLAYER1_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	case LEFT: { // algorithm of this bloc is similar to player 1 UP direction
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == EMPTY) {
			player2_snake_left_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == MOUSE) {
			if (*(player2_snake_length) == MAX_LEN_SNAKES) {
				player2_snake_left_direction_empty(snake_horizon, snake_vertical, board_content);
			}
			else {
				int i = *player2_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
				snake_vertical[zero] = board_side_movement(snake_vertical[zero], LEFT);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
				*(player2_snake_length) = *(player2_snake_length)+one;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == POISON) {
			*(player2_snake_length) = *(player2_snake_length)-one;
			if (*(player2_snake_length) < MINIMUM_SNAKE_LENGTH) {
				system("cls");
				reset_console();
				printf("player 1 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player2_snake_length)]][snake_vertical[*(player2_snake_length)]] = EMPTY;
			snake_horizon[*(player2_snake_length)] = minus_one;
			snake_vertical[*(player2_snake_length)] = minus_one;
			player2_snake_left_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == PLAYER2_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == PLAYER1_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], LEFT)] == PLAYER1_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw\n");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	case RIGHT: { // algorithm of this bloc is similar to player 1 UP direction
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == EMPTY) {
			player2_snake_right_direction_empty(snake_horizon, snake_vertical, board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == MOUSE) {
			if (*(player2_snake_length) == MAX_LEN_SNAKES) {
				player2_snake_right_direction_empty(snake_horizon, snake_vertical, board_content);
			}
			else {
				int i = *player2_snake_length;
				board_content[snake_horizon[i]][snake_vertical[i]] = EMPTY;
				while (i > zero) {
					snake_horizon[i] = snake_horizon[i - one];
					snake_vertical[i] = snake_vertical[i - one];
					i--;
				}
				board_content[snake_horizon[one]][snake_vertical[one]] = PLAYER2_SNAKE_BODY;
				snake_vertical[zero] = board_side_movement(snake_vertical[zero], RIGHT);
				board_content[snake_horizon[zero]][snake_vertical[zero]] = PLAYER2_SNAKE_HEAD;
				*(player2_snake_length) = *(player2_snake_length)+one;
			}
			Mouse_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == POISON) {
			*(player2_snake_length) = *(player2_snake_length)-one;
			if (*(player2_snake_length) < MINIMUM_SNAKE_LENGTH) {
				system("cls");
				reset_console();
				printf("player 1 is winner\n");
				exit(0);
			}
			board_content[snake_horizon[*(player2_snake_length)]][snake_vertical[*(player2_snake_length)]] = EMPTY;
			snake_horizon[*(player2_snake_length)] = minus_one;
			snake_vertical[*(player2_snake_length)] = minus_one;
			player2_snake_right_direction_empty(snake_horizon, snake_vertical, board_content);
			poison_random_location(board_content);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == PLAYER2_SNAKE_BODY) { // this block have bug: ghesmate akhar badan mar tooye khoone moonde va jabeja nashode
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == PLAYER1_SNAKE_BODY) {
			system("cls");
			reset_console();
			printf("player 1 is winner\n");
			exit(0);
		}
		if (board_content[snake_horizon[zero]][board_side_movement(snake_vertical[zero], RIGHT)] == PLAYER1_SNAKE_HEAD) {
			if (*player1_snake_length == *player2_snake_length) {
				system("cls");
				reset_console();
				printf("draw");
				exit(0);
			}
			if (*player1_snake_length > *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player1 is winner\n");
				exit(0);
			}
			if (*player1_snake_length < *player2_snake_length) {
				system("cls");
				reset_console();
				printf("player2 is winner\n");
				exit(0);
			}
		}
		break;
	}
	}
}
// prototypes
void init_screen();
void reset_console();
void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction);
void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]);

int main() {
	//First define the variables you need during the game
	char board_content[BOARD_SIZE][BOARD_SIZE];
	int player1_snake_direction, player2_snake_direction;
	int player1_snake_length = INITIAL_SNAKE_LENGTH, player2_snake_length = INITIAL_SNAKE_LENGTH;
	int player1_snake_horizon[MAX_LEN_SNAKES], player1_snake_vertical[MAX_LEN_SNAKES], player2_snake_horizon[MAX_LEN_SNAKES], player2_snake_vertical[MAX_LEN_SNAKES];
	//Do not forget to Initialize your variables, specially board_content
	for (int i = zero; i < BOARD_SIZE; i++) { // initialize Boardcontent
		for (int j = zero; j < BOARD_SIZE; j++) {
			if (i < INITIAL_SNAKE_LENGTH - one && j == BOARD_SIZE - one) board_content[i][j] = PLAYER1_SNAKE_BODY;
			else if (i>BOARD_SIZE - INITIAL_SNAKE_LENGTH && j == zero) board_content[i][j] = PLAYER2_SNAKE_BODY;
			else if (i == BOARD_SIZE - INITIAL_SNAKE_LENGTH && j == zero) board_content[i][j] = PLAYER2_SNAKE_HEAD;
			else if (i == INITIAL_SNAKE_LENGTH - one  && j == BOARD_SIZE - one) board_content[i][j] = PLAYER1_SNAKE_HEAD;
			else board_content[i][j] = EMPTY;
		}
	}
	for (int i = zero; i < NUMBER_OF_MOUSES; i++) { // placement mouses
		Mouse_random_location(board_content);
	}
	for (int i = 0; i < NUMBER_OF_POISONS; i++) { // placement poison
		poison_random_location(board_content);
	}
	for (int i = zero; i < MAX_LEN_SNAKES; i++) {   //initialize player1_snake_horizon & player1_snake_vertical & player2_snake_horizon & player2_snake_vertical
		player1_snake_vertical[i] = minus_one;
		player2_snake_vertical[i] = minus_one;
		player1_snake_horizon[i] = minus_one;
		player2_snake_horizon[i] = minus_one;
	}
	for (int i = zero; i < INITIAL_SNAKE_LENGTH; i++) {    //initialize snakes
		player1_snake_horizon[i] = INITIAL_SNAKE_LENGTH - (i + one);
		player1_snake_vertical[i] = BOARD_SIZE - one;
		player2_snake_horizon[i] = BOARD_SIZE - INITIAL_SNAKE_LENGTH + i;
		player2_snake_vertical[i] = zero;
	}

	/*
	player1_snake_horizon[zero] = two;
	player1_snake_horizon[one] = one;
	player1_snake_horizon[two] = zero;
	player1_snake_vertical[zero] = BOARD_SIZE - one;
	player1_snake_vertical[one] = BOARD_SIZE - one;
	player1_snake_vertical[two] = BOARD_SIZE - one;
	player2_snake_horizon[zero] = BOARD_SIZE - three;
	player2_snake_horizon[one] = BOARD_SIZE - two;
	player2_snake_horizon[two] = BOARD_SIZE - one;
	player2_snake_vertical[zero] = zero;
	player2_snake_vertical[one] = zero;
	player2_snake_vertical[two] = zero;
	}*/
	init_screen(); //call this to make window adjustments
	while (TRUE) {

		draw_board(board_content);
		wait_and_get_direction(&player1_snake_direction, &player2_snake_direction);
		player1_movemnt(player1_snake_horizon, player1_snake_vertical, player1_snake_direction, board_content, &player1_snake_length, &player2_snake_length);
		player2_movemnt(player2_snake_horizon, player2_snake_vertical, player2_snake_direction, board_content, &player1_snake_length, &player2_snake_length);
		//do some check after key press, and let snakes move forward one house
	}
	//The Game is Finished, Show the result
	return 0;
}
