#include<iostream>
#include<fstream>
#include<cstdlib>
#include<memory.h>

using namespace std;

int MakeColumn(char**, char**, int, int);
void RemoveOverlap(char**, int, int);

int main()
{
	int count = -1, len, mc = 0;
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
	char** real_column = new char* [count];
	char** prime = new char* [count];
	char** true_minterm = new char* [mc];
	for (int i = 0; i < count; i++)
	{
		column[i] = new char[len + 1];
		real_column[i] = new char[len + 1];
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
	
	char** lastColumn = nullptr;  //최종 열
	int lastCount = 0;            //최종 열 요소 카운팅

	while (1)
	{
		static int Newcount = 0;

		Newcount = MakeColumn(real_column, column, len, count);   //새로운 열의 요소수를 세어 Newcount에 저장
		if (Newcount == 0) {   //다음 열에 요소가 없으면 반복문 탈출
			lastColumn = new char* [count];
			for (int i = 0; i < count; i++)
			{
				lastColumn[i] = new char[len + 1];
			}

			for (int i = 0; i < count; i++)
			{
				for (int j = 0; j < len + 1; j++)
				{
					lastColumn[i][j] = column[i][j];
				}
				lastCount++;
			}
			break;
		}

		count = MakeColumn(column, real_column, len, Newcount);  //새로운 열의 요소수를 세어 count에 저장
		if (count == 0) {   //다음 열에 요소가 없으면 반복문 탈출
			lastColumn = new char* [Newcount];
			for (int i = 0; i < Newcount; i++)
			{
				lastColumn[i] = new char[len + 1];
			}

			for (int i = 0; i < Newcount; i++)
			{
				for (int j = 0; j < len + 1; j++)
				{
					lastColumn[i][j] = real_column[i][j];
				}
				lastCount++;
			}
			break;
		}	
	}

	cout << endl;


	//최종 열 요소에 값이 잘 저장되었는지 확인
	for (int i = 0; i < lastCount; i++)
	{
		for (int j = 0; j < len + 1; j++)
		{
			cout << lastColumn[i][j];
		}
		cout << endl;
	}
	return 0;
}

int MakeColumn(char** real_column, char** column, int len, int count)
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
					index = 0;
				}
			}
			if (Hamming_dis == 1)
			{
				for (int z = 0; z < len + 1; z++)
				{
					real_column[u][z] = column[i][z];
					if (z == index)
					{
						real_column[u][z] = '-';
					}
				}
				u++;   //새로운 요소 개수
			}
		}
	}

	cout << endl;

	for (int i = 0; i < u; i++)
	{
		for (int j = 0; j < len + 1; j++)
		{
			cout << real_column[i][j];
		}
		cout << endl;
	}

	return u;
}
