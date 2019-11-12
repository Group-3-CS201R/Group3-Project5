#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Dice.h"

using namespace std;

// constructs Dice by seeding the random number generator and setting currRoll to 0
Dice::Dice() : currRoll(0) {
	srand(time(NULL));
}

int Dice::GetDiceValue() {
	if (currRoll != 0) {
		return currRoll;
	}
}

void Dice::RollDice() {
	currRoll = rand() % 6 + 1;
}