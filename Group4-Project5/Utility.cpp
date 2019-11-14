#include <iostream>
#include <string>

#include "Utility.h"
#include "Tile.h"

using namespace std;

Utility::Utility(string name, int cost, int mortgage) : Tile(name, "Utility"), propCost(cost),
	mortgageValue(mortgage), numOwned(0), propOwned(false), ownedByNum(0) {}

int Utility::GetOwnedBy() {
	return ownedByNum;
}

void Utility::SetOwnedBy(int num) {
	ownedByNum = num;
	propOwned = true;
}

int Utility::GetCost() {
	return propCost;
}

bool Utility::PropIsOwned() {
	return propOwned;
}

// Releases ownership variables. Used when player goes bankrupt
void Utility::ReleaseProp() {
	ownedByNum = 0;
	propOwned = false;
}

// Calculates rent using the dice roll and the number of utilities to get this number
int Utility::GetRent(int roll, int numUtils) {
	if (numUtils == 1) {
		return roll * 4;
	}
	else {
		return roll * 10;
	}
}

void Utility::PrintDescription() {
	cout << this->GetName() << endl;
	cout << "Cost: $" << propCost << endl;
	cout << "Mortgage value: $" << mortgageValue << endl;
}
