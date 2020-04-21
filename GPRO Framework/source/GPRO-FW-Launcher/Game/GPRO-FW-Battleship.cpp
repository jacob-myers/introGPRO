
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

//-----------------------------------------------------------------------------
// DECLARATIONS

#define GS_BATTLESHIP_PLAYERS				2
#define GS_BATTLESHIP_BOARD_WIDTH			10
#define GS_BATTLESHIP_BOARD_HEIGHT			10

enum gs_battleship_space_state
{
	// invalid space state
	gs_battleship_space_invalid = -1,

	// states visible to both players
	gs_battleship_space_open,			// space is open and unguessed
	gs_battleship_space_miss,			// space was guessed and missed
	gs_battleship_space_hit,			// space was guessed and hit

	// states hidden from opponent player
	gs_battleship_space_patrol2,		// 2-unit patrol boat
	gs_battleship_space_submarine3,		// 3-unit submarine
	gs_battleship_space_destroyer3,		// 3-unit destroyer
	gs_battleship_space_battleship4,	// 4-unit battleship
	gs_battleship_space_carrier5,		// 5-unit carrier
};
#ifndef __cplusplus
typedef		enum gs_battleship_space_state	gs_battleship_space_state;
#endif	// !__cplusplus

// battleship game state
typedef		unsigned char					gs_battleship_index;
typedef		gs_battleship_space_state		gs_battleship[GS_BATTLESHIP_PLAYERS][GS_BATTLESHIP_BOARD_WIDTH][GS_BATTLESHIP_BOARD_HEIGHT];


#define	GS_VALIDATE_COORDINATE(x,y,z,w,h,d)		(x < w && y < h && z < d)
#define GS_BATTLESHIP_VALID(p,x,y)				GS_VALIDATE_COORDINATE(x,y,p,GS_BATTLESHIP_BOARD_WIDTH,GS_BATTLESHIP_BOARD_HEIGHT,GS_BATTLESHIP_PLAYERS)


inline gs_battleship_space_state gs_battleship_getSpaceState(gs_battleship const game, gs_battleship_index const player, gs_battleship_index const xpos, gs_battleship_index const ypos)
{
	if (GS_BATTLESHIP_VALID(player, xpos, ypos))
		return (game[player][xpos][ypos]);
	return gs_battleship_space_invalid;
}

inline gs_battleship_space_state gs_battleship_setSpaceState(gs_battleship game, gs_battleship_space_state const state, gs_battleship_index const player, gs_battleship_index const xpos, gs_battleship_index const ypos)
{
	if (GS_BATTLESHIP_VALID(player, xpos, ypos))
		return (game[player][xpos][ypos] = state);
	return gs_battleship_space_invalid;
}

inline gs_battleship_index gs_battleship_reset(gs_battleship game)
{
	gs_battleship_index player, xpos, ypos, total;
	for (player = 0; player < GS_BATTLESHIP_PLAYERS; ++player)
		for (xpos = 0; xpos < GS_BATTLESHIP_BOARD_WIDTH; ++xpos)
			for (ypos = 0; ypos < GS_BATTLESHIP_BOARD_HEIGHT; ++ypos)
				game[player][xpos][ypos] = gs_battleship_space_open;
	total = (player * xpos * ypos);
	return total;
}


//-----------------------------------------------------------------------------
// DEFINITIONS

void display(gs_battleship game, int player, bool self)
{
	for (int row = 0; row < GS_BATTLESHIP_BOARD_HEIGHT; row++)
	{
		for (int col = 0; col < GS_BATTLESHIP_BOARD_WIDTH; col++)
		{
			if (gs_battleship_getSpaceState(game, player, col, row) == gs_battleship_space_open)
			{
				cout << "- ";
			}
			else if (gs_battleship_getSpaceState(game, player, col, row) == gs_battleship_space_miss)
			{
				cout << "  ";
			}
			else if (gs_battleship_getSpaceState(game, player, col, row) == gs_battleship_space_hit)
			{
				cout << "X ";
			}
			else if (gs_battleship_getSpaceState(game, player, col, row) > 2 && self)
			{
				cout << "O ";
			}
			else
			{
				cout << "- ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void guess(gs_battleship& game, int targetPlayer)
{
	bool doneInput = false;
	while (!doneInput)
	{
		int ROW = 0, COL = 0;
		cout << "Enter a position (ROW COL) to guess: ";
		cin >> ROW >> COL;

		if (gs_battleship_getSpaceState(game, targetPlayer, COL - 1, ROW - 1) == 0)
		{
			if (gs_battleship_setSpaceState(game, gs_battleship_space_miss, targetPlayer, COL - 1, ROW - 1) == gs_battleship_space_miss)
			{
				doneInput = true;
			}
		}
		else if (gs_battleship_getSpaceState(game, targetPlayer, COL - 1, ROW - 1) > 2)
		{
			if (gs_battleship_setSpaceState(game, gs_battleship_space_hit, targetPlayer, COL - 1, ROW - 1) == gs_battleship_space_hit)
			{
				doneInput = true;
			}
		}
	}
}

//1 = Up 2 = Down 3 = Left 4 = Right
bool placeShip(gs_battleship& game, int player, int x, int y, int dir, gs_battleship_space_state state, int length)
{
	if (dir == 1 || dir == 2)
	{
		for (int i = 0; i < length; i++)
		{
			if (dir == 1)
			{
				if (gs_battleship_getSpaceState(game, player, x, y - i) != gs_battleship_space_open)
					return false;
			}

			else
			{
				if (gs_battleship_getSpaceState(game, player, x, y + i) != gs_battleship_space_open)
					return false;
			}

		}

		for (int i = 0; i < length; i++)
		{
			if (dir == 1)
			{
				gs_battleship_setSpaceState(game, state, player, x, y - i);
			}

			else
			{
				gs_battleship_setSpaceState(game, state, player, x, y + i);
			}
		}
	}

	else if (dir == 3 || dir == 4)
	{
		for (int i = 0; i < length; i++)
		{
			if (dir == 3)
			{
				if (gs_battleship_getSpaceState(game, player, x - i, y) != gs_battleship_space_open)
					return false;
			}

			else
			{
				if (gs_battleship_getSpaceState(game, player, x + i, y) != gs_battleship_space_open)
					return false;
			}

		}

		for (int i = 0; i < length; i++)
		{
			if (dir == 3)
			{
				gs_battleship_setSpaceState(game, state, player, x - i, y);
			}

			else
			{
				gs_battleship_setSpaceState(game, state, player, x + i, y);
			}
		}
	}

	return true;
}

void getShipPlaces(gs_battleship& game, int player)
{
	int x, y, dir = 0;

	while (true) //Place Patrol
	{
		cout << "Enter x and y for the patrol ship(Length: 2): ";
		cin >> x >> y;

		cout << "Enter direction for the patrol ship(Up: 1, Down: 2, Left: 3, Right: 4): ";
		cin >> dir;

		if (placeShip(game, player, x - 1, y - 1, dir, gs_battleship_space_patrol2, 2))
		{
			break;
		}

		else
		{
			cout << "Invalid placement" << endl;
		}
	}

	while (true) // Place Sub
	{
		cout << "Enter x and y for the submarine(Length: 3): ";
		cin >> x >> y;

		cout << "Enter direction for the submarine(Up: 1, Down: 2, Left: 3, Right: 4): ";
		cin >> dir;

		if (placeShip(game, player, x - 1, y - 1, dir, gs_battleship_space_submarine3, 3))
		{
			break;
		}

		else
		{
			cout << "Invalid placement" << endl;
		}
	}

	while (true) // Place Destroy
	{
		cout << "Enter x and y for the destroyer(Length: 3): ";
		cin >> x >> y;

		cout << "Enter direction for the destroyer(Up: 1, Down: 2, Left: 3, Right: 4): ";
		cin >> dir;

		if (placeShip(game, player, x - 1, y - 1, dir, gs_battleship_space_destroyer3, 3))
		{
			break;
		}

		else
		{
			cout << "Invalid placement" << endl;
		}
	}

	while (true) // Place Battle
	{
		cout << "Enter x and y for the battleship(Length: 4): ";
		cin >> x >> y;

		cout << "Enter direction for the battleship(Up: 1, Down: 2, Left: 3, Right: 4): ";
		cin >> dir;

		if (placeShip(game, player, x - 1, y - 1, dir, gs_battleship_space_battleship4, 4))
		{
			break;
		}

		else
		{
			cout << "Invalid placement" << endl;
		}
	}

	while (true) // Place Carrier
	{
		cout << "Enter x and y for the carrier(Length: 5): ";
		cin >> x >> y;

		cout << "Enter direction for the carrier(Up: 1, Down: 2, Left: 3, Right: 4): ";
		cin >> dir;

		if (placeShip(game, player, x - 1, y - 1, dir, gs_battleship_space_carrier5, 5))
		{
			break;
		}

		else
		{
			cout << "Invalid placement" << endl;
		}
	}
}

int getWon(gs_battleship game)
{ 
	int player1Hits = 0, player2Hits = 0;
	for (int row = 0; row < GS_BATTLESHIP_BOARD_HEIGHT; row++)
	{
		for (int col = 0; col < GS_BATTLESHIP_BOARD_WIDTH; col++)
		{
			if (gs_battleship_getSpaceState(game, 0, col, row) == gs_battleship_space_hit)
			{
				player1Hits++;
			}
		}
	}

	for (int row = 0; row < GS_BATTLESHIP_BOARD_HEIGHT; row++)
	{
		for (int col = 0; col < GS_BATTLESHIP_BOARD_WIDTH; col++)
		{
			if (gs_battleship_getSpaceState(game, 0, col, row) == gs_battleship_space_hit)
			{
				player2Hits++;
			}
		}
	}

	if (player1Hits == 17)
	{
		return 1;
	}
	else if (player2Hits == 17)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int launchBattleship()
{
	gs_battleship game;// = { 0 };

	gs_battleship_reset(game);

	getShipPlaces(game, 0);
	getShipPlaces(game, 1);

	bool done = false;
	int player = 0;
	int targetPlayer = 1;
	
	while (!done)
	{
		cout << "Your board: " << endl;
		display(game, player, true);
		//cout << "Press any key to continue...";
		//cin.get();
		//system("CLS");

		cout << "Enemy board: " << endl;
		display(game, targetPlayer, false);
		guess(game, targetPlayer);
		cout << "Enemy board: " << endl;
		display(game, targetPlayer, false);

		if (getWon(game) != -1)
		{
			if (getWon(game) == 0)
				cout << "Player 1 Wins!" << endl;
			else
				cout << "Player 2 Wins!" << endl;
			break;
		}

		cout << "Pass to other player before continuing...";
		cin.get();
		cin.get();
		system("CLS");
		if (targetPlayer == 0)
		{
			targetPlayer = 1;
			player = 0;
		}
		else
		{
			targetPlayer = 0;
			player = 1;
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
