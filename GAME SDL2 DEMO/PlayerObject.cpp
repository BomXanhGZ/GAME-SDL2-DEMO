

#include "PlayerObject.h"


void PlayerObject::Set_check_tile()
{
	//SPECIAL_TILE
	check_tile_.push_back(MYSTERY_TILE); //...
	check_tile_.push_back(HEART_TILE);  // + lives
	check_tile_.push_back(HP_TILE);
	check_tile_.push_back(COIN_TILE);
	check_tile_.push_back(FLAG_TILE);  //save_game

	//BULLET_TYPE
	check_tile_.push_back(STAR_TYPE);
	check_tile_.push_back(ROUND_TYPE);
	check_tile_.push_back(BALL_TYPE);
	check_tile_.push_back(PRO_TYPE);
	check_tile_.push_back(ROCKET_TYPE);

	//...;
}

PlayerObject::PlayerObject()
{
	x_pos_ = 135;
	y_pos_ = 75;
	x_val_ = 0;
	y_val_ = 0;
	map_x_ = 0;
	map_y_ = 0;

	on_ground_ = false;
	come_back_ = false;
	check_up_ = false;
	check_action_ = false;
	fram_ = 0;
	ply_bullet_type_ = STANDERD_TYPE;
	player_hp_ = PLAYER_HP;
	player_lives_ = PLAYER_LIVES;
	shield_duration_ = SHIELD_DURATION;
	
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	input_type_.jump_ = 0;
	status_ = Dir_RIGHT;
	check_dir_ = Dir_NONE;
}

PlayerObject::~PlayerObject()
{
	;
}

void PlayerObject::Doplayer(Map& map_data, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
{
	x_val_ = 0;
	y_val_ += GRAVITY_SPEED;
	if(y_val_ >= MAX_FALL_SPEED)
	{
		y_val_ = MAX_FALL_SPEED;
	}
	

	if(input_type_.left_ == 1)
	{
		x_val_ -= PLAYER_SPEED;
	}

	if(input_type_.right_ == 1)
	{
		x_val_ += PLAYER_SPEED;
	}

	if(x_val_ != 0)
	  {check_action_ = true; }
	else
	  {check_action_ = false; }

	if(input_type_.jump_ == 1)
	{
		if(on_ground_ == true)
		{
			y_val_ = -PLAYER_JUMP_SPEED;
			on_ground_ = false;
		}

		input_type_.jump_ = 0;
	}

	CheckToMap(map_data, des, list_effect);
	CenteEntityOnMap(map_data, des);

	if(come_back_)
	{
		ply_bullet_type_ = STANDERD_TYPE;
		ammo_icon_.Set_AmmoIcon(ply_bullet_type_, des);
		shield_duration_ = SHIELD_DURATION;

		y_pos_ = 0;
		if(rect_.x >= SCREEN_WIDTH / 4)
		{
			x_pos_ -= TILE_SIZE*4;
		}

		come_back_ = false;
	}
	else if(shield_duration_ > 0)
	{
		list_effect.at(GREEN_SHIELD_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM),
																	   this->x_pos_, this->y_pos_   );
		list_effect.at(GREEN_SHIELD_EFFECT)->Set_is_show(true);

		shield_duration_--;
	}
}

void PlayerObject::HandelInputAction(SDL_Event events, SDL_Renderer* des)
{

	if(events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{

		case SDLK_d:
			{
				status_ = Dir_RIGHT;
				check_dir_ = Dir_RIGHT;
				input_type_.right_ = 1;
				input_type_.left_ = 0;

			}
			break;

		case SDLK_a:
			{
				status_ = Dir_LEFT;
				check_dir_ = Dir_LEFT;
				input_type_.left_ = 1;
				input_type_.right_ = 0;
			}
			break;

		case SDLK_SPACE:
			{
				if(on_ground_ == true)
				{
					input_type_.jump_ = 1;
					Mix_PlayChannel(-1, g_audio.at(P_JUMP_SOUND), 0);
				}
			}
			break;

		case SDLK_w:
			{
				status_ = Dir_UP;
				input_type_.up_ = 1;
				check_up_ = true;
			}
			break;

		case SDLK_s:
			{
 				status_ = Dir_DOWN;
				input_type_.down_ = 1;
			}
			break;

		case SDLK_ESCAPE:
			{
				is_pause = !is_pause;
			}
			
		default:
			break;
		}
	}
	else
	if(events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{

		case SDLK_d:
			{
				input_type_.right_ = 0;
			}
			break;

		case SDLK_a:
			{
				input_type_.left_ = 0;
			}
			break;

		case SDLK_SPACE:
			{
				input_type_.jump_ = 0;
			}
			break;

		case SDLK_w:
			{
				input_type_.up_ = 0;
				check_up_ = false;

				if(check_dir_ == Dir_LEFT)
				  {status_ = Dir_LEFT; }
				else if(check_dir_ == Dir_RIGHT)
				{status_ = Dir_RIGHT; }
			}
			break;

		case SDLK_s:
			{
				input_type_.down_ = 0;

				if(check_dir_ == Dir_LEFT)
				  {status_ = Dir_LEFT; }
				else if(check_dir_ == Dir_RIGHT)
				{status_ = Dir_RIGHT; }
			}
			break;

		default:
			break;
		}
	}
	
	if(events.type == SDL_MOUSEBUTTONDOWN)
	{
		if(events.button.button == SDL_BUTTON_LEFT)
		{
			BulletObject* p_bullet = new BulletObject;
			p_bullet->SetDataBullet(ply_bullet_type_);

			if(p_bullet->Get_bullet_type() == PRO_TYPE)
			{
				p_bullet->LoadAnimation(p_bullet->Get_img_bullet_type(status_), des, PRO_BULLET_FRAM);
				p_bullet->Set_bullet_fram(PRO_BULLET_FRAM);

				Mix_PlayChannel(-1, g_audio.at(SHOOT_PROBULLET_SOUND), 0);
			}
			else
			{
				p_bullet->LoadAnimation(p_bullet->Get_img_bullet_type(status_), des, 1);  // get bullet fram num

				Mix_PlayChannel(-1, g_audio.at(SHOOT_BULLET_SOUND), 0);
			}

			if(status_ == Dir_RIGHT)
			{
				if(check_up_)
				{
					p_bullet->Set_position(this->x_pos_+ 0.65*this->width_fram_, this->y_pos_ + height_fram_ / 3);
					p_bullet->Set_bullet_dir(Dir_RIGHT_UP);
				}
				else if(input_type_.down_ == 1)
				{
					p_bullet->Set_position(this->x_pos_+ 0.65*width_fram_, this->y_pos_ + height_fram_ / 3);
					p_bullet->Set_bullet_dir(Dir_RIGHT_DOWN);
				}
				else
				{
					p_bullet->Set_position(this->x_pos_+ 0.65*width_fram_, this->y_pos_ + height_fram_ / 3);
					p_bullet->Set_bullet_dir(Dir_RIGHT);
				}

			}
			else if(status_ == Dir_LEFT)
			{
				if(check_up_)
				{
					p_bullet->Set_position(this->x_pos_+ 0.35*width_fram_ - p_bullet->GetWidthFram(), this->y_pos_ + height_fram_ / 3);
					p_bullet->Set_bullet_dir(Dir_LEFT_UP);
				}
				else if(input_type_.down_ == 1)
				{
					p_bullet->Set_position(this->x_pos_+ 0.35*width_fram_ - p_bullet->GetWidthFram(), this->y_pos_ + height_fram_ / 3);
					p_bullet->Set_bullet_dir(Dir_LEFT_DOWN);
				}
				else
				{
					p_bullet->Set_position(this->x_pos_+ 0.35*width_fram_ - p_bullet->GetWidthFram(), this->y_pos_ + height_fram_ / 3);
					p_bullet->Set_bullet_dir(Dir_LEFT);
				}
			}
			else if(status_ == Dir_UP)
			{
				if(!check_action_)
				{
					p_bullet->Set_position(this->x_pos_+ 0.5*width_fram_ - p_bullet->GetWidthFram()*0.5, this->y_pos_ + height_fram_*0.6 - p_bullet->GetHeightFram());
					p_bullet->Set_bullet_dir(Dir_UP);
				}
				else
				{
					if(check_dir_ == Dir_LEFT)
					{
					p_bullet->Set_position(this->x_pos_+ 0.35*width_fram_ - p_bullet->GetWidthFram(), this->y_pos_ + height_fram_*0.2 - p_bullet->GetHeightFram());
						p_bullet->Set_bullet_dir(Dir_LEFT_UP);
					}
					if(check_dir_ == Dir_RIGHT)
					{
					p_bullet->Set_position(this->x_pos_+ 0.65*width_fram_, this->y_pos_ + height_fram_*0.2 - p_bullet->GetHeightFram());
						p_bullet->Set_bullet_dir(Dir_RIGHT_UP);
					}
				}

			}
			else if(status_ == Dir_DOWN)
			{
				if(!check_action_)
				{
					p_bullet->Set_position(this->x_pos_+ 0.5*width_fram_ - p_bullet->GetWidthFram()*0.5, this->y_pos_ + height_fram_*0.2);
					p_bullet->Set_bullet_dir(Dir_DOWN);
				}
				else
				{
					if(check_dir_ == Dir_LEFT)
					{
					p_bullet->Set_position(this->x_pos_+ 0.35*width_fram_ - p_bullet->GetWidthFram(), this->y_pos_ + height_fram_*0.2);
						p_bullet->Set_bullet_dir(Dir_LEFT_DOWN);
					}
					else if(check_dir_ == Dir_RIGHT)
					{
					p_bullet->Set_position(this->x_pos_+ 0.65*width_fram_, this->y_pos_ + height_fram_*0.2);
						p_bullet->Set_bullet_dir(Dir_RIGHT_DOWN);
					}
				}
			}

			p_bullet->Set_x_val(BULLET_SPEED);
			p_bullet->Set_y_val(BULLET_SPEED*0.7);

			p_bullet->Set_is_move(true);
			p_bullet_list_.push_back(p_bullet);
		}
	}
	/*
	else
	if(events.type == SDL_MOUSEBUTTONUP)
	{
		;// todo
	}
	*/
}

void PlayerObject::Show(SDL_Renderer* des)
{
	
	UpdateImgPlayer(des);

	if(input_type_.left_ == 1 ||
	   input_type_.right_ == 1)
	{
		fram_++;
		if(fram_ >= PLAYER_FRAM_NUM)
		  {fram_ = 0;}
	}
	else
		{fram_ = 0;}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	SDL_Rect* current_clip = &fram_clip_[fram_];       //fram_rect_in_img

	SDL_Rect renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};    //quad_rect in screen

	SDL_RenderCopy(des, p_object_, current_clip, &renderquad);
}

void PlayerObject::CenteEntityOnMap(Map& map_data, SDL_Renderer* des)
{

	// HORIZONTAL
	map_data.star_x_ = x_pos_ - SCREEN_WIDTH/3;
	if(map_data.star_x_ <= 0)
	{
		map_data.star_x_ = 0;
	}
	else if(map_data.star_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.star_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}


	// VENTICAL
	map_data.star_y_ = y_pos_ - SCREEN_HEIGHT/3;
	if(map_data.star_y_ <= 0)
	{
		map_data.star_y_ = 0;
	}
	else if(map_data.star_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.star_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

void PlayerObject::CheckToMap(Map& map_data, SDL_Renderer* des, std::vector<EffectObject*> list_effect)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//check horizontal
	int height_min = height_fram_< TILE_SIZE ? height_fram_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_fram_) / TILE_SIZE;

	y1 = y_pos_ / TILE_SIZE;
	y2 = (y_pos_ + height_min -1) / TILE_SIZE;     //neu ko -1, y2 se luon cham tile_ground


	if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y1 <= MAX_MAP_Y)
	{
		if(x_val_ > 0)
		{

			if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y1][x2]) != check_tile_.end())
			{
				//check change bullet type
				CheckBulletTile(map_data, y1, x2, des);

				//check power up tile
				CheckPowerUpTile(map_data, y1, x2);

				//set blank tile
				map_data.tile[y1][x2] = BLANK_TILE;

			}
			else if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y2][x2]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y2, x2, des);
				CheckPowerUpTile(map_data, y2, x2);
				map_data.tile[y2][x2] = BLANK_TILE;
			}
			else if(map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
			{

 				CheckInvisibleTile(map_data, y1, x2);
				CheckInvisibleTile(map_data, y2, x2);

				CheckTrapTile(map_data, y1, x2, list_effect);
				CheckTrapTile(map_data, y2, x2, list_effect);

				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= (width_fram_ + 1);
				x_val_ = 0;
			}
		}
		else if(x_val_ < 0)
		{
			int top_ = map_data.tile[y1][x1];
			int bot_ = map_data.tile[y2][x1];
			if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y1][x1]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y1, x1, des);
				CheckPowerUpTile(map_data, y1, x1);
				map_data.tile[y1][x1] = BLANK_TILE;
			}
			else if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y2][x1]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y2, x1, des);
				CheckPowerUpTile(map_data, y2, x1);
				map_data.tile[y2][x1] = BLANK_TILE;
			}
			else if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
			{

				CheckInvisibleTile(map_data, y1, x1);
				CheckInvisibleTile(map_data, y2, x1);

				CheckTrapTile(map_data, y1, x1, list_effect);
				CheckTrapTile(map_data, y2, x1, list_effect);

				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
			}
		}
	}

	/*
	CASE horizontal / x_val_ < 0:

	gan x1 = (x_pos_ + x_val_) / TILE_SIZE;
	khi event: left x_val = gia tri (-);
	<=> g.tri x1 sau khi nhan 1 luong x_val va chia cho tile size lay phan nguyen se la g.tri it hon vi tri current_tile cua nhan vat 1 tile;
	sau khi check gia tri tile va toa do thoa dk != blank_tile se thuc hien gan lai x_pos_ giu nguyen vi tri;
	vi o tren khi set x1, x_pos da nhan 1 luong g.tri (-) => current tile khi chia lay phan nguyen da bi giam 1 tile;
	=> +1(tile) vao x1 truoc khi ( * tile_size) de giu lai vi tri tize hien tai cua object;
	!: neu k thuc hien +1(tile) x_pos se duoc tinh bang gia tri (x1 da nhan vao 1 luong (-x_val));
	   <=> se la toa do trong tile noi co tile != blank tile...
	   => ket hop voi case check tile khac se gay ra loi dich chuyen qua len tile ma khong can jump
	   g.thich: loi nay xay ra khi o lan kiem tra tiep theo, bien canh cua object se bi hieu nham la da nam o trong bien cua tile



	CASE horizontal ? x_val_ > 0:

	gan x2 = (x_pos_ + x_val_ + width_fram_) / TILE_SIZE;
	... <=> (case < 0)
	khi co va cham xay ra se gan lai x_pos_ = x2 * tile_size  - width_fram_ -1(mpx)
	!: neu k(-1mpx) => o lan k.tra tiep theo, x2 se trung voi bien va bi hieu nham la da nam trong bien.
	   ket hop cung cac case khac se gay ra loi <=> case X_val_ < 0; 
	*/


	//check vatical
	int width_min = width_fram_ < TILE_SIZE ? width_fram_ : TILE_SIZE;

	x1 = x_pos_ / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_fram_ -1) / TILE_SIZE;

	if(x1 >= 0 && x2 <= MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) // ??? y2 <= MAX_MAP_Y ???
	{
		if(y_val_ > 0)
		{
			if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y2][x1]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y2, x1, des);
				CheckPowerUpTile(map_data, y2, x2);
				map_data.tile[y2][x1] = BLANK_TILE;
			}
			else if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y2][x2]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y2, x2, des);
				CheckPowerUpTile(map_data, y2, x2);
				map_data.tile[y2][x2] = BLANK_TILE;
			}
			else if(map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
			{

				CheckInvisibleTile(map_data, y2, x1);
				CheckInvisibleTile(map_data, y2, x2);

				CheckTrapTile(map_data, y2, x1, list_effect);
				CheckTrapTile(map_data, y2, x2, list_effect);

				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_fram_ +1);
				y_val_ = 0;

				on_ground_ = true;
			}

		}
		else if(y_val_ < 0)
		{
			if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y1][x1]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y1, x1, des);
				CheckPowerUpTile(map_data, y1, x1);
				map_data.tile[y1][x1] = BLANK_TILE;
			}
			else if(std::find(check_tile_.begin(), check_tile_.end(), map_data.tile[y1][x2]) != check_tile_.end())
			{
				CheckBulletTile(map_data, y1, x2, des);
				CheckPowerUpTile(map_data, y1, x2);
				map_data.tile[y1][x2] = BLANK_TILE;
			}
			else if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
			{

				CheckInvisibleTile(map_data, y1, x1);
				CheckInvisibleTile(map_data, y1, x2);

				CheckTrapTile(map_data, y2, x1, list_effect);
				CheckTrapTile(map_data, y2, x2, list_effect);

				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
	}

	//POSITON
	x_pos_ += x_val_;
	if(x_pos_ < 0)
	  {x_pos_ = 0;}
	else
	if(x_pos_ + width_fram_ > MAX_MAP_X * TILE_SIZE)  //???
	  {x_pos_ = (MAX_MAP_X * TILE_SIZE) - width_fram_ -1;}

	y_pos_ += y_val_;
	if(y_pos_ >= SCREEN_HEIGHT*2)
	{
		come_back_ = true;
		on_ground_ = false;
		HandlePowerPlayer(DIE_VAL, list_effect);
	}
}

void PlayerObject::UpdateImgPlayer(SDL_Renderer* des)
{
	if(on_ground_ == true)
	{
		if(check_dir_ == Dir_LEFT)
		{
			LoadImg(player_left_img, des);
		}
		else
		{
			LoadImg(player_right_img, des);
		}
	}
	else
	{
		if(check_dir_ == Dir_LEFT)
		{
			LoadImg(player_jump_left_img, des);
		}
		else
		{
			LoadImg(player_jump_right_img, des);
		}
	}
}


void PlayerObject::SetClip()
{
	if(width_fram_ > 0 && height_fram_ > 0)
	{
		for(int i = 0 ; i < PLAYER_FRAM_NUM ; i++)
		{
			fram_clip_[i].x = i * width_fram_;
			fram_clip_[i].y = 0;
			fram_clip_[i].w = width_fram_;
			fram_clip_[i].h = height_fram_;
		}
	}
}

void PlayerObject::Init_PowerShow(SDL_Renderer* des)
{
	Hp_icon_.LoadImg(player_hp_img, des);
	Hp_icon_.Set_Pos_Show_Hp();

	lives_icon_.LoadImg(player_lives_img, des);
	lives_icon_.SetRect(LIVES_ICON_X, LIVES_ICON_Y);
}

void PlayerObject::HandlePowerPlayer(const int& dmg, std::vector<EffectObject*> list_effect)
{
	if(shield_duration_ <= 0)
	{
		if(player_lives_ >= 0)
		{
			if(player_hp_ >= 1)
			{
				player_hp_ -= dmg; //dmg bullet threat
			}
		
			if(player_hp_ < 1)
			{
				player_lives_--;
				player_hp_ = PLAYER_HP;
				come_back_ = true;

				list_effect.at(PLAYER_RIP_EFFECT)->Set_is_show(true);
				list_effect.at(PLAYER_RIP_EFFECT)->SetRect_PositionEffect(   this->GetRectWhileHaveNumFramIs(PLAYER_FRAM_NUM), 
																			this->x_pos_, this->y_pos_   );
				Mix_PlayChannel(-1, g_audio.at(P_DIE_SOUND), 0);
			}
		}

		if(player_lives_ < 0)
		{
			if(MessageBox(NULL, L"GameOver !", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			is_quit = true;
		}
	}
	else
	{
		;// shield_sound
	}
}

void PlayerObject::ShowPower(SDL_Renderer* des)
{
	Hp_icon_.ShowPlayerHP(des, player_hp_);

	lives_icon_.Render(des);

	ammo_icon_.ShowAmmoIcon(des);

}

void PlayerObject::HandleBullet(SDL_Renderer* des)
{
	for(int i = 0; i < p_bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = p_bullet_list_.at(i);
		if(p_bullet != NULL)
		{
			if(p_bullet->Get_is_move() == true)
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);

				p_bullet->SetMapXY(map_x_, map_y_);

				p_bullet->ShowBullet(des);
			}
			else
			{
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if(p_bullet != NULL)
				{
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}

void  PlayerObject::CheckBulletTile(Map& map_data, int& y, int& x, SDL_Renderer* des)
{

		if(map_data.tile[y][x] == STAR_TYPE)
		{
			Mix_PlayChannel(-1, g_audio.at(P_SELECT_BULLET_SOUND), 0);
			ply_bullet_type_ = STAR_TYPE;
		}
		else if(map_data.tile[y][x] == ROUND_TYPE)
		{
			Mix_PlayChannel(-1, g_audio.at(P_SELECT_BULLET_SOUND), 0);
			ply_bullet_type_ = ROUND_TYPE;
		}
		else if(map_data.tile[y][x] == BALL_TYPE)
		{
			Mix_PlayChannel(-1, g_audio.at(P_SELECT_BULLET_SOUND), 0);
			ply_bullet_type_ = BALL_TYPE;
		}
		else if(map_data.tile[y][x] == PRO_TYPE)
		{
			Mix_PlayChannel(-1, g_audio.at(P_SELECT_BULLET_SOUND), 0);
			ply_bullet_type_ = PRO_TYPE;
		}
		else if(map_data.tile[y][x] == ROCKET_TYPE)
		{
			Mix_PlayChannel(-1, g_audio.at(P_SELECT_BULLET_SOUND), 0);
			ply_bullet_type_ = ROCKET_TYPE;
		}

		// ... (NEW BULLET TYPE)

		else
		{
			;
		}

		ammo_icon_.Set_AmmoIcon(ply_bullet_type_ ,des);
}

void PlayerObject::CheckPowerUpTile(Map& map_data, int& y, int& x)
{
	if(map_data.tile[y][x] == HEART_TILE)
	{
		Mix_PlayChannel(-1, g_audio.at(P_SELECT_POWER_SOUND), 0);
		player_lives_++;
	}
	else if(map_data.tile[y][x] == HP_TILE)
	{
		Mix_PlayChannel(-1, g_audio.at(P_SELECT_POWER_SOUND), 0);
		player_hp_ += 2;
	}

	// ... NEW POWERUP

}

void PlayerObject::CheckInvisibleTile(Map& map_data, int& y, int& x)
{
	if(map_data.tile[y][x] == INVISIBLE_TILE)
	{
		map_data.tile[y][x] = IRON_TILE;
	}
}

void PlayerObject::CheckTrapTile(Map& map_data, const int& y, const int& x, std::vector<EffectObject*> effect_list)
{
	if(map_data.tile[y][x] == STIKE_TILE)
	{
		HandlePowerPlayer(DIE_VAL, effect_list);
	}
}



