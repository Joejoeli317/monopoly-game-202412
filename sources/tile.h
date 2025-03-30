#ifndef TILE_H
#define TILE_H

#include <string>
#include "itemStore.h"

class Player; // 前向宣告 Player 類別，避免不必要的依賴

class Tile {
public:
    // 格子類型常量
    static const int REWARD_COIN = 0;
    static const int REWARD_STORE = 1;
    static const int REWARD_MOVE = 2;
    static const int PENALTY_BACK = 3;
    static const int PENALTY_COIN = 4;

    // Constructor
    Tile(int id, int row, int col, ItemStore* store = nullptr);

    // 應用格子效果
    void applyEffect(Player& player, int boardSize);

    // 根據 ID 確定格子類型
    static int determineType(int id);

    // getter
    int getId() const;
    int getType() const;
    int getRow() const;
    int getCol() const;

private:
    int id;            // 格子編號
    int row;
    int col;
    int type;          // 格子類型
    ItemStore* itemStore; // 指向商店的指針（可為空）
};

#endif // TILE_H
