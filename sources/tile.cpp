#include "tile.h"
#include "player.h" // 因為在 applyEffect 中調用了 Player 的方法
#include <iostream>

// Constructor
Tile::Tile(int id, int row, int col, ItemStore* store)
    : id(id), row(row), col(col), type(determineType(id)), itemStore(store) {}

int Tile::determineType(int id) {
    if (id % 8 == 0) return PENALTY_BACK;
    if (id % 7 == 0) return REWARD_STORE;
    if (id % 6 == 0) return REWARD_MOVE;
    if (id % 2 == 0) return PENALTY_COIN;
    return REWARD_COIN;
}

void Tile::applyEffect(Player& player, int boardSize) {
    if (type == REWARD_COIN) {
        std::cout << "[POSITION] 金幣獎勵格: " << player.getName() << " 得到了 5 金幣！\n";
        player.addCoins(5);
    } else if (type == REWARD_STORE) {
        std::cout << "[POSITION] 道具商店格: " << player.getName() << "進入道具商店\n";
        if (itemStore) {
            itemStore->triggerStore(player); // 觸發商店
        }
    } else if (type == REWARD_MOVE) {
        std::cout << "[POSITION] 移動獎勵格: " << player.getName() << "向前移動 2 格\n";
        player.move(2, boardSize);    // 正向移動，傳入地圖大小
    } else if (type == PENALTY_BACK) {
        std::cout << "[POSITION] 移動懲罰格: " << player.getName() << "向後倒退 2 格\n";
        player.move(-3, boardSize);   // 反向移動，傳入地圖大小
    } else if (type == PENALTY_COIN) {
        std::cout << "[POSITION] 金幣懲罰格: " << player.getName() << "被沒收 3 個金幣\n";
        player.reduceCoins(3);
    } else {
        std::cout << "Unknown tile type.\n";
    }
}

int Tile::getId() const {
    return id;
}

int Tile::getType() const {
    return type;
}

int Tile::getRow() const { return row; }
int Tile::getCol() const { return col; }
