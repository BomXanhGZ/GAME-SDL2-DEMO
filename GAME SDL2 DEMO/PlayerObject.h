#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_


#include <vector>
#include "FunctionFile.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "BulletObject.h"
#include "PlayerPower.h"
#include "EffectObject.h"


//PLAYER
#define PLAYER_FRAM_NUM    8
#define PLAYER_SPEED       6        //6
#define PLAYER_JUMP_SPEED  14		//14
#define PLAYER_LIVES       5
#define PLAYER_HP          5
#define DIE_VAL            5
#define SHIELD_DURATION    150


typedef struct Input    //ctdl move action main
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
};


class PlayerObject : public BaseObject
{
public:

	PlayerObject(); 
	~PlayerObject();

	void Show(SDL_Renderer* des);
	void HandelInputAction(SDL_Event events, SDL_Renderer* des);
	void Doplayer(Map& map_data, SDL_Renderer* des, std::vector<EffectObject*> list_effect);
	void Init_PowerShow(SDL_Renderer* des);
	void ShowPower(SDL_Renderer* des);
	void HandlePowerPlayer(const int& dmg, std::vector<EffectObject*> list_effect);
	void CheckToMap(Map& map_data, SDL_Renderer* des, std::vector<EffectObject*> list_effect);
	void CenteEntityOnMap(Map& map_data, SDL_Renderer* des);
	void UpdateImgPlayer(SDL_Renderer* des);
	void HandleBullet(SDL_Renderer* des);
	void CheckBulletTile(Map& map_data, int& y, int& x, SDL_Renderer* des);
	void CheckPowerUpTile(Map& map_data, int& y, int& x);
	void CheckInvisibleTile(Map& map_data, int& y, int& x);
	void CheckTrapTile(Map& map_data, const int& y, const int& x, std::vector<EffectObject*> effect_list);

	//Set_function
	void SetClip();
	void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x, map_y_ = map_y; }
	void Set_bullet_list(std::vector<BulletObject*> bulet_list) {p_bullet_list_ = bulet_list; }
	void Set_check_tile();

	//Get_function
	float Get_x_pos() const {return x_pos_; }
	float Get_y_pos() const {return y_pos_; }
	int GetStatus() const {return status_;}
	int GetWidthFram() const {return width_fram_; }
	int GetHeightFram() const {return height_fram_; }
	std::vector<BulletObject*> Get_bullet_list() {return p_bullet_list_; }
	int Get_ply_bullet_type() const {return ply_bullet_type_; }
	int Get_bFramNum_BulletType();
	int Get_ply_lives() const {return player_lives_; }


private:

	int map_x_;
	int map_y_;
	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;

	bool check_up_;
	bool check_action_;
	bool on_ground_;
	bool come_back_;
	int player_hp_;
	int player_lives_;
	int ply_bullet_type_;
	int status_;
	int check_dir_;
	int fram_;
	float shield_duration_;
	SDL_Rect fram_clip_[PLAYER_FRAM_NUM];
	std::vector<int> check_tile_;
	std::vector<BulletObject*> p_bullet_list_;
	HpIcon Hp_icon_;
	PlayerLivesIcon lives_icon_;
	AmmoIcon ammo_icon_;
	Input input_type_;

};


#endif //MAIN_OBJECT_H_