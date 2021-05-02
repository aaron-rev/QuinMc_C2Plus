#include <stdio.h>

int main()
{
	FILE *fp;
	char input[10][20];
	
	fp = fopen("input_minterm.txt", "r");
	if(fp == NULL)
	{
		printf("파일이 열리지 않았습니다.\n");
		return 1;
	}
	
	for(int i = 0; i < 10; i++)
	{
		fgets(input[i], sizeof(input[i]), fp);
	}
	
	for(int i = 0; i < 10; i++)
	{
		printf("%s", input[i]);
	}
	
	fclose(fp);
	return 0;
}
