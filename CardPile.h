#ifndef CARDPILE_H
#define CARDPILE_H

#include "Card.h"
#include <vector>
#include <random>
#include <iostream>

// 牌堆类：管理主牌库、两个弃牌堆，实现洗牌/抽牌/放牌核心逻辑
class CardPile {
private:
    std::vector<Card> mainPile;    // 主牌库（游戏核心牌库，洗牌后从这里抽牌）
    std::vector<Card> discardPile1;// 弃牌堆1
    std::vector<Card> discardPile2;// 弃牌堆2
    std::mt19937 rng;              // 随机数生成器（洗牌用，比rand更随机）

    // 私有：从指定牌堆末尾移除并返回卡牌
    Card popCardFrom(std::vector<Card>& pile);
    // 私有：向指定牌堆添加卡牌（末尾）
    void pushCardTo(std::vector<Card>& pile, const Card& card);

public:
    // 构造函数：初始化随机数生成器
    CardPile();

    // 牌库初始化：向主牌库添加所有卡牌（游戏开局调用）
    void initMainPile(const std::vector<Card>& allCards);
    // 洗牌：打乱主牌库顺序（游戏开局/暴风雨事件调用）
    void shuffleMainPile();
    // 洗牌指定弃牌堆
    void shuffleDiscardPile(int pileNo); // pileNo:1=弃牌堆1，2=弃牌堆2

    // 抽牌方法1：从主牌库抽N张牌
    std::vector<Card> drawFromMain(int n);
    // 抽牌方法2：从指定弃牌堆顶抽1张（游戏抽牌阶段的可选方式）
    Card drawFromDiscardTop(int pileNo);

    // 放牌方法1：向指定弃牌堆顶放1张牌（游戏抽牌阶段的弃牌操作）
    void putToDiscardTop(int pileNo, const Card& card);
    // 放牌方法2：向指定弃牌堆添加多张牌（效果触发/弃牌用）
    void putToDiscard(int pileNo, const std::vector<Card>& cards);
    // 放回主牌库底部（龙虾+螃蟹效果用）
    void putToMainBottom(const std::vector<Card>& cards);
    // 从主牌库移除一张指定牌（保险箱跨轮带牌时避免复制）
    bool removeFromMain(const Card& card);
    // 从主牌库中取出一张指定收集类型的牌（事件牌用）
    Card takeCollectFromMain(CollectSubType subType);

    // 牌库状态查询
    int getMainPileSize() const;    // 主牌库剩余卡牌数
    int getDiscardPileSize(int pileNo) const; // 指定弃牌堆卡牌数
    bool isMainPileEmpty() const;   // 主牌库是否为空
    bool isDiscardPileEmpty(int pileNo) const; // 指定弃牌堆是否为空

    // 控制台打印：打印指定牌堆的所有卡牌
    void printPile(int pileNo) const; // pileNo:0=主牌库，1=弃牌堆1，2=弃牌堆2

    // 获取弃牌堆1顶牌的名称（空堆返回"空"）
    std::string getDiscard1TopCardName() const {
        if (discardPile1.empty()) {
            return "空";
        }
        return discardPile1.back().getCardName();
    }

    // 获取弃牌堆2顶牌的名称（空堆返回"空"）
    std::string getDiscard2TopCardName() const {
        if (discardPile2.empty()) {
            return "空";
        }
        return discardPile2.back().getCardName();
    }

    int mainCount() const { return mainPile.size(); }
    int discard1Count() const { return discardPile1.size(); }
    int discard2Count() const { return discardPile2.size(); }

};

#endif // CARDPILE_H
