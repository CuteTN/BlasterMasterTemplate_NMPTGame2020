#pragma once
#include <d3d9.h>
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"

class CSection
{
private:
	// int sectionId;					we may not need this
	int backgroundTextureId;
	// LPSCENE scene;					why we need to know its scene anyway?

public:
	CSection() {};
	CSection(int bgTextureId);

	void RenderBackground();
	void Update(DWORD dt);
	void Render();

	vector<CGameObject*> Objects;
};

typedef CSection* LPSECTION;