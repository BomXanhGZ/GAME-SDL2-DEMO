
#ifndef FIRE_RAIN_H_
#define FIRE_RAIN_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "EffectObject.h"
#include "GameMap.h"
#include "PlayerObject.h"


#define FIRE_NUM          2
#define FIRERAIN_FRAM_NUM 8
#define FIRE_SPEED_X      1.5   //2
#define FIRE_SPEED_Y      FIRE_SPEED_X*2
#define FIRE_DMG_VAL      1
#define FIRE_Y_START      0
#define FIRE_X_START      200

class FireRain : public BaseObject
{
public:

	FireRain();
	~FireRain();

	std::vector<FireRain*> InitFireRain(SDL_Renderer* des);
	void HandleFireRain(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);
	void Set_clip();
	void CheckToMap(Map* g_map, std::vector<EffectObject*> effect_list);
	void RePosRad();

	//Set_fucntion
	void Set_mapXY(float mapx, float mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_x_pos(float xpos) {x_pos_ = xpos; }
	void Set_y_pos(float ypos) {y_pos_ = ypos; }

	//Get_fucntion


private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	int fram_;
	bool is_ReShow_;
	SDL_Rect Fram_clip_[FIRE_FRAM_NUM];
	TileType check_obstacle_tile_;
};


#endif //FIRE_RAIN_H_