
#include "BossPower.h"


void IconBossHp::InitHpBoss(SDL_Renderer* des)
{
	LoadAnimation(boss_hp_img, des, BOSS_HP_FRAM_NUM);
}

void IconBossHp::update_show_hp_boss(float& hp_boss, SDL_Renderer* des)
{
	for(int i = 0; i < hp_boss; i++)
	{
		rect_show_hp_[i].x = BOSS_RECT_HP_X;
		rect_show_hp_[i].y = BOSS_RECT_HP_Y + i*height_fram_;
		rect_show_hp_[i].w = width_fram_;
		rect_show_hp_[i].h = height_fram_;

		int check = SDL_RenderCopy(des, p_object_, NULL, &rect_show_hp_[i]);
	}
}


void BossIcon::InitIconBoss(std::string path, SDL_Renderer* des)
{
	LoadImg(path, des);
	SetRect(	BOSS_ICON_RECT_X - this->rect_.w*0.5 + 8, 
				BOSS_ICON_RECT_Y - this->rect_.h - 6	);
}

void BossIcon::ShowIconBoss(SDL_Renderer* des)
{
	Render(des);
}