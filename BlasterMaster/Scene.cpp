#include "Scene.h"
#include <string>
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include <map>

using namespace std;

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

void CScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	//int R = atoi(tokens[2].c_str());
	//int G = atoi(tokens[3].c_str());
	//int B = atoi(tokens[4].c_str());

	int R = 255;
	int G = 255;
	int B = 255;

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int texID = atoi(tokens[1].c_str());
	int l = atoi(tokens[2].c_str());
	int t = atoi(tokens[3].c_str());
	int r = atoi(tokens[4].c_str());
	int b = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == nullptr)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSpriteLib::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	////DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int duration = atoi(tokens[i + 1].c_str());
		ani->AddFrame(sprite_id, duration);
	}

	CAnimationLib::GetInstance()->Add(ani_id, ani);
}
	
void CScene::_ParseSection_STATE_ANIMATION(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; 

	LPANIMATION ani = new CAnimation();

	int state_id = atoi(tokens[0].c_str());
	int ani_id = atoi(tokens[1].c_str());
	int flipX = atoi(tokens[2].c_str());
	int flipY = atoi(tokens[3].c_str());
	int Rotate = atoi(tokens[4].c_str());

	//Nghia code

}

void CScene::_ParseSection_OBJECT_ANIMATION(string line)
{

	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int objectAni_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i ++)
	{
		int ani_id = atoi(tokens[i].c_str());
		//Add ani into Object_Animations
	}

	//Nghia code
}

void CScene::_ParseSection_COLLISION_BOXES(string line)
{
	//Nghia code
}

void CScene::_ParseSection_SECTIONS(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int section_ID = atoi(tokens[0].c_str());
	int texture_ID = atoi(tokens[1].c_str());

	LPDIRECT3DTEXTURE9 background = CTextures::GetInstance()->Get(texture_ID);
	if (background == nullptr)	
	{
		DebugOut(L"[ERROR] Backgound of section %d not found!\n", texture_ID);
		return;
	}

	//Add section
}
	
void CScene::_ParseSection_CLASSES(string line)
{
	//Already has const file
}
	
void CScene::_ParseSection_OBJECTS(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	int obj_ID = atoi(tokens[0].c_str());
	int class_ID = atoi(tokens[1].c_str());

	//Create list properties 
	map <string, int> Properties;
	for (int i = 2; i < tokens.size(); i+=2)
	{
		string prop_name = tokens[i].c_str();
		int prop_value = atoi(tokens[i + 1].c_str());
	}

	//Nghia code
}