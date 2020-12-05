#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"

void CGameObjectBehaviour::BlockObject(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	LPGAMEOBJECT movableObj = coEvent->sourceObject;
	LPGAMEOBJECT blockableObj = coEvent->otherObject;

	float x, y, vx, vy;
	movableObj->GetPosition(x, y);
	movableObj->GetSpeed(vx, vy);
	
	// move the object to the collision position, then set the velocity to 0
	if (coEvent->nx != 0)
	{
		float dx = coEvent->rdx * coEvent->timeEntry / dt;
		x += dx;
		vx = 0;
	}

	if (coEvent->ny != 0)
	{
		float dy = coEvent->rdy * coEvent->timeEntry / dt;
		y += dy;
		vy = 0;
	}

	movableObj->SetPosition(x, y);
	movableObj->SetSpeed(vx, vy);

	// direction to deoverlap
	float doX, doY;
	CCollisionSolver::DeOverlap(movableObj, blockableObj, doX, doY);

	movableObj->SetPosition(x + doX, y + doY);
}

void CGameObjectBehaviour::CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y)
{
	if (obj == nullptr)
	{
		x = y = 0;
		return;
	}

	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	x = (l + r) / 2;
	y = (t + b) / 2;
}

void CGameObjectBehaviour::SetBoundingBoxPos(LPGAMEOBJECT obj, float x, float y)
{
	float l, t, r, b, oX, oY;
	obj->GetBoundingBox(l, t, r, b);
	obj->GetPosition(oX, oY);

	float offsetX = l - oX;
	float offsetY = t - oY;

	obj->SetPosition(x - offsetX, y - offsetY);
}

void CGameObjectBehaviour::SetBoundingBoxCenter(LPGAMEOBJECT obj, float x, float y)
{
	float oX, oY, cX, cY;
	obj->GetPosition(oX, oY);
	CalcBoundingBoxCenter(obj, cX, cY);

	float offsetX = cX - oX;
	float offsetY = cY - oY;

	obj->SetPosition(x - offsetX, y - offsetY);
}

void CGameObjectBehaviour::CalcDirecttionToPlayer(LPGAMEOBJECT obj, float& x, float& y)
{
	if (obj == nullptr)
		return;
	float xO, yO;
	CalcBoundingBoxCenter(obj, xO, yO);

	LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentPlayer();
	
	if (player == nullptr)
		return;
	float xP, yP;
	CalcBoundingBoxCenter(player, xP, yP);

	x = (xP - xO);
	y = (yP - yO);
}

float CGameObjectBehaviour::CalcMagnitudeVector2(float x, float y)
{
	return sqrt(x * x + y * y);
}

void CGameObjectBehaviour::NormalizeVector2(float x, float y, float& nx, float& ny)
{
	float d = CalcMagnitudeVector2(x, y);
	nx = x / d;
	ny = y / d;
}
