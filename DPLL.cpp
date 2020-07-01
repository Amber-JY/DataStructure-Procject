/***************************************************************************
*This source file contains the detail of basic functions of DPLL.          *
***************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"DPLL.h"
#include"IN_OUT.h"


/*
*Judge if single clause exists.
*@output:
*num_Clause(Exist.The location of single clause.) or FALSE(Didn't exist.).
*/
int SinClauseExist(Clause* ClauseBase, Literal* Variety) {
	int num_Clause = 1;
	int num_variety = 0;
	while (num_Clause <= (ClauseBase->Length / 5000)) {
		if ((ClauseBase + num_Clause)->State == 1 && (ClauseBase + num_Clause)->Length % 200 == 1) {
			while (num_variety < VARIETY_SIZE) {
				for (int i = 0; i < (ClauseBase + num_Clause)->Length / 200; i++) {
					if (Variety[num_variety].State == 1 && Variety[num_variety].Value == abs((ClauseBase + num_Clause)->Sub[i].Value)) {
						if ((ClauseBase + num_Clause)->Sub[i].Value > 0)
							Variety[num_variety].State = 0;
						else if ((ClauseBase + num_Clause)->Sub[i].Value < 0)
							Variety[num_variety].State = -1;
						break;
					}
				}
				num_variety++;
			}//while
			return num_Clause;
		}//if
		num_Clause++;
	}//while
	return FALSE;
}


/*
*Delete clause contains true v or literal equals to non_v in all clauses.
*@parameter:
*ClauseBase:The base address of clause set.
*v:The variety chosen by some strategy.
*/
int SimpClause(Clause* ClauseBase, int v) {
	int num_Clause = 1;
	int num_Literal = 0;
	int length = ClauseBase->Length;//The backup of length.
	if (!ClauseBase) {
		printf("The base address is error!\n ");
		return ERROR;
	}
	while (num_Clause <= (ClauseBase->Length / 5000)) {
		//printf("%d %d", ClauseBase[num_Clause].State, ClauseBase[num_Clause].Length);//test
		if (ClauseBase[num_Clause].State == 1 && ClauseBase[num_Clause].Length % 200 != 0) {
			while (num_Literal < ClauseBase[num_Clause].Length / 200) {
				if (ClauseBase[num_Clause].Sub[num_Literal].Value == v) {
					ClauseBase[num_Clause].State = 0;
					length--;
					break;
					}
				num_Literal++;
			}//End of inwhile
			num_Literal = 0;
		}//End of if
		num_Clause++;
	}//End of outwhile
	ClauseBase->Length = length;
	//if clause set is empty, return OK.
	if ((ClauseBase->Length % 5000) == 0)
		return OK;
	//else, delete non-v in every clause.
	num_Clause = 1;
	num_Literal = 0;
	while (num_Clause <= (ClauseBase->Length / 5000)) {
		if (ClauseBase[num_Clause].State == 1 && ClauseBase[num_Clause].Length % 200 != 0) {
			while (num_Literal < ClauseBase[num_Clause].Length / 200) {
				if (ClauseBase[num_Clause].Sub[num_Literal].Value == -v) {
					ClauseBase[num_Clause].Sub[num_Literal].State = 0;
					ClauseBase[num_Clause].Length--;
					break;
				}
				num_Literal++;
			}//End of inwhile
			num_Literal = 0;
		}
		num_Clause++;
	}//End of outwhile
	return OK;
}


/*
*This function aims to take some strategy to choose a variety
*/
int ChooseV(Literal* Variety) {
	int num_variety = 0;
	while (num_variety < VARIETY_SIZE) {
		if (Variety[num_variety].State == 1) {
			Variety[num_variety].State = 0;
			return Variety[num_variety].Value;
		}
		num_variety++;
	}
	printf("Search Over.\n");
	return -1;
}



/*
*This function aims to add the variety chosen to the last position of clause set.
*@parameter:
*ClauseBase:The base address of clause set.
*v:The variety is chosen.
*/
int AddToTail(Clause* ClauseBase, int v) {
	if (!ClauseBase) {
		printf("The base address is error!\n ");
		return ERROR;
	}
	ClauseBase->Length = (ClauseBase->Length / 5000) * 5000 + ClauseBase->Length % 5000 + 1;
	ClauseBase[ClauseBase->Length / 5000].State = 1;
	ClauseBase[ClauseBase->Length / 5000].Length = 201;
	ClauseBase[ClauseBase->Length / 5000].Location = ClauseBase->Length / 5000;
	ClauseBase[ClauseBase->Length / 5000].Sub->Location = 0;
	ClauseBase[ClauseBase->Length / 5000].Sub->State = 1;
	ClauseBase[ClauseBase->Length / 5000].Sub->Value = v;
	return OK;
}


/*
*Resursion to run DPLL.
*Putout:
*OK(satisfiable) or FALSE(unsatisfiable) or ERROR(error).
*/
int DPLL(Clause* ClauseBase) {
	int location;//The location of first single clause.
	int num_Literal = 0;
	int num_Clause = 1;
	int v;//The variety been chosen.
	int state;
	int SinValue;//The single clause is chosen;
	Clause* Memory = ClauseSet_init();
	Clause* Current = ClauseSet_init();
	while ((location = SinClauseExist(ClauseBase, ClauseBase[0].Sub)) != FALSE) {
		for (int i = 0; i < ClauseBase[location].Length / 200; i++)//Find the single value.
			if (ClauseBase[location].Sub[i].State == 1) {
				SinValue = ClauseBase[location].Sub[i].Value;
				break;
			}
		if(SimpClause(ClauseBase, SinValue)==ERROR)
			exit(ERROR);
		//Here aims to show the process of DPLL, but costs too much time.
		//printf("Begin\n-------------------------------------------\n");
		//PutoutData(ClauseBase);
		//printf("-------------------------------------------\nEnd\n\n");
		if ((ClauseBase->Length % 5000) == 0)
			return OK;
		while (num_Clause <= (ClauseBase->Length / 5000)) {
			if (ClauseBase[num_Clause].State == 1 && ClauseBase[num_Clause].Length % 200 == 0)
				return FALSE;
			num_Clause++;
		}
		num_Clause = 0;
	}//End of while

	CopyClauseSet(ClauseBase, Current);
	CopyClauseSet(Current, Memory);
	//positive literal
	if ((v = ChooseV(Current[0].Sub)) == -1)
		exit(ERROR);
	if (AddToTail(Current, v) == OK) {
		state = DPLL(Current);
		if ( state == OK) {
			for (int i = 0; i < VARIETY_SIZE; i++) {
				ClauseBase->Sub[i].Value = Current->Sub[i].Value;
				ClauseBase->Sub[i].State = Current->Sub[i].State;
			}	
			free(Current);
			free(Memory);
			return OK;
		}//if
		free(Current);
	}//if
	
	//negative literal
	if (AddToTail(Memory, -v) == OK) {
		state = DPLL(Memory);
		if(state)
			for (int i = 0; i < VARIETY_SIZE; i++) {
				ClauseBase->Sub[i].Value = Memory->Sub[i].Value;
				ClauseBase->Sub[i].State = Memory->Sub[i].State;
			}
		free(Memory);
		return state;
	}//if
	return FALSE;
}

