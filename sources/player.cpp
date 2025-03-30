#include "player.h"
#include "tile.h"   
#include "item.h" 
#include <iostream>
#include <algorithm> // for std::max

// Constructor
Player::Player(const std::string& playerName, const std::string& characterName, const Dice& dice)
    : Character(characterName, dice), playerName(playerName), coins(0), stars(0), position(0), 
      extraTurn(false), skipTurn(false), speedMultiplier(1), useSpecialDice(false) {}
// 基本屬性
const std::string& Player::getPlayerName() const { return playerName; }
int Player::getId() const { return id; }
int Player::getCoins() const { return coins; }
void Player::addCoins(int amount) {
    coins += amount;
}

// getter
void Player::reduceCoins(int amount) {
    int actualReduction = std::min(coins, amount);
    coins = std::max(0, coins - amount);
}

int Player::getStars() const { return stars; }
void Player::addStars(int amount) {
    stars += amount;
}

int Player::getPosition() const { return position; }
void Player::setPosition(int newPosition) {
    int oldPosition = position;
    position = newPosition;
    std::cout << "[MOVE] " << getName() << "從格子 " << oldPosition << " 移動到格子 " << position << std::endl;
}

int Player::getSpeedMultiplier() const { return speedMultiplier; }

// setter
void Player::setSpeedMultiplier(int multiplier) { speedMultiplier = multiplier; }

// 道具功能
void Player::addItem(const Item& item) {
    inventory.push_back(item);
    std::cout << "[ITEM] " << getName() << " 獲得道具: " << item.getName() << std::endl;
}

void Player::useItem(int index) {
    try {
        if (index < 0 || index >= inventory.size()) {
            throw std::out_of_range("[ERROR] 無效的道具索引！");
        }
        const Item& selectedItem = inventory[index];
        selectedItem.applyEffect(*this);
        inventory.erase(inventory.begin() + index);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Player::displayInventory() const {
    std::cout << "[ITEM] " << getName() << "的道具:" << std::endl;
    if (inventory.empty()) {
        std::cout << " - 沒有可用道具。" << std::endl;
    } else {
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i + 1 << ". " << inventory[i].getName() << " - " << inventory[i].getDescription() << std::endl;
        }
    }
}
const std::vector<Item>& Player::getInventory() const {
    return inventory;
}

// 狀態功能
void Player::move(int steps, int boardSize) {
    int oldPosition = position;
    position = (position + steps + boardSize) % boardSize; // 環狀地圖處理
    std::cout << "[MOVE] " << getName() << " 從格子 " << oldPosition
              << " 移動到了格子 " << position << " (" 
              << (steps > 0 ? "+" : "") << steps << " 格)" << std::endl;
}

// 速度功能
void Player::doubleSpeed() {
    speedMultiplier = 2;
    std::cout << "[INFO] " << getName() << "速度加倍!\n";
}

void Player::tripleSpeed() {
    speedMultiplier = 3;
    std::cout << "[INFO] " << getName() << "速度三倍!急速向前衝！\n";
}

void Player::resetSpeed() {
    speedMultiplier = 1;
    //std::cout << "[INFO] " << character.getName() << "以正常速度行走。\n";
}

// 特殊骰子功能
void Player::setSpecialDice(const std::vector<int>& faces) {
    specialDice = faces;
    std::cout << "[ITEM] " << getName() << " 獲得了特殊骰子！\n";
    useSpecialDice = true;

}

bool Player::hasSpecialDice() const {
    return useSpecialDice && !specialDice.empty();
}

int Player::rollSpecialDice() {
    if (specialDice.empty()) {
        std::cout << "[ITEM] 沒有特殊骰子，使用角色專屬骰子\n";
        return rollDice(); // 使用普通骰子
    }

    // 隨機擲特殊骰子
    int index = std::rand() % specialDice.size();
    std::cout << "[ITEM] " << getName() << " 使用 " << specialDice[index] << " 特殊骰子。\n";
    useSpecialDice = false;
    return specialDice[index];
}

// 與 Tile 整合
void Player::applyTileEffect(Tile& tile, int boardSize) {
    std::cout << "[MOVE] " << getName() << " 移動到了 " << tile.getId() << "格" << std::endl;
    tile.applyEffect(*this, boardSize);
}

// 購買星星
bool Player::purchaseStar(int starCost) {
    if (coins >= starCost) {
        reduceCoins(starCost);
        addStars(1);
        std::cout << "[SUCCESS] " << getName() << " 使用 " << starCost
                  << " 金幣購買無敵星星 " << std::endl;
        return true;
    }
    std::cout << "[ERROR] " << getName() << "的錢不夠啦！沒辦法購買無敵星星。" << std::endl;
    return false;
}

// Operator overload，比較時按星星數量優先，金幣數量次之
bool Player::operator>(const Player& other) const {
    if (stars > other.stars) return true;
    if (stars == other.stars && coins > other.coins) return true;
    return false;
}