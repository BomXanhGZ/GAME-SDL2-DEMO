#ifndef GUARD_THREAT_H_
#define GUARD_THREAT_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "BulletObject.h"
#include "EffectObject.h"

class PlayerObject; // Forward declaration


//ON_GROUND_THREAT
#define GUARD_THREAT_NUM 2
#define GUARD_THREAT_FRAM_NUM 8
#define GUARD_THREAT_SPEED 4
#define GUARD_THREAT_MOVE_SPACE 3  //tile
#define BULLET_SPACE_LIMIT 7*TILE_SIZE
#define GUARD_THREAT_RELOAD_BULLET_TIME 20
#define GUARD_THREAT_CAMEBACK 1250
#define GUARD_THREAT_HP 3

//MAP1 POS
#define M1_POS_1  830
#define M1_POS_2  1475
#define M1_POS_3  2800
#define M1_POS_4  3500
#define M1_POS_5  5000
#define M1_POS_6  6000
#define M1_POS_7  7200
#define M1_POS_8  8350
#define M1_POS_9  9500
#define M1_POS_10 10200
#define M1_POS_WHILE_SHOW_BOSS_1 11500
#define M1_POS_WHILE_SHOW_BOSS_2 12500

//MAP2 POS
#define M2_POS_1 1750
#define M2_POS_2 4475
#define M2_POS_3 5450
#define M2_POS_4 7690
#define M2_POS_5 8265
#define M2_POS_6 9680
#define M2_POS_7 12500

//MAP3 POS0
#define M3_POS_1 950
#define M3_POS_2 2600
#define M3_POS_3 3650
#define M3_POS_4 5050
#define M3_POS_5 6100
#define M3_POS_6 6600
#define M3_POS_7 8380
#define M3_POS_8 11000


typedef struct InPut
{
	int up_;
	int down_;
	int left_;
	int right_;
};


class GuardThreat : public BaseObject
{
public:

	GuardThreat();
	~GuardThreat();

	std::vector<GuardThreat*> InitThreats(SDL_Renderer* des, const int& map_num);
	void HandleThreat(Map& map_data, SDL_Renderer* des, PlayerObject& player, std::vector<EffectObject*> list_effect);
	void Show(SDL_Renderer* des);
	void DoThreat(Map& map_data, SDL_Renderer* des);
	void CheckToMap(Map& map_data);
	void HandleMove(SDL_Renderer* des);
	void MakeOgBullet(SDL_Renderer* des);
	void ReloadOgBullet(SDL_Renderer* des, const SDL_Rect& player_rect);
	SDL_Rect HandleOgBullet(SDL_Renderer* des, int x_bdr, int y_bdr, std::vector<EffectObject*> list_effect);
	void RestartThreat();
	void SetReloadBullet(const int& reload_time, std::vector<EffectObject*> list_effect);
	void Make_guardthreat_pos_list(const int& map_num);
	float GetXposWithMapNum(const int& idx, const int& map_num);
	std::string ChangeIMG_OgThreat();

	//Set_Function
	void SetClip();
	void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x, map_y_ = map_y; }
	void Set_x_val(const int& x_val) {x_val_ = x_val; }
	void Set_y_val(const int & y_val) {y_val_ = y_val; }
	void SetPosition(float y_pos, float x_pos) {y_pos_ = y_pos, x_pos_ = x_pos ; }
	void Set_threat_hp(const float& hp) {threat_hp_ = hp; }
	void Set_reload_time_bullet(int reload_time) {reload_time_bullet_ = reload_time; } 
	void Set_is_show_effec(bool val) {is_show_effect_ = val; }
	void Set_MapNum(const int& map_num) {map_num_ = map_num; }

	//Get_Function
	int GetWidthFram() const {return width_fram_; }
	int GetHeightFram() const {return height_fram_; }
	int Get_threat_type() {return threat_type_; }
	int Get_MapNum() const {return map_num_; }
	bool Get_onground() const {return on_ground_;}
	bool GetIsHandle() const {return is_handle_; }
	bool Get_is_show_effect() const {return is_show_effect_; }
	float Get_threat_hp() {return threat_hp_; }
	BulletObject GetThreatBullet() {return p_bullet; } 




private:

	int map_x_;
	int map_y_;
	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;

	bool come_back_;
	bool on_ground_;
	bool is_handle_;
	bool is_show_effect_;
	float threat_hp_;
	float move_space_;
	int come_back_time_;
	int fram_;
	int dir_;
	int map_num_;
	int threat_type_;
	int thr_bullet_type_;
	int reload_time_bullet_;
	SDL_Rect Fram_clip_[GUARD_THREAT_FRAM_NUM];
	BulletObject p_bullet;
	InPut input_type_;
	TileType check_obstacle_tile_;

};

#endif //GUARD_THREAT_H_