#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "dice.h"

class Character {
protected: // 繼承可以直接訪問
    std::string characterName; // 宣告角色名稱
    Dice dice;                 // 宣告角色的骰子

public:
    Character(const std::string& characterName, const Dice& dice);
    const std::string& getName() const; // 提供訪問角色名稱的方法

    // Roll the character's dice
    int rollDice() const;
};

#endif // CHARACTER_H
