#ifndef Header_h
#define Header_h

#include <string>
#include <vector>
#include "player.h" // 引用player
#include "item.h"
using namespace std;

// 管理多個item
// 顯示道具列表、處理玩家購買道具操作
// 設定是由block觸發itemStore
// 玩家到達itemStoreBlock時，main function 呼叫 triggerStore function
class ItemStore
{
private:
    vector<Item> items; // 商店內的道具
    
public:
    // constructor
    ItemStore(); // 默认构造函数
    explicit ItemStore(const std::vector<Item>& gameItems);
    
    // 顯示所有可購買的道具
    void displayItems() const;
    
    // 玩家購買道具
    void purchaseItem(Player& player);
    
    // block觸發商店
    void triggerStore(Player& player);
};

#endif /* Header_h */
