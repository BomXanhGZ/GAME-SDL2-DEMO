
#include "DeathEyeBoss.h"
#include "PlayerObject.h"

DeathEyeBoss::DeathEyeBoss()
{
	map_x_ = 0;
	map_y_ = 0;
	x_pos_ = DEATH_EYE_BOSS_FIRST_POS;
	y_pos_ = Y_START;
	x_val_ = 0;
	y_val_ = 0;

	hp_ = DEATH_EYE_BOSS_HP;
	Dir_ = Dir_LEFT;
	fram_ = 0;
	hits_counter_ = 0;
	reload_laze_val_ = DEATH_EYE_BOSS_RELOAD_LAZE_VAL;
	charge_thuntherweapon_counter_ = 0;
	is_RePos_ = false;
	is_charge_laze_ = false;
	is_shut_ = false;
	is_attack_while_rage_ = false;
	is_RePos_ = false;
}

DeathEyeBoss::~DeathEyeBoss()
{
	;
}

void DeathEyeBoss::InitDeathEyeBoss(SDL_Renderer* des)
{
	srand(time(NULL));

	LoadAnimation(eye_boss_left, des, DEATH_EYE_BOSS_FRAM_NUM);
	Set_clip();
	icon_BossHP_.InitHpBoss(des);
	boss_icon_.InitIconBoss(eye_boss_icon_img, des);
	deatheye_lazebullet_.InitLazeBullet(des);
	deatheye_thunderweapon.InitThunderWeapon(des);
}

void DeathEyeBoss::HandleDeathEyeBoss(SDL_Renderer* des, Map& g_map, PlayerObject* p_player, std::vector<EffectObject*> effect_list)
{

	Set_mapXY(g_map.star_x_, g_map.star_y_);

	HandleMove(des, p_player, effect_list); //->get state weapon

	UpdateIMG(des);

	CheckCollision(p_player, effect_list);

	icon_BossHP_.update_show_hp_boss(this->hp_, des);

	boss_icon_.ShowIconBoss(des);

	HandleLazeBullet(des, effect_list);

	HandleThunderWeapon(des, &g_map, p_player, effect_list);

	Show(des);
}

void DeathEyeBoss::Show(SDL_Renderer* des)
{
	fram_++;
	if(fram_ >= DEATH_EYE_BOSS_FRAM_NUM)
	{
		fram_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect Renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

	int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &Renderquad);
}

void DeathEyeBoss::UpdateIMG(SDL_Renderer* des)
{

	if(hp_ > DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL)														//boss calm_mode
	{
		if(Dir_ == Dir_LEFT)
		{
			LoadAnimation(eye_boss_left, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
		else if(Dir_ == Dir_LEFT_DOWN)
		{
			LoadAnimation(eye_boss_down_left, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
		else if(Dir_ == Dir_LEFT_UP)
		{
			LoadAnimation(eye_boss_up_left, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
		else if(Dir_ == Dir_RIGHT)
		{
			LoadAnimation(eye_boss_right, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
		else if(Dir_ == Dir_RIGHT_DOWN)
		{
			LoadAnimation(eye_boss_down_right, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
		else if(Dir_ == Dir_RIGHT_UP)
		{
			LoadAnimation(eye_boss_up_right, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
	}


	else																				//boss rage_mode
	{
		if(Dir_ == Dir_LEFT || Dir_ == Dir_LEFT_DOWN || Dir_ == Dir_LEFT_UP)
		{
			LoadAnimation(eye_boss_RageMode_left, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
		else if(Dir_ == Dir_RIGHT || Dir_ == Dir_RIGHT_DOWN || Dir_ == Dir_RIGHT_UP)
		{
			LoadAnimation(eye_boss_RageMode_right, des, DEATH_EYE_BOSS_FRAM_NUM);
		}
	}
}

void DeathEyeBoss::HandleMove(SDL_Renderer* des, PlayerObject* p_player, std::vector<EffectObject*> effect_list)
{

	//RePos:
	if(is_RePos_ == true)
	{
		float new_xPos;
		float new_yPos;

		int ReposType = rand()% 4;

		if(ReposType == POS_LEFT)
		{
			new_xPos = -SCREEN_WIDTH*0.25;
			new_yPos = rand()% SCREEN_HEIGHT;
		}
		else if(ReposType == POS_RIGHT)
		{
			new_xPos =  SCREEN_WIDTH + SCREEN_WIDTH*0.25;
			new_yPos = rand()% SCREEN_HEIGHT;
		}
		else if(ReposType == POS_UP)
		{
			new_yPos = -SCREEN_HEIGHT*0.25;
			new_xPos = rand()% SCREEN_WIDTH;
		}
		else if(ReposType == POS_DOWN)
		{
			new_yPos = SCREEN_HEIGHT + SCREEN_HEIGHT*0.25;
			new_xPos = rand()% SCREEN_WIDTH;
		}

		x_pos_ = map_x_ + new_xPos;
		y_pos_ = map_y_ + new_yPos;



		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		if(hp_ <= DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL)
		{
			is_attack_while_rage_ = true;
		}
		
		is_RePos_ = false;
	}

	
	//Object Rect
	SDL_Rect ply_rect = p_player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);
	SDL_Rect boss_rect = this->GetRectWhileHaveNumFramIs(DEATH_EYE_BOSS_FRAM_NUM);

	//Change Direction
	if(ply_rect.x < boss_rect.x)														    //left
	{
		if(ply_rect.y + ply_rect.h < boss_rect.y)
		{
			Dir_ = Dir_LEFT_UP;
		}
		else if(ply_rect.y > boss_rect.y + boss_rect.h)
		{
			Dir_ = Dir_LEFT_DOWN; 
		}
		else
		{
			Dir_ = Dir_LEFT;
		}
	}

	else if(ply_rect.x + ply_rect.w > boss_rect.x + boss_rect.w)							//right
	{
		if(ply_rect.y + ply_rect.h < boss_rect.y)
		{
			Dir_ = Dir_RIGHT_UP;
		}
		else if(ply_rect.y > boss_rect.y + boss_rect.h)
		{
			Dir_ = Dir_RIGHT_DOWN; 
		}
		else
		{
			Dir_ = Dir_RIGHT;
		}
	}




	//CALM MODE MOVE

	if(hp_ > DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL 
		&& std::abs(ply_rect.x - boss_rect.x) > DEATH_EYE_BOSS_MAX_STOP_MOVE_VAL)

	{

		reload_laze_val_ = DEATH_EYE_BOSS_RELOAD_LAZE_VAL;

		if(Dir_ == Dir_LEFT)
		{
			x_val_ = -DEATH_EYE_BOSS_CALM_SPEED;
			y_val_ = 0;
		}
		else if(Dir_ == Dir_LEFT_UP)
		{
			x_val_ = -DEATH_EYE_BOSS_CALM_SPEED;
			y_val_ = -DEATH_EYE_BOSS_CALM_SPEED;
		}
		else if(Dir_ == Dir_LEFT_DOWN)
		{
			x_val_ = -DEATH_EYE_BOSS_CALM_SPEED;
			y_val_ = DEATH_EYE_BOSS_CALM_SPEED;
		}
		else if(Dir_ == Dir_RIGHT)
		{
			x_val_ = DEATH_EYE_BOSS_CALM_SPEED;
			y_val_ = 0;
		}
		else if(Dir_ == Dir_RIGHT_UP)
		{
			x_val_ = DEATH_EYE_BOSS_CALM_SPEED;
			y_val_ = -DEATH_EYE_BOSS_CALM_SPEED;
		}
		else if(Dir_ == Dir_RIGHT_DOWN)
		{
			x_val_ = DEATH_EYE_BOSS_CALM_SPEED;
			y_val_ = DEATH_EYE_BOSS_CALM_SPEED;
		}
	}
	
	else if(hp_ > DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL								//offset
		 && std::abs(ply_rect.y - boss_rect.y) >= DEATH_EYE_BOSS_CALM_SPEED*2)
	{
		x_val_ = 0;

		if(ply_rect.y < boss_rect.y)
		{
			y_val_ = -DEATH_EYE_BOSS_CALM_SPEED*0.65;
		}
		else if(ply_rect.y > boss_rect.y)
		{
			y_val_ = DEATH_EYE_BOSS_CALM_SPEED*0.65;
		}
	}
	


	else if(Dir_ == Dir_LEFT && hp_ > DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL 			//laze
	|| Dir_ == Dir_RIGHT && hp_ > DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL) 
	{
		x_val_ = 0;
		y_val_ = 0;
		reload_laze_val_--;

		//charge_laze effect
		if(effect_list.at(CHARGELAZE_EFFECT)->Get_is_show() == false
		&& deatheye_lazebullet_.Get_ShowTime() <= 0)
		{
			
			SDL_Rect rect_for_effect_with_boss_rect;	//rect_effect

			if(Dir_ == Dir_LEFT)
			{
				rect_for_effect_with_boss_rect.w = 0;
			}
			else if(Dir_ == Dir_RIGHT)
			{
				rect_for_effect_with_boss_rect.w = boss_rect.w*2;
			}
			
			rect_for_effect_with_boss_rect.x =  boss_rect.x;
			rect_for_effect_with_boss_rect.y = boss_rect.y;
			rect_for_effect_with_boss_rect.h = boss_rect.h;

			effect_list.at(CHARGELAZE_EFFECT)->SetRect_PositionEffect(   rect_for_effect_with_boss_rect, this->x_pos_ , this->y_pos_   );
			effect_list.at(CHARGELAZE_EFFECT)->Set_is_show(true);
		}

		//set is_shut = true while reload laze_val <= 0
		if(reload_laze_val_ <= 0)
		{
			is_shut_ = true;
			reload_laze_val_ = DEATH_EYE_BOSS_RELOAD_LAZE_VAL;
			Mix_PlayChannel(-1, g_audio.at(DEATHEYE_LAZEBULLET_SOUND), 0);
		}
	}




	//RAGE MODE MOVE

	else
	{
		if(is_attack_while_rage_== true)
		{
			float distance_x = ply_rect.x - boss_rect.x;
			float distance_y = ply_rect.y - boss_rect.y;

			float magnitude = sqrt(distance_x*distance_x + distance_y*distance_y);

			x_val_ = distance_x / magnitude * DEATH_EYE_BOSS_RAGE_SPEED;
			y_val_ = distance_y / magnitude * DEATH_EYE_BOSS_RAGE_SPEED;

			charge_thuntherweapon_counter_++;
			if(charge_thuntherweapon_counter_ >= DEATH_EYE_BOSS_MAX_CHARGE_THUNDERWEARPON)
			{
				deatheye_thunderweapon.Set_IsUseThunderWeapon(true);
				charge_thuntherweapon_counter_ = 0;

				Mix_PlayChannel(-1, g_audio.at(DEATHEYE_ELECTRIK_SOUND), 0);
			}

			is_attack_while_rage_ = false;
		}


		static float s_death_eye_move_distance_x = 0;			//boss move distance
		static float s_death_eye_move_distance_y = 0;

		s_death_eye_move_distance_x += x_val_;
		s_death_eye_move_distance_y += y_val_;

		if(std::abs(s_death_eye_move_distance_x) > DEATH_EYE_BOSS_RAGE_MOVE_DISTANCE_X 
		|| std::abs(s_death_eye_move_distance_y) > DEATH_EYE_BOSS_RAGE_MOVE_DISTANCE_Y)
		{

			s_death_eye_move_distance_x = 0;
			s_death_eye_move_distance_y = 0;

			is_RePos_ = true;
		}
	}


	x_pos_ += x_val_;
	y_pos_ += y_val_;

}

void DeathEyeBoss::HandleLazeBullet(SDL_Renderer* des, std::vector<EffectObject*> effect_list)
{

	if(is_shut_ == true)
	{
		deatheye_lazebullet_.Set_IsShow(true);
		deatheye_lazebullet_.Set_ShowTime(LAZE_BULLET_SHOW_FRAM);

		is_shut_ = false;
	}

	//handle
	deatheye_lazebullet_.HandleLazeBullet(des, this->map_x_, this->map_y_, this->Dir_,
											  GetRectWhileHaveNumFramIs(DEATH_EYE_BOSS_FRAM_NUM), effect_list);

	//show
	if(deatheye_lazebullet_.Get_ShowTime() > 0)
	{
		deatheye_lazebullet_.Show(des);

		if(deatheye_lazebullet_.Get_ShowTime() <= 0)
		{
			reload_laze_val_ = DEATH_EYE_BOSS_RELOAD_LAZE_VAL;
		}
	}
}

void DeathEyeBoss::HandleThunderWeapon(SDL_Renderer* des, Map* g_map, PlayerObject* p_player, std::vector<EffectObject* > list_effect)
{
	//set direction thunder_weapon
	if(deatheye_thunderweapon.Get_is_use_TdWeapon() == true)
	{
		deatheye_thunderweapon.Set_Direction(   this->Dir_   );
	}

	//hnadle move
	deatheye_thunderweapon.HandleThundeWeapon(g_map, p_player, list_effect);

	//show thunder
	deatheye_thunderweapon.ShowThunder(des);
}

void DeathEyeBoss::CheckCollision(PlayerObject* p_player, std::vector<EffectObject*> effect_list)
{

	static int wait_handle_powerPlayer = 0;
	static bool is_handle_powerPlayer = false;
	static int boss2_music_channel_id = 5;

	//Rect Obj:
	SDL_Rect ply_rect = p_player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);
	SDL_Rect boss_rect = this->GetRectWhileHaveNumFramIs(DEATH_EYE_BOSS_FRAM_NUM);

	if(SDLFunction::CheckCollision(ply_rect, boss_rect)   ==   true)						//ply vs boss
	{
		p_player->HandlePowerPlayer(DIE_VAL, effect_list);
	}

	if(deatheye_lazebullet_.Get_ShowTime() > 0)
	{
		if(SDLFunction::CheckCollision(ply_rect, deatheye_lazebullet_.GetRect())   ==   true)	//ply vs LazeBullet
		{
			if(is_handle_powerPlayer == true)
			{
				p_player->HandlePowerPlayer(LAZE_BULLET_DMG, effect_list);
			}

			is_handle_powerPlayer = false;
		}
	}
	else
	{
		is_handle_powerPlayer = true;
	}


	std::vector<BulletObject*> ply_bulet_list = p_player->Get_bullet_list();				//ply_bullet vs boss
	int vt_size =  ply_bulet_list.size();
	for(int pbl = 0; pbl < vt_size; pbl++)
	{
		BulletObject* p_bullet = ply_bulet_list.at(pbl);
		if(p_bullet != NULL)
		{
			if(SDLFunction::CheckCollision(   p_bullet->GetRectWhileHaveNumFramIs(p_bullet->Get_bullet_fram_num()),   
									       boss_rect   )
										   == true   )
			{

				//handle val
				p_bullet->Set_is_move(false);
				hp_ -= p_bullet->Get_bullet_dmg();
				hits_counter_ ++;


				if(hp_ > DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL)										//while calm mode
				{
					if(hits_counter_ >= DEATH_EYE_BOSS_CALM_MODE_MAX_HITS_COUNTER)
					{
						is_RePos_ = true;
						hits_counter_ = 0;
					}
				}


				else if(hp_ <= DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL && hp_ > 0
					 && hits_counter_ >= DEATH_EYE_BOSS_RAGE_MODE_MAX_HITS_COUNTER   )							//while rage mode
				{
					is_RePos_ = true;
					hits_counter_ = 0;
					is_attack_while_rage_ = true;
				}


				else if(hp_ <= 0 )																				 //while boss die
				{
					effect_list.at(FLAST_EFFECT)->SetRect_PositionEffect(boss_rect, this->x_pos_, this->y_pos_);
					effect_list.at(FLAST_EFFECT)->Set_is_show(true);

					SDL_Rect screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
					effect_list.at(WINGAME_EFFECT)->SetRect_PositionEffect(screen, MAX_MAP_X*TILE_SIZE - (SCREEN_WIDTH), 0 );
					effect_list.at(WINGAME_EFFECT)->Set_is_show(true);
				}
			}
		}
	}

	//handle music boss2
	if(hp_ <= DEATH_EYE_BOSS_HP*DEATH_EYE_BOSS_CHANGE_MODE_VAL && hp_ > 0 
	   && !Mix_Playing(boss2_music_channel_id))	
	{
		is_RePos_ = true;
		Mix_HaltChannel(-1);
		Mix_PlayChannel(boss2_music_channel_id, g_audio.at(BOSS2_SOUND), 0);
	}

}

void DeathEyeBoss::Set_clip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < DEATH_EYE_BOSS_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//DEATH_EYE OF LAZE_BULLET

LazeBullet::LazeBullet()
{
	map_x_ = 0;
	map_y_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;

	is_show_ = false;
	Show_time_ = 0;
}

LazeBullet::~LazeBullet()
{
	;
}

void LazeBullet::InitLazeBullet(SDL_Renderer* des)
{
	LoadImg(EyeBoss_LazeBullet, des);
}

void LazeBullet::HandleLazeBullet(SDL_Renderer* des, float map_x, float map_y, int dir_,
								  SDL_Rect obj_rect, std::vector<EffectObject*> effect_list)
{
	if(Show_time_ > 0)
	{

		this->rect_.y = obj_rect.y + obj_rect.h*0.35;

		if(dir_ == Dir_LEFT || dir_ == Dir_LEFT_DOWN || dir_ == Dir_LEFT_UP)
		{
			this->rect_.x = obj_rect.x - this->rect_.w + 5;
		}
		else if(dir_ == Dir_RIGHT || dir_ == Dir_RIGHT_DOWN || dir_ == Dir_RIGHT_UP)
		{
			this->rect_.x = obj_rect.x + obj_rect.w - 5;
		}
	}
}

void LazeBullet::Show(SDL_Renderer* des)
{
	if(Show_time_ > 0)
	{

		SDL_Rect RenderQuad = {rect_.x, rect_.y, rect_.w, rect_.h};

		int check = SDL_RenderCopy(des, p_object_, NULL, &RenderQuad);

		Show_time_--;
	}
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//THUNDER WEAPON

ThunderWeapon::ThunderWeapon()
{
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	map_x_ = 0;
	map_y_ = 0;

	Dir_ = Dir_NONE;
	thunder_show_time_ = 0;
	fram_ = 0;
}

ThunderWeapon::~ThunderWeapon()
{
	;
}

void ThunderWeapon::InitThunderWeapon(SDL_Renderer* des)
{
	LoadAnimation(EyeBoss_ThunderWeapon, des, THUNDER_WEAPON_FRAM_NUM);
	SetClip();
}

void ThunderWeapon::HandleThundeWeapon(Map* g_map, PlayerObject* p_player, std::vector<EffectObject* > list_effect)
{

	//handle data thunder
	if(is_use_thunderweapon == true)
	{
		//dir
		if(Dir_ == Dir_LEFT || Dir_ == Dir_LEFT_DOWN || Dir_ == Dir_LEFT_UP)				//thunder move right
		{
			x_pos_ = p_player->Get_x_pos() - SCREEN_WIDTH*0.5;

			x_val_ = THUNDER_MOVE_SPEED;
		}
		else if(Dir_ == Dir_RIGHT || Dir_ == Dir_RIGHT_DOWN || Dir_ == Dir_RIGHT_UP)		//thunder move left
		{
			x_pos_ = p_player->Get_x_pos() + SCREEN_WIDTH*0.5;

			x_val_ = -THUNDER_MOVE_SPEED;
		}
		
		//show time
		thunder_show_time_ = THUNDER_SHOW_TIME;


	is_use_thunderweapon = false;
	}


	//handle move thunder
	if(thunder_show_time_ > 0)
	{
		Set_mapXY(g_map->star_x_, g_map->star_y_);
		x_pos_ += x_val_;


		//colission
		if(SDLFunction::CheckCollision(   p_player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM),
										  this->GetRectWhileHaveNumFramIs(THUNDER_WEAPON_FRAM_NUM)   )
									   == true)
		{
			p_player->HandlePowerPlayer(DIE_VAL, list_effect);
			
			//todo
		}
	}
}

void ThunderWeapon::ShowThunder(SDL_Renderer* des)
{
	if(thunder_show_time_ > 0)
	{
		fram_++;
		if(fram_ >= THUNDER_WEAPON_FRAM_NUM)
		{
			fram_ = 0;
		}

		rect_.x = x_pos_ - map_x_;
		rect_.y = 0;

		SDL_Rect renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

		int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &renderquad);

		thunder_show_time_--;
	}
}

void ThunderWeapon::SetClip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < THUNDER_WEAPON_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;
		}
	}
}