/***************************************************************************
*This source file contains the detail of basic functions of data structure *
*used in this Solution.	                                                   *
***************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"Data structure.h"

int CLAUSE_SETSIZE = 200;
int VARIETY_SIZE = 0;
int LITERAL_SETSIZE = 20;

/*
*This function is used to initialize the clause set.
*@output:The address of clause set.
*/
Clause* ClauseSet_init() {
	Clause* base = (Clause*)malloc(sizeof(Clause) * CLAUSE_SETSIZE);
	return base;
}

/*
*This function is used to initialize the literal set.
*@output:The address of literal set.
*/
Literal* LiteralSet_init() {
	Literal* base = (Literal*)malloc(sizeof(Literal) * LITERAL_SETSIZE);
	for (int i = 0; i < LITERAL_SETSIZE; i++) {
		base[i].Location = 0;
		base[i].Value = 0;
		base[i].State = 0;
	}
	
	return base;
}

Literal* VarietySet_init() {
	Literal* base = (Literal*)malloc(sizeof(Literal) * VARIETY_SIZE);
	for (int i = 0; i < VARIETY_SIZE; i++) {
		base[i].Location = i;
		base[i].State = 0;
		base[i].Value = -1;
	}
	return base;
}

Clause* ClauseResize(Clause* base, int num) {
	base = (Clause*)realloc(base, sizeof(Clause) * num);
	if (!base) {
		printf("Insufficient storage space!\n");
		exit(-1);
	}
	return base;
}

Literal* LiteralResize(Literal* base, int num) {
	base = (Literal*)realloc(base, sizeof(Literal) * num);
	if (!base) {
		printf("Insufficient storage space!\n");
		exit(-1);
	}
	/*for (int i = num_Literal; i < num_Literal + INCREASE; i++) {
		base[i].Location = 0;
		base[i].Value = 0;
		base[i].State = 0;
	}*/
	return base;
}


/*
*/

int CopyClauseSet(Clause* Clausebase, Clause* Memory) {
	int num_Clause = 0;
	int num_Literal = 0;
	if (Clausebase == NULL || Memory == NULL) {
		printf("The address is NULL!\n");
		exit(ERROR);
	}
	while (num_Clause <= (Clausebase->Length / 5000)) {
		Memory[num_Clause].Length = Clausebase[num_Clause].Length;
		Memory[num_Clause].Location = Clausebase[num_Clause].Location;
		Memory[num_Clause].State = Clausebase[num_Clause].State;
	
		Literal* LiteralBase = LiteralSet_init();
		if (num_Clause == 0) {
			if(VARIETY_SIZE > LITERAL_SETSIZE)
				LiteralBase = LiteralResize(LiteralBase, VARIETY_SIZE);
			while (num_Literal < VARIETY_SIZE) {
				LiteralBase[num_Literal].Location = Clausebase[num_Clause].Sub[num_Literal].Location;
				LiteralBase[num_Literal].State = Clausebase[num_Clause].Sub[num_Literal].State;
				LiteralBase[num_Literal].Value = Clausebase[num_Clause].Sub[num_Literal].Value;
				num_Literal++;
			}
		}
		else 
			while (num_Literal < Clausebase[num_Clause].Length / 200) {
				LiteralBase[num_Literal].Location = Clausebase[num_Clause].Sub[num_Literal].Location;
				LiteralBase[num_Literal].State = Clausebase[num_Clause].Sub[num_Literal].State;
				LiteralBase[num_Literal].Value = Clausebase[num_Clause].Sub[num_Literal].Value;
				num_Literal++;
			}
		Memory[num_Clause].Sub = LiteralBase;
		num_Clause++;
		num_Literal = 0;
	}
	return OK;
}
