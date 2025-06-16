
#include "DinoThreat.h"
#include "PlayerObject.h"

DinoThreat::DinoThreat()
{
	map_x_ = 0;
	map_y_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;

	fram_ = 0;
	dir_ = Dir_LEFT;
	hp_ = DINO_THREAT_HP;
	dino_reattack_ = DINP_REATTACK_VAL;
	come_back_ = false;
	on_ground_ = false;
	is_Handle_DinoThreat_ = true;
}

DinoThreat::~DinoThreat()
{

}

void DinoThreat::InitDinoThreat(SDL_Renderer* des)
{
	Make_DinoPosList();

	LoadAnimation(death_dino_left_img, des, DINO_THREAT_FRAM_NUM);
	Set_clip();
	Set_x_pos(DINO_POS_1);
	Set_y_pos(Y_START);

	check_obstacle_tile_.Set_obstacle_tile();
}

void DinoThreat::HandleDinoThreat(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list)
{
	Set_mapXY(g_map->star_x_, g_map->star_y_);

	DoThreat(des, player, effect_list);

	CheckToMap(g_map);
	
	Show(des);
}

void DinoThreat::Show(SDL_Renderer* des)
{
	fram_++;
	if(fram_ >= DINO_THREAT_FRAM_NUM)
	{
		fram_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect Renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

	int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &Renderquad);
}

void DinoThreat::Set_clip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < DINO_THREAT_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;
		}
	}
}

void DinoThreat::CheckToMap(Map* g_map)
{
	if(on_ground_ == false)
	{
		y_val_ += GRAVITY_SPEED;

		if(y_val_ >= MAX_FALL_SPEED)
		{
			y_val_ = MAX_FALL_SPEED;
		}
	}
	else
	{
		y_val_ = 0;
	}


	int x1,x2,y1,y2;
	std::vector<int> obstacle_tile = check_obstacle_tile_.Get_obstancle_tile();

	int height_min = height_fram_ < TILE_SIZE ? height_fram_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_fram_) / TILE_SIZE;

	y1 = y_pos_ / TILE_SIZE;
	y2 = (y_pos_ + height_min -1) / TILE_SIZE;

	if(x1 > 0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
	{
		if(x_val_ < 0)
		{
			int check_left_up = g_map->tile[y1][x1];
			int check_left_down = g_map->tile[y2][x1];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_left_up) != obstacle_tile.end()
			&& std::find(obstacle_tile.begin(), obstacle_tile.end(), check_left_down) != obstacle_tile.end())
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
			}
		}
		else if(x_val_ >  0)
		{

			int check_right_up = g_map->tile[y1][x2];
			int check_right_down = g_map->tile[y2][x2];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_right_up) != obstacle_tile.end()
			&& std::find(obstacle_tile.begin(), obstacle_tile.end(), check_right_down) != obstacle_tile.end())
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= (width_fram_); // -1
				x_val_ = 0;
			}
		}
	}

	//ventical
	int width_min = width_fram_ < TILE_SIZE ? width_fram_ :	TILE_SIZE;

	x1 = x_pos_ / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_fram_ -1) / TILE_SIZE;

	if(x1 > 0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
	{
		if(y_val_ < 0)
		{
			int check_up_left = g_map->tile[y1][x1];
			int check_up_right =g_map->tile[y1][x2];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_up_left) != obstacle_tile.end()
			|| std::find(obstacle_tile.begin(), obstacle_tile.end(), check_up_right) != obstacle_tile.end())
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
		else if(y_val_ >= 0)
		{
			int check_down_left = g_map->tile[y2][x1];
			int check_down_right = g_map->tile[y2][x2];

			if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_down_left) != obstacle_tile.end()
			|| std::find(obstacle_tile.begin(), obstacle_tile.end(), check_down_right) != obstacle_tile.end())
			{
				y_pos_ = y2 * TILE_SIZE +1 -height_fram_;
				y_val_ = 0;
				on_ground_ = true;
			}
			else
			{
				on_ground_ = false;
			}
		}
	}

	if (y2 >= 1.05 * MAX_MAP_Y)
	{
		RestartThreat();
	}
	else if(this->rect_.x < -SCREEN_WIDTH*0.25)
	{
		RestartThreat();
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;
}

void DinoThreat::DoThreat(SDL_Renderer* des, PlayerObject* player, std::vector<EffectObject*> effect_list)
{

	SDL_Rect ply_rect = player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);
	float ply_pos_x = player->Get_x_pos();


	//dino move
	if(this->x_pos_ - map_x_ < SCREEN_WIDTH)
	{
		if(dir_ == Dir_LEFT)
		{
			x_val_ = -DINO_THREAT_SPEED;
		}
		else if(dir_ == Dir_RIGHT)
		{
			x_val_ = DINO_THREAT_SPEED;
		}
	}
	else
	{
		x_val_ = 0;
	}


	//dino chasing
	if(ply_pos_x < this->x_pos_ - DINO_MOVE_SPACE)
	{
		dir_ = Dir_LEFT;
		LoadAnimation(death_dino_left_img, des, DINO_THREAT_FRAM_NUM);
	}
	else if(ply_pos_x > this->x_pos_ + DINO_MOVE_SPACE)
	{
		dir_ = Dir_RIGHT;
		LoadAnimation(death_dino_right_img, des, DINO_THREAT_FRAM_NUM);
	}


	//dino come back
	if(come_back_ == true)
	{
		y_pos_ = Y_START;

		if(dino_poslist_.size() > 0)
		{
			x_pos_ = dino_poslist_.at(0);

			dino_poslist_.erase( dino_poslist_.begin() );
		}
		else
		{
			is_Handle_DinoThreat_ = false;
		}

		come_back_ = false;
	}


	//collide
	if(SDLFunction::CheckCollision(   ply_rect, this->GetRectWhileHaveNumFramIs(DINO_THREAT_FRAM_NUM)   )       //ply vs dino threat
								   ==    true)
	{
		if(dino_reattack_ <= 0)
		{
			player->HandlePowerPlayer(DINO_DMG_VAL, effect_list);
			this->hp_ -= 1;
			if(hp_ <= 0)
			{
				RestartThreat();
			}

			dino_reattack_ = DINP_REATTACK_VAL;
		}
		else
		{
			dino_reattack_--;
		}
	}

	std::vector<BulletObject*> ply_bulletlist = player->Get_bullet_list();										//bullet's player vs dino threat
	for(int pb = 0; pb < ply_bulletlist.size(); pb++)
	{
		BulletObject* ply_bullet = ply_bulletlist.at(pb);
		if(ply_bullet != NULL)
		{
			if(SDLFunction::CheckCollision(   ply_bullet->GetRectWhileHaveNumFramIs(ply_bullet->Get_bullet_fram_num()), 
											  this->GetRectWhileHaveNumFramIs(DINO_THREAT_FRAM_NUM)   )
											  ==  true)
			{

				ply_bullet->Set_is_move(false);

				hp_ -= ply_bullet->Get_bullet_dmg();
				if(hp_ <= 0)
				{
					RestartThreat();

					effect_list.at(EXP_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(DINO_THREAT_FRAM_NUM),
																		  this->x_pos_, this->y_pos_   );
					effect_list.at(EXP_EFFECT)->Set_is_show(true);
				}
			}
		}
	}
}

void DinoThreat::RestartThreat()
{
	hp_ = DINO_THREAT_HP;
	on_ground_ = false;
	come_back_ = true;
}

void DinoThreat::Make_DinoPosList()
{
	dino_poslist_.push_back(DINO_POS_2);
	dino_poslist_.push_back(DINO_POS_3);
	dino_poslist_.push_back(DINO_POS_4);
}
