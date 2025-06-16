
#ifndef DEATH_EYE_BOSS_H_
#define DEATH_EYE_BOSS_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"
#include "EffectObject.h"
#include "BossPower.h"

class PlayerObject; //Forwar Decleration



//LAZE_BULLET OF EYE_BOSS:

#define LAZE_BULLET_DMG 3.5
#define LAZE_BULLET_SHOW_FRAM 50


class LazeBullet : public BaseObject
{
public:

	LazeBullet();
	~LazeBullet();

	void InitLazeBullet(SDL_Renderer* des);
	void HandleLazeBullet(SDL_Renderer* des, float map_x, float map_y, int dir_,
						  SDL_Rect obj_rect, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);

	//Set_fucntion
	void Set_IsShow(bool is_shut) {is_show_ = is_shut; }
	void Set_mapXY(float map_x, float map_y) {map_x_ = map_x, map_y_ = map_y; }
	void SetPos(float x_pos, float y_pos) {x_pos_ = x_pos, y_pos_ = y_pos; }
	void Set_ShowTime(int time_val) {Show_time_ = time_val; }

	//Get_fucntion
	bool Get_IsShow() const {return is_show_; }
	int Get_ShowTime() const {return Show_time_; }

private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;

	bool is_show_;
	int Show_time_;

};



//THUNDER WEAPON

#define THUNDER_WEAPON_FRAM_NUM 11
#define THUNDER_MOVE_SPEED 3.5
#define THUNDER_SHOW_TIME 300


class ThunderWeapon : public BaseObject
{
public:
	
	ThunderWeapon();
	~ThunderWeapon();

	void InitThunderWeapon(SDL_Renderer* des);
	void HandleThundeWeapon(Map* g_map, PlayerObject* p_player, std::vector<EffectObject* > list_effect);
	void ShowThunder(SDL_Renderer* des);
	void SetClip();

	//Set_function
	void Set_mapXY(float mapx, float mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_x_pos(float xpos) {x_pos_ = xpos; }
	void Set_IsUseThunderWeapon(bool is_use) {is_use_thunderweapon = is_use; }
	void Set_Direction(int dir) {Dir_ = dir; } 

	//Get_fuction
	bool Get_is_use_TdWeapon() const {return is_use_thunderweapon; }
	int Get_Direction() const {return Dir_; }


private:

	float map_x_;
	float map_y_;
	float x_pos_;
	float y_pos_;
	float x_val_;

	bool is_use_thunderweapon;
	int Dir_;
	int thunder_show_time_;
	int fram_;
	SDL_Rect Fram_clip_[THUNDER_WEAPON_FRAM_NUM];


};



//DEATH EYE BOSS

#define DEATH_EYE_BOSS_HP 100
#define DEATH_EYE_BOSS_FRAM_NUM 8
#define DEATH_EYE_BOSS_FIRST_POS ((MAX_MAP_X - 2)*64)
#define DEATH_EYE_BOSS_CHANGE_MODE_VAL 0.4 //0.4

#define DEATH_EYE_BOSS_CALM_SPEED 3.5
#define DEATH_EYE_BOSS_CALM_SPACING_VAL TILE_SIZE*7
#define DEATH_EYE_BOSS_CALM_MODE_MAX_HITS_COUNTER 5
#define DEATH_EYE_BOSS_MAX_STOP_MOVE_VAL TILE_SIZE*9
#define DEATH_EYE_BOSS_RELOAD_LAZE_VAL 50 // fram

#define DEATH_EYE_BOSS_RAGE_SPEED 8
#define DEATH_EYE_BOSS_RAGE_MOVE_DISTANCE_X TILE_SIZE*30
#define DEATH_EYE_BOSS_RAGE_MOVE_DISTANCE_Y TILE_SIZE*15
#define DEATH_EYE_BOSS_RAGE_MODE_MAX_HITS_COUNTER 5

#define DEATH_EYE_BOSS_MAX_CHARGE_THUNDERWEARPON 3


class DeathEyeBoss : public BaseObject
{
public:

	enum RandRePosType
	{
		POS_LEFT = 0,
		POS_RIGHT = 1,
		POS_UP = 2,
		POS_DOWN = 3
	};

	DeathEyeBoss();
	~DeathEyeBoss();

	void InitDeathEyeBoss(SDL_Renderer* des);
	void HandleDeathEyeBoss(SDL_Renderer* des, Map& g_map, PlayerObject* p_player, std::vector<EffectObject*> effect_list);
	void Show(SDL_Renderer* des);
	void Set_clip();
	void UpdateIMG(SDL_Renderer* des);
	void HandleMove(SDL_Renderer* des, PlayerObject* p_player, std::vector<EffectObject*> effect_list);
	void HandleLazeBullet(SDL_Renderer* des, std::vector<EffectObject*> effect_list);
	void HandleThunderWeapon(SDL_Renderer* des, Map* g_map, PlayerObject* p_player, std::vector<EffectObject* > list_effect);
	void CheckCollision(PlayerObject* p_player, std::vector<EffectObject*> effect_list);
	void RePosBoss();


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

	float hp_;
	int Dir_;
	int fram_;
	int hits_counter_;
	int reload_laze_val_;
	int charge_thuntherweapon_counter_;
	bool is_RePos_;
	bool is_charge_laze_;
	bool is_shut_;
	bool is_attack_while_rage_;
	SDL_Rect Fram_clip_[DEATH_EYE_BOSS_FRAM_NUM];
	LazeBullet deatheye_lazebullet_;
	ThunderWeapon deatheye_thunderweapon;
	IconBossHp icon_BossHP_;
	BossIcon boss_icon_;

};

#endif //DEATH_EYE_BOSS_H_