
#include "GuardThreat.h"
#include "PlayerObject.h"

GuardThreat::GuardThreat()
{
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 400;
	y_pos_ = 175;
	map_x_ = 0;
	map_y_ = 0;

	come_back_ = false;
	is_handle_ = true;
	is_show_effect_ = false;
	on_ground_ = false;
	come_back_time_ = 0;
	move_space_ = 0;
	fram_ = 0;
	threat_hp_ = 0.0;
	dir_ = Dir_LEFT;
	reload_time_bullet_ = GUARD_THREAT_RELOAD_BULLET_TIME;
	thr_bullet_type_ = STANDERD_TYPE;
	map_num_ = -1;
}

GuardThreat::~GuardThreat()
{
	;
}


std::vector<GuardThreat*> GuardThreat::InitThreats(SDL_Renderer* des,const int& map_num)
{
	
	Make_guardthreat_pos_list(map_num);

	std::vector<GuardThreat*> threat_list;
	GuardThreat* threat_obj = new GuardThreat[GUARD_THREAT_NUM];
	for(int i = 0; i < GUARD_THREAT_NUM; i++)
	{
		if (threat_obj != NULL)
		{
			GuardThreat* p_threat = (threat_obj + i);

			p_threat->Set_MapNum(map_num);
			p_threat->LoadAnimation(standard_threat_left_img[map_num], des, GUARD_THREAT_FRAM_NUM);
			p_threat->Set_threat_hp(GUARD_THREAT_HP);
			p_threat->MakeOgBullet(des);
			p_threat->SetClip();

			float xpos = GetXposWithMapNum(i, map_num);
			float ypos = Y_START;
			p_threat->SetPosition(ypos, xpos);

			threat_list.push_back(p_threat);
		}
	}

	return threat_list;
}

void GuardThreat::HandleThreat(Map& map_data, SDL_Renderer* des, PlayerObject& player, std::vector<EffectObject*> list_effect)
{

	SDL_Rect player_rect = player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);

	SetMapXY(map_data.star_x_, map_data.star_y_);
	DoThreat(map_data, des);
	SDL_Rect threat_rect = GetRectWhileHaveNumFramIs(GUARD_THREAT_FRAM_NUM);

	ReloadOgBullet(des, player_rect);
	SDL_Rect t_bullet_rect = HandleOgBullet(des, SCREEN_WIDTH, SCREEN_HEIGHT, list_effect);


	//CHECK COLISSION
	bool Check_Ply_vs_Tbullet = SDLFunction::CheckCollision(t_bullet_rect, player_rect);
	if(Check_Ply_vs_Tbullet)
	{
		SetReloadBullet(GUARD_THREAT_RELOAD_BULLET_TIME, list_effect);
		player.HandlePowerPlayer( p_bullet.Get_bullet_dmg(), list_effect );  //dmg bullet threat;
	}

	std::vector<BulletObject*> ply_bullet_list = player.Get_bullet_list();
	for(int pbl = 0; pbl < ply_bullet_list.size(); pbl++)
	{
		BulletObject* ply_bullet = ply_bullet_list.at(pbl);
		if(ply_bullet != NULL)
		{
			SDL_Rect ply_bullet_rect = ply_bullet->GetRectWhileHaveNumFramIs(ply_bullet->Get_bullet_fram_num());

			//colistion bullet of ply vs threat
			bool Check_OG_Thr_vs_PlyBullet  = SDLFunction::CheckCollision(ply_bullet_rect, threat_rect);
			if(Check_OG_Thr_vs_PlyBullet)
			{
				int a = 2;
				ply_bullet->Set_is_move(false);
				if(threat_hp_ > 0) {threat_hp_ -= ply_bullet->Get_bullet_dmg(); }
				if(threat_hp_ <= 0) 
				{
					list_effect.at(0)->Set_is_show(true);
					list_effect.at(0)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(GUARD_THREAT_FRAM_NUM), 
																 this->x_pos_, this->y_pos_   );

					RestartThreat();

					if(g_OG_threat_pos_list_.size() <= 0)
					{
						come_back_time_ = GUARD_THREAT_CAMEBACK;
						rect_.y = -Y_START;
					}
				}
			}
		}
	}

	Show(des);
}

void GuardThreat::Show(SDL_Renderer* des)
{
	if (come_back_ == false)
	{
		fram_++;
		if(fram_ >= GUARD_THREAT_FRAM_NUM)
		{
			fram_ = 0;
		}

		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		SDL_Rect* curren_clip = &Fram_clip_[fram_];
		SDL_Rect renderquard = {rect_.x, rect_.y, width_fram_, height_fram_};
		int check = SDL_RenderCopy(des, p_object_, curren_clip, &renderquard);
		if(check == 0)
		{
			int a = 2;
		}
	}
}

void GuardThreat::DoThreat(Map& map_data, SDL_Renderer* des)
{
	if (come_back_ == false && threat_hp_ > 0)
	{
		x_val_ = 0;
		y_val_ += GRAVITY_SPEED;

		if(on_ground_ == true)
		{
			HandleMove(des);
		}

		CheckToMap(map_data);

		if(map_x_ > this->x_pos_ + SCREEN_WIDTH*0.4)
		{
			RestartThreat();
		}
	}

	else if (come_back_ == true && come_back_time_ <= 0)
	{
		x_val_ = 0;
		y_val_ = 0;

		come_back_ = false;
  		on_ground_ = false;

		if(g_OG_threat_pos_list_.size() > 0)
		{
 			x_pos_ = *g_OG_threat_pos_list_.begin();
			g_OG_threat_pos_list_.erase(g_OG_threat_pos_list_.begin());
		}
		else
		{
			is_handle_ = false;
		}
		
		y_pos_ = Y_START;

		dir_ = Dir_LEFT;
		move_space_ = 0;
	}

	else
	{
		come_back_time_--;
	}
}

void GuardThreat::CheckToMap(Map& map_data)
{
	
	if(on_ground_ == true)
	{
		y_val_ = 0;
	}
	if(y_val_ >= MAX_FALL_SPEED)
	{
		y_val_ = MAX_FALL_SPEED;
	}

	int x1, x2, y1, y2;
	std::vector<int> obstacle_tile_ = check_obstacle_tile_.Get_obstancle_tile();
	
	//horizontal
	int height_min = height_fram_ < TILE_SIZE ? height_fram_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_fram_) / TILE_SIZE;

	y1 = y_pos_ / TILE_SIZE;
	y2 = (y_pos_ + height_min -1) / TILE_SIZE;

	if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if(x_val_ > 0)
		{
			move_space_ += GUARD_THREAT_SPEED;					//= x_val

			int check_right_up = map_data.tile[y1][x2];
			int check_right_down = map_data.tile[y2][x2];

			if(std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_right_up) != obstacle_tile_.end()
			&& std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_right_down) != obstacle_tile_.end()
			|| move_space_ >= GUARD_THREAT_MOVE_SPACE * TILE_SIZE)
			{
				x_pos_ = x2 * TILE_SIZE - (width_fram_ + 1);
				x_val_ = 0;
				move_space_ = 0;
				dir_ = Dir_LEFT;
			}
		}

		else if(x_val_ < 0)
		{	
			move_space_ -= GUARD_THREAT_SPEED;

			int check_left_up = map_data.tile[y1][x1];
			int check_left_down = map_data.tile[y2][x1];

			if(std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_left_up) != obstacle_tile_.end()
			&& std::find(obstacle_tile_.begin(), obstacle_tile_.end(), check_left_down) != obstacle_tile_.end()
			|| move_space_ <= -GUARD_THREAT_MOVE_SPACE * TILE_SIZE)
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
				move_space_ = 0;
				dir_ = Dir_RIGHT;
			}
		}
	}
	


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
			int check_down_left = map_data.tile[y2][x1];
			int check_down_right = map_data.tile[y2][x2];

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
			int check_up_left = map_data.tile[y1][x1];
			int check_up_right = map_data.tile[y1][x2];

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

	x_pos_ += x_val_;
	y_pos_ += y_val_;
}

void GuardThreat::HandleMove(SDL_Renderer* des)
{
	if(dir_ == Dir_LEFT)
	{
		LoadAnimation(standard_threat_left_img[map_num_] , des, GUARD_THREAT_FRAM_NUM);
		x_val_ = -GUARD_THREAT_SPEED;
	}
	else if(dir_ == Dir_RIGHT)
	{
		LoadAnimation(standard_threat_right_img[map_num_] , des, GUARD_THREAT_FRAM_NUM);
		x_val_ = GUARD_THREAT_SPEED;
	}
}

void GuardThreat::MakeOgBullet(SDL_Renderer* des)
{
	if     (map_num_ == 0) {thr_bullet_type_ = STANDERD_TYPE; }
	else if(map_num_ == 1) {thr_bullet_type_ = STAR_TYPE; }
	else if(map_num_ == 2) {thr_bullet_type_ = ROUND_TYPE; }
	
	p_bullet.SetDataBullet(thr_bullet_type_);
	p_bullet.LoadAnimation(p_bullet.Get_img_bullet_type(dir_), des, 1);
}

void GuardThreat::ReloadOgBullet(SDL_Renderer* des, const SDL_Rect& player_rect)
{

	//rect space threat
	int left_t = this->rect_.x  - BULLET_SPACE_LIMIT;
	int right_t = this->rect_.x + width_fram_ + BULLET_SPACE_LIMIT;

	//rect main
	int left_m = player_rect.x;
	int right_m = player_rect.x + player_rect.w;

	if (right_m >= left_t && right_m <= right_t
	   ||left_m >= left_t && left_m <= right_t)
	{
		if(reload_time_bullet_ > 0)
		{
			reload_time_bullet_ -= 1;
		}

		if(on_ground_ 
		&& reload_time_bullet_ <= 0)
		{
			if(p_bullet.Get_is_move() != true)
			{

				if (dir_ == Dir_LEFT)  {p_bullet.Set_bullet_dir(Dir_LEFT); }    //tao ra vien dan co wark cua threat va dua vao list
		   else if (dir_ == Dir_RIGHT) {p_bullet.Set_bullet_dir(Dir_RIGHT); }

	   			p_bullet.Set_position(this->x_pos_ + this->width_fram_ * 0.5, this->y_pos_ + this->rect_.h * 0.5);
				p_bullet.Set_x_val(THREAT_BULLET_SPEED);
				p_bullet.Set_is_move(true);
			}
		}
	}
}

SDL_Rect GuardThreat::HandleOgBullet(SDL_Renderer* des, int x_bdr, int y_bdr, std::vector<EffectObject*> list_effect)
{
	if(reload_time_bullet_ <= 0)
	{
		
		if(p_bullet.Get_is_move() == true)
		{

			p_bullet.SetMapXY(this->map_x_, this->map_y_);
			p_bullet.HandleMove(x_bdr, y_bdr);
			if(p_bullet.Get_is_move() == false)
			{
				SetReloadBullet(GUARD_THREAT_RELOAD_BULLET_TIME, list_effect);
			}

			p_bullet.ShowBullet(des);
		}

		SDL_Rect bullet_rect = p_bullet.GetRectWhileHaveNumFramIs(1);

		return bullet_rect;
	}
}

void GuardThreat::RestartThreat()
{
	on_ground_ = false;
	threat_hp_ = GUARD_THREAT_HP;
	come_back_ = true;

	if(g_OG_threat_pos_list_.size() <= 0)
	{
		come_back_time_ = GUARD_THREAT_CAMEBACK;
	}
}

void GuardThreat::SetReloadBullet(const int& reload_time, std::vector<EffectObject*> list_effect)
{
	p_bullet.Set_is_move(false);
	p_bullet.SetRect(this->rect_.x, this->rect_.y);
	reload_time_bullet_ = reload_time;
}

void GuardThreat::Make_guardthreat_pos_list(const int& map_num)
{
	if(g_OG_threat_pos_list_.size() > 0)
	{
		for(int l = 0; l < g_OG_threat_pos_list_.size(); l++)
		{
			g_OG_threat_pos_list_.erase(g_OG_threat_pos_list_.begin() + l);
		}
	}

	if(map_num == MAP_1)
	{
		g_OG_threat_pos_list_.push_back(M1_POS_3);
		g_OG_threat_pos_list_.push_back(M1_POS_4);
		g_OG_threat_pos_list_.push_back(M1_POS_5);
		g_OG_threat_pos_list_.push_back(M1_POS_6);
		g_OG_threat_pos_list_.push_back(M1_POS_7);
		g_OG_threat_pos_list_.push_back(M1_POS_8);
		g_OG_threat_pos_list_.push_back(M1_POS_9);
		g_OG_threat_pos_list_.push_back(M1_POS_10);	
		g_OG_threat_pos_list_.push_back(M1_POS_WHILE_SHOW_BOSS_1);
		g_OG_threat_pos_list_.push_back(M1_POS_WHILE_SHOW_BOSS_2);
	}
	else if(map_num == MAP_2)
	{
		g_OG_threat_pos_list_.push_back(M2_POS_3);
		g_OG_threat_pos_list_.push_back(M2_POS_4);
		g_OG_threat_pos_list_.push_back(M2_POS_5);
		g_OG_threat_pos_list_.push_back(M2_POS_6);
		g_OG_threat_pos_list_.push_back(M2_POS_7);
	}
	else if(map_num == MAP_3)
	{
		g_OG_threat_pos_list_.push_back(M3_POS_3);
		g_OG_threat_pos_list_.push_back(M3_POS_4);
		g_OG_threat_pos_list_.push_back(M3_POS_5);
		g_OG_threat_pos_list_.push_back(M3_POS_6);
		g_OG_threat_pos_list_.push_back(M3_POS_7);
		g_OG_threat_pos_list_.push_back(M3_POS_8);
	}
}

float GuardThreat::GetXposWithMapNum(const int& idx, const int& map_num)
{

	float l_xpos = 0;

		if(map_num == MAP_1)
		{
			l_xpos = M1_POS_1 + idx*M1_POS_2 - idx*M1_POS_1;
		}
		else if(map_num == MAP_2)
		{
			l_xpos = M2_POS_1 + idx*M2_POS_2 - idx*M2_POS_1;
		}
		else if(map_num == MAP_3)
		{
			l_xpos = M3_POS_1 + idx*M3_POS_2 - idx*M3_POS_1;
		}

	return l_xpos;
}

void GuardThreat::SetClip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < GUARD_THREAT_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;

		}
	}
}