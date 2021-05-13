#include<iostream>
#include<fstream>
#include<cstdlib>

using namespace std;

void MakeColumn(char**, char**, int, int, int*);
void CopyPrime(char**, char**, int, int, int*, int); //usage를 세야 해서 int가 하나 더 들어감
void eliminate(char**, int, int);

int main()
{
	int count = -1, len, mc = 0;
	char line[100];
	int i, j;

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
	char** real_column = new char*[count];
	char** prime = new char*[count];
	int* usage = new int[count]; //column의 각 행의 사용횟수를 측정. 0인 행은 prime implicant로 옮겨주면 됨.
	char** true_minterm = new char*[mc];
	for (i = 0; i < count; i++)
	{
		column[i] = new char[len + 1];
		real_column[i] = new char[len + 1];
		prime[i] = new char[len + 1];
	}
	for (i = 0; i < mc; i++)
		true_minterm[i] = new char[len + 1];
	int c_row = 0, t_row = 0, w = 0;
	fin.open("input_minterm.txt");
	while (!fin.eof())
	{
		fin.getline(line, sizeof(line));

		if (line[0] == 'm')
		{
			w = 0;
			for (i = 2; line[i] != '\0'; i++, w++)
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
			for (i = 2; line[i] != '\0'; i++, w++)
			{
				column[c_row][w] = line[i];
			}
			c_row++;
		}

	}
	fin.close();
	cout << "column" << endl;
	for (i = 0; i < count; i++)
	{
		for (j = 0; j < len + 1; j++)
		{
			cout << column[i][j];
		}
		cout << endl;
	}
	cout << "true_minterm" << endl;
	for (i = 0; i < mc; i++)
	{
		for (j = 0; j < len + 1; j++)
		{
			cout << true_minterm[i][j];
		}
		cout << endl;
	}
	cout << endl;
	MakeColumn(real_column, column, len, count, usage);
	int p = 0; //p=prime implicant의 갯수
	CopyPrime(prime, column, len, count, usage, p);
	for (i = 0; i < count; i++) //다 썼으니까 초기화
		usage[i] = 0;

	eliminate(column, len, count);
	/*
	column 2
	column 2 remain to prime
	column 2 used to column 3
	column 3 (어차피 저 안에서 없음)
	column 3 remain to prime
	print prime //END
	*/
}

void MakeColumn(char** real_column, char** column, int len, int count, int* usage)
{
	int i, j;
	int u = 0;
	for (i = 0; i < count; i++) //usage의 초기화
		usage[i] = 0;
	for (i = 0; i < count; i++)
	{
		int index = 0;
		for (j = i + 1; j < count; j++)
		{
			int hamm = 0; //hamming distance
			int w = 0;
			for (w = 0; w < len + 1; w++)
			{
				if (column[i][w] != column[j][w])
				{
					hamm++;
					index = w;
				}
				if (hamm > 1)
					break;
			}
			if (hamm == 1)
			{
				for (int z = 0; z < len + 1; z++)
				{
					real_column[u][z] = column[i][z];
					if (z == index)
					{
						real_column[u][z] = '-';
					}
				}
				usage[i]++;
				usage[j]++;
				u++;
			}
		}
	}
	for (i = 0; i < u; i++)
	{
		for (j = 0; j < len + 1; j++)
		{
			cout << real_column[i][j];
		}
		cout << endl;
	}
}

void CopyPrime(char** prime, char** real_column, int len, int count, int* usage, int p) {
	std::cout << endl << "Current Situation" << endl;
	//p=prime implicant의 갯수
	for (int i = 0; i < count; i++) {
		if (usage[i] == 0) {
			cout << endl;
			for (int j = 0; j < len + 1; j++) {
				prime[p][j] = real_column[i][j];
				cout << prime[p][j]; //prime implicant의 정상 구현 확인을 위해 써놓음. 확인 후 지울 예정이고, 출력 부분은 끝에서 구현.
			}
			cout << endl;
			p++;
		}
	}

	/*
	//참고) 마지막 칼럼은 통째로 prime 배열로 옮겨야 함
	if(이번 column이 마지막 ⇔ 더이상 hamm 값이 나오지 않음){
	for(int i=0; i<count; i++){
		for(int j=0; j<len+1; j++){
			prime[p][j] = real_column[i][j];
			cout << prime[p][j];
		}
		cout << endl;
		p++;
	}
	*/
}

void eliminate(char** column, int len, int count) {
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < len + 1; j++)
			column[i][j] = '\0'; //배열 내용을 지우는 방법
	} //비워진 것 확인됨
}
