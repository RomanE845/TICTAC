#include <iostream>
#include <iomanip>
#include <random>
#include <stdlib.h>
#include <chrono>

using namespace std;

enum Cell {
	CROSS = 'X',
	ZERO = 'O',
	EMPTY = '.',
};

enum GameProgress {
	GAME_CONT,
	GAME_END,
	WIN,
	LOOSE,
	DRAW
};

struct Coord {
	size_t x;
	size_t y;
};

int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

struct Game {
	GameProgress progress;
	Cell human;
	Cell AI;
	char** arrField;
	size_t size;
	size_t lead;

};

void clearSCR() {
	system("cls");
}

void initField(Game& f) {
	cout << "input size of field" << endl;
	cin >> f.size;
	f.arrField = new char* [f.size];
	for (size_t i = 0; i < f.size; i++)
		f.arrField[i] = new char[f.size];

	for (int x = 0; x < f.size; x++)
		for (int y = 0; y < f.size; y++)
			f.arrField[x][y] = EMPTY;

	f.progress = GAME_CONT;

	if (getRandomNum(0, 1000) > 500)
	{
		f.AI = CROSS;
		f.human = ZERO;
		f.lead = 0;
	}
	else
	{
		f.AI = ZERO;
		f.human = CROSS;
		f.lead = 1;
	}

}

void deConstrField(Game& f) {
	for (size_t i = 0; i < f.size; i++)
		delete[] f.arrField[i];
	delete[] f.arrField;
	f.arrField = nullptr;
}

void printField(const Game& f) {
	f.arrField;

	cout << "human : " << (char)f.human << endl\
		<< "robot : " << (char)f.AI << endl << endl;
	cout << "    ";
	for (int y = 0; y < f.size; y++)
	{
		cout << setw(3) << "Y" << y + 1;
	}
	for (int x = 0; x < f.size; x++)
	{
		cout << endl << "X" << x + 1 << " " << "|  ";
		for (int y = 0; y < f.size; y++)
		{
			cout << f.arrField[x][y] << setw(4);
		}
		cout << "|";
		cout << endl;
	}



}

Coord getCoordH(Game& g)
{
	Coord ms{ 0 };
	do
	{
		cout << "Enter X coord: ";
		cin >> ms.x;
		cout << "Enter Y coord: ";
		cin >> ms.y;
	} while (ms.x > g.size || ms.y > g.size || g.arrField[ms.x - 1][ms.y - 1] != EMPTY);
	ms.x--;
	ms.y--;
	return ms;
}

GameProgress value(Game& g);

Coord getCoordA(Game& g)
{
	for (size_t x = 0; x < g.size; x++)
	{
		for (size_t y = 0; y < g.size; y++)
		{
			if (g.arrField[x][y] == EMPTY)
			{
				g.arrField[x][y] = g.AI;
				if (value(g) == LOOSE)
				{
					g.arrField[x][y] = EMPTY;
					return{ x , y };
				}
				g.arrField[x][y] = EMPTY;
			}
		}
	}

	for (size_t x = 0; x < g.size; x++)
	{
		for (size_t y = 0; y < g.size; y++)
		{
			if (g.arrField[x][y] == EMPTY)
			{
				g.arrField[x][y] = g.human;
				if (value(g) == WIN)
				{
					g.arrField[x][y] = EMPTY;
					return{ x , y };
				}
				g.arrField[x][y] = EMPTY;
			}
		}
	}
	if (g.arrField[1][1] == EMPTY)
	{
		return { 1, 1 };
	}
	if (g.arrField[0][0] == EMPTY)
	{
		return { 0, 0 };
	}
	if (g.arrField[0][2] == EMPTY)
	{
		return { 0, 2 };
	}
	if (g.arrField[2][0] == EMPTY)
	{
		return { 2, 0 };
	}
	if (g.arrField[2][2] == EMPTY)
	{
		return { 2, 2 };
	}
	if (g.arrField[1][0] == EMPTY)
	{
		return { 1, 0 };
	}
	if (g.arrField[0][1] == EMPTY)
	{
		return { 0, 1 };
	}
	if (g.arrField[2][1] == EMPTY)
	{
		return { 2, 1 };
	}
	if (g.arrField[1][2] == EMPTY)
	{
		return { 1, 2 };
	}
}

GameProgress value(Game& g)
{
	int val = 0;
	int aval = 0;
	int dval = 0;
	int odval = 0;
	int val1 = 0;
	int aval1 = 0;
	int dval1 = 0;
	int odval1 = 0;
	bool empty = false;
	for (int x = 0; x < g.size; x++)
	{
		for (int y = 0; y < g.size; y++)
		{
			if (g.arrField[y][x] == g.human)
				val++;

			if (g.arrField[x][y] == g.human)
				aval++;

			if (g.arrField[y][y] == g.human)
				dval++;

			if (g.arrField[(g.size - 1) - y][y] == g.human)
				odval++;

			if (g.arrField[y][x] == g.AI)
				val1++;

			if (g.arrField[x][y] == g.AI)
				aval1++;

			if (g.arrField[y][y] == g.AI)
				dval1++;

			if (g.arrField[(g.size - 1) - y][y] == g.AI)
				odval1++;

			if (g.arrField[x][y] == EMPTY)
				empty = true;
		}
		if (val == 3 || aval == g.size || dval == g.size || odval == g.size)
		{
			return WIN;
			break;
		}

		if (val1 == g.size || aval1 == g.size || dval1 == g.size || odval1 == g.size)
		{
			return LOOSE;
			break;
		}
		val = 0;
		aval = 0;
		dval = 0;
		odval = 0;

		val1 = 0;
		aval1 = 0;
		dval1 = 0;
		odval1 = 0;
	}

	if (empty == true)
		return GAME_CONT;
	else
		return DRAW;

}

int main()
{
	Game g;
	initField(g);
	clearSCR();
	printField(g);

	while (g.progress == GAME_CONT)
	{
		if (g.lead % 2 == 0)
		{
			Coord ms = getCoordH(g);
			g.arrField[ms.x][ms.y] = g.human;
		}
		else
		{
			Coord ms = getCoordA(g);
			g.arrField[ms.x][ms.y] = g.AI;
		}
		g.lead++;
		clearSCR();
		printField(g);

		g.progress = value(g);
	}

	switch (g.progress)
	{
	case (WIN): cout << "congratulations, you won!" << endl;
		break;
	case (LOOSE): cout << "You've lost, go get some exercise" << endl;
		break;
	case (DRAW): cout << "Draw" << endl;
	default:
		break;
	}

	deConstrField(g);
}

