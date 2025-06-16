
#include "RockTitanBoss.h"
#include "PlayerObject.h"

RockTitanBoss::RockTitanBoss()
{
	rect_.x = 0;
	x_pos_ = ROCK_TITAN_BOSS_X_POS;
	y_pos_ = 0.0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	map_x_ = 0;
	map_y_ = 0;

	change_angle_ = true;
	on_ground_ = false;
	boss_hp_ = ROCK_TITAN_BOSS_HP;
	bullet_type_ = BALL_TYPE;
	fram_ = 0;
}

RockTitanBoss::~RockTitanBoss()
{
	;
}

void RockTitanBoss::Init_boss(SDL_Renderer* des)
{
	LoadAnimation(rock_titan_boss_img, des, ROCK_TITAN_BOSS_FRAM_NUM);
	icon_BossHP_.InitHpBoss(des);
	MakeBullet(des);
	Set_clip(ROCK_TITAN_BOSS_FRAM_NUM);
}

void RockTitanBoss::HandleBoss(SDL_Renderer* des, Map& map_data, PlayerObject& player, std::vector<EffectObject*> list_effect)
{
	Set_map_x_y(map_data.star_x_, map_data.star_y_);

	HanldeMove();

	CheckToMap(map_data);

	CheckColision(player, list_effect);

	icon_BossHP_.update_show_hp_boss(this->boss_hp_, des);

	HandleBullet(player, list_effect);

	ShowBullet(des);

	ShowBoss(des);

}


void RockTitanBoss::ShowBoss(SDL_Renderer* des)
{

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	fram_++;
	if(fram_ >= ROCK_TITAN_BOSS_FRAM_NUM)
	{
		fram_ = 0;
	}

	SDL_Rect current_clip = clip_[fram_];
	SDL_Rect renderQuard = {rect_.x, rect_.y, width_fram_, height_fram_};
	int checkshow = SDL_RenderCopy(des, p_object_, &current_clip, &renderQuard);

	if(checkshow < 0)
	{
		int a = 2;
	}
}

void RockTitanBoss::HanldeMove()
{
	y_val_ += GRAVITY_SPEED;
	if(on_ground_ == true)
	{
		y_val_ = 0;
	}
}

void RockTitanBoss::CheckColision(PlayerObject& player, std::vector<EffectObject*> list_effect)
{
	//check colision
	bool ply_vs_boss = SDLFunction::CheckCollision(                                //ply_vs_boss
					   player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM), 
					   GetRectWhileHaveNumFramIs(ROCK_TITAN_BOSS_FRAM_NUM)		);
	if(ply_vs_boss == true)
	{
		player.HandlePowerPlayer(DIE_VAL, list_effect);
	}


	std::vector<BulletObject*> p_bullet_list = player.Get_bullet_list();
	for(int p_b = 0; p_b < p_bullet_list.size(); p_b ++ )
	{
		BulletObject* p_bullet = p_bullet_list.at(p_b);

		bool ply_bullet_vs_boss = SDLFunction::CheckCollision(
								  p_bullet->GetRectWhileHaveNumFramIs(	p_bullet->Get_bullet_fram_num()	),
								  GetRectWhileHaveNumFramIs(ROCK_TITAN_BOSS_FRAM_NUM)		);
		{
			if(ply_bullet_vs_boss == true)
			{
				p_bullet->Set_is_move(false);

				boss_hp_ -= p_bullet->Get_bullet_dmg();

				if(boss_hp_ <= 0)
				{
					list_effect.at(FLAST_EFFECT)->Set_is_show(true);
					list_effect.at(FLAST_EFFECT)->SetRect_PositionEffect(   GetRectWhileHaveNumFramIs(ROCK_TITAN_BOSS_FRAM_NUM),
																			this->x_pos_, this->y_pos_  );
					Mix_PlayChannel(-1, g_audio.at(BLIND_EXP_SOUND), 0);
				}
			}
		}
	}
}

void RockTitanBoss::MakeBullet(SDL_Renderer* des)
{
	for(int i = 0; i < ROCK_TITAN_BOSS_BULLET_NUM; i++)
	{
		BulletObject* boss_bullet = new BulletObject;
		boss_bullet->SetDataBullet(bullet_type_);
		boss_bullet->LoadAnimation(boss_bullet->Get_img_bullet_type(1), des, boss_bullet->Get_bullet_fram_num());
		boss_bullet->Set_is_move(true);
		boss_bullet_list.push_back(boss_bullet);
	}
}

void RockTitanBoss::HandleBullet(PlayerObject& player, std::vector<EffectObject*> list_effect)
{

	float boss_bullet_x_speed = ROCK_TITAN_BOSS_BULLET_SPEED_X;
	float boss_bullet_y_speed = ROCK_TITAN_BOSS_BULLET_SPEED_Y;

	if(!change_angle_)
	{
		 boss_bullet_y_speed += 3;
	}


	for(int i = 0; i < boss_bullet_list.size(); i++)
	{
		BulletObject* boss_bullet = boss_bullet_list.at(i);

		if(i == 0)
		{
			boss_bullet->Set_x_val(boss_bullet_x_speed);
			boss_bullet->Set_y_val(boss_bullet_y_speed);
			boss_bullet->Set_bullet_dir(Dir_LEFT);
		}
		else if(i == 1)
		{
			boss_bullet->Set_x_val(boss_bullet_x_speed);
			boss_bullet->Set_y_val(boss_bullet_y_speed);
			boss_bullet->Set_bullet_dir(Dir_LEFT_UP);
		}
		else if(i == 2)
		{
			boss_bullet->Set_x_val(boss_bullet_x_speed);
			boss_bullet->Set_y_val(boss_bullet_y_speed);
			boss_bullet->Set_bullet_dir(Dir_LEFT_DOWN);
		}

		boss_bullet->SetMapXY(this->map_x_, this->map_y_);
		boss_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);

		//check colision
		bool ply_vs_boss_bullet = SDLFunction::CheckCollision
								  (
									  player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM),
									  boss_bullet->GetRectWhileHaveNumFramIs( boss_bullet->Get_bullet_fram_num() )
								  );
		if(ply_vs_boss_bullet == true)
		{
			boss_bullet->Set_is_move(false);
			player.HandlePowerPlayer(boss_bullet->Get_bullet_dmg(), list_effect);
		}

		//ReShowBullet
		if(boss_bullet->Get_is_move() == false)
		{
			boss_bullet->Set_position(this->x_pos_ + width_fram_*1/4, this->y_pos_ + this->height_fram_*1/5);
			boss_bullet->Set_is_move(true);
			change_angle_ = !change_angle_;
		}
	}
}

void RockTitanBoss::ShowBullet(SDL_Renderer* des)
{
	for(int i = 0; i < boss_bullet_list.size(); i++)
	{
		BulletObject* boss_bullet = boss_bullet_list.at(i);
		boss_bullet->ShowBullet(des);
	}
}

void RockTitanBoss::CheckToMap(Map& map_data)
{

	int x1,x2,y1,y2;
	std::vector<int> obstacle_tile = check_obstacle_tile_.Get_obstancle_tile();

	int height_min = height_fram_ < TILE_SIZE ? height_fram_ : TILE_SIZE;
	int width_min = width_fram_ < TILE_SIZE ? width_fram_ :	TILE_SIZE;
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	y1 = y_pos_ / TILE_SIZE;

	//horizontal
	x2 = (x_pos_ + width_fram_) / TILE_SIZE - 1;
	y2 = (y_pos_ + height_min) / TILE_SIZE;

	if(x1 > 0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
	{
		if(x_val_ < 0)
		{
			int check_left_up = map_data.tile[y1][x1];
			int check_left_down = map_data.tile[y2][x1];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_left_up) != obstacle_tile.end()
			&& std::find(obstacle_tile.begin(), obstacle_tile.end(), check_left_down) != obstacle_tile.end())
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
			}
		}
		else if(x_val_ >  0)
		{

			int check_right_up = map_data.tile[y1][x2];
			int check_right_down = map_data.tile[y2][x2];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_right_up) != obstacle_tile.end()
			&& std::find(obstacle_tile.begin(), obstacle_tile.end(), check_right_down) != obstacle_tile.end())
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= (width_fram_ + 1);
				x_val_ = 0;
			}
		}
	}

	//ventical
	x2 = (x_pos_ + x_val_ + width_min) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_fram_) / TILE_SIZE - 1;

	if(x1 > 0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
	{
		if(y_val_ < 0)
		{
			int check_up_left = map_data.tile[y1][x1];
			int check_up_right = map_data.tile[y1][x2];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_up_left) != obstacle_tile.end()
			|| std::find(obstacle_tile.begin(), obstacle_tile.end(), check_up_right) != obstacle_tile.end())
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
		else if(y_val_ >= 0)
		{
			int check_down_left = map_data.tile[y2][x1];
			int check_down_right = map_data.tile[y2][x2];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_down_left) != obstacle_tile.end()
			|| std::find(obstacle_tile.begin(), obstacle_tile.end(), check_down_right) != obstacle_tile.end())
			{
				on_ground_ = true;

				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_fram_ + 1);
				y_val_ = 0;
			}
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

}

void RockTitanBoss::Set_clip(const int& fram_num)
{
	for(int i = 0; i < fram_num; i++)
	{
		clip_[i].x = width_fram_*i;
		clip_[i].y = 0;
		clip_[i].w = width_fram_;
		clip_[i].h = height_fram_;
	}
}