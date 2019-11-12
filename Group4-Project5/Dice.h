#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>

class Dice {
public:
	Dice();
	int GetDiceValue();
	void RollDice();
private:
	int currRoll;
};