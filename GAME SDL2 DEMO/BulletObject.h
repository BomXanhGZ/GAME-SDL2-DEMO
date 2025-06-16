#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_


#include "FunctionFile.h"
#include "BaseObject.h"


const int BULLET_SPEED = 15;
const int THREAT_BULLET_SPEED = 9;
const int PRO_BULLET_FRAM = 4;

//BULLET DMG
#define STANDARD_DMG       1
#define STAR_DMG		   1.5
#define ROUND_DMG          1.75
#define BALL_DMG		   2
#define ROCKET_DMG         2.25
#define PRO_DMG            2.5

#define RELOAD_BULLET_VAL 50 //fram


class BulletObject : public BaseObject
{
public:

	BulletObject();
	~BulletObject();

	void HandleMove(const int& x_boder, const int& y_boder);
	void HandelMoveThreat(const int& x_limit, const int& y_limit);
	void SetDataBullet(const int& obj_bullet_type);
	void ShowBullet(SDL_Renderer* des);

	//reload_bullet
	void SetReloadBullet(const int val = RELOAD_BULLET_VAL) {reload_bullet_val_ = val; }
	void HandleReloadBullet() {reload_bullet_val_--; }
	int GetReloadBullet() const {return reload_bullet_val_; }


	//Set_Function
	void Set_x_val(const float& x_val) {x_val_ =  x_val; }
	void Set_y_val(const float& y_val) {y_val_ =  y_val; }
	void Set_position(const float& x_pos, const float& y_pos) {x_pos_ =  x_pos, y_pos_ = y_pos; }
	void Set_is_move(const bool& is_move) {is_move_ = is_move; }
	void Set_bullet_dir(const int& bullet_dir) {bullet_dir_ = bullet_dir; }
	void Set_bullet_dmg(float bullet_dmg) {bullet_dmg_ = bullet_dmg; }
	void Set_bullet_fram(const int& bullet_framm_num);
	void SetMapXY(const int& map_x, const int& map_y) {map_x_ = map_x, map_y_ = map_y; }


	//Get_Function
	float Get_x_val() const {return x_val_; }
	float Get_y_val() const {return y_val_; }
	float Get_xpos() const {return x_pos_; }
	float Get_ypos() const {return y_pos_; }
	bool Get_is_move() const {return is_move_; }
	int Get_bullet_dir() const {return bullet_dir_; }
	float Get_bullet_dmg() const {return bullet_dmg_; }
	int Get_bullet_type() const {return bullet_type_; }
	std::string Get_img_bullet_type(const int& walk_);
	int Get_bullet_fram_num() const {return bullet_fram_num_; }
	int Get_width_Bullet() const {return width_fram_; }


private:

	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;
	int map_x_;
	int map_y_;

	bool is_move_;
	int bullet_dir_; 
	int bullet_type_;
	float bullet_dmg_;
	int bullet_fram_num_;


	//pro_bullet
	SDL_Rect bullet_clip_[8];
	int fram_;

	//threat_only
	int reload_bullet_val_;

};

#endif //BULLET_OBJECT_H_