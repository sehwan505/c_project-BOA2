#pragma once

enum KeyPressSurfaces //Ű�Է� ������ ��� ������
{
	KEY_PRESS_SURFACE_DEFAULT, //=0
	KEY_PRESS_SURFACE_UP,      //=1
	KEY_PRESS_SURFACE_DOWN,    //=2
	KEY_PRESS_SURFACE_LEFT,    //=3
	KEY_PRESS_SURFACE_RIGHT,   //=4
	KEY_PRESS_SURFACE_O,       //=5
	KEY_PRESS_SURFACE_TOTAL,   //=6
};

typedef struct _LTexture       //�ؽ��� ����ü
{
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

}_LTexture;

typedef struct _LPlayer //�÷��̾� ����ü
{
	int Player_WIDTH;
	int Player_HEIGHT;
	int Player_VEL;

	SDL_Rect mBox;
	int mVelX, mVelY;
}_LPlayer;

typedef struct _LTile //Ÿ�� ����ü
{
	SDL_Rect mBox;
	int mType;
}_LTile;

extern SDL_Renderer* gRenderer;									 //������ ������
extern SDL_Surface* loadSurface(char* path);					 //��ο� �ִ� ���ǽ� �ε� �Լ�
extern SDL_Texture* loadTexture(char* path);					 //���� ���� ��� ( ���ǽ����� ���� ����)
extern SDL_Window* gWindow;										 //������ ������ ������
extern SDL_Surface* gScreenSurface;								 //��ũ�����ǽ� ������
extern SDL_Texture* gTexture;									 //�⺻ �ؽ��� ������
extern SDL_Texture* gLetterbox ;								 //���ʾƷ� �޽����ڽ� ������
extern SDL_Texture* gMinimap;									 //������ �Ʒ� �̴ϸ� ������
extern TTF_Font* gFont;											 //�⺻ ��Ʈ ������

//������,��ũ�� ���� ���
extern const int LEVEL_WIDTH ;									 //������ ����
extern const int LEVEL_HEIGHT;									 //������ ����

extern const int SCREEN_WIDTH ;									 //��ũ�� ����
extern const int SCREEN_HEIGHT ;								 //��ũ�� ����

extern const int SCREEN_FPS ;								     //��ũ�� ������
extern const int SCREEN_TICK_PER_FRAME ;						 //ƽ�� ������

//Ÿ�� ���
extern const int TILE_WIDTH;									 //Ÿ�� ����ũ��
extern const int TILE_HEIGHT;									 //Ÿ�� ����ũ��
extern const int TOTAL_TILES;									 //�� Ÿ�� ����
extern const int TOTAL_TILE_SPRITES;							 //�� Ÿ�� ��������Ʈ ����


//�ؽ��� �� ������ ����ü�� ����
extern struct _LTexture gMainplayerTexture[KEY_PRESS_SURFACE_TOTAL+2];	//����ĳ���� �ؽ���
extern struct _LTexture gDuckTexture[KEY_PRESS_SURFACE_TOTAL+2];		//���� �ؽ���
extern struct _LTexture gCurrentDuck;									//���� ���� ���
extern struct _LTexture gCurrentSurface;								//���� ǥ�õǴ� ���ǽ�

extern struct _LTexture gTimeText;										//Ÿ�� �ؽ�Ʈ(Time : )
extern struct _LTexture gCurrentTime;									//���� �ð� �ؽ���
extern struct _LTexture gSightLimiter;									//�þ� ������
extern struct _LTexture gTextTexture[2];								//�ý�Ʈ�� ����ü �迭�� ���Ͽ� �̸� ����־����, �̺�Ʈ�� ���� ������ ������ �� �� �ִ�.
extern struct _LTexture gCurrentText;								    //���� ǥ�õǴ� �ؽ�Ʈ
extern struct _LTexture gTileTexture[23];								//Ÿ�ϼ� �ؽ�ó

extern struct _LTexture gLeaderBoard[5];								//�������� �ؽ�Ʈ
extern struct _LTexture gScore[5];										//���ھ� �ؽ�Ʈ
extern struct _LTexture gStartText;										//��ŸƮ �ؽ�Ʈ

extern struct _LPlayer gPlayer;											//�÷��̾� ����ü
extern struct _LPlayer gDuck[5];										//���� ����ü

extern struct _LTimer timer;											//Ÿ�̸� ����ü


//�Լ� ����
bool init(); //�ʱ�ȭ
bool loadMedia(_LTile*); //�̵�� �ε�
void close(); // SDL ���̺귯�� �ݱ�
bool loadFromFile(_LTexture*, SDL_Renderer*, char*);  //�ؽ��� �ҷ�����
void lfree(_LTexture*); //�ݱ�
void render(_LTexture* , SDL_Renderer*, int , int ); //������ ��ǥ�� ������
bool loadFromRenderedText(_LTexture*, SDL_Renderer* , TTF_Font*, char* , SDL_Color); //�ؽ�Ʈ �ҷ�����
void V_handleEvent(_LPlayer*, SDL_Event*);  //�̺�Ʈ�� ���� �ӵ�����
void reverse_V_handleEvent(_LPlayer*, SDL_Event*);  //�̺�Ʈ�� ���� �ӵ�����
void T_handleEvent(_LTexture* , _LTexture* , SDL_Event*,int ); //�̺�Ʈ�� ���� �ؽ��� ����
void reverse_T_handleEvent(_LTexture*, _LTexture*, SDL_Event*, int); //�̺�Ʈ�� ���� �ؽ��� ����
bool setTiles(_LTile *);
bool touchesWall(SDL_Rect , _LTile* );

//character
void move(_LPlayer* , _LTile *);
bool checkCollision(SDL_Rect , SDL_Rect ); //�浹����
int getPosX(_LPlayer* );
int getPosY(_LPlayer* );
void setCamera(_LPlayer* , SDL_Rect* );

//fileIO
void fileRead(char* buffer);
int refToken(char* buf[], char *inp[]);
void selectionSort(char* pArr[], int num);
void SWAP(int *pa, int *pb);
void fileInput(int inp);