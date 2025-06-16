
#ifndef ROCK_TITAN_H_
#define ROCK_TITAN_H_


#include "FunctionFile.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"
#include "EffectObject.h"
#include "BossPower.h"

class PlayerObject; // Forward declaration


//BOSS_LEVEL_1
#define ROCK_TITAN_BOSS_HP 100
#define ROCK_TITAN_BOSS_SPEED 4
#define ROCK_TITAN_BOSS_FRAM_NUM 8
#define ROCK_TITAN_BOSS_X_POS  ((MAX_MAP_X - 2)*64)

#define ROCK_TITAN_BOSS_BULLET_NUM 3
#define ROCK_TITAN_BOSS_BULLET_SPEED_X 4.5
#define ROCK_TITAN_BOSS_BULLET_SPEED_Y 0.675


class RockTitanBoss : public BaseObject
{
public:

	RockTitanBoss();
	~RockTitanBoss();

	void Init_boss(SDL_Renderer* des);
	void HandleBoss(SDL_Renderer* des, Map& map_data, PlayerObject& player, std::vector<EffectObject*> list_effect);
	void HanldeMove();
	void ShowBoss(SDL_Renderer* des);
	void CheckToMap(Map& map_data);
	void CheckColision(PlayerObject& player, std::vector<EffectObject*> list_effect);
	void MakeBullet(SDL_Renderer* des);
	void HandleBullet(PlayerObject& player, std::vector<EffectObject*> list_effect);
	void ShowBullet(SDL_Renderer* des);

	//Set_function
	void Set_x_pos(const int& xpos) {x_pos_ = xpos; }
	void Set_y_pos(const int& ypos) {y_pos_ = ypos; }
	void Set_x_val(const int& xval) {x_val_ = xval; }
	void Set_y_val(const int& yval) {y_val_ = yval; }
	void Set_map_x_y(const int& mapx, const int& mapy) {map_x_ = mapx, map_y_ = mapy; }
	void Set_clip(const int& fram_num);

	
	//Get_function
	int Get_x_pos() const {return x_pos_; }
	int Get_y_pos() const {return y_pos_; }
	std::vector<BulletObject*> Get_boss_bullet_list() {return boss_bullet_list;}
	float Get_boss_hp() const {return boss_hp_;}


private:

	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;
	int map_x_;
	int map_y_;

	bool on_ground_;
	bool change_angle_;
	float boss_hp_;
	int fram_;
	int bullet_type_;
	SDL_Rect clip_[ROCK_TITAN_BOSS_FRAM_NUM];
	std::vector<BulletObject*> boss_bullet_list;
	TileType check_obstacle_tile_;
	IconBossHp icon_BossHP_;

};

#endif //ROCK_TITAN_H_