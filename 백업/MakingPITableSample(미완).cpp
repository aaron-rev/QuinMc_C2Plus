#include <iostream>

using namespace std;

int CheckBreak(int a[], int); // 마지막 열을 모두 더한게 TMCount와 같으면 모든 요소가 1로 체크 된 것이므로 break.

int main()
{
	int len;
	int TMCount = 0, PICount = 0, EssentialPICount = 0;
	char** PIColumn, ** TMRow;
	int **PITable;
	char** EssentialPI;

	cout << "비트 수를 입력해주세요 : ";
	cin >> len;

	cout << "TrueMinterm의 개수를 입력해주세요 : ";
	cin >> TMCount;

	cout << "PI의 개수를 입력해주세요 : ";
	cin >> PICount;

	PIColumn = new char* [PICount];
	TMRow = new char* [TMCount];
	EssentialPI = new char* [PICount];   //대충 넉넉하게 PI 개수 만큼 선언했는데 메모리가 낭비되므로 나중에 최적화할때 수정 필요함.

	for (int i = 0; i < PICount; i++)
	{
		PIColumn[i] = new char[len + 1];
	}
	for (int i = 0; i < TMCount; i++)
	{
		TMRow[i] = new char[len + 1];
	}
	for (int i = 0; i < PICount; i++)
	{
		EssentialPI[i] = new char[len + 1];
	}

	cout << "PI 목록을 입력해주세요";
	cout << endl;                 /* 의미 없는데 안 하면 모양 안 예쁨 / 반복문 구현하면 삭제 필요 */
	for (int i = 0; i < PICount; i++)
	{
		/* PI 입력 받음 */
	}

	cout << "TM 목록을 입력해주세요";
	cout << endl;				  /* 의미 없는데 안 하면 모양 안 예쁨 / 반복문 구현하면 삭제 필요 */
	for (int i = 0; i < TMCount; i++)
	{
		/* TM 입력 받음 */
	}

	/* PITable 만들기 */
	PITable = new int* [PICount + 1];   //마지막 확인용 행 때문에 +1
	for (int i = 0; i < PICount + 1; i++)
	{
		PITable[i] = new int[TMCount];
	}

	/* PITable 전부 0으로 초기화 */
	for (int i = 0; i < PICount + 1; i++)
	{
		for (int j = 0; j < TMCount; j++)
		{
			PITable[i][j] = 0;
		}
	}

	/* TM과 PI 비교해서 같은 부분에 표시 */
	for (int i = 0; i < PICount; i++)
	{
		int count = 0;
		for (int j = 0; j < TMCount; j++)
		{
			for(int k = 0; k < len; k++)
			{
				if (PIColumn[i][k] == TMRow[j][k]) count++;       //***아직 PI입력하고 TM 입력 구현 안 해서 초록 밑줄 그어져있음***
				else if (PIColumn[i][k] == '-') count++;
			}
			if (count == len) PITable[i][j] = 1;
		}
	}

	/* 1이 한개만 있는 열 찾기 */
	for (int i = 0; i < TMCount; i++)
	{
		int PIRowIdx, PIColumnIdx;
		int OneCount = 0;
		for (int j = 0; j < PICount; j++)
		{
			if (PITable[j][i] == 1) 
			{
				OneCount++;
				PIRowIdx = j;
				PIColumnIdx = i;
			}
		}

		if (OneCount == 1)					/* 만약 1이 1개만 있는 열을 발견한다면 */
		{
			for (int i = 0; i < len; i++)
			{
				EssentialPI[EssentialPICount][i] = PIColumn[PIRowIdx][i];
				PITable[PICount][PIColumnIdx] = 1;    /* 확인 행에 1 표시 */
			}
			EssentialPICount++;   // EPI 개수 1개 증가
		}
	}


	/* 검토용 출력 */
	for (int i = 0; i < PICount + 1; i++)
	{
		for (int j = 0; j < TMCount; j++)
		{
			cout << PITable[i][j];
		}
		cout << endl;
	}

	for (int i = 0; i < PICount + 1; i++)
	{
		delete[] PITable[i];
	}
	for (int i = 0; i < PICount; i++)
	{
		delete[] PIColumn[i];
	}
	for (int i = 0; i < TMCount; i++)
	{
		delete[] TMRow[i];
	}
	for (int i = 0; i < PICount; i++)
	{
		delete[] EssentialPI[i];
	}
	delete[] PIColumn;
	delete[] TMRow;
	delete[] PITable;
	delete[] EssentialPI;
	return 0;
}

int CheckBreak(int CheckRow[], int TMCount)  // PITable의 마지막 행과 TM 개수를 매개변수로 넘김.
{
	int sum = 0;
	for (int i = 0; i < TMCount; i++)
	{
		sum += CheckRow[i];
	}

	return sum;  //Sum == TMCount 이면 반복문 빠져나감.
}
