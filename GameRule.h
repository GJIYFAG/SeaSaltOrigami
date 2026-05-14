#ifndef GAMERULE_H
#define GAMERULE_H
#include "Card.h"
#include "CardPile.h"
#include "Player.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

class GameRule
{
private:
    int playerNum;                  // 2/3/4人
    int currentPlayerIndex;         // 当前玩家索引
    bool isGameOver;                // 游戏是否结束
    CardPile gamePile;              // 牌堆（主牌库+2个弃牌堆）
    std::vector<Player> players;    // 玩家列表
    std::vector<Card> allEventCards;// 事件牌库
    const std::unordered_map<int, int> WIN_SCORE = {{2, 40}, {3, 35}, {4, 30}}; // 胜利分数

    // 私有核心方法（原有逻辑保留，新增颜色分/美人鱼计分相关）
    std::vector<Card> initAllGameCards();
    void initEventCards();
//    bool isValidPair(const Card& c1, const Card& c2);
    void triggerPairEffect(const Card& pairCard, Player& currPlayer);
    void triggerEventEffect(Player& currPlayer);
    int getMaxScorePlayerIndex() const; // 获取本轮最高分玩家索引
    void nextPlayer();

    // 新增：颜色分相关核心计算（严格按规则）
    int getMaxColorCount() const; // 获取全场最高颜色牌数量（所有玩家中最多的颜色数量）
    int getColorRewardScore(int colorCount) const; // 颜色数量 → 颜色奖励分（1:1映射，可自定义）
    std::vector<int> getPlayerRoundScores() const; // 获取所有玩家的本轮基础分（不含颜色分）

    // 新增：美人鱼单张计分（按颜色奖励分排名）
    int calcMermaidSingleScore(const Player& player) const;

    // 原有收集牌计分（保留档位+海马伪装，新增美人鱼单张计分整合）
    int calcCollectCardScore(const Player& player) const;
    // 原有增益牌计分（保留配合对牌规则）
    int calcBuffCardScore(const Player& player) const;

    int calcColorBonus(const Player& player) const;
    int calcRoundBaseScore(const Player& player) const;
    void prepareNextRound(int nextStartIndex);

    int calcShellScore(int count) const;
    int calcOctopusScore(int count) const;
    int calcPenguinScore(int count) const;
    int calcAnchorScore(int count) const;
public:
    // 收集牌数量统计
    int getShellCount(const Player& player) const;
    int getOctopusCount(const Player& player) const;
    int getPenguinCount(const Player& player) const;
    int getAnchorCount(const Player& player) const;
    int getSeahorseCount(const Player& player) const;

    // 收集牌得分统计
    int getShellScore(const Player& player) const;
    int getOctopusScore(const Player& player) const;
    int getPenguinScore(const Player& player) const;
    int getAnchorScore(const Player& player) const;

    // 构造函数（原有）
    GameRule(int pNum, const std::vector<std::string>& pNames);

    // 原有公开方法（保留）
    void gameStart();
    void drawCardPhase(Player& currPlayer, int drawType, int pileNo = 1);
    bool playCardPhase(Player& currPlayer, const Card& c1, const Card& c2);
    void roundEnd(bool isAllIn = false, int allInStarterIndex = -1); // isAllIn=true=放手一搏，false=直接截停
    bool checkGameWin();
    Player getWinPlayer() const;
    void doCurrentPlayerTurn(int drawType, int pileNo = 1);
    Player& getCurrentPlayer();
    int getPlayerNum() const;
    bool isGameOverFlag() const;
    CardPile& getGamePile();
    void printGameState() const;
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }
    void endCurrentTurn() { nextPlayer(); }
    bool isValidPair(const Card& c1, const Card& c2); // 公开对牌判断

    // 新增：UI层专用接口（获取精准计分数据）
    int getMaxColorCountPublic() const { return getMaxColorCount(); }
    int getColorRewardScorePublic(int colorCount) const { return getColorRewardScore(colorCount); }
    int calcCollectCardScorePublic(const Player& player) const { return calcCollectCardScore(player); }
    int calcBuffCardScorePublic(const Player& player) const { return calcBuffCardScore(player); }
    int calcMermaidSingleScorePublic(const Player& player) const { return calcMermaidSingleScore(player); }
    int calcRoundBaseScorePublic(const Player& player) const { return calcRoundBaseScore(player); }
    std::vector<int> getColorRewardRankings() const; // 获取颜色奖励分排名（降序：最多→次高→第三高→第四高）

    // 新增：UI层获取收集牌子类型总得分
    int getCollectSubTypeTotalScore(const Player& player, CollectSubType subType) const {
        int total = 0;
        for (const auto& card : player.getCollectCards()) {
            if (card.getCollectSubType() == subType) {
                total += card.getScore();
            }
        }
        return total;
    }

};

#endif // GAMERULE_H
