#include "Bill.h"

void CBill::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	if (vx > 0 && x > 300) x = 300;
	if (vx < 0 && x < 5) x = 5;
}

void CBill::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (aniId == -1) aniId = ID_ANI_BILL_IDLE_RIGHT;
	switch (state) {
	case BILL_STATE_WALKING_RIGHT:
		aniId = ID_ANI_BILL_WALKING_RIGHT;
		break;
	case BILL_STATE_WALKING_LEFT:
		aniId = ID_ANI_BILL_WALKING_LEFT;
		break;
	case BILL_STATE_WALKING_DOWN:
		aniId = ID_ANI_BILL_WALKING_DOWN;
		break;
	case BILL_STATE_WALKING_UP:
		aniId = ID_ANI_BILL_WALKING_UP;
		break;
	case BILL_STATE_IDLE:
		if (nx < 0) {
			aniId = ID_ANI_BILL_IDLE_LEFT;
		}
		else
		{
			aniId = ID_ANI_BILL_IDLE_RIGHT;
		}
		break;
	}
	animations->Get(aniId)->Render(x, y);
}

void CBill::SetState(int state)
{
	switch (state)
	{
	case BILL_STATE_WALKING_RIGHT:
		vx = BILL_WALKING_SPEED;
		vy = 0;
		nx = 1;
		break;
	case BILL_STATE_WALKING_LEFT:
		vx = -BILL_WALKING_SPEED;
		vy = 0;
		nx = -1;
		break;
	case BILL_STATE_WALKING_DOWN:
		vy = BILL_WALKING_SPEED;
		vx = 0;
		break;
	case BILL_STATE_WALKING_UP:
		vy = -BILL_WALKING_SPEED;
		vx = 0;
		break;
	case BILL_STATE_IDLE:
		vx = 0.0f;
		vy = 0.0f;
		break;
	}

	CGameObject::SetState(state);
}