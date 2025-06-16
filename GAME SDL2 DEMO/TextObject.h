
#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_


#include "FunctionFile.h"


#define PLAYER_LIVES_RECT_Y 5
#define PLAYER_LIVES_RECT_X 16
#define TIME_GAME_RECT_Y 5
#define TIME_GAME_RECT_X (SCREEN_WIDTH/2 - TILE_SIZE)
#define FPS_SHOW_RECT_Y 5
#define FPS_SHOW_RECT_X (SCREEN_WIDTH*3/4 - TILE_SIZE)



class TextObject
{
public:

	enum TextColor
	{
		RED_TEXT = 0,
		WHILE_TEXT = 1,
		BLACK_TEXT = 2,
		BLUE_TEXT = 3,
		GREEN_TEXT = 4,
		YELLOW_TEXT = 5,
		ORANGE_TEXT = 6

	};

	TextObject();
	~TextObject();

	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* des);
	void Free();

	void RenderText(SDL_Renderer* des,
					int xp, int yp, 
					SDL_Rect* clip = NULL, 
					double angle = 0.0, 
					SDL_Point* center = NULL, 
					SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Set_function
	void SetColor(Uint8 red, Uint8 Green, Uint8 blue);
	void SetColor(int type);
	void SetText(const std::string& text) {str_val_ = text; }

	//Get_Function
	int GetWidth() const {return width_; }
	int GetHeight() const {return height_; }
	std::string GetText() const {return str_val_; }


private:

	std::string str_val_;
	SDL_Color text_color_;
	SDL_Texture* texttrue_;
	int width_;
	int height_;
};



#endif //TEXT_OBJECT_H_