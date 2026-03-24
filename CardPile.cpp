#include "CardPile.h"
#include <algorithm>
#include <chrono>

// 构造函数：用系统时间初始化随机数生成器
CardPile::CardPile() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

// 私有：从指定牌堆末尾移除并返回卡牌
Card CardPile::popCardFrom(std::vector<Card>& pile) {
    if (pile.empty()) {
        throw std::runtime_error("牌堆为空，无法抽牌！");
    }
    Card card = pile.back();
    pile.pop_back();
    return card;
}

// 私有：向指定牌堆添加卡牌（末尾）
void CardPile::pushCardTo(std::vector<Card>& pile, const Card& card) {
    pile.push_back(card);
}

// 初始化主牌库
void CardPile::initMainPile(const std::vector<Card>& allCards) {
    mainPile = allCards;
}

// 洗牌主牌库
void CardPile::shuffleMainPile() {
    std::shuffle(mainPile.begin(), mainPile.end(), rng);
    std::cout << "[牌堆] 主牌库洗牌完成！" << std::endl;
}

// 洗牌指定弃牌堆
void CardPile::shuffleDiscardPile(int pileNo) {
    if (pileNo == 1) {
        std::shuffle(discardPile1.begin(), discardPile1.end(), rng);
        std::cout << "[牌堆] 弃牌堆1洗牌完成！" << std::endl;
    } else if (pileNo == 2) {
        std::shuffle(discardPile2.begin(), discardPile2.end(), rng);
        std::cout << "[牌堆] 弃牌堆2洗牌完成！" << std::endl;
    } else {
        std::cout << "[牌堆] 无效的弃牌堆编号！" << std::endl;
    }
}

// 从主牌库抽N张牌
std::vector<Card> CardPile::drawFromMain(int n) {
    std::vector<Card> cards;
    for (int i = 0; i < n; ++i) {
        if (isMainPileEmpty()) break;
        cards.push_back(popCardFrom(mainPile));
    }
    return cards;
}

// 从指定弃牌堆顶抽1张
Card CardPile::drawFromDiscardTop(int pileNo) {
    if (pileNo == 1) {
        return popCardFrom(discardPile1);
    } else if (pileNo == 2) {
        return popCardFrom(discardPile2);
    } else {
        throw std::runtime_error("无效的弃牌堆编号！");
    }
}

// 向指定弃牌堆顶放1张牌
void CardPile::putToDiscardTop(int pileNo, const Card& card) {
    if (pileNo == 1) {
        pushCardTo(discardPile1, card);
    } else if (pileNo == 2) {
        pushCardTo(discardPile2, card);
    } else {
        std::cout << "[牌堆] 无效的弃牌堆编号！" << std::endl;
    }
}

// 向指定弃牌堆添加多张牌
void CardPile::putToDiscard(int pileNo, const std::vector<Card>& cards) {
    for (const auto& card : cards) {
        putToDiscardTop(pileNo, card);
    }
}

// 牌库状态查询：主牌库大小
int CardPile::getMainPileSize() const { return mainPile.size(); }
// 指定弃牌堆大小
int CardPile::getDiscardPileSize(int pileNo) const {
    return (pileNo == 1) ? discardPile1.size() : discardPile2.size();
}
// 主牌库是否为空
bool CardPile::isMainPileEmpty() const { return mainPile.empty(); }
// 指定弃牌堆是否为空
bool CardPile::isDiscardPileEmpty(int pileNo) const {
    return (pileNo == 1) ? discardPile1.empty() : discardPile2.empty();
}

// 控制台打印指定牌堆
void CardPile::printPile(int pileNo) const {
    const std::vector<Card>* pile = nullptr;
    std::string pileName;
    if (pileNo == 0) { pile = &mainPile; pileName = "主牌库"; }
    else if (pileNo == 1) { pile = &discardPile1; pileName = "弃牌堆1"; }
    else if (pileNo == 2) { pile = &discardPile2; pileName = "弃牌堆2"; }
    else { std::cout << "[牌堆] 无效的牌堆编号！" << std::endl; return; }

    std::cout << "========== " << pileName << " (共" << pile->size() << "张) ==========" << std::endl;
    if (pile->empty()) {
        std::cout << "【空】" << std::endl;
        return;
    }
    for (size_t i = 0; i < pile->size(); ++i) {
        const auto& card = (*pile)[i];
        std::cout << i+1 << ". " << getCardTypeStr(card.getMainType()) << " - "
                  << card.getCardName() << " (" << getCardColorStr(card.getColor()) << ")" << std::endl;
    }
    std::cout << "========================================" << std::endl;
}
