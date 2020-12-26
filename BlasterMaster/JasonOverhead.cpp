#include "JasonOverhead.h"
#include "TileArea.h"
#include "Portal.h"
#include "GameObjectBehaviour.h"
#include "PortalLib.h"
#include "GameGlobal.h"
#include "WalkInPortalEvent.h"

CJasonOverhead* CJasonOverhead::__instance = nullptr;

CJasonOverhead::CJasonOverhead()
{
    Init();
}

void CJasonOverhead::Init()
{
    classId = CLASS_JASONOVERHEAD;
    SetState(JASONOVERHEAD_STATE_IDLE_DOWN);

    // CuteTN Note: DIRTY AF
    LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(JASON_OVERHEAD_ANIMATIONS);
    animationHandlers = objAnims->GenerateAnimationHanlders();

    this->allowOverlapWithBlocks = true;

    //invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION, 1);
    //invulnerableTimer->Stop();

    //dyingEffectTimer = new CTimer(this, DYING_EFFECT_DURATION, 1);
    //dyingEffectTimer->Stop();

    //flagInvulnerable = false;

    //vulnerableFlashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, JASONSIDEVIEW_VULNERABLE_EFFECT_FLASHING_DURATION);
}


void CJasonOverhead::HandleKeys(DWORD dt)
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

void CJasonOverhead::HandleKeyUp(DWORD dt, int keyCode)
{
}

void CJasonOverhead::HandleKeyDown(DWORD dt, int keyCode)
{
}

void CJasonOverhead::HandleKeysHold(DWORD dt)
{
    MovingDirX = MovingDirY = 0;

    if (IsKeyDown(DIK_UP))
        MovingDirY--;
    if (IsKeyDown(DIK_DOWN))
        MovingDirY++;
    if (IsKeyDown(DIK_LEFT))
        MovingDirX--;
    if (IsKeyDown(DIK_RIGHT))
        MovingDirX++;
}

bool CJasonOverhead::IsMoving()
{
    return (MovingDirX != 0) || (MovingDirY != 0);
}

void CJasonOverhead::UpdateState()
{
    int newState = state;

    if (IsMoving())
    {
        if (MovingDirX != 0 && MovingDirY != 0)
            ;
        else
        {
            if (MovingDirX < 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_LEFT;
                FaceDir = JASON_OVERHEAD_FACEDIR_LEFT;
            }
            else if (MovingDirX > 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_RIGHT;
                FaceDir = JASON_OVERHEAD_FACEDIR_RIGHT;
            }
            else if (MovingDirY < 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_UP;
                FaceDir = JASON_OVERHEAD_FACEDIR_UP;
            }
            else if (MovingDirY > 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_DOWN;
                FaceDir = JASON_OVERHEAD_FACEDIR_DOWN;
            }
        }
    }
    else
    {
        switch (FaceDir)
        {
        case JASON_OVERHEAD_FACEDIR_UP: newState = JASONOVERHEAD_STATE_IDLE_UP; break;
        case JASON_OVERHEAD_FACEDIR_DOWN: newState = JASONOVERHEAD_STATE_IDLE_DOWN; break;
        case JASON_OVERHEAD_FACEDIR_LEFT: newState = JASONOVERHEAD_STATE_IDLE_LEFT; break;
        case JASON_OVERHEAD_FACEDIR_RIGHT: newState = JASONOVERHEAD_STATE_IDLE_RIGHT; break;
        }
    }

    SetState(newState);
}

CJasonOverhead* CJasonOverhead::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CJasonOverhead();
    }

    return __instance;
}

CJasonOverhead* CJasonOverhead::InitInstance(int x, int y, int sectionId)
{
    GetInstance();
    __instance->SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    __instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;

    return __instance;
}

void CJasonOverhead::UpdateVelocity(DWORD dt)
{
    vx = MovingDirX * JASON_OVERHEAD_MOVING_SPEED_X;
    vy = MovingDirY * JASON_OVERHEAD_MOVING_SPEED_Y;
}

void CJasonOverhead::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
        {
			CGameObjectBehaviour::BlockObject(dt, coEvent);
            break;
        }

        case CLASS_TILE_PORTAL:
        {
            LPPORTAL fromPortal = dynamic_cast<LPPORTAL>(obj);
            LPPORTAL toPortal = CPortalLib::GetInstance()->Get(fromPortal->associatedPortalId);

            // Sanh code from here!
            LPGAME_EVENT newEvent = new CWalkInPortalEvent("WalkInPortalEvent", fromPortal, toPortal);
            CGame::GetInstance()->AddGameEvent(newEvent);
            // to do: create an event to CGame, let CGame handle switching section
            DebugOut(L"Jason to portal %d of section %d, tick %d\n", toPortal->associatedPortalId, toPortal->currentSectionId, GetTickCount());

            break;
        }


        }
    }

    if (dynamic_cast<CEnemy*>(obj))
    {
        CGameGlobal::GetInstance()->beingAttackedByEnemy();
    }

    if (dynamic_cast<CBullet*>(obj))
    {
        CBullet* bullet = dynamic_cast<CBullet*>(obj);
        if (!bullet->isFriendly)
            CGameGlobal::GetInstance()->beingAttackedByEnemy();
    }
}

void CJasonOverhead::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CJasonOverhead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
    HandleKeys(dt);
    CAnimatableObject::Update(dt, coObjs);
    UpdateState();

    //SANH
    GameState _gameState = CGame::GetInstance()->GetState();
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_DOWN)
        SetSpeed(0, 0.1);
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_LEFT)
        SetSpeed(-0.1, 0);
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_RIGHT)
        SetSpeed(0.1, 0);
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_UP)
        SetSpeed(0, -0.1);      
}

void CJasonOverhead::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + JASONOVERHEAD_BOUNDBOX_OFFSETX;
    top = y + JASONOVERHEAD_BOUNDBOX_OFFSETY;
    right = left + JASONOVERHEAD_BOUNDBOX_WIDTH;
    bottom = top + JASONOVERHEAD_BOUNDBOX_HEIGHT;
}
