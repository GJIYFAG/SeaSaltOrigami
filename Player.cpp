#include "Player.h"
#include <algorithm>

// 构造函数：初始化玩家状态
Player::Player(const std::string& name)
    : playerName(name), totalScore(0),eventScore(0), roundScore(0), mermaidCount(0),
    isSkipTurn(false), hasDrawnThisTurn(false) {} // ✅ 初始化抽牌标记

// 新增：事件牌加分方法
void Player::addEventScore(int s) {
    eventScore += s;
    std::cout << "[" << playerName << "] 事件牌得分+" << s << "，当前事件牌得分：" << eventScore << std::endl;
}

// 新增：获取事件牌得分
int Player::getEventScore() const { return eventScore; }

// 新增：重置事件牌得分（轮次结束）
void Player::resetEventScore() { eventScore = 0; }

// ✅ 新增：获取本回合是否已抽牌
bool Player::getHasDrawnThisTurn() const {
    return hasDrawnThisTurn;
}

// ✅ 新增：设置本回合抽牌状态
void Player::setHasDrawnThisTurn(bool hasDrawn) {
    hasDrawnThisTurn = hasDrawn;
}

// ✅ 新增：重置回合状态（回合切换/船牌触发时调用）
void Player::resetTurnState() {
    hasDrawnThisTurn = false;
    isSkipTurn = false;
    std::cout << "[" << playerName << "] 回合状态已重置，可重新抽牌！" << std::endl;
}

// 私有：从指定容器移除卡牌
bool Player::removeCardFrom(std::vector<Card>& cardVec, const Card& card) {
    auto it = std::find_if(cardVec.begin(), cardVec.end(),
                           [&](const Card& c) { return c.getCardName() == card.getCardName() && c.getMainType() == card.getMainType(); });
    if (it != cardVec.end()) {
        cardVec.erase(it);
        return true;
    }
    return false;
}

// 手牌操作实现
void Player::addHandCard(const Card& card) {
    handCards.push_back(card);
    autoClassifyCards();
}

void Player::addHandCards(const std::vector<Card>& cards) {
    handCards.insert(handCards.end(), cards.begin(), cards.end());
    autoClassifyCards();
}

bool Player::removeHandCard(const Card& card) { return removeCardFrom(handCards, card); }
void Player::clearHandCards() { handCards.clear(); }
void Player::clearCollectCards() { collectCards.clear(); }
void Player::clearBuffCards() { buffCards.clear(); }

void Player::resetForNextRound() {
    handCards.clear();
    collectCards.clear();
    buffCards.clear();
    lockedCards.clear();
    pairPlayArea.clear();
    roundScore = 0;
    eventScore = 0;
    mermaidCount = 0;
    isSkipTurn = false;
    hasDrawnThisTurn = false;
}

const std::vector<Card>& Player::getHandCards() const { return handCards; }

bool Player::moveHandCard(int fromIndex, int toIndex) {
    if (fromIndex < 0 || toIndex < 0 || fromIndex >= static_cast<int>(handCards.size()) || toIndex >= static_cast<int>(handCards.size()) || fromIndex == toIndex) {
        return false;
    }
    Card moving = handCards[static_cast<size_t>(fromIndex)];
    handCards.erase(handCards.begin() + fromIndex);
    handCards.insert(handCards.begin() + toIndex, moving);
    return true;
}

// 收集牌/增益牌操作实现
void Player::addCollectCard(const Card& card) {
    collectCards.push_back(card);
    if (card.getCollectSubType() == CollectSubType::MERMAID) {
        addMermaidCard();
    }
}
void Player::addBuffCard(const Card& card) { buffCards.push_back(card); }
const std::vector<Card>& Player::getCollectCards() const { return collectCards; }
const std::vector<Card>& Player::getBuffCards() const { return buffCards; }

// 锁定牌操作实现
void Player::addLockedCard(const Card& card) {
    for (auto& handCard : handCards) {
        if (handCard.getCardName() == card.getCardName() && handCard.getMainType() == card.getMainType()) {
            handCard.lockCard();
            lockedCards.push_back(handCard);
            return;
        }
    }
    Card locked = card;
    locked.lockCard();
    lockedCards.push_back(locked);
}
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
    std::cout << "本回合是否已抽牌：" << (hasDrawnThisTurn ? "是" : "否") << std::endl;
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

// 自动识别手牌中的收集牌/增益牌，转移到对应容器
void Player::autoClassifyCards() {
    std::vector<Card> newHandCards;
    for (auto& card : handCards) {
        if (card.getMainType() == CardType::COLLECT_CARD) {
            collectCards.push_back(card);
            if (card.getCollectSubType() == CollectSubType::MERMAID) {
                addMermaidCard();
            }
            std::cout << "[" << playerName << "] 收集到：" << card.getCardName() << "，分值：" << card.getScore() << std::endl;
        } else if (card.getMainType() == CardType::BUFF_CARD) {
            buffCards.push_back(card);
            std::cout << "[" << playerName << "] 获得增益牌：" << card.getCardName() << std::endl;
        } else {
            newHandCards.push_back(card);
        }
    }
    handCards = newHandCards;
}

// 统计指定收集牌子类型的总得分
int Player::getCollectSubTypeTotalScore(CollectSubType subType) const {
    int total = 0;
    for (const auto& card : collectCards) {
        if (card.getCollectSubType() == subType) {
            total += card.getScore();
        }
    }
    return total;
}

// 添加一对有效对牌到打出区
void Player::addPairToPlayArea(const Card& c1, const Card& c2) {
    pairPlayArea.push_back(c1);
    pairPlayArea.push_back(c2);
}

// 获取对牌打出区
const std::vector<Card>& Player::getPairPlayArea() const {
    return pairPlayArea;
}

// 清空打出区（轮次结束调用）
void Player::clearPairPlayArea() {
    pairPlayArea.clear();
}
