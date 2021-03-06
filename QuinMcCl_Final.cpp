#pragma warning(disable: 4996)
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include <string>

using namespace std;

int MakeColumn(string*, string*, int*);		//다음 칼럼 생성 함수
void CopyPrime(string*, string*, int*, int*); 
void reset(string*);
bool CheckFinish(string*);
bool CheckSame(string*, string*, int, int);
void LastPrime(string*, string*, int*);
void ShowPrime(string*);

int CheckNumberOne(int*, int);
int MakeTransNum(int, string*);
int CheckAnd(int, string*);
int CheckNot(int, string*);
bool CheckBreak(int**, int, int);

int main()
{
	string line;
	string* column = new string[1000]; //홀수번 칼럼
	string* column2 = new string[1000]; //짝수번 칼럼
	string* prime = new string[1000]; //Prime Implicant 배열
	int* usage = new int[1000]; //column의 각 행의 사용횟수를 측정. 0인 행은 prime implicant로 옮겨주면 됨.
	string* true_minterm = new string[1000];
	int c_row = 0, t_row = 0;

	int TMCount = 0, PICount = 0, EssentialPICount = 0;
	string* PIColumn = nullptr, *TMRow = nullptr;  //PI를 저장할 열과 TM을 저장할 행
	int** PITable = nullptr;
	string* EssentialPI = nullptr;

	ifstream fin("input_minterm.txt");
	if (!fin.is_open())			//파일 열기 실패 시, 에러문 출력하고 프로그램 종료
	{
		cout << "false" << endl;
		return 1;
	}
	while (!fin.eof())
	{
		getline(fin, line);
		if (line[0] == 'm')				//True Minterm 읽어서 저장
		{
			column[c_row] = line.substr(2);
			true_minterm[t_row] = line.substr(2);
			t_row++;
			c_row++;
		}
		else if (line[0] == 'd')			//Don't Care 읽어서 
		{
			column[c_row] = line.substr(2);
			c_row++;
		}
	}
	fin.close();
	cout << "column" << endl;				//칼럼1 출력
	for (int i = 0; column[i].empty() != true; i++)
		cout << column[i] << endl;
	cout << endl;
	int ColumnNum = 0;
	int p = 0; //prime implicant 갯수
	while (1)
	{
		if (ColumnNum % 2 == 0)					// 짝수번 칼럼 출력 
		{
			MakeColumn(column2, column, usage);
			CopyPrime(prime, column, usage, &p);
			if (CheckFinish(column2) == true) break;    //마지막 칼럼 확인시 반복문 탈출
			reset(column);
			ColumnNum++;
		}
		else							// 홀수번 칼럼 출력
		{
			MakeColumn(column, column2, usage);
			CopyPrime(prime, column2, usage, &p);
			if (CheckFinish(column) == true) break;
			reset(column2);
			ColumnNum++;
		}
	}
	//Last Column => 마지막 칼럼을 전부 prime 배열로 옮김
	if (ColumnNum % 2 == 0)
		LastPrime(column2, prime, &p);
	else
		LastPrime(column, prime, &p);
	//prime 배열에 저장된 모든 prime implicant를 출력
	ShowPrime(prime);

	//Essential PI 구하기

	TMCount = t_row;
	PICount = p;

	/* 개수별로 동적할당 */
	TMRow = new string[TMCount];
	PIColumn = new string[PICount];
	EssentialPI = new string[PICount];

	/* PI 입력 */
	for (int i = 0; i < PICount; i++)
		PIColumn[i] = prime[i];

	/* TM 입력 */
	for (int i = 0; i < TMCount; i++)
		TMRow[i] = true_minterm[i];

	/* PITable 만들기 */
	PITable = new int*[PICount + 1];   //마지막 확인용 행 때문에 + 1
	for (int i = 0; i < PICount + 1; i++)
		PITable[i] = new int[TMCount];

	/* PITable 전부 0으로 초기화 */
	for (int i = 0; i < PICount + 1; i++)
	{
		for (int j = 0; j < TMCount; j++)
			PITable[i][j] = 0;
	}

	/* TM과 PI 비교해서 테이블 표시 (O : 0, X : 1) */
	for (int i = 0; i < PICount; i++)
	{
		for (int j = 0; j < TMCount; j++)
		{
			int count = 0;
			for (unsigned int k = 0; k < PIColumn[i].length(); k++)
			{
				if (PIColumn[i][k] == TMRow[j][k]) count++;
				else if (PIColumn[i][k] == '-') count++;
			}
			if (count == PIColumn[i].length()) PITable[i][j] = 1;
		}
	}
       /*PITable에 1이 한개인 열은 해당 PI를 EPI에 저장하고 해당 열 0으로 초기화*/ 
	for (int i = 0; i < TMCount; i++)	//1이 한개인 열 찾기
	{
		int PIRowIdx, PIColumnIdx;
		int OneCount = 0;		//1의 수 체크하는 변수
		for (int j = 0; j < PICount; j++)
		{
			if (PITable[j][i] == 1)
			{
				OneCount++;
				PIRowIdx = j;
				PIColumnIdx = i;
			}
		}

		if (OneCount == 1 && PITable[PICount][i] != 1)		//1이 한개고 PIT 확인행에 체크가 안된 경우
		{
			EssentialPI[EssentialPICount++] = PIColumn[PIRowIdx];
			for (int k = 0; k < TMCount; k++)
			{
				/* 행 확인해서 1 적혀있으면 확인행에 1 표시 */
				if (PITable[PIRowIdx][k] == 1)
				{
					for (int w = 0; w < PICount; w++)	//열 초기화
						PITable[w][k] = 0;
					PITable[PICount][k] = 1;
				}
			}
		}

	}


	cout << endl;
	while (1)
	{
		if (CheckBreak(PITable, PICount, TMCount) == true)break;	//PIT 확인행 전부 체크시 반복문 탈출
		int MaxOne = 0;  	//가장 많은 1의 갯수 저장
		int MaxIndex = 0;	// MaxOne 해당 행 Index 저장
		int Count_1; 		//1의 갯수 저장
		for (int i = 0; i < PICount; i++)	//PIT확인행 체크된 열 초기화 후 남은 열 중 비교
		{
			Count_1 = 0;
			for (int j = 0; j < TMCount; j++)
			{
				if (PITable[i][j] == 1)   //1의 갯수 체크
					Count_1++;
			}
			if (Count_1 > MaxOne)		//가장 많은 1의 갯수 체크
			{
				MaxOne = Count_1;
				MaxIndex = i;
			}
		}
		for (int i = 0; i < TMCount; i++)
		{
			if (PITable[MaxIndex][i] == 1)
			{
				for (int w = 0; w < PICount; w++)  //확인행 체크 될 열 
					PITable[w][i] = 0;
				PITable[PICount][i] = 1;
			}
		}

		EssentialPI[EssentialPICount++] = PIColumn[MaxIndex];// 확인행에 가장 많이 체크되는 PI를 EPI에 저장
	}

	for (int i = 0; i < EssentialPICount; i++)
	{
		cout << endl;
		cout << EssentialPI[i];
	}

	cout << endl << "Cost(# of transistors) : " << MakeTransNum(EssentialPICount, EssentialPI) << endl;

	//결과 파일 result.txt 출력

	ofstream writeResult;
	writeResult.open("result.txt");  // 값을 출력할 파일 오픈
	string temp;  // 출력할 값을 저장할 string
	
	/* 모든 EPI들을 temp에 저장*/
	for (int i = 0; i < EssentialPICount; i++)
		temp += (EssentialPI[i] + '\n');
	temp += "\nCost(# of transistors) : ";  /* 트랜지스터 개수 출력 안내 메세지도 temp에 이어서 저장 */
	writeResult.write(temp.c_str(), temp.size()); /* temp에 모아놨던 값들 출력 */
	writeResult << MakeTransNum(EssentialPICount, EssentialPI);  /* 트랜지스터 개수 출력 */

	writeResult.close();  // 파일 폐쇄 

	/* 동적할당했던 메모리들 해제 */
	for (int i = 0; i < PICount + 1; i++)
		delete[] PITable[i];

	delete[] column;
	delete[] column2;
	delete[] prime;
	delete[] usage;
	delete[] true_minterm;
	delete[] PIColumn;
	delete[] TMRow;
	delete[] EssentialPI;
	delete[] PITable;
	return 0;
}

bool CheckFinish(string* column) // 마지막 칼럼 확인 함수
{
	for (int i = 0; column[i].empty() != true; i++)
	{
		for (int j = 1; column[i + j].empty() != true; j++)
		{
			int hamm = 0; //hamming distance
			for (int w = 0; column[i][w] != '\0'; w++)
			{
				if (column[i][w] != column[i + j][w])
					hamm++;
				if (hamm > 1)
					break;
			}
			if (hamm == 1) //= 더 줄일 수 있다 = 마지막 column이 아니다
				return false;
		}
	}
	return true; //이 경우 해당 함수가 마지막이므로 LastPrime으로 넘어감

}

bool CheckSame(string* column2, string* column, int line, int row) // 중복 확인 함수
{
	for (int i = 0; i < row; i++)
	{
		if (column2[i].compare(column[line]) == 0) //같을 때
			return false;
	}
	return true;
}

int MakeColumn(string* column2, string* column, int* usage)       //다음 칼럼 생성 함수
{
	int u = 0;
	int i, j;
	for (i = 0; column[i].empty() != true; i++)
		usage[i] = 0;
	for (i = 0; column[i].empty() != true; i++)
	{
		int index = 0;
		for (j = 1; column[j + i].empty() != true; j++)		//행끼리 비교하여 Hamming Distance = 1인 경우 체크
		{
			int hamm = 0; //=Hamming Distance
			int w = 0;
			for (w = 0; column[i][w] != '\0'; w++)
			{
				if (column[i][w] != column[i + j][w])
				{
					hamm++;
					index = w;
				}
				if (hamm > 1)	//Hamming Distance != 1 이면 
					break;
			}
			if (hamm == 1) //Hamming Distance = 1인 경우 해당 index '-' 변환 후 다음 칼럼에 저장
			{
				char ch = column[i][index];
				column[i][index] = '-';
				if (CheckSame(column2, column, i, u) == true) // 중복 예외 처리
				{
					column2[u] = column[i];
					u++;
				}
				usage[i]++;
				usage[i + j]++;
				column[i][index] = ch;
			}
		}
	}
	return 0;
}

void CopyPrime(string* prime, string* real_column, int* usage, int* p) {
	//p=prime implicant의 갯수
	for (int i = 0; real_column[i].empty() != true; i++) {
		if (usage[i] == 0) {
			prime[*p] = real_column[i];
			(*p)++;
		}
	}
}

void reset(string* column) { //다음 column 이동 (여기서는 column2->column) 을 원활하게 하기 위해 column을 초기화. usage는 MakeColumn에서 초기화하니 여기선 패스.
	for (int i = 0; column[i].empty() != true; i++)
		column[i].clear();
}

void LastPrime(string* column, string* prime, int* p) { //마지막 column은 통째로 prime 배열로 옮겨준다
	for (int i = 0; column[i].empty() != true; i++) {
		prime[*p] = column[i];
		(*p)++;
	}
}

void ShowPrime(string* prime) { //prime implicant 출력
	cout << endl << "Prime Implicant" << endl;
	for (int i = 0; prime[i].empty() != true; i++)
	{
		cout << prime[i];
		cout << endl;
	}
}

int CheckNumberOne(int* PITableRow, int TMCount)
{
	int count = 0;
	for (int i = 0; i < TMCount; i++)
		if (PITableRow[i] == 1) count++;

	return count;
}

/* 트랜지스터의 개수를 구하는 함수 */
int MakeTransNum(int EssentialPICount, string* EssentialPI)
{
	int A, O, N; // and, or, not

	O = EssentialPICount * 2 + 2;                  // input의 개수가 EPI의 개수와 동일하므로 NOR gate 트랜지스터 개수 (EPICount * 2)개, NOT gate 트랜지스터 개수 2개
	A = CheckAnd(EssentialPICount, EssentialPI);   //함수에서 AND 게이트 트랜지스터 개수를 계산하여 A에 저장
	N = CheckNot(EssentialPICount, EssentialPI);   //함수에서 NOT 게이트 트랜지스터 개수를 계산하여 N에 저장

	return O + A + N;    // 위에서 계산한 트랜지스터 개수의 합을 리턴
}

/*****************************************************************************************
	AND 게이트 트랜지스터 개수 구하는 함수
	각 EPI에서 ('-'이 아닌 값의 개수) * 2 + 2의 합을 구함
	이것은 전체 EPI에서 ('-'이 아닌 값의 개수) * 2 + 2 * (EPI의 개수)를 한 것과 동일하다
******************************************************************************************/
int CheckAnd(int EssentialPICount, string* EssentialPI) 
{
	int ACount = 0;  //'0'과 '1'의 개수를 저장할 변수

	/* 전체 EPI에서 '0'과 '1'의 개수를 세어서 ACount에 저장 */
	for (int i = 0; i < EssentialPICount; i++)
	{
		for (unsigned int j = 0; j < EssentialPI[i].length(); j++) //int -> unsigned int (error c4018 해결)
			if (EssentialPI[i][j] == '1' || EssentialPI[i][j] == '0') ACount++;
	}

	return ACount * 2 + 2 * EssentialPICount;  // 트랜지스터의 개수를 계산하여 리턴 
}

/*****************************************************************************************
	NOT 게이트 트랜지스터 개수 구하는 함수
	각 EPI 자릿수에 0이 사용되었는지 확인한 수 
	사용된 자릿수의 개수 * 2를 리턴
******************************************************************************************/
int CheckNot(int EssentialPICount, string* EssentialPI)
{
	int NCount = 0;  //체크된 자리 개수를 셀 변수
	int* CheckNot = new int[EssentialPI[0].length()];  // 자릿수 체크할 배열을 동적할당
	
	/* 배열을 0으로 초기화 */
	for (unsigned int i = 0; i < EssentialPI[0].length(); i++)
		CheckNot[i] = 0;

	/* EPI를 체크하여 0이 사용되었는지 확인한 후 
	사용되었으면 거기에 해당하는 자리를 배열에 표시함 */
	for (int i = 0; i < EssentialPICount; i++)
	{
		for (unsigned int j = 0; j < EssentialPI[0].length(); j++) //int -> unsigned int (error c4018 해결)
		{
			if (EssentialPI[i][j] == '0')
				CheckNot[j] = 1;
		}
	}

	/* 표기된 자리 개수를 셈 */
	for (unsigned int i = 0; i < EssentialPI[0].length(); i++)
		NCount += CheckNot[i];
	delete[] CheckNot;  // 동적할당한 배열 메모리 해제
	return NCount * 2;  // 트랜지스터의 개수를 계산하여 리턴 
}

bool CheckBreak(int** PIT, int CheckPI, int n)	//EPI 저장할때 무한반복문 탈출 
{
	for (int i = 0; i < n; i++)
	{
		if (PIT[CheckPI][i] != 1)
			return false;
	}
	return true;
}
