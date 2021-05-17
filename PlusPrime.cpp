#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>

using namespace std;

int MakeColumn(char**, char**, int, int, int*);
void CopyPrime(char**, char**, int, int, int*, int); //usage�� ���� �ؼ� int�� �ϳ� �� ��
void reset(char**, int*, int, int);
bool CheckFinish(char**, int, int);
bool CheckSame(char**, char**, int, int);
void LastPrime(char**, char**, int, int, int *);
void ShowPrime(char**, int, int); //�̰� �� �ƹ��͵� �� ������?

int main()
{
	int count = -1, len = 0, mc = 0;
	char line[100];

	ifstream fin("input_minterm.txt");
	if (!fin.is_open())
	{
		cout << "false" << endl;
	}
	while (!fin.eof())
	{
		fin.getline(line, sizeof(line));
		if (line[0] == 'm')
			mc++;
		if (count == -1)
			len = atoi(line);
		count++;
	}
	fin.close();
	char** column = new char*[count];
	char** column2 = new char*[count];
	char** prime = new char*[count];
	int* usage = new int[count]; //column�� �� ���� ���Ƚ���� ����. 0�� ���� prime implicant�� �Ű��ָ� ��.
	char** true_minterm = new char*[mc];
	//int* plc = new int[count];
	for (int i = 0; i < count; i++)
	{
		column[i] = new char[len + 1];
		column2[i] = new char[len + 1];
		prime[i] = new char[len + 1];
	}
	for (int i = 0; i < mc; i++)
		true_minterm[i] = new char[len + 1];
	int c_row = 0, t_row = 0, w = 0;
	fin.open("input_minterm.txt");
	while (!fin.eof())
	{
		fin.getline(line, sizeof(line));

		if (line[0] == 'm')
		{
			w = 0;
			for (int i = 2; line[i] != '\0'; i++, w++)
			{
				column[c_row][w] = line[i];
				true_minterm[t_row][w] = line[i];
			}
			t_row++;
			c_row++;
		}
		else if (line[0] == 'd')
		{
			w = 0;
			for (int i = 2; line[i] != '\0'; i++, w++)
			{
				column[c_row][w] = line[i];
			}
			c_row++;
		}

	}
	fin.close();
	cout << "column" << endl;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < len + 1; j++)
		{
			cout << column[i][j];
		}
		cout << endl;
	}
	cout << "true_minterm" << endl;
	for (int i = 0; i < mc; i++)
	{
		for (int j = 0; j < len + 1; j++)
		{
			cout << true_minterm[i][j];
		}
		cout << endl;
	}
	cout << endl;
	int ColumnNum = 0;
	int ColumnRow = 0;
	int p = 0; //prime implicant ����
	while (1)
	{
		if (ColumnNum % 2 == 0)
		{
			cout << "Column " << ColumnNum+2 << endl;
			ColumnRow = MakeColumn(column2, column, len, count, usage);
			CopyPrime(prime, column, len, count, usage, p);
			if (CheckFinish(column2, len, ColumnRow) == true) break;    //������ Į�� Ȯ�ν� �ݺ��� Ż��
			reset(column, usage, len, count);
			ColumnNum++;
		}
		else
		{
			cout << "Column " << ColumnNum+2 << endl;
			ColumnRow = MakeColumn(column, column2, len, count, usage);
			CopyPrime(prime, column2, len, count, usage, p);
			if (CheckFinish(column, len, ColumnRow) == true) break;
			reset(column2, usage, len, count);
			ColumnNum++;
		}
	}
	//Last Column => ������ Į���� ���� prime �迭�� �ű�
	if (ColumnNum % 2 == 0)
		LastPrime(column2, prime, len, count, &p);
	else
		LastPrime(column, prime, len, count, &p);
	//prime �迭�� ����� ��� prime implicant�� ��� -> �̰� ���� �ȵǰ� ����
	cout << endl << "Prime Implicant" << endl;
	for (int i = 0; i < p; i++) {
		for (int j = 0; j < len + 1; j++) {
			cout << prime[i][j];
		}
		cout << endl;
	}
	
	return 0;
}
bool CheckFinish(char** column, int len, int row) // ������ Į�� Ȯ�� �Լ� �̿� 
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 1; i + j < row; j++)
		{
			int Hd = 0;
			for (int w = 0; w < len + 1; w++)
			{
				if (column[i][w] != column[i + j][w])
					Hd++;
				if (Hd > 1)
					break;
			}
			if (Hd == 1)
				return false;
		}
	}
	return true;
}
bool CheckSame(char** column2, char** column, int line, int row) // �ߺ� Ȯ�� �Լ�
{
	for (int i = 0; i < row; i++)
	{
		if (strcmp(column2[i], column[line]) == 0)
			return false;
	}
	return true;
}
int MakeColumn(char** column2, char** column, int len, int count, int* usage)
{
	int u = 0;
	int i, j;
	for (i = 0; i < count; i++) //usage�� �ʱ�ȭ
		usage[i] = 0;
	for (i = 0; i < count; i++)
	{
		int index = 0;
		for (j = 1; j + i < count; j++)
		{
			int hamm = 0; //=Hamming Distance
			int w = 0;
			for (w = 0; w < len + 1; w++)
			{
				if (column[i][w] != column[i + j][w])
				{
					hamm++;
					index = w;
				}
				if (hamm > 1)
				{
					break;
				}
			}
			if (hamm == 1)
			{
				column[i][index] = '-';
				if (CheckSame(column2, column, i, u) == true)
				{
					for (int z = 0; z < len + 1; z++)
					{
						column2[u][z] = column[i][z];
					}
					usage[i]++;
					usage[j]++;
					u++;
				}
			}
		}
	}
	for (i = 0; i < u; i++)
	{
		for (j = 0; j < len + 1; j++)
		{
			cout << column2[i][j];
		}
		cout << endl;
	}
	return u;
}

void CopyPrime(char** prime, char** real_column, int len, int count, int* usage, int p) {
	//p=prime implicant�� ����
	cout <<endl<<"Prime From Column" << endl;
	for (int i = 0; i < count; i++) {
		if (usage[i] == 0) {
			for (int j = 0; j < len + 1; j++) {
				prime[p][j] = real_column[i][j];
				cout << prime[p][j]; //prime implicant�� ���� ���� Ȯ���� ���� �����. Ȯ�� �� ���� �����̰�, ��� �κ��� ������ ����.
			}
			cout << endl;
			p++;
		}
	}
}

void reset(char** column, int* usage, int len, int count) {
	for (int i = 0; i < count; i++) //�� �����ϱ� �ʱ�ȭ
		usage[i] = 0;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < len + 1; j++)
			column[i][j] = '\0'; //�迭 ������ ����� ���
	} //����� �� Ȯ�ε�
}

void LastPrime(char** column, char** prime, int len, int count, int* p) {
	cout << endl<<"Last Prime"<<endl;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < len + 1; j++) {
			prime[*p][j] = column[i][j];
			cout << prime[*p][j];
		}
		cout << endl;
		(*p)++;
	}
}

void ShowPrime(char** prime, int len, int p) { //���⼭ prime implicant�� ���� �ϴµ� �ƹ��͵� �ȵǰ� ����
	cout << endl << "Prime Implicant" << endl;
	for (int i = 0; i < p; i++) {
		for (int j = 0; j < len + 1; j++) {
			cout << prime[i][j];
		}
		cout << endl;
	}
}
