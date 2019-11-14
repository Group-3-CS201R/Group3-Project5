#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>

#include "Dice.h"
#include "Property.h"
#include "Player.h"
#include "Railroad.h"
#include "Action.h"
#include "Tile.h"
#include "Utility.h"

using namespace std;

class GameLogic {
public:
	GameLogic();
	void PlayGame();
	void FillGameBoard();
	void SequenceDecision(int position, int roll);
	void PropertySequence(int position);
	void RailroadSequence(int position);
	void UtilitySequence(int position, int roll);
	void ActionSequence(int position);
	void JailSequence();
	void FillPlayersVect();
	void PrintPropsOwned(vector<int> positions);
	void BankruptcyHandler(vector<int> positions);
	void PurchaseHouseHotelSequence(vector<int> positions);
	bool ValidPosition(vector<int> positions, int position);
private:
	Dice dice1;
	Dice dice2;
	Player player1;
	Player player2;
	Player player3;
	Player player4;
	Player player5;
	Player player6;
	vector<Player> players;
	int currentTurn;
	map<int, Property> properties;
	map<int, Railroad> railroads;
	map<int, Utility> utilities;
	map<int, Action> actions;
	int jailLocation;
};