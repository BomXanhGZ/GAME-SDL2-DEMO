#ifndef AERIAL_THREAT_H_
#define AERIAL_THREAT_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "BulletObject.h"
#include "EffectObject.h"

class PlayerObject; // Forward declaration


//AERIAL_THREAT
#define AERIAL_THREAT_HP 7
#define AERIAL_THREAT_SPEED 5
#define AERIAL_THREAT_FRAM_NUM 1
#define AERIAL_THREAT_POS_1 2240
#define AERIAL_THREAT_POS_2 3200
#define AERIAL_THREAT_POS_3 4500
#define MAX_CB_TURN 3
#define AERIAL_THREAT_RELOAD_TIME_BULLET 25



class AerialThreat : public BaseObject
{

public:

	AerialThreat();
	~AerialThreat();

	void InitAerialThreat(SDL_Renderer* des);
	void HandleThreat(Map& map_data, SDL_Renderer* des, PlayerObject& player, std::vector<EffectObject*> list_effect);
	void Show(SDL_Renderer* des);
	void DoThreat(Map& map_data, SDL_Renderer* des); 
	void HandleMove(SDL_Renderer* des);
	void MakeIsBullet(SDL_Renderer* des);
	void ReloadIsBullet(SDL_Renderer* des, const SDL_Rect& player_rect);
	SDL_Rect HandleIsBullet(SDL_Renderer* des, int x_bdr, int y_bdr, std::vector<EffectObject*> list_effect);
	void ReloadBullet(const int& reload_time, std::vector<EffectObject*> list_effect);//

	//Set_Function
	void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x, map_y_ = map_y; }
	void Set_x_val(const int& x_val) {x_val_ = x_val; }
	void Set_y_val(const int & y_val) {y_val_ = y_val; }
	void SetPosition(float y_pos, float x_pos) {y_pos_ = y_pos, x_pos_ = x_pos ; }
	void Set_threat_hp(const float& hp) {threat_hp_ = hp; }
	void Set_reload_time_bullet(int reload_time) {reload_time_bullet_ = reload_time; } 
	void Set_is_show_effec(bool val) {is_show_effect_ = val; }

	//Get_Function
	int GetWidthFram() const {return width_fram_; }
	int GetHeightFram() const {return height_fram_; }
	BulletObject GetThreatBullet() {return p_bullet; } 
	float Get_threat_hp() {return threat_hp_; }
	bool Get_is_show_effect() const {return is_show_effect_; }



private:

	int map_x_;
	int map_y_;
	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;

	bool is_show_effect_;
	float threat_hp_;
	int threat_type_;
	int thr_bullet_type_;
	int reload_time_bullet_;
	int cameback_turn_;
	int dir_;
	BulletObject p_bullet;
	
};

#endif //AERIAL_THREAT_H_