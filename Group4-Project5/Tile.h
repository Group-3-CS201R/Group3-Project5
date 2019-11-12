#pragma once
#include <iostream>
#include <string>

using namespace std;

class Tile {
public: 
	Tile();
	Tile(string name, string type);
	string GetName();
	string GetType();
	virtual void PrintDescription();
private:
	string tileName;
	string tileType;
};