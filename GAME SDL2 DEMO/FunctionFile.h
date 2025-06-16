#ifndef FUNCTION_FILE_H_
#define FUNCTION_FILE_H_


#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <stdlib.h>
#include <ctime>



//SCREEN
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#define SCREEN_BPP 32

#define COLOR_KEY_R 167
#define COLOR_KEY_G 175
#define COLOR_KEY_B 180

#define RENDER_DRAW_COLOR 255



//GAME
#define FRAM_PED_SECOND     60		//FPS
#define TIME_GAME           350     //TIME_GAME
#define GRAVITY_SPEED       0.5
#define MAX_FALL_SPEED      10



//CONST_VARIABLE
const int WINGAME_DELAY_VAL = 300;
const int MAP_NUM = 3;
#define Y_START 165
#define INERTIA 3



//GLOBAL VARIABLE
extern SDL_Renderer* g_screen;
extern SDL_Event g_event;
extern SDL_Window* g_window;

extern TTF_Font* g_font;

extern std::vector<Mix_Chunk*> g_audio;

 /*
extern Mix_Chunk* g_exp_sound[];
extern Mix_Chunk* g_bullet_sound[];
extern Mix_Chunk* g_SelectItem_sound[];
extern Mix_Chunk* g_music_game[];
extern Mix_Chunk* g_another_sound[];
extern Mix_Chunk* g_player_sound[];
*/

extern std::vector<int> g_OG_threat_pos_list_;
extern std::vector<int> g_map_threat_pos_list_;

extern int wingame_delay;

//OPTION
extern bool is_quit;									//QUIT_GAME
extern bool is_exit;									//EXIT_GAME
extern bool is_pause;									//PAUSE_GAME


//IMG LINK

const std::string back_ground_menu_img[MAP_NUM] =									//--------background
{
	 "img/background_menu1.png",
	 "img/background_menu2.png",
	 "img/background_menu3.png"
};

const std::string back_ground_img[MAP_NUM] = 
{
	 "img/background1.png",	
	 "img/background2.png",
	 "img/background3.png"
};


const std::string map_path[MAP_NUM]    =											//--------map
{
	"map/map1.dat",
	"map/map2.dat",
	"map/map3.dat"
};


const std::string standard_threat_left_img[MAP_NUM] =                               //---------global_threat
{
	"img/ninja_left.png",
	"img/theif_left.png",
	"img/solider_left.png"
};

const std::string standard_threat_right_img[MAP_NUM] = 
{
	"img/ninja_right.png",
	"img/theif_right.png",
	"img/solider_right.png"

};

const char aerial_threat_left_img[]	   =     "img/aerial_left.png";                 //---------map_threat
const char aerial_threat_right_img[]   =     "img/aerial_right.png";
const char static_threat_left_img[]    =	 "img/static_threat_left.png";
const char static_threat_right_img[]   =	 "img/static_threat_right.png";
const char death_dino_left_img[]	   =	 "img/dino_left.png";
const char death_dino_right_img[]	   =	 "img/dino_right.png";
const char chasing_rocket_img[]		   =	 "img/chasing_rocket_left.png";
const char fire_rain_left_img[]		   =	 "img/fire_rain_left.png";


const char player_left_img[]           =     "img/player_left.png";                 //--------player
const char player_right_img[]          =     "img/player_right.png";
const char player_jump_left_img[]      =     "img/player_jump_left.png";
const char player_jump_right_img[]     =     "img/player_jump_right.png";
const char player_lives_img[]		   =     "img/player_lives.png";
const char player_hp_img[]			   =     "img/player_hp.png";
          

const char boss_hp_img[]			   =	 "img/boss_hp.png";                     //-------boss

const char rock_titan_boss_img[]	   =     "img/stone_statue_boss.png";
const char tank_left_img[]			   =     "img/tank_left.png";
const char tank_right_img[]			   =	 "img/tank_right.png";
const char tankboss_icon_img[]		   =	 "img/tankbossicon.png";

const char RWboss_left_img[]		   =	 "img/RWboss_left.png";
const char RWboss_right_img[]		   =	 "img/RWboss_right.png";
const char RWboss_shut_left_img[]	   =	 "img/RWboss_shut_left.png";
const char RWboss_shut_right_img[]	   =	 "img/RWboss_shut_right.png";
const char RWboss_icon_img[]		   =	 "img/RWbossicon.png";

const char eye_boss_left[]			   =	 "img/eye_boss_left.png";
const char eye_boss_right[]			   =	 "img/eye_boss_right.png";
const char eye_boss_up_left[]		   =	 "img/eye_boss_up_left.png";
const char eye_boss_up_right[]		   =	 "img/eye_boss_up_right.png";
const char eye_boss_down_left[]		   =	 "img/eye_boss_down_left.png";
const char eye_boss_down_right[]	   =	 "img/eye_boss_down_right.png";
const char eye_boss_RageMode_left[]	   =	 "img/eye_boss_rage_mode_left.png";
const char eye_boss_RageMode_right[]   =	 "img/eye_boss_rage_mode_right.png";
const char eye_boss_icon_img[]		   =	 "img/deatheyebossicon.png";
const char EyeBoss_LazeBullet[]		   =	 "img/eyebosslazebullet.png";
const char EyeBoss_ThunderWeapon[]	   =	 "img/thunder.png";



const char standard_bullet_img[]	   =     "img/standard_bullet.png";             //-------bullet
const char round_bullet_img[]          =     "img/round_bullet.png";
const char star_bullet_img[]           =     "img/star_bullet.png";
const char ball_bullet_img[]           =     "img/ball_bullet.png";


const char rocket_bullet_left_img[]	   =     "img/walk_left_rocket_bullet.png";     //-------rocket_bullet
const char rocket_bullet_right_img[]   =     "img/walk_right_rocket_bullet.png";
const char rocket_bullet_up_img[]	   =     "img/walk_up_rocket_bullet.png";
const char rocket_bullet_down_img[]	   =     "img/walk_down_rocket_bullet.png";

const char pro_bullet_icon_img[]       =     "img/pro_bullet_icon.png";
const char pro_bullet_up_img[]         =     "img/pro_bullet_up.png";               //-------pro_bullet
const char pro_bullet_down_img[]       =     "img/pro_bullet_down.png";
const char pro_bullet_right_img[]      =     "img/pro_bullet_right.png";
const char pro_bullet_left_img[]       =     "img/pro_bullet_left.png";


const char explosion_img[]			   =     "img/explosion.png";                   //-------effect
const char flash_effect_img[]		   =	 "img/flash_effect.png";
const char run_blind_effect_img[]	   =	 "img/blind_effect1.png";
const char blind_effect_img[]		   =     "img/blind_effect2.png";
const char win_effect_img[]			   =	 "img/win.png";
const char fire_effect_img[]		   =	 "img/fire.png";
const char player_rip_effect_img[]	   =	 "img/rip.png";
const char rocket_explosion_img[]	   =	 "img/rocket_explosion.png";
const char blue_shield_img[]		   =	 "img/blue_shield.png";
const char green_shield_img[]		   =	 "img/green_shield.png";
const char charge_bullet_effect_img[]  =	 "img/charge_laze_effect.png";


const char menu_game_sound[]		   =	 "audio/menu_game_music.wav";			//--------audio
const char game1_sound[]			   =	 "audio/music_game_1.wav";
const char game2_sound[]			   =	 "audio/music_game_2.wav";
const char game3_sound[]			   =	 "audio/music_game_3.wav";
const char boss1_sound[]			   =	 "audio/music_boss_1.wav";
const char boss2_sound[]			   =	 "audio/music_boss_2.wav";

const char player_jump_sound[]		   =	 "audio/jump.wav";
const char player_die_sound[]		   =	 "audio/player_die.wav";
const char player_select_power_sound[] =	 "audio/player_select_power.wav";
const char player_select_bullet_sound[]=	 "audio/player_select_bullet.wav";

const char open_option_sound[]		   =	 "audio/option_open.wav";
const char close_option_sound[]		   =	 "audio/option_close.wav";
const char chose_option_sound[]		   =	 "audio/option_chose.wav";

const char shoot_bullet_sound[]		   =	 "audio/shoot_bullet.wav";
const char shoot_probullet_sound[]	   =	 "audio/shoot_probullet.wav";
const char deatheye_lazebullet_sound[] =	 "audio/deatheye_lazebullet.wav";
const char deatheye_electrik_sound[]   =	 "audio/deatheye_electrik.wav";

const char blind_exp_sound[]		   =	 "audio/blind_exp.wav";
const char standard_exp_sound[]		   =	 "audio/standard_exp.wav";
const char chasingrocket_exp_sound[]   =	 "audio/chasingrocket_exp.wav";
const char rocketbullet_exp_sound[]	   =	 "audio/rocketbullet_exp.wav";
const char fire_sound[]				   =	 "audio/fire.wav";


const char regular_font[]			   =	 "font/PlaywriteAUVIC-Regular.ttf";     //--------font
const char light_font[]				   =	 "font/PlaywriteAUVIC-Light.ttf";


//MAP
#define MAX_TILE_TYPE      99		//size
#define TILE_SIZE          64
#define MAX_MAP_X          200
#define MAX_MAP_Y          10

#define BLANK_TILE         0	    //ground
#define GROUND_TILE_1      1
#define GROUND_TILE_2      2
#define GROUND_TILE_3      3
#define IRON_TILE		   4
#define LEFT_LAND_TILE	   6
#define RIGHT_LAND_TILE    7
#define SMALL_LAND_TILE    8
#define BOMXANH_GZ_TILE    11
#define BARREL_TILE		   12
#define LEFT_SNOW_TILE	   13
#define BETWEEN_SNOW_TILE  14
#define RIGHT_SNOW_TILE	   15
#define INVISIBLE_TILE     20
#define STIKE_TILE		   40

#define HEART_TILE		   21       //item
#define HP_TILE            22
#define FLAG_TILE          23
#define COIN_TILE		   25
#define MYSTERY_TILE       24

#define STANDERD_TYPE      99       //ammo
#define ROCKET_TYPE        98
#define BALL_TYPE          97
#define STAR_TYPE          96
#define ROUND_TYPE         95
#define PRO_TYPE           94


//DIRECTION
enum Direction
{
	Dir_NONE = 0,
	Dir_LEFT = 1,
	Dir_RIGHT = 2,
	Dir_UP = 3,
	Dir_DOWN = 4,
	Dir_LEFT_UP = 5,
	Dir_LEFT_DOWN = 6,
	Dir_RIGHT_UP = 7,
	Dir_RIGHT_DOWN = 8
};

//MAP_NUMBER
enum MapNumber
{
	MAP_1 = 0,
	MAP_2 = 1,
	MAP_3 = 2
};

//AUDIO
enum AudioNumber
{
	MENU_SOUND = 0,
	GAME1_SOUND = 1,
	GAME2_SOUND = 2,
	GAME3_SOUND = 3,
	BOSS1_SOUND = 4,
	BOSS2_SOUND = 5,

	P_JUMP_SOUND = 6,
	P_DIE_SOUND = 7,
	P_SELECT_POWER_SOUND = 8,
	P_SELECT_BULLET_SOUND = 9,

	OPENT_OPTION_SOUND = 10,
	CLOSE_OPTION_SOUND = 11,
	CHOSE_OPTION_SOUND = 12,

	SHOOT_BULLET_SOUND = 13,
	SHOOT_PROBULLET_SOUND = 14,
	DEATHEYE_LAZEBULLET_SOUND = 15,
	DEATHEYE_ELECTRIK_SOUND = 16,

	BLIND_EXP_SOUND = 17,
	STANDARD_EXP_SOUND = 18,
	CHARSINGREOCKET_EXP_SOUND = 19,
	ROCKETBULLET_EXP_SOUND = 20,
	FIRE_SOUND = 21
};

namespace SDLFunction
{
	bool CheckCollision(const SDL_Rect& Object1, const SDL_Rect& Object2);
	bool CheckFoscustMouseWithRect(const int& xm, const int& ym, SDL_Rect& object);
	void Win_QuitGame(SDL_Renderer* des);
}


#endif //FUNCTION_FILE_H_





// *** NOTE !
// map_x_               <=> luong screen da di chuyen qua tinh tu tile dau tien cho den s_creen.x
// map_y_               <=> ...