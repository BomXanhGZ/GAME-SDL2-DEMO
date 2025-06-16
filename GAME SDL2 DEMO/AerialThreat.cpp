
#include "AerialThreat.h"
#include "PlayerObject.h"

AerialThreat::AerialThreat()
{
	width_fram_ = 0;
	height_fram_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 400;
	y_pos_ = 175;
	is_show_effect_ = false;
	cameback_turn_ = 1;
	dir_ = Dir_LEFT;
	threat_hp_ = 0.0;
	thr_bullet_type_ = STANDERD_TYPE;
}

AerialThreat::~AerialThreat()
{
	;
}

void AerialThreat::InitAerialThreat(SDL_Renderer* des)
{
	LoadAnimation(aerial_threat_left_img, des, AERIAL_THREAT_FRAM_NUM);
	Set_threat_hp(AERIAL_THREAT_HP);
	MakeIsBullet(des);
	SetPosition(Y_START, AERIAL_THREAT_POS_1);
}

void AerialThreat::HandleThreat(Map& map_data, SDL_Renderer* des, PlayerObject& player, std::vector<EffectObject*> list_effect)
{
	SDL_Rect player_rect = player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);

	SetMapXY(map_data.star_x_, map_data.star_y_);
	DoThreat(map_data, des);
	SDL_Rect threat_rect   = GetRectWhileHaveNumFramIs(AERIAL_THREAT_FRAM_NUM);

	ReloadIsBullet(des, player_rect);
	SDL_Rect t_bullet_rect = HandleIsBullet(des, SCREEN_WIDTH, SCREEN_HEIGHT*0.8, list_effect);
					

	//CHECK COLISSION
				
	bool Check_Ply_vs_Tbullet = SDLFunction::CheckCollision(t_bullet_rect, player_rect);
	if(Check_Ply_vs_Tbullet)
	{
 		int b = 2;
		ReloadBullet(AERIAL_THREAT_RELOAD_TIME_BULLET, list_effect);

		player.HandlePowerPlayer( p_bullet.Get_bullet_dmg(), list_effect );
	}

	std::vector<BulletObject*> ply_bullet_list = player.Get_bullet_list();
	for(int pbl = 0; pbl < ply_bullet_list.size(); pbl++)
	{
		BulletObject* ply_bullet = ply_bullet_list.at(pbl);
		if(ply_bullet != NULL)
		{
			SDL_Rect ply_bullet_rect = ply_bullet->GetRectWhileHaveNumFramIs(ply_bullet->Get_bullet_fram_num());

			//colistion bullet of ply vs threat
			bool Check_IS_Thr_vs_PlyBullet  = SDLFunction::CheckCollision(ply_bullet_rect, threat_rect);
			if(Check_IS_Thr_vs_PlyBullet)
			{
				int a = 2;
				ply_bullet->Set_is_move(false);

				if(threat_hp_ > 0) {threat_hp_ -= ply_bullet->Get_bullet_dmg(); }  //val = DMG BULLET TYPE
				if(threat_hp_ <= 0)
				{ 
					list_effect.at(EXP_EFFECT)->Set_is_show(true);
					list_effect.at(EXP_EFFECT)->SetRect_PositionEffect(    GetRectWhileHaveNumFramIs(AERIAL_THREAT_FRAM_NUM),
																			this->x_pos_, this->y_pos_   );
					//Mix_PlayChannel(-1, g_audio.at(STANDARD_EXP_SOUND), 0);

					threat_hp_ = AERIAL_THREAT_HP;
					cameback_turn_++;
					if(cameback_turn_ == 2)
					{
						SetPosition(Y_START, AERIAL_THREAT_POS_2);
					}
					else if (cameback_turn_ == 3)
					{
						SetPosition(Y_START, AERIAL_THREAT_POS_3);
					}
					else if (cameback_turn_ > MAX_CB_TURN)
					{
						x_pos_ = map_x_ - SCREEN_WIDTH*3.5;
					}
				}
			}
		}
	}

	Show(des);
}

void AerialThreat::Show(SDL_Renderer* des)
{
	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	Render(des);
}

void AerialThreat::DoThreat(Map& map_data, SDL_Renderer* des)
{

	if(threat_hp_ > 0)
	{
		if(cameback_turn_ <= MAX_CB_TURN)
		{
			if(map_x_ + SCREEN_WIDTH > x_pos_)
			{
				HandleMove(des);
				x_pos_ += x_val_;
				y_pos_ += y_val_;

				if(rect_.x < -SCREEN_WIDTH)
				{
					threat_hp_ = AERIAL_THREAT_HP;

					cameback_turn_++;
					if(cameback_turn_ == 2)
					{
						SetPosition(Y_START, AERIAL_THREAT_POS_2);
					}
					else if (cameback_turn_ == 3)
					{
						SetPosition(Y_START, AERIAL_THREAT_POS_3);
					}
					else if (cameback_turn_ > 3)
					{
						x_pos_ = map_x_ - SCREEN_WIDTH*3.5;
					}
				}
			}
		}
		else if(cameback_turn_ > MAX_CB_TURN)
		{
			dir_ = Dir_RIGHT;
			HandleMove(des);
			x_pos_ += x_val_;
			y_pos_ += y_val_;

			//std::cout <<"rect_fly is:" <<rect_.x <<std::endl;

			if(this->rect_.x > SCREEN_WIDTH*2)
			{
				x_pos_ = map_x_ - SCREEN_WIDTH*3.5;
			}
		}
	}
}

void AerialThreat::HandleMove(SDL_Renderer* des)
{
	if(dir_ == Dir_LEFT)
	{
		LoadAnimation(aerial_threat_left_img, des, 1);
		x_val_ = -AERIAL_THREAT_SPEED;
		y_val_ = 0;
	}
	else if(dir_ == Dir_RIGHT)
	{
		LoadAnimation(aerial_threat_right_img, des, 1);
		x_val_ = AERIAL_THREAT_SPEED;
		y_val_ = 0;
	}
}

void AerialThreat::MakeIsBullet(SDL_Renderer* des)
{
	thr_bullet_type_ = ROCKET_TYPE;
	p_bullet.SetDataBullet(thr_bullet_type_);
	p_bullet.LoadAnimation(p_bullet.Get_img_bullet_type(dir_), des, 1);
}



void AerialThreat::ReloadIsBullet(SDL_Renderer* des, const SDL_Rect& player_rect)
{

	if(reload_time_bullet_ > 0)
	{
		reload_time_bullet_ -= 1;
	}

	if(map_x_ + SCREEN_WIDTH > x_pos_ && map_x_ + SCREEN_WIDTH/8 < x_pos_
	&& reload_time_bullet_ <= 0)
	{
		if(p_bullet.Get_is_move() != true)
		{

			if(dir_ == Dir_LEFT) 
			{
				p_bullet.Set_bullet_dir(Dir_LEFT_DOWN);
			}
			else if(dir_ == Dir_RIGHT) 
			{
				p_bullet.Set_bullet_dir(Dir_RIGHT_DOWN); 
			}

			//Set_Update
			p_bullet.LoadAnimation(   p_bullet.Get_img_bullet_type(dir_), des, p_bullet.Get_bullet_fram_num()    );
	   		p_bullet.Set_position(this->x_pos_ + this->width_fram_ * 0.5, this->y_pos_ + this->rect_.h);
			p_bullet.SetRect(this->x_pos_ - this->map_x_ , this->y_pos_ - this-> map_y_ + this->rect_.h);
			p_bullet.Set_x_val(INERTIA);  // -= ...
			p_bullet.Set_y_val(MAX_FALL_SPEED);
			p_bullet.Set_is_move(true);
		}
	}
}

SDL_Rect AerialThreat::HandleIsBullet(SDL_Renderer* des, int x_bdr, int y_bdr, std::vector<EffectObject*> list_effect)
{
	if(reload_time_bullet_ <= 0)
	{
		if(map_x_ + SCREEN_WIDTH >= x_pos_ && map_x_ <= x_pos_ + width_fram_)
		{
			if(p_bullet.Get_is_move() == true)
			{
				p_bullet.SetMapXY(this->map_x_, this->map_y_);
				p_bullet.HandleMove(x_bdr, y_bdr);
				if(p_bullet.Get_is_move() == false)
				{
					ReloadBullet(AERIAL_THREAT_RELOAD_TIME_BULLET, list_effect);
				}

				p_bullet.ShowBullet(des);
			}

			SDL_Rect bullet_rect = p_bullet.GetRectWhileHaveNumFramIs(1);

			return bullet_rect;
		}
	}
}

void AerialThreat::ReloadBullet(const int& reload_time, std::vector<EffectObject*> list_effect)
{
	list_effect.at(ROCKET_EXP_EFFECT)->Set_is_show(true);
	list_effect.at(ROCKET_EXP_EFFECT)->SetRect_PositionEffect(  p_bullet.GetRectWhileHaveNumFramIs(p_bullet.Get_bullet_fram_num()),
																p_bullet.Get_xpos(), p_bullet.Get_ypos()   );

	p_bullet.Set_is_move(false);
	p_bullet.SetRect(this->rect_.x, this->rect_.y);
	reload_time_bullet_ = reload_time;
}





