#pragma once

#include "Enemy.h"
#include "Item.h"

#define MAX_HEALTH_SOPHIA 80
#define MAX_HEALTH_JASONSIDEVIEW 80
#define MAX_HEALTH_JASONOVERHEAD 80
#define MAX_GUN_LEVEL 8
#define BODY_DAMAGE_ENEMY 5 * 1
#define BODY_DAMAGE_BULLET 5 * 1
#define BODY_DAMAGE_LAVA 20 * 1
#define BODY_DAMAGE_SPIKE 15 * 1
#define BODY_DAMAGE_FALL 80

#define MAX_AMMUNITIONS 99

enum class Items {
	PowerGrey, // add 1 bar
	PowerFlashingRed, // add 4 bars
	HoverGrey, 
	HoverFlashingRed,
	GunGrey,
	GunFlashingRed,
	HomingMissile, // item weapon
	MultiwarheadMissile,// item weapon
	ThunderBreak,// item weapon
	HyperBeam, //item to destroy the guardian that stands between you and Area 2.
};

enum class TypeWeapons
{
	HomingMissile,
	MultiwarheadMissile,
	ThunderBreak,
};

class CGameGlobal : public ITimeTrackable
{
private:
	CGameGlobal();
	static CGameGlobal * _instance;
	const int BOSS_EFFECT_DURATION = 2000;
	const int BOSS_EFFECT_FADE_IN_DURATION = 500;
	const int BOSS_EFFECT_RENDER_FLASHING = 200;
	const int BOSS_EFFECT_RENDER = 1000;
	const int BOSS_EFFECT_DEAD = 2000;
	const int BOSS_EFFECT_CRUSHER_DELAY = 2000;

	int times_render_boss = 0;
	//Save Game
	bool Saved = false;
	int left = 3;
	int flagPlayer = 1; // 1: Sophia 2:Jason SideView 3:Jason OverHead
	float playerX = 0, playerY = 0;
	float sophiaX = 0, sophiaY = 0;
	float jasonX = 0, jasonY = 0;
	int IDSectionSophia = 0;
	int IDSectionJason = 0;
	int IDCurrentSection = -1;
	LPTIMER effectBossFlashingTimer;
	LPTIMER effectBossFadeInTimer;
	LPTIMER effectBossRenderFlashing;
	LPTIMER effectBossRender;
	LPTIMER effectBossBossDead;
	LPTIMER effectBossCrusherDelay;

	//AnimationHandler Render 
	CObjectAnimationHanlders HealthPow;
	CObjectAnimationHanlders HealthOverhead;
	CObjectAnimationHanlders Hov;
	CObjectAnimationHanlders Gun;
	CObjectAnimationHanlders WeaponMenu;
	CObjectAnimationHanlders Numbers;
	CObjectAnimationHanlders SelectedLeft;
	CObjectAnimationHanlders SelectedRight;
	CObjectAnimationHanlders EffectFaded;
	CObjectAnimationHanlders EffectFadedIn;
	CObjectAnimationHanlders EffectFadedOut;

	//health player 
	int healthSophia;
	int healthJason;

	//Gun Power
	int levelGunPower = 4; // [0..8]
	
	//Weapons
	bool isWeaponMenuActive = false;
	int idSelectedItem = 1;
	int ammunitions_HomingMissile = MAX_AMMUNITIONS;
	int ammunitions_MultiwarheadMissile = MAX_AMMUNITIONS;
	int ammunitions_ThunderBreak = MAX_AMMUNITIONS;

	//Input for Render Pow & Gun
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);

	//Support funtion, U shouldn't care it 
	void SupportRenderHeath(int health);
	int SupportGetIDNumber(int number);
	void SupportAnalysNumber(int number, int& first, int & second);

public:
	//reset health
	//get value methods
	int ID_SECTION_BOSSOVERHEAD = -1;
	bool stateBossBlackBackground = false;
	bool isDeadBoss = false;
	static CGameGlobal* GetInstance();
	int get_healthSophia() { return this->healthSophia; }
	int get_healthJasonSideView() { return this->healthJason; }
	int get_healthJasonOverHead() { return this->healthJason; }

	//event method
	void initEffectFaded();
	void getItem(Items item);
	void beingAttackedByEnemy();
	void beingAttackedByBullet();
	void beingAttackedByLava();
	void beingAttackedBySpike();
	void beingAttackedByDrop();
	void beingAttackedByLowFall();
	void jasonJumpIntoSophia();
	int GetLevelGun() { return this->levelGunPower; };

	void AddToSophiaHealth(int amount);
	void AddToJasonHealth(int amount);
	void AddToGunLevel(int amount);

	//Update
	void Update(DWORD dt);
	void UpdateEffect(DWORD dt);

	//Render
	void RenderHealth();
	void RenderHealthGun();
	void RenderWeapon();
	void RenderEffect();
	void OpenMenuWeapon();
	void CloseMenuWeapon();
	bool isMenuWeaponOpen() { return this->isWeaponMenuActive; }

	//Reset health 
	void resetHealth();
	void resetGame();

	//Save game
	bool isOverheadtoSideView = false;
	bool isSaved() { return this->Saved; }
	int getLeft() { return this->left; }
	int getPlayer() { return this->flagPlayer; }
	int getCurrentSection() { return this->IDCurrentSection; }
	void saveGame();
	void savePlayer(int kindPlayer, float offx = 0, float offy = 0);
	void subLeft();
	void getCheckPoint(float& x, float& y) { x = this->playerX; y = this->playerY; }
	void saveSophia(float offx = 0, float offy = 0);
	void getInfoSophia(float& x, float& y, int& id) { x = this->sophiaX; y = this->sophiaY; id = this->IDSectionSophia; }
	void saveJason(float offx = 0, float offy = 0);
	void getInfoJason(float& x, float& y, int& id) { x = this->jasonX; y = this->jasonY; id = this->IDSectionJason; }

	//CuteTN weapon
	TypeWeapons selectedWeapon = TypeWeapons::MultiwarheadMissile;
	bool CheckSophiaCanUseWeapon();
	void AddToSelectedWeapon(int amount);
	
	void AddToHomingMissile(int amount);
	void AddToMultiwarheadMissile(int amount);
	void AddToThunderBreak(int amount);

	//Control key Weapon
	void NextSelectedItem();
	void BackSelectedItem();
	virtual void HandleTimerTick(LPTIMER sender);

	//Effect
	bool isEffectFaded = false;
	bool isEffectBoss = false;
	bool isEffectBossFadeIn = false;
	bool isRenderBossFlashing = false;
	bool isRenderBoss = false;
	void openEffectFlashingBoss();
	void openEffectBossDead();

	// CuteTN
	int sectionCrusherBeamComeback = -1;
	bool HasCrusherBeam = false;
	void comeBackAfterGetCrusherBeam();

	bool isWinGame = false;
};


//INFO LEVEL GUN POWER
//1	Jason fires shots that stop halfway across the screen.
//2	Jason can fire two shots that travel all the way across the screen.
//3	Jason can fire three shots that travel all the way across the screen.
//4	Jason can fire three shots that travel all the way across the screen.
//5	Some bullets that Jason fires will fly off in slowly expanding circles to either side.
//6	Some bullets that Jason fires will fly off in quickly expanding circles to either side.
//7	Jason can fire up to five bullets that travel in slowly expanding waves
//8	Jason can fire a stream of bullets that can pass through solid objects, and travel in quickly expanding waves.

//Also note that Jason can recover all of his power by returning to SOPHIA