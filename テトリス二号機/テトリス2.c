#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<process.h>
#include<windows.h>
#include<synchapi.h>




const int MAXFIELD_Y = 20;//
const int MAXFIELD_X = 10;//
const char AIR = '□';//
const char BLOCK = '■';//
const char BR = '\n';//
int x, y;//
int by = 0, bx = 2;//操作ミノの基準座標
char n = 't';//
char* com = &n;//
char field[20][11];//
int block_number;//
int cell[29];
int rb[7] = { 0,0,0,0,0,0,0 };


void Field();//フィールドを初期化
void scan_recieve();//Fieldで初期化、mino_testを呼出
void mino_test();//ミノのデータ、fall呼出
void mino_test2();//関連
int fall();//落下処理、ミノ操作一連の流れの最終地点。mino_testに戻る。
int under_block();//ミノ下側の対ブロック当たり判定
int left_block();//ミノ左側の対ブロック当たり判定
int right_block();//ミノ右側の対ブロック当たり判定
int get_rand();//ランダムに数字を取り出す
int rand_block();
void Bool_Line();//横一列が埋まったか判別する。
void clear_line(int[]);//ブロックを列で消す処理
void turn_righf();//右回転
int under2();///ミノ下側の対フィールド当たり判定
int Left_wall2();//ミノ左側の対フィールド当たり判定
int Right_wall2();//ミノ右側の対フィールド当たり判定
int cell_cmp();//cell変数の中身を比較する。

unsigned ThreadProc(void* p) {



	for (int m = 0; m < 10000; m++) {
		

		scanf("%s", com);

		mino_test2();

		/*
		scanf("%d,%d", &y,&x);
		field[x][y] = BLOCK;
		system("cls");
		for (y = 0; y < MAXFIELD_Y; y++) {//フィールド生成

			for (x = 0; x < MAXFIELD_X + 1; x++) {

				printf("%c", field[y][x]);//座標確認用
			}

		}
		*/


	}
	return 0;
}
int main()
{



	const HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, 0, 0, NULL);
	if (hThread) {

		scan_recieve();
		//test(&cs, '#');
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

}


void scan_recieve() {


	/*char air_check[2] = {'■'};

	if (strcmp(field, air_check) != 0) {*/
	Field();
	

	mino_test();



}


void Field() {
	for (y = 0; y < MAXFIELD_Y; y++) {//フィールド生成

		for (x = 0; x < MAXFIELD_X; x++) {
			field[y][x] = AIR;
			if (x == MAXFIELD_X - 1)
				field[y][MAXFIELD_X] = BR;
		}
		if (y == MAXFIELD_Y - 1)
			field[y][MAXFIELD_X] = '\0';/*配列を終了*/
	}
}
 int rand_block(){
	 int rbcmp[7] = { 1,1,1,1,1,1,1 };
	 while (memcmp(rbcmp, rb, sizeof(rbcmp)) != 0) {
		 int num = (int)(rand() * (6 + 1.0) / (1.0 + RAND_MAX));
		 if (rb[num] == 0) {
			 rb[num] = 1;
			 return num;

		 }
	 }
	memset(rb, 0, sizeof rb);
	int num = (int)(rand() * (6 + 1.0) / (1.0 + RAND_MAX));
	rb[num] = 1;
	return num;
}
void mino_test() {
	char cmp1[2] = { 'a' };
	char cmp2[2] = { 't' };
	int num = rand_block();
	/*01235    操作中のミノを16個のブロックで区別する
	  6789a　　ブロックの座標をint cell_yに入れる
　　　bcdef　　　　　　　　　　int cell_x
	  ghijk　  回転軸は 8番で統一する*/






	printf("%d\n", num);
	if (num == 0) {
		by = 0, bx = 2, cell[2] = 1, cell[8] = 1, cell[14] = 1, cell[15] = 1;
	}
	if (num == 1) {
		by = 0, bx = 2, cell[2] = 1, cell[8] = 1, cell[14] = 1, cell[13] = 1;
	}
	if (num == 2) {
		by = 0, bx = 2, cell[2] = 1, cell[3] = 1, cell[7] = 1, cell[8] = 1;
	}
	if (num == 3) {
		by = 0, bx = 2, cell[1] = 1, cell[2] = 1, cell[8] = 1, cell[9] = 1;
		//Z
	}
	if (num == 4) {
		by = 0, bx = 2, cell[2] = 1, cell[3] = 1, cell[8] = 1, cell[9] = 1;
		//O
	}
	if (num == 5) {
		by = 0, bx = 2, cell[2] = 1, cell[7] = 1, cell[8] = 1, cell[9] = 1;
		//T
	}
	if (num == 6) {
		by = 0, bx = 2, cell[2] = 1, cell[8] = 1, cell[14] = 1, cell[20] = 1;
		//I
	}
	for (int c_num1 = 0; c_num1 < 30; c_num1++)
		if (cell[c_num1] == 1) {
			field[by + c_num1 / 6][bx + c_num1 % 6] = BLOCK;
		}


	

	fall();

}

void mino_test2() {
	char cmp1[2] = { 'a' };
	char cmp2[2] = { 't' };
	char cmp3[2] = { 'd' };
	char cmp4[2] = { 'q' };
	int m1 = Right_wall2();
	int end1 = right_block();
	int m2 = Left_wall2();
	int end2 = left_block();
	if (strcmp(com, cmp1) == 0) {//

		if (m2 == 0 && end2 == 0) {//



			for (int c_num2 = 0; c_num2 < 30; c_num2++) {//
				if (cell[c_num2] == 1) {//
					field[by + c_num2 / 6][bx + c_num2 % 6] = AIR;
				}
			}

			bx--;

			for (int c_num3 = 0; c_num3 < 30; c_num3++) {//
				if (cell[c_num3] == 1) {//
					field[by + c_num3 / 6][bx + c_num3 % 6] = BLOCK;
				}
			}



		}
	}




	if (strcmp(com, cmp3) == 0) {//
		if (m1 == 0 && end1 == 0) {//
			
				for (int c_num2 = 0; c_num2 < 30; c_num2++) {//
					if (cell[c_num2] == 1) {//
						field[by + c_num2 / 6][bx + c_num2 % 6] = AIR;
					}
				}

			bx++;

			for (int c_num3 = 0; c_num3 < 30; c_num3++) {//
				if (cell[c_num3] == 1) {
					field[by + c_num3 / 6][bx + c_num3 % 6] = BLOCK;
				}
			}
		}


	}

	if (strcmp(com, cmp4) == 0) {//
		if (m1 == 0 && end1 == 0) {
			if (m2 == 0 && end2 == 0) {
				int end3 = under_block();
				if (end3 == 0) {
					turn_righf();
				}
			}
		}
	}

}

void turn_righf() {
	/*01235    操作中のミノを16個のブロックで区別する
	  6789a　　
　　　bcdef　　　　　　　　　　
	  ghijk　  回転軸は 8番で統一する*/

	int data[8];
	data[0] = cell[1];
	data[1] = cell[2];
	data[2] = cell[3];
	data[3] = cell[7];
	data[4] = cell[9];
	data[5] = cell[13];
	data[6] = cell[14];
	data[7] = cell[15];
	cell[1] = data[5];
	cell[2] = data[3];
	cell[3] = data[0];
	cell[7] = data[6];
	cell[9] = data[1];
	cell[13] = data[7];
	cell[14] = data[4];
	cell[15] = data[2];

	for (int c_num1 = 0; c_num1 < 30; c_num1++) {//

		field[by + c_num1 / 6][bx + c_num1 % 6] = AIR;
	}
	for (int c_num1 = 0; c_num1 < 30; c_num1++) {//
		if (cell[c_num1] == 1) {
			field[by + c_num1 / 6][bx + c_num1 % 6] = BLOCK;
		}
	}
}

int fall() {
	system("cls");
	printf("%s", field);
	for (int c_num1 = 0; c_num1 < 30; c_num1++) {//
		Sleep(300);
		int tes = under2();
		if (tes == 1) {
			break;
		}

		for (int c_num2 = 0; c_num2 < 30; c_num2++) {//
			if (cell[c_num2] == 1) {
				field[by + c_num2 / 6][bx + c_num2 % 6] = AIR;
			}
		}

		by++;

		for (int c_num3 = 0; c_num3 < 30; c_num3++) {//
			if (cell[c_num3] == 1) {
				field[by + c_num3 / 6][bx + c_num3 % 6] = BLOCK;
			}
		}

		system("cls");
		printf("%s", field);

		int end = under_block();
		if (end == 1) {
			Bool_Line();
			break;
		}
	}
	memset(cell, 0, sizeof cell);
	Bool_Line();
	*com = 't';
	mino_test();
	return 0;
}

int under_block() {
	for (int c_num = 0; c_num < 30; c_num++) {
		if (cell[c_num] == 1)
			if (field[by + c_num / 6][bx + c_num % 6] == field[by + 1 + c_num / 6][bx + c_num % 6]) {
				for (int c_num2 = c_num + 1; c_num2 < 30; c_num2++) {
					if (&field[by + 1 + c_num / 6][bx + c_num % 6] == &field[by + c_num2 / 6][bx + c_num2 % 6])
						if (cell[c_num2] != 1)
							return 1;

				}
			}
	}
	return 0;
}

int left_block() {
	for (int c_num = 0; c_num < 30; c_num++) {
		if (cell[c_num] == 1)
			if (field[by + c_num / 6][bx + c_num % 6] == field[by + c_num / 6][bx - 1 + c_num % 6]) {
				for (int c_num2 = 0; c_num2 < 30; c_num2++) {
					if (&field[by + c_num / 6][bx - 1 + c_num % 6] == &field[by + c_num2 / 6][bx + c_num2 % 6])
						if (cell[c_num2] != 1)
							return 1;

				}
			}
	}
	return 0;
}
int right_block() {
	for (int c_num = 0; c_num < 30; c_num++) {
		if (cell[c_num] == 1)
		if (field[by + c_num / 6][bx + c_num % 6] == field[by + c_num / 6][bx + 1 + c_num % 6]) {
			for (int c_num2 = 0; c_num2 < 30; c_num2++) {
				if (&field[by + c_num / 6][bx + 1 + c_num % 6] == &field[by + c_num2 / 6][bx + c_num2 % 6])
					if (cell[c_num2] != 1)
						return 1;

			}
		}
	}
	return 0;
}

int get_rand() {
	int num;
	do {
		num = rand() % 10;

	} while (num < 7);
	return num;

}

void Bool_Line() {
	int Bool_lene[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	for (int num = 0; num < 20; num++) {
		for (int NUM = 0; NUM < 10; NUM++) {
			if (field[num][NUM] != BLOCK) {
				break;
			}
			else if (NUM == 9) {
				Bool_lene[num] = 1;//num列の横一列がすべてブロックなら１を入れる

			}
		}

	}
	clear_line(Bool_lene);
}

void clear_line(int Bool_lene2[]) {
	for (int num = 0; num < 20; num++) {// 配列の中身を０～１９まで順に確認 
		if (Bool_lene2[num] == 1) {//配列の中身が１なら以下の処理を開始、Bool_lene2[num]はnum列が一列埋まった場合１を示す
			for (int NUMBER = 0; NUMBER < 10; NUMBER++) {
				field[num][NUMBER] = AIR;
				if (NUMBER == 9) {
					for (int number = 0; number < 10; number++) {
						for (int ynum = 0; num - 1 - ynum >= 0; ynum++) {
							if (field[num - 1 - ynum][number] == BLOCK) {
								field[num - 1 - ynum][number] = AIR;

								field[num - ynum][number] = BLOCK;
							}
						}
					}
				}
			}
		}

	}
}

int under2() {
	for (int c_num = 0; c_num < 30; c_num++) {

		if (cell[c_num] == 1 && (by + c_num / 6) > 18) {
			return 1;
		}
	}
	return 0;
}
int Left_wall2() {

	for (int c_num = 0; c_num < 19; c_num++) {

		if (cell[c_num] == 1 && (bx + c_num % 6) <= 0) {
			return 1;

		}
	}
	return 0;
}
int Right_wall2() {

	for (int c_num = 0; c_num < 19; c_num++) {

		if (cell[c_num] == 1 && (bx + c_num % 6) > 8) {
			return 1;

		}
	}
	return 0;
}
int cell_cmp() {
	for (int c_num3 = 0; c_num3 < 16; c_num3++) {
		if (cell[c_num3] == 1)

		for (int c_num2 = 0; c_num2 < 16; c_num2++) {
			if (cell[c_num2] == 1) {
				if (&field[by + 1 + c_num3 / 4][bx + c_num3 % 4] != &field[by + c_num2 / 4][bx + c_num2 % 4]) {
					return 1;
				}
			}
		}
	}
	return 0;
}

