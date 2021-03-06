#pragma once

#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "Textures.h"
#include "Game.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CGameObject
class CGameObject
{
protected:
	/// CuteTN Note: using both dx and vx could be confusing... So I removed it :^)

	/// <summary>
	/// <para> position of GameObject relative to game world (NOT the camera) </para> 
	/// </summary>
	float x, y;

	/// <summary>
	/// velocity of this gameObject
	/// </summary>
	float vx, vy;
	float vxMax, vyMax;

	/// <summary>
	/// acceleration of this gameObject
	/// </summary>
	float ax, ay;

	int state = 0;

public: 
	int currentSectionId = 0;
	int classId = -1;
	int objectId = -1;
	int zIndex = ZINDEX_DEFAULT;
	bool isHiddenByForeground = true;
	bool isUpdatedWhenOffScreen = false;

	CGameObject();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	
	void SetAcceleration(float ax, float ay) { this->ax = ax, this->ay = ay; }
	void GetAcceleration(float &ax, float &ay) { ax = this->ax; ay = this->ay; }
	
	virtual void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	void RenderBoundingBox(float offsetX, float offsetY);

	/// <summary>
	/// <para> Getting the actual hitbox (collision box) of a game object. Relative to gameworld, not to the sprite itself. </para>
	/// <para> Should be implemented with O(1) complexity so that we can call it everytime needed without worrying about the performance (otherwise consider using some properties as cache instead?) </para>
	/// <para> WARNING 1: be aware of game object state and direction </para>
	/// <para> WARNING 2: please define a constant whenever you need it, don't spam cancerous numbers into this function! </para>
	/// </summary>
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;

	/// <summary>
	/// <para> Update function is called at most once per frame (conventionally) </para>
	/// <para> Updates this object internal state based on its model (path, physics), interaction with other objects and user input </para>
	/// <para> </para>
	/// </summary>
	/// <param name="dt:">Time elapsed from last frame. be acknowledged that this is NOT A CONSTANT, it's based on real-time</param>
	/// <param name="coObjects:">Collision objects. I've not yet known how can this parameter help, but let us keep it that way...</param>
	virtual void Update(DWORD dt, vector<CGameObject*>* coObjects) = 0;

	/// <summary>
	/// <para> Render function is called at most once per frame (conventionally) </para>
	/// <para> Render this game object to user screen </para>
	/// <para> WARNING 1: this could cover other game objects and causes visual bugs. The order of rendering is important, too </para>
	/// </summary>
	virtual void Render(float offsetX, float offsetY) = 0;

	~CGameObject();
};

typedef CGameObject * LPGAMEOBJECT;
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CCollisionEvent
/// <summary>
/// <para> Stores infomation of a collision event from ONE game object to ANOTHER </para>
/// <para> Based on Swept AABB algorithm </para>
/// </summary>
struct CCollisionEvent
{
	LPGAMEOBJECT sourceObject;
	LPGAMEOBJECT otherObject;

	/// <summary>
	/// <para> Predicted time until colision based on swept AABB Algoithm </para>
	/// <para> Collision on next frame only when: 0&lt;timeEntry&lt;1 </para>
	/// </summary>
	float timeEntry;

	/// <summary>
	/// <para> A small distance to move in order not to collide to otherObject </para> 
	/// <para> Backward of (dx,dy) </para>
	/// <para> Value can only be either -1, 0 or 1 </para>
	/// </summary>
	float nx, ny;

	/// <summary>
	/// <para> Relative movement from sourceObject to otherObject perspective (i.e otherObject is static) </para>
	/// </summary>
	float rdx, rdy;

	CCollisionEvent(LPGAMEOBJECT sourceObject, LPGAMEOBJECT otherObject, float timeEntry, float rdx, float rdy, float nx, float ny)
	{
		this->sourceObject = sourceObject;
		this->otherObject = otherObject;
		this->timeEntry = timeEntry;
		this->rdx = rdx;
		this->rdy = rdy;
		this->nx = nx;
		this->ny = ny;
	}

	/// <summary>
	/// <para> One doesn't have to go to this function definition just to know it compares 2 entryTime, do they? </para>
	/// </summary>
	/// <returns>returns TRUE iff a.timeEntry&lt;b.timeEntry</returns>
	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->timeEntry < b->timeEntry;
	}
};
#pragma endregion
