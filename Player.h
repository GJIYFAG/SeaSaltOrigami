#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <vector>
#include <string>
#include <iostream>

// 玩家类：管理玩家的卡牌、分数、状态，封装核心行为
class Player {
private:
    std::string playerName;    // 玩家名称
    int totalScore;            // 总分数（累计所有轮次，用于全局胜利判断）
    int roundScore;            // 本轮分数（单轮结算用）
    int mermaidCount;          // 美人鱼牌数量（凑4张直接胜利）
    bool isSkipTurn;           // 是否被跳过回合（水母/荒岛事件）

    // 玩家的卡牌分类（严格匹配游戏规则）
    std::vector<Card> handCards;       // 手牌（可抽/打/弃）
    std::vector<Card> collectCards;    // 收集牌（不可打出，凑数得分）
    std::vector<Card> buffCards;       // 增益牌（不可打出，配合对牌得分）
    std::vector<Card> lockedCards;     // 锁定牌（保险箱事件，不可被影响，带入下一轮）

    // 私有：移除指定卡牌（从指定卡牌容器）
    bool removeCardFrom(std::vector<Card>& cardVec, const Card& card);

public:
    // 构造函数
    Player(const std::string& name);

    // 手牌操作
    void addHandCard(const Card& card);    // 添加手牌
    void addHandCards(const std::vector<Card>& cards); // 添加多张手牌
    bool removeHandCard(const Card& card); // 移除指定手牌
    void clearHandCards();                 // 清空手牌（特殊效果）
    const std::vector<Card>& getHandCards() const; // 获取手牌（只读）

    // 收集牌/增益牌操作
    void addCollectCard(const Card& card); // 添加收集牌
    void addBuffCard(const Card& card);    // 添加增益牌
    const std::vector<Card>& getCollectCards() const;
    const std::vector<Card>& getBuffCards() const;

    // 锁定牌操作（保险箱事件）
    void addLockedCard(const Card& card);  // 添加锁定牌
    void unlockAllCards();                 // 解锁所有牌
    const std::vector<Card>& getLockedCards() const;

    // 美人鱼牌操作
    void addMermaidCard();                 // 获得一张美人鱼牌
    int getMermaidCount() const;           // 获取美人鱼牌数量
    void resetMermaidCount();              // 重置美人鱼数量（轮次结束）

    // 分数操作
    void addRoundScore(int s);             // 增加本轮分数
    void addTotalScore(int s);             // 增加总分数
    void resetRoundScore();                // 重置本轮分数（轮次结束）
    int getRoundScore() const;
    int getTotalScore() const;

    // 回合状态
    void setSkipTurn(bool skip);           // 设置是否跳过回合
    bool isPlayerSkipTurn() const;         // 是否跳过回合

    // 控制台打印：打印玩家所有信息（手牌/分数/美人鱼数量等）
    void printPlayerInfo() const;

    // 玩家名称
    std::string getPlayerName() const;
};

#endif // PLAYER_H

