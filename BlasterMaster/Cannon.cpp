#include "Cannon.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Bullet_Cannon.h"

void CCannon::UpdateState()
{

}

CCannon::CCannon(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = CANNON_HEALTHPOINT;

	UpdateState();
	flagVertical = true;
	flagHorizontal = true;

};

void CCannon::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
	}
}


void CCannon::ShootHorizontal()
{
	CBullet_Cannon* bullet1 = new CBullet_Cannon(0, 0, 0, 1, 0);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet1, this);

	CBullet_Cannon* bullet2 = new CBullet_Cannon(0, 0, 0, -1, 0);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet2, this);
}

void CCannon::ShootVertical()
{
	CBullet_Cannon* bullet1 = new CBullet_Cannon(0, 0, 0, 0, 1);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet1, this);
	CBullet_Cannon* bullet2 = new CBullet_Cannon(0, 0, 0, 0, -1);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet2, this);
}

void CCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (animationHandlers[state]->currentFrameIndex == 2) {
		flagVertical = true;
		if (flagHorizontal) {
			ShootHorizontal();
			flagHorizontal = false;
		}
	}
	if (animationHandlers[state]->currentFrameIndex == 0) {
		flagHorizontal = true;
		if (flagVertical) {
			ShootVertical();
			flagVertical = false;
		}
	}
	CEnemy::Update(dt, coObjs);
}

void CCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + CANNON_BOUNDBOX_OFFSETX;
	right = left + CANNON_BOUNDBOX_WIDTH;
	top = y + CANNON_BOUNDBOX_OFFSETY;
	bottom = top + CANNON_BOUNDBOX_HEIGHT;
}

void CCannon::HandleTimerTick(LPTIMER sender)
{

}