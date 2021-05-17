#pragma warning (disable:4996) 
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
	if (!fin.is_open())					// ÆÄÀÏÀÌ ¿­·È´ÂÁö Ã¼Å©
	{
		cout << "false" << endl;
	}
	while (!fin.eof())
	{
		fin.getline(line, sizeof(line));
		if (line[0] == 'm')
			mc++;						// Æ®·ç¹ÎÅÒ °¹¼ö Ä«¿îÆÃ
		if (count == -1)
			len = atoi(line);
		count++;						// ¹ÎÅÒ ÃÑ °¹¼ö Ä«¿îÆÃ
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
			column[c_row][w] = '\0';
			true_minterm[t_row][w] = '\0';
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
			column[c_row][w] = '\0';
			c_row++;
		}

	}
	fin.close();
	cout << "column" << endl;
	for (int i = 0; i < count; i++)
		cout << column[i] << endl;
	cout << "true_minterm" << endl;
	for (int i= 0; i < len; i++)
		cout << true_minterm[i]<<endl;
	cout << endl;
	int ColumnNum = 0;
	int ColumnRow = 0;
	while (1)
	{
		if (ColumnNum % 2 == 0)
		{
			ColumnRow=MakeColumn(column2, column, len, count);
			if (CheckFinish(column2, len, ColumnRow) == true) break;    //¸¶Áö¸· Ä®·³ È®ÀÎ½Ã ¹İº¹¹® Å»Ãâ
			ColumnNum++;
		}
		else
		{
			ColumnRow = MakeColumn(column, column2, len, count);
			if (CheckFinish(column, len, ColumnRow) == true) break;
			ColumnNum++;
		}
	}

}
bool CheckFinish(char** column, int len,int row) // ¸¶Áö¸· Ä®·³ È®ÀÎ ÇÔ¼ö 
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
bool CheckSame(char** column2, char** column, int line,int row) // Áßº¹ È®ÀÎ ÇÔ¼ö
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
			for (w = 0; w < len; w++)
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
				char ch = column[i][index];
				column[i][index] = '-';
				//cout << column[i] << endl;
				if (CheckSame(column2, column, i, u) == true)
				{
					strcpy(column2[u], column[i]);
					u++;
				}
				column[i][index] = ch;

			}
		}
	}
	for (int i = 0; i < u; i++)
		cout << column2[i] << endl;

	cout << endl;
	return u;
}