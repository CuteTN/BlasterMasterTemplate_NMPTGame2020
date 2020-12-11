#pragma once
#include "Enemy.h"

// bounding box
// 8 0 23 15

const int FLOATER_BOUNDBOX_WIDTH = 15;
const int FLOATER_BOUNDBOX_HEIGHT = 15;
const int FLOATER_BOUNDBOX_OFFSETX = 8;
const int FLOATER_BOUNDBOX_OFFSETY = 0;

const float FLOATER_FLY_SPEED = 0.06f;
const float FLOATER_BULLET_SPEED = 0.08f;



class CFloater : public CEnemy
{
private:
    void UpdateState();
public:
    CFloater() {};
    CFloater(int classId, int x, int y, int sectionId, int animsId);
    
    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CFloater* LPFLOATER;
