#include "Map.h"
#include "Winuser.h";

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
	init();
}

Map::Map(short height, short width)
{
	this->height = height;
	this->width = width;
	init();
}

Map::~Map()
{
	cout << "\n\n\n";
	cout << "The game ends here";
}

void Map::makeRoad(short row, short col)
{

	if (row < 0 || row > height - 1 || col < 0 || col > width - 1 || map[row][col] == UNCOVERED || map[row][col] == MINE || map[row][col] == FLAGGED_MINE) return;

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

	if (row >= 1 && row <= height - 2 && col >= 1 && col <= width - 2)
	{
		for (int t = row - 1; t < row + 2; t++)
			for (int k = col - 1; k < col + 2; k++)
				mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
	}
	else if (col >= 1 && col <= width - 2)
	{

		if (row == 0)
		{
			for (int t = 0; t < 2; t++)
				for (int k = col - 1; k < col + 2; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
		else
		{
			for (int t = height - 2; t < height; t++)
				for (int k = col - 1; k < col + 2; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
	}
	else if (row >= 1 && row <= height - 2)
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
				for (int k = width - 2; k < width; k++)
					mines_count += map[t][k] == MINE || map[t][k] == FLAGGED_MINE;
		}
	}
	else
	{
		short coeff1 = (row == 0) - (row == height - 1);
		short coeff2 = (col == 0) - (col == width - 1);
		mines_count += (map[row + coeff1][col + coeff2] == MINE) + (map[row][col + coeff2] == MINE) + (map[row + coeff1][col] == MINE);
		mines_count += (map[row + coeff1][col + coeff2] == FLAGGED_MINE) + (map[row][col + coeff2] == FLAGGED_MINE) + (map[row + coeff1][col] == FLAGGED_MINE);
	}

	return mines_count;
}

void Map::draw(bool uncovered)
{

	for (short i = 0; i < height; i++)
	{
		for (short j = 0; j < width; j++)
		{

			if (uncovered)
			{
				int bombs_count;
				switch (map[i][j])
				{
				case FLAGGED:
					cout << "!";
					break;
				case FLAGGED_MINE:
					cout << "!";
					break;
				case UNCOVERED:
					bombs_count = getMinesCount(i, j);

					if (bombs_count == 0)
						cout << "-";
					else
						cout << bombs_count;
					break;
				default: cout << "*";
				}
			}
			else
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
		}
		cout << endl;
	}

}

void Map::gameloop()
{

	srand(time(0));

	short choose, row, col;
	bool exitGame = false;

	while (true)
	{
		draw();

		cin >> choose;

		if (choose == 2) return;
		else if (choose == 1) break;
		clearWindow();
	}

	while (true)
	{
		cin >> row >> col;

		if (row<1 || row>height || col<1 || col>width)
		{
			MessageBox(NULL, L"Номера строки и столбца должны быть в пределах размера поля (начиная с 1).", L"Ошибка", MB_ICONERROR | MB_OK);
			clearWindow();
			draw();
		}
		else break;

	}

	for (short i = 0; i < height; i++)
	{
		for (short j = 0; j < width; j++)
		{
			if (row != i + 1 || col != j + 1)
			{
				int rn = getNumberInRange(1, 100);
				if (rn < 10) map[i][j] = MINE;
			}
		}
	}

	makeRoad(row - 1, col - 1);

	clearWindow();

	while (true)
	{

		draw();

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
			case COVERED:
				map[row - 1][col - 1] = FLAGGED;
				break;
			case MINE:
				map[row - 1][col - 1] = FLAGGED_MINE;
				break;
			case FLAGGED:
				map[row - 1][col - 1] = COVERED;
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

	draw(false);

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

void Map::init()
{

	for (short i = 0; i < height; i++)
	{
		map.push_back({});
		for (short j = 0; j < width; j++)
			map[i].push_back(COVERED);
	}

}
