/***************************************************************************
*This source file contains the detail of basic functions of SokudoToCNF.   *
***************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"Data structure.h"
#include"SokudoToCNF.h"
#include"IN_OUT.h"


/**/
Clause* InitialSokudo(char name[10], int level){
	char name_[10] = { '\0' }, _name_[10] = { '\0' };
	char path[50] = "./Initial/";
	char path_[50] = "./Initial/";//Relative path.
	for (int i = 0; name[i] != '\0'; i++){
		_name_[i] = name[i];
		name_[i] = name[i];
	}

	strcat(_name_, ".txt");
	strcat(path, _name_);
	FILE* fp = NULL;
	FILE* out = NULL;
	if ((fp = fopen(path, "r+")) == NULL){
		printf("Can't open that file!\n");
		exit(ERROR);
	}
	int content[64] = {0};
	int num_content = 0;
	Clause* SingleSet = ClauseSet_init();
	Literal* Variety = LiteralSet_init();
	while (fscanf(fp, "%d", &content[num_content]) != EOF) {
		if (num_content > LITERAL_SETSIZE)
			Variety = LiteralResize(Variety, num_content + INCREASE);
		Variety[num_content].Location = num_content;
		Variety[num_content].State = 1;
		Variety[num_content].Value = content[num_content];
		num_content++;
	}
	SingleSet->Length = num_content * 5000 + num_content;
	SingleSet->Location = 0;
	SingleSet->State = 0;
	SingleSet->Sub = Variety;
	for (int i = 1; i <= SingleSet->Length / 5000; i++) {
		Literal* Literal = LiteralSet_init();
		SingleSet[i].Sub = Literal;
		SingleSet[i].Length = 1;
		SingleSet[i].Location = i;
		SingleSet[i].State = 1;
		SingleSet[i].Sub->Location = 0;
		SingleSet[i].Sub->State = 1;
		SingleSet[i].Sub->Value = content[i - 1];
	}
	fclose(fp);
	strcat(name_, ".cnf");
	strcat(path_, name_);
	if ((out = fopen(path_, "w+")) == NULL) {
		printf("Can't create that file!\n");
		exit(ERROR);
	}

	//Calculate the total variety and clause number in this puzzle.
	int num_va = pow(level, 2) + (3 * level + 1) * (level - 1) * level;
	int num_cl = num_content + 4 * level * (level - 2)
		+ (level - 1) * level * (10 * level + 1);
	int temp_1 = 1, temp_2 = 1;
	for (int i = 1; i <= level / 2 - 1; i++)
	{
		temp_1 *= i;
		temp_2 *= level + 1 - i;
	}
	num_cl += (temp_2 / temp_1) * 4 * level;//

	fprintf(out, "%s %d %d\n", "p cnf", num_va, num_cl);
	for (int i = 1; i <= SingleSet->Length / 5000; i++) {
		fprintf(out, "%d 0\n", SingleSet[i].Sub->Value);
	}
	fclose(out);
	return SingleSet;
}

/**/
int Con_2nd_L(int level, int Row, int Line, int data[10], FILE* out) {
	int count = Row; char content[100];//
	while (count <= ((level + Row) % 2 ? (level + Row) / 2 : (level + Row) / 2 + 1)) {
		data[Row - 1] = Line * 10 + count;
		if (Row < level / 2 + 1) Con_2nd_L(level, Row + 1, Line, data, out);
		else {
			setbuf(out, content);
			for (int i = 0; i < level / 2 + 1; i++) {
				fprintf(out, "%d ", data[i]);
				printf("%d ", data[i]);//
			}
			for (int i = 0; i < level / 2 + 1; i++) {
					fprintf(out, "%d ", -data[i]);
					printf("%d ", -data[i]);//

			}
			fprintf(out, "0\n");
			printf("0\n");
			fflush(out);//
		}
		count++;
	}//While
	return OK;
};

/**/
int Con_2nd_R(int level, int Row, int Line, int data[10], FILE* out) {
	int count = Line; 
	while (count <= ((level + Row) % 2 ? (level + Row) / 2 : (level + Row) / 2 + 1)) {
		data[Line - 1] = (Line - 1) * 10 + count;
		if (Line < level / 2 + 1) Con_2nd_R(level, Row + 1, Line, data, out);
		else {
			for (int i = 0; i < level / 2 + 1; i++) {
				if (i != level / 2)
					fprintf(out, "%d ", data[i]);
				else fprintf(out, "%d\n", data[i]);
			}
			for (int i = 0; i < level / 2 + 1; i++) {
				if (i != level / 2)
					fprintf(out, "%d ", -data[i]);
				else fprintf(out, "%d\n", -data[i]);
			}
			fprintf(out, "0\n");
		}
		count++;
	}//While
	return OK;
};

/**/
int Constraint(Clause* SingleSet, char name[10]){
	int level = name[0] - '0';
	char path[40] = "./Initial/";
	char content[32];
	int content_1[8];//Every three items is a clause.
	int content_2[10];
	strcat(name, ".cnf");
	strcat(path, name);
	FILE* out = NULL;
	if ((out = fopen(path, "a+")) == NULL) {
		printf("Can't open that file!\n");
		exit(ERROR);
	}

	//Constraint[1]
	for (int j = 1; j <= level; j++) {//line
		for (int i = 1; i <= level - 2; i++) {//row
			content_1[0] = j * 10 + i;
			content_1[1] = j * 10 + i + 1;
			content_1[2] = j * 10 + i + 2;
			fprintf(out, "%d %d %d 0\n", content_1[0], content_1[1], content_1[2]);
			fprintf(out, "%d %d %d 0\n", -content_1[0], -content_1[1], -content_1[2]);
		}
	}
	for (int i = 1; i <= level; i++) {
		for (int j = 1; j <= level - 2; j++) {
			content_1[0] = j * 10 + i;
			content_1[1] = (j + 1) * 10 + i;
			content_1[2] = (j + 2) * 10 + i;
			fprintf(out, "%d %d %d 0\n", content_1[0], content_1[1], content_1[2]);
			fprintf(out, "%d %d %d 0\n", -content_1[0], -content_1[1], -content_1[2]);
		}
	}

	//Constraint[2]

	for (int i = 1; i <= level; i++) {
		Con_2nd_L(level, 1, i, content_2, out);
		Con_2nd_R(level, i, 1, content_2, out);
	}

	//Constraint[3]
	int Add_1, Add_2, Add_3_T, Add_3_F;//157   1571    15711
	int num_1, num_2;
	int temp[15];
	for (int line_1 = 1; line_1 < level; line_1++) {
		for (int line_2 = line_1 + 1; line_2 <= level; line_2++) {
			for (int row = 1; row <= level; row++) {
				num_1 = line_1 * 10 + row;
				num_2 = line_2 * 10 + row;
				Add_3_T = 10000 + line_1 * 1000 + line_2 * 100 + row + 1;
				Add_3_F = 10000 + line_1 * 1000 + line_2 * 100 + row;
				fprintf(out, "%d %d 0\n", num_1, -Add_3_T);
				fprintf(out, "%d %d 0\n", num_2, -Add_3_T);
				fprintf(out, "%d %d %d 0\n", -num_1, -num_2, Add_3_T);
				fprintf(out, "%d %d 0\n", -num_1, -Add_3_F);
				fprintf(out, "%d %d 0\n", -num_2, -Add_3_F);
				fprintf(out, "%d %d %d 0\n", num_1, num_2, Add_3_F);
				Add_2 = 1000 + line_1 * 100 + line_2 * 10 + row;
				fprintf(out, "%d %d 0\n", -Add_3_T, Add_2);
				fprintf(out, "%d %d 0\n", -Add_3_F, Add_2);
				fprintf(out, "%d %d %d 0\n", Add_3_T, Add_3_F, -Add_2);
				temp[row - 1] = Add_2;
			}//for
			Add_1 = 100 + line_1 * 10 + line_2;
			for (int i = 0; i <= level; i++) {
				fprintf(out, "%d %d 0\n", temp[i], Add_1);
			}
			for (int i = 0; i <= level; i++) {
				fprintf(out, "%d", -temp[i]);
				if (i == level)
					fprintf(out, "%d 0\n", -Add_1);
				else fprintf(out, " ");
			}
		}//for
	}//for

	for (int row_1 = 1; row_1 < level; row_1++) {
		for (int row_2 = row_1 + 1; row_2 <= level; row_2++) {
			for (int line_1 = 1; line_1 <= level; line_1++) {
				num_1 = line_1 * 10 + row_1;
				num_2 = line_1 * 10 + row_2;
				Add_3_T = 20000 + row_1 * 1000 + row_2 * 100 + line_1 + 1;
				Add_3_F = 20000 + row_1 * 1000 + row_2 * 100 + line_1;
				fprintf(out, "%d %d 0\n", num_1, -Add_3_T);
				fprintf(out, "%d %d 0\n", num_2, -Add_3_T);
				fprintf(out, "%d %d %d 0\n", -num_1, -num_2, Add_3_T);
				fprintf(out, "%d %d 0\n", -num_1, -Add_3_F);
				fprintf(out, "%d %d 0\n", -num_2, -Add_3_F);
				fprintf(out, "%d %d %d 0\n", num_1, num_2, Add_3_F);
				Add_2 = 2000 + row_1 * 100 + row_2 * 10 + line_1;
				fprintf(out, "%d %d 0\n", -Add_3_T, Add_2);
				fprintf(out, "%d %d 0\n", -Add_3_F, Add_2);
				fprintf(out, "%d %d %d 0\n", Add_3_T, Add_3_F, -Add_2);
				temp[line_1 - 1] = Add_2;
			}//for
			Add_1 = 100 + row_1 * 10 + row_2;
			for (int i = 0; i <= level; i++) {
				fprintf(out, "%d %d 0\n", temp[i], Add_1);
			}
			for (int i = 0; i <= level; i++) {
				fprintf(out, "%d", -temp[i]);
				if (i == level)
					fprintf(out, "%d 0\n", -Add_1);
				else fprintf(out, " ");
			}
		}//for
	}//for
	fclose(out);
	return OK;
}