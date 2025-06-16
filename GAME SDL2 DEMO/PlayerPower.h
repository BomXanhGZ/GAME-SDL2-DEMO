
#ifndef POWER_PLAYER_H_
#define POWER_PLAYER_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "Geomitric.h"


#define PALYER_MAX_HP 7
#define STAR_HP_ICON_X 50
#define STAR_HP_ICON_Y 50

#define LIVES_ICON_X 70
#define LIVES_ICON_Y 5

#define AMMO_ICON_X (SCREEN_WIDTH/4 - TILE_SIZE)
#define AMMO_ICON_Y 5


class HpIcon : public BaseObject
{
public:

	HpIcon() {;}
	~HpIcon() {;}

	void Set_Pos_Show_Hp();
	void ShowPlayerHP(SDL_Renderer* des, int& player_hp);

private:

	SDL_Rect pos_show_HP[PALYER_MAX_HP];
};


class PlayerLivesIcon : public BaseObject
{
public:

	PlayerLivesIcon() {;}
	~PlayerLivesIcon() {;}
};


class AmmoIcon : public BaseObject
{
public:

	AmmoIcon() {;}
	~AmmoIcon() {;}

	void Set_AmmoIcon(const int& ammo_type, SDL_Renderer* des);
	void ShowAmmoIcon(SDL_Renderer* des);

};

#endif //POWER_PLAYER_H_