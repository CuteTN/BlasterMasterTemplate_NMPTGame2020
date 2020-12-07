#include "Floater.h"

CFloater::CFloater(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId) 
{
	SetState(FLOATER_STATE_FLY_LEFT);
};

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + FLOATER_BOUNDBOX_OFFSETX;
	right = left + FLOATER_BOUNDBOX_WIDTH;
	top = y + FLOATER_BOUNDBOX_OFFSETY;
	bottom = top + FLOATER_BOUNDBOX_HEIGHT;
}
