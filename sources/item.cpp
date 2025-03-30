#include <iostream>
#include <vector>
#include "item.h"
#include "player.h"
using namespace std;

extern vector<Player> players; // extern應該會在main.cpp中的玩家列表（？）

// 效果函數
// 以下含player的member function: doubleSpeed, tripleSpeed, setPosition, setSpecialDice, setExtraTurn, getCoins, reduceCoins, addCoins

void speedDoubleEffect(Player& player)
{
    cout << "[ITEM] " << player.getName() << " 使用 2 倍加速器，移動速度翻倍！" << endl;
    player.doubleSpeed();
}

void speedTripleEffect(Player& player)
{
    cout << "[ITEM] " << player.getName() << " 使用 3 倍加速器，急速向前衝！" << endl;
    player.tripleSpeed();
}

void pipeEffect(Player& player, int targetPosition) 
{
    cout << "[ITEM] " << player.getName() << " 使用了地下水管，直接移動到格子：" << targetPosition << endl;
    player.setPosition(targetPosition); // 假設 Player 有 setPosition 方法
}

void specialDiceEffect(Player& player, const vector<int>& diceFaces) 
{
    cout << "[ITEM] " << player.getName() << " 使用了特殊骰子，此局將擲特殊骰子！：" << diceFaces[0] << endl;
    player.setSpecialDice(diceFaces); 
}

// class Item
// 單一玩家效果的Constructor
Item::Item(const string& name, int price, const string& description, SingleEffectFunction effect)
    : name(name), price(price), description(description), singleEffect(effect), doubleEffect(nullptr) {}

// 雙玩家效果的Constructor
Item::Item(const string& name, int price, const string& description, DoubleEffectFunction effect)
    : name(name), price(price), description(description), singleEffect(nullptr), doubleEffect(effect) {}

// getter
const string& Item::getName() const
{
    return name;
}

int Item::getPrice() const
{
    return price;
}

const string& Item::getDescription() const 
{
    return description;
}

// 單一玩家效果的執行
void Item::applyEffect(Player& player) const 
{
    if (singleEffect) 
    {
        singleEffect(player);
    }
}

// 雙玩家效果的執行
void Item::applyEffect(Player& currentPlayer, Player& nextPlayer) const 
{
    if (doubleEffect)
    {
        doubleEffect(currentPlayer, nextPlayer);
    }
}
