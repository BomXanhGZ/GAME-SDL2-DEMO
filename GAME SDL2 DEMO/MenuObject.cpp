#include "MenuObject.h"


MenuObject::MenuObject()
{
	map_number_ = MAP_1;
}

MenuObject::~MenuObject()
{
	;
}

void MenuObject::InitMenu(SDL_Renderer* des, TTF_Font* font)
{
	
	menu_background_.LoadImg(back_ground_menu_img[map_number_], des);

	
	//const_text
	std::string m_text = "=> MENU GAME <=";
	menugame_text_.SetText(m_text);
	menugame_text_.SetColor(TextObject::ORANGE_TEXT);
	menugame_text_.LoadFromRenderText(font, des);

	std::string b_game = "GAME BY:   BomXanhG-Z!";
	bomxanh_text_.SetText(b_game);
	bomxanh_text_.SetColor(TextObject::GREEN_TEXT);
	bomxanh_text_.LoadFromRenderText(font, des);
	

	//select_text
	std::string s_text = "START GAME <3";
	start_text_.SetText(s_text);
	start_text_.SetColor(TextObject::RED_TEXT);
	start_text_.LoadFromRenderText(font, des);

	std::string q_game = "EXIT GAME :v";
	exit_text_.SetText(q_game);
	exit_text_.SetColor(TextObject::RED_TEXT);
	exit_text_.LoadFromRenderText(font, des);

	std::string q_text = "QUIT GAME ???";
	quit_text_.SetText(q_text);
	quit_text_.SetColor(TextObject::GREEN_TEXT);
	quit_text_.LoadFromRenderText(font, des);

	std::string c_text = "CONTINUE !!!";
	continue_text_.SetText(c_text);
	continue_text_.SetColor(TextObject::GREEN_TEXT);
	continue_text_.LoadFromRenderText(font, des);

	std::string cm_text = "CHANGE MAP %";
	change_map_text_.SetText(cm_text);
	change_map_text_.SetColor(TextObject::GREEN_TEXT);
	change_map_text_.LoadFromRenderText(font, des);

}

int MenuObject::RunMenu(SDL_Renderer* des, TTF_Font* font)
{

	SDL_Event m_event;
	int xm = 0;
	int ym = 0;
	
	SDL_Rect rect_s_text = {X_TEXT, START_Y_TEXT, start_text_.GetWidth(), start_text_.GetHeight()};
	SDL_Rect rect_e_text = {X_TEXT - 3, EXIT_Y_TEXT, exit_text_.GetWidth(), exit_text_.GetHeight()};
	SDL_Rect rect_cm_text = {X_TEXT - 3, CHANGE_MAP_Y_TEXT, change_map_text_.GetWidth(), change_map_text_.GetHeight()};


	while (true)
	{

		menu_background_.Render(des);

		bomxanh_text_.LoadFromRenderText(font, des);
		menugame_text_.LoadFromRenderText(font, des);
		start_text_.LoadFromRenderText(font, des);
		exit_text_.LoadFromRenderText(font, des);
		change_map_text_.LoadFromRenderText(font, des);

		bomxanh_text_.RenderText(des, X_BOMXANH_TEXT, Y_BOMXANH_TEXT);
		menugame_text_.RenderText(des, X_TEXT - 20, MENUGAME_Y_TEXT);
		start_text_.RenderText(des, X_TEXT, START_Y_TEXT);
		exit_text_.RenderText(des, X_TEXT - 3, EXIT_Y_TEXT);
		change_map_text_.RenderText(des, X_TEXT, CHANGE_MAP_Y_TEXT);

		while(SDL_PollEvent(&m_event))
		{
			switch(m_event.type)
			{

			case SDL_QUIT:
				return EXIT_SELECT;



			case SDL_MOUSEMOTION:

				{
					xm = m_event.motion.x;
					ym = m_event.motion.y;


					if(SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_s_text))
					{
						start_text_.SetColor(TextObject::GREEN_TEXT);
					}
					else
					{
						start_text_.SetColor(TextObject::RED_TEXT);
					}


					if(SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_cm_text))
					{
						change_map_text_.SetColor(TextObject::GREEN_TEXT);
					}
					else
					{
						change_map_text_.SetColor(TextObject::RED_TEXT);
					}


					if(SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_e_text))
					{
						exit_text_.SetColor(TextObject::GREEN_TEXT);
					}
					else
					{
						exit_text_.SetColor(TextObject::RED_TEXT);
					}


					break;
				}


			case SDL_MOUSEBUTTONDOWN:

				{
					bool cl_start_text = SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_s_text);
					if(cl_start_text == true)
					{
						return START_SELECT;
					}

					bool cl_change_text = SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_cm_text);
					if(cl_change_text == true)
					{
						map_number_++;
						if(map_number_ >= MAP_NUM)
						{
							map_number_ = MAP_1;
						}

						menu_background_.LoadImg(back_ground_menu_img[map_number_], des);
					}

					bool cl_quit_text = SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_e_text);
					if(cl_quit_text == true)
					{
						return EXIT_SELECT;
					}

					break;
				}
			}
		}

			SDL_RenderPresent(des);
	}

	return 0;
}

int MenuObject::RunPause(SDL_Renderer* des, TTF_Font* font)
{
	Mix_Pause(-1);
	Mix_PlayChannel(-1, g_audio.at(OPENT_OPTION_SOUND), 0);

	SDL_Event m_event;
	int xm = 0;
	int ym = 0;
	
	SDL_Rect rect_c_text = {X_TEXT, CONTINUE_Y_TEXT, continue_text_.GetWidth(), continue_text_.GetHeight()};
	SDL_Rect rect_q_text = {X_TEXT - 3, QUIT_Y_TEXT, quit_text_.GetWidth(), quit_text_.GetHeight()};


	while (true)
	{

		continue_text_.LoadFromRenderText(font, des);
		quit_text_.LoadFromRenderText(font, des);

		continue_text_.RenderText(des, X_TEXT, CONTINUE_Y_TEXT);
		quit_text_.RenderText(des, X_TEXT - 3, QUIT_Y_TEXT);

		while(SDL_PollEvent(&m_event))
		{
			switch(m_event.type)
			{

			case SDL_QUIT:
				{
					is_exit = true;
					return QUIT_SELECT;
				}


			case SDL_KEYDOWN:
				{
					if(m_event.key.keysym.sym == SDLK_ESCAPE)
					{
						Mix_PlayChannel(-1, g_audio.at(CLOSE_OPTION_SOUND), 0);
						return CONTINIUE_SELECT;
					}
				}


			case SDL_MOUSEMOTION:

				{
					xm = m_event.motion.x;
					ym = m_event.motion.y;

					if(SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_c_text))
					{
						continue_text_.SetColor(TextObject::RED_TEXT);
					}
					else
					{
						continue_text_.SetColor(TextObject::GREEN_TEXT);
					}


					if(SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_q_text))
					{
						quit_text_.SetColor(TextObject::RED_TEXT);
					}
					else
					{
						quit_text_.SetColor(TextObject::GREEN_TEXT);
					}


					break;
				}


			case SDL_MOUSEBUTTONDOWN:

				{
					bool cl_continue_text = SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_c_text);
					if(cl_continue_text == true)
					{
						Mix_PlayChannel(-1, g_audio.at(CHOSE_OPTION_SOUND), 0);
						return CONTINIUE_SELECT;
					}

					bool cl_quit_text = SDLFunction::CheckFoscustMouseWithRect(xm, ym, rect_q_text);
					if(cl_quit_text == true)
					{
						Mix_PlayChannel(-1, g_audio.at(CHOSE_OPTION_SOUND), 0);
						return QUIT_SELECT;
					}

					break;
				}
			}
		}

			SDL_RenderPresent(des);
	}

	return 0;
}