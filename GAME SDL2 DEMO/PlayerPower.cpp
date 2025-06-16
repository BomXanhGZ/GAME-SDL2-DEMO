#include "PlayerPower.h"


void HpIcon::Set_Pos_Show_Hp()
{
	for(int h = 0; h < PALYER_MAX_HP; h++)
	{
		pos_show_HP[h].x = STAR_HP_ICON_X;
		pos_show_HP[h].y = STAR_HP_ICON_Y + h*15; 
		pos_show_HP[h].w = rect_.w;
		pos_show_HP[h].h = rect_.h;
	}
}

void HpIcon::ShowPlayerHP(SDL_Renderer* des, int& player_hp)
{
	if(player_hp > PALYER_MAX_HP)
	{
		player_hp = PALYER_MAX_HP;
	}

	for(int i = 0; i < player_hp; i++)
	{
		SDL_RenderCopy(des, p_object_, NULL, &pos_show_HP[i]);
	}
}

void AmmoIcon::Set_AmmoIcon(const int& ammo_type, SDL_Renderer* des)
{
	//SET DATA
	std::string link_img;

	if(ammo_type == STANDERD_TYPE) {link_img = standard_bullet_img; }
	else if(ammo_type == BALL_TYPE) {link_img = ball_bullet_img; }
	else if(ammo_type == ROUND_TYPE) {link_img = round_bullet_img; }
	else if(ammo_type == STAR_TYPE) {link_img = star_bullet_img; }
	else if(ammo_type == ROCKET_TYPE) 
	{
		link_img = rocket_bullet_right_img; 
	}
	else if(ammo_type == PRO_TYPE) {link_img = pro_bullet_icon_img; }
	
	LoadImg(link_img, des);

	//SET RECT
	SetRect(   AMMO_ICON_X, (RECT_TANGLE_H*0.5 - this->rect_.h*0.5)   );
}

void AmmoIcon::ShowAmmoIcon(SDL_Renderer* des)
{
	Render(des);
}