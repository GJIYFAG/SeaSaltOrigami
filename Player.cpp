#include "Player.h"
#include <algorithm>

// 构造函数：初始化玩家状态
Player::Player(const std::string& name)
    : playerName(name), totalScore(0), roundScore(0), mermaidCount(0), isSkipTurn(false) {}

// 私有：从指定容器移除卡牌
bool Player::removeCardFrom(std::vector<Card>& cardVec, const Card& card) {
    auto it = std::find_if(cardVec.begin(), cardVec.end(),
                           [&](const Card& c) { return c.getCardName() == card.getCardName() && c.getMainType() == c.getMainType(); });
    if (it != cardVec.end()) {
        cardVec.erase(it);
        return true;
    }
    return false;
}

// 手牌操作实现
void Player::addHandCard(const Card& card) { handCards.push_back(card); }
void Player::addHandCards(const std::vector<Card>& cards) {
    handCards.insert(handCards.end(), cards.begin(), cards.end());
}
bool Player::removeHandCard(const Card& card) { return removeCardFrom(handCards, card); }
void Player::clearHandCards() { handCards.clear(); }
const std::vector<Card>& Player::getHandCards() const { return handCards; }

// 收集牌/增益牌操作实现
void Player::addCollectCard(const Card& card) { collectCards.push_back(card); }
void Player::addBuffCard(const Card& card) { buffCards.push_back(card); }
const std::vector<Card>& Player::getCollectCards() const { return collectCards; }
const std::vector<Card>& Player::getBuffCards() const { return buffCards; }

// 锁定牌操作实现
void Player::addLockedCard(const Card& card) { lockedCards.push_back(card); }
void Player::unlockAllCards() { lockedCards.clear(); }
const std::vector<Card>& Player::getLockedCards() const { return lockedCards; }

// 美人鱼牌操作实现
void Player::addMermaidCard() {
    mermaidCount++;
    std::cout << "[" << playerName << "] 获得1张美人鱼牌，当前累计：" << mermaidCount << "张！" << std::endl;
}
int Player::getMermaidCount() const { return mermaidCount; }
void Player::resetMermaidCount() { mermaidCount = 0; }

// 分数操作实现
void Player::addRoundScore(int s) {
    roundScore += s;
    std::cout << "[" << playerName << "] 本轮分数+" << s << "，当前本轮分数：" << roundScore << std::endl;
}
void Player::addTotalScore(int s) {
    totalScore += s;
    std::cout << "[" << playerName << "] 总分数+" << s << "，当前总分数：" << totalScore << std::endl;
}
void Player::resetRoundScore() { roundScore = 0; }
int Player::getRoundScore() const { return roundScore; }
int Player::getTotalScore() const { return totalScore; }

// 回合状态实现
void Player::setSkipTurn(bool skip) {
    isSkipTurn = skip;
    std::cout << "[" << playerName << "] " << (skip ? "被跳过下一回合！" : "回合解锁！") << std::endl;
}
bool Player::isPlayerSkipTurn() const { return isSkipTurn; }

// 控制台打印玩家所有信息
void Player::printPlayerInfo() const {
    std::cout << "========== 玩家：" << playerName << " 信息 ==========" << std::endl;
    std::cout << "总分数：" << totalScore << " | 本轮分数：" << roundScore << std::endl;
    std::cout << "美人鱼牌数量：" << mermaidCount << " | 是否跳过回合：" << (isSkipTurn ? "是" : "否") << std::endl;

    std::cout << "手牌 (" << handCards.size() << "张)：";
    for (const auto& c : handCards) std::cout << c.getCardName() << " | ";
    std::cout << std::endl;

    std::cout << "收集牌 (" << collectCards.size() << "张)：";
    for (const auto& c : collectCards) std::cout << c.getCardName() << " | ";
    std::cout << std::endl;

    std::cout << "增益牌 (" << buffCards.size() << "张)：";
    for (const auto& c : buffCards) std::cout << c.getCardName() << " | ";
    std::cout << std::endl;

    std::cout << "锁定牌 (" << lockedCards.size() << "张)：";
    for (const auto& c : lockedCards) std::cout << c.getCardName() << " | ";
    std::cout << std::endl;
    std::cout << "=============================================" << std::endl;
}

// 获取玩家名称
std::string Player::getPlayerName() const { return playerName; }
