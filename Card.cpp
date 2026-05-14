#include "Card.h"

// 对牌构造
Card::Card(CardType type, PairSubType pSub, CardColor c, const std::string& name, int s)
    : mainType(type), pairSubType(pSub), color(c), cardName(name), score(s), canPlay(true), isLocked(false) {}

// 收集牌构造
Card::Card(CardType type, CollectSubType cSub, CardColor c, const std::string& name, int s)
    : mainType(type), collectSubType(cSub), color(c), cardName(name), score(s), canPlay(false), isLocked(false) {}

// 增益牌构造
Card::Card(CardType type, BuffSubType bSub, const std::string& name, int s)
    : mainType(type), buffSubType(bSub), cardName(name), score(s), canPlay(false), isLocked(false), color(CardColor::RED) {}

// 事件牌构造
Card::Card(CardType type, EventSubType eSub, const std::string& name, int s)
    : mainType(type), eventSubType(eSub), cardName(name), score(s), canPlay(true), isLocked(false), color(CardColor::RED) {}

// 基础访问方法实现
CardType Card::getMainType() const { return mainType; }
CardColor Card::getColor() const { return color; }
int Card::getScore() const { return score; }
bool Card::isCanPlay() const { return canPlay; }
bool Card::isCardLocked() const { return isLocked; }
std::string Card::getCardName() const { return cardName; }

// 子类型访问方法实现
PairSubType Card::getPairSubType() const { return pairSubType; }
CollectSubType Card::getCollectSubType() const { return collectSubType; }
BuffSubType Card::getBuffSubType() const { return buffSubType; }
EventSubType Card::getEventSubType() const { return eventSubType; }

// 状态修改实现
void Card::lockCard() { isLocked = true; }
void Card::unlockCard() { isLocked = false; }
void Card::setScore(int s) { score = s; }

// 全局工具：卡牌主类型转字符串
std::string getCardTypeStr(CardType type) {
    switch (type) {
    case CardType::PAIR_CARD: return "对牌";
    case CardType::COLLECT_CARD: return "收集牌";
    case CardType::BUFF_CARD: return "增益牌";
    default: return "未知牌型";
    }
}

// 全局工具：卡牌颜色转字符串
std::string getCardColorStr(CardColor color) {
    switch (color) {
    case CardColor::RED: return "红色";
    case CardColor::BLUE: return "蓝色";
    case CardColor::GREEN: return "绿色";
    case CardColor::YELLOW: return "黄色";
    case CardColor::PURPLE: return "紫色";
    default: return "未知颜色";
    }
}
