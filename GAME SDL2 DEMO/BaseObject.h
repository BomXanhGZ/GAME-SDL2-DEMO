#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "FunctionFile.h"

class BaseObject
{
public:

	BaseObject();
	~BaseObject();

	void Free();
	void Render(SDL_Renderer* des, SDL_Rect* fram_rect = NULL);

	//set_function
	void SetRect(const int& x, const int& y) {rect_.x = x, rect_.y = y ;}
	bool LoadImg(std::string path, SDL_Renderer* des);
	bool LoadAnimation(std::string path, SDL_Renderer* des, const int& obj_framm_num);

	//get_function
	SDL_Rect GetRect() const {return rect_;}
	SDL_Rect GetRectWhileHaveNumFramIs(const int& fram_num);
	SDL_Texture* GetObject() const {return p_object_;}
	int GetHeightFram() const {return height_fram_; }
	int GetWidthFram() const {return width_fram_; }
	

protected:
	SDL_Texture* p_object_;
	SDL_Rect rect_;
	int width_fram_;
	int height_fram_;
};


#endif //BASE_OBJECT_H_