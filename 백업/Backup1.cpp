#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class QM
{
private:
	int length;
	char mark;   //m, d 구분

public:

	int numberOne = 0;
	char* number = NULL;
	bool check = false;     //겹치는 부분 계산 했을 경우 true로 변환

	void SetLength(int len)
	{
		length = len;
		number = new char[length];
	}

	void SetMark(char c)
	{
		mark = c;
	}
	
	void CheckNumberOne(int len)
	{
		int cnt = 0;

		for (int i = 0; i < len; i++)
		{
			if (number[i] == '1') cnt++;
		}

		numberOne = cnt;
	}
};

int main()
{
	int count = -1, len, mc = 0;
	char line[100];

	ifstream fin("input_minterm.txt");  //파일 개방

	if (!fin.is_open())     //파일이 열리지 않았을 경우
	{
		cout << "false" << endl;    //실패 메시지 출력
	}

	while (!fin.eof())      //파일 내용이 끝날 때까지 반복
	{
		fin.getline(line, sizeof(line));  //줄 단위 입력
		if (line[0] == 'm')               /*true minterm은 따로 카운트*/
			mc++;
		if (count == -1)                  /*처음 입력되는 숫자는 bit length로 처리*/
			len = atoi(line);
		count++;
	}
	fin.close();   //파일 폐쇄

	QM* column1 = new QM[count];
	QM* column2 = new QM[count];
	QM* true_minterm = new QM[mc];

	for (int i = 0; i < count; i++)
	{
		column1[i].SetLength(len);
		column2[i].SetLength(len);
	}
	for (int i = 0; i < mc; i++)
	{
		true_minterm[i].SetLength(len);
	}

	int c_row = 0, t_row = 0, w = 0;
	fin.open("input_minterm.txt");
	while (!fin.eof())
	{
		fin.getline(line, sizeof(line));

		if (line[0] == 'm')
		{
			w = 0;
			column1[c_row].SetMark('m');

			for (int i = 2; line[i] != '\0'; i++, w++)
			{
				column1[c_row].number[w] = line[i];
				true_minterm[t_row].number[w] = line[i];
			}
			t_row++;
			c_row++;
		}

		else if (line[0] == 'd')
		{
			w = 0;
			column1[c_row].SetMark('d');

			for (int i = 2; line[i] != '\0'; i++, w++)
			{
				column1[c_row].number[w] = line[i];
			}
			c_row++;
		}

	}
	fin.close();

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < len; j++)
		{
			cout << column1[i].number[j];
		}
		cout << endl;
	}

	cout << endl;

	for (int i = 0; i < mc; i++)
	{
		for (int j = 0; j < len; j++)
		{
			cout << true_minterm[i].number[j];
		}
		cout << endl;
	}

	delete[] column1;
	delete[] column2;
	delete[] true_minterm;
	return 0;
}
