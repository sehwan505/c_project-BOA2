#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include <stdint.h>               //use uint32_t
#include <SDL.h>  
#include "timer.h"

/*
timer.c
fps ����� Ÿ�̸� ���� �Լ�
-Ÿ�̸� ����
-Ÿ�̸� ����
-Ÿ�̸� �Ͻ�����
-Ÿ�̸� �Ͻ����� ����
-����ð�
*/

void timer_start(_LTimer* timer)
{
	timer->mStarted = true;
	timer->mPaused = false;
	timer->mStartTicks = SDL_GetTicks();//SDL���̺귯���� �ʱ�ȭ �� �� ������ �ð�(ms) ����
	timer->mPauseTicks = 0;
}//Ÿ�̸Ӹ� ���۽�Ŵ
void timer_stop(_LTimer* timer)
{
	timer->mStarted = false;
	timer->mPaused = false;
	timer->mStartTicks = 0;
	timer->mPauseTicks = 0;
}//Ÿ�̸Ӹ� ��� �ʱ�ȭ��
void timer_pause(_LTimer* timer)
{
	if (timer->mStarted && !(timer->mPaused)) //Ÿ�̸Ӱ� ���۵ưų� ������°� �ƴ϶��
	{
		timer->mPaused = true; //������·� �����
		timer->mPauseTicks = SDL_GetTicks() - (timer->mStartTicks); //�Ͻ������� �ð��� ����
		timer->mStartTicks = 0;
	}
}
void timer_unpause(_LTimer* timer) 
{
	if (timer->mStarted && timer->mPaused) //Ÿ�̸Ӱ� ���۵ưų� ������¶��
	{
		timer->mPaused = false; 
		timer->mStartTicks = SDL_GetTicks() - (timer->mPauseTicks); //�Ͻ������� �ð��� �ٽ� �ҷ���
		timer->mPauseTicks = 0;

	}
}
Uint32 getTicks(_LTimer* timer)
{
	Uint32 time = 0;

	if (timer->mStarted) //Ÿ�̸Ӱ� ���۵� ���¶��
	{
		if (timer->mPaused) //Ÿ�̸Ӱ� ����ٸ�
		{
			time = timer->mPauseTicks; //���� �ð��� ����
		}
		else
		{
			time = SDL_GetTicks() - timer->mStartTicks; //Ÿ�̸Ӱ� ������°� �ƴ϶�� Ÿ�̸Ӱ� ���� �ð��� ����
		}
	}
	return time;
} //SDL �⺻�Լ��� SDL_GetTicks�� �����Ͽ� Ÿ�̸� ����ü�� �µ��� ����
bool timer_isStarted(_LTimer* timer)
{
	return timer->mStarted; //���ۿ��� ����
}
bool timer_isPaused(_LTimer* timer)
{
	return timer->mPaused && timer->mStarted; //���㿩�� ����
}