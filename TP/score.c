#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <SDL_ttf.h>              //SDL ttf lib
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include <stdint.h>               //use uint32_t
#include "game.h"

/*
score.c
���ھ� ���� ����°��� �Լ���
-Input
-Output
-Sort
*/

void fileInput(int inp) { //���ھ� ���� �ҷ��� ����
	FILE* fp;
	fp = fopen("score.txt", "a");

	fprintf(fp, " %d", inp);

	fclose(fp);
}
void fileRead(char* buffer) //���ھ� ���� �о����
{
	char tempbuf[120];
	FILE* fp = fopen("score.txt", "r");

	fgets(tempbuf, sizeof(tempbuf), fp);

	fclose(fp);
	strcpy(buffer, tempbuf);

}
int refToken(char* buf[], char* inp[]) //���� �о ���鹮�� ������ �߶� �ٸ� �迭�� �ѱ�� �Լ�
{
	int i = 0;
	char* ptr = strtok(buf, " ");

	while (ptr != NULL)
	{
		inp[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}
	return i; //�迭�� ���� ����(��� �߶�����) ��ȯ��
}
void selectionSort(char* pArr[], int num) //��������(���ھ� ���Ŀ�)
{
	int temp[1024];
	for (int j = 0; j < num; j++) {
		temp[j] = atoi(*(pArr + j));
	}
	for (int i = 0; i < num; i++)
	{
		if (temp[i] == NULL)
			return;
		int n = i;
		for (int j = (i + 1); j < num; j++)
		{
			if (temp[j] > temp[n])
				n = j;
		}
		SWAP((temp + i), (temp + n));
	}
	for (int k = 0; k < num; k++) {
		sprintf(*(pArr + k), "%d", *(temp + k));
	} //int�� ���� sort�����Ƿ� �ؽ�Ʈ�� ������ �ϱ� ���ؼ� �ٽ� string���� �ٲپ��ش�
}
void SWAP(int* pa, int* pb) //sort�� �ڸ��ٲٱ� �Լ�
{
	int temp;
	temp = *pa;
	*pa = *pb;
	*pb = temp;
}
