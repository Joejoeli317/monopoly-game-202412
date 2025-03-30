#include "itemStore.h"
#include "game.h" // 确保访问 Game 的道具列表
#include <iostream>
using namespace std;

// Constructor：初始化商店內的道具
ItemStore::ItemStore() {
    // 使用全局或预定义道具初始化
    items = {};
}

ItemStore::ItemStore(const std::vector<Item>& gameItems) {
    items = gameItems; // 从 Game 中获取所有道具
}

// display items in item store
void ItemStore::displayItems() const
{
    cout << "歡迎來到道具商店！以下是可供購買的道具：" << endl;
    for (size_t i = 0; i < items.size(); ++i) {
        cout << i + 1 << ". " << items[i].getName()
             << " - 價格：" << items[i].getPrice()
             << " - 描述：" << items[i].getDescription() << endl;
    }
}

// 處理購買邏輯
void ItemStore::purchaseItem(Player& player) {
    int choice;
    while (true) {
        cout << "請輸入要購買的道具編號，若不購買道具則輸入 0 跳過：";
        if (!(cin >> choice)) {
            cin.clear(); // 清除錯誤 cin 狀態
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "無效的輸入，請輸入一個數字。" << endl;
            continue;
        }

        if (choice == 0) {
            cout << "玩家選擇跳過購買。" << endl;
            return;
        }

        if (choice < 1 || choice > items.size()) {
            cout << "無效的選擇，請重新操作。" << endl;
            continue;
        }

        Item selectedItem = items[choice - 1];

        // 檢查金幣是否足夠
        if (player.getCoins() >= selectedItem.getPrice()) {
            player.reduceCoins(selectedItem.getPrice());
            // 改成將道具加入玩家背包，不會直接用
            player.addItem(selectedItem); 
            cout << "您購買了：" << selectedItem.getName() << "！剩餘金幣：" << player.getCoins() << endl;
            return;
        } else {
            cout << "金幣不足，無法購買該道具。" << endl;
        }
    }
}

// 玩家到達itemStoreBlock, trigger item store
void ItemStore::triggerStore(Player& player)
{
    cout << "[COIN] " << "您目前擁有的金幣數量：" << player.getCoins() << endl;
    displayItems();
    purchaseItem(player);
}
