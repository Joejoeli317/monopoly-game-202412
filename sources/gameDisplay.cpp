#include "gameDisplay.hpp"
#include <iostream>
using namespace std;

GameDisplay::GameDisplay(int termWidth, int termHeight, int cols, int rows, int cellWidth, int cellHeight, int boxWidth, int boxHeight, int infoGap)
: termWidth(termWidth), termHeight(termHeight), cols(cols), rows(rows), cellWidth(cellWidth), cellHeight(cellHeight), infoPanelWidth(65), boxWidth(boxWidth), boxHeight(boxHeight), infoGap(infoGap){
    // 設定各種起始位置
    startX = 1;
    startY = 1;
    startMapX = startX + infoPanelWidth + 4;
    startMapY = startY + 3;
    statusX = startMapX + 3 * cellWidth + 4;
    statusY = startMapY + 2 * cellHeight + 1;
    infoX = startMapX + cols * cellWidth + 10;
    infoY = startY + 1;
}

// =============== getter ===============
int GameDisplay::getCols() const {return cols;}
int GameDisplay::getRows() const {return rows;}
int GameDisplay::getCellWidth() const {return cellWidth;}
int GameDisplay::getCellHeight() const {return cellHeight;}
int GameDisplay::getStartMapX() const {return startMapX;}
int GameDisplay::getStarMapY() const {return startMapY;}

// ============== tool function ==============
// 保存游標位置
void GameDisplay::saveCursor() {
    cout << "\033[s";
}
// 恢復游標位置
void GameDisplay::restoreCursor() {
    cout << "\033[u";
}
// 移動終端游標
void GameDisplay::gotoxy(int x, int y)
{
    cout << "\033[" << y << ";" << x << "H";
}
// 游標移至資訊欄起始位置
void GameDisplay::switchToInfoPanel()
{
    gotoxy(startX, startY + 1);
}
// 游標移至地圖區域起始位置
void GameDisplay::switchToMapArea()
{
    gotoxy(startMapX, startMapY);
}
// 游標移至地圖內每格內容輸出起始位置
void GameDisplay::switchToTileContent(int x, int y, int cellWidth, int cellHeight)
{
    gotoxy((x + cellWidth / 2) - 3, (y + cellHeight / 2)); // 內容確切位置可再視美觀程度微調
}
// 清空整個畫面並移動游標到左上角, 遊戲結束時使用
void GameDisplay::clearScreen()
{
    cout << "\033[2J\033[H";
}
// 清空資訊欄
void GameDisplay::clearInfoPanel()
{
    for (int i = 1; i < termHeight - 1; ++i)
    {
        gotoxy(startX, startY + i);
        cout << string(infoPanelWidth - 2, ' '); // 用空格清空每一行
    }
    switchToInfoPanel();
}

// ============== 背景窗口 ==============
void GameDisplay::drawBackgroundWindow()
{
    // 地圖的寬度
    int mapWidth = termWidth - infoPanelWidth;
    
    // 繪製上邊框
    clearScreen();
    gotoxy(startX, startY);
    cout << "╔";
    for (int i = 0; i < termWidth - 1; i++) cout << "═";
    cout << "╗";

    // 繪製左右邊框
    for (int i = 1; i < termHeight - 1; i++)
    {
        gotoxy(startX + infoPanelWidth - 1, startY + i);
        cout << "║";
        gotoxy(startX + termWidth, startY + i);
        cout << "║";
    }

    // 繪製下邊框
    gotoxy(startX, startY + termHeight - 1);
    cout << "╚";
    for (int i = 0; i < termWidth - 1; i++) cout << "═";
    cout << "╝";
    
    // 繪製完成後，切換到資訊欄輸出處
    switchToInfoPanel();
}

// =============== 地圖繪製 ===============
// 繪製地圖單格矩形
void GameDisplay::drawCell(int x, int y, int width, int height, const int type)
{
    string colorCode;
    
    // 根據格子類型選擇顏色
    if (type == 1) {
        colorCode = "\033[33m";  // 黃色
    } else if (type == 0) {
        colorCode = "\033[36m";  // 青色
    } else if (type == 4) {
        colorCode = "\033[31m";  // 紅色
    } else if (type == 2) {
        colorCode = "\033[36m";  // 青色
    } else if (type == 3) {
        colorCode = "\033[31m";  // 紅色
    } else {
        colorCode = "\033[0m";   // 默認顏色
    }
    
    gotoxy(x, y);
    cout << colorCode << "┌";
    for (int i = 0; i < width - 2; i++) cout << "─";
    cout << "┐\033[0m";
    
    for (int i = 1; i < height - 1; i++)
    {
        gotoxy(x, y + i);
        cout << colorCode << "│";
        gotoxy(x + width - 1, y + i);
        cout << "│\033[0m";
    }
    
    gotoxy(x, y + height - 1);
    cout << colorCode << "└";
    for (int i = 0; i < width - 2; i++) cout << "─";
    cout << "┘\033[0m";
}

// 繪製狀態單格矩形
void GameDisplay::drawCell(int x, int y, int width, int height, const string &content)
{
    gotoxy(x, y);
    cout << "╔";
    for (int i = 0; i < width - 2; i++) cout << "═";
    cout << "╗";
    
    for (int i = 1; i < height - 1; i++)
    {
        gotoxy(x, y + i);
        cout << "║";
        gotoxy(x + width - 1, y + i);
        cout << "║";
    }
    
    gotoxy(x, y + height - 1);
    cout << "╚";
    for (int i = 0; i < width - 2; i++) cout << "═";
    cout << "╝";
    
    // 顯示格子內容
    gotoxy((x + cellWidth / 2) - 1, (y + cellHeight / 2) - 1);
    cout << content;
}

// 繪製多格子組成之地圖
void GameDisplay::drawMap(const std::vector<Tile>& board)
{
    int number = 0;
    
    // 上邊
    for (int i = 0; i < cols; i++)
    {
        int x = startMapX + i * cellWidth;
        int y = startMapY;
        drawCell(x, y, cellWidth, cellHeight, board[number].getType()); // 注意此處number和game中初始化地圖每格的currentId不同，此處從1開始
        
        // 在格子上方顯示 number
        gotoxy((x + cellWidth / 2) - 1, y - 1);
        cout << number++;
    }
    
    // 右邊
    for (int i = 1; i < rows - 1; i++)
    {
        int x = startMapX + (cols - 1) * cellWidth;
        int y = startMapY + i * cellHeight;
        drawCell(x, y, cellWidth, cellHeight, board[number].getType());
        
        // 在格子右側顯示 number
        gotoxy(x + cellWidth + 1, (y + cellHeight / 2) - 1);
        cout << number++;
    }
    
    // 下邊
    for (int i = cols - 1; i >= 0; i--)
    {
        int x = startMapX + i * cellWidth;
        int y = startMapY + (rows - 1) * cellHeight;
        drawCell(x, y, cellWidth, cellHeight, board[number].getType());
        
        // 在格子下方顯示 number
        gotoxy((x + cellWidth / 2) - 1, y + cellHeight);
        cout << number++;
    }

    // 左邊
    for (int i = rows - 2; i > 0; i--)
    {
        int x = startMapX;
        int y = startMapY + i * cellHeight;
        drawCell(x, y, cellWidth, cellHeight, board[number].getType());
        
        // 在格子左側顯示 number
        gotoxy(x - 3, (y + cellHeight / 2) - 1);
        cout << number++;
    }
    
    switchToInfoPanel();
}

// 更新地圖上的玩家和星星
void GameDisplay::updateMap(const vector<Player>& players, const std::vector<Tile>& board, int starPosition)
{
    // 切換到地圖區域
    switchToMapArea();
    
    // 清空所有格子內容
    for (const Tile& tile : board) {
        int x = startMapX + tile.getCol() * cellWidth;
        int y = startMapY + tile.getRow() * cellHeight;

        for (int dy = 1; dy < cellHeight - 1; ++dy) {
            gotoxy(x + 1, y + dy);
            cout << string(cellWidth - 2, ' '); // 清空格子內容
        }
    }
    
    // 記錄格子的玩家列表
    map<int, vector<string>> playersInTile;
    
    for (size_t i = 0; i < players.size(); ++i) {
    playersInTile[players[i].getPosition()].push_back("P" + to_string(i + 1));
    }
        
    // 遍歷地圖格子
    for (const Tile& tile : board)
    {
        int x = startMapX + tile.getCol() * cellWidth;  // 計算格子左上角 X
        int y = startMapY + tile.getRow() * cellHeight; // 計算格子左上角 Y

        // 檢查當前格子是否有星星和玩家
        bool hasStar = (tile.getId() == starPosition);
        bool hasPlayers = (playersInTile.find(tile.getId()) != playersInTile.end());

        // 根據不同情況顯示格子內容
        if (hasStar && hasPlayers) // 僅顯示玩家因玩家會當下馬上購買星星
        {
            switchToTileContent(x, y, cellWidth, cellHeight);
            string combinedPlayers;
            for (size_t i = 0; i < playersInTile[tile.getId()].size(); ++i) 
            {
                if (i < 2) 
                {
                    combinedPlayers += playersInTile[tile.getId()][i];
                } 
                else
                {
                    combinedPlayers += "..";
                    break;
                }
            }
            cout << combinedPlayers;
        }
        else if (hasStar) 
        {
            // 只有星星：黃色背景填上半部
            for (int dy = 1; dy < cellHeight - 2; ++dy)
            {
                gotoxy(x + 1, y + dy);
                cout << "\033[43m" << string(cellWidth - 2, ' ') << "\033[0m"; // 黃色背景
            }
        }
        else if (hasPlayers) 
        {
            // 只有玩家：下半部顯示玩家標記
            switchToTileContent(x, y, cellWidth, cellHeight);
            string combinedPlayers;
            for (size_t i = 0; i < playersInTile[tile.getId()].size(); ++i) 
            {
                if (i < 2) 
                {
                    combinedPlayers += playersInTile[tile.getId()][i];
                } 
                else
                {
                    combinedPlayers += "..";
                    break;
                }
            }
            cout << combinedPlayers;
        }
        else 
        {
            // 空格子：清空內容
            for (int dy = 1; dy < cellHeight - 1; ++dy) 
            {
                gotoxy(x + 1, y + dy);
                cout << string(cellWidth - 2, ' '); // 清空格子內容
            }
        }
    }
    
    // 更新地圖完成後，切換回資訊欄下端
    gotoxy(1, startY + termHeight - 4);
}

// =============== 投擲骰子呈現 ===============
// 繪製框格
void GameDisplay::drawStatusBoxes()
{
    int x = statusX;      // 每行兩個方框
    int y = statusY;      // 每兩個換行

    drawCell(x, y, boxWidth, boxHeight, "投擲骰子");
}

void GameDisplay::drawDice(int number)
{
    // 定義骰子的起始位置，位於 statusBox 下方
    int diceX = statusX;
    int diceY = statusY + boxHeight + 1;
    
    // 繪製骰子的邊框
    gotoxy(diceX, diceY);
    cout << "╔═══════════╗";
    gotoxy(diceX, diceY + 1);
    cout << "║           ║";
    gotoxy(diceX, diceY + 2);
    cout << "║           ║";
    gotoxy(diceX, diceY + 3);
    cout << "║           ║";
    gotoxy(diceX, diceY + 4);
    cout << "║           ║";
    gotoxy(diceX, diceY + 5);
    cout << "║           ║";
    gotoxy(diceX, diceY + 6);
    cout << "╚═══════════╝";
    
    switch (number) {
        case 1:
            gotoxy(diceX + 6, diceY + 3);
            cout << "◎";
            break;
        case 2:
            gotoxy(diceX + 4, diceY + 2);
            cout << "◎";
            gotoxy(diceX + 8, diceY + 4);
            cout << "◎";
            break;
        case 3:
            gotoxy(diceX + 3, diceY + 1);
            cout << "◎";
            gotoxy(diceX + 6, diceY + 3);
            cout << "◎";
            gotoxy(diceX + 9, diceY + 5);
            cout << "◎";
            break;
        case 4:
            gotoxy(diceX + 4, diceY + 2);
            cout << "◎";
            gotoxy(diceX + 8, diceY + 2);
            cout << "◎";
            gotoxy(diceX + 4, diceY + 4);
            cout << "◎";
            gotoxy(diceX + 8, diceY + 4);
            cout << "◎";
            break;
        case 5:
            gotoxy(diceX + 3, diceY + 1);
            cout << "◎";
            gotoxy(diceX + 9, diceY + 1);
            cout << "◎";
            gotoxy(diceX + 6, diceY + 3);
            cout << "◎";
            gotoxy(diceX + 3, diceY + 5);
            cout << "◎";
            gotoxy(diceX + 9, diceY + 5);
            cout << "◎";
            break;
        case 6:
            gotoxy(diceX + 4, diceY + 1);
            cout << "◎";
            gotoxy(diceX + 8, diceY + 1);
            cout << "◎";
            gotoxy(diceX + 4, diceY + 3);
            cout << "◎";
            gotoxy(diceX + 8, diceY + 3);
            cout << "◎";
            gotoxy(diceX + 4, diceY + 5);
            cout << "◎";
            gotoxy(diceX + 8, diceY + 5);
            cout << "◎";
            break;
    }
    gotoxy(1, startY + 18); // 回復位置再微調
}

// =============== 玩家資訊欄 ===============
void GameDisplay::displayPlayerInfo(const vector<Player>& players)
{
    int updateInfoY = infoY;
    // 逐個顯示玩家資訊
    for (size_t i = 0; i < players.size(); ++i) 
    {
        const Player& player = players[i];
        
        gotoxy(infoX, updateInfoY);
        cout << "P" << i + 1;
        
        gotoxy(infoX, updateInfoY + 1);
        cout << "玩家名稱: " << player.getPlayerName();
        
        gotoxy(infoX, updateInfoY + 2);
        cout << "角色名稱: " << player.getName();

        gotoxy(infoX, updateInfoY + 3);
        cout << "金幣數量: " << player.getCoins();

        gotoxy(infoX, updateInfoY + 4);
        cout << "星星數量: " << player.getStars();

        updateInfoY += infoGap;  // 每個玩家資訊間隔 4 行
    }
    
}

void GameDisplay::updatePlayerInfo(const Player& player, size_t playerIndex)
{
    int updateInfoY = infoY + playerIndex * infoGap;
    
    // 清空當前玩家的資訊區域
    for (int i = 0; i < infoGap; ++i) {
        gotoxy(infoX, updateInfoY);
        cout << string(20, ' ');  // 清空每一行（30個空格，可根據需要調整）
    }
    
    // 重新顯示單個玩家的資訊
    gotoxy(infoX, updateInfoY);
    cout << "P" << playerIndex + 1;

    gotoxy(infoX, updateInfoY + 1);
    cout << "玩家名稱: " << player.getPlayerName();

    gotoxy(infoX, updateInfoY + 2);
    cout << "角色名稱: " << player.getName();

    gotoxy(infoX, updateInfoY + 3);
    cout << "金幣數量: " << player.getCoins();

    gotoxy(infoX, updateInfoY + 4);
    cout << "星星數量: " << player.getStars();
}

// =============== 格子類型說明欄 ===============
void GameDisplay::displayTileType()
{
    // 定義格子說明的起始位置
    int tileTypeX = infoX;
    int tileTypeY = infoY + 4 * infoGap + 2; // 根據玩家資訊欄的最後位置向下偏移

    // 定義格子類型和顏色反白說明
    vector<string> tileTypes = {
        "\033[43m   \033[0m  道具商店格",
        "\033[46m   \033[0m  獎勵格",
        "\033[41m   \033[0m  懲罰格",
    };

    // 顯示每種格子的說明
    for (size_t i = 0; i < tileTypes.size(); ++i) {
        gotoxy(tileTypeX, tileTypeY + i * 2);
        cout << tileTypes[i];
    }
}
