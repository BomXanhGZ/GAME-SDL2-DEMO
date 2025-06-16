#include "PlayerObject.h"
#include "StaticThreat.h"

StaticThreat::StaticThreat()
{
	map_x_ = 0;
	map_y_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	hp_ = 0;
	fram_ = 0;
	dir_ = Dir_LEFT;
	is_shut_  = false;
	is_translate_ = false;
	on_ground_ = false;
	come_back_ = false;
	is_handle_static_threat_ = true;
	bullet_type_ = 0;
}

StaticThreat::~StaticThreat()
{
	;
}

std::vector<StaticThreat*> StaticThreat::InitStaticThreat(SDL_Renderer* des)
{

	Make_StaticThreat_pos_list();
	std::vector<StaticThreat*> static_threat_list;

	StaticThreat* p_static_threat = new StaticThreat[STATIC_THREAT_NUM];
	for(int st = 0; st < STATIC_THREAT_NUM; st++)
	{
		p_static_threat = (p_static_threat + st);

		if(p_static_threat != NULL)
		{
			p_static_threat->LoadAnimation(static_threat_left_img, des, STATIC_THREAT_FRAM_NUM);
			p_static_threat->Set_StaticThreatHP(STATIC_THREAT_HP);
			p_static_threat->Set_clip();
			p_static_threat->MakeBullet(des);

			float x_pos = STATIC_POS_1 - STATIC_POS_1*st + STATIC_POS_2*st;
			p_static_threat->Set_x_pos(x_pos);
			p_static_threat->Set_y_pos(Y_START);

			if(st == 1)
			{
				p_static_threat->Set_y_pos(TILE_SIZE*7.5);
			}
		}

		static_threat_list.push_back(p_static_threat);
	}

	return static_threat_list;
}


void StaticThreat::Handle(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list)
{
	Set_mapXY(g_map->star_x_, g_map->star_y_);

	CheckToMap(g_map);
	DoThreat(des, player, effect_list);  //col // re_pos
	HandleBullet(des, g_map);			//col

	Show(g_screen);
}

void StaticThreat::Show(SDL_Renderer* des)
{
	fram_++;
	if(fram_ >= STATIC_THREAT_FRAM_NUM)
	{
		fram_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect Renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

	int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &Renderquad);
}


void StaticThreat::Set_clip()
{
	for(int st = 0; st < STATIC_THREAT_FRAM_NUM; st++)
	{
		Fram_clip_[st].x = st *  width_fram_;
		Fram_clip_[st].y = 0;
		Fram_clip_[st].w = width_fram_;
		Fram_clip_[st].h = height_fram_;
	}
}

void StaticThreat::DoThreat(SDL_Renderer* des, PlayerObject* player, std::vector<EffectObject*> effect_list)
{

	SDL_Rect ply_rect = player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM); 



	//Re_Position
	if(come_back_ == true)
	{
		y_pos_ = Y_START;

		if(g_map_threat_pos_list_.size() > 0)
		{
			x_pos_ = *g_map_threat_pos_list_.begin();
			g_map_threat_pos_list_.erase(  g_map_threat_pos_list_.begin()  );
		}
		else
		{
			is_handle_static_threat_ = false;
		}

		come_back_ = false;
	}



	//Change_Dir
	if(ply_rect.x < this->rect_.x)
	{
		dir_ = Dir_LEFT;
		LoadAnimation(static_threat_left_img, des, STATIC_THREAT_FRAM_NUM);
	}
	else
	{
		dir_ = Dir_RIGHT;
		LoadAnimation(static_threat_right_img, des, STATIC_THREAT_FRAM_NUM);
	}



	//check_Collition

	if(SDLFunction::CheckCollision(ply_rect, this->GetRectWhileHaveNumFramIs(STATIC_THREAT_FRAM_NUM)) == true)					//ply vs threat
	{
		player->HandlePowerPlayer(DIE_VAL, effect_list);
		RestartThreat();

		effect_list.at(EXP_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(STATIC_THREAT_FRAM_NUM),
															  this->x_pos_, this->y_pos_   );
		effect_list.at(EXP_EFFECT)->Set_is_show(true);
	}

	
	for(int tb = 0; tb < STATIC_THREAT_BULLET_NUM; tb++)																		 //ply vs bullet's threat
	{
		SDL_Rect tb_rect =  bullet_[tb].GetRectWhileHaveNumFramIs(  bullet_[tb].Get_bullet_fram_num()  );
		if(SDLFunction::CheckCollision(ply_rect, tb_rect) == true)
		{
			effect_list.at(ROCKET_EXP_EFFECT)->SetRect_PositionEffect(   player->GetRectWhileHaveNumFramIs(STATIC_THREAT_FRAM_NUM),
																		 player->Get_x_pos(), player->Get_y_pos()   );
			effect_list.at(ROCKET_EXP_EFFECT)->Set_is_show(true);

			player->HandlePowerPlayer(bullet_[tb].Get_bullet_dmg(), effect_list) ; //todo
			bullet_[tb].SetReloadBullet();
			bullet_[tb].Set_position(-500, -500);
		}
	}

	
	std::vector<BulletObject*> ply_bulletlist = player->Get_bullet_list();														//bullet's ply vs threat
	for(int pb = 0; pb < ply_bulletlist.size(); pb++)
	{
		BulletObject* ply_bullet = ply_bulletlist.at(pb);
		if(ply_bullet != NULL)
		{
			if(SDLFunction::CheckCollision(   ply_bullet->GetRectWhileHaveNumFramIs(  ply_bullet->Get_bullet_fram_num()  ),
											  this->GetRectWhileHaveNumFramIs(STATIC_THREAT_FRAM_NUM)   )
											  == true)
			{
				ply_bullet->Set_is_move(false);
				this->hp_ -= ply_bullet->Get_bullet_dmg();
				if(hp_ <= 0)
				{
					effect_list.at(EXP_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(STATIC_THREAT_FRAM_NUM),
																				 this->x_pos_, this->y_pos_   );
					effect_list.at(EXP_EFFECT)->Set_is_show(true);
					come_back_ = true;
					hp_ = STATIC_THREAT_HP;
				}
			}
		}
	}
}

void StaticThreat::MakeBullet(SDL_Renderer* des )
{
	for(int b = 0; b < STATIC_THREAT_BULLET_NUM; b++)
	{
		Set_BulletType(ROCKET_TYPE);
		bullet_[b].SetDataBullet( this->Get_BulletType() );
		bullet_[b].LoadAnimation(   bullet_[b].Get_img_bullet_type( Dir_LEFT), des, bullet_[b].Get_bullet_fram_num()   );
		bullet_[b].Set_x_val(STATIC_THREAT_BULLET_SPEED_X);
		bullet_[b].Set_y_val(STATIC_THREAT_BULLET_SPEED_Y);
	}
}

void StaticThreat::HandleBullet(SDL_Renderer* des, Map* g_map)
{
	for(int b = 0; b < STATIC_THREAT_BULLET_NUM; b++)
	{
		bullet_[b].SetMapXY(g_map->star_x_, g_map->star_y_);

		//HandleBullet
		if(bullet_[b].Get_is_move() == true && bullet_[b].GetReloadBullet() <= 0)
		{
			bullet_[b].HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);

			if(bullet_[b].Get_is_move() == false)
			{
				bullet_[b].SetReloadBullet();
			}
		}

		//ResetBullet
		else if(bullet_[b].GetReloadBullet() <= 0)
		{
			if(this->dir_ == Dir_LEFT)
			{
				if(b == 0)
				{
					bullet_[b].Set_bullet_dir(Dir_LEFT_DOWN);
				}
				else if(b == 1)
				{
					bullet_[b].Set_bullet_dir(Dir_LEFT);
				}
				else if(b == 2)
				{
					bullet_[b].Set_bullet_dir(Dir_LEFT_UP);
				}
			}
			else if(this->dir_ == Dir_RIGHT)
			{
				if(b == 0)
				{
					bullet_[b].Set_bullet_dir(Dir_RIGHT_DOWN);
				}
				else if(b == 1)
				{
					bullet_[b].Set_bullet_dir(Dir_RIGHT);
				}
				else if(b == 2)
				{
					bullet_[b].Set_bullet_dir(Dir_RIGHT_UP);
				}
			}

			bullet_[b].LoadAnimation(   bullet_[b].Get_img_bullet_type(this->dir_), des, bullet_[b].Get_bullet_fram_num()   );
			bullet_[b].Set_position(this->x_pos_ + this->width_fram_*0.5, this->y_pos_ + this->height_fram_*0.5);
			bullet_[b].Set_is_move(true);
		}
		else
		{
			bullet_[b].HandleReloadBullet();
		}

		bullet_[b].ShowBullet(des);
	}
}

void StaticThreat::CheckToMap(Map* g_map)
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

	int x1, x2, y1, y2;
	std::vector<int> obstacle_tile_ = check_obstacle_tile_.Get_obstancle_tile();
	
	
	//ventical
	int width_min = width_fram_ < TILE_SIZE ? width_fram_ : TILE_SIZE;

	x1 = x_pos_ / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = y_pos_ / TILE_SIZE;
	y2 = (y_pos_ + height_fram_ -1) / TILE_SIZE;

	if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if(y_val_ >= 0)
		{
			int check_down_left = g_map->tile[y2][x1];
			int check_down_right = g_map->tile[y2][x2];

			if(std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_down_left) != obstacle_tile_.end()
			|| std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_down_right) != obstacle_tile_.end())
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
		else if(y_val_ < 0)
		{
			int check_up_left = g_map->tile[y1][x1];
			int check_up_right = g_map->tile[y1][x2];

			if(std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_up_left) != obstacle_tile_.end()
			|| std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_up_right) != obstacle_tile_.end())
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
	}

	if (y2 >= 1.5 * MAX_MAP_Y)
	{
		RestartThreat();
	}
	else if(this->rect_.x < -SCREEN_WIDTH*0.3)
	{
		RestartThreat();
	}

	y_pos_ += y_val_;
}

void StaticThreat::RestartThreat()
{
	on_ground_ = false;
	hp_ = STATIC_THREAT_HP;
	come_back_ = true;
}

void StaticThreat::Make_StaticThreat_pos_list()
{
	if(g_map_threat_pos_list_.size() > 0)
	{
		for(int l = 0; l < g_map_threat_pos_list_.size(); l++)
		{
			g_map_threat_pos_list_.erase(g_map_threat_pos_list_.begin() + l);
		}
	}
	
	g_map_threat_pos_list_.push_back(STATIC_POS_3);
	g_map_threat_pos_list_.push_back(STATIC_POS_4);
	g_map_threat_pos_list_.push_back(STATIC_POS_5);
	g_map_threat_pos_list_.push_back(STATIC_POS_6);
}