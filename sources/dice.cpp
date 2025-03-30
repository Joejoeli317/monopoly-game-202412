#include "dice.h"
#include <cstdlib> // for rand()
#include <ctime>   // for seeding rand()

Dice::Dice(const std::vector<int>& faceValues) : faces(faceValues) {
    std::srand(std::time(nullptr)); // Initialize random seed
}

int Dice::roll() const {
    return faces[std::rand() % faces.size()];
}