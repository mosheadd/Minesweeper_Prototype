#include "Map.h"

using namespace mscp;
using namespace std;

int mscp::getNumberInRange(int min_val, int max_val){ return min_val + rand() % max_val; }

void mscp::clearWindow()
{

	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);

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

	if (row < 0 || row > 9 || col < 0 || col > 9 || map[row][col] == UNCOVERED || map[row][col] == MINE || map[row][col] == FLAGGED_MINE) return;

	int cell_mines_count = getMinesCount(row, col);

	map[row][col] = UNCOVERED;

	if (cell_mines_count > 0) return;

	makeRoad(row - 1, col - 1);
	makeRoad(row - 1, col);
	makeRoad(row - 1, col + 1);
	makeRoad(row, col - 1);
	makeRoad(row, col + 1);
	makeRoad(row + 1, col - 1);
	makeRoad(row + 1, col);
	makeRoad(row + 1, col + 1);

}

short Map::getMinesCount(short row, short col)
{
	int mines_count = 0;

	if (row >= 1 && row <= 8 && col >= 1 && col <= 8)
	{
		for (int t = row - 1; t < row + 2; t++)
			for (int k = col - 1; k < col + 2; k++)
				mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
	}
	else if (col >= 1 && col <= 8)
	{

		if (row == 0)
		{
			for (int t = 0; t < 2; t++)
				for (int k = col - 1; k < col + 2; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
		else
		{
			for (int t = 8; t < 10; t++)
				for (int k = col - 1; k < col + 2; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
	}
	else if (row >= 1 && row <= 8)
	{
		if (col == 0)
		{
			for (int t = row - 1; t < row + 2; t++)
				for (int k = 0; k < 2; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
		else
		{
			for (int t = row - 1; t < row + 2; t++)
				for (int k = 8; k < 10; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
	}
	else
	{
		short coeff1 = (row == 0) - (row == 9);
		short coeff2 = (col == 0) - (col == 9);
		mines_count += map[row + coeff1][col + coeff2] == MINE + map[row][col + coeff2] == MINE + map[row + coeff1][col] == MINE;
		mines_count += map[row + coeff1][col + coeff2] == FLAGGED_MINE + map[row][col + coeff2] == FLAGGED_MINE + map[row + coeff1][col] == FLAGGED_MINE;
	}

	return mines_count;
}

void Map::gameloop()
{

	srand(time(0));

	short choose, row, col;
	bool exitGame = false;

	cin >> choose;

	if (choose == 2) return;

	cin >> row >> col;

	for (short i = 0; i < 10; i++)
	{
		map.push_back({});
		for (short j = 0; j < 10; j++)
		{
			if (row == i + 1 && col == j + 1)
				map[i].push_back(COVERED);
			else
			{
				int rn = getNumberInRange(0, 100);
				if (rn > 15) map[i].push_back(COVERED);
				else map[i].push_back(MINE);
			}
		}
	}

	makeRoad(row - 1, col - 1);

	clearWindow();

	while (true)
	{
		for (short i = 0; i < 10; i++)
		{
			for (short j = 0; j < 10; j++)
			{
				if (map[i][j] == COVERED || map[i][j] == MINE) cout << "*";
				else if (map[i][j] == FLAGGED || map[i][j] == FLAGGED_MINE) cout << "!";
				else if (map[i][j] == UNCOVERED)
				{
					int bombs_count = getMinesCount(i, j);

					if (bombs_count == 0)
						cout << "-";
					else
						cout << bombs_count;
				}
			}
			cout << endl;
		}

		cin >> choose;

		switch (choose)
		{
		case EXIT:
			exitGame = true;
			break;
		case UNCOVER:
			cin >> row >> col;

			if (map[row - 1][col - 1] == MINE || map[row - 1][col - 1] == FLAGGED_MINE)
			{
				clearWindow();
				cout << "lol you died" << endl;
				exitGame = true;
			}
			else
			{
				makeRoad(row - 1, col - 1);
			}
			break;
		case FLAG:
			cin >> row >> col;

			switch (map[row - 1][col - 1])
			{
			case UNCOVERED:
				map[row - 1][col - 1] = FLAGGED;
				break;
			case MINE:
				map[row - 1][col - 1] = FLAGGED_MINE;
				break;
			case FLAGGED:
				map[row - 1][col - 1] = UNCOVERED;
				break;
			case FLAGGED_MINE:
				map[row - 1][col - 1] = MINE;
				break;
			default:
				break;
			}

			break;
		default:
			break;
		}

		if (exitGame)
			break;

		clearWindow();

		bool game_won = checkWinCondition();

		if (game_won)
		{
			cout << "you won\n";
			return;
		}

	}

	for (short i = 0; i < 10; i++)
	{
		for (short j = 0; j < 10; j++)
		{
			if (map[i][j] == MINE || map[i][j] == FLAGGED_MINE) cout << "+";
			else
			{
				int bombs_count = getMinesCount(i, j);

				if (bombs_count == 0)
					cout << "-";
				else
					cout << bombs_count;
			}
		}
		cout << endl;
	}

}

bool Map::checkWinCondition()
{

	for (auto i : map)
	{
		for (auto j : i)
		{
			if (j == COVERED || j == FLAGGED)
				return false;
		}
	}

	return true;

}

