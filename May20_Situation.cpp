#pragma warning(disable: 4996)
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include <string>

using namespace std;

int MakeColumn(string*, string*, int*);
void CopyPrime(string*, string*, int*, int*); //usage를 세야 해서 int가 하나 더 들어감
void reset(string*, int*);
bool CheckFinish(string*);
bool CheckSame(string*, string*, int, int);
void LastPrime(string*, string*, int*);
void ShowPrime(string*, int*);

int main()
{
	string line;
	string* column = new string[1000];
	string* column2 = new string[1000];
	string* prime = new string[1000];
	int* usage = new int[1000]; //column의 각 행의 사용횟수를 측정. 0인 행은 prime implicant로 옮겨주면 됨.
	string* true_minterm = new string[1000];
	int c_row = 0, t_row = 0;
	ifstream fin("input_minterm.txt");
	if (!fin.is_open())
	{
		cout << "false" << endl;
	}
	while (!fin.eof())
	{
		getline(fin, line);
		if (line[0] == 'm')
		{
			column[c_row] = line.substr(2);
			true_minterm[t_row] = line.substr(2);
			t_row++;
			c_row++;
		}
		else if (line[0] == 'd')
		{
			column[c_row] = line.substr(2);
			c_row++;
		}
	}
	fin.close();
	cout << "column" << endl;
	for (int i = 0; column[i].empty() != true; i++)
		cout << column[i] << endl;
	cout << endl;
	int ColumnNum = 0;
	int p = 0; //prime implicant 갯수
	while (1)
	{
		if (ColumnNum % 2 == 0)
		{
			cout << endl << "Column " << ColumnNum + 2 << endl;
			MakeColumn(column2, column, usage);
			CopyPrime(prime, column, usage, &p);
			if (CheckFinish(column2) == true) break;    //마지막 칼럼 확인시 반복문 탈출
			reset(column, usage);
			ColumnNum++;
		}
		else
		{
			cout << endl << "Column " << ColumnNum + 2 << endl;
			MakeColumn(column, column2, usage);
			CopyPrime(prime, column2, usage, &p);
			if (CheckFinish(column) == true) break;
			reset(column2, usage);
			ColumnNum++;
		}
	}
	//Last Column => 마지막 칼럼을 전부 prime 배열로 옮김
	if (ColumnNum % 2 == 0)
		LastPrime(column2, prime, &p);
	else
		LastPrime(column, prime, &p);
	//prime 배열에 저장된 모든 prime implicant를 출력
	ShowPrime(prime, &p);

	return 0;
}
bool CheckFinish(string* column) // 마지막 칼럼 확인 함수
{
	for (int i = 0; column[i].empty()!=true; i++)
	{
		for (int j = 1; column[i+j].empty()!=true; j++)
		{
			int hamm = 0; //hamming distance
			for (int w = 0; column[i][w]!='\0'; w++)
			{
				if (column[i][w] != column[i + j][w])
					hamm++;
				if (hamm > 1)
					break;
			}
			if (hamm == 1)
				return false;
		}
	}
	return true;

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
int MakeColumn(string* column2, string* column, int* usage)
{
	int u = 0;
	int i, j;
	for (i = 0; column[i].empty()!=true; i++)										//수정 필요
		usage[i] = 0;
	for (i = 0; column[i].empty() != true; i++)
	{
		int index = 0;
		for (j = 1; column[j + i].empty() != true; j++)
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
				if (hamm > 1)
					break;
			}
			if (hamm == 1)
			{
				char ch = column[i][index];
				column[i][index] = '-';
				if (CheckSame(column2, column, i, u) == true) { //다를 때
					column2[u] = column[i];
					u++;
				}
				usage[i]++;
				usage[i + j]++;
				column[i][index] = ch;
			}
		}
	}
	for (i = 0; i < u; i++)
		cout << column2[i] << endl;
	cout << endl;
	return 0;
}

void CopyPrime(string* prime, string* real_column, int* usage, int* p) {
	//p=prime implicant의 갯수
	cout << endl << "Prime From Column" << endl;
	for (int i = 0; real_column[i].empty() != true; i++) {
		if (usage[i] == 0) {
			for (int j = 0; real_column[i].empty()!=true; j++) {
				prime[*p][j] = real_column[i][j];
				cout << prime[*p][j];
			}
			cout << endl;
			(*p)++;
		}
	}
}

void reset(string* column, int* usage) {
	for (int i = 0; column[i].empty() != true; i++) {
		for (int j = 0; column[i].empty() != true; i++)
			column[i][j] = '\0'; //배열 내용을 지우는 방법
	}
}

void LastPrime(string* column, string* prime, int* p) {
	cout << endl << "Last Prime" << endl;
	for (int i = 0; column[i].empty() != true; i++) {
		prime[*p] = column[i];
		cout << prime[*p];
		cout << endl;
		(*p)++;
	}
}

void ShowPrime(string* prime, int* p) {
	cout << endl << "Prime Implicant" << endl;
	for (int i = 0; i < *p; i++) { //요기는 놔두는 게 나을라나
		for (int j = 0; prime[i].empty()!=true; j++) {
			cout << prime[i][j];
		}
		cout << endl;
	}
}
