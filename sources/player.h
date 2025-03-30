#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "character.h"
#include "item.h" // 必須包含，因為 Item 是成員變數

class Tile; // 前向宣告，避免不必要的依賴

class Player : public Character {
private:
    std::string playerName;
    int id;
    int coins;                    // 金幣數量
    int stars;                    // 星星數量
    int position;                 // 地圖位置
    bool extraTurn;               // 是否有額外回合
    bool skipTurn;                // 是否跳過回合
    int speedMultiplier;          // 當前速度倍率 (1 表示正常)
    bool useSpecialDice = false;  // 預設不會使用特殊骰子
    std::vector<int> specialDice; // 特殊骰子面數
    std::vector<Item> inventory;  // 道具庫存

public:
    // Constructor
    Player(const std::string& playerName, const std::string& characterName, const Dice& dice);
    // 基本屬性
    const std::string& getPlayerName() const;
    int getId() const;
    int getCoins() const;
    void addCoins(int amount);
    void reduceCoins(int amount);
    int getStars() const;
    void addStars(int amount);
    int getPosition() const;
    void setPosition(int newPosition);

    // Speed multiplier
    int getSpeedMultiplier() const;
    void setSpeedMultiplier(int multiplier);

    // 道具功能
    void addItem(const Item& item);
    void useItem(int index);
    void displayInventory() const;
    const std::vector<Item>& getInventory() const;

    // 狀態功能
    void move(int steps, int boardSize); // 接收地圖大小作為參數

    // 速度與骰子
    void doubleSpeed();                // 設置移動速度翻倍
    void tripleSpeed();                // 設置移動速度三倍
    void resetSpeed();                 // 重置移動速度
    void setSpecialDice(const std::vector<int>& faces); // 設置特殊骰子
    bool hasSpecialDice() const; // 檢查有沒有特殊骰子
    int rollSpecialDice() ;      // 擲特殊骰子

    // 與 Tile 整合
    void applyTileEffect(Tile& tile, int boardSize);

    // 購買星星
    bool purchaseStar(int starCost);

    // Operator overload:比較運算子（按星星數量和金幣數量比較）
    bool operator>(const Player& other) const;

};

#endif // PLAYER_H
