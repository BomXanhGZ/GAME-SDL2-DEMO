
#include "FunctionFile.h"


SDL_Renderer* g_screen = NULL;
SDL_Event g_event;
SDL_Window* g_window = NULL;
 /*
extern Mix_Chunk* g_player_sound[]		  = {NULL};
extern Mix_Chunk* g_exp_sound[]           = {NULL, NULL};
extern Mix_Chunk* g_bullet_sound[]        = {NULL, NULL};
extern Mix_Chunk* g_SelectItem_sound[]    = {NULL, NULL};
extern Mix_Chunk* g_music_game[]          = {NULL, NULL};
extern Mix_Chunk* g_another_sound[]       = {NULL, NULL, NULL};
*/


TTF_Font* g_font = NULL;

std::vector<Mix_Chunk*> g_audio;

std::vector<int> g_OG_threat_pos_list_;
std::vector<int> g_map_threat_pos_list_;

bool is_quit  = false;
bool is_exit  = false;
bool is_pause = false;

int wingame_delay = WINGAME_DELAY_VAL;


bool SDLFunction::CheckCollision(const SDL_Rect& Object1, const SDL_Rect& Object2)
{

		int left1   = Object1.x;
		int right1  = Object1.x + Object1.w;
		int top1    = Object1.y;
		int bottom1 = Object1.y + Object1.h;

		int left2   = Object2.x;
		int right2  = Object2.x + Object2.w;
		int top2    = Object2.y;
		int bottom2 = Object2.y + Object2.h;


		if(right1 < left2 || left1 > right2 || top1 > bottom2 || bottom1 < top2)
		{
			return false;
		}
		
		return true;
}

bool SDLFunction::CheckFoscustMouseWithRect(const int& xm, const int& ym, SDL_Rect& object)
{
	bool check = false;

	if(xm >= object.x && xm <= (object.x + object.w) 
	&& ym >= object.y && ym <= (object.y + object.h))
	{
		check = true;
	}

	return check;
}
