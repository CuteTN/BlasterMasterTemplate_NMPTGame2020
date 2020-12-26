#pragma once

#include "Enemy.h"

#define MAX_HEALTH_SOPHIA 80
#define MAX_HEALTH_JASONSIDEVIEW 80
#define MAX_HEALTH_JASONOVERHEAD 80
#define BODY_DAMAGE_ENEMY 0
#define BODY_DAMAGE_LAVA 20
#define BODY_DAMAGE_SPIKE 60

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

class CGameGlobal
{
private:
	CGameGlobal();
	static CGameGlobal * _instance;

	//Save Game
	bool Saved = false;
	int left = 3;
	int flagPlayer = 1; // 1: Sophia 2:Jason SideView 3:Jason OverHead
	float playerX = 0, playerY = 0;
	float sophiaX = 0, sophiaY = 0;
	int IDSectionSophia = 0;
	int IDCurrentSection = -1;

	//AnimationHandler Render 
	CObjectAnimationHanlders HealthPow;
	CObjectAnimationHanlders Hov;
	CObjectAnimationHanlders Gun;

	//health player 
	int healthSophia;
	int healthJasonSideView;
	int healthJasonOverHead;

	//Gun Power
	int levelGunPower; // [0..8]

	//Weapons
	const int MAX_AMMUNITIONS = 99;
	int ammunitions_HomingMissile = 99;
	int ammunitions_MultiwarheadMissile = 99;
	int ammunitions_ThunderBreak = 99;

	//Input for Render Pow & Gun
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);

	//Support funtion, U shouldn't care it 
	void SupportRenderHeath(int health);

public:
	//reset health
	//get value methods
	static CGameGlobal* GetInstance();
	int get_healthSophia()		  { return this->healthSophia;		  }
	int get_healthJasonSideView() { return this->healthJasonSideView; }
	int get_healthJasonOverHead() { return this->healthJasonOverHead; }

	//event method
	void getItem(Items item);
	void beingAttackedByEnemy();
	void beingAttackedByLava();
	void beingAttackedBySpike();
	void jasonJumpIntoSophia();
	//Render
	void RenderHeath();
	
	//Reset health 
	void resetHealth();
	void resetGame();

	//Save game
	bool isSaved() { return this->Saved; }
	int getLeft() { return this->left; }
	int getPlayer() { return this->flagPlayer; }
	int getCurrentSection() { return this->IDCurrentSection; }
	void saveGame();
	void savePlayer(int kindPlayer);
	void subLeft();
	void getCheckPoint(float& x, float& y) { x = this->playerX; y = this->playerY; }
	void saveSophia();
	void getInfoSophia(float& x, float& y, int& id) { x = this->sophiaX; y = this->sophiaY; id = this->IDSectionSophia; }
	void beingAttackedByEnemy(CEnemy* enemy);

	//CuteTN weapon
	TypeWeapons selectedWeapon = TypeWeapons::MultiwarheadMissile;
	bool CheckSophiaCanUseWeapon();
	void AddToSelectedWeapon(int amount);
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