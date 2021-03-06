#include "Explosion.h"
#include "RemoveObjectEvent.h"
#include "GameObjectBehaviour.h"

#pragma region init

CExplosion::CExplosion(int classId, int x, int y, int sectionId, int animsId) : CAnimatableObject(classId, x, y, sectionId, animsId)
{
}

CExplosion::CExplosion(int classId, int x, int y, int sectionId) : CAnimatableObject(classId, x, y, sectionId)
{
	ax = ay = vx = vy = 0;

	// only hidden by foreground when the explosion is in sideview
	// CuteTN Note: Dirtyyyyyyyyyyyyyyyyyyyyyyyy
	if (classId == CLASS_SMALL_EXPLOSION_SIDEVIEW || classId == CLASS_LARGE_EXPLOSION_SIDEVIEW || classId == CLASS_SOPHIA_EXPLOSION)
		this->isHiddenByForeground = false;
	else
		this->isHiddenByForeground = true;

	this->isUpdatedWhenOffScreen = true;

	zIndex = ZINDEX_EXPLOSION;
}

#pragma endregion

void CExplosion::UpdateVelocity(DWORD dt)
{
}

void CExplosion::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CExplosion::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CExplosion::ResolveInteractions(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	vector<LPCOLLISIONEVENT>* colEvents = new vector<LPCOLLISIONEVENT>();

	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisions(dt, colEvents);

	vector<LPGAMEOBJECT>* overlappedObjs = new vector<LPGAMEOBJECT>();
	CheckOverlaps(coObjs, *overlappedObjs);
	HandleOverlaps(overlappedObjs);
}

void CExplosion::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + EXPLOSION_BOUNDBOX_OFFSETX;
	right = left + EXPLOSION_BOUNDBOX_WIDTH;
	top = y + EXPLOSION_BOUNDBOX_OFFSETY;
	bottom = top + EXPLOSION_BOUNDBOX_HEIGHT;
}

void CExplosion::Render(float offsetX, float offsetY)
{
	animationHandlers[state]->Render(x + offsetX, y + offsetY, 255, modifyR, modifyG, modifyB);
	animationHandlers[state]->Update();
}

void CExplosion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	// auto dispose after done its animation
	if (animationHandlers[state]->animation->GetNumberOfFrames() - 1 == animationHandlers[state]->currentFrameIndex)
	{
		CGameObjectBehaviour::RemoveObject(this);
	}
}
