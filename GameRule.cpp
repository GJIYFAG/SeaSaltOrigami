#include "GameRule.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_map>
#include <chrono>

namespace {
bool hasPairSubType(const Card& c1, const Card& c2, PairSubType subType)
{
    return c1.getPairSubType() == subType || c2.getPairSubType() == subType;
}

std::unordered_map<CardColor, int> ownedColorCount(const Player& player)
{
    std::unordered_map<CardColor, int> colorCount;
    for (const auto& card : player.getHandCards()) {
        colorCount[card.getColor()]++;
    }
    for (const auto& card : player.getCollectCards()) {
        colorCount[card.getColor()]++;
    }
    for (const auto& card : player.getPairPlayArea()) {
        colorCount[card.getColor()]++;
    }
    return colorCount;
}
}

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
    // ========== 贝壳牌（6张） ==========
    int shellScores[] = {0,0,0,0,0,0}; // 单张无分，仅凑数加分
    for (int i = 0; i < 6; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::SHELL, colors[colorIdx%5], "贝壳牌" + std::to_string(i+1), shellScores[i]);
        colorIdx++;
    }

    // ========== 章鱼牌（5张） ==========
    int octopusScores[] = {0,0,0,0,0}; // 单张无分
    for (int i = 0; i < 5; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::OCTOPUS, colors[colorIdx%5], "章鱼牌" + std::to_string(i+1), octopusScores[i]);
        colorIdx++;
    }

    // ========== 企鹅牌（3张） ==========
    int penguinScores[] = {0,0,0}; // 单张无分
    for (int i = 0; i < 3; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::PENGUIN, colors[colorIdx%5], "企鹅牌" + std::to_string(i+1), penguinScores[i]);
        colorIdx++;
    }

    // ========== 船锚牌（2张） ==========
    int anchorScores[] = {0,0}; // 单张无分
    for (int i = 0; i < 2; ++i) {
        allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::ANCHOR, colors[colorIdx%5], "船锚牌" + std::to_string(i+1), anchorScores[i]);
        colorIdx++;
    }

    // ========== 海马牌（1张） ==========
    allCards.emplace_back(CardType::COLLECT_CARD, CollectSubType::SEAHORSE, colors[colorIdx%5], "海马牌", 0);
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

// 私有：判定两张牌是否为有效对牌（严格遵循文档规则）
bool GameRule::isValidPair(const Card& c1, const Card& c2) {
    // 1. 必须都是对牌（排除收集牌/增益牌）
    if (c1.getMainType() != CardType::PAIR_CARD || c2.getMainType() != CardType::PAIR_CARD) {
        return false;
    }

    PairSubType sub1 = c1.getPairSubType();
    PairSubType sub2 = c2.getPairSubType();

    // 2. 特殊组合：游泳者+鲨鱼、游泳者+水母、龙虾+螃蟹
    if (
        ((sub1 == PairSubType::SWIMMER && sub2 == PairSubType::SHARK) ||
         (sub1 == PairSubType::SHARK && sub2 == PairSubType::SWIMMER)) ||
        ((sub1 == PairSubType::SWIMMER && sub2 == PairSubType::JELLYFISH) ||
         (sub1 == PairSubType::JELLYFISH && sub2 == PairSubType::SWIMMER)) ||
        ((sub1 == PairSubType::LOBSTER && sub2 == PairSubType::CRAB) ||
         (sub1 == PairSubType::CRAB && sub2 == PairSubType::LOBSTER))
        ) {
        return true;
    }

    // 3. 普通对牌：同子类型有效，但排除需要组合触发的单张类别
    if (sub1 == sub2) {
        if (sub1 == PairSubType::SWIMMER || sub1 == PairSubType::SHARK
            || sub1 == PairSubType::JELLYFISH || sub1 == PairSubType::LOBSTER) {
            return false;
        }
        return true;
    }

    // 4. 其他情况（非上述组合）：无效
    return false;
}


// 私有：触发对牌效果（核心，匹配文档）
void GameRule::triggerPairEffect(const Card& c1, const Card& c2, Player& currPlayer) {
    PairSubType pType = c1.getPairSubType();
    if (hasPairSubType(c1, c2, PairSubType::LOBSTER)) {
        pType = PairSubType::LOBSTER;
    } else if (hasPairSubType(c1, c2, PairSubType::JELLYFISH)) {
        pType = PairSubType::JELLYFISH;
    } else if (hasPairSubType(c1, c2, PairSubType::SHARK)) {
        pType = PairSubType::SHARK;
    }
    switch (pType) {
    case PairSubType::FISH: {
        // 鱼牌：抽1张加入手牌
        std::vector<Card> draw = gamePile.drawFromMain(1);
        currPlayer.addHandCards(draw);
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出鱼牌对，抽1张牌加入手牌！" << std::endl;
        break;
    }
    case PairSubType::CRAB: {
        // 螃蟹牌：从一堆弃牌堆中选择一张。当前交互层未提供挑选界面，默认取牌更多的一堆顶牌。
        Card drawnCard;
        int firstPile = gamePile.discard2Count() > gamePile.discard1Count() ? 2 : 1;
        int secondPile = firstPile == 1 ? 2 : 1;
        if (!gamePile.isDiscardPileEmpty(firstPile)) {
            drawnCard = gamePile.drawFromDiscardTop(firstPile);
        }
        if (drawnCard.getCardName() == "空" && !gamePile.isDiscardPileEmpty(secondPile)) {
            drawnCard = gamePile.drawFromDiscardTop(secondPile);
        }

        if (drawnCard.getCardName() != "空") {
            currPlayer.addHandCard(drawnCard);
            std::cout << "[" << currPlayer.getPlayerName() << "] 打出螃蟹牌对，从弃牌堆抽1张加入手牌！" << std::endl;
        } else {
            std::cout << "[" << currPlayer.getPlayerName() << "] 打出螃蟹牌对，所有弃牌堆仅剩1张/为空，抽牌失败！" << std::endl;
        }
        break;
    }
    case PairSubType::SHIP: {
        // 船牌：重置自己回合（取消跳过状态+重置抽牌机会）
        currPlayer.resetTurnState(); // ✅ 新增：重置回合抽牌标记
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
        // 龙虾+螃蟹：抽5张选1张，其余按顺序放回主牌库底部
        std::vector<Card> draw5 = gamePile.drawFromMain(5);
        if (!draw5.empty()) {
            currPlayer.addHandCard(draw5[0]); // 选第一张
            std::vector<Card> putBack(draw5.begin()+1, draw5.end());
            gamePile.putToMainBottom(putBack);
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
        // 美人鱼事件：优先从主牌库中取得真实美人鱼牌，避免凭空复制卡牌。
        Card mermaidCard = gamePile.takeCollectFromMain(CollectSubType::MERMAID);
        if (mermaidCard.getCardName() != "空") {
            currPlayer.addCollectCard(mermaidCard);
            std::cout << "【美人鱼事件】" << currPlayer.getPlayerName() << "获得1张美人鱼牌！当前累计：" << currPlayer.getMermaidCount() << "张" << std::endl;
        } else {
            std::cout << "【美人鱼事件】牌库中已没有美人鱼牌，本次事件无可获取目标。" << std::endl;
        }
        break;
    }
    case EventSubType::TREASURE: {
        // 沉船宝藏：抽2张牌+得分+1
        currPlayer.addHandCards(gamePile.drawFromMain(2));
        currPlayer.addEventScore(1);
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
    std::unordered_map<CardColor, int> colorCount = ownedColorCount(player);
    int maxCount = 0;
    for (const auto& pair : colorCount) maxCount = std::max(maxCount, pair.second);
    return maxCount;
}

// 私有：切换下一个玩家（顺时针）
void GameRule::nextPlayer() {
    for (int checked = 0; checked < playerNum; ++checked) {
        currentPlayerIndex = (currentPlayerIndex + 1) % playerNum;
        if (players[currentPlayerIndex].isPlayerSkipTurn()) {
            players[currentPlayerIndex].setSkipTurn(false);
            players[currentPlayerIndex].setHasDrawnThisTurn(false);
            std::cout << "[" << players[currentPlayerIndex].getPlayerName() << "] 跳过本回合。" << std::endl;
            continue;
        }
        players[currentPlayerIndex].setHasDrawnThisTurn(false);
        return;
    }
    players[currentPlayerIndex].setHasDrawnThisTurn(false);
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
        // 方式1：主牌库抽2弃1（不足2张时抽1张直接加入）
        std::vector<Card> draw2 = gamePile.drawFromMain(2);
        if (draw2.size() >= 2) {
            currPlayer.addHandCard(draw2[0]);
            gamePile.putToDiscardTop(pileNo, draw2[1]);
            std::cout << "[" << currPlayer.getPlayerName() << "] 从主牌库抽2张，选1张加入手牌，1张放入弃牌堆" << pileNo << std::endl;
        } else if (draw2.size() == 1) {
            // 修复：主牌库只剩1张时，直接加入手牌，不弃牌
            currPlayer.addHandCard(draw2[0]);
            std::cout << "[" << currPlayer.getPlayerName() << "] 主牌库仅剩1张，直接加入手牌！" << std::endl;
        } else {
            std::cout << "[" << currPlayer.getPlayerName() << "] 主牌库已空，抽牌失败！" << std::endl;
        }
    } else if (drawType == 2) {
        // 方式2：从指定弃牌堆顶抽1张加入手牌（原有逻辑不变）
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
// 玩家出牌阶段：打出对牌，触发效果
// 玩家出牌阶段：打出对牌，触发效果
bool GameRule::playCardPhase(Player& currPlayer, const Card& c1, const Card& c2) {
    // 判定是否为有效对牌
    if (!isValidPair(c1, c2)) {
        std::cout << "[" << currPlayer.getPlayerName() << "] 打出的牌不是有效对牌，出牌失败！" << std::endl;
        return false;
    }

    // ✅ 核心修复1：先从手牌移除这两张对牌（彻底杜绝无限打出）
    bool rm1 = currPlayer.removeHandCard(c1);
    bool rm2 = currPlayer.removeHandCard(c2);
    if (!rm1 || !rm2) {
        std::cout << "[" << currPlayer.getPlayerName() << "] 手牌中无此两张牌，出牌失败！" << std::endl;
        return false;
    }

    // ✅ 核心修复2：将移除的对牌存入「对牌打出区」（用于后续增益牌计分）
    currPlayer.addPairToPlayArea(c1, c2);

    // 触发对牌效果（原有逻辑不变）
    triggerPairEffect(c1, c2, currPlayer);

    return true;
}



int GameRule::calcRoundBaseScore(const Player& player) const {
    return player.getRoundScore()
           + player.getEventScore()
           + calcCollectCardScore(player)
           + calcBuffCardScore(player)
           + calcMermaidSingleScore(player);
}

void GameRule::prepareNextRound(int nextStartIndex) {
    // 新一轮必须把所有轮内牌重新洗回主牌库，避免手牌/收集牌/弃牌堆残留导致分数和牌库异常。
    std::vector<std::vector<Card>> carriedLockedCards;
    carriedLockedCards.reserve(players.size());
    for (const auto& p : players) {
        carriedLockedCards.push_back(p.getLockedCards());
    }

    for (auto& p : players) {
        p.resetForNextRound();
    }

    gamePile = CardPile();
    gamePile.initMainPile(initAllGameCards());
    for (const auto& lockedSet : carriedLockedCards) {
        for (const Card& card : lockedSet) {
            gamePile.removeFromMain(card);
        }
    }
    gamePile.shuffleMainPile();

    allEventCards.clear();
    initEventCards();

    if (nextStartIndex >= 0 && nextStartIndex < playerNum) {
        currentPlayerIndex = nextStartIndex;
    } else {
        currentPlayerIndex = 0;
    }

    for (int i = 0; i < playerNum; ++i) {
        Player& p = players[i];
        int carriedCount = 0;
        for (Card card : carriedLockedCards[static_cast<size_t>(i)]) {
            card.unlockCard();
            p.addHandCard(card);
            carriedCount++;
        }
        int drawCount = std::max(0, 2 - carriedCount);
        std::vector<Card> initHand = gamePile.drawFromMain(drawCount);
        p.addHandCards(initHand);
        p.resetTurnState();
        std::cout << "[" << p.getPlayerName() << "] 新一轮起始手牌：" << carriedCount
                  << "张保险箱牌+" << drawCount << "张抽牌！" << std::endl;
    }

    std::vector<Card> firstDiscard = gamePile.drawFromMain(1);
    if (!firstDiscard.empty()) {
        gamePile.putToDiscardTop(1, firstDiscard[0]);
    }
    std::vector<Card> secondDiscard = gamePile.drawFromMain(1);
    if (!secondDiscard.empty()) {
        gamePile.putToDiscardTop(2, secondDiscard[0]);
    }

    std::cout << "[游戏] 下一轮已重新发牌，弃牌堆已初始化。" << std::endl;
}

// 轮次结束阶段：isAllIn=是否放手一搏
void GameRule::roundEnd(bool isAllIn, int allInStarterIndex) {
    std::cout << "========================================" << std::endl;
    std::cout << "[游戏] 本轮结束，开始分数结算！" << std::endl;

    int starterIndex = (allInStarterIndex >= 0 && allInStarterIndex < playerNum)
                           ? allInStarterIndex
                           : currentPlayerIndex;

    std::vector<int> baseScores(playerNum, 0);
    std::vector<int> colorBonuses(playerNum, 0);
    for (int i = 0; i < playerNum; ++i) {
        baseScores[i] = calcRoundBaseScore(players[i]);
        colorBonuses[i] = getColorRewardScore(getMaxColorCount());
    }

    bool allInSuccess = false;
    if (isAllIn) {
        int maxOtherScore = -1;
        for (int i = 0; i < playerNum; ++i) {
            if (i == starterIndex) {
                continue;
            }
            maxOtherScore = std::max(maxOtherScore, baseScores[i]);
        }
        allInSuccess = baseScores[starterIndex] > maxOtherScore;
        std::cout << "[放手一搏] 发起者=" << players[starterIndex].getPlayerName()
                  << "，结果=" << (allInSuccess ? "成功" : "失败") << std::endl;
    }

    for (int i = 0; i < playerNum; ++i) {
        Player& p = players[i];
        int totalRound = baseScores[i];

        if (!isAllIn) {
            // 直接截停：只结算本轮卡面/效果得分，不加入颜色奖励分。
            std::cout << "[" << p.getPlayerName() << "] 直接截停结算：本轮基础分=" << totalRound << std::endl;
        } else if (allInSuccess) {
            totalRound = (i == starterIndex) ? (baseScores[i] + colorBonuses[i]) : colorBonuses[i];
            std::cout << "[" << p.getPlayerName() << "] 放手一搏成功规则结算=" << totalRound << std::endl;
        } else {
            totalRound = (i != starterIndex && baseScores[i] > baseScores[starterIndex])
                             ? (baseScores[i] + colorBonuses[i])
                             : colorBonuses[i];
            std::cout << "[" << p.getPlayerName() << "] 放手一搏失败规则结算=" << totalRound << std::endl;
        }

        p.addTotalScore(totalRound);
    }

    std::cout << "[游戏] 本轮分数结算完成！" << std::endl;
    std::cout << "========================================" << std::endl;

    if (checkGameWin()) {
        isGameOver = true;
        Player winPlayer = getWinPlayer();
        std::cout << "[游戏] 游戏结束！胜利者：" << winPlayer.getPlayerName() << "，总分数：" << winPlayer.getTotalScore() << std::endl;
        return;
    }

    // 下一轮从发起结束轮次玩家的下一位开始；同时把所有轮内卡牌洗回牌库重新发牌。
    prepareNextRound((starterIndex + 1) % playerNum);
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
    // 3. 判定是否可结束轮次（本轮分数>=7）
    if (calcRoundBaseScore(currPlayer) >= 7) {
        std::cout << "[" << currPlayer.getPlayerName() << "] 本轮分数>=7，可选择结束本轮！" << std::endl;
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

// 获取全场最高颜色牌数量（所有玩家中，某颜色的最大持有数）
int GameRule::getMaxColorCount() const {
    int maxCount = 0;
    for (const auto& player : players) {
        std::unordered_map<CardColor, int> colorCount = ownedColorCount(player);
        for (const auto& [color, count] : colorCount) {
            if (count > maxCount) maxCount = count;
        }
    }
    return maxCount;
}

// 颜色数量 → 颜色奖励分（规则未明确映射，默认1:1，可修改为任意规则，如count×2）
int GameRule::getColorRewardScore(int colorCount) const {
    return colorCount; // 示例：最高颜色数量=5 → 颜色奖励分=5
}

// 获取所有玩家的本轮基础分（不含颜色分、不含美人鱼单张计分）
std::vector<int> GameRule::getPlayerRoundScores() const {
    std::vector<int> scores;
    for (const auto& player : players) {
        scores.push_back(player.getRoundScore());
    }
    return scores;
}

// 获取颜色奖励分排名（降序：最多→次高→第三高→第四高）
std::vector<int> GameRule::getColorRewardRankings() const {
    std::vector<int> allColorCounts;
    // 收集所有玩家的所有颜色数量
    for (const auto& player : players) {
        std::unordered_map<CardColor, int> colorCount = ownedColorCount(player);
        for (const auto& [color, count] : colorCount) {
            allColorCounts.push_back(count);
        }
    }
    // 去重+降序排序，取前4名（对应4张美人鱼牌）
    std::sort(allColorCounts.rbegin(), allColorCounts.rend());
    // 去重（保留唯一排名）
    auto last = std::unique(allColorCounts.begin(), allColorCounts.end());
    allColorCounts.erase(last, allColorCounts.end());
    // 补充不足4名的情况（用0填充）
    while (allColorCounts.size() < 4) {
        allColorCounts.push_back(0);
    }
    // 转换为颜色奖励分
    std::vector<int> rewardRankings;
    for (int count : allColorCounts) {
        rewardRankings.push_back(getColorRewardScore(count));
    }
    return rewardRankings;
}

// 美人鱼单张计分（核心修正：第1张=最多颜色奖励分，第2张=次高，依次类推）
int GameRule::calcMermaidSingleScore(const Player& player) const {
    int mermaidCount = player.getMermaidCount();
    if (mermaidCount == 0) return 0;

    std::vector<int> rewardRankings = getColorRewardRankings(); // 颜色奖励分排名
    int total = 0;
    for (int i = 0; i < mermaidCount; ++i) {
        // 取对应排名的颜色奖励分（i=0→最多，i=1→次高...）
        total += rewardRankings[i];
    }
    return total;
}
//收集牌相关积分
int GameRule::calcCollectCardScore(const Player& player) const {
    int total = 0;

    // 1. 统计各类收集牌数量（排除美人鱼）
    int shellCount = 0;     // 贝壳牌
    int octopusCount = 0;   // 章鱼牌
    int penguinCount = 0;   // 企鹅牌
    int anchorCount = 0;    // 船锚牌
    int seahorseCount = 0;  // 海马牌

    for (const auto& card : player.getCollectCards()) {
        CollectSubType cSub = card.getCollectSubType();
        switch (cSub) {
        case CollectSubType::SHELL: shellCount++; break;
        case CollectSubType::OCTOPUS: octopusCount++; break;
        case CollectSubType::PENGUIN: penguinCount++; break;
        case CollectSubType::ANCHOR: anchorCount++; break;
        case CollectSubType::SEAHORSE: seahorseCount++; break;
        case CollectSubType::MERMAID: break; // 美人鱼单独计分
        default: break;
        }
    }

    // 2. 处理海马伪装：选择最优伪装方式，计算最高总得分
    int bestTotal = 0;
    if (seahorseCount == 0) {
        // 无海马：直接计算实际各类得分
        bestTotal = calcShellScore(shellCount)
                    + calcOctopusScore(octopusCount)
                    + calcPenguinScore(penguinCount)
                    + calcAnchorScore(anchorCount);
    } else {
        // ✅ 特殊规则：无任何基础收集牌时，海马无法伪装，得0分
        if (shellCount == 0 && octopusCount == 0 && penguinCount == 0 && anchorCount == 0) {
            bestTotal = 0;
        } else {
            // 尝试将所有海马伪装到每一类，取总得分最高的方案
            // 伪装成贝壳
            {
                int fakeShell = shellCount + seahorseCount;
                int score = calcShellScore(fakeShell)
                            + calcOctopusScore(octopusCount)
                            + calcPenguinScore(penguinCount)
                            + calcAnchorScore(anchorCount);
                bestTotal = std::max(bestTotal, score);
            }
            // 伪装成章鱼
            {
                int fakeOctopus = octopusCount + seahorseCount;
                int score = calcShellScore(shellCount)
                            + calcOctopusScore(fakeOctopus)
                            + calcPenguinScore(penguinCount)
                            + calcAnchorScore(anchorCount);
                bestTotal = std::max(bestTotal, score);
            }
            // 伪装成企鹅
            {
                int fakePenguin = penguinCount + seahorseCount;
                int score = calcShellScore(shellCount)
                            + calcOctopusScore(octopusCount)
                            + calcPenguinScore(fakePenguin)
                            + calcAnchorScore(anchorCount);
                bestTotal = std::max(bestTotal, score);
            }
            // 伪装成船锚
            {
                int fakeAnchor = anchorCount + seahorseCount;
                int score = calcShellScore(shellCount)
                            + calcOctopusScore(octopusCount)
                            + calcPenguinScore(penguinCount)
                            + calcAnchorScore(fakeAnchor);
                bestTotal = std::max(bestTotal, score);
            }
        }
    }

    total = bestTotal;

    // 调试输出（保留便于查看计分逻辑）
    std::cout << "[计分] 贝壳牌：" << shellCount << "张 → +" << calcShellScore(shellCount) << "分\n";
    std::cout << "[计分] 章鱼牌：" << octopusCount << "张 → +" << calcOctopusScore(octopusCount) << "分\n";
    std::cout << "[计分] 企鹅牌：" << penguinCount << "张 → +" << calcPenguinScore(penguinCount) << "分\n";
    std::cout << "[计分] 船锚牌：" << anchorCount << "张 → +" << calcAnchorScore(anchorCount) << "分\n";
    if (seahorseCount > 0) {
        std::cout << "[计分] 海马牌：" << seahorseCount << "张 → 伪装后总得分+" << total << "分\n";
    }

    return total;
}


// 增益牌配合分（修复：基于【对牌打出区】计分，永久生效）
// 增益牌配合分（修复：企鹅群牌适配企鹅收集牌）
// ✅ 修复：增益牌无论打出与否都生效，严格匹配文档规则
int GameRule::calcBuffCardScore(const Player& player) const {
    int total = 0;

    // 1. 统计【手牌+打出区】所有对应对牌的数量（鱼/螃蟹/船）
    std::unordered_map<PairSubType, int> pairTotalCount;
    // 统计手牌里的对牌
    for (const auto& card : player.getHandCards()) {
        if (card.getMainType() == CardType::PAIR_CARD) {
            pairTotalCount[card.getPairSubType()]++;
        }
    }
    // 统计打出区的对牌
    for (const auto& card : player.getPairPlayArea()) {
        if (card.getMainType() == CardType::PAIR_CARD) {
            pairTotalCount[card.getPairSubType()]++;
        }
    }

    // 2. 统计收集牌里的对应数量（企鹅/船锚）
    int penguinCollectCount = 0;
    int anchorCollectCount = 0;
    for (const auto& card : player.getCollectCards()) {
        if (card.getCollectSubType() == CollectSubType::PENGUIN) {
            penguinCollectCount++;
        }
        if (card.getCollectSubType() == CollectSubType::ANCHOR) {
            anchorCollectCount++;
        }
    }

    // 3. 严格按文档规则计算增益分（无论打出与否都生效）
    for (const auto& card : player.getBuffCards()) {
        BuffSubType bSub = card.getBuffSubType();
        int addScore = 0;
        switch (bSub) {
        case BuffSubType::FISH_GROUP:
            // 鱼群牌：每有一张鱼牌 +1分（手牌+打出区都算）
            addScore = pairTotalCount[PairSubType::FISH] * 1;
            std::cout << "[增益分] 鱼群牌：" << pairTotalCount[PairSubType::FISH] << "张鱼牌 → +" << addScore << "分\n";
            break;
        case BuffSubType::CRAB_GROUP:
            // 螃蟹群牌：每有一张螃蟹牌 +1分（手牌+打出区都算）
            addScore = pairTotalCount[PairSubType::CRAB] * 1;
            std::cout << "[增益分] 螃蟹群牌：" << pairTotalCount[PairSubType::CRAB] << "张螃蟹牌 → +" << addScore << "分\n";
            break;
        case BuffSubType::SHIP_GROUP:
            // 船队牌：每有一张船牌 +1分（手牌+打出区都算）
            addScore = pairTotalCount[PairSubType::SHIP] * 1;
            std::cout << "[增益分] 船队牌：" << pairTotalCount[PairSubType::SHIP] << "张船牌 → +" << addScore << "分\n";
            break;
        case BuffSubType::PENGUIN_GROUP:
            // 企鹅群牌：每有一张企鹅收集牌 +2分
            addScore = penguinCollectCount * 2;
            std::cout << "[增益分] 企鹅群牌：" << penguinCollectCount << "张企鹅收集牌 → +" << addScore << "分\n";
            break;
        case BuffSubType::ANCHOR_PILE:
            // 船锚堆牌：每有一张船锚收集牌 +3分
            addScore = anchorCollectCount * 3;
            std::cout << "[增益分] 船锚堆牌：" << anchorCollectCount << "张船锚收集牌 → +" << addScore << "分\n";
            break;
        }
        total += addScore;
    }
    return total;
}

// 数量统计
int GameRule::getShellCount(const Player& player) const {
    int c = 0; for (auto& card : player.getCollectCards()) if (card.getCollectSubType() == CollectSubType::SHELL) c++; return c;
}
int GameRule::getOctopusCount(const Player& player) const {
    int c = 0; for (auto& card : player.getCollectCards()) if (card.getCollectSubType() == CollectSubType::OCTOPUS) c++; return c;
}
int GameRule::getPenguinCount(const Player& player) const {
    int c = 0; for (auto& card : player.getCollectCards()) if (card.getCollectSubType() == CollectSubType::PENGUIN) c++; return c;
}
int GameRule::getAnchorCount(const Player& player) const {
    int c = 0; for (auto& card : player.getCollectCards()) if (card.getCollectSubType() == CollectSubType::ANCHOR) c++; return c;
}
int GameRule::getSeahorseCount(const Player& player) const {
    int c = 0; for (auto& card : player.getCollectCards()) if (card.getCollectSubType() == CollectSubType::SEAHORSE) c++; return c;
}

// 得分统计
int GameRule::getShellScore(const Player& player) const {
    int c = getShellCount(player);
    if (c < 1) return 0;
    int s = (c-1)*2; return c>6 ? 10 : s;
}
int GameRule::getOctopusScore(const Player& player) const {
    int c = getOctopusCount(player);
    if (c < 1) return 0;
    int s = (c-1)*3; return c>5 ? 12 : s;
}
int GameRule::getPenguinScore(const Player& player) const {
    int c = getPenguinCount(player);
    if (c < 1) return 0;
    int s = 2*c-1; return c>3 ? 5 : s;
}
int GameRule::getAnchorScore(const Player& player) const {
    return getAnchorCount(player)>=2 ? 5 : 0;
}

bool GameRule::checkGameWin() {
    // 胜利条件判断代码（和你之前的实现一致即可）
    // 条件1：任意玩家集齐4张美人鱼牌 → 直接胜利
    for (const auto& player : players) {
        if (player.getMermaidCount() >= 4) {
            std::cout << "[胜利条件触发] " << player.getPlayerName() << "集齐4张美人鱼牌！" << std::endl;
            return true;
        }
    }
    // 条件2：任意玩家总分数达到对应人数的胜利分数
    int winScore = WIN_SCORE.at(playerNum);
    for (const auto& player : players) {
        if (player.getTotalScore() >= winScore) {
            std::cout << "[胜利条件触发] " << player.getPlayerName() << "总分达到" << winScore << "分！" << std::endl;
            return true;
        }
    }
    return false;
}

// 贝壳牌计分：0/2/4/6/8/10（1-6张）
int GameRule::calcShellScore(int count) const {
    if (count < 1) return 0;
    int score = (count - 1) * 2;
    return (count > 6) ? 10 : score;
}

// 章鱼牌计分：0/3/6/9/12（1-5张）
int GameRule::calcOctopusScore(int count) const {
    if (count < 1) return 0;
    int score = (count - 1) * 3;
    return (count > 5) ? 12 : score;
}

// 企鹅牌计分：1/3/5（1-3张）
int GameRule::calcPenguinScore(int count) const {
    if (count < 1) return 0;
    int score = 2 * count - 1;
    return (count > 3) ? 5 : score;
}

// 船锚牌计分：0/5（<2张=0，≥2张=5）
int GameRule::calcAnchorScore(int count) const {
    return (count >= 2) ? 5 : 0;
}
