#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <pthread.h>

//키패드 정의
#define C1 24
#define C2 23
#define C3 18
#define C4 15
#define R1 25
#define R2 8
#define R3 7
#define R4 1
//세그먼트 정의
#define A 17
#define F 27
#define B 22
#define G 5
#define C 6
#define D 19
#define E 26
#define H 13

#define L1 21 
#define L2 20
#define L3 16
#define L4 12

#define ROW 4 		//키패드 열 정의
#define COL 3		//키패드 행	정의
#define NCOL 4		//배열 Number 열 정의 
#define SROW 10	//세그먼트 열	정의
#define SCOL 8		//세그먼트 행	정의

char key[ROW][COL] = {
		{'1','2','3'},
		{'4','5','6'},
		{'7','8','9'},
		{'*','0','#'}
}; //키패드 출력배열

int seg[SROW][SCOL] = {
		{1,1,1,1,1,1,0,0}, //0
		{0,1,1,0,0,0,0,0}, //1
		{1,1,0,1,1,0,1,0}, //2
		{1,1,1,1,0,0,1,0}, //3
		{0,1,1,0,0,1,1,0}, //4
		{1,0,1,1,0,1,1,0}, //5
		{1,0,1,1,1,1,1,0}, //6
		{1,1,1,0,0,1,0,0}, //7
		{1,1,1,1,1,1,1,0}, //8
		{1,1,1,1,0,1,1,0}  //9
}; //세그먼트 출력 배열

int Pin[] = { A,B,C,D,E,F,G,H };
int Line[] = { L1, L2, L3, L4 };
int row[ROW] = { R1,R2,R3,R4 };
int col[COL] = { C1,C2,C3 };
char inputkey[5];
char userinput = 0;
void Setup();
int Keypad();
int CastingNum(char CN);
void elevator();
void SegNum(int);
int current = 1;
int Number1 = 0;

int main()
{
	wiringPiSetupGpio();
	Setup();

	while (1) {
		elevator();
	}
	return 0;
}
void Setup() //키, 세그먼트 초기화
{
	for (int i = 0; i < ROW; i++) {
		pinMode(row[i], OUTPUT);
		for (int j = 0; j < COL; j++) {
			pinMode(col[j], INPUT);
			pullUpDnControl(col[j], PUD_UP);
		}
	}
	for (int k = 0; k < sizeof(Pin) / sizeof(Pin[0]); k++) {
		pinMode(Pin[k], OUTPUT);
	}
	for (int n = 0; n < sizeof(Line) / sizeof(Line[0]); n++) {
		pinMode(Line[n], OUTPUT);
		digitalWrite(Line[n], HIGH);
	}
}
int Keypad() //키패드 입력
{
	for (int i = 0; i < ROW; i++) {
		digitalWrite(row[i], LOW);
		for (int j = 0; j < COL; j++) {
			if (digitalRead(col[j]) == LOW) {
				userinput = key[i][j];
				printf("input : %c\n", userinput);
				delay(200);
			}
		}
		digitalWrite(row[i], HIGH);
	}
	return userinput;
}
int CastingNum(char CN) // 입력받은 문자를 정수로 변환
{
	inputkey[0] = '\0';
	CN = Keypad();
	int target = CN - '0';
	if (target >= 0 && target <= 9) {
		return target;
	}
	else { return CN; }
}
void elevator() // 변환된 정수만큼 엘레베이터 이동
{
	int Number;
	char inputchar;

	Number = CastingNum(inputchar);

	if (Number >= 0 && Number <= 9) {
		Number1 = Number1 * 10 + Number;
	}
	else if(Number == '*'){
		if (Number1 > current) {
			for (int floor = current; floor <= Number1; floor++) {
				SegNum(floor);
				current = Number1;
			}
		}
		else if (Number1 < current) {
			for (int floor = current; floor >= Number1; floor--) {
				SegNum(floor);
				currnet = Number1;
			}
		}
		Number1 = 0;
	}
}
void SegNum(int floor) // 세그먼트 표시
{
	int Number[NCOL];
	Number[0] =  floor % 10;
	Number[1] = (floor /10)%10;
	Number[2] = (floor /100)%10;
	Number[3] = (floor /1000)%10;
	for(int i = 0; i<10; i++){
		for(int j=0; j<sizeof(Number)/sizeof(Number[0]);j++){
			digitalWrite(Line[j], LOW);
			for(int k=0; k<SCOL; k++){
				digitalWrite(Pin[k],seg[Number[j]][k]);
			}
			delay(5);
			digitalWrite(Line[j],HIGH);
		}
	}
}
	
/*void SegNum(int floor)
{	
	for(int j=0; j<SCOL;j++){
			digitalWrite(Line[0], LOW);
			digitalWrite(Pin[j],seg[Number[0]][j]);
		}
		delay(5);
		digitalWrite(Line[0], HIGH);
		for(int j=0; j<SCOL; j++){
			digitalWrite(Line[1], LOW);
			digitalWrite(Pin[j], seg[Number[1]][j]);
		}
		delay(5);
		digitalWrite(Line[1], HIGH);
		for(int j=0; j<SCOL; j++){
			digitalWrite(Line[2], LOW);
			digitalWrite(Pin[j], seg[Number[2]][j]);
		}
		delay(5);
		digitalWrite(Line[2], HIGH);
		for(int j=0; j<SCOL; j++){
			digitalWrite(Line[3], LOW);
			digitalWrite(Pin[j], seg[Number[3]][j]);
		}
		delay(5);
		digitalWrite(Line[3], HIGH);
	}
}*/
