
#ifndef GAME_MAP_H_
#define GAME_MAP_H_


#include "FunctionFile.h"
#include "BaseObject.h"


typedef struct Map      // Rect~num_tile
{
	int star_x_;        // <=> star screen
	int star_y_;

	int max_x_;         // <=> max img
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_;
};


class TileMat : public BaseObject    //load~save img_tile
{
public:
	TileMat() {;}
	~TileMat() {;}
};


class TileType
{
public:

	TileType()
	{
		Set_obstacle_tile();
	}

	~TileType() {;}


	//Set_function
	void Set_obstacle_tile();

	//Get_function
	std::vector<int> Get_obstancle_tile() const {return obstacle_tile_; }

private:

	std::vector<int> obstacle_tile_;
	//power tile
};


class GameMap       // make from TileMat & Map
{
public:

	GameMap() {;}
	~GameMap() {;}

	void DrawMap(SDL_Renderer* des);        //Find img tile in des

	//Set_Function
	void LoadMap(char* name);               //load~read file_map
	void LoadTiles(SDL_Renderer* des);       //loadimg_tile
	void SetMap(Map& map_data) {game_map_ = map_data;}

	//Get_Function
	Map Getmap() {return game_map_;}

private:

	Map game_map_;
	TileMat tile_type_[MAX_TILE_TYPE];
};


#endif //GAME_MAP_H_