#pragma once
#include <iostream>
#include <string>

#include "Tile.h"

using namespace std;

class Utility : public Tile {
public:
	Utility() {};
	Utility(string name, int cost, int mortgage);
	void PrintDescription();
	int GetOwnedBy();
	void SetOwnedBy(int num);
	int GetCost();
	bool PropIsOwned();
	void ReleaseProp();
	int GetRent(int roll, int numUtils);
private:
	bool propOwned;
	int ownedByNum;
	int propCost;
	int mortgageValue;
	int numOwned;
};