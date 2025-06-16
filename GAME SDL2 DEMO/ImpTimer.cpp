
#include "ImpTimer.h"
#include "FunctionFile.h"

ImpTimer::ImpTimer()
{
	start_tick_ = 0;
	paused_tick_ = 0;
	pause_time_ = 0;
	is_started_ = false;
	is_paused_ = false;
}

ImpTimer::~ImpTimer()
{
	;//
}

void ImpTimer::start()
{
	is_started_ = true;
	is_paused_  = false;

	start_tick_ = SDL_GetTicks();
}


void ImpTimer::paused()
{
	if(is_started_ == true && is_paused_ == false)
	{
		is_paused_   = true;
		paused_tick_ = SDL_GetTicks(); // - start_tick_;  // current tick
	}
}

void ImpTimer::unpaused()
{
	if(is_paused_ == true)
	{
		is_paused_ = false;
		pause_time_ += (SDL_GetTicks() - paused_tick_);    //star tick == pause time
	
		paused_tick_ = 0; 
	}
}


int ImpTimer::get_ticks()   //get  paused_tick_
{
	if(is_started_ == true)
	{
		if(is_paused_ == true)
		{
			return paused_tick_;
		}
		else
		{
			int ticks = SDL_GetTicks() - start_tick_;
			return ticks;
		}
	}

	return 0;
}