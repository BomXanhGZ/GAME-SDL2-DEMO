#ifndef IMP_TIMER_H
#define IMP_TIMER_H


class ImpTimer 
{
public:

	ImpTimer();
	~ImpTimer();


	//Set_function
	void start();
	void paused();
	void unpaused();

	//Get_function
	int get_ticks();
	int Get_PauseTime() const {return pause_time_; }

private:

	int start_tick_;
	int paused_tick_;

	bool is_started_;
	bool is_paused_;

	int pause_time_;

};

#endif //IMP_TIMER_H