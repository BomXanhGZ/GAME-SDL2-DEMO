
#include "EffectObject.h"

EffectObject::EffectObject()
{
	x_pos_ = 0;
	y_pos_ = 0;
	fram_ = 0;
	fram_num_ = 0;
	is_show_ = false;
}

EffectObject::~EffectObject()
{
	;//
}

void EffectObject::InitEffect(std::string path, SDL_Renderer* des, int effect_fram_num)
{
	fram_num_ = effect_fram_num;
	LoadAnimation(path, des, fram_num_);
	Set_effect_clip();
}


void EffectObject::Set_effect_clip()
{
	for(int i = 0; i < fram_num_; i++)
	{
		effect_clip[i].x = width_fram_ * i;
		effect_clip[i].y = 0;
		effect_clip[i].w = width_fram_;
		effect_clip[i].h = height_fram_;
	}
}

void EffectObject::ShowEffect(SDL_Renderer* des, Map* g_map)
{
	if(fram_ < fram_num_)
	{
		SDL_Rect render_quad = 
		{
			rect_.x = this->x_pos_ - g_map->star_x_ + 0.5*obj_rect_.w - 0.5*width_fram_,
			rect_.y = this->y_pos_ - g_map->star_y_ + 0.5*obj_rect_.h - 0.5*height_fram_,
			rect_.w = width_fram_, 
			rect_.h = height_fram_,
		};

		int check = SDL_RenderCopy(des, p_object_, &effect_clip[fram_], &render_quad);
		SDL_Delay(12);

		fram_++;
	}
	else
	{
		is_show_ = false;
		fram_ = 0;
	}
}