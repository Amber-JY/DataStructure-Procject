/**/
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
#include"Data structure.h"
#include"Visual.h"
#include"SokudoToCNF.h"

/*This function helps to control the location of input*/
void Goto(int x, int y)
{
	++x, ++y;
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/*This function helps to control the color of sudoku show*/
void Color(int b)
{
	HANDLE hConsole = GetStdHandle((STD_OUTPUT_HANDLE));
	SetConsoleTextAttribute(hConsole, b);
}


/*This function is aiming to create the board of sudoku game*/
int InitBoard(Clause* SingleSet, int level, int(* So)[16]){
	Goto(3, 1);
	Color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//Dark line.
	for (int i = 0; i < level; i++)
	{
		Color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		Goto(3, i * 2 + 1);
		//if (i%3 == 0)
			//Color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		for (int k = 0; k < level; k++)
			printf("+---");
		printf("+");
		for (int j = 0; j <= level; j++){
			Goto(3 + j * 4, (i + 1) * 2);
			if (j%3 == 0)
				Color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
			printf("|   ");
		}
	}
	//
	for (int i = 0; i < SingleSet->Length / 5000; i++) {
		Color(FOREGROUND_BLUE);
		Goto(1 + abs(SingleSet->Sub[i].Value % 10) * 4, abs(SingleSet->Sub[i].Value / 10) * 2);
		if (SingleSet->Sub[i].Value > 0)
			printf("1");
		else
			printf("0");
		So[abs(SingleSet->Sub[i].Value / 10)-1][abs(SingleSet->Sub[i].Value % 10)-1] = 0;
	}

		
	//
	Color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	Goto(3, level * 2 + 1);
	for (int i = 0; i < level; i++)
		printf("+---");
	printf("+");
	for (int i = 1; i <= level; ++i)
	{
		Goto(4 * i + 1, 0);
		printf("%d", i);
		Goto(1, 2 * i);
		printf("%d", i);
	}
	//
	Goto(60, 1);
	printf("Level %d Binary Puzzle", level);
	Goto(55, 3);
	printf("Here are the intrauctions for you.");
	Goto(55, 5);
	printf("Input [1] to work out the puzzle manually.");
	Goto(55, 7);
	printf("Input [2] to work out the puzzle automatically.");
	Goto(55, 9);
	return OK;
}

int ShowBoard_auto(Clause* ClauseBase, int level) {
	if (!ClauseBase) {
		printf("The Cluase set address is error.\n");
		exit(ERROR);
	}
	for (int i = 0; i < level; i++){
		for (int j = 0; j < level; j++){
			Color(FOREGROUND_BLUE);
			Goto(1 + j * 4, i + 2);
			printf("%d", ClauseBase->Sub[i * level + j - 1].Value > 0 ? 1 : 0);
		}
	}
}

int ShowBoard_manu(int level, int(*So)[16]) {
	char location[4];
	Color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	Goto(55, 5);
	printf("Now, you are in manual mode. Please input as follow to game.");
	Goto(55, 7);
	printf("Input as [xyz]. [x] means line x(1-%d). [y] means row y(1-%d). ", level, level);
	Goto(55, 9);
	printf("[z] means True or False(0 or 1).");
	Goto(55, 11);
	printf("Input [001] to judge. Input [000] to exit.");
	while (1) {
		Goto(55, 13);
		Color(FOREGROUND_GREEN);
		printf("[           ]");
		Goto(60, 13);
		scanf("%s", location);
		if (location[0] == '0' && location[1]=='0' && location[2] == '0')
			return OK;
		if (location[0] == '0' && location[1]=='0' && location[2] == '1') {
			break;
		}
		if (location[0] == '0' || location[1] == '0') {
			Goto(57, 15);
			Color(FOREGROUND_GREEN);
			printf("Over Scale.");
			continue;
		}
		if (So[location[0] - '1'][location[1] - '1'] == 0) {
			Goto(55, 15);
			Color(FOREGROUND_GREEN);
			printf("Fixed item.");
			continue;
		}
		Goto(55, 15);
		Color(FOREGROUND_GREEN);
		printf("-----------");
		Goto((location[1] - '0') * 4 + 1, (location[0] - '0' - 1) * 2 + 2);
		printf("%d", location[2] - '0');
	}
	return OK;
}
