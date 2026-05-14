#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QTimer>
#include "GameRule.h"
#include "Card.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshAllUI();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_btn_draw_clicked();
    void on_btn_play_clicked();
    void on_btn_endTurn_clicked();
    void on_btn_drawDiscard1_clicked();
    void on_btn_drawDiscard2_clicked();
    void on_btn_roundEnd_clicked();
    void onTurnTimeOut(); // ✅ 新增：回合倒计时超时处理
    void refreshTurnTimer(); // ✅ 新增：刷新回合倒计时

private:
    Ui::MainWindow *ui;
    GameRule *gameRule;
    QTimer *turnTimer; // ✅ 新增：回合倒计时计时器
    int turnRemainTime; // ✅ 新增：回合剩余时间（秒）

    QList<Card> selectedCards;
    QList<QLabel*> selectedLabels;

    void refreshPlayerHand();
    void refreshPileInfo();
    void refreshScore();
    void refreshTurn();
    void clearSelected();
    void updateCardStyle(QLabel* lb, bool isSelected);
    Card getCardFromLabel(QLabel* lb);

    QString getScoreDetail(Player& player);
    void showScoreDetail();
    void showMermaidGainTip(Player& player, int mermaidCount);
    void showPairEffectTip(const Card& c1, const Card& c2);

    void refreshCollectCardsShow();
    void refreshBuffCardsShow();
};
#endif // MAINWINDOW_H
