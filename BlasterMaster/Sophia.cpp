#include "Sophia.h"
#include "TileArea.h"

CSophia::CSophia(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId)
{
	SetState(SOPHIA_STATE_WALK_RIGHT);
	vyMax = 100;
	vxMax = 10;
};


#pragma region key events handling

void CSophia::HandleKeys(DWORD dt)
{
	HandleKeysHold(dt);

	auto keyEvents = NewKeyEvents();
	for (auto e : keyEvents)
	{
		int keyCode = e->GetKeyCode();
		if (e->IsDown())
			HandleKeyDown(dt, keyCode);
		else
			HandleKeyUp(dt, keyCode);
	}
}

void CSophia::HandleKeysHold(DWORD dt)
{
	if (IsKeyDown(DIK_RIGHT))
	{
		SetState(SOPHIA_STATE_WALK_RIGHT);
		vx = 0.1;
	}
	else if (IsKeyDown(DIK_LEFT))
	{
		SetState(SOPHIA_STATE_WALK_LEFT);
		vx = -0.1;
	}
	if (IsKeyDown(DIK_UP))
	{
		vy = -0.1;
	}
	if (IsKeyDown(DIK_DOWN))
	{
		vy = 0.1;
	}
}
void CSophia::HandleKeyUp(DWORD dt, int keyCode)
{
	if (keyCode == DIK_RIGHT || keyCode == DIK_LEFT)
		vx = 0;

	if (keyCode == DIK_UP || keyCode == DIK_DOWN)
		vy = 0;
}
void CSophia::HandleKeyDown(DWORD dt, int keyCode)
{
	if (!flagOnAir && keyCode == DIK_X)
	{
		ay = -0.015;
		flagOnAir = true;
	}
}

void CSophia::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (dynamic_cast<LPTILE_AREA>(obj))
	{
		LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);

		switch (tileArea->GetClass())
		{
		case CLASS_TILE_BLOCKABLE:
		{
			// on top of a blockable tile
			if (coEvent->ny < 0)
			{
				flagOnAir = false;
			}
			break;
		}
		}
	}
}

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CGameObject::Update(dt, coObjs);
	HandleKeys(dt);

	// Simple fall down
	vy += SOPHIA_GRAVITY; 
	//SolveClassicalMechanics();
	float dx, dy;
	GetPositionDifference(dx, dy);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if(!flagDead)
		CalcPotentialCollisions(coObjs, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		//x += dx;
		//y += dy;
		SolveClassicalMechanics();
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		// block every object first!
		// CuteTN Note: wth is 0.4f??? WHAT IS IT?
		x += min_tx * dx+ nx *0.4f;
		y += min_ty * dy+ ny *0.4f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// collision logic
		for (LPCOLLISIONEVENT coEvent : coEvents)
			HandleCollision(dt, coEvent);

	}

}

void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SOPHIA_BOUNDBOX_OFFSETX;
	top = y + SOPHIA_BOUNDBOX_OFFSETY;
	right = left + SOPHIA_BOUNDBOX_WIDTH;
	bottom = top + SOPHIA_BOUNDBOX_HEIGHT;
}
