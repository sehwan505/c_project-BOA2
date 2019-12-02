#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <SDL_ttf.h>              //SDL ttf lib
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include <stdint.h>               //use uint32_t
#include "game.h"
#include "han2unicode.h"
#include "timer.h"



bool init() //�ʱ�ȭ �Լ�
{
	bool Sflag = true; //success flag

	//SDL �ʱ�ȭ
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL�� �ʱ�ȭ �� �� �����ϴ�. : %s", SDL_GetError());
		Sflag = false;
	}
	else
	{
		//������ ����
		gWindow = SDL_CreateWindow("BoA2 Production", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("�����츦 ������ �� �����ϴ�. : %s", SDL_GetError());
			Sflag = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); //������ ����
			if (gRenderer == NULL)
			{
				printf("������ ���� ����! : %s", SDL_GetError());
				Sflag = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //�������� ���ȭ������ �׸����� ����


				int imgFlags = IMG_INIT_PNG; //PNG �ε� �÷���
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL�̹��� ����� �ʱ�ȭ �� �� �����ϴ�! : %s", IMG_GetError());
					Sflag = false;
				}
				else if (TTF_Init() == -1)
				{
					printf("SDL_TTF ����� �ʱ�ȭ �� �� �����ϴ�! : %s", TTF_GetError());
					Sflag = false;
				}
				else
				{
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}


			}
		}
	}
	//�÷��̾�&Ÿ�̸� ���� �ʱ�ȭ

	gPlayer.Player_HEIGHT = 78;
	gPlayer.Player_WIDTH = 60;
	gPlayer.Player_VEL = 3;

	//�⺻ �÷��̾� ��ġ ����
	gPlayer.mBox.x = 0;
	gPlayer.mBox.y = 0;
	gPlayer.mBox.w = gPlayer.Player_WIDTH;
	gPlayer.mBox.h = gPlayer.Player_HEIGHT;
	gPlayer.mVelX = 0;
	gPlayer.mVelY = 0;

	//���� ���� �ʱ�ȭ
	for (int i = 0; i < 5; i++)
	{
		gDuck[i].Player_HEIGHT = 75;
		gDuck[i].Player_WIDTH = 75;
		gDuck[i].Player_VEL = 4;
		gDuck[i].mBox.w = gDuck[i].Player_WIDTH;
		gDuck[i].mBox.h = gDuck[i].Player_HEIGHT;
		gDuck[i].mVelX = 0;
		gDuck[i].mVelY = 0;
		gDuck[i].mBox.x = 0;
		gDuck[i].mBox.y = 0;
	}

	//Ÿ�̸� ����ü �ɹ� �ʱ�ȭ
	timer.mStartTicks = 0;
	timer.mPauseTicks = 0;
	timer.mPaused = false;
	timer.mStarted = false;

	return Sflag;

}
SDL_Surface* loadSurface(char* path)  //�̹��� �ε� �Լ�(��Ʈ�� �̹���)
{
	//����� �̹��� �ε�
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL) {
		printf("�̹��� �ε� ����! : %s\n", SDL_GetError());
	}
	return loadedSurface;
}
SDL_Texture* loadTexture(char* path)  //�̹��� �ε�&�ؽ��� ��ȯ �Լ�(PNG)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("�̹��� �ε� ����! : %s\n", IMG_GetError()); //IMG��� ���� SDL_GetError ��� IMG_GetError���
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("�ؽ��� ���� ����! : %s\n", SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface); //�ڿ� �ݳ�
	}
	return newTexture;

}
bool loadMedia(_LTile* tiles[]) //���� �̹��� �ε�
{
	bool Sflag = true; //success flag

	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT], gRenderer, "images/player_front.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_DEFAULT].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ��� �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_UP], gRenderer, "images/player_back.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_UP].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(up) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_DOWN], gRenderer, "images/player_front.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_DOWN].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(down) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_LEFT], gRenderer, "images/player_left.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_LEFT].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(left) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_RIGHT], gRenderer, "images/player_right.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_RIGHT].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(right) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[KEY_PRESS_SURFACE_O], gRenderer, "images/duck_insane.png");
	if (gMainplayerTexture[KEY_PRESS_SURFACE_O].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(insane) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[6], gRenderer, "images/player_left02.png");
	if (gMainplayerTexture[6].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(left02) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gMainplayerTexture[7], gRenderer, "images/player_right02.png");
	if (gMainplayerTexture[7].mTexture == NULL)
	{
		printf("�����÷��̾� ����Ʈ �ؽ���(right02) �ε� ����!\n");
		Sflag = false;
	}


	loadFromFile(&gDuckTexture[0], gRenderer, "images/duck_front.png");
	if (gDuckTexture[0].mTexture == NULL)
	{
		printf("����(����Ʈ) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[1], gRenderer, "images/duck_back.png");
	if (gDuckTexture[1].mTexture == NULL)
	{
		printf("����(��) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[2], gRenderer, "images/duck_front.png");
	if (gDuckTexture[2].mTexture == NULL)
	{
		printf("����(��) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[3], gRenderer, "images/duck_left.png");
	if (gDuckTexture[3].mTexture == NULL)
	{
		printf("����(left) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[4], gRenderer, "images/duck_right.png");
	if (gDuckTexture[4].mTexture == NULL)
	{
		printf("����(right) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[5], gRenderer, "images/duck_insane.png");
	if (gDuckTexture[5].mTexture == NULL)
	{
		printf("����(insane) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[6], gRenderer, "images/duck_left_02.png");
	if (gDuckTexture[6].mTexture == NULL)
	{
		printf("����(left02) �ε� ����!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture[7], gRenderer, "images/duck_right_02.png");
	if (gDuckTexture[7].mTexture == NULL)
	{
		printf("����(right02) �ε� ����!\n");
		Sflag = false;
	}


	loadFromFile(&gSightLimiter, gRenderer, "images/SightLimiter.png");
	if (gSightLimiter.mTexture == NULL)
	{
		printf("����Ʈ������ �ε� ����!\n");
		Sflag = false;
	}
	gLetterbox = loadTexture("images/Lbox.png");
	if (gLetterbox == NULL)
	{
		printf("�ؽ����̹���(gLetterbox) �ε� ����!\n");
		Sflag = false;
	}

	gMinimap = loadTexture("images/minimap.png");
	if (gMinimap == NULL)
	{
		printf("�ؽ����̹���(gMinimap) �ε� ����!\n");
		Sflag = false;
	}

	// ��Ʈ �ҷ���
	gFont = TTF_OpenFont("korean.ttf", 28);
	if (gFont == NULL)
	{
		printf("��Ʈ �ε� ����! : %s", TTF_GetError());
		Sflag = false;
	}
	else
	{
		SDL_Color textColor = { 255,255,255 };

		if (!loadFromRenderedText(&gTextTexture[0], gRenderer, gFont, "������ ���� ��Ƴ�������!", textColor))
		{
			printf("�ؽ��� ���� ����!");
			Sflag = false;
		}
		if (!loadFromRenderedText(&gTimeText, gRenderer, gFont, "Time :", textColor))
		{
			printf("Ÿ�̸Ӹ� ������ �� �����ϴ�! \n");
		}
		if (!loadFromRenderedText(&gLeaderBoard[0], gRenderer, gFont, "1�� : ", textColor))
		{
			printf("�������� ���� ����!");
			Sflag = false;
		}
		if (!loadFromRenderedText(&gLeaderBoard[1], gRenderer, gFont, "2�� : ", textColor))
		{
			printf("�������� ���� ����!");
			Sflag = false;
		}
		if (!loadFromRenderedText(&gLeaderBoard[2], gRenderer, gFont, "3�� : ", textColor))
		{
			printf("�������� ���� ����!");
			Sflag = false;
		}
		if (!loadFromRenderedText(&gLeaderBoard[3], gRenderer, gFont, "4�� : ", textColor))
		{
			printf("�������� ���� ����!");
			Sflag = false;
		}
		if (!loadFromRenderedText(&gLeaderBoard[4], gRenderer, gFont, "5�� : ", textColor))
		{
			printf("�������� ���� ����!");
			Sflag = false;
		}

		if (!loadFromRenderedText(&gStartText, gRenderer, gFont, "EnterŰ�� ���� ������ �����ϼ���", textColor))
		{
			printf("��ŸƮ�ؽ�Ʈ ���� ����!");
			Sflag = false;
		}

		textColor.g = 0;
		textColor.b = 0;
		if (!loadFromRenderedText(&gTextTexture[1], gRenderer, gFont, "�ð��� �� ���� �ʾ� ������ ����� ���������ϴ�!", textColor))
		{
			printf("�ؽ��� ���� ����!");
			Sflag = false;
		}
	}

	//Ÿ�� �ؽ��� ����
	loadFromFile(&gTileTexture[0], gRenderer, "images/tiles/00.png");
	loadFromFile(&gTileTexture[1], gRenderer, "images/tiles/01.png");
	loadFromFile(&gTileTexture[2], gRenderer, "images/tiles/02.png");
	loadFromFile(&gTileTexture[3], gRenderer, "images/tiles/03.png");
	loadFromFile(&gTileTexture[4], gRenderer, "images/tiles/04.png");
	loadFromFile(&gTileTexture[5], gRenderer, "images/tiles/05.png");
	loadFromFile(&gTileTexture[6], gRenderer, "images/tiles/06.png");
	loadFromFile(&gTileTexture[7], gRenderer, "images/tiles/07.png");
	loadFromFile(&gTileTexture[8], gRenderer, "images/tiles/08.png");
	loadFromFile(&gTileTexture[9], gRenderer, "images/tiles/09.png");
	loadFromFile(&gTileTexture[10], gRenderer, "images/tiles/10.png");
	loadFromFile(&gTileTexture[11], gRenderer, "images/tiles/11.png");
	loadFromFile(&gTileTexture[12], gRenderer, "images/tiles/12.png");
	loadFromFile(&gTileTexture[13], gRenderer, "images/tiles/13.png");
	loadFromFile(&gTileTexture[14], gRenderer, "images/tiles/14.png");
	loadFromFile(&gTileTexture[15], gRenderer, "images/tiles/15.png");
	loadFromFile(&gTileTexture[16], gRenderer, "images/tiles/16.png");
	loadFromFile(&gTileTexture[17], gRenderer, "images/tiles/17.png");
	loadFromFile(&gTileTexture[18], gRenderer, "images/tiles/18.png");
	loadFromFile(&gTileTexture[19], gRenderer, "images/tiles/19.png");
	loadFromFile(&gTileTexture[20], gRenderer, "images/tiles/20.png");
	loadFromFile(&gTileTexture[21], gRenderer, "images/tiles/21.png");
	loadFromFile(&gTileTexture[22], gRenderer, "images/tiles/22.png");
	for (int i = 0; i < 23; i++) {
		if (gTileTexture[i].mTexture == NULL)
		{
			printf("Ÿ�� �ؽ���(%d) �ε� ����! \n", i);
			Sflag = false;
			break;
		}
	}
	if (!setTiles(tiles))
	{
		printf("Ÿ�ϼ� ���� ����!\n");
		Sflag = false;
	}
	//���� ���� ���ϴ� �̹����� �����Ϳ� ������ �������� ���� ��ġ���Ѽ� ������Ʈ 
	return Sflag;
}
void close() //�ڿ� �ݳ� & ���̺귯�� ����
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyTexture(gMinimap);
	gMinimap = NULL;

	SDL_DestroyTexture(gLetterbox);
	gLetterbox = NULL;

	for (enum KeyPressSurfaces i = KEY_PRESS_SURFACE_DEFAULT; i < KEY_PRESS_SURFACE_TOTAL + 2; i++)
		lfree(&gMainplayerTexture[i]);
	for (enum KeyPressSurfaces i = KEY_PRESS_SURFACE_DEFAULT; i < KEY_PRESS_SURFACE_TOTAL; i++)
		lfree(&gMainplayerTexture[i]);
	for (int i = 0; i < 12; i++)
		lfree(&gTileTexture[i]);
	lfree(&gFont);
	//���ǽ� �ʱ�ȭ
	lfree(&gCurrentSurface);


	TTF_CloseFont(gFont);
	gFont = NULL;
	//������ �ݱ�
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//sdl ���̺귯�� �ݱ�
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}
bool loadFromFile(_LTexture * LT, SDL_Renderer * Renderer, char* path) //PNG�ε��ؼ� �ؽ��� ����(�̹��� ũ�⵵ ����) -> �ؽ��� ����ü�� ��ȯ
{
	lfree(LT);

	SDL_Texture* newTexture = NULL;  //�ؽ��� ������

	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface == NULL)
	{
		printf("�̹��� �ε� ����! : %s", IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF)); //�÷�Ű ����(������ RGB�� �������� ��������)
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("�ؽ��� ���� ����! : %s", SDL_GetError());
		}
		else
		{
			LT->mWidth = loadedSurface->w;
			LT->mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}

	LT->mTexture = newTexture;
	return LT->mTexture != NULL;
}
void lfree(_LTexture * LT) //�ؽ��� ����ü�� �����ؼ� �η� �ʱ�ȭ
{
	if (LT != NULL)
	{
		if (LT->mTexture != NULL)
		{
			SDL_DestroyTexture(LT->mTexture);
			LT->mTexture = NULL;
			LT->mWidth = 0;
			LT->mHeight = 0;
		}
	}
}
void render(_LTexture * LT, SDL_Renderer * Renderer, int x, int y) //������
{
	SDL_Rect renderQuad = { x,y,LT->mWidth,LT->mHeight };
	SDL_RenderCopy(Renderer, LT->mTexture, NULL, &renderQuad);
}
bool loadFromRenderedText(_LTexture * LT, SDL_Renderer * Renderer, TTF_Font * Font, char* stringText, SDL_Color textColor)
{
	lfree(LT);
	unsigned short unicode[128]; //�����ڵ� �迭 ����
	han2unicode(stringText, unicode); //��Ʈ���� �����ڵ�� ��ȯ
	SDL_Surface* textSurface = TTF_RenderUNICODE_Solid(Font, unicode, textColor); //���ǽ� �����Ϳ� ��Ʈ �����Լ��ֱ�
	if (textSurface == NULL)
	{
		printf("�ؽ��� ���ǽ� ���� ����! : %s\n", TTF_GetError());
	}
	else
	{
		LT->mTexture = SDL_CreateTextureFromSurface(Renderer, textSurface);
		if (LT->mTexture == NULL)
		{
			printf("�ؽ��� ���� ���� : %s", SDL_GetError());
		}
		else
		{
			LT->mWidth = textSurface->w;
			LT->mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return LT->mTexture != NULL;
}//�ؽ��� �ε�