#pragma once
#include <iostream> 
#include <string>
#include <vector>

#include "Tile.h"

using namespace std;

class Railroad : public Tile {
public:
	Railroad() {};
	Railroad(string name, int cost, int rentBase, int rentWithTwo, int rentWithThree, int rentWithFour, int mortgage);
	void PrintDescription();
	int GetOwnedBy();
	void SetOwnedBy(int num);
	int GetCost();
	bool PropIsOwned();
	void ReleaseProp();
	int GetRent(int numRailroads);
private:
	bool propOwned;
	int ownedByNum;
	int propCost;
	int mortgageValue;
	int numOwned;
	vector<int> rents;
};