#include "BulletObject.h"
#include "PlayerObject.h"


BulletObject::BulletObject()
{
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	map_x_ = 0;
	map_y_ = 0;
	bullet_type_ = STANDERD_TYPE;
	bullet_dmg_ = 1;
	is_move_ = false;
	fram_ = 0;
	reload_bullet_val_ = 0;
}

BulletObject::~BulletObject()
{
	;
}

void BulletObject::Set_bullet_fram(const int& bullet_framm_num)
{
	for(int i = 0; i < bullet_framm_num; i++)
	{
		bullet_clip_[i].x = i * width_fram_;
		bullet_clip_[i].y = 0;
		bullet_clip_[i].w = width_fram_;
		bullet_clip_[i].h = height_fram_;
	}
}

void BulletObject::ShowBullet(SDL_Renderer* des)
{
	if(is_move_)
	{
		if(bullet_type_ == PRO_TYPE)
		{
			fram_++;
			if(fram_ >= PRO_BULLET_FRAM)
			{
				fram_ = 0;
			}
			
			rect_.x = x_pos_ - map_x_;
			rect_.y = y_pos_ - map_y_;
			SDL_Rect renderquad = {rect_.x, rect_.y, width_fram_, height_fram_};    //quad_rect in screen

			SDL_Rect* current_clip = &bullet_clip_[fram_];       //fram_rect_in_img

			SDL_RenderCopy(des, p_object_, current_clip, &renderquad);
		}

		else
		{
			rect_.x = x_pos_ - map_x_;
			rect_.y = y_pos_ - map_y_;

			Render(des);
		}
	}
}


void BulletObject::SetDataBullet(const int& obj_bullet_type)
{

	bullet_type_ = obj_bullet_type;

	if(bullet_type_ == STANDERD_TYPE)
	{
		bullet_dmg_ = STANDARD_DMG;
		bullet_fram_num_ = 1;
		// bullet speed
	}
	else if(bullet_type_ == ROUND_TYPE)
	{
		bullet_dmg_ = ROUND_DMG;
		bullet_fram_num_ = 1;
	}
	else if(bullet_type_ == STAR_TYPE)
	{
		bullet_dmg_ = STAR_DMG;
		bullet_fram_num_ = 1;
	}
	else if(bullet_type_ == BALL_TYPE)
	{
		bullet_dmg_ = BALL_DMG;
		bullet_fram_num_ = 1;
	}
	else if(bullet_type_ == PRO_TYPE)
	{
		bullet_dmg_ = PRO_DMG;
		bullet_fram_num_ = 4;
	}
	else if(bullet_type_ == ROCKET_TYPE)
	{
		bullet_dmg_ = ROCKET_DMG;
		bullet_fram_num_ = 1;
	}
	// else if...
}

void BulletObject::HandleMove(const int& x_boder = SCREEN_WIDTH, const int& y_boder = SCREEN_HEIGHT)
{
	if(bullet_dir_ == Dir_RIGHT)
	{
		x_pos_ += x_val_;
	}
	else if(bullet_dir_ == Dir_LEFT)
	{
		x_pos_ -= x_val_;
	}
	else if(bullet_dir_ == Dir_UP)
	{
		y_pos_ -= y_val_;
	}
	else if(bullet_dir_ == Dir_DOWN)
	{
		y_pos_ += y_val_;
	}
	else if(bullet_dir_ == Dir_RIGHT_UP)
	{
		x_pos_ += x_val_;
		y_pos_ -= y_val_;
	}
	else if(bullet_dir_ == Dir_RIGHT_DOWN)
	{
		x_pos_ += x_val_;
		y_pos_ += y_val_;
	}
	else if(bullet_dir_ == Dir_LEFT_UP)
	{
		x_pos_ -= x_val_;
		y_pos_ -= y_val_;
	}
	else if(bullet_dir_ == Dir_LEFT_DOWN)
	{
		x_pos_ -= x_val_;
		y_pos_ += y_val_;
	}
	else if(bullet_dir_ == Dir_NONE)
	{
		x_pos_ += x_val_;
		y_pos_ += y_val_;
	}

	if(rect_.x > x_boder * 1.5 || rect_.x < -0.5 * x_boder || rect_.y > y_boder|| rect_.y < 0)	
	{
		is_move_ = false; 
	}
	
}

void BulletObject::HandelMoveThreat(const int& x_limit, const int& y_limit)
{
	if(bullet_dir_ == Dir_RIGHT)
	{
		rect_.x += x_val_;
		if(rect_.x >= x_limit)
		{is_move_ = false; }
	}
	else if(bullet_dir_ == Dir_LEFT)
	{
		rect_.x -= x_val_;
		if(rect_.x <= 0)
		{is_move_ = false; }
	}
}


std::string BulletObject::Get_img_bullet_type(const int& Obj_dir_)
{
	if(bullet_type_ == STAR_TYPE)
	{
		return star_bullet_img;
	}
	else if(bullet_type_ == ROUND_TYPE)
	{
		return  round_bullet_img;
	}
	else if(bullet_type_ == BALL_TYPE)
	{
		return ball_bullet_img;
	}
	else if(bullet_type_ == PRO_TYPE)
	{
		if(Obj_dir_ ==  Dir_RIGHT)
		{
			return pro_bullet_right_img;
		}
		else if(Obj_dir_== Dir_LEFT)
		{
			return pro_bullet_left_img;
		}
		else if(Obj_dir_ ==  Dir_UP)
		{
			return pro_bullet_up_img;
		}
		else if(Obj_dir_ ==  Dir_DOWN)
		{
			return pro_bullet_down_img;
		}
	}
	else if(bullet_type_ == ROCKET_TYPE)
	{
		if(Obj_dir_ == Dir_LEFT)
		{
			return rocket_bullet_left_img;
		}
		else if(Obj_dir_ == Dir_RIGHT)
		{
			return rocket_bullet_right_img;
		}
		else if(Obj_dir_ == Dir_UP)
		{
			return rocket_bullet_up_img;
		}
		else if(Obj_dir_ == Dir_DOWN)
		{
			return rocket_bullet_down_img;
		}
	}

	// ... (NEW BULLET TYPE)
	else
	{
		return standard_bullet_img;
	}
}