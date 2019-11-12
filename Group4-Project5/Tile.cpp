#include <iostream>
#include <string>

#include "Tile.h"

using namespace std;

Tile::Tile() : tileName(""), tileType("") {}

Tile::Tile(string name, string type) : tileName(name), tileType(type) {}

string Tile::GetName() {
	return tileName;
}

string Tile::GetType() {
	return tileType;
}

void Tile::PrintDescription() {
	cout << "" << endl;
}

