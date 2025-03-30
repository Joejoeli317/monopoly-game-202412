#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "characterDice.h"
using namespace std;

// 定義角色名稱與其對應的專屬骰子點數組合，每個角色的骰子點數總和固定為 21
std::map<std::string, std::vector<int>> characterDice = {
    {"HarryPotter", {1, 2, 3, 4, 5, 6}},
    {"SpongeBob", {0, 0, 4, 5, 6, 6}},
    {"Doraemon", {2, 2, 3, 4, 5, 5}},
    {"Luya", {1, 3, 4, 4, 5, 4}},
    {"Chiikawa", {1, 1, 3, 6, 5, 5}},
    {"CrayonSmallNew", {0, 1, 4, 5, 6, 5}}
};

// Constructor
Game::Game(int numPlayers, int numTiles, int numTurns, int starPrice)
    : numTurns(numTurns), starPrice(starPrice), display(145, 40, 9, 8, 8, 4, 13, 3, 7) {
    srand(time(0)); // 隨機數初始化
    initializePlayers();
    initializeBoard(numTiles);
}

// 初始化玩家
void Game::initializePlayers() {
    cout << "\033[7m歡迎進入歡樂大富翁遊戲！\033[0m" << endl;
    
    cout << "請輸入玩家人數(2-4皆可）" << endl;
    int playerNum;
    while (true) {
        cin >> playerNum;
        if (cin.fail() || playerNum <=1) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[ERROR] 請輸入大於1的玩家人數！" << endl;
        } else {
            break;
        }
    }
    
    cout<< "遊戲規則：遊戲開始前請玩家輸入名稱，並選擇想玩的角色。遊戲總共有 6 個角色可供選擇，每個角色都擁有初始金幣 50 個，以及角色的專屬骰子，專屬骰子可能為常見的（1,2,3,4,5,6)點，也可能是（1,1,3,4,6,6)點，不變的是，骰子的總點數將會是 21 點。選擇角色後，我們將會告訴玩家對應角色的專屬骰子點數組合。" << endl;
    
    cout << "在遊戲中你可能會遇到以下幾種地圖格子。" << endl;
    cout << "1. 金幣獎勵：獎勵玩家 5 金幣" << endl;
    cout << "2. 金幣懲罰：沒收玩家 3 金幣" << endl;
    cout << "3. 道具商店：玩家可進入道具商店購買特殊道具" << endl;
    cout << "4. 移動獎勵：玩家向前移動 2 格" << endl;
    cout << "5. 移動懲罰：玩家向後倒退 2 格" << endl;
    
    cout << "獲勝條件：超級星星將會隨機出現在地圖中，只要玩家走到超級星星附近的 3 格內，就可以使用金幣購買超級星星!最先贏得 3 顆超級星星的玩家就是勝利者！" << endl;
    cout << "現在就讓我們開始遊戲吧！請開始輸入玩家名稱。" << endl;
    
    
    for (int i = 0; i < playerNum; ++i) { // 可選擇玩家人數
        string playerName;
        cout << "玩家 " << i + 1 << " 名稱：";
        cin >> playerName;

        cout << "可選角色：" << endl;
        for (const auto& character : characterDice) {
            cout << "- " << character.first << endl;
        }

        string chosenCharacter;
        while (true) {
            cout << "請選擇角色：";
            cin >> chosenCharacter;

            if (characterDice.find(chosenCharacter) != characterDice.end()) {
                break;
            } else {
                cout << "角色名稱無效，請重新選擇！" << endl;
            }
        }

        // 創建角色和骰子
        Dice dice(characterDice[chosenCharacter]);
        Player newPlayer(playerName, chosenCharacter, dice);


        // 初始化玩家的初始金幣
        newPlayer.addCoins(50); // 初始 50 金幣

        players.emplace_back(newPlayer);

        // 顯示玩家的角色與初始金幣
        cout << "[INFO] 玩家 " << playerName << " 選擇了角色 " << chosenCharacter
             << "，並擁有初始金幣 50。" << endl;
        cout << "[DICE] 專屬骰子：[ ";
        for (int face : characterDice[chosenCharacter]) {
            cout << face << " ";
        }
        cout << "]" << endl;
    }
}

// 初始化地圖
void Game::initializeBoard(int numTiles) {
    registerItems(); // 注册道具

    int cols = display.getCols();          // 每行格子數
    int rows = display.getRows();          // 每列格子數
    int currentId = 0;
    
    // 上邊
    for (int col = 0; col < cols; ++col) {
        if (currentId % 7 == 0) {
            board.emplace_back(currentId, 0, col, new ItemStore(items));
        } else {
            board.emplace_back(currentId, 0, col);
        }
        ++currentId;
    }

    // 右邊
    for (int row = 1; row < rows - 1; ++row) {
        if (currentId % 7 == 0) {
            board.emplace_back(currentId, row, cols - 1, new ItemStore(items));
        } else {
            board.emplace_back(currentId, row, cols - 1);
        }
        ++currentId;
    }

    // 下邊
    for (int col = cols - 1; col >= 0; --col) {
        if (currentId % 7 == 0) {
            board.emplace_back(currentId, rows - 1, col, new ItemStore(items));
        } else {
            board.emplace_back(currentId, rows - 1, col);
        }
        ++currentId;
    }

    // 左邊
    for (int row = rows - 2; row > 0; --row) {
        if (currentId % 7 == 0) {
            board.emplace_back(currentId, row, 0, new ItemStore(items));
        } else {
            board.emplace_back(currentId, row, 0);
        }
        ++currentId;
    }
    
    // 隨機選擇初始星星位置（避免選擇商店格子）
    do {
        starPosition = rand() % numTiles;
    } while (starPosition % 7 == 0); // 確保星星不在商店格子
}

// 一個玩家的回合
void Game::playTurn(Player& player) {
    cout << "\n--- " << player.getName() << " 的回合 ---\n";

    // 在投骰子前詢問是否使用道具
    if (!player.getInventory().empty()) {
        char useItemChoice;
        cout << "是否使用道具？(y/n): ";
        cin >> useItemChoice;
        if (useItemChoice == 'y' || useItemChoice == 'Y') {
            player.displayInventory();
            int itemChoice;
            cout << "選擇道具編號：";
            cin >> itemChoice;
            player.useItem(itemChoice - 1);
        }
    }

    // 等待玩家按下 Enter 開始投骰子
    waitForEnter();

    // 定義 diceRoll 變量
    int diceRoll;
    
    
    // 投擲骰子
    if (player.hasSpecialDice()) { // 检查是否有特殊骰子
        diceRoll = player.rollSpecialDice(); // 使用特殊骰子
        cout << "[DICE] " << player.getName() << " 使用了特殊骰子，擲出了 " << diceRoll << " 點數！" << endl;
    } else {
        diceRoll = player.rollDice(); // 使用普通骰子
        cout << "[DICE] " << player.getName() << " 擲出了 " << diceRoll << " 點數！" << endl;
    }
    
    // 繪製骰子顯示擲出的點數
    display.drawDice(diceRoll);

    // 計算移動步數
    int steps = diceRoll * player.getSpeedMultiplier();
    //cout << player.getName() << " 移動了 " << steps << " 格。" << endl;
    player.move(steps, board.size());

    // 檢查是否在星星格附近（距離 <= 3）-->就可以買星星了
    checkStarProximity(player);

    // 觸發當前格子的效果
    board[player.getPosition()].applyEffect(player, board.size());

    // 重置速度
    player.resetSpeed();
    
    // 更新玩家資訊欄
    display.updatePlayerInfo(player, static_cast<size_t>(&player - &players[0]));
    
    // 更新地圖顯示
    display.updateMap(players, board, starPosition);
    
    waitForEnterPlayer();
}

void Game::updateStarPosition() {
    int newStarPosition;
    do {
        newStarPosition = rand() % board.size(); // 隨機選擇一個新位置
    } while (newStarPosition == starPosition);  // 確保新位置不同於當前位置

    starPosition = newStarPosition; // 更新星星位置
    cout << "\033[7m超級星星已移動至新位置：" << starPosition << " 格。\033[0m" << endl;
}

// 宣布贏家
void Game::declareWinner() const {
    const Player* winner = nullptr;

    for (const auto& player : players) {
        if (!winner || player > *winner) {
            winner = &player;
        }
    }

    cout << "\n遊戲結束！\n";
    if (winner) {
        cout << "恭喜 " << winner->getName() << " 獲勝！" << endl;
        cout << "星星數量：" << winner->getStars() << "，金幣數量：" << winner->getCoins() << endl;
    } else {
        cout << "無玩家獲勝！" << endl;
    }
    
    waitForEnterEnd();
}

// 執行整個遊戲
void Game::playGame() {
    
    display.drawBackgroundWindow();
    display.drawStatusBoxes();
    display.displayPlayerInfo(players); // 初始顯示所有玩家資訊
    display.displayTileType();
    display.drawMap(board);

    for (int turn = 1; turn <= numTurns; ++turn) {
        cout << "\n=== 第 " << turn << " 回合 ===\n";
        for (auto& player : players) {
            playTurn(player);

            // 檢查是否有玩家達到 3 顆星星
            if (player.getStars() >= 3) {
                cout << player.getName() << " 已經收集了 3 顆超級星星！遊戲提前結束！" << endl;
                declareWinner();
                return; // 結束程式
            }
        }
    }

    // 如果到達最大回合數後仍無人達成條件，正常結束遊戲
    declareWinner();
}

// 註冊各種道具的價格與對應玩家效果
void Game::registerItems() {
    items = {
        Item("加速器", 10, "移動速度翻倍", [](Player& player) {
            player.doubleSpeed();
        }),
        Item("加速器", 15, "移動速度 3 倍", [](Player& player) {
            player.tripleSpeed();
        }),
        Item("地下水管", 25, "讓玩家直接移動到指定的格子", [this](Player& player) {
            int targetPosition;
            std::cout << "請輸入目標格子位置：";
            std::cin >> targetPosition;
            player.setPosition(targetPosition);
            std::cout << player.getName() << " 使用地下水管，直接移動到格子：" << targetPosition << std::endl;

            // 檢查是否在星星格附近
            checkStarProximity(player);

            // 觸發當前格子的效果
            board[player.getPosition()].applyEffect(player, board.size());
        }),
        Item("特殊骰子", 20, "使用一次性特殊骰子", [](Player& player) {
            int choice;
            std::cout << "請選擇特殊骰子類型：" << std::endl;
            std::cout << "1. 111111\n2. 222222\n3. 333333\n4. 444444\n5. 555555\n6. 666666" << std::endl;
            std::cin >> choice;
            if (choice == 1) {
                player.setSpecialDice({1, 1, 1, 1, 1, 1});
            } else if (choice == 2) {
                player.setSpecialDice({2, 2, 2, 2, 2, 2});
            } else if (choice == 3) {
                player.setSpecialDice({3, 3, 3, 3, 3, 3});
            }else if (choice == 4) {
                player.setSpecialDice({4, 4, 4, 4, 4, 4});
            }else if (choice == 5) {
                player.setSpecialDice({5, 5, 5, 5, 5, 5});
            }else if (choice == 6) {
                player.setSpecialDice({6, 6, 6, 6, 6, 6});
            }else {
                std::cout << "選擇無效，取消使用。" << std::endl;
            }
        }),
        Item("交換位置", 25, "選擇一名玩家交換位置", [this](Player& currentPlayer) {
            Player& targetPlayer = chooseTargetPlayer(currentPlayer);
            swapPositionEffect(currentPlayer, targetPlayer);
            // 檢查是否在星星附近
            checkStarProximity(currentPlayer);
            // 觸發當前格子的效果
            board[currentPlayer.getPosition()].applyEffect(currentPlayer, board.size());
            // 檢查是否在星星附近
            checkStarProximity(targetPlayer);
            // 觸發當前格子的效果
            board[targetPlayer.getPosition()].applyEffect(targetPlayer, board.size());
        }),
        Item("偷取金幣", 27, "搶奪指定玩家一半的金幣", [this](Player& currentPlayer) {
            Player& targetPlayer = chooseTargetPlayer(currentPlayer);
            stealCoinsEffect(currentPlayer, targetPlayer);
        })
    };
}

// 選擇要偷錢或交換位置的玩家
Player& Game::chooseTargetPlayer(const Player& currentPlayer) {
    std::cout << "可選擇的目標玩家：" << std::endl;

    for (size_t i = 0; i < players.size(); ++i) {
        if (&players[i] != &currentPlayer) {
            std::cout << i + 1 << ". " << players[i].getName() << std::endl;
        }
    }

    int choice;
    while (true) {
        std::cout << "請選擇目標玩家(輸入玩家編號）：";
        if (std::cin >> choice && choice >= 1 && choice <= players.size() && &players[choice - 1] != &currentPlayer) {
            return players[choice - 1]; // 返回目標玩家
        } else {
            std::cin.clear(); // 清除錯誤狀態
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 丟棄緩衝區的無效數據
            std::cout << "無效輸入，請重新選擇（打數字即可）。" << std::endl;
        }
    }
}

// 交換位置效果需寫在game中，以接收當前玩家與目標玩家做為參數
void Game::swapPositionEffect(Player& currentPlayer, Player& targetPlayer) {
    int tempPosition = currentPlayer.getPosition();
    currentPlayer.setPosition(targetPlayer.getPosition());
    targetPlayer.setPosition(tempPosition);

    cout << "[INFO] " << currentPlayer.getName() << " 與 " << targetPlayer.getName()
         << " 交換了位置！" << endl;
}

// 偷錢效果需寫在game中，以接收當前玩家與目標玩家做為參數
void Game::stealCoinsEffect(Player& currentPlayer, Player& targetPlayer) {
    int stolenAmount = targetPlayer.getCoins() / 2;
    targetPlayer.reduceCoins(stolenAmount);
    currentPlayer.addCoins(stolenAmount);

    std::cout <<"[INFO] " << currentPlayer.getName() << " 搶奪了 " << targetPlayer.getName()
              << " 的一半金幣，共計：" << stolenAmount << " 枚金幣。" << std::endl;
}

// 檢查星星是否在附近（可購買的距離內）
void Game::checkStarProximity(Player& player) {
    int distance = abs(player.getPosition() - starPosition);
    if (distance > board.size() / 2) { // 環狀地圖需要考慮邊界跨越
        distance = board.size() - distance;
    }

    if (distance <= 3) {
        std::cout << "[STAR] " << player.getName() << " 距離超級星星 " << distance << " 格，有機會購買星星！" << std::endl;

        // 嘗試購買星星（錢夠就會買）
        if (player.purchaseStar(starPrice)) {
            updateStarPosition(); // 購買成功後，更新星星位置
        }
    }
}

// 每回合輪到每位玩家時，清空緩衝區並等待玩家按下 Enter
void Game::waitForEnter() {
    if (std::cin.peek() == '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "[提示] 按 Enter 鍵開始擲骰子..." << std::endl;
    std::cin.get();
}

// 清空緩衝區並等待玩家按下 Enter
void Game::waitForEnterPlayer() {
    if (std::cin.peek() == '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "[提示] 按 Enter 鍵進入下一步..." << std::endl;
    std::cin.get();
    display.clearInfoPanel();
}

// 結束遊戲時清空緩衝區並等待玩家按下 Enter
void Game::waitForEnterEnd() const{
    if (std::cin.peek() == '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "[提示] 按 Enter 鍵結束遊戲..." << std::endl;
    std::cin.get();
}
