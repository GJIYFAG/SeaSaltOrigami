#include "GameRule.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_map>
#include <chrono>



// 构造函数：初始化游戏
GameRule::GameRule(int pNum, const std::vector<std::string>& pNames)
    : playerNum(pNum), currentPlayerIndex(0), isGameOver(false) {
    // 校验玩家人数
    if (pNum < 2 || pNum > 4) {
        throw std::runtime_error("玩家人数必须为2/3/4人！");
    }
    // 初始化玩家
    for (const auto& name : pNames) {
        players.emplace_back(name);
    }
    // 初始化所有卡牌并加载到牌堆
    std::vector<Card> allCards = initAllGameCards();
    gamePile.initMainPile(allCards);
    // 初始化事件牌库
    initEventCards();
    std::cout << "[游戏规则] 游戏初始化完成，玩家人数：" << playerNum << "人" << std::endl;
}

// 私有：初始化所有游戏卡牌（对牌+收集牌+增益牌）
std::vector<Card> GameRule::initAllGameCards() {
    std::vector<Card> allCards;
    CardColor colors[] = {CardColor::RED, CardColor::BLUE, CardColor::GREEN, CardColor::YELLOW, CardColor::PURPLE};
    int colorIdx = 0;

    // 1. 添加对牌（严格匹配文档中的数量）
    // 鱼牌(10)
    for (int i = 0; i < 10; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::FISH, colors[colorIdx%5], "鱼牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 螃蟹牌(7)
    for (int i = 0; i < 7; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::CRAB, colors[colorIdx%5], "螃蟹牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 船牌(8)
    for (int i = 0; i < 8; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::SHIP, colors[colorIdx%5], "船牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 游泳者牌(8)
    for (int i = 0; i < 8; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::SWIMMER, colors[colorIdx%5], "游泳者牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 鲨鱼牌(5)
    for (int i = 0; i < 5; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::SHARK, colors[colorIdx%5], "鲨鱼牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 水母牌(3)
    for (int i = 0; i < 3; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::JELLYFISH, colors[colorIdx%5], "水母牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 龙虾牌(1)
    allCards.emplace_back(CardType::PAIR_CARD, PairSubType::LOBSTER, colors[colorIdx%5], "龙虾牌1");
    colorIdx++;
    // 海龟牌(6)
    for (int i = 0; i < 6; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::TURTLE, colors[colorIdx%5], "海龟牌" + std::to_string(i+1));
        colorIdx++;
    }
    // 问号事件牌(6)
    for (int i = 0; i < 6; ++i) {
        allCards.emplace_back(CardType::PAIR_CARD, PairSubType::QUESTION, colors[colorIdx%5], "问号事件牌" + std::to_string(i+1));
        colorIdx++;
    }

    // 2. 添加收集牌（严格匹配文档中的数量和分值）
    // 贝壳牌(6)：0/2/4/6/8/10
    int shellScores[] = {0,2,4,6,8,10};
    for (int i = 0; i < 6; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::SHELL, colors[colorIdx%5], "贝壳牌" + std::to_string(i+1), shellScores[i]);
        colorIdx++;
    }
    // 章鱼牌(5)：0/3/6/9/12
    int octoScores[] = {0,3,6,9,12};
    for (int i = 0; i < 5; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::OCTOPUS, colors[colorIdx%5], "章鱼牌" + std::to_string(i+1), octoScores[i]);
        colorIdx++;
    }
    // 企鹅牌(3)：1/3/5
    int penguinScores[] = {1,3,5};
    for (int i = 0; i < 3; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::PENGUIN, colors[colorIdx%5], "企鹅牌" + std::to_string(i+1), penguinScores[i]);
        colorIdx++;
    }
    // 船锚牌(2)：0/5
    int anchorScores[] = {0,5};
    for (int i = 0; i < 2; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::ANCHOR, colors[colorIdx%5], "船锚牌" + std::to_string(i+1), anchorScores[i]);
        colorIdx++;
    }
    // 海马牌(1)：分值0
    allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::SEAHORSE, colors[colorIdx%5], "海马牌1", 0);
    colorIdx++;
    // 美人鱼牌(4)：分值0（特殊收集牌）
    for (int i = 0; i < 4; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::MERMAID, colors[colorIdx%5], "美人鱼牌" + std::to_string(i+1), 0);
        colorIdx++;
    }

    // 3. 添加增益牌（每种各1张，分值0）
    allCards.emplace_back(CardType::BUFF_CARD, BuffSubType::FISH_GROUP, "鱼群牌");
    allCards.emplace_back(CardType::BUFF_CARD, BuffSubType::CRAB_GROUP, "螃蟹群牌");
    allCards.emplace_back(CardType::BUFF_CARD, BuffSubType::SHIP_GROUP, "船队牌");
    allCards.emplace_back(CardType::BUFF_CARD, BuffSubType::PENGUIN_GROUP, "企鹅群牌");
    allCards.emplace_back(CardType::BUFF_CARD, BuffSubType::ANCHOR_PILE, "船锚堆牌");

    std::cout << "[游戏规则] 卡牌初始化完成，总卡牌数：" << allCards.size() << "张" << std::endl;
    return allCards;
}

// 私有：初始化事件牌库
void GameRule::initEventCards() {
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::MINE, "水雷事件牌");
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::MERMAID_E, "美人鱼事件牌");
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::TREASURE, "沉船宝藏事件牌");
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::SAFE, "保险箱事件牌");
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::STORM, "暴风雨事件牌");
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::ISLAND, "荒岛事件牌");
    allEventCards.emplace_back(CardType::PAIR_CARD, EventSubType::LAND, "凯旋登陆事件牌");
    // 洗牌事件牌库
    std::shuffle(allEventCards.begin(), allEventCards.end(), std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()));
}

// 私有：判定两张牌是否为有效对牌
bool GameRule::isValidPair(const Card& c1, const Card& c2) {
    // 必须都是对牌，且子类型相同
    if (c1.getMainType() != CardType::PAIR_CARD || c2.getMainType() != CardType::PAIR_CARD) {
        return false;
    }
    // 特殊组合：游泳者+鲨鱼、龙虾+螃蟹 也是有效对牌
    if ((c1.getPairSubType() == PairSubType::SWIMMER && c2.getPairSubType() == PairSubType::SHARK) ||
        (c1.getPairSubType() == PairSubType::SHARK && c2.getPairSubType() == PairSubType::SWIMMER) ||
        (c1.getPairSubType() == PairSubType::LOBSTER && c2.getPairSubType() == PairSubType::CRAB) ||
        (c1.getPairSubType() == PairSubType::CRAB && c2.getPairSubType() == PairSubType::LOBSTER)) {
        return true;
    }
    // 普通对牌：子类型相同
    return c1.getPairSubType() == c2.getPairSubType();
}

// 私有：触发对牌效果（核心，匹配文档）
void GameRule::triggerPairEffect(const Card& pairCard, Player& currPlayer) {
    PairSubType pType = pairCard.getPairSubType();
    switch (pType) {
    case PairSubType::FISH: {
        // 鱼牌：抽1张加入手牌
        std::vector<Card> draw = gamePile.drawFromMain(1);
        currPlayer.addHandCards(draw);
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出鱼牌对，抽1张牌加入手牌！" << std::endl;
        break;
    }
    case PairSubType::CRAB: {
        // 螃蟹牌：从一个弃牌堆选1张加入手牌（默认选弃牌堆1）
        if (!gamePile.isDiscardPileEmpty(1)) {
            Card c = gamePile.drawFromDiscardTop(1);
            currPlayer.addHandCard(c);
            std::cout << "[" << currPlayer.getPlayerName() << "] 打出螃蟹牌对，从弃牌堆1抽1张加入手牌！" << std::endl;
        } else {
            Card c = gamePile.drawFromDiscardTop(2);
            currPlayer.addHandCard(c);
            std::cout << "[" << currPlayer.getPlayerName() << "] 打出螃蟹牌对，从弃牌堆2抽1张加入手牌！" << std::endl;
        }
        break;
    }
    case PairSubType::SHIP: {
        // 船牌：重置自己回合（取消跳过状态）
        currPlayer.setSkipTurn(false);
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出船牌对，重置自身回合！" << std::endl;
        break;
    }
    case PairSubType::SWIMMER:
    case PairSubType::SHARK: {
        // 游泳者+鲨鱼：偷取任意玩家1张手牌（默认偷第一个其他玩家）
        if (players.size() > 1) {
            Player& target = players[(currentPlayerIndex+1)%playerNum];
            if (!target.getHandCards().empty() && !target.getHandCards()[0].isCardLocked()) {
                Card stealCard = target.getHandCards()[0];
                target.removeHandCard(stealCard);
                currPlayer.addHandCard(stealCard);
                std::cout << "[" << currPlayer.getPlayerName() << "] 打出游泳者+鲨鱼对，偷取"
                          << target.getPlayerName() << "的手牌：" << stealCard.getCardName() << std::endl;
            }
        }
        break;
    }
    case PairSubType::JELLYFISH: {
        // 水母牌：麻痹任意玩家跳过一回合（默认麻痹第一个其他玩家）
        if (players.size() > 1) {
            Player& target = players[(currentPlayerIndex+1)%playerNum];
            target.setSkipTurn(true);
            std::cout << "[" << currPlayer.getPlayerName() << "] 打出水母牌对，麻痹"
                      << target.getPlayerName() << "跳过一回合！" << std::endl;
        }
        break;
    }
    case PairSubType::LOBSTER: {
        // 龙虾+螃蟹：抽5张选1张，其余放回主牌库
        std::vector<Card> draw5 = gamePile.drawFromMain(5);
        if (!draw5.empty()) {
            currPlayer.addHandCard(draw5[0]); // 选第一张
            std::vector<Card> putBack(draw5.begin()+1, draw5.end());
            for (const auto& c : putBack) {
                gamePile.putToDiscard(1, {c}); // 放回弃牌堆1（可改为主牌库）
            }
            std::cout << "[" << currPlayer.getPlayerName() << "] 打出龙虾+螃蟹对，抽5张选1张，其余放回！" << std::endl;
        }
        break;
    }
    case PairSubType::TURTLE: {
        // 海龟牌：抵消其他玩家对牌效果（控制台打印提示，实际效果在出牌时判定）
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出海龟牌对，可抵消任意玩家的对牌效果！" << std::endl;
        break;
    }
    case PairSubType::QUESTION: {
        // 问号事件牌：触发事件牌效果
        triggerEventEffect(currPlayer);
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出问号事件牌对，触发随机事件！" << std::endl;
        break;
    }
    default: break;
    }
    // 所有对牌打出后，基础得分+1
    currPlayer.addRoundScore(1);
}

// 私有：触发事件牌效果
void GameRule::triggerEventEffect(Player& currPlayer) {
    if (allEventCards.empty()) initEventCards();
    Card eventCard = allEventCards.back();
    allEventCards.pop_back();
    EventSubType eType = eventCard.getEventSubType();

    switch (eType) {
    case EventSubType::MINE: {
        // 水雷：摧毁任意玩家1张非锁定手牌
        Player& target = players[(currentPlayerIndex+1)%playerNum];
        for (auto& c : target.getHandCards()) {
            if (!c.isCardLocked()) {
                target.removeHandCard(c);
                std::cout << "【水雷事件】摧毁" << target.getPlayerName() << "的手牌：" << c.getCardName() << std::endl;
                break;
            }
        }
        break;
    }
    case EventSubType::MERMAID_E: {
        // 美人鱼：获得1张美人鱼牌
        currPlayer.addMermaidCard();
        std::cout << "【美人鱼事件】" << currPlayer.getPlayerName() << "获得1张美人鱼牌！" << std::endl;
        break;
    }
    case EventSubType::TREASURE: {
        // 沉船宝藏：抽2张牌+得分+1
        currPlayer.addHandCards(gamePile.drawFromMain(2));
        currPlayer.addRoundScore(1);
        std::cout << "【沉船宝藏事件】" << currPlayer.getPlayerName() << "抽2张牌，本轮分数+1！" << std::endl;
        break;
    }
    case EventSubType::SAFE: {
        // 保险箱：锁定手牌中2张牌（带入下一轮）
        if (currPlayer.getHandCards().size() >= 2) {
            currPlayer.addLockedCard(currPlayer.getHandCards()[0]);
            currPlayer.addLockedCard(currPlayer.getHandCards()[1]);
            std::cout << "【保险箱事件】" << currPlayer.getPlayerName() << "锁定2张手牌，带入下一轮！" << std::endl;
        }
        break;
    }
    case EventSubType::STORM: {
        // 暴风雨：所有玩家弃1张非锁定牌，洗牌弃牌堆
        for (auto& p : players) {
            for (auto& c : p.getHandCards()) {
                if (!c.isCardLocked()) {
                    p.removeHandCard(c);
                    gamePile.putToDiscard(1, {c});
                    std::cout << "【暴风雨事件】" << p.getPlayerName() << "弃1张牌：" << c.getCardName() << std::endl;
                    break;
                }
            }
        }
        gamePile.shuffleDiscardPile(1);
        gamePile.shuffleDiscardPile(2);
        break;
    }
    case EventSubType::ISLAND: {
        // 荒岛：与任意玩家比颜色，相同则偷牌（简化版：比第一张牌颜色）
        Player& target = players[(currentPlayerIndex+1)%playerNum];
        if (!currPlayer.getHandCards().empty() && !target.getHandCards().empty()) {
            Card c1 = currPlayer.getHandCards()[0];
            Card c2 = target.getHandCards()[0];
            if (c1.getColor() == c2.getColor()) {
                target.removeHandCard(c2);
                currPlayer.addHandCard(c2);
                std::cout << "【荒岛事件】颜色相同，" << currPlayer.getPlayerName() << "偷取"
                          << target.getPlayerName() << "的手牌：" << c2.getCardName() << std::endl;
            } else {
                std::cout << "【荒岛事件】颜色不同，无效果！" << std::endl;
            }
        }
        break;
    }
    case EventSubType::LAND: {
        // 凯旋登陆：抽玩家数+1张，选2张，其余给其他玩家
        int drawNum = playerNum + 1;
        std::vector<Card> draw = gamePile.drawFromMain(drawNum);
        if (draw.size() >= 2) {
            currPlayer.addHandCard(draw[0]);
            currPlayer.addHandCard(draw[1]);
            std::vector<Card> giveOthers(draw.begin()+2, draw.end());
            int idx = (currentPlayerIndex+1)%playerNum;
            for (const auto& c : giveOthers) {
                players[idx].addHandCard(c);
                std::cout << "【凯旋登陆事件】给" << players[idx].getPlayerName() << "发牌：" << c.getCardName() << std::endl;
                idx = (idx+1)%playerNum;
            }
        }
        break;
    }
    default: break;
    }
}

// 私有：获取场上最高分玩家索引
int GameRule::getMaxScorePlayerIndex() const {
    int maxScore = -1;
    int idx = 0;
    for (int i = 0; i < playerNum; ++i) {
        if (players[i].getRoundScore() > maxScore) {
            maxScore = players[i].getRoundScore();
            idx = i;
        }
    }
    return idx;
}

// 私有：计算颜色奖励分（最高颜色牌数量）
int GameRule::calcColorBonus(const Player& player) const {
    std::unordered_map<CardColor, int> colorCount;
    // 统计手牌+收集牌的颜色数量
    for (const auto& c : player.getHandCards()) colorCount[c.getColor()]++;
    for (const auto& c : player.getCollectCards()) colorCount[c.getColor()]++;
    // 找最大值
    int maxCount = 0;
    for (const auto& p : colorCount) maxCount = std::max(maxCount, p.second);
    return maxCount;
}

// 私有：切换下一个玩家（顺时针）
void GameRule::nextPlayer() {
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % playerNum;
    } while (players[currentPlayerIndex].isPlayerSkipTurn()); // 跳过被麻痹的玩家
}

// 游戏开局流程
void GameRule::gameStart() {
    // 1. 洗牌主牌库
    gamePile.shuffleMainPile();
    // 2. 每个玩家发2张手牌（匹配文档开局规则）
    for (auto& p : players) {
        std::vector<Card> initHand = gamePile.drawFromMain(2);
        p.addHandCards(initHand);
        std::cout << "[" << p.getPlayerName() << "] 获得开局手牌2张！" << std::endl;
    }
    // 3. 从主牌库抽2张牌，分别放入两个弃牌堆（初始化弃牌堆）
    gamePile.putToDiscardTop(1, gamePile.drawFromMain(1)[0]);
    gamePile.putToDiscardTop(2, gamePile.drawFromMain(1)[0]);
    std::cout << "[游戏] 开局完成！两个弃牌堆各初始化1张牌！" << std::endl;
    // 4. 打印初始游戏状态
    printGameState();
}

// 玩家抽牌阶段：drawType=1（主牌库抽2弃1），drawType=2（弃牌堆顶抽1）
void GameRule::drawCardPhase(Player& currPlayer, int drawType, int pileNo) {
    if (drawType == 1) {
        // 方式1：主牌库抽2张，选1张加入手牌，1张放入任意弃牌堆
        std::vector<Card> draw2 = gamePile.drawFromMain(2);
        if (draw2.size() >= 2) {
            currPlayer.addHandCard(draw2[0]);
            gamePile.putToDiscardTop(pileNo, draw2[1]);
            std::cout << "[" << currPlayer.getPlayerName() << "] 从主牌库抽2张，选1张加入手牌，1张放入弃牌堆" << pileNo << std::endl;
        }
    } else if (drawType == 2) {
        // 方式2：从指定弃牌堆顶抽1张加入手牌
        if (!gamePile.isDiscardPileEmpty(pileNo)) {
            Card c = gamePile.drawFromDiscardTop(pileNo);
            currPlayer.addHandCard(c);
            std::cout << "[" << currPlayer.getPlayerName() << "] 从弃牌堆" << pileNo << "顶抽1张加入手牌！" << std::endl;
        } else {
            std::cout << "[" << currPlayer.getPlayerName() << "] 弃牌堆" << pileNo << "为空，抽牌失败！" << std::endl;
        }
    }
}

// 玩家出牌阶段：打出对牌，触发效果
bool GameRule::playCardPhase(Player& currPlayer, const Card& c1, const Card& c2) {
    // 判定是否为有效对牌
    if (!isValidPair(c1, c2)) {
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出的牌不是有效对牌，出牌失败！" << std::endl;
        return false;
    }
    // 移除手牌中的两张牌
    bool rm1 = currPlayer.removeHandCard(c1);
    bool rm2 = currPlayer.removeHandCard(c2);
    if (!rm1 || !rm2) {
        std::cout << "[" << currPlayer.getPlayerName() << "] 手牌中无此两张牌，出牌失败！" << std::endl;
        return false;
    }
    // 触发对牌效果
    triggerPairEffect(c1, currPlayer);
    // 将打出的牌放入弃牌堆1
    gamePile.putToDiscard(1, {c1, c2});
    return true;
}

// 轮次结束阶段：isAllIn=是否放手一搏
void GameRule::roundEnd(bool isAllIn) {
    std::cout << "========================================" << std::endl;
    std::cout << "[游戏] 本轮结束，开始分数结算！" << std::endl;
    int maxScoreIdx = getMaxScorePlayerIndex();
    // 遍历所有玩家结算分数
    for (int i = 0; i < playerNum; ++i) {
        Player& p = players[i];
        int colorBonus = calcColorBonus(p);
        int totalRound = p.getRoundScore();
        // 最高分玩家获得双倍颜色奖励分，其他玩家获得基础颜色奖励分
        if (i == maxScoreIdx) {
            totalRound += colorBonus * 2;
            std::cout << "[" << p.getPlayerName() << "] 为本轮最高分，颜色奖励分x2：" << colorBonus*2 << std::endl;
        } else {
            totalRound += colorBonus;
            std::cout << "[" << p.getPlayerName() << "] 颜色奖励分：" << colorBonus << std::endl;
        }
        // 本轮分数计入总分数
        p.addTotalScore(totalRound);
        // 重置轮次状态
        p.resetRoundScore();
        p.resetMermaidCount();
        p.unlockAllCards();
    }
    std::cout << "[游戏] 本轮分数结算完成！" << std::endl;
    std::cout << "========================================" << std::endl;
    // 检查全局胜利
    if (checkGameWin()) {
        isGameOver = true;
        Player winPlayer = getWinPlayer();
        std::cout << "[游戏] 游戏结束！胜利者：" << winPlayer.getPlayerName() << "，总分数：" << winPlayer.getTotalScore() << std::endl;
    } else {
        // 准备下一轮：洗牌所有牌堆，重新发牌
        gamePile.shuffleMainPile();
        gamePile.shuffleDiscardPile(1);
        gamePile.shuffleDiscardPile(2);
        std::cout << "[游戏] 准备下一轮，所有牌堆重新洗牌！" << std::endl;
    }
}

// 全局胜利判断
bool GameRule::checkGameWin() {
    // 条件1：任意玩家凑齐4张美人鱼牌
    for (const auto& p : players) {
        if (p.getMermaidCount() >= 4) {
            std::cout << "[胜利] " << p.getPlayerName() << "收集齐4张美人鱼牌，直接胜利！" << std::endl;
            return true;
        }
    }
    // 条件2：任意玩家总分数达到对应胜利分数
    int winScore = WIN_SCORE.at(playerNum);
    for (const auto& p : players) {
        if (p.getTotalScore() >= winScore) {
            std::cout << "[胜利] " << p.getPlayerName() << "总分数达到" << winScore << "，获得胜利！" << std::endl;
            return true;
        }
    }
    return false;
}

// 获取胜利玩家
Player GameRule::getWinPlayer() const {
    int winScore = WIN_SCORE.at(playerNum);
    for (const auto& p : players) {
        if (p.getMermaidCount() >=4 || p.getTotalScore() >= winScore) {
            return p;
        }
    }
    return players[0];
}

// 执行当前玩家完整回合
void GameRule::doCurrentPlayerTurn(int drawType, int pileNo) {
    Player& currPlayer = getCurrentPlayer();
    std::cout << "========================================" << std::endl;
    std::cout << "[游戏] 开始" << currPlayer.getPlayerName() << "的回合！" << std::endl;
    // 1. 抽牌阶段
    drawCardPhase(currPlayer, drawType, pileNo);
    // 2. 出牌阶段（简化版：打印手牌，提示可出牌，实际测试可手动指定）
    std::cout << "[" << currPlayer.getPlayerName() << "] 手牌：";
    for (const auto& c : currPlayer.getHandCards()) {
        std::cout << c.getCardName() << " | ";
    }
    std::cout << std::endl;
    // 3. 判定是否可结束轮次（本轮分数>=10）
    if (currPlayer.getRoundScore() >= 10) {
        std::cout << "[" << currPlayer.getPlayerName() << "] 本轮分数>=10，可选择结束本轮！" << std::endl;
        // 测试默认选择直接截停轮次
        roundEnd(false);
    } else {
        // 4. 切换到下一个玩家
        nextPlayer();
        std::cout << "[" << currPlayer.getPlayerName() << "] 回合结束，切换到下一个玩家！" << std::endl;
    }
}

// 获取当前回合玩家
Player& GameRule::getCurrentPlayer() {
    return players[currentPlayerIndex];
}

// 游戏状态查询
int GameRule::getPlayerNum() const { return playerNum; }
bool GameRule::isGameOverFlag() const { return isGameOver; }
CardPile& GameRule::getGamePile() { return gamePile; }

// 打印游戏全局状态
void GameRule::printGameState() const {
    std::cout << "========================================" << std::endl;
    std::cout << "************** 游戏全局状态 **************" << std::endl;
    // 打印所有玩家信息
    for (const auto& p : players) {
        p.printPlayerInfo();
    }
    // 打印牌堆信息
    gamePile.printPile(0);  // 主牌库
    gamePile.printPile(1);  // 弃牌堆1
    gamePile.printPile(2);  // 弃牌堆2
    std::cout << "========================================" << std::endl;
}
