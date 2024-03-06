/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 03 - KEYBOARD AND OBJECT STATE

	This sample illustrates how to:

		1/ Process keyboard input
		2/ Control object state with keyboard events
================================================================ */

#include <windows.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Animation.h"
#include "Animations.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Mario.h"
#include "Brick.h"
#include "Bill.h"
#include "Enemy.h"

#include "SampleKeyEventHandler.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"03 - Keyboard and Mario states"
#define WINDOW_ICON_PATH L"mario.ico"


#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

#define ID_TEX_BILL 555

#define ID_SPRITE_BRICK 20001

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc.png"
#define TEXTURE_PATH_BILL TEXTURES_DIR "\\Players_no_bg.png"

#define MARIO_START_X 200.0f
#define MARIO_START_Y 50.0f

#define BRICK_X 0.0f
#define BRICK_Y GROUND_Y + 20.0f
#define NUM_BRICKS 50

CMario* mario = NULL;
CBill* bill = NULL;
CEnemy* enemy = NULL;

CSampleKeyHandler* keyHandler;

vector<LPGAMEOBJECT> objects;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	LPANIMATION ani;

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_BILL, TEXTURE_PATH_BILL);

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPTEXTURE texEnemy = textures->Get(ID_TEX_BILL);
	//IDLE RIGHT AND LEFT
	sprites->Add(8001, 546, 292, 569, 327, texEnemy);
	sprites->Add(8002, 569, 292, 546, 327, texEnemy);

	//ENEMY WALKING RIGHT
	sprites->Add(8011, 25, 292, 43, 327, texEnemy);
	sprites->Add(8012, 90, 293, 109, 326, texEnemy);
	sprites->Add(8013, 157, 292, 172, 327, texEnemy);
	sprites->Add(8014, 220, 292, 237, 327, texEnemy);
	sprites->Add(8015, 285, 293, 304, 326, texEnemy);
	sprites->Add(8016, 353, 292, 368, 327, texEnemy);

	//ENEMY WALKING LEFT
	sprites->Add(8021, 43, 292, 25, 327, texEnemy);
	sprites->Add(8022, 109, 293, 90, 326, texEnemy);
	sprites->Add(8023, 172, 292, 157, 327, texEnemy);
	sprites->Add(8024, 237, 292, 220, 327, texEnemy);
	sprites->Add(8025, 304, 293, 285, 326, texEnemy);
	sprites->Add(8026, 368, 292, 353, 327, texEnemy);

	//ENEMY WALKING UP
	sprites->Add(8031, 673, 489, 693, 529, texEnemy);
	sprites->Add(8032, 739, 491, 759, 531, texEnemy);
											  
	//ENEMY WALKING DOWN						  
	sprites->Add(8041, 693, 529, 673, 489, texEnemy);
	sprites->Add(8042, 759, 531, 739, 491, texEnemy);

	ani = new CAnimation(100);
	ani->Add(8001);
	animations->Add(ID_ANI_ENEMY_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(8002);
	animations->Add(ID_ANI_ENEMY_IDLE_LEFT, ani);

	ani = new CAnimation(150);
	ani->Add(8011);
	ani->Add(8012);
	ani->Add(8013);
	ani->Add(8014);
	ani->Add(8015);
	ani->Add(8016);
	animations->Add(ID_ANI_ENEMY_WALKING_RIGHT, ani);

	ani = new CAnimation(150);
	ani->Add(8021);
	ani->Add(8022);
	ani->Add(8023);
	ani->Add(8024);
	ani->Add(8025);
	ani->Add(8026);
	animations->Add(ID_ANI_ENEMY_WALKING_LEFT, ani);

	ani = new CAnimation(450);
	ani->Add(8031);
	ani->Add(8032);
	animations->Add(ID_ANI_ENEMY_WALKING_UP, ani);

	ani = new CAnimation(450);
	ani->Add(8041);
	ani->Add(8042);
	animations->Add(ID_ANI_ENEMY_WALKING_DOWN, ani);

	enemy = new CEnemy(100.0f, 150.0f);
	objects.push_back(enemy);







	LPTEXTURE texBill = textures->Get(ID_TEX_BILL);
	//IDLE RIGHT AND LEFT
	sprites->Add(9001, 546, 24, 569, 59, texBill);
	sprites->Add(9002, 569, 24, 546, 59, texBill);

	//BILL WALKING RIGHT
	sprites->Add(9011, 25, 24, 43, 59, texBill);
	sprites->Add(9012, 90, 25, 109, 58, texBill);
	sprites->Add(9013, 157, 24, 172, 59, texBill);
	sprites->Add(9014, 220, 24, 237, 59, texBill);
	sprites->Add(9015, 285, 25, 304, 58, texBill);
	sprites->Add(9016, 353, 24, 368, 59, texBill);

	//BILL WALKING LEFT
	sprites->Add(9021, 43, 24, 25, 59, texBill);
	sprites->Add(9022, 109, 25, 90, 58, texBill);
	sprites->Add(9023, 172, 24, 157, 59, texBill);
	sprites->Add(9024, 237, 24, 220, 59, texBill);
	sprites->Add(9025, 304, 25, 285, 58, texBill);
	sprites->Add(9026, 368, 24, 353, 59, texBill);

	//BILL WALKING UP
	sprites->Add(9031, 673, 221, 693, 261, texBill);
	sprites->Add(9032, 739, 223, 759, 263, texBill);

	//BILL WALKING DOWN
	sprites->Add(9041, 693, 261, 673, 221, texBill);
	sprites->Add(9042, 759, 263, 739, 223, texBill);

	ani = new CAnimation(100);
	ani->Add(9001);
	animations->Add(ID_ANI_BILL_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(9002);
	animations->Add(ID_ANI_BILL_IDLE_LEFT, ani);

	ani = new CAnimation(150);
	ani->Add(9011);
	ani->Add(9012);
	ani->Add(9013);
	ani->Add(9014);
	ani->Add(9015);
	ani->Add(9016);
	animations->Add(ID_ANI_BILL_WALKING_RIGHT, ani);

	ani = new CAnimation(150);
	ani->Add(9021);
	ani->Add(9022);
	ani->Add(9023);
	ani->Add(9024);
	ani->Add(9025);
	ani->Add(9026);
	animations->Add(ID_ANI_BILL_WALKING_LEFT, ani);

	ani = new CAnimation(450);
	ani->Add(9031);
	ani->Add(9032);
	animations->Add(ID_ANI_BILL_WALKING_UP, ani);

	ani = new CAnimation(450);
	ani->Add(9041);
	ani->Add(9042);
	animations->Add(ID_ANI_BILL_WALKING_DOWN, ani);

	bill = new CBill(MARIO_START_X, MARIO_START_Y);
	objects.push_back(bill);
	
	//LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	//sprites->Add(10001, 246, 154, 260, 181, texMario);

	//sprites->Add(10002, 275, 154, 290, 181, texMario);
	//sprites->Add(10003, 304, 154, 321, 181, texMario);

	//sprites->Add(10011, 186, 154, 200, 181, texMario);

	//sprites->Add(10012, 155, 154, 170, 181, texMario);
	//sprites->Add(10013, 125, 154, 140, 181, texMario);

	//// RUNNING RIGHT 
	//sprites->Add(10021, 335, 154, 335 + 18, 154 +26, texMario);
	//sprites->Add(10022, 363, 154, 363 + 18, 154 + 26, texMario);
	//sprites->Add(10023, 393, 154, 393 + 18, 154 + 26, texMario);

	//// RUNNING LEFT
	//sprites->Add(10031, 92, 154, 92 + 18, 154 + 26, texMario);
	//sprites->Add(10032, 66, 154, 66 + 18, 154 + 26, texMario);
	//sprites->Add(10033, 35, 154, 35 + 18, 154 + 26, texMario);

	//// JUMP WALK RIGHT & LEFT 
	//sprites->Add(10041, 395, 275, 395 + 16, 275 + 25, texMario);
	//sprites->Add(10042, 35, 275, 35 + 16, 275 + 25, texMario);

	//// JUMP RUN RIGHT & LEFT 
	//sprites->Add(10043, 395, 195, 395 + 18, 195 + 25, texMario);
	//sprites->Add(10044, 33, 195, 33 + 18, 195 + 25, texMario);

	//// SIT RIGHT/LEFT
	//sprites->Add(10051, 426, 239, 426 + 14, 239 + 17, texMario);
	//sprites->Add(10052, 5, 239, 5 + 14, 239 + 17, texMario);

	//// BRACING RIGHT/LEFT
	//sprites->Add(10061, 425, 154, 425 + 15, 154 + 27, texMario);
	//sprites->Add(10062, 5, 154, 5 + 15, 154 + 27, texMario);

	//

	//ani = new CAnimation(100);	
	//ani->Add(10001);
	//animations->Add(ID_ANI_MARIO_IDLE_RIGHT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10011);
	//animations->Add(ID_ANI_MARIO_IDLE_LEFT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10001);
	//ani->Add(10002);
	//ani->Add(10003);
	//animations->Add(ID_ANI_MARIO_WALKING_RIGHT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//animations->Add(ID_ANI_MARIO_WALKING_LEFT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10021);
	//ani->Add(10022);
	//ani->Add(10023);
	//animations->Add(ID_ANI_MARIO_RUNNING_RIGHT, ani);

	//ani = new CAnimation(50);	// Mario runs faster hence animation speed should be faster
	//ani->Add(10031);
	//ani->Add(10032);
	//ani->Add(10033);
	//animations->Add(ID_ANI_MARIO_RUNNING_LEFT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10041);
	//animations->Add(ID_ANI_MARIO_JUMP_WALK_RIGHT, ani);

	//ani = new CAnimation(100);	
	//ani->Add(10042);
	//animations->Add(ID_ANI_MARIO_JUMP_WALK_LEFT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10043);
	//animations->Add(ID_ANI_MARIO_JUMP_RUN_RIGHT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10044);
	//animations->Add(ID_ANI_MARIO_JUMP_RUN_LEFT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10051);
	//animations->Add(ID_ANI_MARIO_SIT_RIGHT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10052);
	//animations->Add(ID_ANI_MARIO_SIT_LEFT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10061);
	//animations->Add(ID_ANI_MARIO_BRACE_RIGHT, ani);

	//ani = new CAnimation(100);
	//ani->Add(10062);
	//animations->Add(ID_ANI_MARIO_BRACE_LEFT, ani);

	mario = new CMario(MARIO_START_X, MARIO_START_Y);
	//objects.push_back(mario);

	// Brick objects 
	/*LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_BRICK, 372, 153, 372+15, 153+15, texMisc);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BRICK);
	animations->Add(ID_ANI_BRICK,ani);

	for (int i=0;i<NUM_BRICKS;i++) 
	{
		CBrick* b = new CBrick(BRICK_X + i * BRICK_WIDTH, BRICK_Y);
		objects.push_back(b);
	}*/
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Update(dt);
	}
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), BACKGROUND_COLOR, 0xffffffff);

	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Render();
	}

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			CGame::GetInstance()->ProcessKeyboard();
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	keyHandler = new CSampleKeyHandler();
	game->InitKeyboard(keyHandler);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	LoadResources();

	Run();

	return 0;
}