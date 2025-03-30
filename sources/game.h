#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "player.h"
#include "tile.h"
#include "itemStore.h"
#include "gameDisplay.hpp"

class Game {
private:
    std::vector<Player> players;      // 玩家列表
    std::vector<Item> items; // 存储所有道具
    std::vector<Tile> board;          // 地圖格子
    ItemStore itemStore;              // 道具商店
    int numTurns;                     // 總回合數
    int starPrice;                    // 星星價格
    int starPosition;                 // 星星的位置
    
    GameDisplay display;              // 顯示相關操作交由 GameDisplay 處理

    void initializePlayers();         // 初始化玩家
    void initializeBoard(int numTiles); // 接受格子數量參數

    void registerItems(); // 新增注册道具的方法
    Player& chooseTargetPlayer(const Player& currentPlayer);
    void swapPositionEffect(Player& currentPlayer, Player& targetPlayer);
    void stealCoinsEffect(Player& currentPlayer, Player& targetPlayer);

    // 新增等待按下 Enter 的函數聲明
    void waitForEnter();
    void waitForEnterPlayer();
    void waitForEnterEnd()const;

    void playTurn(Player& player);    // 執行單個玩家的回合
    void updateStarPosition();         // 更新星星位置
    void checkStarProximity(Player& player);
    
public:
    Game(int numPlayers, int numTiles, int numTurns, int starPrice);
    void playGame();                  // 執行整個遊戲
    void declareWinner() const;       // 結算並宣布贏家
};

#endif // GAME_H
