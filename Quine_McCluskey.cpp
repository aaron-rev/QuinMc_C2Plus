#pragma warning(disable: 4996)
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include <string>

using namespace std;

int MakeColumn(string*, string*, int*);
void CopyPrime(string*, string*, int*, int*); //usage�� ���� �ؼ� int�� �ϳ� �� ��
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
	int* usage = new int[1000]; //column�� �� ���� ���Ƚ���� ����. 0�� ���� prime implicant�� �Ű��ָ� ��.
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
	int p = 0; //prime implicant ����
	while (1)
	{
		if (ColumnNum % 2 == 0)
		{
			cout << endl << "Column " << ColumnNum + 2 << endl;
			MakeColumn(column2, column, usage);
			CopyPrime(prime, column, usage, &p);
			if (CheckFinish(column2) == true) break;    //������ Į�� Ȯ�ν� �ݺ��� Ż��
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
	//Last Column => ������ Į���� ���� prime �迭�� �ű�
	if (ColumnNum % 2 == 0)
		LastPrime(column2, prime, &p);
	else
		LastPrime(column, prime, &p);
	//prime �迭�� ����� ��� prime implicant�� ���
	ShowPrime(prime, &p);

	return 0;
}
bool CheckFinish(string* column) // ������ Į�� Ȯ�� �Լ�
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
			if (hamm == 1)
				return false;
		}
	}
	return true;

}
bool CheckSame(string* column2, string* column, int line, int row) // �ߺ� Ȯ�� �Լ�
{
	for (int i = 0; i < row; i++)
	{
		if (column2[i].compare(column[line]) == 0) //���� ��
			return false;
	}
	return true;
}
int MakeColumn(string* column2, string* column, int* usage)
{
	int u = 0;
	int i, j;
	for (i = 0; column[i].empty() != true; i++)										//���� �ʿ�
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
				if (CheckSame(column2, column, i, u) == true) { //�ٸ� ��
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
	//p=prime implicant�� ����
	cout << endl << "Prime From Column" << endl;
	for (int i = 0; real_column[i].empty() != true; i++) {
		if (usage[i] == 0) {
			prime[*p] = real_column[i];
			cout << prime[*p];
			cout << endl;
			(*p)++;
		}
	}
}

void reset(string* column, int* usage) {
	for (int i = 0; column[i].empty() != true; i++)
	{
		column[i].clear();
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
	for (int i = 0; prime[i].empty()!=true; i++)
	{ 
		cout << prime[i];
		cout << endl;
	}
}