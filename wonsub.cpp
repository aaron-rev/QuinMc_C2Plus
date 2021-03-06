#include <iostream>
#include <fstream> // 파일 입출력할 때 쓰임 - 입력을 파일로 받을 거라서 필요함

using namespace std;
/*
int input() { //굳이 분리를 해야 되나 싶긴 하지만..
	int count = -1, len, mc = 0; //mc=Minterm Count
	char line[100];
	ifstream fin("input_minterm.txt"); //파일 열기 (없으면 생성)
	if (!fin.is_open())
		cout << "FAILURE" << endl;
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
	return len;
}

void output() {
	
}
*/

int main() {
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
	char** column = new char* [count]; //Column I & Column III
	char** real_column = new char* [count]; //Column II
	//char** wls_column = new char* [count]; //Column III //칼럼은 두개로 충분하다는 의견이 있었음 - Column I 있던 자리를 비우면 되므로
	char** true_minterm = new char* [mc];
	int* usage = new int[count]; //column의 각 행의 사용횟수를 측정. 0인 행은 prime implicant로 옮겨주면 됨.
	char** prime = new char* [count]; //Prime Implicant
	for (int i = 0; i < count; i++) {
		column[i] = new char[len + 1];
		real_column[i] = new char[len + 1];
		prime[i] = new char[len + 1];
	}
	for (int i = 0; i < mc; i++)
		true_minterm[i] = new char[len + 1];
	int c_row = 0, t_row = 0, w = 0; //c_row = column 줄 개수, t_row = true_minterm 줄 개수
	fin.open("input_minterm.txt");
	while (!fin.eof()) {
		fin.getline(line, sizeof(line)); //파일 안의 내용을 배열에 넣는 과정

		if (line[0] == 'm') {//해당 줄이 True minterm일 때
			w = 0;
			for (int i = 2; line[i] != '\0'; i++, w++) {
				column[c_row][w] = line[i];
				true_minterm[t_row][w] = line[i];
			}
			t_row++; //true minterm이므로 t_row를 count함
			c_row++;
		}
		else if (line[0] == 'd') { //해당 줄이 Don't care minterm일 때
			w = 0;
			for (int i = 2; line[i] != '\0'; i++, w++)
				column[c_row][w] = line[i];
			c_row++;
		}

	}
	fin.close();
	cout << "column" << endl; //column 안의 내용을 출력하는 과정
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < len + 1; j++)
			cout << column[i][j];
		cout << endl;
	}
	cout << endl << "true_minterm" << endl; //true_minterm 안의 내용을 출력하는 과정 - 임의로 넣은 거라서 나중에 뺄 거임
	for (int i = 0; i < mc; i++) {
		for (int j = 0; j < len + 1; j++)
			cout << true_minterm[i][j];
		cout << endl;
	}

	cout << endl << "Column I" << endl; //prime implicant 찾기
	//hamming distance = 1인 것 찾기
	int index = 0, u=0, hamm=0; // u: real_column에 저장된 minterm의 갯수
	int i, j; // hamm= hamming distance
	//Build-Up: usage 초기화
	for (i = 0; i < count; i++)
		usage[i] = 0;
	//Column I
	for (int h = 0; h < count - 1; h++) {
		index = 0;
		for (i = h + 1; i < count; i++) {
			hamm = 0; //이로써 Column II로 갈 때 중간이 잘라먹히는 문제 해결
			for (j = 0; j < len + 1; j++) {
				if (column[h][j] != column[i][j]) {
					hamm++;
					index = j;
				}
				if (hamm > 1)
					//index = 0; //다시 앞으로 돌아감 - 보기는 편하나 없어도 문제는 없음. 속도 가산점을 위해 과감히 빼버림
					break; //문제 없음. 시간 절약 가능
			}
			if (hamm == 1) {
				for (int w = 0; w < len + 1; w++) {
					real_column[u][w] = column[i][w];
					if (w == index)
						real_column[u][w] = '-'; //ex) 0100과 0101일 경우, '010_'를 column에 더한다 - 다음 column으로 넘길 때 이것만 남기고 전부 지울 거임
					cout << real_column[u][w];
				}
				usage[h]++;
				usage[i]++;
				cout << endl;
				u++; //real_column에 minterm이 저장되었으므로 갯수(u)를 count한다
			}
		}
	}
	
	//새 Column을 출력하기 전, prime implicant를 별도의 prime 배열로 옮겨준다. real_column[i]는 Column 2->3 과정에서 쓰니까 백업 안해도 됨
	//내가 할 일: 바로 윗줄에 써있음
	std::cout << endl << "Current Situation" << endl;
	int p = 0; //p=prime implicant의 갯수
	for (i = 0; i < count; i++) {
		if (usage[i] == 0) { //현재 상황: 이 안이 완전히 씹히고 있음
			for (j = 0; j < len + 1; j++) { //if문 안이 출력이 안되서 prime implicant가 제대로 전달되는지도 확인 불가
				prime[p][j] = column[i][j]; //위에서 hamm =1일때 real_column으로 옮기는 과정과 똑같이 해주면 됨
				cout << prime[p][j];
			}
			cout << endl;
			p++;
		}
	}
	/*
	//prime implicant가 제대로 구현되는지 확인하려고 써놓음. 확인 후 지울 예정.
	std::cout << endl << "Current Situation" << endl;
	for (i = 0; i < p; i++) {
		for (j = 0; j < len + 1; j++) //제대로 짰으면 '1010' 이 출력되어야 함
			cout << prime[i][j]; //문제점) 이 부분이 아무것도 나오지 않음
		cout << endl;
	}
	*/
	for(i = 0; i < count; i++) //다 썼으니까 초기화
		usage[i] = 0;

	//column을 전부 지우기 - Column II의 내용을 Column III에 덮어 쓰기 위한 빌드업 (Column을 두 개만 쓰기 위함)
	for (i = 0; i < u; i++) {
		for (j = 0; j < len+1; j++)
			column[i][j]='\0'; //배열 내용을 지우는 방법
	} //비워진 것 확인됨
	
	/*
	std::cout<<endl<<"Column II"<<endl;
	int v=0; //Column II의 항 개수를 세려는 것이므로 새로 상수를 정의함
	for (int h = 0; h < count-1; h++) { //이 부분은 코드를 완성하고 나서 Class로 완성도를 높일 수 있을 것 같음
		for (i = h + 1; i < count; i++) {
			hamm=0;
			for (j = 0; j < len + 1; j++) {
				if (real_column[h][j] != real_column[i][j]) {
					hamm++;
					index = j;
				}
				if (hamm > 1)
					// 여기다가 break;를 넣었을 때는 딱히 문제가 생기진 않고 있지만, 혹시 모르니 빼는 게 나을 듯
					index = 0;//다시 앞으로 돌아감
			}
			if (hamm == 1) {
				for (int w = 0; w < len + 1; w++) {
					real_column[q][w] = column[i][w];
					if (w == index)
						column[q][w] = '-'; //ex) 0100과 0101일 경우, '010_'를 column에 더한다 - 다음 column으로 넘길 때 이것만 남기고 전부 지울 거임
				}
				v++; //column에 minterm이 저장되었으므로 갯수(q)를 count한다
			}
		}
	}

	for (i = 0; i < q; i++) {
		for (j = 0; j < len + 1; j++)
			cout << wls_column[i][j];
		cout << endl;
	}
	*/
	//
	//Column III
	/* 
	//wls_column -> prime
	for (i = p; i-p < q; i++) {
		if(usage[i-p]==0){
			for (j = 0; j < len+1; j++)]
				prime[i][j] = wls_column[i-p][j];
	}

	//Show Prime Implicant
	/*
	std::cout<<"Prime Implicant"<<endl;
	for (i = 0; i < p + q + r - 2; i++) { //(<p+q+r-2)인 이유: (p-1)+(q-1)+(r-1)까지 <= 이것도 고쳐야 할 듯
		for (j = 0; j < len + 1; j++)
			cout << prime[i][j];
		cout << endl;
	}
	*/
	//output();
	//정리하기 - 동적할당 해제
	delete[] column;
	delete[] real_column;
	//delete[] wls_column;
	delete[] true_minterm;
	delete[] usage;
	delete[] prime;
	return 0;
}
