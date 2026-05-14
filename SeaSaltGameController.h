#ifndef SEASALTGAMECONTROLLER_H
#define SEASALTGAMECONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QVariantList>
#include <vector>

#include "GameRule.h"
#include "Card.h"

class SeaSaltGameController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString scoreText READ scoreText NOTIFY stateChanged)
    Q_PROPERTY(QString playerText READ playerText NOTIFY stateChanged)
    Q_PROPERTY(QString mainPileText READ mainPileText NOTIFY stateChanged)
    Q_PROPERTY(QString discard1Text READ discard1Text NOTIFY stateChanged)
    Q_PROPERTY(QString discard2Text READ discard2Text NOTIFY stateChanged)
    Q_PROPERTY(QStringList handCardNames READ handCardNames NOTIFY stateChanged)
    Q_PROPERTY(QVariantList handSelected READ handSelected NOTIFY stateChanged)
    Q_PROPERTY(int selectedHandCount READ selectedHandCount NOTIFY stateChanged)
    Q_PROPERTY(QStringList collectCardLines READ collectCardLines NOTIFY stateChanged)
    Q_PROPERTY(QStringList buffCardNames READ buffCardNames NOTIFY stateChanged)
    Q_PROPERTY(QStringList playedPairCardNames READ playedPairCardNames NOTIFY stateChanged)
    Q_PROPERTY(bool allInPending READ allInPending NOTIFY stateChanged)
    Q_PROPERTY(bool drawDiscard1Enabled READ drawDiscard1Enabled NOTIFY stateChanged)
    Q_PROPERTY(bool drawDiscard2Enabled READ drawDiscard2Enabled NOTIFY stateChanged)
    Q_PROPERTY(bool roundEndVisible READ roundEndVisible NOTIFY stateChanged)
    Q_PROPERTY(bool roundEndHighlight READ roundEndHighlight NOTIFY stateChanged)
    Q_PROPERTY(QString windowTitleText READ windowTitleText NOTIFY titleChanged)
    Q_PROPERTY(bool gameActive READ gameActive NOTIFY gameActiveChanged)
    Q_PROPERTY(bool singlePlayerMode READ singlePlayerMode NOTIFY singlePlayerModeChanged)
    Q_PROPERTY(bool computerTurn READ computerTurn NOTIFY stateChanged)
    Q_PROPERTY(bool pendingDrawActive READ pendingDrawActive NOTIFY stateChanged)
    Q_PROPERTY(QStringList pendingDrawCardNames READ pendingDrawCardNames NOTIFY stateChanged)
    Q_PROPERTY(int pendingKeepIndex READ pendingKeepIndex NOTIFY stateChanged)
    Q_PROPERTY(QString toastText READ toastText NOTIFY stateChanged)
    Q_PROPERTY(bool toastVisible READ toastVisible NOTIFY stateChanged)

public:
    explicit SeaSaltGameController(QObject *parent = nullptr);
    ~SeaSaltGameController() override;

    QString scoreText() const { return m_scoreText; }
    QString playerText() const { return m_playerText; }
    QString mainPileText() const { return m_mainPileText; }
    QString discard1Text() const { return m_discard1Text; }
    QString discard2Text() const { return m_discard2Text; }
    QStringList handCardNames() const { return m_handCardNames; }
    QVariantList handSelected() const { return m_handSelected; }
    int selectedHandCount() const { return static_cast<int>(m_selectedHandIndices.size()); }
    QStringList collectCardLines() const { return m_collectCardLines; }
    QStringList buffCardNames() const { return m_buffCardNames; }
    QStringList playedPairCardNames() const { return m_playedPairCardNames; }
    bool allInPending() const { return m_allInPending; }
    bool drawDiscard1Enabled() const { return m_drawDiscard1Enabled; }
    bool drawDiscard2Enabled() const { return m_drawDiscard2Enabled; }
    bool roundEndVisible() const { return m_roundEndVisible; }
    bool roundEndHighlight() const { return m_roundEndHighlight; }
    QString windowTitleText() const { return m_windowTitleText; }
    bool gameActive() const { return m_gameActive; }
    bool singlePlayerMode() const { return m_singlePlayerMode; }
    bool computerTurn() const { return m_computerTurn; }
    bool pendingDrawActive() const { return m_pendingDrawActive; }
    QStringList pendingDrawCardNames() const { return m_pendingDrawCardNames; }
    int pendingKeepIndex() const { return m_pendingKeepIndex; }
    QString toastText() const { return m_toastText; }
    bool toastVisible() const { return m_toastVisible; }

    Q_INVOKABLE void startGame(bool singlePlayer);
    Q_INVOKABLE void returnToMenu();

    Q_INVOKABLE void drawMain();
    Q_INVOKABLE void selectPendingKeep(int index);
    Q_INVOKABLE void finishPendingMainDraw(int discardPileNo);
    Q_INVOKABLE void drawDiscard1();
    Q_INVOKABLE void drawDiscard2();
    Q_INVOKABLE void play();
    Q_INVOKABLE void endTurn();
    Q_INVOKABLE void endRound();
    Q_INVOKABLE void toggleHandCard(int index);
    Q_INVOKABLE void moveHandCard(int fromIndex, int toIndex);
    Q_INVOKABLE void showScoreDetail();
    Q_INVOKABLE void dismissToast();
    Q_INVOKABLE void quitApp();

signals:
    void stateChanged();
    void titleChanged();
    void requestQuit();
    void gameActiveChanged();
    void singlePlayerModeChanged();
    void drawAnimationRequested(QString sourcePile, QString cardName);
    void discardAnimationRequested(QString targetPile, QString cardName);
    void pendingDrawAnimationRequested(QString firstCardName, QString secondCardName);
    void zoneAnimationRequested(QString sourceArea, QString targetArea, QString cardName);

private slots:
    void onTurnTimeOut();

private:
    GameRule *gameRule = nullptr;
    QTimer *turnTimer = nullptr;
    int turnRemainTime = 30;

    bool m_gameActive = false;
    bool m_singlePlayerMode = false;
    bool m_computerTurn = false;
    bool m_aiScheduled = false;
    bool m_allInPending = false;
    int m_allInStarterIndex = -1;
    int m_allInTurnsRemaining = 0;

    std::vector<int> m_selectedHandIndices;
    std::vector<Card> m_pendingDrawCards;

    bool m_pendingDrawActive = false;
    int m_pendingKeepIndex = 0;
    QStringList m_pendingDrawCardNames;
    QString m_toastText;
    bool m_toastVisible = false;

    QString m_scoreText;
    QString m_playerText;
    QString m_mainPileText;
    QString m_discard1Text;
    QString m_discard2Text;
    QStringList m_handCardNames;
    QVariantList m_handSelected;
    QStringList m_collectCardLines;
    QStringList m_buffCardNames;
    QStringList m_playedPairCardNames;
    bool m_drawDiscard1Enabled = true;
    bool m_drawDiscard2Enabled = true;
    bool m_roundEndVisible = false;
    bool m_roundEndHighlight = false;
    QString m_windowTitleText;

    int currentRoundUiScore(Player &player) const;
    void completeCurrentTurn();
    void settleRound(bool isAllIn, int starterIndex);
    void refreshAllUI();
    void refreshTurnTimer();
    void clearSelected();
    void rebuildHandSelectionFlags();
    Card cardAtHandIndex(int index) const;
    bool hasPendingDraw() const { return m_pendingDrawActive; }
    void clearPendingDraw();
    void rebuildPendingDrawNames();
    void showToast(const QString &message, int timeoutMs = 2200);

    void updateComputerTurnFlag();
    bool currentPlayerIsComputer() const;
    void scheduleAiIfComputerTurn();
    void aiTakeTurnStep();
    void aiDrawRandomMain();
    bool aiTryDrawDiscard(int pileNo);
    bool aiTryPlayOnePair(bool silentTips);

    QString getScoreDetail(Player &player);
    void showPairEffectTip(const Card &c1, const Card &c2, bool silent = false);
    void showMermaidGainTip(Player &player, int mermaidCount, bool silent = false);
};

#endif
