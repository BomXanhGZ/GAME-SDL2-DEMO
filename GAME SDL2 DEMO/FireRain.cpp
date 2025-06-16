
#include "FireRain.h"

FireRain::FireRain()
{
	map_x_= 0;
	map_y_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;

	fram_ = 0;
	is_ReShow_ = false;
}

FireRain::~FireRain()
{
	;
}

std::vector<FireRain*> FireRain::InitFireRain(SDL_Renderer* des)
{
	srand(time(NULL));

	std::vector<FireRain*> firerain_list;

	FireRain* p_fire = new FireRain[FIRE_NUM];

	for(int fr = 0; fr < FIRE_NUM; fr++)
	{
		FireRain* fire = (p_fire + fr);
		if(fire != NULL)
		{
			fire->LoadAnimation(fire_rain_left_img, des, FIRERAIN_FRAM_NUM);
			fire->Set_clip();

			fire->Set_y_pos(FIRE_Y_START - (SCREEN_HEIGHT * 0.15 * (1 + rand()%5)));
			fire->Set_x_pos(FIRE_X_START + (SCREEN_WIDTH * 0.15 * (1+ rand()%5)));
		}
		
		firerain_list.push_back(fire);
	}

	return firerain_list;
}

void FireRain::HandleFireRain(SDL_Renderer* des, Map* g_map,PlayerObject* player, std::vector<EffectObject*> effect_list)
{

	Set_mapXY(g_map->star_x_, g_map->star_y_);
	SDL_Rect ply_rect = player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM);


	//Handle Move
	x_val_ = -FIRE_SPEED_X;
	y_val_ = FIRE_SPEED_Y;
	x_pos_ += x_val_;
	y_pos_ += y_val_;


	//collide
	if(SDLFunction::CheckCollision(   ply_rect, this->GetRectWhileHaveNumFramIs(FIRERAIN_FRAM_NUM)   )
								   == true)
	{
		effect_list.at(FIRE_EFFECT)->SetRect_PositionEffect(   player->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM), 
															   player->Get_x_pos(), player->Get_y_pos()   );
		effect_list.at(FIRE_EFFECT)->Set_is_show(true);

		player->HandlePowerPlayer(FIRE_DMG_VAL, effect_list);

		Mix_PlayChannel(-1, g_audio.at(FIRE_SOUND), 0);

		is_ReShow_ = true;
	}
	

	//checkToMap
	CheckToMap(g_map, effect_list);


	//RePos
	if(is_ReShow_ == true)
	{
		RePosRad();
		is_ReShow_ = false;
	}
	else if(this->rect_.y + this->rect_.h > SCREEN_HEIGHT)
	{
		RePosRad();
	}


	Show(des);
}

void FireRain::Show(SDL_Renderer* des)
{
	fram_++;
	if(fram_ >= FIRERAIN_FRAM_NUM)
	{
		fram_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect Renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};

	int check = SDL_RenderCopy(des, p_object_, &Fram_clip_[fram_], &Renderquad);
}

void FireRain::Set_clip()
{
	if(width_fram_ != 0 && height_fram_ != 0)
	{
		for(int i = 0; i < FIRERAIN_FRAM_NUM; i++)
		{
			Fram_clip_[i].x = width_fram_ * i;
			Fram_clip_[i].y = 0;
			Fram_clip_[i].w = width_fram_;
			Fram_clip_[i].h = height_fram_;
		}
	}
}

void FireRain::CheckToMap(Map* g_map, std::vector<EffectObject*> effect_list)
{

	int x1,x2,y1,y2;
	std::vector<int> obstacle_tile = check_obstacle_tile_.Get_obstancle_tile();

	int height_min = height_fram_ < TILE_SIZE ? height_fram_ : TILE_SIZE;
	int width_min = width_fram_ < TILE_SIZE ? width_fram_ :	TILE_SIZE;
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	y1 = y_pos_ / TILE_SIZE;

	
	//ventical
	x2 = (x_pos_ + x_val_ + width_min) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_fram_ -1) / TILE_SIZE;

	if(x1 > 0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
	{

		int check_down_left = g_map->tile[y2][x1];
		int check_down_right = g_map->tile[y2][x2];

		if(std::find(obstacle_tile.begin(), obstacle_tile.end(), check_down_left) != obstacle_tile.end()							 //handle
		|| std::find(obstacle_tile.begin(), obstacle_tile.end(), check_down_right) != obstacle_tile.end())
		{
			effect_list.at(FIRE_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(FIRERAIN_FRAM_NUM),
																   this->x_pos_, 
																   this->y_pos_ - TILE_SIZE*0.25
															   ); 
			effect_list.at(FIRE_EFFECT)->Set_is_show(true);

			is_ReShow_ = true;
		}
	}
}

void FireRain::RePosRad()
{
	y_pos_ = FIRE_Y_START;
	x_pos_ = map_x_ + 150+ rand()%1400;
}