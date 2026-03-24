#ifndef CARD_H
#define CARD_H

#include <string>
#include <unordered_map>

// 卡牌主类型（核心分类）
enum class CardType {
    PAIR_CARD,    // 对牌（可成对打出，触发效果）
    COLLECT_CARD, // 收集牌（不可打出，凑数得分）
    BUFF_CARD     // 增益牌（不可打出，配合对牌得分）
};

// 对牌子类型（匹配文档中的对牌种类）
enum class PairSubType {
    FISH,       // 鱼牌(10)
    CRAB,       // 螃蟹牌(7)
    SHIP,       // 船牌(8)
    SWIMMER,    // 游泳者牌(8)
    SHARK,      // 鲨鱼牌(5)
    JELLYFISH,  // 水母牌(3)
    LOBSTER,    // 龙虾牌(1)
    TURTLE,     // 海龟牌(6)
    QUESTION    // 问号事件牌(6)
};

// 收集牌子类型（匹配文档中的收集牌种类）
enum class CollectSubType {
    SHELL,      // 贝壳牌(6)
    OCTOPUS,    // 章鱼牌(5)
    PENGUIN,    // 企鹅牌(3)
    ANCHOR,     // 船锚牌(2)
    SEAHORSE,   // 海马牌(1)
    MERMAID     // 美人鱼牌(4)（特殊收集牌，凑4张直接胜利）
};

// 增益牌子类型（匹配文档，每种各1张）
enum class BuffSubType {
    FISH_GROUP, // 鱼群牌
    CRAB_GROUP, // 螃蟹群牌
    SHIP_GROUP, // 船队牌
    PENGUIN_GROUP, // 企鹅群牌
    ANCHOR_PILE    // 船锚堆牌
};

// 事件牌子类型（问号对牌打出后抽取）
enum class EventSubType {
    MINE,       // 水雷
    MERMAID_E,  // 美人鱼
    TREASURE,   // 沉船宝藏
    SAFE,       // 保险箱
    STORM,      // 暴风雨
    ISLAND,     // 荒岛
    LAND        // 凯旋登陆
};

// 卡牌颜色（用于荒岛事件判定）
enum class CardColor {
    RED, BLUE, GREEN, YELLOW, PURPLE
};

// 卡牌类：单张卡牌的核心封装
class Card {
private:
    CardType mainType;       // 主类型
    std::string cardName;    // 卡牌名称（控制台打印用）
    CardColor color;         // 卡牌颜色（荒岛事件）
    int score;               // 基础分值（收集牌用）
    bool canPlay;            // 是否可打出（对牌可打，收集/增益不可打）
    bool isLocked;           // 是否被保险箱锁定（不可被影响）

    // 子类型（联合存储，仅对应主类型生效）
    PairSubType pairSubType;
    CollectSubType collectSubType;
    BuffSubType buffSubType;
    EventSubType eventSubType;

public:
    // 构造函数：对牌
    Card(CardType type, PairSubType pSub, CardColor c, const std::string& name, int s = 0);
    // 构造函数：收集牌
    Card(CardType type, CollectSubType cSub, CardColor c, const std::string& name, int s);
    // 构造函数：增益牌
    Card(CardType type, BuffSubType bSub, const std::string& name, int s = 0);
    // 构造函数：事件牌
    Card(CardType type, EventSubType eSub, const std::string& name, int s = 0);

    // 基础访问方法
    CardType getMainType() const;
    CardColor getColor() const;
    int getScore() const;
    bool isCanPlay() const;
    bool isCardLocked() const;
    std::string getCardName() const;

    // 子类型访问方法（按需获取，外部需先判断主类型）
    PairSubType getPairSubType() const;
    CollectSubType getCollectSubType() const;
    BuffSubType getBuffSubType() const;
    EventSubType getEventSubType() const;

    // 状态修改
    void lockCard();   // 锁定卡牌（保险箱事件）
    void unlockCard(); // 解锁卡牌
    void setScore(int s); // 修改分值（特殊规则用）
};

// 全局工具：获取卡牌类型的字符串（控制台打印）
std::string getCardTypeStr(CardType type);
std::string getCardColorStr(CardColor color);

#endif // CARD_H

