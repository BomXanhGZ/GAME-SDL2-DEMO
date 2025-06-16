
#ifndef DINO_THREAT_H_
#define DINO_THREAT_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "EffectObject.h"
#include "GameMap.h"

class PlayerObject; //Forwar Decleration


#define DINO_THREAT_SPEED (PLAYER_SPEED + PLAYER_SPEED*0.3)
#define DINO_THREAT_HP 4
#define DINO_THREAT_FRAM_NUM 8
#define DINO_MOVE_SPACE TILE_SIZE*5
#define DINO_DMG_VAL 2
#define DINP_REATTACK_VAL 7
#define DINO_POS_1 1500
#define DINO_POS_2 2760
#define DINO_POS_3 3835
#define DINO_POS_4 10080


class DinoThreat : public BaseObject
{
public:

	DinoThreat();
	~DinoThreat();

	void InitDinoThreat(SDL_Renderer* des);
	void HandleDinoThreat(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);
	void CheckToMap(Map* g_map);
	void DoThreat(SDL_Renderer* des, PlayerObject* player, std::vector<EffectObject*> effect_list);
	void RestartThreat();
	void Make_DinoPosList();


	//Set_Fucntion
	void Set_mapXY(float mapx, float mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_x_pos(float xpos) {x_pos_ = xpos; }
	void Set_y_pos(float ypos) {y_pos_ = ypos; }
	void Set_DinoThreatHP(float hp) {hp_ = hp; }
	void Set_clip();
	void Set_isHandleDino(bool val) {is_Handle_DinoThreat_ = val; }

	//Get_Function
	bool Get_isHandleDino() const {return is_Handle_DinoThreat_; }

private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	float hp_;
	int dir_;
	int dino_reattack_;
	bool come_back_;
	bool on_ground_;
	bool is_Handle_DinoThreat_;
	SDL_Rect Fram_clip_[DINO_THREAT_FRAM_NUM];
	int fram_;
	TileType check_obstacle_tile_;
	std::vector<float> dino_poslist_;


};

#endif //DINO_THREAT_H_