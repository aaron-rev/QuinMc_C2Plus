#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
/* 문제 생겼던 함수 부분 다 지움 / 일단 백업 */
using namespace std;

int CheckHammingDistanceOne(QM& qm1, QM& qm2, int length);
int FindDiffIndex(QM& qm1, QM& qm2, int length);
void CheckPI(QM** column1, QM** implicant_minterm, int count);
void MakeGroupOne(QM** column, QM** MakeGroupOne, int len, int* countOne, int count);
void CheckCountOne(QM** column, int** countOne, int count);

class QM
{
private:
	int length;
	char mark;   //m, d 구분

public:
	char* number = nullptr;
	bool check = false;     //겹치는 부분 계산 했을 경우 true로 변환
	int numberOne = 0;

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
	QM* implicant_minterm = new QM[count];
	QM* true_minterm = new QM[mc];
	QM** MakeGroupOne;

	int* countOne = new int[len + 1];

	/* 각 칼럼 요소들의 비트수 저장 */
	for (int i = 0; i < count; i++)
	{
		column1[i].SetLength(len);
		column2[i].SetLength(len);
	}
	for (int i = 0; i < mc; i++)
	{
		true_minterm[i].SetLength(len);
	}

	/* 파일로부터 값을 입력 받기 */
	int c_row = 0, t_row = 0, w = 0;
	fin.open("input_minterm.txt");
	while (!fin.eof())
	{
		fin.getline(line, sizeof(line));

		if (line[0] == 'm')  //첫 글자가 m인 경우
		{
			w = 0;
			column1[c_row].SetMark('m'); 

			for (int i = 2; line[i] != '\0'; i++, w++)
			{
				column1[c_row].number[w] = line[i];
				true_minterm[t_row].number[w] = line[i];  //배열에 따로 저장
			}
			t_row++;
			c_row++;
		}

		else if (line[0] == 'd')   //첫 글자가 d인 경우
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

	/* 이 밑으로는 확인용 */
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

	cout << endl;

	for (int i = 0; i < count; i++)
	{
		column1[i].CheckNumberOne(len);
		cout << column1[i].numberOne << endl;
	}

	/* 동적 할당한 메모리 해제 */
	delete[] column1;
	delete[] column2;
	delete[] true_minterm;
	delete[] implicant_minterm;
	delete[] countOne;
	delete[] MakeGroupOne;
	return 0;
}

/* 해밍 디스턴스가 1인지 체크해주는 함수(디버깅 안 했음) */
bool CheckHammingDistanceOne(QM& qm1, QM& qm2, int length)
{
	int HammingDistance = 0;

	for (int i = 0; i < length; i++)
	{
		if (qm1.number[i] != qm2.number[i]) HammingDistance++;
		if (HammingDistance > 1) return false;
	}
	if (HammingDistance == 0) return false;

	return true;
}

/* 해밍 디스턴스가 1인 경우 어디가 다른지 알려주는 함수 */
int FindDiffIndex(QM& qm1, QM& qm2, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (qm1.number[i] != qm2.number[i]) return i;
	}
}

void CheckPI(QM** column1, QM** implicant_minterm, int count)
{
	static int countImplicant = 0;

	for (int i = 0; i < count; i++)
	{
		if ((column1[i]->check) == false)
		{
			implicant_minterm[countImplicant] = column1[i];
			countImplicant++;
		}
	}
}

void CheckCountOne(QM** column, int** countOne, int count)
{
	for (int i = 0; i < count; i++)
	{
		countOne[column[i]->numberOne]++;
	}
}

void MakeGroupOne(QM** column, QM** MakeGroupOne, int len, int* countOne, int count)
{
	MakeGroupOne = new QM*[len];

	for (int i = 0; i < len; i++)
	{
		MakeGroupOne[i] = new QM[countOne[i]];
	}

	for (int i = 0; i < count; i++)
	{
		
	}

}
