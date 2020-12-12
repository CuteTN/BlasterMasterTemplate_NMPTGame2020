#include "Orb.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include <stdlib.h>    
#include <time.h>  

void COrb::UpdateState()
{
	if (!suicide) {
		if (flagTurn > 0) {
			SetState(ORB_TURN_RIGHT);
			flagTurn--;
		}
		else  if (flagTurn < -0) {
			SetState(ORB_TURN_LEFT);
			flagTurn++;
		}
	else
		if (vx < 0) {
			SetState(ORB_FLY_LEFT);
		}
		else if (vx > 0) {
			SetState(ORB_FLY_RIGHT);
		}
		else if (vy != 0) {
			if (this->state == ORB_FLY_LEFT)
				SetState(ORB_TURN_AROUND_LEFT);
			if (this->state == ORB_FLY_RIGHT)
				SetState(ORB_TURN_AROUND_RIGHT);
		}
	}
	else {
		if (vx < 0) {
			SetState(ORB_TURN_AROUND_LEFT);
		}
		else {
			SetState(ORB_TURN_AROUND_RIGHT);
		}
	}
}

COrb::COrb(int classId, int x, int y, int sectionId, int initLeft, int animsId, bool behaviorId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	suicide = behaviorId;
	if (!suicide) {
		flagTurn = 0;
		vx = ORB_MOVE_SPEED;
		vy = 0;
		yStart = 0;
		if (initLeft) {
			vx = -vx;
		}
		UpdateState();
	}
	else {
		vx = 0;
		vy = 0;
		UpdateState();
	}
}

void COrb::UpdateVelocity(DWORD dt)
{
	if (!suicide) {
		if (GetTickCount() - lastTimeVertical >= VERTICAL_PERIOD) {
			int up = rand() % 2;
			if (up) {
				vy = -ORB_MOVE_SPEED / 2;
				yStart = y;
				vx = 0;
			}
			else {
				vy = ORB_MOVE_SPEED / 2;
				yStart = y;
				vx = 0;
			}
			lastTimeVertical = GetTickCount();
		}
		int halfStep = rand() % 4;
		if (halfStep) {
			if (abs(yStart - y) >= 8) {
				vy = 0;
			}
		}
		else
			if (abs(yStart - y) >= 16) {
				vy = 0;
			}
		if (vy == 0) {
			if (this->state == ORB_TURN_AROUND_LEFT)
				vx = -ORB_MOVE_SPEED;
			if (this->state == ORB_TURN_AROUND_RIGHT)
				vx = ORB_MOVE_SPEED;
		}
		UpdateState();
	}
	else {
		float Xplayer, Yplayer;
		CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);
		if (x > Xplayer+12)
			vx = -ORB_CHASE_SPEED;
		else
			vx = ORB_CHASE_SPEED;
		if (y > Yplayer+20)
			vy = -ORB_CHASE_SPEED;
		else
			vy = ORB_CHASE_SPEED;
		UpdateState();
	}
}

void COrb::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
		if (coEvent == nullptr)
			return;
		if (coEvent->otherObject == this)
			return;

		LPGAMEOBJECT obj = coEvent->otherObject;

		if (dynamic_cast<LPTILE_AREA>(obj))
		{
			LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);

			switch (tileArea->classId)
			{
			case CLASS_TILE_BLOCKABLE:
			case CLASS_TILE_PORTAL:
			{
				CGameObjectBehaviour::BlockObject(dt, coEvent);

				if (!suicide) {
					if (coEvent->nx != 0)
					{
						flagTurn = coEvent->nx*10;
						vx = coEvent->nx * ORB_MOVE_SPEED;
						UpdateState();
					}
				}

				break;
			}
			}
		}
}

void COrb::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// Chibi cute
}

void COrb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ORB_BOUNDBOX_OFFSETX;
	right = left + ORB_BOUNDBOX_WIDTH;
	top = y + ORB_BOUNDBOX_OFFSETY;
	bottom = top + ORB_BOUNDBOX_HEIGHT;
}



