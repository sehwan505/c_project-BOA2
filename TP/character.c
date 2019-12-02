#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <SDL_ttf.h>              //SDL ttf lib
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include <stdint.h>               //use uint32_t
#include "game.h"

/*
character.c
ĳ���Ϳ� ���õ� �Լ���
-���� ��ǥ ��ȯ
-������Ģ ����(�̵�, �ӵ����, ��ǥ���)
-ī�޶� ��ǥ ���
*/

int getPosX(_LPlayer* LP) //���� �÷��̾��� x��ǥ ��ȯ
{
	return LP->mBox.x;
}
int getPosY(_LPlayer* LP) //���� �÷��̾��� y��ǥ ��ȯ
{
	return LP->mBox.y;
}
void V_handleEvent(_LPlayer* LP, SDL_Event* e) //Ű�Է¿� ���� ��ǥ����
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) //Ű�� ��������
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY += LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX -= LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX += LP->Player_VEL; break;
		} //���⿡ �°� �ӵ� ����
	}
	else if (e->type == SDL_KEYUP && e->key.repeat == 0) //Ű�� ������
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY += LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX += LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX -= LP->Player_VEL; break;
		} //���⿡ �°� ������ ����
	}
} //�� ������ ��� ������ �ӵ� ������ �����Ͽ� �Ի��Ѵ�. 
void reverse_V_handleEvent(_LPlayer* LP, SDL_Event* e) //Ű�Է¿� ���� ��ǥ����(���� -> �÷��̾�� ������)
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) //Ű�� ��������
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY += LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX += LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX -= LP->Player_VEL; break;
		} //���⿡ �°� �ӵ� ����
	}
	else if (e->type == SDL_KEYUP && e->key.repeat == 0) //Ű�� ������
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY += LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX -= LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX += LP->Player_VEL; break;
		} //���⿡ �°� ������ ����
	}
} //�� ������ ��� ������ �ӵ� ������ �����Ͽ� �Ի��Ѵ�. 
void move(_LPlayer* LP, _LTile* tiles) //�÷��̾� ���� & �浹ó��
{
	LP->mBox.x += LP->mVelX; //���� �ӵ���ŭ x��ǥ ���

	if ((LP->mBox.x < 0) || (LP->mBox.x + LP->Player_WIDTH > LEVEL_WIDTH) || touchesWall(LP->mBox, tiles))
	{
		LP->mBox.x -= LP->mVelX;
	} //���� �浹�ϰų� �ʹ����� ������ �ٽ� ���ش�.

	LP->mBox.y += LP->mVelY; //���� �ӵ���ŭ y��ǥ ���

	if ((LP->mBox.y < 0) || (LP->mBox.y + LP->Player_HEIGHT > LEVEL_HEIGHT) || touchesWall(LP->mBox, tiles))
	{
		LP->mBox.y -= LP->mVelY;
	} //���� �浹�ϰų� �ʹ����� ������ �ٽ� ���ش�.
}
void setCamera(_LPlayer* LP, SDL_Rect* camera) //ī�޶� ����
{
	camera->x = (getPosX(LP) + LP->Player_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera->y = (getPosY(LP) + LP->Player_HEIGHT / 2) - (SCREEN_HEIGHT * 2 / 3) / 2;
	//ī�޶� ��ǥ �ʱ�ȭ

	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	if (camera->x > LEVEL_WIDTH - camera->w)
	{
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if (camera->y > LEVEL_HEIGHT - camera->h)
	{
		camera->y = LEVEL_HEIGHT - camera->h;
	}
	//ī�޶� ��ǥ�� ������ ������ �����ٸ� ��������
}