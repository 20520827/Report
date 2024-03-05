#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define BILL_WALKING_SPEED		0.1f

#define BILL_STATE_IDLE				0
#define BILL_STATE_WALKING_RIGHT	100
#define BILL_STATE_WALKING_LEFT		200
#define BILL_STATE_WALKING_UP		300
#define BILL_STATE_WALKING_DOWN		400


#pragma region ANIMATION_ID

#define ID_ANI_BILL_IDLE_RIGHT 500
#define ID_ANI_BILL_IDLE_LEFT 501

#define ID_ANI_BILL_WALKING_RIGHT 600
#define ID_ANI_BILL_WALKING_LEFT 601
#define ID_ANI_BILL_WALKING_UP 602
#define ID_ANI_BILL_WALKING_DOWN 603

class CBill : public CGameObject
{
public:
	CBill(float x, float y) : CGameObject(x, y)
	{
		
	}
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};