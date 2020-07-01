/**/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"Data structure.h"
#include"SokudoToCNF.h"
#include"Visual.h"
#include"DPLL.h"
#include"IN_OUT.h"


int main() {
	bool PASS;
	int choice = 1;
	while (choice) {
		printf("-------------------Puzzle or SAT--------------------\n");
		printf("Welcome to use this system.Please enter your choice:\n");
		printf("[1]: SAT solver.\n[2]: Puzzle solver.\n[0]: Exit.\n");
		printf("----------------------------------------------------\n");
		scanf("%d", &choice);
		system("cls");
		if (choice == 1) {//CNF solver.
			clock_t Begin, End;
			double RunningTime;
			Clause* ClauseBase = ClauseSet_init();
			ClauseBase = ReadFile(ClauseBase);
			//PutoutData(ClauseBase);
			//Clocking.
			Begin = clock();
			if (DPLL(ClauseBase))
			{
				printf("---------------------------------\n");
				printf("\n*This sample is satiable!*\n");
				PASS = 1;
			}
			else
			{
				printf("---------------------------------\n");
				printf("\n*This sample is unsatisable!*\n");
				PASS = 0;
			}
			End = clock();

			RunningTime = End - Begin;
			printf("\n*Running time is [%f]ms.*\n", RunningTime);
			printf("\n-----------------------------------\n");
			system("pause");
			system("cls");

			int temp;
			printf("\nPlease choose what to do next.\n");
			printf("[1]:Save the result as file.\n");
			printf("[2]:Quit the solver.\n");
			printf("------>");
			scanf("%d", &temp);
			if (temp == 1) {
				system("cls");
				Sort(ClauseBase->Sub);
				SaveAsFile(ClauseBase, PASS, RunningTime);
				system("pause");
			}
			else {
				system("pause");
			}
		}
		else if (choice == 0){
			printf("See you next time!\n");
			break;
		}
		else {//Binary puzzle solver.
			char name[10] = { '\0' };
			int level;
			int So[16][16];
			for (int i = 0; i < 16; i++)
				for (int j = 0; j < 16; j++)
					So[i][j] = 1;
			Clause* SingleSet = NULL;
			printf("Please enter the name of sokudo you want to play as \"x-y\".\n\n");
			printf("In this formula, [x] means the level of binary puzzle. [y] means the number(0-9).\n\n---->");
			scanf("%s", &name);
			level = name[0] - '0';
			system("cls");
			SingleSet = InitialSokudo(name, level);
			InitBoard(SingleSet, level, So);
			int choice_;//Choose to manually or auto;
			scanf("%d", &choice_);
			switch (choice_) {
			case 1:
				ShowBoard_manu(level, So);
				system("pause");
				break;
			case 2:
				Constraint(SingleSet, name);
				strcat(name, ".cnf");
				Clause* ClauseBase = ClauseSet_init();
				fprintf(stdin, "%s", name);
				ClauseBase = ReadFile(ClauseBase);
				PutoutData(ClauseBase);
				if (DPLL(ClauseBase))
				{
					printf("This sample is satiable!\n");
					ShowBoard_auto(ClauseBase, level);
					SaveAsFile(ClauseBase, 1, -1);
				}
				else
				{
					printf("This sample is unsatisable!\n");
				}
				system("pause");
				break;
			}//switch
		}//else
		system("cls");
	}//while
	return 0;
}