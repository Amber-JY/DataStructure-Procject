/*********************************************************************
*This head file contains the statements of input and output functions*
*to be used in both screen I/O and file I/O.Besides, the functions	 *
*about sukudo visualization are also delared in this part.           *
*********************************************************************/
#ifndef _IN_OUT_H_
#define _IN_OUT_H_


Clause* ReadFile(Clause* ClauseBase);
int PutoutData(Clause* ClauseBase);
int SaveAsFile(Clause* ClauseBase, bool PASS, double time);
int PushVariety(Literal* Variety, int v);
int Sort(Literal* Variety);


#endif // !_IN_OUT_H_
