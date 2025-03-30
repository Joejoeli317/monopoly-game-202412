#ifndef item_h
#define item_h

#include <string>
#include <vector>
#include <functional>
using namespace std;

class Player;

// 定義個別道具屬性：名稱、價格、描述、效果

using SingleEffectFunction = function<void(Player&)>;
using DoubleEffectFunction = function<void(Player&, Player&)>;

class Item
{
private:
    string name;
    int price;
    string description;
    SingleEffectFunction singleEffect;  // 單一玩家效果
    DoubleEffectFunction doubleEffect;  // 雙玩家效果
    
    
public:
    // constructor
    Item(const string& name, int price, const string& description, SingleEffectFunction effect);
    Item(const string& name, int price, const string& description, DoubleEffectFunction effect);
    
    //getter
    const string& getName() const;
    int getPrice() const;
    const string& getDescription() const;
    
    //執行效果
    void applyEffect(Player& player) const;
    void applyEffect(Player& currentPlayer, Player& nextPlayer) const;
};


#endif /* item_h */
