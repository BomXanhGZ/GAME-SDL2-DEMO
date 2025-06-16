

#include "GameMap.h"

void GameMap::LoadMap(char* file_map_name)
{
	FILE* p_file = NULL;
	fopen_s(&p_file, file_map_name, "rb");   //opent_file voi che do "rb"  va gan all data vao con tro p_file
	if(p_file == NULL)
	  {return;}


	game_map_.max_x_ = 0;
    game_map_.max_y_ = 0;

	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for(int j = 0; j < MAX_MAP_X; j++)
		{
			fscanf_s(p_file, "%d", &game_map_.tile[i][j]);
			int val = game_map_.tile[i][j];
			if (val > 0)
			{
				if(j > game_map_.max_x_)      //???
				{
					game_map_.max_x_ = j;
				}

				if(i > game_map_.max_y_)
				{
					game_map_.max_y_ = i;
				}
			}
		}
	}

	game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE;
	game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE;

	game_map_.star_x_ = 0;
	game_map_.star_y_ = 0;

	game_map_.file_name_ = file_map_name;
	fclose(p_file);
}


void GameMap::LoadTiles(SDL_Renderer* des)
{
	char file_img[20];
	FILE* fp = NULL;

	for(int i = 0; i < MAX_TILE_TYPE; i++)
	{
		sprintf_s(file_img, "map/%d.png", i);

		fopen_s(&fp, file_img, "rb");
		if(fp == NULL)
		{
			continue;
		}

		fclose(fp);

		tile_type_[i].LoadImg(file_img, des);
	}
}


void GameMap::DrawMap(SDL_Renderer* des)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	int map_x = 0;   
	int map_y = 0;

	map_x = game_map_.star_x_/TILE_SIZE;         // tile_number map_x (star screen w)
	x1 = (game_map_.star_x_%TILE_SIZE)*-1;               //x1
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);  //x2

	map_y = game_map_.star_y_/TILE_SIZE;         // tile_number map_y (star screen h)
	y1 = (game_map_.star_y_%TILE_SIZE)*-1;               //y1
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE); //y2

	for(int i = y1; i < y2; i += TILE_SIZE)
	{
		map_x = game_map_.star_x_/TILE_SIZE;

		for(int j = x1; j < x2; j += TILE_SIZE)
		{
			int val = game_map_.tile[map_y][map_x];    // lay data_tile tu dl da dc luu vao tile khi load map
			if (val > 0)
			{
				tile_type_[val].SetRect(j, i);         // goi tile co gt tuong ung / set rect tai j,i
				tile_type_[val].Render(des);           // show img co gt tuong ung tai rec j,i da set
			}
			map_x++;
		}
		map_y++;
	}
}

void TileType::Set_obstacle_tile()
{
	obstacle_tile_.push_back(GROUND_TILE_1);
	obstacle_tile_.push_back(GROUND_TILE_2);
	obstacle_tile_.push_back(GROUND_TILE_3);
	obstacle_tile_.push_back(IRON_TILE);
	obstacle_tile_.push_back(LEFT_LAND_TILE);
	obstacle_tile_.push_back(RIGHT_LAND_TILE);
	obstacle_tile_.push_back(SMALL_LAND_TILE);
	obstacle_tile_.push_back(LEFT_SNOW_TILE);
	obstacle_tile_.push_back(BETWEEN_SNOW_TILE);
	obstacle_tile_.push_back(RIGHT_SNOW_TILE);
	obstacle_tile_.push_back(BOMXANH_GZ_TILE);
	obstacle_tile_.push_back(BARREL_TILE);

	//...;
}