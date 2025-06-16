
#include "FunctionFile.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "PlayerObject.h"
#include "ImpTimer.h"
#include "EffectObject.h"
#include "TextObject.h"
#include "Geomitric.h"
#include "MenuObject.h"
#include "GuardThreat.h"
#include "AerialThreat.h"
#include "StaticThreat.h"
#include "DinoThreat.h"
#include "ChasingRocket.h"
#include "FireRain.h"
#include "RockTitanBoss.h"
#include "DestructionTankBoss.h"
#include "RocketWardenBoss.h"
#include "DeathEyeBoss.h"


BaseObject g_background;


bool InitData()
{
	//Init SDL
	bool succes = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret < 0)
	{return false;}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	//Make Window
	g_window = SDL_CreateWindow("Game By BomXanhG-Z", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(g_window == NULL)
	{succes = false;}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if(g_screen == NULL) 
		  {succes = false;}
		else
		{
			//Render Draw Color
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) && imgFlags))   //??????
			  {succes = false;}
		}
		
		//Init_tff(font_text)
		if(TTF_Init() < 0)
		{
			succes = false;
		}
		
		g_font = TTF_OpenFont(regular_font, 20);
		if (g_font == NULL)
		{
	    	succes = false;
		}

		//Init_mixer(sound)
		if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)	
		{
			succes = false;
		}
		else
		{
			Mix_Chunk* menu_game_ado;

			// !!! BECAUSE AUDIO OF NUMBER ORDER IS CONSTAN, DON'T CHANGE
			g_audio.push_back(Mix_LoadWAV(menu_game_sound));					//menu_game_sound is: 0
			g_audio.push_back(Mix_LoadWAV(game1_sound));						//game1_sound is: 1
			g_audio.push_back(Mix_LoadWAV(game2_sound));						//game2_sound is: 2
			g_audio.push_back(Mix_LoadWAV(game3_sound));						//game3_sound is: 3
			g_audio.push_back(Mix_LoadWAV(boss1_sound));						//boss1_sound is: 4
			g_audio.push_back(Mix_LoadWAV(boss2_sound));						//boss2_sound is: 5

			g_audio.push_back(Mix_LoadWAV(player_jump_sound));					//player_jump_sound is: 6
			g_audio.push_back(Mix_LoadWAV(player_die_sound));					//player_die_sound is: 7
			g_audio.push_back(Mix_LoadWAV(player_select_power_sound));			//player_select_power_sound is: 8 
			g_audio.push_back(Mix_LoadWAV(player_select_bullet_sound));			//player_select_bullet_sound is: 9

			g_audio.push_back(Mix_LoadWAV(open_option_sound));					//open_option_sound is: 10
			g_audio.push_back(Mix_LoadWAV(close_option_sound));					//close_option_sound is: 11
			g_audio.push_back(Mix_LoadWAV(chose_option_sound));					//chose_option_sound is: 12

			g_audio.push_back(Mix_LoadWAV(shoot_bullet_sound));					//shoot_bullet_sound is: 13
			g_audio.push_back(Mix_LoadWAV(shoot_probullet_sound));				//shoot_pro_bullet is: 14
			g_audio.push_back(Mix_LoadWAV(deatheye_lazebullet_sound));			//deatheye_lasebullet_sound is: 15
			g_audio.push_back(Mix_LoadWAV(deatheye_electrik_sound));			//deatheye_electtrick_sound is: 16

			g_audio.push_back(Mix_LoadWAV(blind_exp_sound));					//blind_exp_sound is: 17
			g_audio.push_back(Mix_LoadWAV(standard_exp_sound));					//standard_exp_sound is: 18
			g_audio.push_back(Mix_LoadWAV(chasingrocket_exp_sound));			//chasingrocket_exp_sound is:19
			g_audio.push_back(Mix_LoadWAV(rocketbullet_exp_sound));				//rocketbullet_exp_sound is: 20
			g_audio.push_back(Mix_LoadWAV(fire_sound));							//fire_sound is: 21

			for(int ado = 0; ado < g_audio.size(); ado++)
			{
				if(g_audio.at(ado) == NULL)
				{
					succes = false;
					std::cout <<"audio error at: " <<ado <<std::endl;
				}
			}
		}
	}

	return succes;
}


void close()
{
	
	for(int ado = 0; ado < g_audio.size(); ado++)
	{
		if(g_audio.at(ado) != NULL)
		{
			Mix_FreeChunk(   g_audio.at(ado)   );
		}
	}

	g_audio.clear();



	TTF_CloseFont(g_font);								//font
	g_font = NULL;

	g_background.Free();								//bkground
	
	SDL_DestroyRenderer(g_screen);						//SDL
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	Mix_CloseAudio();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool LoadBackGround(std::string path)
{
	bool ret = g_background.LoadImg(path, g_screen);
	if(ret == false)
	  {return false;}

	return true;
}



int main(int argc, char* argv[])
{

	//Init_data SDL
	if(InitData() == false)
	{return -1;}

	
	//make_menu
	MenuObject menu_game;
	menu_game.InitMenu(g_screen, g_font);
	int select;
	int l_map_num;
	Mix_PlayChannel(-1, g_audio.at(MENU_SOUND), 0);


	//main_loop
	while(!is_exit)
	{

		//menu_option
		select =  menu_game.RunMenu(g_screen, g_font);
		if(select == MenuObject::START_SELECT)
		{
			is_quit = false;
			Mix_HaltChannel(-1);
		}
		else if(select == MenuObject::EXIT_SELECT)
		{
			is_quit = true;
			is_exit = true;
		}

		l_map_num = menu_game.Get_MapNumBer();



		//##########################
		//##### MUTABLE DATA #######
		//##########################

		//Load bkground
		if(LoadBackGround(   back_ground_img[l_map_num]   ) == false)
		{return -1;}


		//make map
		GameMap game_map;

		std::string str_map_path = map_path[l_map_num];
		char char_map_path[25];
		strcpy(char_map_path, str_map_path.c_str());

		game_map.LoadMap(char_map_path);
		game_map.LoadTiles(g_screen);


		//game music
		int star_music_channel;


		//make_GuardThreat
		std::vector<GuardThreat*> guardthreat_list;
		GuardThreat guardthreats;
		bool is_use_GuardThreat = false;

		//make_StaticThreat
		std::vector<StaticThreat*> staticthreat_list;
		StaticThreat staticthreats;
		bool is_use_StaticThreat = false;

		//make_DinoTHreat
		DinoThreat dinothreat;

		//make_AerialThreat
		AerialThreat aerialthreat;

		//make_ChasingRocket
		ChasingRocket chasingrocket;

		//make_FireRain
		std::vector<FireRain*> firerain_list;
		FireRain firerain;
		bool is_use_FireRain = false;


		//make_RockTitanBoss
		RockTitanBoss boss_level_1;

		//make_DestructionTank
		DestructionTankBoss boss_level_2;

		//make_RocketWardenBoss
		RocketWardenBoss rocketwardenboss;

		//make_DeathEysBoss
		DeathEyeBoss deatheyeboss;



	//##### MAP 1 ######

		if(l_map_num == MAP_1)
		{
			//music
			if(is_quit == false) star_music_channel = Mix_PlayChannel(-1, g_audio.at(GAME1_SOUND), 0);

			//threat
			guardthreat_list = guardthreats.InitThreats(g_screen, l_map_num);
			is_use_GuardThreat = true;

			aerialthreat.InitAerialThreat(g_screen);
			
	
			//boss
			boss_level_1.Init_boss(g_screen);
		}



	//##### MAP 2 ######
		if(l_map_num == MAP_2)
		{
			//music
			if(is_quit == false) star_music_channel = Mix_PlayChannel(-1, g_audio.at(GAME2_SOUND), 0);

			//threat
			guardthreat_list = guardthreats.InitThreats(g_screen, l_map_num);
			is_use_GuardThreat = true;

			staticthreat_list = staticthreats.InitStaticThreat(g_screen);
			is_use_StaticThreat = true;

			firerain_list = firerain.InitFireRain(g_screen);
			is_use_FireRain = true;


			//boss
			deatheyeboss.InitDeathEyeBoss(g_screen);
		}

		

	//##### MAP 3 ######
		if(l_map_num == MAP_3)
		{
			//music
			if(is_quit == false) star_music_channel = Mix_PlayChannel(-1, g_audio.at(GAME3_SOUND), 0);
			
			//threat
			guardthreat_list = guardthreats.InitThreats(g_screen, l_map_num);
			is_use_GuardThreat = true;

			dinothreat.InitDinoThreat(g_screen);
			
			chasingrocket.InitChasingRoccket(g_screen);

			//boss
			rocketwardenboss.InitRocketWardenBoss(g_screen);
		}



		//#########################
		//###### CONST DATA #######
		//#########################

		//make_main_object
		PlayerObject p_player;
		p_player.LoadAnimation(player_right_img, g_screen, PLAYER_FRAM_NUM);
		p_player.SetClip();
		p_player.Set_check_tile();
		p_player.Init_PowerShow(g_screen);


		//Init_effect						
		// !!! BECAUSE EFFECTS OF NUMBER ORDER IS CONSTAN, DON'T CHANGE
		std::vector<EffectObject*> list_effect;

		EffectObject* exp_effect = new EffectObject;																//exp_effect is: 0
		exp_effect->InitEffect(explosion_img, g_screen, EXPLOSION_FRAM_NUM);
		list_effect.push_back(exp_effect);

		EffectObject* rocket_explosion_effect = new EffectObject;													//rocket_expeffect is: 1
		rocket_explosion_effect->InitEffect(rocket_explosion_img, g_screen, ROCKET_EXP_FRAM_NUM);
		list_effect.push_back(rocket_explosion_effect);

		EffectObject* flash_effect = new EffectObject;																//flast_effect is: 2
		flash_effect->InitEffect(flash_effect_img, g_screen, FLASH_FRAM_NUM);
		list_effect.push_back(flash_effect);

		EffectObject* run_blind_effect = new EffectObject;															//run_blind_effect is: 3
		run_blind_effect->InitEffect(run_blind_effect_img, g_screen, BLIND_1_FRAM_NUM);
		list_effect.push_back(run_blind_effect);

		EffectObject* blind_effect = new EffectObject;																//blind_effect is: 4
		blind_effect->InitEffect(blind_effect_img, g_screen, BLIND_2_FRAM_NUM);
		list_effect.push_back(blind_effect);

		EffectObject* player_rip_effect = new EffectObject;															//player_death_effect is: 5
		player_rip_effect->InitEffect(player_rip_effect_img, g_screen, PLAYER_RIP_FRAM_NUM);
		list_effect.push_back(player_rip_effect);

		EffectObject* wingame_effect = new EffectObject;															//wingame_effect is: 6
		wingame_effect->InitEffect(win_effect_img, g_screen, WINGAME_FRAM_NUM);
		list_effect.push_back(wingame_effect);

		EffectObject* fire_effect = new EffectObject;																//fire_effect is: 7
		fire_effect->InitEffect(fire_effect_img, g_screen, FIRE_FRAM_NUM);
		list_effect.push_back(fire_effect);

		EffectObject* blue_shield_effect = new EffectObject;														//blue_shield_effect is: 8
		blue_shield_effect->InitEffect(blue_shield_img, g_screen, SHIELD_FRAM_NUM);
		list_effect.push_back(blue_shield_effect);

		EffectObject* charge_laze_effect = new EffectObject;														//charge_effect is: 9
		charge_laze_effect->InitEffect(charge_bullet_effect_img, g_screen, CHARGELAZE_EFFECT_FRAM_NUM);
		list_effect.push_back(charge_laze_effect);

		EffectObject* green_shield_effect = new EffectObject;														//green_shiel_efffectis: 10
		green_shield_effect->InitEffect(green_shield_img, g_screen, SHIELD_FRAM_NUM);
		list_effect.push_back(green_shield_effect);


		//make fps_timer
		ImpTimer fps_timer;
		int real_imp_time = 1;
		int pause_time_val = 0;
		int passed_time_val = SDL_GetTicks();

	
		//make_text
		TextObject time_game;
		time_game.SetColor(TextObject::WHILE_TEXT);
		TextObject ply_lives_num;
		ply_lives_num.SetColor(TextObject::RED_TEXT);
		TextObject fps_show;
		fps_show.SetColor(TextObject::BLACK_TEXT);
	




	//##### GAME LOOP #######

		while(!is_quit)
		{

			//fps_time
			fps_timer.start();

			//event loop
			while(SDL_PollEvent(&g_event) != 0)
			{
				if(g_event.type == SDL_QUIT)
				{
					is_quit = true;
					is_exit = true;
					break;
				}

				p_player.HandelInputAction(g_event, g_screen);
			}


			if(is_pause)
			{
				fps_timer.paused();

				select = menu_game.RunPause(g_screen, g_font);
				if(select == MenuObject::CONTINIUE_SELECT)
				{
					is_pause = !is_pause;
					fps_timer.unpaused();
					pause_time_val = fps_timer.Get_PauseTime();

					Mix_Resume(-1);
				}
				else if(select == MenuObject::QUIT_SELECT)
				{
					is_pause = !is_pause;
					is_quit = true;
					Mix_HaltChannel(-1);
				}
			}


			//SET RENDER
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			SDL_RenderClear(g_screen);


			//BACK GROUND																		// ==>> MUTABLE <<==
			g_background.Render(g_screen);


			//GET MAP
			Map map_data = game_map.Getmap();


			//DRAW GEOMITRIC
			GeometricFormat rectangle_size(0, 0, RECT_TANGLE_W, RECT_TANGLE_H);
			ColorData tangle_color_data(0, 80, 150);
			Geometric::RenderRecttangle(rectangle_size, tangle_color_data, g_screen);

			GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 2, 38);
			ColorData outline_color_data(255, 0, 0);
			Geometric::RenderOutLine(outLineSize, outline_color_data, g_screen);



			//PLAYER
			p_player.SetMapXY(map_data.star_x_, map_data.star_y_);
			p_player.Doplayer(map_data, g_screen, list_effect);
		
			game_map.SetMap(map_data);               //---map
			game_map.DrawMap(g_screen);

			p_player.ShowPower(g_screen);
			p_player.HandleBullet(g_screen);
			p_player.Show(g_screen);



		//##### MAP 1 #######

			if(l_map_num == MAP_1)
			{

				//handle GuardThreat
				for(int i = 0; i < guardthreat_list.size(); i++)												
				{
					if(guardthreat_list.at(i) != NULL)
					{
						guardthreat_list.at(i)->HandleThreat(map_data, g_screen, p_player, list_effect);
					}
				}

				//handle AerialThreat
				aerialthreat.HandleThreat(map_data, g_screen, p_player, list_effect);

				//Handle boss
				if(boss_level_1.Get_boss_hp() > 0 && map_data.star_x_ > MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*3)
				{

					boss_level_1.HandleBoss(g_screen, map_data, p_player, list_effect);

					if(boss_level_1.Get_boss_hp() <= 0)
					{
				
						//free bullet
						std::vector<BulletObject*> boss_bullet = boss_level_1.Get_boss_bullet_list();
						for(int b = 0; b < boss_bullet.size(); b++)
						{
							BulletObject* b_bullet = boss_bullet.at(b);
							delete b_bullet;
							b_bullet = NULL;
						}
						boss_bullet.clear();

						//init boss 2
						boss_level_2.Init_boss(g_screen);
						boss_level_2.Set_is_run_boss_lv2(true);

						Mix_HaltMusic();

						Mix_HaltChannel(star_music_channel);
						int boss1_music_ = Mix_PlayChannel(-1, g_audio.at(BOSS1_SOUND), -1);
					}
				}

				if(boss_level_2.Get_is_run_boss_lv2() == true && map_data.star_x_ > (MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*3) )
				{
					boss_level_2.HandleBoss(map_data, p_player, g_screen, list_effect);
				}
			}



		//##### MAP 2 #######

			if(l_map_num == MAP_2)
			{
				
				//handle GuardThreat
				for(int i = 0; i < guardthreat_list.size(); i++)												
				{
					if(guardthreat_list.at(i)->GetIsHandle() == true)
					{
						guardthreat_list.at(i)->HandleThreat(map_data, g_screen, p_player, list_effect);
					}
				}

				//handle StaticThreat
				for(int st = 0; st < staticthreat_list.size(); st++)
				{
					if(staticthreat_list.at(st)->Get_IsHanle_StaticThreat() == true)
					{
						staticthreat_list.at(st)->Handle(g_screen, &map_data, &p_player, list_effect);
					}
				}
			
				//Handle FireRain
				for(int fr = 0; fr < firerain_list.size(); fr++)
				{
					firerain_list.at(fr)->HandleFireRain(g_screen, &map_data, &p_player, list_effect);
				}
				
				//Hande DeathEyeBoss
				if(     deatheyeboss.Get_hp() > 0 && map_data.star_x_ > (MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*3)     )
				{
					deatheyeboss.HandleDeathEyeBoss(g_screen, map_data, &p_player, list_effect);
				}
			}



		//##### MAP 3 #######
			if(l_map_num == MAP_3)
			{

				//handle GuardThreat
				for(int i = 0; i < guardthreat_list.size(); i++)												
				{
					if(guardthreat_list.at(i)->GetIsHandle() == true)
					{
						guardthreat_list.at(i)->HandleThreat(map_data, g_screen, p_player, list_effect);
					}
				}

				//handle DinoThreat
				if(dinothreat.Get_isHandleDino() == true)
				{
					dinothreat.HandleDinoThreat(g_screen, &map_data, &p_player, list_effect);
				}
				

				//handle ChasingRocket
				if(chasingrocket.Get_IsShow() == true)
				{
					chasingrocket.HandleChasingRocket(g_screen, &map_data, &p_player, list_effect); 
				}

				//Handle RocketWardenBoss
				if(     rocketwardenboss.Get_hp() > 0 && map_data.star_x_ > (MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*3)     )
				{
 					rocketwardenboss.HandleRocketWardenBoss(g_screen, map_data, &chasingrocket, &p_player, list_effect);
				}
			}



			//EFFECT
			if(list_effect.at(FLAST_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(FLAST_EFFECT)->ShowEffect(g_screen, &map_data);

				if(list_effect.at(FLAST_EFFECT)->Get_is_show() == false)
				{
					list_effect.at(RUN_BLIND_EFFECT)->SetRect_PositionEffect(	list_effect.at(FLAST_EFFECT)->Get_effect_rect(), 
																				list_effect.at(FLAST_EFFECT)->Get_xpos(), list_effect.at(FLAST_EFFECT)->Get_ypos()   ); //pos
					list_effect.at(RUN_BLIND_EFFECT)->Set_is_show(true);

					list_effect.at(BLIND_EFFECT)->SetRect_PositionEffect(	list_effect.at(FLAST_EFFECT)->Get_effect_rect(),
																			list_effect.at(FLAST_EFFECT)->Get_xpos(), list_effect.at(FLAST_EFFECT)->Get_ypos()   ); //pos
					list_effect.at(RUN_BLIND_EFFECT)->Set_is_show(true);

					Mix_PlayChannel(-1, g_audio.at(BLIND_EXP_SOUND), 0);
				}
			}

			if(list_effect.at(RUN_BLIND_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(RUN_BLIND_EFFECT)->ShowEffect(g_screen, &map_data);

				if(list_effect.at(RUN_BLIND_EFFECT)->Get_is_show() == false)
				{
					list_effect.at(BLIND_EFFECT)->Set_is_show(true);
					list_effect.at(BLIND_EFFECT)->Set_fram(0);
					list_effect.at(BLIND_EFFECT)->ShowEffect(g_screen, &map_data);	
					SDL_RenderPresent(g_screen);

					SDL_Delay(FLASH_EFFECT_DELAY);
				}
			}

			if(list_effect.at(EXP_EFFECT)->Get_is_show() == true)
			{
				if(list_effect.at(EXP_EFFECT)->Get_fram() == 0)
				{
					Mix_PlayChannel(-1, g_audio.at(STANDARD_EXP_SOUND), 0);
				}
				list_effect.at(EXP_EFFECT)->ShowEffect(g_screen, &map_data);
			}

			if(list_effect.at(ROCKET_EXP_EFFECT)->Get_is_show() == true)
			{
				if(list_effect.at(ROCKET_EXP_EFFECT)->Get_fram() == 0)
				{
					Mix_PlayChannel(-1, g_audio.at(ROCKETBULLET_EXP_SOUND), 0);
				}
				list_effect.at(ROCKET_EXP_EFFECT)->ShowEffect(g_screen, &map_data);
			}

			if(list_effect.at(FIRE_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(FIRE_EFFECT)->ShowEffect(g_screen, &map_data);
			}

			if(list_effect.at(BLUE_SHIELD_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(BLUE_SHIELD_EFFECT)->ShowEffect(g_screen, &map_data);
			}

			if(list_effect.at(GREEN_SHIELD_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(GREEN_SHIELD_EFFECT)->ShowEffect(g_screen, &map_data);
			}

			if(list_effect.at(PLAYER_RIP_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(PLAYER_RIP_EFFECT)->ShowEffect(g_screen, &map_data);
			}

			if(list_effect.at(CHARGELAZE_EFFECT)->Get_is_show() == true)
			{
				list_effect.at(CHARGELAZE_EFFECT)->ShowEffect(g_screen, &map_data);
			}



			//WIN GAME
			if(list_effect.at(WINGAME_EFFECT)->Get_is_show() == true)
			{
				wingame_delay--;
				for(int w = 0; w < WINGAME_FRAM_NUM; w++)
				{
					list_effect.at(WINGAME_EFFECT)->ShowEffect(g_screen, &map_data);
					list_effect.at(WINGAME_EFFECT)->Set_is_show(true);

					SDL_RenderPresent(g_screen);
				}

				if(wingame_delay <= 0)
				{
					is_quit = true;
					list_effect.at(WINGAME_EFFECT)->Set_is_show(false);
					wingame_delay = WINGAME_DELAY_VAL;
				}
			}

		

			//SHOW_TEXT
		
			//time_game
			std::string str_time = "Time: ";                            
			UINT32 time_val = (     SDL_GetTicks() - passed_time_val - pause_time_val    ) / 1000  ;
			UINT32 val_time = TIME_GAME - time_val;
			if(val_time <= 0)
			{
				if(MessageBox(NULL, L"GameOver !", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
				is_quit = true;
			}
			else
			{
				std::string str_val = std::to_string(val_time);
				str_time += str_val;

				time_game.SetText(str_time);
				time_game.LoadFromRenderText(g_font, g_screen);
				time_game.RenderText(g_screen, TIME_GAME_RECT_X, TIME_GAME_RECT_Y);
			}
		
			//ply_lives
			std::string x_double = "X";
			UINT32 lives_num = p_player.Get_ply_lives();
			std::string num = std::to_string(lives_num);
			std::string lives_text = x_double + num;

			ply_lives_num.SetText(lives_text);
			ply_lives_num.LoadFromRenderText(g_font, g_screen);
			ply_lives_num.RenderText(g_screen ,PLAYER_LIVES_RECT_X ,PLAYER_LIVES_RECT_Y);
 
			
			//Quit_game
			if(is_quit == true)
			{
				break;
			}


			//FPS  //wait change
			real_imp_time = fps_timer.get_ticks();
			std::cout <<"ms is: " <<real_imp_time <<std::endl;      //Real fps: 5 ~ 10ms

			float time_one_fram = 1000 / FRAM_PED_SECOND;              //Max fps: ~10ms
			int delay_time;

			if(real_imp_time < time_one_fram)
			{
				delay_time = time_one_fram - real_imp_time;      //max delay: 1~5ms
				std::cout <<"delay time is: " <<delay_time << std::endl;   
				SDL_Delay(delay_time);
			}

			//show text fps
			std::string fps_is = "FPS is: ";
			Uint32 fps_num = 1000 / (real_imp_time + delay_time);
			std::string str_fps_num = std::to_string(fps_num);
			fps_is += str_fps_num;

			fps_show.SetText(fps_is);
			fps_show.LoadFromRenderText(g_font, g_screen);
			fps_show.RenderText(g_screen, FPS_SHOW_RECT_X, FPS_SHOW_RECT_Y);


			//UPDATE
			SDL_RenderPresent(g_screen);
		}



	//##### FREE #######

		//Free Threat
		if(is_use_GuardThreat == true)
		{
			for (int grd = 0; grd < guardthreat_list.size(); grd++)
			{
				if(guardthreat_list.at(grd) != NULL)
				{
					guardthreat_list.at(grd)->Free();
					guardthreat_list.at(grd) = NULL;
				}
			}
			guardthreat_list.clear();

			is_use_GuardThreat = false;
		}

		if(is_use_StaticThreat == true)
		{
			for(int st = 0; st < staticthreat_list.size(); st++)
			{
				StaticThreat* p_sta = staticthreat_list.at(st);
				if(p_sta != NULL)
				{
					p_sta->Free();
					p_sta = NULL;
				}
			}
			staticthreat_list.clear();

			is_use_StaticThreat = false;
		}

		if(is_use_FireRain == true)
		{
			for(int fr = 0; fr < FIRE_NUM; fr++)
			{
				if(firerain_list.at(fr) != NULL)
				{
					firerain_list.at(fr)->Free();
					firerain_list.at(fr) = NULL;
				}
			}

			firerain_list.clear();

			is_use_FireRain = false;
		}


		//Free Effect
		for (int f = 0; f < list_effect.size(); f++)
		{
			if(list_effect.at(f) != NULL)
			{
				delete list_effect.at(f);
				list_effect.at(f) = NULL;
			}
		}
		list_effect.clear();
	
	}

	//Free SDL_data
	close();

	return 0;
}
