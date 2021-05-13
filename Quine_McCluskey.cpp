#include<iostream>
#include<fstream>
#include<cstdlib>

using namespace std;

int MakeColumn(char**, char**, int, int);
bool CheckFinish(char**, int,int);
bool CheckSame(char**, char**, int, int);

int main()
{
	int count = -1, len=0, mc = 0;
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
	char** column = new char* [count];
	char** column2 = new char* [count];
	char** prime = new char* [count];
	char** true_minterm = new char* [mc];
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
	while (1)
	{
		if (ColumnNum % 2 == 0)
		{
			ColumnRow=MakeColumn(column2, column, len, count);
			if (CheckFinish(column2, len, ColumnRow) == true) break;    //마지막 칼럼 확인시 반복문 탈출
		}
		else
		{
			ColumnRow = MakeColumn(column, column2, len, count);
			if (CheckFinish(column, len, ColumnRow) == true) break;
		}
	}

}
bool CheckFinish(char** column, int len,int row) // 마지막 칼럼 확인 함수 미완
{
	int Hd = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 1; i + j < row; j++)
		{
			for (int w = 0; w < len + 1; w++)
			{

			}
		}
	}
}
bool CheckSame(char** column2, char** column, int line,int row) // 중복 확인 함수
{
	for (int i = 0; i < row; i++)
	{
		if (strcmp(column2[i], column[line]) == 0)
			return false;
	}
	return true;
}
int MakeColumn(char** column2, char** column, int len, int count)
{
	int u = 0;
	for (int i = 0; i < count; i++)
	{
		int index = 0;
		for (int j = 1; j + i < count; j++)
		{
			int Hamming_dis = 0;
			int w = 0;
			for (w = 0; w < len + 1; w++)
			{
				if (column[i][w] != column[i + j][w])
				{
					Hamming_dis++;
					index = w;
				}
				if (Hamming_dis > 1)
				{
					break;
				}
			}
			if (Hamming_dis == 1)
			{
				column[i][index] = '-';
				if (CheckSame(column2, column, i, u) == true)
				{
					for (int z = 0; z < len + 1; z++)
					{
						column2[u][z] = column[i][z];
					}
					u++;
				}
			}
		}
	}
	for (int i = 0; i < u; i++)
	{
		for (int j = 0; j < len + 1; j++)
		{
			cout << column2[i][j];
		}
		cout << endl;
	}
	return u;
}