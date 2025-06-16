
#include "DestructionTankBoss.h"
#include "PlayerObject.h"

DestructionTankBoss::DestructionTankBoss()
{
	x_pos_ = DT_TANK_BOSS_X_POS;
	y_pos_ = 0.0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	map_x_ = 0;
	map_y_ = 0;
	hp_ = DT_TANK_BOSS_HP;
	bullet_type_ = PRO_TYPE;
	driver_bullet_type_ = ROUND_TYPE;
	on_ground_ = false;
	dir_ = Dir_LEFT;
	fram_ = 0;
	is_run_DestructionTankBoss_ = false;
	reload_bullet_time_ = 0;
	reload_driver_bullet_time_ = 0;
	regenhp_ = DT_TANK_BOSS_REGEN_HP_TIME;
}

DestructionTankBoss::~DestructionTankBoss()
{
	;
}

void DestructionTankBoss::Init_boss(SDL_Renderer* des)
{
	LoadAnimation(tank_left_img, des, DT_TANK_BOSS_FRAM_NUM);
	icon_bosshp_.InitHpBoss(des);
	boss_icon_.InitIconBoss(tankboss_icon_img, des);
	MakeBullet(des);
	Set_clip(DT_TANK_BOSS_FRAM_NUM);
}

void DestructionTankBoss::HandleBoss(Map& map_data, PlayerObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
{
	Set_map_x_y(map_data.star_x_, map_data.star_y_);

	HandleMove(map_data, player, des, list_effect);

	CheckToMap(map_data);

	CheckColision(player, list_effect);

	icon_bosshp_.update_show_hp_boss(hp_, des);

	boss_icon_.ShowIconBoss(des);

	RegenHP();

	HandleBullet(player, des, list_effect);

	ShowBullet(des);

	ShowBoss(des);
}

void DestructionTankBoss::HandleMove(Map& map_data, PlayerObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
{
	
	//handle speed
	y_val_ += GRAVITY_SPEED;
	if(on_ground_ == true)
	{
		y_val_ = 0;
	}
	
	//handle GiveSpace
	float player_left = player.GetRect().x;
	float player_right = player_left + player.GetWidthFram();

	float boss_left = this->rect_.x;
	float boss_right = this->rect_.x + width_fram_;
	float boss_give_space_left = boss_left - DT_TANK_BOSS_GIVE_SPACE;
	float boss_give_space_right = boss_right + DT_TANK_BOSS_GIVE_SPACE;


	if(player_left > boss_right)
	{
		dir_ = Dir_RIGHT;

		if(player_left > boss_give_space_right)
		{
			x_val_ = DT_TANK_BOSS_SPEED;
		}
		else {x_val_ = 0;}
	}
	else if(player_right < boss_left)
	{
		dir_ = Dir_LEFT;

		if(player_right < boss_give_space_left)
		{
			x_val_ = -DT_TANK_BOSS_SPEED;
		}
		else {x_val_ = 0;}
	}

	//UpDate_IMG
	if(dir_ == Dir_LEFT)
	{
		LoadAnimation(tank_left_img, des, DT_TANK_BOSS_FRAM_NUM);
	}
	else if(dir_ == Dir_RIGHT)
	{
		LoadAnimation(tank_right_img, des, DT_TANK_BOSS_FRAM_NUM);
	}
}

void DestructionTankBoss::ShowBoss(SDL_Renderer* des)
{

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	fram_++;
	if(fram_ >= DT_TANK_BOSS_FRAM_NUM)
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

void DestructionTankBoss::CheckColision(PlayerObject& player, std::vector<EffectObject*> list_effect)
{
	//collision
	bool ply_vs_boss2 = SDLFunction::CheckCollision(                               //ply_vs_boss
						player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM),
						this->GetRectWhileHaveNumFramIs(DT_TANK_BOSS_FRAM_NUM)		);
	if(ply_vs_boss2 == true)
	{
		player.HandlePowerPlayer(DIE_VAL, list_effect);
	}

	std::vector<BulletObject*> ply_bullet_list = player.Get_bullet_list();         //ply_bullet vs boss
	for(int p_b = 0; p_b < ply_bullet_list.size(); p_b++)
	{
		BulletObject* ply_bullet = ply_bullet_list.at(p_b);

		bool p_bulet_vs_boss2 = SDLFunction::CheckCollision(
								this->GetRectWhileHaveNumFramIs(DT_TANK_BOSS_FRAM_NUM),
								ply_bullet->GetRectWhileHaveNumFramIs( ply_bullet->Get_bullet_fram_num() )		);

		if(p_bulet_vs_boss2 == true)
		{
			ply_bullet->Set_is_move(false);

			hp_-= ply_bullet->Get_bullet_dmg();
			if(hp_ <= 0)
			{
				list_effect.at(FLAST_EFFECT)->Set_is_show(true);
				list_effect.at(FLAST_EFFECT)->SetRect_PositionEffect(   GetRectWhileHaveNumFramIs(DT_TANK_BOSS_FRAM_NUM),
																		this->x_pos_, this->y_pos_   );

				SDL_Rect screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
				list_effect.at(WINGAME_EFFECT)->SetRect_PositionEffect(screen, MAX_MAP_X*TILE_SIZE - (SCREEN_WIDTH), 0 );
				list_effect.at(WINGAME_EFFECT)->Set_is_show(true);
				
				is_run_DestructionTankBoss_ = false;
			}
		}
	}
}

void DestructionTankBoss::MakeBullet(SDL_Renderer* des)
{
	boss_bullet_.SetDataBullet(bullet_type_);
	boss_bullet_.LoadAnimation(boss_bullet_.Get_img_bullet_type(dir_), des, boss_bullet_.Get_bullet_fram_num());
	boss_bullet_.Set_bullet_fram( boss_bullet_.Get_bullet_fram_num() );
	boss_bullet_.Set_bullet_dir(Dir_LEFT);
	boss_bullet_.Set_x_val(DT_TANK_BOSS_BULLET_SPEED_X);
	boss_bullet_.SetRect(this->x_pos_, SCREEN_HEIGHT*4/5);
	boss_bullet_.Set_is_move(true);

	driver_bullet_.SetDataBullet(driver_bullet_type_);
	driver_bullet_.LoadAnimation(driver_bullet_.Get_img_bullet_type(dir_), des, driver_bullet_.Get_bullet_fram_num());
	driver_bullet_.Set_bullet_dir(Dir_UP);
	driver_bullet_.Set_y_val(DIRVER_BULLET_SPEED_Y);
}

void DestructionTankBoss::HandleBullet(PlayerObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
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
										  player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM),
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
				Mix_PlayChannel(-1, g_audio.at(SHOOT_PROBULLET_SOUND), 0);
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
											player.GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM),
											driver_bullet_.GetRectWhileHaveNumFramIs( driver_bullet_.Get_bullet_fram_num() )
										);
			if(ply_vs_driver_bullet == true)
			{
				driver_bullet_.Set_is_move(false);
				player.HandlePowerPlayer(driver_bullet_.Get_bullet_dmg(), list_effect);

				list_effect.at(FIRE_EFFECT)->Set_is_show(true);
				list_effect.at(FIRE_EFFECT)->SetRect_PositionEffect(   player.GetRectWhileHaveNumFramIs (PLAYER_FRAM_NUM),
																	   player.Get_x_pos(), player.Get_y_pos()   );

				Mix_PlayChannel(-1, g_audio.at(FIRE_SOUND), 0);
			}

			//reload_driver_bullet
			if(driver_bullet_.Get_is_move() == false)
			{
				reload_driver_bullet_time_ = DRIVER_TIME_RELOAD_BULLET;
				
			}

		}
		else
		{
			reload_driver_bullet_time_--;
		}
	}
}

void DestructionTankBoss::ShowBullet(SDL_Renderer* des)
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

void DestructionTankBoss::ReLoadTankBullet(SDL_Renderer* des)
{
	//ReShowBullet
	if(boss_bullet_.Get_is_move() == false)
	{
		if(dir_ == Dir_LEFT)
		{
			boss_bullet_.Set_position(this->x_pos_ + boss_bullet_.Get_width_Bullet(), this->y_pos_ + this->height_fram_*0.625);
			boss_bullet_.Set_bullet_dir(Dir_LEFT);
		}
		else if(dir_ == Dir_RIGHT)
		{
			boss_bullet_.Set_position(this->x_pos_ + width_fram_ - boss_bullet_.Get_width_Bullet(), this->y_pos_ + this->height_fram_*0.625);
			boss_bullet_.Set_bullet_dir(Dir_RIGHT);
		}

		boss_bullet_.LoadAnimation(boss_bullet_.Get_img_bullet_type(dir_), des, boss_bullet_.Get_bullet_fram_num());

		reload_bullet_time_ = DRIVER_TIME_RELOAD_BULLET;
		boss_bullet_.Set_is_move(true);
	}
}

void DestructionTankBoss::CheckToMap(Map& map_data)
{

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

void DestructionTankBoss::Set_clip(const int& fram_num)
{
	for(int i = 0; i < fram_num; i++)
	{
		clip_[i].x = width_fram_*i;
		clip_[i].y = 0;
		clip_[i].w = width_fram_;
		clip_[i].h = height_fram_;
	}
}

void DestructionTankBoss::RegenHP()
{
	if(hp_ <= DT_TANK_BOSS_HP*0.4)
	{
		regenhp_--;
		if(regenhp_ <= 0)
		{
			regenhp_ = DT_TANK_BOSS_REGEN_HP_TIME;
			hp_ += DT_TANK_BOSS_REGEN_HP_VAL;
		}
	}
}
