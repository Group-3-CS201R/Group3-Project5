#include <iostream>
#include <string>

#include "Action.h"

using namespace std;

Action::Action(string name) : Tile(name, "Action") {}

string Action::GetActionType() {
	return this->GetName();
}

//uses action type to print out the proper description
void Action::PrintDescription() {
	if (this->GetName() == "GO") {
		cout << "This is the GO tile, once you pass this tile you will recieve $200" << endl;
	}
	else if (this->GetName() == "FREE_PARKING") {
		cout << endl << this->GetName() << endl;
		cout << endl << "You are free to stay on this space until the next turn." << endl;
	}
	else if (this->GetName() == "GO_TO_JAIL") {
		cout << endl << this->GetName() << endl;
		cout << endl << "You are hereby sentenced to jail." << endl;
	}
	else if (this->GetName() == "COMMUNITY_CHEST") {
		cout << endl << this->GetName() << endl;
		cout << endl << "Draw a Community Chest card and resolve its effect." << endl;
	}
	else if (this->GetName() == "CHANCE") {
		cout << endl << this->GetName() << endl;
		cout << endl << "Draw a Community Chest card and resolve its effect." << endl;
	}
	else {
		cout << this->GetName() << endl;
	}
}
