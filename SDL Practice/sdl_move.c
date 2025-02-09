#include <SDL.h>                  //SDL lib
#include <SDL_image.h>            //SDL_image lib
#include <SDL_ttf.h>
#include <stdio.h>                //basic C lib
#include <stdbool.h>              //use bool fuctions
#include <stdlib.h>               //basic C lib
#include "han2unicode.h"

const int SCREEN_WIDTH = 640; //스크린 가로
const int SCREEN_HEIGHT = 480; //스크린 세로

enum KeyPressSurfaces //키입력 변수
{
	KEY_PRESS_SURFACE_DEFAULT, //=0
	KEY_PRESS_SURFACE_UP,      //=1
	KEY_PRESS_SURFACE_DOWN,    //=2
	KEY_PRESS_SURFACE_LEFT,    //=3
	KEY_PRESS_SURFACE_RIGHT,   //=4
	KEY_PRESS_SURFACE_TOTAL,   //=5
};

typedef struct _LTexture       //텍스쳐 구조체
{
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

}_LTexture;
typedef struct _LPlayer
{
	int Player_WIDTH;
	int Player_HEIGHT;
	int Player_VEL;

	int mPosX, mPosY;
	int mVelX, mVelY;
}_LPlayer;

struct _LTexture gBackgroundTexture;
struct _LTexture gMainplayerTexture;
struct _LTexture gDuckTexture;
struct _LTexture gTextTexture;

struct _LPlayer gPlayer;


bool init(); //초기화
bool loadMedia(); //미디어 로딩
void close(); // SDL 라이브러리 닫기
bool loadFromFile(struct _LTexture* LT, SDL_Renderer* Renderer, char* path);
void lfree(struct _LTexture* LT);
void render(_LTexture* LT, SDL_Renderer* Renderer, int x, int y); //지정된 좌표에 랜더링
bool loadFromRenderedText(_LTexture* LT, SDL_Renderer* Renderer, TTF_Font* Font, char* stringText, SDL_Color textColor);
void handleEvent(_LPlayer* LP, SDL_Event*e);
void move(_LPlayer* LP);

SDL_Surface* loadSurface(char* path); //경로에 있는 서피스 로드 함수
SDL_Texture* loadTexture(char* path); //위와 같은 기능 ( 서피스보다 성능 좋음)

SDL_Window* gWindow = NULL; //랜더링 윈도우 포인터
SDL_Surface* gScreenSurface = NULL; //스크린서피스 포인터


SDL_Renderer* gRenderer = NULL;     //랜더러 포인터

SDL_Texture* gTexture = NULL;       //기본 텍스쳐 포인터
SDL_Texture* gLetterbox = NULL;
SDL_Texture* gMinimap = NULL;

SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; //키에 따른 이미지 포인터

SDL_Surface* gCurrentSurface = NULL; //현재 표시되는 이미지를 가리키는 포인터

TTF_Font* gFont = NULL;

int main()
{

	if (!init())
	{
		printf("초기화 실패!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//기본 키 설정 서피스
			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					handleEvent(&gPlayer, &e);
					/*
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							printf("UP key\n");
							break;
						case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							printf("DOWN key\n");
							break;
						case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							printf("LEFT key\n");
							break;
						case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							printf("RIGHT key\n");
							break;
						default:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
					*/
				}

				//이미지 적용
				//SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				//서피스 업데이트
				//SDL_UpdateWindowSurface(gWindow);

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				move(&gPlayer);

				//흰색화면 랜더링 후 랜더러 클리어

				SDL_Rect botLeftViewport;
				botLeftViewport.x = 0;
				botLeftViewport.y = SCREEN_HEIGHT * 2 / 3;
				botLeftViewport.w = SCREEN_WIDTH * 2 / 3;
				botLeftViewport.h = SCREEN_HEIGHT / 3;
				SDL_RenderSetViewport(gRenderer, &botLeftViewport);

				SDL_RenderCopy(gRenderer, gLetterbox, NULL, NULL);
				render(&gTextTexture, gRenderer, 12, 12);



				SDL_Rect botRightViewport;
				botRightViewport.x = SCREEN_WIDTH * 2 / 3;
				botRightViewport.y = SCREEN_HEIGHT * 2 / 3;
				botRightViewport.w = SCREEN_WIDTH / 3;
				botRightViewport.h = SCREEN_HEIGHT / 3;
				SDL_RenderSetViewport(gRenderer, &botRightViewport);

				SDL_RenderCopy(gRenderer, gMinimap, NULL, NULL);



				SDL_Rect topViewport;
				topViewport.x = 0;
				topViewport.y = 0;
				topViewport.w = SCREEN_WIDTH;
				topViewport.h = SCREEN_HEIGHT * 2 / 3;
				SDL_RenderSetViewport(gRenderer, &topViewport); //위에서 설정한 뷰포트로 랜더러 설정

				//SDL_RenderCopy(gRenderer, gBackgroundTexture->mTexture, NULL, NULL); //텍스쳐 -> 랜더러 로 카피
				render(&gBackgroundTexture, gRenderer, 0, 0);
				render(&gMainplayerTexture, gRenderer, 24, 19);
				
				render(&gDuckTexture, gRenderer, gPlayer.mPosX, gPlayer.mPosY);

				SDL_RenderPresent(gRenderer);  //Update

			}
		}

	}

	close();

	return 0;
}

bool init()
{
	bool Sflag = true; //success flag

	//SDL 초기화
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL을 초기화 할 수 없습니다. : %s", SDL_GetError());
		Sflag = false;
	}
	else
	{
		//윈도우 생성
		gWindow = SDL_CreateWindow("Sprite Render", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("윈도우를 생성할 수 없습니다. : %s", SDL_GetError());
			Sflag = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); //랜더러 생성
			if (gRenderer == NULL)
			{
				printf("랜더러 생성 실패! : %s", SDL_GetError());
				Sflag = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //랜더러를 흰색화면으로 그리도록 세팅


				int imgFlags = IMG_INIT_PNG; //PNG 로딩 플래그
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL이미지 모듈을 초기화 할 수 없습니다! : %s", IMG_GetError());
					Sflag = false;
				}
				else if (TTF_Init() == -1)
				{
					printf("SDL_TTF 모듈을 초기화 할 수 없습니다! : %s", TTF_GetError());
					Sflag = false;
				}
				else
				{
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}


			}
		}
	}
	//아니 왜 함수 바깥에서 초기화 안됨? 그래서 여기서 함ㅋㅋ
	gPlayer.Player_HEIGHT = 20;
	gPlayer.Player_WIDTH = 20;
	gPlayer.Player_VEL = 1;
	gPlayer.mPosX = 0;
	gPlayer.mPosY = 0;
	gPlayer.mVelX = 0;
	gPlayer.mVelY = 0;

	return Sflag;

}

SDL_Surface* loadSurface(char* path)
{
	//경로의 이미지 로드
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL) {
		printf("이미지 로드 실패! : %s\n", SDL_GetError());
	}
	return loadedSurface;
}

SDL_Texture* loadTexture(char* path)
{
	SDL_Texture* newTexture = NULL;

	//SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("이미지 로드 실패! : %s\n", IMG_GetError()); //IMG모듈 사용시 SDL_GetError 대신 IMG_GetError사용
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("텍스쳐 생성 실패! : %s\n", SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;

}

bool loadMedia()
{
	bool Sflag = true; //success flag

	/* 방향키에 따른 서피스 로드
	//디폴트
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("example.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("디폴트 이미지 로드 실패!\n");
		Sflag = false;
	}
	//업
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("위 이미지 로드 실패!\n");
		Sflag = false;
	}
	//다운
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("아래 이미지 로드 실패!\n");
		Sflag = false;
	}
	//왼쪽
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("왼쪽 이미지 로드 실패!\n");
		Sflag = false;
	}
	//오른쪽
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("오른쪽 이미지 로드 실패!\n");
		Sflag = false;
	}
	*/
	/*
	gTexture = loadTexture("example.png");
	if (gTexture == NULL)
	{
		printf("텍스쳐이미지 로드 실패!\n");
		Sflag = false;
	}
	*/
	loadFromFile(&gBackgroundTexture, gRenderer, "BG.png");
	if (gBackgroundTexture.mTexture == NULL)
	{
		printf("백그라운드 텍스쳐 로드 실패!\n");
		Sflag = false;
	}

	loadFromFile(&gMainplayerTexture, gRenderer, "player.png");
	if (gMainplayerTexture.mTexture == NULL)
	{
		printf("메인플레이어 텍스쳐 로드 실패!\n");
		Sflag = false;
	}
	loadFromFile(&gDuckTexture, gRenderer, "duck.png");
	if (gDuckTexture.mTexture == NULL)
	{
		printf("오리 로드 실패!\n");
		Sflag = false;
	}

	gLetterbox = loadTexture("Lbox.png");
	if (gLetterbox == NULL)
	{
		printf("텍스쳐이미지(gLetterbox) 로드 실패!\n");
		Sflag = false;
	}

	gMinimap = loadTexture("minimap.png");
	if (gMinimap == NULL)
	{
		printf("텍스쳐이미지(gMinimap) 로드 실패!\n");
		Sflag = false;
	}

	//위랑 같이 폰트 불러옴
	gFont = TTF_OpenFont("korean.ttf", 28);
	if (gFont == NULL)
	{
		printf("폰트 로드 실패! : %s", TTF_GetError());
		Sflag = false;
	}
	else
	{
		SDL_Color textColor = { 255,0,0 };
		if (!loadFromRenderedText(&gTextTexture, gRenderer, gFont, "오리는 꽥꽥", textColor))
		{
			printf("텍스쳐 랜더 실패!");
			Sflag = false;
		}
	}

	//위와 같이 원하는 이미지를 포인터에 박은후 랜더러에 각각 매치시켜서 업데이트 
	return Sflag;
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyTexture(gMinimap);
	gMinimap = NULL;

	SDL_DestroyTexture(gLetterbox);
	gLetterbox = NULL;

	lfree(&gBackgroundTexture);
	lfree(&gMainplayerTexture);
	lfree(&gFont);
	//서피스 초기화
	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;


	TTF_CloseFont(gFont);
	gFont = NULL;
	//윈도우 닫기
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//sdl 라이브러리 닫기
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

bool loadFromFile(_LTexture* LT, SDL_Renderer* Renderer, char* path)
{

	lfree(LT);

	SDL_Texture* newTexture = NULL;  //텍스쳐 포인터

	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface == NULL)
	{
		printf("이미지 로드 실패! : %s", IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("텍스쳐 생성 실패! : %s", SDL_GetError());
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

void lfree(_LTexture* LT) //텍스쳐 구조체에 접근해서 널로 초기화
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

void render(_LTexture* LT, SDL_Renderer* Renderer, int x, int y) //랜더링
{
	SDL_Rect renderQuad = { x,y,LT->mWidth,LT->mHeight };
	SDL_RenderCopy(Renderer, LT->mTexture, NULL, &renderQuad);
}

bool loadFromRenderedText(_LTexture* LT, SDL_Renderer* Renderer, TTF_Font* Font, char* stringText, SDL_Color textColor)
{
	lfree(LT);
	unsigned short unicode[128]; //유니코드 배열 만들어서
	han2unicode(stringText, unicode); //스트링을 유니코드로 변환
	SDL_Surface* textSurface = TTF_RenderUNICODE_Solid(Font, unicode, textColor); //서피스 포인터에 폰트 랜더함수넣기
	if (textSurface == NULL)
	{
		printf("텍스쳐 서피스 랜더 실패! : %s\n", TTF_GetError());
	}
	else
	{
		LT->mTexture = SDL_CreateTextureFromSurface(Renderer, textSurface);
		if (LT->mTexture == NULL)
		{
			printf("텍스쳐 생성 실패 : %s", SDL_GetError());
		}
		else
		{
			LT->mWidth = textSurface->w;
			LT->mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return LT->mTexture != NULL;
}//텍스쳐 로딩

void handleEvent(_LPlayer* LP, SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY += LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX -= LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX += LP->Player_VEL; break;
		}
	}
	else if (e->type == SDL_KEYUP && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_UP: LP->mVelY += LP->Player_VEL; break;
		case SDLK_DOWN: LP->mVelY -= LP->Player_VEL; break;
		case SDLK_LEFT: LP->mVelX += LP->Player_VEL; break;
		case SDLK_RIGHT: LP->mVelX -= LP->Player_VEL; break;
		}
	}
}

void move(_LPlayer* LP)
{
	LP->mPosX += LP->mVelX;

	if ((LP->mPosX < 0) || (LP->mPosX + LP->Player_WIDTH > SCREEN_WIDTH))
	{
		LP->mPosX -= LP->mVelX;
	}

	LP->mPosY += LP->mVelY;

	if ((LP->mPosY < 0) || (LP->mPosY + LP->Player_HEIGHT> SCREEN_HEIGHT*2/3))
	{
		LP->mPosY -= LP->mVelY;
	}
}