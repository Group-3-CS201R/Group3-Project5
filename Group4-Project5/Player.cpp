#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Player.h"
#include "Property.h"

using namespace std;

Player::Player() {}

Player::Player(string name, int num) : playerName(name), playerNum(num), netWorth(1500), playerPosition(1),
inJail(false), rollsInJail(0), numRailRoads(0), playerDoubles(0), numUtils(0)
{
	// Creates entries in the map that correspond with the properties owned by a player of each color
	colorMap.insert({ "PINK", vector<Property*>()});
	colorMap.insert({ "ORANGE", vector<Property*>() });
	colorMap.insert({ "RED", vector<Property*>() });
	colorMap.insert({ "YELLOW", vector<Property*>() });
	colorMap.insert({ "GREEN", vector<Property*>() });
	colorMap.insert({ "NAVY", vector<Property*>() });
	colorMap.insert({ "PURPLE", vector<Property*>() });
	colorMap.insert({ "BLUE", vector<Property*>() });
	colorMap.insert({ "BROWN", vector<Property*>() });
}

// textual output of player info. to-be deleted if/when GUI is created
void Player::PrintPlayerInfo() {
	cout << playerName << endl;
	cout << playerNum << endl;
	cout << netWorth << endl;
}

//FIXME: Change to 39 after testing
void Player::MovePosition(int toMove) {
	playerPosition += toMove;
	if (playerPosition > 39) {
		playerPosition -= 39;
		cout << "\nYou passed go. You receive $200 from the bank\n";
		netWorth += 200;
	}
}

int Player::GetPosition() {
	return playerPosition;
}

void Player::GoToJail() {
	cout << "You have been put in jail. You will need to either roll a double, pay $50 to get out, or wait 3 turns then pay $50" << endl;
	inJail = true;
}

bool Player::IsInJail() {  //increments the turns in jail, can be switched to void probably
	if (rollsInJail == 3) { 
		inJail = false; 
		rollsInJail = 0;
		netWorth -= 50;
	}
	rollsInJail++;
	if (inJail) {
		return true;
	}
	else {
		return false;
	}
	return inJail;
}

void Player::FreeFromJail() {  //increments the turns in jail, can be switched to void probably
	inJail = false;
}

bool Player::GetInJail() { // a getter for if they are in jail or not
	return inJail;
}

int Player::GetNetWorth() {
	return netWorth;
}
string Player::GetName() {
	return playerName;
}
string Player::GetColor() {
	return playerColor;
}

// Gets the vector containing the positions of the properties owned
vector<int> Player::GetVect() {
	return propsOwned;
}

// Increments the number of utils owned by a player. Used to caluculate rent
void Player::AddUtil() {
	numUtils++;
}

int Player::GetNumUtils() {
	return numUtils;
}

// Increments the number of railroads owned by a user. Used to calculate rent
void Player::AddRailroad() {
	numRailRoads++;
}

// Adds pointer value to the color map
void Player::AddToColorMap(Property* property) {
	colorMap[property->GetColor()].push_back(property);
}

int Player::GetNumRailroads() {
	return numRailRoads;
}

void Player::SetPosition(int position) {
	playerPosition = position;
}

// Offers user the ability to purchase a hotel if they own 4 houses on a property
void Player::PurchaseHotel(Property& property) {
	string userInp = "n";
	while (true) {
		cout << "You already own 4 houses on this property. Would you like to purchase a hotel? Cost: $" << property.GetHotelCost() << endl;
		cout << "Press y to purchase and n to not purchase." << endl;
		getline(cin, userInp);
		if (userInp == "y" || userInp == "Y") {
			if (netWorth > property.GetHotelCost()) {
				property.AddHouse();
				netWorth -= property.GetHotelCost();
				break;
			}
			else {
				cout << "You do not have enough money to purchase this hotel!" << endl;
				return;
			}
		}
		else if (userInp == "n" || userInp == "N") {
			break;
		}
		else {
			cout << "Invalid input. You must enter either y or n." << endl << endl;
		}
	}
}

// Offers user the ability to purchase a house on a property if all of a color are owned
void Player::PurchaseHouse(Property& property) {
	if (property.GetNumHouses() == 5) {
		cout << "You already own a hotel here. You can no longer build on this property." << endl << endl;
		return;
	}
	if (property.GetNumHouses() == 4) {
		PurchaseHotel(property);
		return;
	}
	int houseNum1;
	int houseNum2;
	int propNum = property.GetNumHouses();
	string color = property.GetColor();
	vector<Property*> colorProps = colorMap[color];
	// Check if player owns the minimum needed properties to purchase house for the color
	if ((color == "PINK" || color == "ORANGE" || color == "RED" || color == "YELLOW" || color == "GREEN" || color == "BLUE") && colorMap[color].size() == 3) {
		// Creates comparison variables based off of what the passed property is
		if (property.GetName() == colorProps.at(0)->GetName()) {
			houseNum1 = colorProps.at(1)->GetNumHouses();
			houseNum2 = colorProps.at(2)->GetNumHouses();
		}
		else if (property.GetName() == colorProps.at(1)->GetName()) {
			houseNum1 = colorProps.at(0)->GetNumHouses();
			houseNum2 = colorProps.at(2)->GetNumHouses();
		}
		else {
			houseNum1 = colorProps.at(0)->GetNumHouses();
			houseNum2 = colorProps.at(1)->GetNumHouses();
		}
		// Checks that houses are being build evenly
		if ((propNum == houseNum1 && propNum == houseNum2) || (abs((propNum + 1) - houseNum1) <= 1 && abs((propNum + 1) - houseNum2) <= 1)) {
			if (netWorth < property.GetHouseCost()) { // checks that player has enough money to purchase the house
				cout << "You don't have enough money to purchase this house!" << endl; 
			}
			else {
				netWorth -= property.GetHouseCost();
				property.AddHouse();
			}
		}
		else {
			cout << "You must buy houses evenly. Purchase a house on aother property(or properties) of this color to purchase a house on this space." << endl;
		}
	}
	// Check if player owns the minimum needs properties to purchase house for the color
	else if ((color == "NAVY" || color == "PURPLE" || color == "BROWN") && colorMap[color].size() == 2) {
		if (property.GetName() == colorProps.at(0)->GetName()) {
			houseNum1 = colorProps.at(1)->GetNumHouses();
		}
		else {
			houseNum1 = colorProps.at(0)->GetNumHouses();
		}
		// Checks if houses are being purchased evenly
		if ( (propNum == houseNum1) || (abs((propNum + 1) - houseNum1) <= 1)) { // Checks if player has enough money to purchase the house
			if (netWorth > property.GetHouseCost()) {
				cout << "You don't have enough money to purchase this house!" << endl;
			}
			else {
				netWorth -= property.GetHouseCost();
				property.AddHouse();
			}
		}
		else {
			cout << "You must buy houses evenly. Purchase a house on aother property(or properties) of this color to purchase a house on this space." << endl;
		}
	}
	else {
		cout << "You must own all of the properties of a color to purchase a house." << endl << endl;
	}
}

// Pays rent when an owned property is landed on
void Player::PayRent(int toPay) {
	if (netWorth >= toPay) {
		netWorth -= toPay;
	}
	else {
		netWorth = 0;	
	}
}

// Collects rent one of your properties is landed on by another player
void Player::CollectRent(int toCollect) {
	netWorth += toCollect;
}

// adds item to owned properties to handle the case of bankruptcy 
void Player::PurchaseProperty(int propCost, int position) {
	netWorth -= propCost;
	propsOwned.push_back(position);
}

// Checks if player is out of money, meaning that the player is bankrupt
bool Player::IsBankrupt() {
	if (netWorth <= 0) { return true; }
	return false;
}

int Player::DoublesRolled() { // a getter for how many doubles a player has rolled
	return playerDoubles;
}

void Player::ResetDoubles() { // resets the doubles to 0
	playerDoubles = 0;
}

void Player::IncrementDoubles() { // increments the player doubles
	playerDoubles += 1;
	if (playerDoubles == 4) {
		playerDoubles = 1;
	}
}