/***************************************************************************
*This source file contains the detail of basic functions about input and   *
*output.				                                                   *
***************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"Data structure.h"
#include"IN_OUT.h"

/*
*This function aims to read data from a specified file, and
*store the data into the clause set.
*@peremeter:
*ClauseBase: The base address of clause set.
*/
Clause* ReadFile(Clause* ClauseBase) {
	if (!ClauseBase) {
		printf("\nClause set doesn't exist.\n");
		exit(ERROR);
	}
	char filename[50];
	char content[200];
	char path[50] = "./Sample/";
	int num_tem_content = 0;
	int num_content = 0;//The location in content array.
	int num_Clause = 1;//The location in clause set.Start from 1.
	int num_Literal = 0;//The location in literal set.
	int flag = 1;//Mark the positive and negative number.
	char tem_content[10];
	printf("Please enter name of the file you want to read.\n\n");
	printf("Here are some samples.\n");
	printf("---------------------------------------------------------------\n");
	printf("Base:\n\n");
	printf("[Function]:\tsat-20\tunsat-5cnf-30\n\n");
	printf("[Performance]:\tais10\tsud00009\n\n");
	printf("Satisfiable:\n\n");
	printf("[S]:\tproblem1-20\tproblem2-50\tproblem3-100\n\tproblem6-50\tproblem8-50\tproblem9-100\n\n");
	printf("[M]:\tproblem5-200\tproblem12-200\n\tsud00001\n\n");
	printf("[L]:\tec-iso-ukn009.shuffled-as.sat05-3632-1584\n\teh-vmpc_25.renamed-as.sat05-1913-625\n\teh-vmpc_29.renamed-as.sat05-1916-841\n\te-par32-3.shuffled-3176\n\n");
	printf("Unsatisfiable:\n\n");
	printf("\ttst_v10_c100\tu-problem7-50\n\tu-homer14.shuffled-300\tu-dp04u03.shuffled-825\n\tu-c6288.shuffled-5008\n\n");
	printf("---------------------------------------------------------------\n");
	printf("------->");
	scanf("%s", filename);
	strcat(filename, ".cnf");
	strcat(path, filename);
	FILE* IN = NULL;
	if ((IN = fopen(path, "r+")) == NULL) {
		printf("Can't open the file!\n");
		exit(ERROR);
	}
	//Skip the comment in cnf files.
	while (!feof(IN)) {
		fgets(content, 50, IN);
		if (content[0] == 'c')
			continue;
		else if (content[0] == 'p') {
			num_content += 6;//Go to the first number in last line of comment.
			break;
		}
	}
	while (content[num_content] != ' ') {
		tem_content[num_tem_content] = content[num_content];
		num_content++;
		num_tem_content++;
	}
	num_tem_content--;
	for (int i = 0, j = num_tem_content; i <= num_tem_content && j >= 0; i++,j--) {
		VARIETY_SIZE += pow(10, j) * (tem_content[i] - '0');
	}
	Literal* Variety = VarietySet_init();
	num_tem_content = 0;
	num_content++;
	CLAUSE_SETSIZE = 0;
	while (content[num_content] != '\n') {
		tem_content[num_tem_content] = content[num_content];
		num_content++;
		num_tem_content++;
	}
	num_tem_content--;
	for (int i = 0, j = num_tem_content; i <= num_tem_content && j >= 0; i++, j--) {
		CLAUSE_SETSIZE += pow(10, j) * (tem_content[i] - '0');
	}
	//The last position is used to add the first variety in DPLL.
	//The first position is used to store the length of clause set.
	CLAUSE_SETSIZE += 2;
	num_content = 0;
	num_tem_content = 0;
	ClauseBase = ClauseResize(ClauseBase, CLAUSE_SETSIZE);//Resize clause set.
	//Push data into clause set.
	while (num_Clause <= CLAUSE_SETSIZE - 2) {
		fgets(content, 200, IN);
		Literal* LiteralBase = LiteralSet_init();
		if (content[0] == 'c' || content[0] == 'p')//Skip comment;
			continue;
		while (content[num_content + 1] != '\n') {
			//Put a literal into tem_content.

			while (content[num_content] != ' ') {
				if (content[num_content] == '-') {
					flag = 0;
					num_content++;
				}
				else {
					tem_content[num_tem_content] = content[num_content];
					num_tem_content++;
					num_content++;
				}
			}//End of while(literal)
			num_tem_content--;
			for (int i = 0, j = num_tem_content; i <= num_tem_content && j >= 0; i++, j--) {
				if (flag) {
					LiteralBase[num_Literal].Value += pow(10, j) * (tem_content[i] - '0');
				}
				else {
					LiteralBase[num_Literal].Value -= pow(10, j) * (tem_content[i] - '0');
				}
			}
			PushVariety(Variety, abs(LiteralBase[num_Literal].Value));
			LiteralBase[num_Literal].Location = num_Literal;
			LiteralBase[num_Literal].State = 1;
			num_Literal++;
			if (num_Literal + 2 >= LITERAL_SETSIZE) {
				LITERAL_SETSIZE += INCREASE;
				LiteralBase = LiteralResize(LiteralBase, num_Literal);
			}
			//
			num_tem_content = 0;
			num_content++;
			flag = 1;
		}//End of while(clause).
		ClauseBase[num_Clause].Sub = LiteralBase;
		ClauseBase[num_Clause].Location = num_Clause;
		ClauseBase[num_Clause].Length = num_Literal * 200 + num_Literal;
		ClauseBase[num_Clause].State = 1;
		num_content = 0;
		num_Literal = 0;
		num_Clause++;
	}//End of while(file).
	//num_Clause--;
	//Let's the last position be unusable.
	Literal* LiteralBase = LiteralSet_init();
	ClauseBase[num_Clause].Sub = LiteralBase;
	ClauseBase[num_Clause].Length = num_Literal * 200 + num_Literal;
	ClauseBase[num_Clause].Location = num_Clause;
	ClauseBase[num_Clause].State = 0;
	//The first position stores the length of clause set.
	//(()*1000)/1000 total clause number.() present clause number.
	ClauseBase[0].Length = (num_Clause)*5000 + (num_Clause - 1);
	ClauseBase[0].Location = 0;
	ClauseBase[0].State = 0;
	ClauseBase[0].Sub = Variety;
	fclose(IN);
	return ClauseBase;	
}

/*
*This function aim to print the data in clause set and the last time
*of program on the screen.Used to affirm the data artificially.
*@parameter:
*ClauseBase: The base address of clause set.
*Lasttime: The last time of program.
*/
int PutoutData(Clause* ClauseBase) {
	if (!ClauseBase) {
		printf("\nClause set doesn't exist.\n");
		return ERROR;
	}
	int num_Clause = 1;//The location in clause set.
	int num_Literal = 0;//The location in literal set.
	while (num_Clause < (ClauseBase[0].Length / 5000)) {
		if (ClauseBase[num_Clause].State) {
			printf("\n*Clause %d*", num_Clause);
			while (num_Literal < ClauseBase[num_Clause].Length / 200) {
				if (ClauseBase[num_Clause].Sub[num_Literal].State)
					printf("%5d", ClauseBase[num_Clause].Sub[num_Literal].Value);
				num_Literal++;
			}//End of inwhile
			printf("*\n");
			num_Literal = 0;
		}
		num_Clause++;
	}//End of outwhile
	return OK;
}

/*
*This function aim to save the data in the clause set as file.
*@parameter:
*ClauseBase: The base address of clause set.
*Lasttime: The last time of program.
*PASS: This clause set is satisfiable or not;
*/
int SaveAsFile(Clause* ClauseBase, bool PASS, double time) {
	if (!ClauseBase) {
		printf("\nClause set doesn't exist.\n");
		return ERROR;
	}
	char filename[20];
	char path[50] = "./Out/";
	printf("\nPlease input the saved file's name.\n");
	scanf("%s", filename);
	strcat(filename, ".res");
	strcat(path, filename);
	FILE* OUT = NULL;
	if ((OUT = fopen(path, "w+")) == NULL) {
		printf("\nCan't open that file!\n");
		return ERROR;
	}
	int num_Variety = 0;
	if (PASS) {
		fprintf(OUT, "S: 1\nV: ");
		while (num_Variety < VARIETY_SIZE) {
			//printf("%d", ClauseBase->Sub[num_Variety].State);
			if(ClauseBase->Sub[num_Variety].State == 0) {
				fprintf(OUT, "%5d", ClauseBase->Sub[num_Variety].Value);
			}
			else if (ClauseBase->Sub[num_Variety].State == -1) {
				fprintf(OUT, "%5d", -ClauseBase->Sub[num_Variety].Value);
			}
			num_Variety++;
		}//End of while.
		fprintf(OUT, "\nT: %f\n", time);
	}
	else
		fprintf(OUT, "S: 0\nT: %f\n", time);
	fclose(OUT);
	return OK;
}

/*
*Push variety into variety set.
*@parameter:
*Variety:The base address of variety set.
*v:The variety.
*/
int PushVariety(Literal* Variety, int v) {
	int location = 0;
	while (1) {
		if (Variety[location].State == false) {
			Variety[location].State = 1;
			Variety[location].Location = location;
			Variety[location].Value = v;
			break;
		}
		else if (Variety[location].State == 1 && Variety[location].Value != v) {
			location++;
			continue;
		}
		else if (Variety[location].State == 1 && Variety[location].Value == v) {
			break;
		}
	}
	return OK;
}

int Sort(Literal* Variety) {
	Literal temp;
	bool exchange;
	for (int i = 0; i < VARIETY_SIZE - 1; i++) {
		exchange = false;
		for (int j = VARIETY_SIZE - 1; j > i; j--) {
			if (abs(Variety[j].Value) < abs(Variety[j - 1].Value)) {
				temp = Variety[j];
				Variety[j] = Variety[j - 1];
				Variety[j - 1] = temp;
				exchange = true;
			}
		}
		if (!exchange)
			return OK;
	}
}
