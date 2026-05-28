#ifndef PLAYER_H
#define PLAYER_H
#include "Card.h"
#include <vector>
#include <string>
#include <iostream>

class Player {
private:
    std::string playerName;
    int totalScore;
    int eventScore;
    int roundScore;
    int mermaidCount;
    bool isSkipTurn;
    bool hasDrawnThisTurn; // ✅ 新增：本回合是否已抽牌（核心限制）
    std::vector<Card> handCards;
    std::vector<Card> collectCards;
    std::vector<Card> buffCards;
    std::vector<Card> lockedCards;
    std::vector<Card> pairPlayArea;

    bool removeCardFrom(std::vector<Card>& cardVec, const Card& card);
public:
    void addEventScore(int s);
    int getEventScore() const;
    void resetEventScore();

    Player(const std::string& name);

    // 手牌操作
    void addHandCard(const Card& card);
    void addHandCards(const std::vector<Card>& cards);
    bool removeHandCard(const Card& card);
    void clearHandCards();
    void clearCollectCards();
    void clearBuffCards();
    void resetForNextRound();
    const std::vector<Card>& getHandCards() const;
    bool moveHandCard(int fromIndex, int toIndex);

    // 收集牌/增益牌操作
    void addCollectCard(const Card& card);
    void addBuffCard(const Card& card);
    const std::vector<Card>& getCollectCards() const;
    const std::vector<Card>& getBuffCards() const;

    // 锁定牌操作
    void addLockedCard(const Card& card);
    void unlockAllCards();
    const std::vector<Card>& getLockedCards() const;

    // 美人鱼牌操作
    void addMermaidCard();
    int getMermaidCount() const;
    void resetMermaidCount();

    // 分数操作
    void addRoundScore(int s);
    void addTotalScore(int s);
    void resetRoundScore();
    int getRoundScore() const;
    int getTotalScore() const;

    // 回合状态
    void setSkipTurn(bool skip);
    bool isPlayerSkipTurn() const;

    // ✅ 新增：回合抽牌状态管理
    bool getHasDrawnThisTurn() const;
    void setHasDrawnThisTurn(bool hasDrawn);
    void resetTurnState(); // 回合切换时重置所有状态

    void printPlayerInfo() const;
    std::string getPlayerName() const;
    void autoClassifyCards();
    int getCollectSubTypeTotalScore(CollectSubType subType) const;
    void setRoundScore(int s) { roundScore = s; }
    void addPairToPlayArea(const Card& c1, const Card& c2);
    const std::vector<Card>& getPairPlayArea() const;
    void clearPairPlayArea();
};
#endif // PLAYER_H
