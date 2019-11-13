#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Tile.h"

using namespace std;

class Property : public Tile {
public:
	Property() {};
	Property(string propName, string color, int propertyCost, int rentBase, int rentOneHouse, int rentTwoHouse,
		int rentThreeHouse,int rentFourHouse, int rentHotel, int mortgage, int houseBuyCost, int hotelBuyCost, int position);
	void PrintDescription();
	bool PropIsOwned();
	void PurchaseProp();
	void SetOwnedBy(int player);
	string GetColor();
	int GetOwnedBy();
	int GetCost();
	int GetRent();
	void ReleaseProp();
	int GetNumHouses();
	void AddHouse();
	int GetHouseCost();
	int GetPosition();
	int GetMortgage();
private:
	int position;
	int numHouses;
	string propColor;
	bool propOwned;
	int ownedByNum;
	int propCost;
	int housesOnProp;
	vector<int> rents;
	int mortgageValue;
	int houseCost;
	int hotelCost;
	bool hotelIsOwned;
};