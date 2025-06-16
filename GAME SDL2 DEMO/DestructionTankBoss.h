#ifndef DESTRUCTION_TANK_H_
#define DESTRUCTION_TANK_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"
#include "EffectObject.h"
#include "BossPower.h"

class PlayerObject; // Forward declaration


//BOSS_LEVEL_2
#define DT_TANK_BOSS_HP 125
#define DT_TANK_BOSS_SPEED 2.5
#define DT_TANK_BOSS_FRAM_NUM 2
#define DT_TANK_BOSS_GIVE_SPACE 0
#define DT_TANK_BOSS_X_POS ((MAX_MAP_X - 2)*64)

#define DT_TANK_BOSS_REGEN_HP_TIME 500;
#define DT_TANK_BOSS_REGEN_HP_VAL DT_TANK_BOSS_HP*0.2;

#define DT_TANK_BOSS_BULLET_SPEED_X 5.5
#define DT_TANK_BOSS_BULLET_SPEED_Y 0.6		  //...
#define DT_TANK_TIME_RELOAD_BULLET 30

#define DRIVER_BULLET_SPEED_X 4
#define DIRVER_BULLET_SPEED_Y 5
#define DRIVER_TIME_RELOAD_BULLET 40


class DestructionTankBoss : public BaseObject
{
public:

	DestructionTankBoss();
	~DestructionTankBoss();

	void Init_boss(SDL_Renderer* des);
	void HandleBoss(Map& map_data, PlayerObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect);
	void HandleMove(Map& map_data, PlayerObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect);
	void ShowBoss(SDL_Renderer* des);
	void CheckToMap(Map& map_data);
	void CheckColision(PlayerObject& player, std::vector<EffectObject*> list_effect);
	void MakeBullet(SDL_Renderer* des);
	void HandleBullet(PlayerObject& player, SDL_Renderer* des, std::vector<EffectObject*> list_effect);
	void ShowBullet(SDL_Renderer* des);
	void ReLoadTankBullet(SDL_Renderer* des);
	void ReloadDriverBullet(SDL_Renderer* des, PlayerObject& player);
	void RegenHP();

	//Set_function
	void Set_x_pos(const int& xpos) {x_pos_ = xpos; }
	void Set_y_pos(const int& ypos) {y_pos_ = ypos; }
	void Set_x_val(const int& xval) {x_val_ = xval; }
	void Set_y_val(const int& yval) {y_val_ = yval; }
	void Set_map_x_y(const int& mapx, const int& mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_clip(const int& fram_num);
	void Set_is_run_boss_lv2(bool is_run) {is_run_DestructionTankBoss_ = is_run; }

	
	//Get_function
	int Get_x_pos() const {return x_pos_; }
	int Get_y_pos() const {return y_pos_; }
	BulletObject Get_boss_bullet_list() {return boss_bullet_;}
	float Get_boss_hp() const {return hp_;}
	bool Get_is_run_boss_lv2() {return is_run_DestructionTankBoss_; }


private:

	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;
	int map_x_;
	int map_y_;
		
	
	bool on_ground_;
	bool is_run_DestructionTankBoss_;
	float hp_;
	int dir_;
	int fram_;
	int regenhp_;
	SDL_Rect clip_[DT_TANK_BOSS_FRAM_NUM];
	IconBossHp icon_bosshp_;
	BossIcon boss_icon_;
	TileType check_obstacle_tile_;
	
	BulletObject boss_bullet_;
	int bullet_type_;
	int reload_bullet_time_;
	
	BulletObject driver_bullet_;
	int driver_bullet_type_;
	int reload_driver_bullet_time_;
};

#endif //DESTRUCTION_TANK_H_