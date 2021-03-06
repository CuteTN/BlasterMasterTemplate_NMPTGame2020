#include "Bullet_Teleporter.h"

#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "Sound.h"

CBullet_Teleporter::CBullet_Teleporter(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_ENEMY_OVERHEAD_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_TELEPORTER_SPEED;
	vy = ny * BULLET_TELEPORTER_SPEED;

	isHiddenByForeground = true;
	zIndex = ZINDEX_OVERHEAD_ENEMY_BULLET;
}

void CBullet_Teleporter::UpdateVelocity(DWORD dt)
{
}

void CBullet_Teleporter::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		Sound::getInstance()->play(SMALL_PINK_BULLET_EXPLOSION, false, 1);
		CGameObjectBehaviour::BlockObject(dt, coEvent);
		CGameObjectBehaviour::RemoveObject(this);
	}

	// disappear when hitting player or player bullet
	if (
		obj == CGame::GetInstance()->GetCurrentPlayer() ||
		(dynamic_cast<CBullet*>(obj) && dynamic_cast<CBullet*>(obj)->isFriendly)
		)
	{
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CBullet_Teleporter::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// disappear when hitting player or player bullet
	if (
		overlappedObj == CGame::GetInstance()->GetCurrentPlayer() ||
		(dynamic_cast<CBullet*>(overlappedObj) && dynamic_cast<CBullet*>(overlappedObj)->isFriendly)
		)
	{
		CGameObjectBehaviour::RemoveObject(this);
	}
}
