
#include "ChasingRocket.h"
#include "PlayerObject.h"

ChasingRocket::ChasingRocket()
{
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	map_x_ = 0;
	map_y_ = 0;
	rect_.x = CHASING_ROCKET_POS_1;

	hp_ = CHASING_ROCKET_HP;
	fram_ = 0;
	is_RePos_ = false;
	is_move_ = false;
	is_show_ = true;
}

ChasingRocket::~ChasingRocket()
{

}

void ChasingRocket::InitChasingRoccket(SDL_Renderer* des)
{
	Make_ChasingRocketPosList();

	LoadAnimation(chasing_rocket_img, des, CHASING_ROCKET_FRAM_NUM);
	Set_clip();
	Set_y_pos(Y_START);
	Set_x_pos(CHASING_ROCKET_POS_1);
}

void ChasingRocket::HandleChasingRocket(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list)
{
	Set_mapXY(g_map->star_x_, g_map->star_y_);

	DoChasingRocket(player, effect_list);
	Show(des);
}

void ChasingRocket::Show(SDL_Renderer* des)
{
	fram_++;
	if(fram_ >= CHASING_ROCKET_FRAM_NUM)
	{
		fram_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect Renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

	int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &Renderquad);
}

void ChasingRocket::DoChasingRocket(PlayerObject* player, std::vector<EffectObject*> effect_list)
{

	SDL_Rect ply_rect = player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);

	//ChasingRocket move
	ChasingRocketHandleMove(player);

	//colide
	if(SDLFunction::CheckCollision(   ply_rect, this->GetRectWhileHaveNumFramIs(CHASING_ROCKET_FRAM_NUM)   )       //ply vs ChasingRocket
								   ==    true)
	{
		player->HandlePowerPlayer(CHASING_ROCKET_DMG_VAL, effect_list);

		effect_list.at(FLAST_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(CHASING_ROCKET_FRAM_NUM), 
																this->x_pos_, this->y_pos_   );

		effect_list.at(FLAST_EFFECT)->Set_is_show(true);

		RestartThreat();
	}

	std::vector<BulletObject*> ply_bulletlist = player->Get_bullet_list();											//bullet's player vs ChasingRocket
	for(int pb = 0; pb < ply_bulletlist.size(); pb++)
	{
		BulletObject* ply_bullet = ply_bulletlist.at(pb);
		if(ply_bullet != NULL)
		{
			if(SDLFunction::CheckCollision(   ply_bullet->GetRectWhileHaveNumFramIs(ply_bullet->Get_bullet_fram_num()), 
											  this->GetRectWhileHaveNumFramIs(CHASING_ROCKET_FRAM_NUM)   )
											  ==  true)
			{

				ply_bullet->Set_is_move(false);

				hp_ -= ply_bullet->Get_bullet_dmg();
				if(hp_ <= 0)
				{
					effect_list.at(EXP_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(CHASING_ROCKET_FRAM_NUM),
																		  this->x_pos_, this->y_pos_   );
					effect_list.at(EXP_EFFECT)->Set_is_show(true);

					Mix_PlayChannel(-1, g_audio.at(STANDARD_EXP_SOUND), 0);

					RestartThreat();
				}
			}
		}
	}

	//come back
	if(is_RePos_ == true)
	{
		y_pos_ = Y_START;

		if(chasing_rocket_poslist_.size() > 0)
		{
			x_pos_ = chasing_rocket_poslist_.at(0);

			if(chasing_rocket_poslist_.size() == 1)			 //boss shut
			{
				y_pos_ = 450;
			}


			chasing_rocket_poslist_.erase( chasing_rocket_poslist_.begin() );
		}
		else
		{
			is_show_ = false;
		}

		is_RePos_ = false;
	}
}

void ChasingRocket::ChasingRocketHandleMove(PlayerObject* player)
{

	if(this->rect_.x < SCREEN_WIDTH)
	{
		is_move_ = true;
	}


	if(is_move_ == true)
	{
		float ply_pos_x = player->Get_x_pos();
		float ply_pos_y = player->Get_y_pos();

		x_val_ = -CHASING_ROCKET_SPEED; 

		if(ply_pos_x < this->x_pos_)
		{
			if(ply_pos_y < this->y_pos_)
			{
				y_val_ = -CHASING_ROCKET_HOMING;
			}
			else if(ply_pos_y > this->y_pos_)
			{
				y_val_ = CHASING_ROCKET_HOMING;
			}
		}
		else
		{
			y_val_ = 0;
		}

		if(this->rect_.x < (SCREEN_WIDTH * (-0.2)))
		{
			RestartThreat();
		}
	}
	

	x_pos_ += x_val_;
	y_pos_ += y_val_;
}

void ChasingRocket::Set_clip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < CHASING_ROCKET_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;
		}
	}
}

void ChasingRocket::RestartThreat()
{
	is_RePos_ = true;
	is_move_ = false;
	hp_ = CHASING_ROCKET_HP;
	x_val_ = 0;
	y_val_ = 0;
}

void ChasingRocket::Make_ChasingRocketPosList()
{
	chasing_rocket_poslist_.push_back(CHASING_ROCKET_POS_2);
	chasing_rocket_poslist_.push_back(CHASING_ROCKET_POS_3);
	chasing_rocket_poslist_.push_back(CHASING_ROCKET_POS_4);
	chasing_rocket_poslist_.push_back(CHASING_ROCKET_POS_5);
	chasing_rocket_poslist_.push_back(CHASING_ROCKET_POS_6);

}