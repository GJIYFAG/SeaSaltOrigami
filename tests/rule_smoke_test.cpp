#include "../Card.h"
#include "../CardPile.h"
#include "../GameRule.h"
#include "../Player.h"

#include <cassert>
#include <vector>

int main()
{
    GameRule rule(2, {"A", "B"});

    Card swimmer(CardType::PAIR_CARD, PairSubType::SWIMMER, CardColor::RED, "swimmer");
    Card shark(CardType::PAIR_CARD, PairSubType::SHARK, CardColor::BLUE, "shark");
    Card jelly(CardType::PAIR_CARD, PairSubType::JELLYFISH, CardColor::GREEN, "jellyfish");
    Card lobster(CardType::PAIR_CARD, PairSubType::LOBSTER, CardColor::YELLOW, "lobster");
    Card crab(CardType::PAIR_CARD, PairSubType::CRAB, CardColor::PURPLE, "crab");
    Card fish1(CardType::PAIR_CARD, PairSubType::FISH, CardColor::RED, "fish1");
    Card fish2(CardType::PAIR_CARD, PairSubType::FISH, CardColor::BLUE, "fish2");

    assert(rule.isValidPair(swimmer, shark));
    assert(rule.isValidPair(swimmer, jelly));
    assert(rule.isValidPair(lobster, crab));
    assert(rule.isValidPair(fish1, fish2));
    assert(!rule.isValidPair(swimmer, swimmer));
    assert(!rule.isValidPair(jelly, jelly));
    assert(!rule.isValidPair(lobster, lobster));

    Player player("tester");
    player.addHandCard(Card(CardType::COLLECT_CARD, CollectSubType::MERMAID, CardColor::RED, "mermaid", 0));
    assert(player.getMermaidCount() == 1);
    assert(player.getCollectCards().size() == 1);

    CardPile pile;
    pile.initMainPile({
        Card(CardType::COLLECT_CARD, CollectSubType::MERMAID, CardColor::RED, "mermaid-main", 0),
        fish1,
    });
    Card taken = pile.takeCollectFromMain(CollectSubType::MERMAID);
    assert(taken.getCardName() == "mermaid-main");
    assert(pile.getMainPileSize() == 1);

    return 0;
}
