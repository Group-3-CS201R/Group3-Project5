#include <iostream>
#include <string>
#include <vector>

#include "Railroad.h"

using namespace std;

Railroad::Railroad(string name, int cost, int rentBase, int rentWithTwo,
	int rentWithThree, int rentWithFour, int mortgage) : Tile(name, "Railroad"), propCost(cost), numOwned(0), mortgageValue(mortgage),
	propOwned(false), ownedByNum(0)
{
	rents.resize(4);
	rents.at(0) = rentBase;
	rents.at(1) = rentWithTwo;
	rents.at(2) = rentWithThree;
	rents.at(3) = rentWithFour;
}


int Railroad::GetCost() {
	return propCost;
}

int Railroad::GetOwnedBy() {
	return ownedByNum;
}

void Railroad::SetOwnedBy(int player) {
	ownedByNum = player;
	propOwned = true;
}

bool Railroad::PropIsOwned() {
	return propOwned;
}

void Railroad::ReleaseProp() {
	ownedByNum = 0;
	propOwned = false;
}

int Railroad::GetRent(int numRailroads) {
	return rents.at(numRailroads);
}


void Railroad::PrintDescription() {
	cout << this->GetName() << endl;
	cout << "Cost: $" << propCost << endl;
	cout << endl << "Rent: \t$" << rents.at(0) << endl;
	cout << "2 Railroads : \t$" << rents.at(1) << endl;
	cout << "3 Railroads : \t$" << rents.at(2) << endl;
	cout << "4 Railroads : \t$" << rents.at(3) << endl;
	cout << endl << "Mortgage Value $" << mortgageValue << endl;
}