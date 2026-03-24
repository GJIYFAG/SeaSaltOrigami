#ifndef GAMERULE_H
#define GAMERULE_H

#include "Card.h"
#include "CardPile.h"
#include "Player.h"
#include <vector>
#include <string>
#include <unordered_map>

// 游戏规则类：核心逻辑中枢，实现所有游戏规则、流程、胜利判断
class GameRule {
private:
    int playerNum;                  // 玩家人数（2/3/4，决定胜利总分数）
    int currentPlayerIndex;         // 当前回合玩家索引
    bool isGameOver;                // 游戏是否结束
    CardPile gamePile;              // 游戏牌堆（主牌库+两个弃牌堆）
    std::vector<Player> players;    // 玩家列表
    std::vector<Card> allEventCards;// 事件牌库（问号对牌打出后抽取）

    // 胜利分数配置（2人=40，3人=35，4人=30）
    const std::unordered_map<int, int> WIN_SCORE = {{2,40}, {3,35}, {4,30}};

    // 私有：初始化游戏所有卡牌（生成对牌、收集牌、增益牌、事件牌）
    std::vector<Card> initAllGameCards();
    // 私有：初始化事件牌库
    void initEventCards();
    // 私有：判定两张牌是否为有效对牌（匹配游戏对牌规则）
    bool isValidPair(const Card& c1, const Card& c2);
    // 私有：触发对牌效果（核心，匹配文档中所有对牌效果）
    void triggerPairEffect(const Card& pairCard, Player& currPlayer);
    // 私有：触发事件牌效果（问号对牌打出后，从事件牌库抽一张触发）
    void triggerEventEffect(Player& currPlayer);
    // 私有：获取场上最高分玩家的索引
    int getMaxScorePlayerIndex() const;
    // 私有：计算玩家的颜色奖励分（最高颜色牌数量）
    int calcColorBonus(const Player& player) const;
    // 私有：切换到下一个玩家回合（顺时针）
    void nextPlayer();

public:
    // 构造函数：初始化游戏（指定玩家人数和玩家名称）
    GameRule(int pNum, const std::vector<std::string>& pNames);

    // 游戏开局流程（匹配文档：发手牌、弃牌堆初始化、洗牌）
    void gameStart();
    // 玩家抽牌阶段（两种抽牌方式，玩家选择）
    void drawCardPhase(Player& currPlayer, int drawType, int pileNo = 1);
    // 玩家出牌阶段（打出指定对牌，触发效果）
    bool playCardPhase(Player& currPlayer, const Card& c1, const Card& c2);
    // 轮次结束阶段（结算本轮分数，重置轮次状态，准备下一轮）
    void roundEnd(bool isAllIn = false);
    // 全局胜利判断（两种胜利条件：总分数达标/凑4张美人鱼）
    bool checkGameWin();
    // 获取胜利玩家
    Player getWinPlayer() const;
    // 执行当前玩家回合（完整流程：抽牌→出牌→判断是否结束轮次）
    void doCurrentPlayerTurn(int drawType, int pileNo = 1);

    // 游戏状态查询
    Player& getCurrentPlayer();     // 获取当前回合玩家
    int getPlayerNum() const;       // 获取玩家人数
    bool isGameOverFlag() const;    // 游戏是否结束
    CardPile& getGamePile();        // 获取游戏牌堆（只读）

    // 控制台打印：打印游戏全局状态（所有玩家+牌堆）
    void printGameState() const;
};

#endif // GAMERULE_H
