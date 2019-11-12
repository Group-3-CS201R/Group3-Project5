#pragma once
#include <iostream>
#include <string>

#include "Tile.h"

using namespace std;

class Action : public Tile {
public:
	Action() {};
	Action(string name);
	string GetActionType();
	void PrintDescription();
private:
};
