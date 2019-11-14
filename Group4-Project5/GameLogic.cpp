#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>

#include "Dice.h"
#include "Property.h"
#include "Player.h"
#include "Railroad.h"
#include "Action.h"
#include "Tile.h"
#include "Utility.h"
#include "GameLogic.h"

using namespace std;

GameLogic::GameLogic() {
	FillPlayersVect();
	dice1 = Dice();
	dice2 = Dice();
	currentTurn = 0;
	jailLocation = 10;
	FillGameBoard();
}

void GameLogic::PlayGame() {
	int numDoubles = 0;
	int turnRoll = 0;
	int dummy = 0;
	string stallyBoi;
	while (true) {
		if (players.at(currentTurn).GetInJail()) { // checks to see if the player is in jail
			JailSequence();
			continue;
		}
		cout << endl << "Player: " << (currentTurn + 1) << "\nPress p to print owned properties, h to purchase house/hotel, and anything else to roll. -> "; // stalls game until user is ready to roll
		cin >> stallyBoi;
		cin.ignore();
		if (stallyBoi == "P" || stallyBoi == "p") { // Displays the properties that are owned by the player whos' turn it currently is
			PrintPropsOwned(players.at(currentTurn).GetVect());
			continue;
		}
		else if (stallyBoi == "H" || stallyBoi == "h") { // Allows player whos' current turn it is to purchase a house or hotel on a property
			PurchaseHouseHotelSequence(players.at(currentTurn).GetVect());
		}
		dice1.RollDice();
		dice2.RollDice();
		turnRoll = dice1.GetDiceValue() + dice2.GetDiceValue(); // turnRoll is the combination of "rolling" the two dice
		cout << endl << "Player: " << (currentTurn + 1) << " Roll: " << turnRoll << endl;
		cout << "\nYou currently have : $" << players.at(currentTurn).GetNetWorth() << "\n\n";
		players.at(currentTurn).MovePosition(turnRoll);
		SequenceDecision(players.at(currentTurn).GetPosition(), turnRoll);
		// this handles the case of 1 or more dice rolls that have the same value
		while (dice1.GetDiceValue() == dice2.GetDiceValue()) {
			players.at(currentTurn).IncrementDoubles();

			if (players.at(currentTurn).DoublesRolled() == 3) { // change this to 1 if you want to test jail
				players.at(currentTurn).SetPosition(jailLocation); // moves player to jail
				players.at(currentTurn).GoToJail(); // changes bool in class
				currentTurn++; // changes player turn and ends the jailed's turn
				players.at(currentTurn).ResetDoubles();
				break;
			}
			// implements an entirely new turn sequence since when double is rolled, the user keeps rolling until a non-double is rolled, or three doubles are rolled and they go to jail
			cout << endl << "You have rolled " << players.at(currentTurn).DoublesRolled() << " double(s). Three doubles in a row will land you in Jail. Shake those dice carefully!" << endl << endl;
			cout << endl << "Player: " << (currentTurn + 1) << "\nPress any key to roll -> ";
			cin >> stallyBoi;
			cin.ignore();
			dice1.RollDice();
			dice2.RollDice();
			turnRoll = dice1.GetDiceValue() + dice2.GetDiceValue();
			cout << endl << "Player: " << (currentTurn + 1) << " Roll: " << turnRoll << endl;
			cout << "\nYou currently have : $" << players.at(currentTurn).GetNetWorth() << "\n\n";
			players.at(currentTurn).MovePosition(turnRoll);
			SequenceDecision(players.at(currentTurn).GetPosition(), turnRoll);
			if (players.at(currentTurn).IsBankrupt()) { break; } // Checks to see if th player has run out of money during their turn
		}
		// this handles the case when the dice rolls do not have the same values
		players.at(currentTurn).ResetDoubles();  // clears the consecutive roll count
		if (players.at(currentTurn).IsBankrupt()) { // checks if the player is bankrupt then executes a sequence free their properties to the bank erase them from the player vector
			cout << endl << players.at(currentTurn).GetName() << " is bankrupt and is now out of the game. All property goes to the bank." << endl;
			BankruptcyHandler(players.at(currentTurn).GetVect());
			players.erase(players.begin() + currentTurn);
		}
		else {
			currentTurn++; // Makes it the next players turn
		}
		if (currentTurn == players.size()) { // checks if the player sequence must be restarted
			currentTurn = 0;
		}
		if (players.size() == 1) { // if only one player remains, they are the winner of the game and the game is complete
			cout << endl << "Player: " << players.at(0).GetName() << " wins the game!" << endl;
			break;
		}
	}
}

// Checks which type of action to take for the tile at the current position
void GameLogic::SequenceDecision(int position, int roll) {
	if (properties.find(position) != properties.end()) {
		PropertySequence(position);
	}
	else if (railroads.find(position) != railroads.end()) {
		RailroadSequence(position);
	}
	else if (utilities.find(position) != utilities.end()) {
		UtilitySequence(position, roll);
	}
	else {
		ActionSequence(position);
	}
}

// Executes when user lands on a railroad tile. If it is owned, the user pays rent. Otherwise, the user is offered the option to purchase the property
void GameLogic::RailroadSequence(int position) {
	int cost = railroads[position].GetCost();
	string userResponse;
	int rent;
	// If the property is owned, than the rent on the property is collected from current player and given to the property owner
	if (railroads[position].PropIsOwned()) {
		cout << railroads[position].GetName() << " railroad is owned by player number: " << railroads[position].GetOwnedBy() + 1 << " you owe $" << railroads[position].GetRent(players.at(currentTurn).GetNumRailroads()) << " in rent.\n";
		rent = railroads[position].GetRent(players.at(currentTurn).GetNumRailroads());
		players.at(currentTurn).PayRent(rent);
		players.at(railroads[position].GetOwnedBy()).CollectRent(rent);
	}
	// 
	else {
		railroads[position].PrintDescription();
		cout << endl << "Would you like to purchase this railroad? (y/n) -> ";
		cin >> userResponse;
		while (userResponse != "y" && userResponse != "Y" && userResponse != "n" && userResponse != "N") {
			cout << "Please give a valid response (y/n)." << endl;
			cout << endl << "Would you like to purchase this railroad? (y/n) -> ";
			cin >> userResponse;
		}
		if (userResponse == "y" || userResponse == "Y") {
			// checks that player has enough money to purchase the proerty in question
			if (players.at(currentTurn).GetNetWorth() >= railroads[position].GetCost()) { // Checks that user has a high enough net worth to purchase the property
				players.at(currentTurn).PurchaseProperty(railroads[position].GetCost(), position);
				railroads[position].SetOwnedBy(currentTurn);
			}
			else { cout << "Sorry, you don't have enough money to purchase this railroad." << endl; }
		}
	}
}

// Executes when user lands on a property tile. If it is owned, the user pays rent. Otherwise, the user is offered the option to purchase the property
void GameLogic::PropertySequence(int position) {
	int cost = properties[position].GetCost();
	string userResponse;
	int rent;
	if (properties[position].PropIsOwned()) {
		cout <<  properties[position].GetName() << " is owned by player number: " << properties[position].GetOwnedBy() + 1 << " you owe $" << properties[position].GetRent() << " in rent.\n";
		rent = properties[position].GetRent();
		players.at(currentTurn).PayRent(rent);
		players.at(properties[position].GetOwnedBy()).CollectRent(rent);
	}
	else {
		properties[position].PrintDescription();
		cout << endl << "Would you like to purchase this property? (y/n) -> ";
		cin >> userResponse;
		while (userResponse != "y" && userResponse != "Y" && userResponse != "n" && userResponse != "N") {
			cout << "Please give a valid response (y/n)." << endl;
			cout << endl << "Would you like to purchase this property? (y/n) -> ";
			cin >> userResponse;
		}
		if (userResponse == "y" || userResponse == "Y") {
			// checks that player has enough money to purchase the proerty in question
			if (players.at(currentTurn).GetNetWorth() >= properties[position].GetCost()) { // Checks that user has a high enough net worth to purchase the property
				players.at(currentTurn).PurchaseProperty(properties[position].GetCost(), position);
				properties[position].SetOwnedBy(currentTurn);
				players.at(currentTurn).AddToColorMap(&properties[position]);
			}
			else { cout << "Sorry, you don't have enough money to purchase this property." << endl; }
		}
	}
}

// Executes when user lands on a utility tile. If it is owned, the user pays rent. Otherwise, the user is offered the option to purchase the property
void GameLogic::UtilitySequence(int position, int roll) {
	int cost = utilities[position].GetCost();
	string userResponse;
	int rent;
	if (utilities[position].PropIsOwned()) {
		cout << utilities[position].GetName() <<" is owned by player number: " << utilities[position].GetOwnedBy() + 1 << " you owe $" << utilities[position].GetRent(roll, players.at(currentTurn).GetNumUtils()) << " in rent.\n";
		rent = utilities[position].GetRent(roll, players.at(currentTurn).GetNumUtils());
		players.at(currentTurn).PayRent(rent);
		players.at(utilities[position].GetOwnedBy()).CollectRent(rent);
	}
	else {
		utilities[position].PrintDescription();
		cout << endl << "Would you like to purchase this utility? (y/n) -> ";
		cin >> userResponse;
		while (userResponse != "y" && userResponse != "Y" && userResponse != "n" && userResponse != "N") {
			cout << "Please give a valid response (y/n)." << endl;
			cout << endl << "Would you like to purchase this utility? (y/n) -> ";
			cin >> userResponse;
		}
		if (userResponse == "y" || userResponse == "Y") {
			// checks that player has enough money to purchase the proerty in question
			if (players.at(currentTurn).GetNetWorth() >= utilities[position].GetCost()) { // Checks that user actually has the funds to purchase the property without going bankrupt
				players.at(currentTurn).PurchaseProperty(utilities[position].GetCost(), position);
				utilities[position].SetOwnedBy(currentTurn);
				players.at(currentTurn).AddUtil();
			}
			else { cout << "Sorry, you don't have enough money to purchase this property." << endl; }
		}
	}
}

// Prints the description of the Action tile that was landed on and then executes the proper action for that tile
void GameLogic::ActionSequence(int position) {
	actions[position].PrintDescription();
	if (actions[position].GetName() == "GO_TO_JAIL") {
		players.at(currentTurn).SetPosition(jailLocation); // moves player to jail
		players.at(currentTurn).GoToJail(); // changes bool in class
	}
	else if (actions[position].GetName() == "INCOME_TAX") {
		players.at(currentTurn).CollectRent(-200);
	}
	else if (actions[position].GetName() == "LUXURY_TAX"){
		players.at(currentTurn).CollectRent(-75);
	}
}

void GameLogic::JailSequence() {
	cout << endl << "Player: " << (currentTurn + 1); // let's you know which player is in jail
	cout << "\nYou are in jail. You can either try to roll doubles or pay $50. If you fail to roll doubles after three turns you must pay $50.\n"; // asks them if they want to roll or pay
	cout << "r) Roll\np) Pay\n";
	string userResponse;
	cin >> userResponse;
	while (userResponse != "r" && userResponse != "R" && userResponse != "p" && userResponse != "P") {
		cout << "Please give a valid response (r/p)." << endl;
		cout << endl << "Would you like to roll or pay? (r/p) -> ";
		cin >> userResponse;
	}
	if ((userResponse == "r") || (userResponse == "R")) {  // if they choose roll the dice are rolled and if they equal they are free, else it increments by one until three
		dice1.RollDice();
		dice2.RollDice();
		cout << "You rolled " << dice1.GetDiceValue() << " and " << dice2.GetDiceValue() << endl;
		if (dice1.GetDiceValue() == dice2.GetDiceValue()) {
			cout << "You are free from jail.\n";
			players.at(currentTurn).FreeFromJail();
		}
		else {
			players.at(currentTurn).IsInJail();
		}
	}
	else if ((userResponse == "p") || (userResponse == "P")) { // if they 
		while (players.at(currentTurn).IsInJail()) {
			players.at(currentTurn).IsInJail();
		}
	}
	
	currentTurn++; // after the jail turn the player is switched
	if (currentTurn == players.size()) {
		currentTurn = 0;
	}
}

// Prints all of the properties that the user owns
void GameLogic::PrintPropsOwned(vector<int> positions) {
	if (positions.size() == 0) {
		cout << "You don't own any properties." << endl;
		return;
	}
	cout << endl << "You own the following properties:" << endl << endl;
	for (int i = 0; i < positions.size(); ++i) {
		if (properties.find(positions.at(i)) != properties.end()) {
			cout << properties[positions.at(i)].GetName() << setw(15) << "Position: " << properties[positions.at(i)].GetPosition() << setw(15) <<  "Color: " << properties[positions.at(i)].GetColor() << setw(15) <<"# houses: " << properties[positions.at(i)].GetNumHouses() << endl;
		}
		else if (railroads.find(positions.at(i)) != railroads.end()) {
			cout << railroads[positions.at(i)].GetName() << setw(15) << "Type: Railroad" << endl;
		}
		else if (utilities.find(positions.at(i)) != utilities.end()) {
			cout << utilities[positions.at(i)].GetName() << setw(15) << "Type: Utility" << endl;
		}
	}
}

// When a player is bankrupt, this takes the positions that the user owns and sets their ownership to 0 and propOwned to false
void GameLogic::BankruptcyHandler(vector<int> positions) {
	for (int i = 1; i < positions.size(); ++i) {
		if (properties.find(i) != properties.end()) {
			properties[i].ReleaseProp();
		}
		else if (railroads.find(i) != railroads.end()) {
			railroads[i].ReleaseProp();
		}
		else if (utilities.find(i) != utilities.end()) {
			utilities[i].ReleaseProp();
		}
	}
}

void GameLogic::PurchaseHouseHotelSequence(vector<int> positions) {
	string stallyBoi = "r";
	int position;
	while (stallyBoi != "q") {
		if (positions.size() == 0) { // Quick check to ensure that user actually owns properties to purchase houses on
			cout << "You don't own any properties. Continuing to turn roll..." << endl;
			return;
		}
		cout << endl << "To purchase a house, you must own all properties of a color. In addition, houses must be purchased evenly." << endl;
		cout << "to purchase a hotel, you must own 4 houses on a property." << endl << endl;
		cout << endl << "You own the following properties:" << endl << endl;
		for (int i = 0; i < positions.size(); ++i) {
			if (properties.find(positions.at(i)) != properties.end()) {
				cout << properties[positions.at(i)].GetName() << "\t\tPosition: " << properties[positions.at(i)].GetPosition() << "\t\tColor: " << properties[positions.at(i)].GetColor() << endl;
			}
		}
		cout << endl << "What property would you like to purchase a house on?" << endl;
		//FIXME: Needs error checking
		cin >> position;
		// Checks if user entered non-numeric value. If so, sequence restarts
		if (cin.fail()) {
			cout << endl << "That is not a valid position!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (ValidPosition(positions, position)) {
			players.at(currentTurn).PurchaseHouse(properties[position]);
		}
		else {
			cout << "You don't own that property!" << endl << endl;
		}
		cout << "Would you like to purchase another house/hotel? Press q to quit, anything else to continue purchasing: ";
		cin >> stallyBoi;
		cout << endl << endl;
	}
}

// Checks if the position that a player chose to build a house or hotel on is one of the positions that the player actually owns
bool GameLogic::ValidPosition(vector<int> positions, int position) {
	bool isValid = false;
	for (int i = 0; i < positions.size(); ++i) {
		if (position == positions.at(i)) {
			isValid = true;
		}
	}
	return isValid;
}

// Reads in from from database of tile info and creates the game board from this information by filling the map corresponding to the tile type
void GameLogic::FillGameBoard() {
	ifstream gameProps("game_props.txt");
	string propType;
	string name;
	string color;
	int cost;
	int rentBase;
	int rent1House;
	int rent2House;
	int rent3House;
	int rent4House;
	int rentHotel;
	int mortgage;
	int houseCost;
	int hotelCost;
	int index = 1;
	while (!gameProps.eof()) {
 		gameProps >> propType;
		// Property types need to have extra information like color and hotel rent in order to function fully
		if (propType == "Property") {
			gameProps >> name;
			gameProps >> color;
			gameProps >> cost;
			gameProps >> rentBase;
			gameProps >> rent1House;
			gameProps >> rent2House;
			gameProps >> rent3House;
			gameProps >> rent4House;
			gameProps >> rentHotel;
			gameProps >> mortgage;
			gameProps >> houseCost;
			gameProps >> hotelCost;
			Property newProp = Property(name, color, cost, rentBase, rent1House, rent2House, rent3House, rent4House, rentHotel, mortgage, houseCost, hotelCost, index);
			properties.insert({ index, newProp });
		}
		// Action tiles simply have a name and the index in the game boards that they are located at
		else if (propType == "Action") {
			gameProps >> name;
			Action newAction = Action(name);
			actions.insert({ index, newAction });
		}
		// Railroad tiles are similar to Property tiles, but don't have a color or a need for hotel rent
		else if (propType == "Railroad") {
			gameProps >> name;
			gameProps >> cost;
			gameProps >> rent1House;
			gameProps >> rent2House;
			gameProps >> rent3House;
			gameProps >> rent4House;
			gameProps >> mortgage;
			Railroad newRR = Railroad(name, cost, rent1House, rent2House, rent3House, rent4House, mortgage);
			railroads.insert({ index, newRR });
		}
		// The final property type is a utility which doesn't have a predefined rent
		else {
			gameProps >> name;
			gameProps >> cost;
			gameProps >> mortgage;
			Utility newUtil = Utility(name, cost, mortgage);
			utilities.insert({ index, newUtil });
		}
		++index;
	}
	gameProps.close();
}

// This function asks for at least two players names' and as many as six players' names upon the opening of the program. This fills a vector of players in the game
void GameLogic::FillPlayersVect() {
	string userName;
	string keepAdding;
	cout << "Welcome to Monopoly!" << endl;
	cout << "Enter at least two, and up to six, players' names to start playing the game." << endl << endl;
	for (int i = 0; i < 6; ++i) { // this limits the maximum number of users in the game to 6
		cout << "Enter name of player #" << (i + 1) << " -> ";
		getline(cin, userName);
		players.push_back(Player(userName, (i + 1)));
		if (i > 0) { // This makes sure that at least two users have been entered before offering the option to start the game
			cout << endl << "Would you like to add another player?\nPress y to add another player, any other key to start playing. -> ";
			getline(cin, keepAdding);
			if (keepAdding != "y" && keepAdding != "Y") {
				break;
			}
		}
		cout << endl;
	}
	cout << endl << "Let's get started!\n" << endl;
  }