
#include "RocketWardenBoss.h"
#include "PlayerObject.h"

RocketWardenBoss::RocketWardenBoss()
{
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = RW_BOSS_FIRST_POS;
	y_pos_ = Y_START;
	map_x_ = 0;
	map_y_ = 0;

	Dir_ = Dir_LEFT;
	fram_ = 0;
	bullet_count_ = 0;
	reload_bullet_val_[1] = 0;
	reload_bullet_val_[2] = 0;
	reload_bullet_val_[3] = 0;
	hp_ = RW_BOSS_HP;
	hits_counter_ = RW_BOSS_RESET_HITS_COUNTER_VAL;
	shield_duration_ = 0;
	on_ground_ = false;
	is_RePos_ = false;
	is_shut_rocket_ = false;
}

RocketWardenBoss::~RocketWardenBoss()
{
	;
}

void RocketWardenBoss::InitRocketWardenBoss(SDL_Renderer* des)
{

	srand(time(NULL));

	LoadAnimation(RWboss_left_img, des, RW_BOSS_FRAM_NUM);
	MakeBullet(des);
	icon_BossHP_.InitHpBoss(des);
	boss_icon_.InitIconBoss(RWboss_icon_img, des);
	Set_Clip();
	check_obstacle_tile_.Set_obstacle_tile();
}

void RocketWardenBoss::HandleRocketWardenBoss(SDL_Renderer* des, Map& g_map, ChasingRocket* c_rocket, PlayerObject* p_player, std::vector<EffectObject*> effect_list)
{
	Set_mapXY(g_map.star_x_, g_map.star_y_);

	CheckToMap(g_map);

	CheckCollision(p_player, effect_list);

	icon_BossHP_.update_show_hp_boss(this->hp_, des);

	boss_icon_.ShowIconBoss(des);

	HandleChasingRocket(c_rocket);

	HandleMove(des, p_player);

	HandleBullet(des, p_player);

	HandleSield(effect_list);
	
	Show(des);
}

void RocketWardenBoss::CheckToMap(Map &map_data)
{
	if(on_ground_ == false)
	{
		y_val_ += GRAVITY_SPEED;
	}
	else
	{
		y_val_ = 0;
	}

	int x1,x2,y1,y2;
	std::vector<int> obstacle_tile = check_obstacle_tile_.Get_obstancle_tile();

	int height_min = height_fram_ < TILE_SIZE ? height_fram_ : TILE_SIZE;
	int width_min = width_fram_ < TILE_SIZE ? width_fram_ :	TILE_SIZE;
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	y1 = y_pos_ / TILE_SIZE;

	//horizontal
	x2 = (x_pos_ + width_fram_ - 1) / TILE_SIZE;
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
	y2 = (y_pos_ + y_val_ + height_fram_ -1) / TILE_SIZE;

	if(x1 > 0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
	{
		if(y_val_ <= 0)
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
		else if(y_val_ > 0)
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
			else
			{
				on_ground_ = false;
			}
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;
}


void RocketWardenBoss::MakeBullet(SDL_Renderer* des)
{
	for(int bbl = 0; bbl < RW_BOSS_BULLET_NUM; bbl++)
	{
		boss_bullet[bbl].SetDataBullet(STAR_TYPE);
		boss_bullet[bbl].LoadAnimation(   boss_bullet[bbl].Get_img_bullet_type(Dir_NONE), des, boss_bullet[bbl].Get_bullet_fram_num()   );
	}
}

void RocketWardenBoss::HandleBullet(SDL_Renderer* des, PlayerObject* player)
{

	boss_bullet[bullet_count_].SetMapXY(this->map_x_, this->map_y_);

	if(boss_bullet[bullet_count_].Get_is_move() == false && reload_bullet_val_[bullet_count_] <= 0)
	{

		//reset_bullet
		float distance_x = (player->Get_x_pos() - this->x_pos_);
		float distance_y = (player->Get_y_pos() - this->y_pos_);

		float magnitude = sqrt(distance_x*distance_x + distance_y*distance_y);

		float bl_xval = distance_x / magnitude * RW_BOSS_BULLET_SPEED;
		float bl_yval = distance_y / magnitude * RW_BOSS_BULLET_SPEED;

		boss_bullet[bullet_count_].Set_x_val(bl_xval);
		boss_bullet[bullet_count_].Set_y_val(bl_yval);

		boss_bullet[bullet_count_].Set_position(this->x_pos_, this->y_pos_);

		boss_bullet[bullet_count_].Set_bullet_dir(Dir_NONE);
		boss_bullet[bullet_count_].Set_is_move(true);

		load_shut_img_time_val_ = RW_BOSS_LOAD_SHUT_VAL;

	}


	bullet_count_++;
	if(bullet_count_ >= RW_BOSS_BULLET_NUM)
	{
		bullet_count_ = 0;
	}
		

	//handle
	for(int bbl = 0; bbl < RW_BOSS_BULLET_NUM; bbl++)
	{
		if(boss_bullet[bbl].Get_is_move() == true)
		{
			boss_bullet[bbl].HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);

			if(boss_bullet[bbl].Get_is_move() == false)
			{
				boss_bullet[bbl].Set_position(-100, -100);
				boss_bullet[bbl].SetRect(this->rect_.x, this->rect_.y);

				reload_bullet_val_[bbl] = RW_BOSS_RELOAD_BULLET + RW_BOSS_BULLET_NUM*RW_BOSS_SHOT_INTERVEL*bbl;

				boss_bullet[bbl].Set_x_val(0);
				boss_bullet[bbl].Set_y_val(0);
			}
		}
		else
		{
			reload_bullet_val_[bbl]--;
		}

		boss_bullet[bbl].ShowBullet(des);
	}
}

void RocketWardenBoss::HandleMove(SDL_Renderer* des, PlayerObject* p_player)
{

	//re_Pos
	if(is_RePos_ == true)
	{
		x_pos_ = (MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*1.5) + rand()% SCREEN_WIDTH*1.25;
		y_pos_ = 1 + rand()%(SCREEN_HEIGHT - TILE_SIZE*4);
		on_ground_ = false;
		is_RePos_ = false;
	}

	//reloadIMG
	if(p_player->Get_x_pos() > this->x_pos_)
	{
		Dir_ = Dir_RIGHT;

		if(load_shut_img_time_val_ > 0)
		{
			LoadAnimation(RWboss_shut_right_img, des, RW_BOSS_FRAM_NUM);
			load_shut_img_time_val_--;
		}
		else
		{
			LoadAnimation(RWboss_right_img, des, RW_BOSS_FRAM_NUM);
		}
	}
	else
	{
		Dir_ = Dir_LEFT;

		if(load_shut_img_time_val_ > 0)
		{
			LoadAnimation(RWboss_shut_left_img, des, RW_BOSS_FRAM_NUM);
			load_shut_img_time_val_--;
		}
		else
		{
			LoadAnimation(RWboss_left_img, des, RW_BOSS_FRAM_NUM);
		}
	}
}

void RocketWardenBoss::HandleSield(std::vector<EffectObject*> effect_list)
{
	if(shield_duration_ > 0)
	{

		effect_list.at(BLUE_SHIELD_EFFECT)->SetRect_PositionEffect(this->GetRectWhileHaveNumFramIs(RW_BOSS_FRAM_NUM),
															this->x_pos_, this->y_pos_);
		effect_list.at(BLUE_SHIELD_EFFECT)->Set_is_show(true);


		shield_duration_--;

		if(shield_duration_ == 0)
		{
			hits_counter_ = RW_BOSS_RESET_HITS_COUNTER_VAL;
		}
	}
}

void RocketWardenBoss::HandleChasingRocket(ChasingRocket* c_rocket)
{
	if(    Dir_ == Dir_LEFT
		&& hp_ < RW_BOSS_HP*0.9
		&& is_shut_rocket_ == true)
	{
		c_rocket->Set_x_pos(this->x_pos_);
		c_rocket->Set_y_pos(this->y_pos_);
		c_rocket->Set_IsShow(true);
	}

	is_shut_rocket_ = false;
}

void RocketWardenBoss::CheckCollision(PlayerObject* p_player, std::vector<EffectObject*> effect_list)
{

	//Rect Obj:
	SDL_Rect ply_rect = p_player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);
	SDL_Rect boss_rect = this->GetRectWhileHaveNumFramIs(RW_BOSS_FRAM_NUM);

	//boss vs ply
	if(SDLFunction::CheckCollision(ply_rect, boss_rect)   == true)
	{
		p_player->HandlePowerPlayer(DIE_VAL, effect_list);
	}


	//bullet' boss vs player
	for(int bbl = 0; bbl < RW_BOSS_BULLET_NUM; bbl++)
	{
		if(SDLFunction::CheckCollision(   ply_rect,
										  boss_bullet[bbl].GetRectWhileHaveNumFramIs(boss_bullet[bbl].Get_bullet_fram_num())   )   
										  == true )
		{
			boss_bullet[bbl].Set_is_move(false);
			boss_bullet[bbl].Set_position(-100, -100);
			boss_bullet[bbl].SetRect(this->rect_.x, this->rect_.y);
			reload_bullet_val_[bbl] = RW_BOSS_RELOAD_BULLET + RW_BOSS_BULLET_NUM*RW_BOSS_SHOT_INTERVEL*bbl;

			boss_bullet[bbl].Set_x_val(0);
			boss_bullet[bbl].Set_y_val(0);

			p_player->HandlePowerPlayer(boss_bullet[bbl].Get_bullet_dmg(), effect_list);
		}
	}


	//bullet's player vs boss
	std::vector<BulletObject*> p_bullet_list = p_player->Get_bullet_list();

	for(int pbl = 0; pbl < p_bullet_list.size(); pbl ++)
	{
		BulletObject* p_bullet = p_bullet_list.at(pbl);
		if(p_bullet != NULL)
		{
			if(SDLFunction::CheckCollision(   p_bullet->GetRectWhileHaveNumFramIs(p_bullet->Get_bullet_fram_num()),
											  boss_rect   )
											  == true)
			{

				p_bullet->Set_is_move(false);
				

				//handel boss mode-hp
				hits_counter_--;
				if(hits_counter_ > 0)
				{
					this->hp_ -= p_bullet->Get_bullet_dmg();
				}
				else if(hits_counter_ == 0)
				{
					shield_duration_ = RW_BOSS_SHIELD_DURATION;

					is_shut_rocket_ = true;
					is_RePos_ = true;
				}


				//handle_boss_die
				if(hp_ <= 0)
				{
					effect_list.at(FLAST_EFFECT)->SetRect_PositionEffect(this->GetRectWhileHaveNumFramIs(RW_BOSS_FRAM_NUM),
																		 this->x_pos_, this->y_pos_);
					effect_list.at(FLAST_EFFECT)->Set_is_show(true);

					SDL_Rect screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
					effect_list.at(WINGAME_EFFECT)->SetRect_PositionEffect(screen, MAX_MAP_X*TILE_SIZE - (SCREEN_WIDTH), 0 );
					effect_list.at(WINGAME_EFFECT)->Set_is_show(true);
				}
			}
		}
	}
}

void RocketWardenBoss::Show(SDL_Renderer* des)
{
	fram_++;
	if(fram_ >= RW_BOSS_FRAM_NUM)
	{
		fram_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect Renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

	int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &Renderquad);
}

void RocketWardenBoss::Set_Clip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < RW_BOSS_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;
		}
	}
}