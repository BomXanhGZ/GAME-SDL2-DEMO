#ifndef ROCKET_WARDEN_BOSS_H_
#define ROCKET_WARDEN_BOSS_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"
#include "EffectObject.h"
#include "ChasingRocket.h"
#include "BossPower.h"

class PlayerObject;  //Forward Decleration


#define RW_BOSS_HP 70
#define RW_BOSS_FRAM_NUM 8
#define RW_BOSS_FIRST_POS ((MAX_MAP_X - 2)*64)
#define RW_BOSS_BULLET_NUM 3
#define RW_BOSS_RELOAD_BULLET 50
#define RW_BOSS_BULLET_SPEED 8
#define RW_BOSS_SHOT_INTERVEL 5  //fram
#define RW_BOSS_RESET_HITS_COUNTER_VAL 3
#define RW_BOSS_SHIELD_DURATION 250 //fram
#define RW_BOSS_LOAD_SHUT_VAL 10 //fram


class RocketWardenBoss : public BaseObject
{
public:

	RocketWardenBoss();
	~RocketWardenBoss();

	void InitRocketWardenBoss(SDL_Renderer* des);
	void HandleRocketWardenBoss(SDL_Renderer* des, Map& g_map, ChasingRocket* c_rocket, PlayerObject* p_player, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);
	void Set_Clip();
	void UpdateIMG(SDL_Renderer* des);
	void CheckToMap(Map &g_map);
	void HandleMove(SDL_Renderer* des, PlayerObject* p_player);
	void MakeBullet(SDL_Renderer* des);
	void HandleBullet(SDL_Renderer* des, PlayerObject* player);
	void HandleSield(std::vector<EffectObject*> effect_list);
	void HandleChasingRocket(ChasingRocket* c_rocket);
	void CheckCollision(PlayerObject* p_player, std::vector<EffectObject*> effect_list);


	//Set_function
	void Set_mapXY(float mapx, float mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_x_pos(float xpos) {x_pos_ = xpos; }
	void Set_y_pos(float ypos) {y_pos_ = ypos; }
	void Set_HP(float hp) {hp_ = hp; }

	//Get_fucntion
	float Get_hp() const {return hp_; }



private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	bool on_ground_;
	bool is_RePos_;
	bool is_shut_rocket_;
	float hp_;
	int fram_;
	int Dir_;
	int hits_counter_;
	int load_shut_img_time_val_;
	int shield_duration_;
	int reload_bullet_val_[RW_BOSS_BULLET_NUM];
	int bullet_count_;
	SDL_Rect Fram_clip_[RW_BOSS_FRAM_NUM];
	TileType check_obstacle_tile_;
	BulletObject boss_bullet[RW_BOSS_BULLET_NUM];
	IconBossHp icon_BossHP_;
	BossIcon boss_icon_;

};

#endif //ROCKET_WARDEN_BOSS_H_