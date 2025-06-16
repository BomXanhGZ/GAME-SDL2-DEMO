
#include "BaseObject.h"


BaseObject::BaseObject()
{
	p_object_ = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;

	width_fram_ = 0;
	height_fram_ = 0;
}

BaseObject::~BaseObject()
{
	Free();
}

SDL_Rect BaseObject::GetRectWhileHaveNumFramIs(const int& fram_num)
{
	SDL_Rect rect_obj;
	rect_obj.x = rect_.x;
	rect_obj.y = rect_.y;
	rect_obj.w = rect_.w / fram_num;
	rect_obj.h = rect_.h;

	return rect_obj;
}

bool BaseObject::LoadAnimation(std::string path, SDL_Renderer* des, const int& obj_framm_num)
{
	bool ret = LoadImg(path, des);

	if(ret)
	{
		width_fram_ = rect_.w / obj_framm_num;
		height_fram_ = rect_.h;
	}

	return ret;
}

bool BaseObject::LoadImg(std::string path, SDL_Renderer* des)
{
	Free();

	SDL_Texture* new_texture = NULL;

	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if(load_surface != NULL)
	{
		Uint32 color = SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);
		SDL_SetColorKey(load_surface, SDL_TRUE, color);

		new_texture = SDL_CreateTextureFromSurface(des, load_surface);
		if(new_texture != NULL)
		{
			rect_.w = load_surface->w;
			rect_.h = load_surface->h;
		}
		else
		{
			SDL_GetError();
			std::cout << std::endl;
		}

		SDL_FreeSurface(load_surface);
	}
	p_object_ = new_texture;

	return p_object_ != NULL;
}


void BaseObject::Render(SDL_Renderer* des, SDL_Rect* fram_rect /* = NULL */)
{
	SDL_Rect renderquad_rect  = {rect_.x, rect_.y, rect_.w, rect_.h};
	if (rect_.w == 0 || rect_.h == 0) 
	{
		std::cerr << "Invalid dimensions for rect_: " << rect_.w << "x" << rect_.h << std::endl;
		return;
	}

    int check = SDL_RenderCopy(des, p_object_, fram_rect, &renderquad_rect);
	if(check < 0)
	{
		 std::cerr << "SDL_RenderCopy Error: " << SDL_GetError() << std::endl;
	}	
}


void BaseObject::Free()
{
	if(p_object_ != NULL)
	{
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}