
#ifndef CHASING_ROCKET_H_
#define CHASING_ROCKET_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "EffectObject.h"
#include "GameMap.h"

class PlayerObject; //Forwar decleration


#define CHASING_ROCKET_SPEED 7
#define CHASING_ROCKET_HOMING 2.75
#define CHASING_ROCKET_FRAM_NUM 2
#define CHASING_ROCKET_DMG_VAL 5
#define CHASING_ROCKET_HP 4.5
#define CHASING_ROCKET_POS_1 2820
#define CHASING_ROCKET_POS_2 5800
#define CHASING_ROCKET_POS_3 7750
#define CHASING_ROCKET_POS_4 9150
#define CHASING_ROCKET_POS_5 10080
#define CHASING_ROCKET_POS_6 12650


class ChasingRocket : public BaseObject
{
public:
	
	ChasingRocket::ChasingRocket();
	ChasingRocket::~ChasingRocket();

	void InitChasingRoccket(SDL_Renderer* des);
	void HandleChasingRocket(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);
	void ChasingRocketHandleMove(PlayerObject* player);
	void DoChasingRocket(PlayerObject* player, std::vector<EffectObject*> effect_list);
	void RestartThreat();
	void Make_ChasingRocketPosList();
	void Set_clip();


	//Set_fucntion
	void Set_mapXY(float mapx, float mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_x_pos(float xpos) {x_pos_ = xpos; }
	void Set_y_pos(float ypos) {y_pos_ = ypos; }
	void Set_ChasingRocketHP(float hp) {hp_ = hp; }
	void Set_IsShow(bool val) {is_show_ = val; }

	//Get_fucntion
	bool Get_IsShow() const {return is_show_; }

private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	float hp_;
	int fram_;
	bool is_RePos_;
	bool is_move_;
	bool is_show_;
	SDL_Rect Fram_clip_[CHASING_ROCKET_FRAM_NUM];
	std::vector<int> chasing_rocket_poslist_;
};

#endif //CHASING_ROCKET_H_