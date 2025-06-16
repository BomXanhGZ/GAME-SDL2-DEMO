
#ifndef BOSS_POWER_H_
#define BOSS_POWER_H_


#include "FunctionFile.h"
#include "BaseObject.h"


#define MAX_BOSS_HP 200
#define BOSS_HP_FRAM_NUM 1
#define BOSS_RECT_HP_Y SCREEN_HEIGHT*0.175
#define BOSS_RECT_HP_X SCREEN_WIDTH*0.95

#define BOSS_ICON_RECT_Y BOSS_RECT_HP_Y  //with boss rect hp
#define BOSS_ICON_RECT_X BOSS_RECT_HP_X  //with boss rect hp


class IconBossHp : public BaseObject
{
public:

	IconBossHp() {;}
	~IconBossHp() {;}

	void InitHpBoss(SDL_Renderer* des);
	void update_show_hp_boss(float& hp_boss, SDL_Renderer* des);

private:

	SDL_Rect rect_show_hp_[MAX_BOSS_HP];
};


class BossIcon : public BaseObject
{
public:

	BossIcon() {;}
	~BossIcon() {;}

	void InitIconBoss(std::string path, SDL_Renderer* des);
	void ShowIconBoss(SDL_Renderer* des);

};

#endif //BOSS_POWER_H_