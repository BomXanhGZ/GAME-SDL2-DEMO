
#include "BossObject.h"
#include "MainObject.h"

BossObject2::BossObject2()
{
	x_pos_ = BOSS_2_X_POS;
	y_pos_ = 0.0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	map_x_ = 0;
	map_y_ = 0;
	boss_hp_ = BOSS_2_HP;
	bullet_type_ = PRO_TYPE;
	driver_bullet_type_ = ROUND_TYPE;
	on_ground_ = false;
	dir_ = Dir_LEFT;
	fram_ = 0;
	is_run_boss_level_2_ = false;
	reload_bullet_time_ = 0;
	reload_driver_bullet_time_ = 0;
	regenhp_ = REGEN_HP_TIME;
}

BossObject2::~BossObject2()
{
	;
}

void BossObject2::Init_boss(SDL_Renderer* des)
{
	LoadAnimation(tank_left_img, des, BOSS_2_FRAM_NUM);
	icon_bosshp_.InitHpBoss(des);
	MakeBullet(des);
	Set_clip(BOSS_2_FRAM_NUM);
}

void BossObject2::HandleBoss(Map& map_data, MainObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
{
	Set_map_x_y(map_data.star_x_, map_data.star_y_);

	y_val_ += GRAVITY_SPEED;
	if(on_ground_ == true)
	{
		y_val_ = 0;
	}

	GiveSpace(player);
	UpDateImg(des);
	CheckToMap(map_data);
	icon_bosshp_.update_show_hp_boss(boss_hp_, rect_, des);
	RegenHP();

	//collision
	bool ply_vs_boss2 = SDLFunction::CheckCollision(                               //ply_vs_boss2
						player.GetRectWhileHaveNumFramIs(MAIN_FRAM_NUM),
						this->GetRectWhileHaveNumFramIs(BOSS_2_FRAM_NUM)		);
	if(ply_vs_boss2 == true)
	{
		player.HandlePowerPlayer(DIE_VAL, list_effect);
	}

	std::vector<BulletObject*> ply_bullet_list = player.Get_bullet_list();         //ply_bullet
	for(int p_b = 0; p_b < ply_bullet_list.size(); p_b++)
	{
		BulletObject* ply_bullet = ply_bullet_list.at(p_b);

		bool p_bulet_vs_boss2 = SDLFunction::CheckCollision(
								this->GetRectWhileHaveNumFramIs(BOSS_2_FRAM_NUM),
								ply_bullet->GetRectWhileHaveNumFramIs( ply_bullet->Get_bullet_fram_num() )		);

		if(p_bulet_vs_boss2 == true)
		{
			ply_bullet->Set_is_move(false);

			boss_hp_-= ply_bullet->Get_bullet_dmg();
			if(boss_hp_ <= 0)
			{
				list_effect.at(FLAST_EFFECT)->Set_is_show(true);
				list_effect.at(FLAST_EFFECT)->SetRect_PositionEffect(   GetRectWhileHaveNumFramIs(BOSS_2_FRAM_NUM),
																		this->x_pos_, this->y_pos_   );

				Mix_PlayChannel(-1, g_another_sound[1], 0);
				Mix_PlayChannel(-1, g_another_sound[0], 0);

				is_run_boss_level_2_ = false;
			}
		}
	}
}


void BossObject2::ShowBoss(SDL_Renderer* des)
{

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	fram_++;
	if(fram_ >= BOSS_2_FRAM_NUM)
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

void BossObject2::GiveSpace(MainObject& player)
{
	float player_left = player.GetRect().x;
	float player_right = player_left + player.GetWidthFram();

	float boss_left = this->rect_.x;
	float boss_right = this->rect_.x + width_fram_;
	float boss_give_space_left = boss_left - BOSS_2_GIVE_SPACE;
	float boss_give_space_right = boss_right + BOSS_2_GIVE_SPACE;


	if(player_left > boss_right)
	{
		dir_ = Dir_RIGHT;

		if(player_left > boss_give_space_right)
		{
			x_val_ = BOSS_2_SPEED;
		}
		else {x_val_ = 0;}
	}
	else if(player_right < boss_left)
	{
		dir_ = Dir_LEFT;

		if(player_right < boss_give_space_left)
		{
			x_val_ = -BOSS_2_SPEED;
		}
		else {x_val_ = 0;}
	}
}

void BossObject2::UpDateImg(SDL_Renderer* des)
{
	if(dir_ == Dir_LEFT)
	{
		LoadAnimation(tank_left_img, des, BOSS_2_FRAM_NUM);
	}
	else if(dir_ == Dir_RIGHT)
	{
		LoadAnimation(tank_right_img, des, BOSS_2_FRAM_NUM);
	}
}

void BossObject2::MakeBullet(SDL_Renderer* des)
{
	boss_bullet_.SetDataBullet(bullet_type_);
	boss_bullet_.LoadAnimation(boss_bullet_.Get_img_bullet_type(dir_), des, boss_bullet_.Get_bullet_fram_num());
	boss_bullet_.Set_bullet_fram( boss_bullet_.Get_bullet_fram_num() );
	boss_bullet_.Set_bullet_dir(Dir_LEFT);
	boss_bullet_.Set_x_val(BOSS_2_BULLET_SPEED_X);
	boss_bullet_.SetRect(this->x_pos_, SCREEN_HEIGHT*4/5);
	boss_bullet_.Set_is_move(true);

	driver_bullet_.SetDataBullet(driver_bullet_type_);
	driver_bullet_.LoadAnimation(driver_bullet_.Get_img_bullet_type(dir_), des, driver_bullet_.Get_bullet_fram_num());
	driver_bullet_.Set_bullet_dir(Dir_UP);
	driver_bullet_.Set_y_val(DIRVER_BULLET_SPEED_Y);
}

void BossObject2::HandleBullet(MainObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
{
	if(on_ground_ == true)
	{
		//tank_bullet
		if(reload_bullet_time_ <= 0)
		{
			boss_bullet_.SetMapXY(this->map_x_, this->map_y_);
			boss_bullet_.HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);

			//check colision tank bullet
			bool ply_vs_boss_bullet = SDLFunction::CheckCollision
									 (
										  player.GetRectWhileHaveNumFramIs(MAIN_FRAM_NUM),
										  boss_bullet_.GetRectWhileHaveNumFramIs( boss_bullet_.Get_bullet_fram_num() )
									  );
			if(ply_vs_boss_bullet == true)
			{
				boss_bullet_.Set_is_move(false);
				player.HandlePowerPlayer(boss_bullet_.Get_bullet_dmg(), list_effect);
			}

			ReLoadTankBullet(des);

		}
		else
		{
			reload_bullet_time_--;

			if(reload_bullet_time_ == 0)
			{
				Mix_PlayChannel(-1, g_bullet_sound[1], 0);
			}
		}



		//driver_bullet
		if(reload_driver_bullet_time_ <= 0)
		{
			if(driver_bullet_.Get_is_move() == false)
			{
				if(dir_ == Dir_LEFT)
				{
					if(this->rect_.x + this->width_fram_*0.2 <= player.GetRect().x)
					{
						driver_bullet_.SetRect(0, 0);
						driver_bullet_.Set_position(player.Get_x_pos(), this->y_pos_ + height_fram_*1/5); 
						driver_bullet_.Set_is_move(true);
					}
				}
				else if(dir_ == Dir_RIGHT)
				{
					if(this->rect_.x + this->width_fram_*0.5 >= player.GetRect().x)
					{
						driver_bullet_.SetRect(0, 0);
						driver_bullet_.Set_position(player.Get_x_pos(), this->y_pos_ + height_fram_*1/5);
						driver_bullet_.Set_is_move(true);
					}
				}
			}
		

			driver_bullet_.SetMapXY(this->map_x_, this->map_y_);
			driver_bullet_.HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);

			//check collision driver bullet
			bool ply_vs_driver_bullet = SDLFunction::CheckCollision
										(
											player.GetRectWhileHaveNumFramIs(MAIN_FRAM_NUM),
											driver_bullet_.GetRectWhileHaveNumFramIs( driver_bullet_.Get_bullet_fram_num() )
										);
			if(ply_vs_driver_bullet == true)
			{
				driver_bullet_.Set_is_move(false);
				player.HandlePowerPlayer(driver_bullet_.Get_bullet_dmg(), list_effect);

				list_effect.at(FIRE_EFFECT)->Set_is_show(true);
				list_effect.at(FIRE_EFFECT)->SetRect_PositionEffect(   player.GetRectWhileHaveNumFramIs (MAIN_FRAM_NUM),
																	   player.Get_x_pos(), player.Get_y_pos()   );

				Mix_PlayChannel(-1, g_another_sound[2], 0);
			}

			//reload_driver_bullet
			if(driver_bullet_.Get_is_move() == false)
			{
				reload_driver_bullet_time_ = TIME_RELOAD_DRIVER_BULLET;
				
			}

		}
		else
		{
			reload_driver_bullet_time_--;
		}
	}
}

void BossObject2::ShowBullet(SDL_Renderer* des)
{
	if(on_ground_)
	{
		if(reload_bullet_time_ <= 0)
		{
			boss_bullet_.ShowBullet(des);
		}

		if(reload_driver_bullet_time_ <= 0)
		{
			driver_bullet_.ShowBullet(des);
		}
	}
}

void BossObject2::ReLoadTankBullet(SDL_Renderer* des)
{
	//ReShowBullet
	if(boss_bullet_.Get_is_move() == false)
	{
		if(dir_ == Dir_LEFT)
		{
			boss_bullet_.Set_position(this->x_pos_ - boss_bullet_.Get_width_Bullet()*0.8, this->y_pos_ + this->height_fram_*0.8);
			boss_bullet_.Set_bullet_dir(Dir_LEFT);
		}
		else if(dir_ == Dir_RIGHT)
		{
			boss_bullet_.Set_position(this->x_pos_ + width_fram_ - boss_bullet_.Get_width_Bullet()*0.2, this->y_pos_ + this->height_fram_*0.8);
			boss_bullet_.Set_bullet_dir(Dir_RIGHT);
		}

		boss_bullet_.LoadAnimation(boss_bullet_.Get_img_bullet_type(dir_), des, boss_bullet_.Get_bullet_fram_num());

		reload_bullet_time_ = TIME_RELOAD_BULLET;
		boss_bullet_.Set_is_move(true);
	}
}

void BossObject2::CheckToMap(Map& map_data)
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
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

}

void BossObject2::Set_clip(const int& fram_num)
{
	for(int i = 0; i < fram_num; i++)
	{
		clip_[i].x = width_fram_*i;
		clip_[i].y = 0;
		clip_[i].w = width_fram_;
		clip_[i].h = height_fram_;
	}
}

void BossObject2::RegenHP()
{
	if(boss_hp_ <= BOSS_2_HP*0.4)
	{
		regenhp_--;
		if(regenhp_ <= 0)
		{
			regenhp_ = REGEN_HP_TIME;
			boss_hp_ += REGEN_HP_VAL;
		}
	}
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



void IconBossHp::InitHpBoss(SDL_Renderer* des)
{
	LoadAnimation(boss_hp_img, des, 1);
}

void IconBossHp::update_show_hp_boss(float& hp_boss, SDL_Rect& rect_boss, SDL_Renderer* des)
{
	for(int i = 0; i < hp_boss; i++)
	{
		pos_show_hp_[i].x = rect_boss.x + ((rect_boss.w/BOSS_2_FRAM_NUM)*0.5 - (hp_boss*width_fram_*0.5) + (i*width_fram_));
		pos_show_hp_[i].y = rect_boss.y - height_fram_; 
		pos_show_hp_[i].w = width_fram_;
		pos_show_hp_[i].h = height_fram_;

		int check = SDL_RenderCopy(des, p_object_, NULL, &pos_show_hp_[i]);
	}
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////




BossObject1::BossObject1()
{
	rect_.x = 0;
	x_pos_ = BOSS_1_X_POS;
	y_pos_ = 0.0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	map_x_ = 0;
	map_y_ = 0;
	boss_hp_ = BOSS_1_HP;
	bullet_type_ = BALL_TYPE;
	change_angle_ = true;
	on_ground_ = false;
	fram_ = 0;
}

BossObject1::~BossObject1()
{
	;
}

void BossObject1::Init_boss(SDL_Renderer* des)
{
	LoadAnimation(stone_statue_boss_img, des, BOSS_1_FRAM_NUM);
	MakeBullet(des);
	Set_clip(BOSS_1_FRAM_NUM);
}

void BossObject1::HandleBoss(Map& map_data, MainObject& player, std::vector<EffectObject*> list_effect)
{
	Set_map_x_y(map_data.star_x_, map_data.star_y_);

	y_val_ += GRAVITY_SPEED;
	if(on_ground_ == true)
	{
		y_val_ = 0;
	}

	CheckToMap(map_data);



	//check colision
	bool ply_vs_boss = SDLFunction::CheckCollision(                                //ply_vs_boss
					   player.GetRectWhileHaveNumFramIs(MAIN_FRAM_NUM), 
					   GetRectWhileHaveNumFramIs(BOSS_1_FRAM_NUM)		);
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
								  GetRectWhileHaveNumFramIs(BOSS_1_FRAM_NUM)		);
		{
			if(ply_bullet_vs_boss == true)
			{
				p_bullet->Set_is_move(false);

				boss_hp_ -= p_bullet->Get_bullet_dmg();

				if(boss_hp_ <= 0)
				{
					int a = 2;

					list_effect.at(FLAST_EFFECT)->Set_is_show(true);
					list_effect.at(FLAST_EFFECT)->SetRect_PositionEffect(   GetRectWhileHaveNumFramIs(BOSS_1_FRAM_NUM),
																			this->x_pos_, this->y_pos_  );

					Mix_PlayChannel(-1, g_another_sound[0], 0);

					//chou_hazeru
				}
			}
		}
	}
}


void BossObject1::ShowBoss(SDL_Renderer* des)
{

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	fram_++;
	if(fram_ >= BOSS_1_FRAM_NUM)
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

void BossObject1::MakeBullet(SDL_Renderer* des)
{
	for(int i = 0; i < BOSS_1_BULLET_NUM; i++)
	{
		BulletObject* boss_bullet = new BulletObject;
		boss_bullet->SetDataBullet(bullet_type_);
		boss_bullet->LoadAnimation(boss_bullet->Get_img_bullet_type(1), des, boss_bullet->Get_bullet_fram_num());
		boss_bullet->Set_is_move(true);
		boss_bullet_list.push_back(boss_bullet);
	}
}

void BossObject1::HandleBullet(MainObject& player, std::vector<EffectObject*> list_effect)
{

	float boss_bullet_x_speed = BOSS_1_BULLET_SPEED_X;
	float boss_bullet_y_speed = BOSS_1_BULLET_SPEED_Y;

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
									  player.GetRectWhileHaveNumFramIs(MAIN_FRAM_NUM),
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

void BossObject1::ShowBullet(SDL_Renderer* des)
{
	for(int i = 0; i < boss_bullet_list.size(); i++)
	{
		BulletObject* boss_bullet = boss_bullet_list.at(i);
		boss_bullet->ShowBullet(des);
	}
}

void BossObject1::CheckToMap(Map& map_data)
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

void BossObject1::Set_clip(const int& fram_num)
{
	for(int i = 0; i < fram_num; i++)
	{
		clip_[i].x = width_fram_*i;
		clip_[i].y = 0;
		clip_[i].w = width_fram_;
		clip_[i].h = height_fram_;
	}
}