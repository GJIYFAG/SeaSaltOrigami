#include "SeaSaltGameController.h"

#include <QAbstractButton>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include <QString>
#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace {

QWidget *dialogParent()
{
    return nullptr;
}

constexpr int ROUND_END_THRESHOLD = 10;

} // namespace

SeaSaltGameController::SeaSaltGameController(QObject *parent)
    : QObject(parent)
{
    turnTimer = new QTimer(this);
    turnTimer->setInterval(1000);
    connect(turnTimer, &QTimer::timeout, this, &SeaSaltGameController::onTurnTimeOut);
    m_windowTitleText = QStringLiteral("海盐折纸");
}

SeaSaltGameController::~SeaSaltGameController()
{
    delete gameRule;
}

void SeaSaltGameController::startGame(bool singlePlayer)
{
    if (gameRule) {
        delete gameRule;
        gameRule = nullptr;
    }
    m_aiScheduled = false;
    m_allInPending = false;
    m_allInStarterIndex = -1;
    m_allInTurnsRemaining = 0;
    m_singlePlayerMode = singlePlayer;
    const std::vector<std::string> names = singlePlayer ? std::vector<std::string>{"你", "电脑"}
                                                        : std::vector<std::string>{"玩家1", "玩家2"};
    gameRule = new GameRule(2, names);
    gameRule->gameStart();
    clearPendingDraw();
    dismissToast();
    m_gameActive = true;
    turnRemainTime = 30;
    emit gameActiveChanged();
    emit singlePlayerModeChanged();
    refreshAllUI();
    refreshTurnTimer();
    scheduleAiIfComputerTurn();
}

void SeaSaltGameController::returnToMenu()
{
    turnTimer->stop();
    m_aiScheduled = false;
    m_allInPending = false;
    m_allInStarterIndex = -1;
    m_allInTurnsRemaining = 0;
    clearPendingDraw();
    dismissToast();
    if (gameRule) {
        delete gameRule;
        gameRule = nullptr;
    }
    m_gameActive = false;
    m_computerTurn = false;
    m_singlePlayerMode = false;
    m_scoreText.clear();
    m_playerText.clear();
    m_handCardNames.clear();
    m_handSelected.clear();
    m_collectCardLines.clear();
    m_buffCardNames.clear();
    m_playedPairCardNames.clear();
    emit gameActiveChanged();
    emit singlePlayerModeChanged();
    emit stateChanged();
    m_windowTitleText = QStringLiteral("海盐折纸");
    emit titleChanged();
}

bool SeaSaltGameController::currentPlayerIsComputer() const
{
    if (!gameRule) {
        return false;
    }
    return QString::fromStdString(gameRule->getCurrentPlayer().getPlayerName()) == QStringLiteral("电脑");
}

void SeaSaltGameController::updateComputerTurnFlag()
{
    const bool next = m_gameActive && m_singlePlayerMode && currentPlayerIsComputer();
    if (m_computerTurn != next) {
        m_computerTurn = next;
    }
}

void SeaSaltGameController::scheduleAiIfComputerTurn()
{
    if (!m_gameActive || !m_singlePlayerMode || !gameRule || m_aiScheduled) {
        return;
    }
    if (!currentPlayerIsComputer()) {
        return;
    }
    m_aiScheduled = true;
    turnTimer->stop();
    QTimer::singleShot(650, this, [this]() {
        m_aiScheduled = false;
        aiTakeTurnStep();
    });
}

void SeaSaltGameController::aiDrawRandomMain()
{
    Player &currPlayer = gameRule->getCurrentPlayer();
    CardPile &pile = gameRule->getGamePile();
    if (currPlayer.getHasDrawnThisTurn() || pile.getMainPileSize() < 2) {
        return;
    }
    std::vector<Card> draw2 = pile.drawFromMain(2);
    if (draw2.size() != 2) {
        return;
    }
    const int keepIdx = QRandomGenerator::global()->bounded(2);
    Card keepCard = draw2[static_cast<size_t>(keepIdx)];
    Card discardCard = draw2[static_cast<size_t>(1 - keepIdx)];
    const int targetPileNo = 1 + QRandomGenerator::global()->bounded(2);
    currPlayer.addHandCard(keepCard);
    pile.putToDiscardTop(targetPileNo, discardCard);
    currPlayer.setHasDrawnThisTurn(true);
}

bool SeaSaltGameController::aiTryDrawDiscard(int pileNo)
{
    Player &currPlayer = gameRule->getCurrentPlayer();
    if (currPlayer.getHasDrawnThisTurn()) {
        return false;
    }
    CardPile &pile = gameRule->getGamePile();
    Card c = pile.drawFromDiscardTop(pileNo);
    if (c.getCardName() == "空") {
        return false;
    }
    currPlayer.addHandCard(c);
    currPlayer.setHasDrawnThisTurn(true);
    return true;
}

bool SeaSaltGameController::aiTryPlayOnePair(bool silentTips)
{
    Player &currPlayer = gameRule->getCurrentPlayer();
    const std::vector<Card> &hand = currPlayer.getHandCards();
    for (size_t i = 0; i < hand.size(); ++i) {
        for (size_t j = i + 1; j < hand.size(); ++j) {
            Card c1 = hand[i];
            Card c2 = hand[j];
            if (!gameRule->isValidPair(c1, c2)) {
                continue;
            }
            if (!gameRule->playCardPhase(currPlayer, c1, c2)) {
                continue;
            }
            showPairEffectTip(c1, c2, silentTips);
            if (currPlayer.getMermaidCount() > 0) {
                showMermaidGainTip(currPlayer, currPlayer.getMermaidCount(), silentTips);
            }
            clearSelected();
            return true;
        }
    }
    return false;
}

void SeaSaltGameController::aiTakeTurnStep()
{
    if (!gameRule || !currentPlayerIsComputer()) {
        return;
    }

    Player &currPlayer = gameRule->getCurrentPlayer();

    if (!currPlayer.getHasDrawnThisTurn()) {
        CardPile &pile = gameRule->getGamePile();
        if (pile.getMainPileSize() >= 2) {
            aiDrawRandomMain();
        } else if (pile.discard1Count() > 1) {
            aiTryDrawDiscard(1);
        } else if (pile.discard2Count() > 1) {
            aiTryDrawDiscard(2);
        } else {
            completeCurrentTurn();
            return;
        }
        refreshAllUI();
        QTimer::singleShot(400, this, [this]() {
            if (!gameRule || !currentPlayerIsComputer()) {
                return;
            }
            if (aiTryPlayOnePair(true)) {
                refreshAllUI();
                if (gameRule->checkGameWin()) {
                    const Player winner = gameRule->getWinPlayer();
                    const QString winReason = (winner.getMermaidCount() >= 4)
                                                  ? QStringLiteral("集齐4张美人鱼牌")
                                                  : QStringLiteral("总分数达到%1分")
                                                        .arg(gameRule->getPlayerNum() == 2
                                                                 ? 40
                                                                 : (gameRule->getPlayerNum() == 3 ? 35 : 30));
                    QMessageBox::information(
                        dialogParent(), QStringLiteral("🎉 游戏胜利"),
                        QStringLiteral("%1 获得胜利！\n胜利原因：%2\n最终总分数：%3")
                            .arg(QString::fromStdString(winner.getPlayerName()), winReason)
                            .arg(winner.getTotalScore()));
                    returnToMenu();
                    return;
                }
                if (!m_allInPending && currentRoundUiScore(gameRule->getCurrentPlayer()) >= ROUND_END_THRESHOLD) {
                    settleRound(false, gameRule->getCurrentPlayerIndex());
                    return;
                }
                scheduleAiIfComputerTurn();
                return;
            }
            completeCurrentTurn();
        });
        return;
    }

    if (aiTryPlayOnePair(true)) {
        refreshAllUI();
        if (gameRule->checkGameWin()) {
            const Player winner = gameRule->getWinPlayer();
            const QString winReason = (winner.getMermaidCount() >= 4)
                                          ? QStringLiteral("集齐4张美人鱼牌")
                                          : QStringLiteral("总分数达到%1分")
                                                .arg(gameRule->getPlayerNum() == 2
                                                         ? 40
                                                         : (gameRule->getPlayerNum() == 3 ? 35 : 30));
            QMessageBox::information(
                dialogParent(), QStringLiteral("🎉 游戏胜利"),
                QStringLiteral("%1 获得胜利！\n胜利原因：%2\n最终总分数：%3")
                    .arg(QString::fromStdString(winner.getPlayerName()), winReason)
                    .arg(winner.getTotalScore()));
            returnToMenu();
            return;
        }
        if (!m_allInPending && currentRoundUiScore(gameRule->getCurrentPlayer()) >= ROUND_END_THRESHOLD) {
            settleRound(false, gameRule->getCurrentPlayerIndex());
            return;
        }
        scheduleAiIfComputerTurn();
        return;
    }

    completeCurrentTurn();
}

int SeaSaltGameController::currentRoundUiScore(Player &player) const
{
    if (!gameRule) {
        return 0;
    }
    return gameRule->calcRoundBaseScorePublic(player);
}

void SeaSaltGameController::settleRound(bool isAllIn, int starterIndex)
{
    if (!gameRule) {
        return;
    }
    m_allInPending = false;
    m_allInStarterIndex = -1;
    m_allInTurnsRemaining = 0;
    clearPendingDraw();
    clearSelected();

    gameRule->roundEnd(isAllIn, starterIndex);
    refreshAllUI();
    refreshTurnTimer();

    if (gameRule->isGameOverFlag()) {
        Player winner = gameRule->getWinPlayer();
        QString winReason = (winner.getMermaidCount() >= 4)
                                ? QStringLiteral("集齐4张美人鱼牌")
                                : QStringLiteral("总分数达到%1分")
                                      .arg(gameRule->getPlayerNum() == 2
                                               ? 40
                                               : (gameRule->getPlayerNum() == 3 ? 35 : 30));
        QMessageBox::information(
            dialogParent(), QStringLiteral("🎉 游戏胜利"),
            QStringLiteral("%1 获得胜利！\n胜利原因：%2\n最终总分数：%3")
                .arg(QString::fromStdString(winner.getPlayerName()), winReason)
                .arg(winner.getTotalScore()));
        returnToMenu();
        return;
    }

    scheduleAiIfComputerTurn();
}

void SeaSaltGameController::completeCurrentTurn()
{
    if (!gameRule) {
        return;
    }

    const int endingIndex = gameRule->getCurrentPlayerIndex();
    gameRule->endCurrentTurn();
    clearSelected();

    if (m_allInPending && endingIndex != m_allInStarterIndex) {
        m_allInTurnsRemaining = std::max(0, m_allInTurnsRemaining - 1);
        if (m_allInTurnsRemaining == 0) {
            settleRound(true, m_allInStarterIndex);
            return;
        }
    }

    refreshAllUI();
    refreshTurnTimer();
    scheduleAiIfComputerTurn();
}

void SeaSaltGameController::clearSelected()
{
    m_selectedHandIndices.clear();
    rebuildHandSelectionFlags();
}

void SeaSaltGameController::rebuildPendingDrawNames()
{
    m_pendingDrawCardNames.clear();
    for (const Card &card : m_pendingDrawCards) {
        m_pendingDrawCardNames.append(QString::fromStdString(card.getCardName()));
    }
}

void SeaSaltGameController::clearPendingDraw()
{
    m_pendingDrawCards.clear();
    m_pendingDrawCardNames.clear();
    m_pendingKeepIndex = 0;
    m_pendingDrawActive = false;
}

void SeaSaltGameController::showToast(const QString &message, int timeoutMs)
{
    m_toastText = message;
    m_toastVisible = true;
    emit stateChanged();
    QTimer::singleShot(timeoutMs, this, [this, message]() {
        if (m_toastText == message) {
            m_toastVisible = false;
            emit stateChanged();
        }
    });
}

void SeaSaltGameController::dismissToast()
{
    m_toastVisible = false;
    m_toastText.clear();
    emit stateChanged();
}

void SeaSaltGameController::selectPendingKeep(int index)
{
    if (!m_pendingDrawActive || currentPlayerIsComputer()) {
        return;
    }
    if (index < 0 || index >= static_cast<int>(m_pendingDrawCards.size())) {
        return;
    }
    m_pendingKeepIndex = index;
    emit stateChanged();
}

void SeaSaltGameController::finishPendingMainDraw(int discardPileNo)
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (!m_pendingDrawActive || m_pendingDrawCards.size() != 2) {
        showToast(QStringLiteral("当前没有待处理的抽2弃1。"));
        return;
    }
    if (discardPileNo != 1 && discardPileNo != 2) {
        showToast(QStringLiteral("请选择弃牌堆1或弃牌堆2。"));
        return;
    }

    Player &currPlayer = gameRule->getCurrentPlayer();
    if (currPlayer.getHasDrawnThisTurn()) {
        clearPendingDraw();
        showToast(QStringLiteral("本回合已完成抽牌。"));
        refreshAllUI();
        return;
    }

    const int keepIndex = (m_pendingKeepIndex == 1) ? 1 : 0;
    const int discardIndex = 1 - keepIndex;
    Card keepCard = m_pendingDrawCards[static_cast<size_t>(keepIndex)];
    Card discardCard = m_pendingDrawCards[static_cast<size_t>(discardIndex)];

    currPlayer.addHandCard(keepCard);
    gameRule->getGamePile().putToDiscardTop(discardPileNo, discardCard);
    currPlayer.setHasDrawnThisTurn(true);

    emit drawAnimationRequested(QStringLiteral("pending"), QString::fromStdString(keepCard.getCardName()));
    emit discardAnimationRequested(discardPileNo == 1 ? QStringLiteral("discard1") : QStringLiteral("discard2"),
                                   QString::fromStdString(discardCard.getCardName()));
    clearPendingDraw();
    showToast(QStringLiteral("保留 %1，弃置 %2 到弃牌堆%3。")
                  .arg(QString::fromStdString(keepCard.getCardName()),
                       QString::fromStdString(discardCard.getCardName()))
                  .arg(discardPileNo));
    refreshAllUI();
    refreshTurnTimer();
}

void SeaSaltGameController::rebuildHandSelectionFlags()
{
    m_handSelected.clear();
    for (int i = 0; i < m_handCardNames.size(); ++i) {
        const bool sel = std::find(m_selectedHandIndices.begin(), m_selectedHandIndices.end(), i)
                         != m_selectedHandIndices.end();
        m_handSelected.append(sel);
    }
}

Card SeaSaltGameController::cardAtHandIndex(int index) const
{
    if (!gameRule) {
        return Card();
    }
    Player &currPlayer = gameRule->getCurrentPlayer();
    const std::vector<Card> &handCards = currPlayer.getHandCards();
    if (index < 0 || index >= static_cast<int>(handCards.size())) {
        return Card();
    }
    return handCards[static_cast<size_t>(index)];
}

void SeaSaltGameController::refreshAllUI()
{
    if (!gameRule) {
        emit stateChanged();
        return;
    }
    Player &currPlayer = gameRule->getCurrentPlayer();
    const std::vector<Card> &handCards = currPlayer.getHandCards();

    clearSelected();
    m_handCardNames.clear();
    for (const Card &card : handCards) {
        m_handCardNames.append(QString::fromStdString(card.getCardName()));
    }
    rebuildHandSelectionFlags();

    CardPile &pile = gameRule->getGamePile();
    m_mainPileText = QStringLiteral("主牌库\n%1").arg(pile.getMainPileSize());
    m_discard1Text = QStringLiteral("弃牌堆1\n%1张\n%2")
                         .arg(pile.discard1Count())
                         .arg(QString::fromStdString(pile.getDiscard1TopCardName()));
    m_discard2Text = QStringLiteral("弃牌堆2\n%1张\n%2")
                         .arg(pile.discard2Count())
                         .arg(QString::fromStdString(pile.getDiscard2TopCardName()));
    m_drawDiscard1Enabled = pile.discard1Count() > 0;
    m_drawDiscard2Enabled = pile.discard2Count() > 0;

    m_collectCardLines.clear();
    for (const Card &card : currPlayer.getCollectCards()) {
        m_collectCardLines.append(QString::fromStdString(card.getCardName()) + "\n+"
                                  + QString::number(card.getScore()) + QStringLiteral("分"));
    }

    m_buffCardNames.clear();
    for (const Card &card : currPlayer.getBuffCards()) {
        m_buffCardNames.append(QString::fromStdString(card.getCardName()));
    }

    m_playedPairCardNames.clear();
    for (const Card &card : currPlayer.getPairPlayArea()) {
        m_playedPairCardNames.append(QString::fromStdString(card.getCardName()));
    }

    int pairScore = currPlayer.getRoundScore();
    int eventScore = currPlayer.getEventScore();
    int collectTotal = gameRule->calcCollectCardScorePublic(currPlayer);
    int buffScore = gameRule->calcBuffCardScorePublic(currPlayer);
    int mermaidScore = gameRule->calcMermaidSingleScorePublic(currPlayer);
    int uiShowTotalRound = pairScore + eventScore + collectTotal + buffScore + mermaidScore;

    m_scoreText = QStringLiteral(
                      "🏆 总分：%1\n"
                      "🔥 本轮分：%2 / %8\n"
                      "- 对牌得分：%3分\n"
                      "- 事件牌得分：%4分\n"
                      "- 收集牌总分：%5分\n"
                      "- 增益牌：%6分\n"
                      "- 美人鱼：%7分\n"
                      "🎨 颜色分：轮次后结算")
                      .arg(currPlayer.getTotalScore())
                      .arg(uiShowTotalRound)
                      .arg(pairScore)
                      .arg(eventScore)
                      .arg(collectTotal)
                      .arg(buffScore)
                      .arg(mermaidScore)
                      .arg(ROUND_END_THRESHOLD);

    m_roundEndVisible = !m_allInPending && uiShowTotalRound >= ROUND_END_THRESHOLD;
    m_roundEndHighlight = m_roundEndVisible;

    QString currentName = QString::fromStdString(currPlayer.getPlayerName());
    if (m_allInPending) {
        m_playerText = QStringLiteral("当前回合：%1（放手一搏最终回合 %2）")
                           .arg(currentName)
                           .arg(m_allInTurnsRemaining);
    } else {
        m_playerText = QStringLiteral("当前回合：%1").arg(currentName);
    }

    m_windowTitleText = QStringLiteral("海盐折纸卡牌游戏 | 剩余回合时间：%1 秒").arg(turnRemainTime);
    emit titleChanged();

    updateComputerTurnFlag();
    emit stateChanged();
    scheduleAiIfComputerTurn();
}

void SeaSaltGameController::toggleHandCard(int index)
{
    if (!gameRule || currentPlayerIsComputer() || hasPendingDraw()) {
        return;
    }
    if (index < 0 || index >= m_handCardNames.size()) {
        return;
    }
    auto it = std::find(m_selectedHandIndices.begin(), m_selectedHandIndices.end(), index);
    if (it != m_selectedHandIndices.end()) {
        m_selectedHandIndices.erase(it);
    } else {
        if (m_selectedHandIndices.size() >= 2) {
            // 玩家点击第三张牌时，按“先选先取消”的方式替换第一张选择。
            m_selectedHandIndices.erase(m_selectedHandIndices.begin());
        }
        m_selectedHandIndices.push_back(index);
    }
    rebuildHandSelectionFlags();
    emit stateChanged();
}


void SeaSaltGameController::moveHandCard(int fromIndex, int toIndex)
{
    if (!gameRule || currentPlayerIsComputer() || hasPendingDraw()) {
        return;
    }
    Player &currPlayer = gameRule->getCurrentPlayer();
    if (currPlayer.moveHandCard(fromIndex, toIndex)) {
        clearSelected();
        refreshAllUI();
    }
}

void SeaSaltGameController::drawMain()
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (hasPendingDraw()) {
        showToast(QStringLiteral("请先完成当前的抽2弃1：选1张保留，再选择弃牌堆。"));
        return;
    }

    Player &currPlayer = gameRule->getCurrentPlayer();
    CardPile &pile = gameRule->getGamePile();

    if (currPlayer.getHasDrawnThisTurn()) {
        showToast(QStringLiteral("本回合已完成抽牌；打出船牌可重置抽牌机会。"));
        return;
    }
    if (pile.getMainPileSize() <= 0) {
        showToast(QStringLiteral("主牌库已空，无法抽牌。"));
        return;
    }

    m_pendingDrawCards = pile.drawFromMain(2);
    if (m_pendingDrawCards.empty()) {
        clearPendingDraw();
        showToast(QStringLiteral("主牌库抽牌异常。"));
        return;
    }
    if (m_pendingDrawCards.size() == 1) {
        Card onlyCard = m_pendingDrawCards[0];
        currPlayer.addHandCard(onlyCard);
        currPlayer.setHasDrawnThisTurn(true);
        emit drawAnimationRequested(QStringLiteral("main"), QString::fromStdString(onlyCard.getCardName()));
        clearPendingDraw();
        showToast(QStringLiteral("主牌库仅剩1张，已直接加入手牌。"));
        refreshAllUI();
        refreshTurnTimer();
        return;
    }

    m_pendingDrawActive = true;
    m_pendingKeepIndex = 0;
    rebuildPendingDrawNames();
    showToast(QStringLiteral("抽到2张牌：请选择保留牌，再点击弃牌堆1或弃牌堆2完成弃置。"), 3200);
    if (m_pendingDrawCardNames.size() == 2) {
        emit pendingDrawAnimationRequested(m_pendingDrawCardNames[0], m_pendingDrawCardNames[1]);
    }
    emit stateChanged();
}

void SeaSaltGameController::drawDiscard1()
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (hasPendingDraw()) {
        showToast(QStringLiteral("请先完成主牌库抽2弃1。"));
        return;
    }
    Player &currPlayer = gameRule->getCurrentPlayer();
    if (currPlayer.getHasDrawnThisTurn()) {
        showToast(QStringLiteral("本回合已完成抽牌；打出船牌可重置抽牌机会。"));
        return;
    }
    CardPile &pile = gameRule->getGamePile();
    Card c = pile.drawFromDiscardTop(1);
    if (c.getCardName() == "空") {
        showToast(QStringLiteral("弃牌堆1为空，不可抽取。"));
        return;
    }
    currPlayer.addHandCard(c);
    currPlayer.setHasDrawnThisTurn(true);
    emit drawAnimationRequested(QStringLiteral("discard1"), QString::fromStdString(c.getCardName()));
    showToast(QStringLiteral("从弃牌堆1拿取 %1。")
                  .arg(QString::fromStdString(c.getCardName())));
    refreshAllUI();
    refreshTurnTimer();
}

void SeaSaltGameController::drawDiscard2()
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (hasPendingDraw()) {
        showToast(QStringLiteral("请先完成主牌库抽2弃1。"));
        return;
    }
    Player &currPlayer = gameRule->getCurrentPlayer();
    if (currPlayer.getHasDrawnThisTurn()) {
        showToast(QStringLiteral("本回合已完成抽牌；打出船牌可重置抽牌机会。"));
        return;
    }
    CardPile &pile = gameRule->getGamePile();
    Card c = pile.drawFromDiscardTop(2);
    if (c.getCardName() == "空") {
        showToast(QStringLiteral("弃牌堆2为空，不可抽取。"));
        return;
    }
    currPlayer.addHandCard(c);
    currPlayer.setHasDrawnThisTurn(true);
    emit drawAnimationRequested(QStringLiteral("discard2"), QString::fromStdString(c.getCardName()));
    showToast(QStringLiteral("从弃牌堆2拿取 %1。")
                  .arg(QString::fromStdString(c.getCardName())));
    refreshAllUI();
    refreshTurnTimer();
}

void SeaSaltGameController::play()
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (hasPendingDraw()) {
        showToast(QStringLiteral("请先完成主牌库抽2弃1。"));
        return;
    }
    Player &currPlayer = gameRule->getCurrentPlayer();

    if (m_selectedHandIndices.size() != 2) {
        QMessageBox::warning(dialogParent(), QStringLiteral("出牌失败"),
                             QStringLiteral("请选择【2张卡牌】后再出牌！\n点击已选中卡牌可取消选择"));
        return;
    }

    const int i0 = m_selectedHandIndices[0];
    const int i1 = m_selectedHandIndices[1];
    Card c1 = cardAtHandIndex(i0);
    Card c2 = cardAtHandIndex(i1);

    if (!gameRule->isValidPair(c1, c2)) {
        QString pairName1 = QString::fromStdString(c1.getCardName());
        QString pairName2 = QString::fromStdString(c2.getCardName());
        QMessageBox::critical(dialogParent(), QStringLiteral("出牌失败"),
                              QStringLiteral("所选卡牌不是有效对牌！\n"
                                             "有效对牌规则：\n"
                                             "1. 两张都是对牌且子类型相同\n"
                                             "2. 游泳者+鲨鱼\n"
                                             "3. 游泳者+水母\n"
                                             "4. 龙虾+螃蟹\n"
                                             "当前选择：%1 + %2")
                                  .arg(pairName1, pairName2));
        clearSelected();
        emit stateChanged();
        return;
    }

    bool playOk = gameRule->playCardPhase(currPlayer, c1, c2);
    if (!playOk) {
        QMessageBox::warning(dialogParent(), QStringLiteral("出牌失败"),
                             QStringLiteral("手牌中无此两张牌，出牌失败！"));
        clearSelected();
        refreshAllUI();
        refreshTurnTimer();
        return;
    }

    emit zoneAnimationRequested(QStringLiteral("hand"), QStringLiteral("played"), QString::fromStdString(c1.getCardName()));
    emit zoneAnimationRequested(QStringLiteral("hand"), QStringLiteral("played"), QString::fromStdString(c2.getCardName()));

    showPairEffectTip(c1, c2);

    if (currPlayer.getMermaidCount() > 0) {
        showMermaidGainTip(currPlayer, currPlayer.getMermaidCount());
    }

    clearSelected();
    refreshAllUI();

    if (gameRule->checkGameWin()) {
        Player winner = gameRule->getWinPlayer();
        QString winReason = (winner.getMermaidCount() >= 4)
                                ? QStringLiteral("集齐4张美人鱼牌")
                                : QStringLiteral("总分数达到%1分")
                                      .arg(gameRule->getPlayerNum() == 2
                                               ? 40
                                               : (gameRule->getPlayerNum() == 3 ? 35 : 30));
        QMessageBox::information(
            dialogParent(), QStringLiteral("🎉 游戏胜利"),
            QStringLiteral("%1 获得胜利！\n胜利原因：%2\n最终总分数：%3")
                .arg(QString::fromStdString(winner.getPlayerName()), winReason)
                .arg(winner.getTotalScore()));
        returnToMenu();
    }
}

void SeaSaltGameController::endTurn()
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (hasPendingDraw()) {
        showToast(QStringLiteral("请先完成主牌库抽2弃1，再结束回合。"));
        return;
    }
    completeCurrentTurn();
}

void SeaSaltGameController::endRound()
{
    if (!gameRule || currentPlayerIsComputer()) {
        return;
    }
    if (hasPendingDraw()) {
        showToast(QStringLiteral("请先完成主牌库抽2弃1，再结束本轮。"));
        return;
    }
    if (m_allInPending) {
        showToast(QStringLiteral("放手一搏已经开始，请等待其他玩家完成最终回合。"));
        return;
    }

    Player &currPlayer = gameRule->getCurrentPlayer();
    int totalRoundScore = currentRoundUiScore(currPlayer);

    if (totalRoundScore < ROUND_END_THRESHOLD) {
        QMessageBox::warning(dialogParent(), QStringLiteral("操作失败"),
                             QStringLiteral("本轮总分不足%1分，无结束本轮资格！")
                                 .arg(ROUND_END_THRESHOLD));
        return;
    }

    QMessageBox msgBox(QMessageBox::Question, QStringLiteral("结束本轮"),
                       QStringLiteral(
                           "请选择结束方式：\n"
                           "1. 直接截停 → 立即结算，本轮不加入颜色奖励分。\n"
                           "2. 放手一搏 → 你的回合立即结束，其他玩家各完成1个回合后结算：\n"
                           "   - 若你最终仍高于所有其他玩家：你获得基础分+颜色奖励，其他玩家仅获得颜色奖励。\n"
                           "   - 若有人追平或超过你：你仅获得颜色奖励，其他玩家获得基础分+颜色奖励。"),
                       QMessageBox::NoButton, dialogParent());

    QAbstractButton *btnStop = msgBox.addButton(QStringLiteral("直接截停"), QMessageBox::YesRole);
    QAbstractButton *btnAllIn = msgBox.addButton(QStringLiteral("放手一搏"), QMessageBox::NoRole);
    msgBox.exec();

    if (msgBox.clickedButton() == btnAllIn) {
        m_allInPending = true;
        m_allInStarterIndex = gameRule->getCurrentPlayerIndex();
        m_allInTurnsRemaining = std::max(0, gameRule->getPlayerNum() - 1);
        showToast(QStringLiteral("已选择放手一搏：你的回合已结束，其他玩家完成最终回合后自动结算。"), 3800);
        gameRule->endCurrentTurn();
        clearSelected();
        refreshAllUI();
        refreshTurnTimer();
        scheduleAiIfComputerTurn();
        return;
    }

    if (msgBox.clickedButton() == btnStop) {
        showToast(QStringLiteral("已直接截停：立即结算并开始下一轮。"), 2800);
        settleRound(false, gameRule->getCurrentPlayerIndex());
    }
}

void SeaSaltGameController::showPairEffectTip(const Card &c1, const Card &c2, bool silent)
{
    if (silent) {
        return;
    }
    QString pairName1 = QString::fromStdString(c1.getCardName());
    QString pairName2 = QString::fromStdString(c2.getCardName());
    QString effectTip = QStringLiteral("✅ 出牌成功！\n对牌：%1 + %2\n\n效果：").arg(pairName1, pairName2);

    if ((c1.getPairSubType() == PairSubType::SWIMMER && c2.getPairSubType() == PairSubType::SHARK)
        || (c1.getPairSubType() == PairSubType::SHARK && c2.getPairSubType() == PairSubType::SWIMMER)) {
        effectTip += QStringLiteral("偷取其他玩家1张手牌！");
    } else if ((c1.getPairSubType() == PairSubType::SWIMMER && c2.getPairSubType() == PairSubType::JELLYFISH)
               || (c1.getPairSubType() == PairSubType::JELLYFISH && c2.getPairSubType() == PairSubType::SWIMMER)) {
        effectTip += QStringLiteral("麻痹其他玩家，使其跳过下一回合！");
    } else if ((c1.getPairSubType() == PairSubType::LOBSTER && c2.getPairSubType() == PairSubType::CRAB)
               || (c1.getPairSubType() == PairSubType::CRAB && c2.getPairSubType() == PairSubType::LOBSTER)) {
        effectTip += QStringLiteral("抽5张牌选1张，其余放回主牌库！");
    } else {
        PairSubType pType = c1.getPairSubType();
        switch (pType) {
        case PairSubType::FISH:
            effectTip += QStringLiteral("从主牌库抽1张牌加入手牌！");
            break;
        case PairSubType::CRAB:
            effectTip += QStringLiteral("从弃牌堆抽1张牌加入手牌（仅剩1张则失败）！");
            break;
        case PairSubType::SHIP:
            effectTip += QStringLiteral("重置自身回合，取消被跳过状态！");
            break;
        case PairSubType::JELLYFISH:
            effectTip += QStringLiteral("需与游泳者牌组合，麻痹其他玩家。");
            break;
        case PairSubType::TURTLE:
            effectTip += QStringLiteral("可抵消任意玩家的对牌效果！");
            break;
        case PairSubType::QUESTION:
            effectTip += QStringLiteral("触发随机事件牌效果（控制台可查看详情）！");
            break;
        default:
            effectTip += QStringLiteral("对牌效果已触发！");
            break;
        }
    }

    QMessageBox::information(dialogParent(), QStringLiteral("对牌效果"), effectTip);
}

void SeaSaltGameController::showMermaidGainTip(Player &player, int mermaidCount, bool silent)
{
    if (silent) {
        return;
    }
    std::vector<int> rewardRankings = gameRule->getColorRewardRankings();
    int currentReward = rewardRankings[static_cast<size_t>(mermaidCount) - 1];
    QString rankDesc = (mermaidCount == 1)   ? QStringLiteral("最多")
                       : (mermaidCount == 2) ? QStringLiteral("次高")
                       : (mermaidCount == 3) ? QStringLiteral("第三高")
                                             : QStringLiteral("第四高");

    QMessageBox::information(
        dialogParent(), QStringLiteral("🧜‍♀️ 获得美人鱼牌"),
        QStringLiteral("恭喜%1获得第%2张美人鱼牌！\n"
                       "计分规则：%3颜色奖励分 → +%4分\n"
                       "当前美人鱼累计得分：+%5分\n"
                       "集齐4张将直接获得游戏胜利！")
            .arg(QString::fromStdString(player.getPlayerName()))
            .arg(mermaidCount)
            .arg(rankDesc)
            .arg(currentReward)
            .arg(gameRule->calcMermaidSingleScorePublic(player)));
}

QString SeaSaltGameController::getScoreDetail(Player &player)
{
    int pairBaseScore = player.getRoundScore();
    int eventScore = player.getEventScore();
    int collectTotalScore = gameRule->calcCollectCardScorePublic(player);
    int buffScore = gameRule->calcBuffCardScorePublic(player);
    int mermaidScore = gameRule->calcMermaidSingleScorePublic(player);
    int collectPureScore = collectTotalScore;
    int roundTotalNoColor = pairBaseScore + eventScore + collectPureScore + buffScore + mermaidScore;
    int maxColorCount = gameRule->getMaxColorCountPublic();
    int maxColorReward = gameRule->getColorRewardScorePublic(maxColorCount);
    std::vector<int> rewardRankings = gameRule->getColorRewardRankings();

    int seahorseCount = 0;
    for (const auto &card : player.getCollectCards()) {
        if (card.getCollectSubType() == CollectSubType::SEAHORSE) {
            seahorseCount++;
        }
    }

    QString collectDetail;
    int shellTotal = gameRule->getCollectSubTypeTotalScore(player, CollectSubType::SHELL);
    if (shellTotal > 0) {
        int shellCount = static_cast<int>(std::count_if(
            player.getCollectCards().begin(), player.getCollectCards().end(), [](const Card &c) {
                return c.getCollectSubType() == CollectSubType::SHELL;
            }));
        collectDetail += QStringLiteral("  • 贝壳牌：%1张（总分）→ +%2分\n").arg(shellCount).arg(shellTotal);
        for (const auto &card : player.getCollectCards()) {
            if (card.getCollectSubType() == CollectSubType::SHELL) {
                collectDetail += QStringLiteral("    - %1：%2分\n")
                                     .arg(QString::fromStdString(card.getCardName()))
                                     .arg(card.getScore());
            }
        }
    }
    int octoTotal = gameRule->getCollectSubTypeTotalScore(player, CollectSubType::OCTOPUS);
    if (octoTotal > 0) {
        int octoCount = static_cast<int>(std::count_if(
            player.getCollectCards().begin(), player.getCollectCards().end(), [](const Card &c) {
                return c.getCollectSubType() == CollectSubType::OCTOPUS;
            }));
        collectDetail += QStringLiteral("  • 章鱼牌：%1张（总分）→ +%2分\n").arg(octoCount).arg(octoTotal);
        for (const auto &card : player.getCollectCards()) {
            if (card.getCollectSubType() == CollectSubType::OCTOPUS) {
                collectDetail += QStringLiteral("    - %1：%2分\n")
                                     .arg(QString::fromStdString(card.getCardName()))
                                     .arg(card.getScore());
            }
        }
    }
    int penguinTotal = gameRule->getCollectSubTypeTotalScore(player, CollectSubType::PENGUIN);
    if (penguinTotal > 0) {
        int penguinCount = static_cast<int>(std::count_if(
            player.getCollectCards().begin(), player.getCollectCards().end(), [](const Card &c) {
                return c.getCollectSubType() == CollectSubType::PENGUIN;
            }));
        collectDetail += QStringLiteral("  • 企鹅牌：%1张（总分）→ +%2分\n").arg(penguinCount).arg(penguinTotal);
        for (const auto &card : player.getCollectCards()) {
            if (card.getCollectSubType() == CollectSubType::PENGUIN) {
                collectDetail += QStringLiteral("    - %1：%2分\n")
                                     .arg(QString::fromStdString(card.getCardName()))
                                     .arg(card.getScore());
            }
        }
    }
    int anchorTotal = gameRule->getCollectSubTypeTotalScore(player, CollectSubType::ANCHOR);
    if (anchorTotal > 0) {
        int anchorCount = static_cast<int>(std::count_if(
            player.getCollectCards().begin(), player.getCollectCards().end(), [](const Card &c) {
                return c.getCollectSubType() == CollectSubType::ANCHOR;
            }));
        collectDetail += QStringLiteral("  • 船锚牌：%1张（总分）→ +%2分\n").arg(anchorCount).arg(anchorTotal);
        for (const auto &card : player.getCollectCards()) {
            if (card.getCollectSubType() == CollectSubType::ANCHOR) {
                collectDetail += QStringLiteral("    - %1：%2分\n")
                                     .arg(QString::fromStdString(card.getCardName()))
                                     .arg(card.getScore());
            }
        }
    }
    int seahorseCounts = static_cast<int>(std::count_if(
        player.getCollectCards().begin(), player.getCollectCards().end(),
        [](const Card &c) { return c.getCollectSubType() == CollectSubType::SEAHORSE; }));
    if (seahorseCounts > 0) {
        int maxFakeScore = 0;
        for (const auto &card : player.getCollectCards()) {
            CollectSubType cSub = card.getCollectSubType();
            if (cSub == CollectSubType::MERMAID || cSub == CollectSubType::SEAHORSE) {
                continue;
            }
            if (card.getScore() > maxFakeScore) {
                maxFakeScore = card.getScore();
            }
        }
        int seahorseScore = seahorseCount * maxFakeScore;
        collectDetail += QStringLiteral("  • 海马牌：%1张（伪装最高档位%2分）→ +%3分\n")
                             .arg(seahorseCount)
                             .arg(maxFakeScore)
                             .arg(seahorseScore);
    }
    if (collectDetail.isEmpty()) {
        collectDetail = QStringLiteral("  • 无\n");
    }

    QString mermaidDetail;
    int mermaidCount = player.getMermaidCount();
    for (int i = 0; i < mermaidCount; ++i) {
        QString rankDesc = (i == 0)   ? QStringLiteral("最多")
                           : (i == 1) ? QStringLiteral("次高")
                           : (i == 2) ? QStringLiteral("第三高")
                                      : QStringLiteral("第四高");
        int reward = rewardRankings[static_cast<size_t>(i)];
        mermaidDetail += QStringLiteral("  • 第%1张：%2颜色奖励分 → +%3分\n")
                             .arg(i + 1)
                             .arg(rankDesc)
                             .arg(reward);
    }
    if (mermaidDetail.isEmpty()) {
        mermaidDetail = QStringLiteral("  • 无\n");
    }

    QString buffDetail;
    std::unordered_map<BuffSubType, QString> buffNameMap = {
        {BuffSubType::FISH_GROUP, QStringLiteral("鱼群牌")},
        {BuffSubType::CRAB_GROUP, QStringLiteral("螃蟹群牌")},
        {BuffSubType::SHIP_GROUP, QStringLiteral("船队牌")},
        {BuffSubType::PENGUIN_GROUP, QStringLiteral("企鹅群牌")},
        {BuffSubType::ANCHOR_PILE, QStringLiteral("船锚堆牌")}};
    for (const auto &card : player.getBuffCards()) {
        BuffSubType bSub = card.getBuffSubType();
        buffDetail += QStringLiteral("  • %1 → +%2分\n").arg(buffNameMap[bSub]).arg(buffScore);
    }
    if (buffDetail.isEmpty()) {
        buffDetail = QStringLiteral("  • 无\n");
    }

    return QStringLiteral(
               "📊 海盐折纸 - 分值明细【%1】\n"
               "====================================\n"
               "一、基础计分维度（本轮基础分：%2分）\n"
               "🃏 对牌基础分+事件牌额外分：+%3分\n"
               "✅ 收集牌档位分（含海马伪装）：+%4分\n"
               "%5"
               "🧜‍♀️ 美人鱼单张计分：+%6分\n"
               "%7"
               "✨ 增益牌配合分：+%8分\n"
               "------------------------------------\n"
               "二、颜色分规则（轮次结束后生效）\n"
               "🎨 全场最高颜色数量：%9张 → 颜色奖励分：%10分\n"
               "  • 直接截停：颜色分不计入总分\n"
               "  • 放手一搏：\n"
               "    - 你是最高分：+%9（颜色分） + %10（奖励分）\n"
               "    - 你不是最高分：+%9（颜色分）\n"
               "    - 分数比你高者：额外+%10（奖励分）\n"
               "------------------------------------\n"
               "三、累计数据\n"
               "🔥 本轮基础分（未含颜色分）：%2分\n"
               "🏆 游戏总分数：%11分\n"
               "💡 本轮分≥10分可点击【结束本轮】")
        .arg(QString::fromStdString(player.getPlayerName()))
        .arg(roundTotalNoColor)
        .arg(pairBaseScore)
        .arg(collectPureScore)
        .arg(collectDetail)
        .arg(mermaidScore)
        .arg(mermaidDetail)
        .arg(buffScore)
        .arg(maxColorCount)
        .arg(maxColorReward)
        .arg(player.getTotalScore())
        .arg(eventScore);
}

void SeaSaltGameController::showScoreDetail()
{
    if (!gameRule) {
        return;
    }
    Player &currPlayer = gameRule->getCurrentPlayer();
    QMessageBox::information(dialogParent(), QStringLiteral("📊 分值明细"), getScoreDetail(currPlayer));
}

void SeaSaltGameController::quitApp()
{
    QCoreApplication::quit();
}

void SeaSaltGameController::onTurnTimeOut()
{
    if (!gameRule || m_computerTurn) {
        return;
    }
    turnRemainTime--;
    m_windowTitleText = QStringLiteral("海盐折纸卡牌游戏 | 剩余回合时间：%1 秒").arg(turnRemainTime);
    emit titleChanged();

    if (turnRemainTime <= 0) {
        turnTimer->stop();
        QMessageBox::warning(dialogParent(), QStringLiteral("回合超时"),
                             QStringLiteral("思考时间已到！强制结束本回合，切换到下一位玩家。"));
        endTurn();
    }
}

void SeaSaltGameController::refreshTurnTimer()
{
    if (!gameRule) {
        return;
    }
    turnTimer->stop();
    turnRemainTime = 30;
    m_windowTitleText = QStringLiteral("海盐折纸卡牌游戏 | 剩余回合时间：%1 秒").arg(turnRemainTime);
    emit titleChanged();
    if (!m_computerTurn) {
        turnTimer->start();
    }
}
