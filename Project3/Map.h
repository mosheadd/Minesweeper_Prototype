#pragma once
#include "iostream"
#include "Windows.h"
#include <vector>

namespace mscp
{

	constexpr auto UNCOVER = 1;
	constexpr auto FLAG = 2;
	constexpr auto EXIT = 3;

	void clearWindow();

	int getNumberInRange(int min_val, int max_val);

	class Map
	{
		std::vector<std::vector<short>> map;
		short height, width;

		enum
		{
			COVERED = 0,
			UNCOVERED = 1,
			FLAGGED = 2,
			MINE = 3,
			FLAGGED_MINE = 4,
		};

	public:
		Map();
		Map(short height, short width);

		~Map();

		void makeRoad(short row, short col);
		void draw(bool uncovered = true);
		void gameloop();

		short getMinesCount(short row, short col);

		bool checkWinCondition();

	};
}