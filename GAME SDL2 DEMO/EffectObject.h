
#ifndef EFFECT_OBJECT_H_
#define EFFECT_OBJECT_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "GameMap.h"


#define MAX_EFFECT_FRAM_NUM 35
#define EXPLOSION_FRAM_NUM 8
#define ROCKET_EXP_FRAM_NUM 10
#define FLASH_FRAM_NUM 12
#define FIRE_FRAM_NUM 12
#define SHIELD_FRAM_NUM 2
#define FLASH_EFFECT_DELAY 1000
#define BLIND_1_FRAM_NUM 8
#define BLIND_2_FRAM_NUM 1
#define WINGAME_FRAM_NUM 2
#define PLAYER_RIP_FRAM_NUM 16
#define CHARGELAZE_EFFECT_FRAM_NUM 16

const int EXP_EFFECT  =  0 ;
const int ROCKET_EXP_EFFECT = 1;
const int FLAST_EFFECT = 2;
const int RUN_BLIND_EFFECT = 3;
const int BLIND_EFFECT = 4;
const int PLAYER_RIP_EFFECT = 5;
const int WINGAME_EFFECT = 6;
const int FIRE_EFFECT = 7;
const int BLUE_SHIELD_EFFECT = 8;
const int CHARGELAZE_EFFECT = 9;
const int GREEN_SHIELD_EFFECT = 10;



class EffectObject : public BaseObject
{
public:

	EffectObject();
	~EffectObject();

	void InitEffect(std::string path, SDL_Renderer* des, int effect_fram_num);
	void ShowEffect(SDL_Renderer* des, Map* g_map);


	//Set_fuction
	void Set_effect_clip();
	void ShowAnimation(const int& animation_type);
	void Set_fram(const int& fram) {fram_ = fram; }
	void Set_is_show(bool val) {is_show_ = val; }
	void SetRect_PositionEffect(const SDL_Rect& obj_rect, const float& xpos, const float& ypos)
							   {obj_rect_ = obj_rect, x_pos_ = xpos, y_pos_ = ypos; }


	//Get_function
	SDL_Rect Get_effect_rect() const {return obj_rect_; }
	int Get_fram() const {return fram_; }
	bool Get_is_show() const {return is_show_; }
	float Get_xpos() const {return x_pos_; }
	float Get_ypos() const {return y_pos_; }
	

private:

	float x_pos_;
	float y_pos_;

	int fram_;
	int fram_num_;
	SDL_Rect effect_clip[MAX_EFFECT_FRAM_NUM];
	SDL_Rect obj_rect_;
	bool is_show_;

};

#endif //EFFECT_OBJECT_H_