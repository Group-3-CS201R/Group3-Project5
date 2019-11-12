#include <iostream>
#include <string>
#include <vector>

#include "Player.h"

using namespace std;

Player::Player() {}

Player::Player(string name, int num) : playerName(name), playerNum(num), netWorth(1500), playerPosition(1),
inJail(false), rollsInJail(0), numRailRoads(0), playerDoubles(0), numUtils(0)
{
	numOfColor.insert({ "PINK", 0 });
	numOfColor.insert({ "ORANGE", 0 });
	numOfColor.insert({ "RED", 0 });
	numOfColor.insert({ "YELLOW", 0 });
	numOfColor.insert({ "GREEN", 0 });
	numOfColor.insert({ "NAVY", 0 });
	numOfColor.insert({ "PURPLE", 0 });
	numOfColor.insert({ "BLUE", 0 });
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

vector<int> Player::GetVect() {
	return propsOwned;
}

void Player::AddUtil() {
	numUtils++;
}

int Player::GetNumUtils() {
	return numUtils;
}

void Player::AddRailroad() {
	numRailRoads++;
}

void Player::AddToColorMap(string colorToAdd) {
	numOfColor[colorToAdd]++;
}

int Player::GetNumRailroads() {
	return numRailRoads;
}

void Player::SetPosition(int position) {
	playerPosition = position;
}

bool Player::CanPurchaseHouse(string colorToPurchase) {
	if (colorToPurchase == "PINK" && numOfColor[colorToPurchase] == 3) {
		return true;
	} else if (colorToPurchase == "ORANGE" && numOfColor[colorToPurchase] == 3) {
		return true;
	} else if (colorToPurchase == "RED" && numOfColor[colorToPurchase] == 3) {
		return true;
	} else if (colorToPurchase == "YELLOW" && numOfColor[colorToPurchase] == 3) {
		return true;
	} else if (colorToPurchase == "GREEN" && numOfColor[colorToPurchase] == 3) {
		return true;
	} else if (colorToPurchase == "PINK" && numOfColor[colorToPurchase] == 3) {
		return true;
	} else if (colorToPurchase == "NAVY" && numOfColor[colorToPurchase] == 2) {
		return true;
	} else if (colorToPurchase == "PURPLE" && numOfColor[colorToPurchase] == 2) {
		return true;
	} else if (colorToPurchase == "BLUE" && numOfColor[colorToPurchase] == 3) {
		return true;
	}
	else {
		return false;
	}
}

void Player::PayRent(int toPay) {
	if (netWorth >= toPay) {
		netWorth -= toPay;
	}
	else {
		netWorth = 0;	
	}
}

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