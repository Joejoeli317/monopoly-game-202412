#ifndef gameDisplay_hpp
#define gameDisplay_hpp

#include <vector>
#include <string>
#include <map>
#include "tile.h"
#include "player.h"

class GameDisplay{
public:
    // constructor: 初始化遊戲畫面
    GameDisplay(int termWidth, int termHeight, int cols, int rows, int cellWidth, int cellHeight, int boxWidth, int boxHeight, int infoGap);
    
    void saveCursor();                        // 保存當前游標位置
    void restoreCursor();                     // 恢復游標位置
    
    // 背景畫面
    void drawBackgroundWindow();
    
    // 繪製地圖
    void drawMap(const std::vector<Tile>& board);
    
    // 更新地圖上的玩家和星星位置
    void updateMap(const std::vector<Player>& players, const std::vector<Tile>& board, int starPosition);
    
    // 骰子
    void drawStatusBoxes();
    void drawDice(int number);
    
    // 清空回合資訊欄
    void clearInfoPanel();
    
    // 玩家資訊欄
    void displayPlayerInfo(const vector<Player>& players);
    void updatePlayerInfo(const Player& player, size_t playerIndex);
    
    // 格子類型說明
    void displayTileType();
    
    // getter
    int getCols() const;
    int getRows() const;
    int getCellWidth() const;
    int getCellHeight() const;
    int getStartMapX() const;
    int getStarMapY() const;
    
private:
    int termWidth;
    int termHeight;
    int cols;
    int rows;
    int cellWidth;
    int cellHeight;
    int infoPanelWidth;
    int boxWidth;
    int boxHeight; // 記得更改constructor 12, 3
    int infoGap;
    
    int startX;
    int startY;
    int startMapX;
    int startMapY;
    int statusX;
    int statusY;
    int infoX;
    int infoY;
    
    void drawCell(int x, int y, int width, int height, int type);
    void drawCell(int x, int y, int width, int height, const std::string &content);
    
    // tool function
    void gotoxy(int x, int y);                // 游標
    void switchToMapArea();                   // 將游標移至地圖區域
    void switchToInfoPanel();                 // 將游標移至資訊欄起始位置
    void switchToTileContent(int x, int y,
                             int cellWidth,
                             int cellHeight); // 游標移至地圖內每格內容輸出起始位置
    void clearScreen();                       // 清屏
};

#endif /* gameDisplay_hpp */
