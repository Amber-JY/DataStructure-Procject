/*************************************************************************
*This head file is used to provide the data structures and the statements*
*of basic operations of these structures, such as creating the structures*
*, adding element, deleting element(changing state of the node),etc..    *                          *
*************************************************************************/
#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

/*
*The following varieties are used to initialize the literal array and clause
*array. Besides, INCREASE is used to resize the array.
*/

#define INCREASE 100
#define FALSE 0
#define ERROR -1
#define OK 1

extern int CLAUSE_SETSIZE;
extern int VARIETY_SIZE;
extern int LITERAL_SETSIZE;

/*
*The following strutures are the basic data structures of SAT-solver
*1.Literal is the data strcture of literal.
*2.Clause is the data structure of clause.
*3.Store is the data structure of locations of literal being deleted, used
*to recover the state when the 1st step of single-literal-rule is false.
*/

typedef struct LiteralStructure {
	int Value = 0;//The value of current literal.
	int Location;//The location of current literal in its clause.
	int State = 0;//Deleted(0) or not(1).
	//Literal* Next;//The point to next literal.
}Literal;

/******************CANTION*****************************
*The first clause is used to store clause set's length.
******************************************************/
typedef struct ClauseStructure {
	int Location;//The location of current clause in clause set.
	long Length = LITERAL_SETSIZE * 100 + LITERAL_SETSIZE;//The length of clause, 0 means it's empty.
	bool State = 0;//Deleted(0) or not(1).
	Literal* Sub = (Literal*)malloc(sizeof(Literal));//The point to the first literal in this clause.
	//Clause* Next;//The point to next Clause.
}Clause;

typedef struct StoreStructure {
	int Literal_Location = 0;
	int Clause_Location = 0;
}Store;


/*
*Here are the statements of basic functions of above_mentioned data
*structures.
*/

Clause* ClauseSet_init();

Literal* LiteralSet_init();

Literal* VarietySet_init();

Clause* ClauseResize(Clause* base, int num_Clause);

Literal* LiteralResize(Literal* base, int num_Literal);

int CopyClauseSet(Clause* Clausebase, Clause* Memory);

#endif // !_DATA_STRUCTURE_H_

