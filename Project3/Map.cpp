#include "Map.h"

using namespace mscp;
using namespace std;

int getNumberInRange(int min_val, int max_val){ return min_val + rand() % max_val; }

void clearWindow()
{

}

Map::Map()
{
	height = 10;
	width = 10;
}

Map::Map(short height, short width)
{
	this->height = height;
	this->width = width;
}

Map::~Map()
{
	cout << "\n\n\n";
	cout << "The game ends here";
}

void Map::makeRoad(short row, short col)
{

}

void Map::getMinesCount()
{

}

void Map::gameloop()
{

}

bool Map::checkWinCondition()
{

}

