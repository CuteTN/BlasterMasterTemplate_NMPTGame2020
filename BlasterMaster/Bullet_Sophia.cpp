#include "Bullet_Sophia.h"
#include "GameObjectBehaviour.h"

#include "TileArea.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Explosion_SmallSideview.h"

CBullet_Sophia::CBullet_Sophia(int x, int y, int sectionId, int dirX, int dirY) : CBullet(x, y, sectionId, BULLET_SOPHIA_SIDEVIEW_ANIMATIONS, true)
{
	// normalize the direction vector
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_SOPHIA_SPEED;
	vy = ny * BULLET_SOPHIA_SPEED;

	if (vx < 0)
		SetState(BULLET_SOPHIA_SIDEVIEW_STATE_LEFT);
	else if (vx > 0)
		SetState(BULLET_SOPHIA_SIDEVIEW_STATE_RIGHT);
	else
		SetState(BULLET_SOPHIA_SIDEVIEW_STATE_UP);
}

void CBullet_Sophia::UpdateVelocity(DWORD dt)
{
	// nothing
}

void CBullet_Sophia::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			Explode(new CExplosion_SmallSideview());
			break;
		}
		}
	}
}

void CBullet_Sophia::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// CuteTN Todo: overlap with enemies?
	// nothing YET
}

void CBullet_Sophia::CalcExplosionCenterPos(float& explosionX, float& explosionY)
{
	float l, r, t, b;
	GetBoundingBox(l, t, r, b);
	CGameObjectBehaviour::CalcBoundingBoxCenter(this, explosionX, explosionY);

	switch (state)
	{
	case BULLET_SOPHIA_SIDEVIEW_STATE_UP:
		explosionY = t;
		break;
	
	case BULLET_SOPHIA_SIDEVIEW_STATE_LEFT:
		explosionX = l;
		break;

	case BULLET_SOPHIA_SIDEVIEW_STATE_RIGHT:
		explosionX = r;
		break;
	}


}

void CBullet_Sophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != BULLET_SOPHIA_SIDEVIEW_STATE_UP)
	{
		left = x + BULLET_SOPHIA_BOUNDBOX_OFFSETX;
		right = left + BULLET_SOPHIA_BOUNDBOX_WIDTH;
		top = y + BULLET_SOPHIA_BOUNDBOX_OFFSETY;
		bottom = top + BULLET_SOPHIA_BOUNDBOX_HEIGHT;
	}
	else
	{
		// flip diagonally
		left = x + BULLET_SOPHIA_BOUNDBOX_OFFSETY;
		right = left + BULLET_SOPHIA_BOUNDBOX_HEIGHT;
		top = y + BULLET_SOPHIA_BOUNDBOX_OFFSETX;
		bottom = top + BULLET_SOPHIA_BOUNDBOX_WIDTH;
	}
}
