#ifndef DICE_H
#define DICE_H

#include <vector>

class Dice {
private:
    std::vector<int> faces;

public:
    // Constructor
    Dice(const std::vector<int>& faceValues);

    // Roll the dice and get a random face value
    int roll() const;
};

#endif // DICE_H
