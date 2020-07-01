/*

*/

#ifndef _SOKUDOTOCNF_H_
#define _SOKUDOTOCNF_H_


Clause* InitialSokudo(char name[10], int level);
int Con_2nd_L(int level, int Row, int Line, int data[10], FILE* out);
int Con_2nd_R(int level, int Row, int Line, int data[10], FILE* out);
int Constraint(Clause* SingleSet, char name[10]);

#endif//! _SOKUDOTOCNF_H_
