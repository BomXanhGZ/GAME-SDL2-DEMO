
#ifndef STATIC_THREAT_H_
#define STATIC_THREAT_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "EffectObject.h"
#include "GameMap.h"

class PlayerObject; //Forwar Declaration


//STATIC THREAT
#define STATIC_THREAT_NUM 2
#define STATIC_THREAT_FRAM_NUM 8
#define STATIC_THREAT_HP 6
#define STATIC_THREAT_BULLET_NUM 3
#define STATIC_THREAT_BULLET_SPEED_X 7
#define STATIC_THREAT_BULLET_SPEED_Y 2.4
#define STATIC_POS_1 1215
#define STATIC_POS_2 2430
#define STATIC_POS_3 2815
#define STATIC_POS_4 4605
#define STATIC_POS_5 6590
#define STATIC_POS_6 8764


class StaticThreat : public BaseObject
{
public:

	StaticThreat();
	~StaticThreat();

	std::vector<StaticThreat*> InitStaticThreat(SDL_Renderer* des);
	void Handle(SDL_Renderer* des, Map* g_map, PlayerObject* player, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);
	void DoThreat(SDL_Renderer* des, PlayerObject* player, std::vector<EffectObject*> effect_list);
	void CheckToMap(Map* g_map);
	void RestartThreat();
	void MakeBullet(SDL_Renderer* des);
	void HandleBullet(SDL_Renderer* des, Map* g_map);
	void Make_StaticThreat_pos_list();

	//Set_fucntion
	void Set_mapXY(float mapx, float mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_x_pos(float xpos) {x_pos_ = xpos; }
	void Set_y_pos(float ypos) {y_pos_ = ypos; }
	void Set_StaticThreatHP(float hp) {hp_ = hp; }
	void Set_clip();
	void Set_BulletType(int b_type) {bullet_type_ = b_type; }

	//Get_fucntion
	int Get_BulletType() const {return bullet_type_; }
	int Get_IsHanle_StaticThreat() {return is_handle_static_threat_; }

private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	bool is_shut_;
	bool is_translate_;
	bool on_ground_;
	bool come_back_;
	bool is_handle_static_threat_;
	float hp_;
	int dir_;
	BulletObject bullet_[STATIC_THREAT_BULLET_NUM];
	int bullet_type_;
	SDL_Rect Fram_clip_[STATIC_THREAT_FRAM_NUM];
	int fram_;
	TileType check_obstacle_tile_;

};


#endif //STATIC_THREAT_H_