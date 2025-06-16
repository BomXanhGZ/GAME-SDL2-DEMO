
#ifndef MENU_OBJECT_H_
#define MENU_OBJECT_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "TextObject.h"


#define X_BOMXANH_TEXT 35
#define Y_BOMXANH_TEXT 35

#define X_TEXT 550

#define MENUGAME_Y_TEXT 200
#define START_Y_TEXT 275
#define CHANGE_MAP_Y_TEXT 350
#define EXIT_Y_TEXT 425
#define CONTINUE_Y_TEXT 200
#define QUIT_Y_TEXT 300





class MenuObject
{
public:

	enum SelectOption
	{
		EXIT_SELECT = 0,
		CONTINIUE_SELECT = 1,
		QUIT_SELECT = 2,
		START_SELECT = 3,
	};

	MenuObject();
	~MenuObject();

	void InitMenu(SDL_Renderer* des, TTF_Font* font);
	int RunMenu(SDL_Renderer* des, TTF_Font* font);
	int RunPause(SDL_Renderer* des, TTF_Font* font);

	//Set_function
	void Set_MapNumber(int num) {map_number_ = num; }

	//Get_fucntion
	int Get_MapNumBer() const {return map_number_; }


private:

	BaseObject menu_background_;
	int map_number_;

	//const_text
	TextObject menugame_text_;
	TextObject bomxanh_text_;

	//select_text
	TextObject start_text_;
	TextObject exit_text_;
	TextObject continue_text_;
	TextObject quit_text_;
	TextObject change_map_text_;

};

#endif //MENU_OBJECT_H_