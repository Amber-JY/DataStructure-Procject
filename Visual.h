#ifndef _VISUAL_H_
#define _VISUAL_H_

int InitBoard(Clause* SingleSet, int level, int(* So)[16]);
int ShowBoard_auto(Clause* ClauseBase, int level);
int ShowBoard_manu(int level, int(*So)[16]);

#endif // !_VISUAL_H_
