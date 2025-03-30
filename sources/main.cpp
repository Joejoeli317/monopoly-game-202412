#include "game.h"
#include <vector>
#include <iostream>

std::vector<Player> players;

int main() {
    // 初始化遊戲：4 位玩家，30 格地圖，20 回合，每顆星星價格 15 金幣
    Game game(3, 30, 20, 15);
    game.playGame(); // 執行遊戲
    
    // 清空終端
    std::cout << "\033[2J\033[H";
    
    return 0;
}
