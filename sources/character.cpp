#include "character.h"

// Constructor
Character::Character(const std::string& characterName, const Dice& dice)
    : characterName(characterName), dice(dice) {}

 // 返回角色名稱
const std::string& Character::getName() const {
    return characterName;
}

// Roll the character's dice
int Character::rollDice() const {
    return dice.roll();
}