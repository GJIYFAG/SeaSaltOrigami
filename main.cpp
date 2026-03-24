#include "GameRule.h"
#include <iostream>

int main() {
    try {
        // 1. 初始化游戏：3人局（可改为2/4人），指定玩家名称
        int playerNum = 3;
        std::vector<std::string> playerNames = {"玩家1", "玩家2", "玩家3"};
        GameRule game(playerNum, playerNames);

        // 2. 游戏开局
        game.gameStart();

        // 3. 游戏主循环（执行玩家回合，直到游戏结束）
        int turnCount = 0;
        while (!game.isGameOverFlag() && turnCount < 10) { // 测试限制10个回合
            turnCount++;
            std::cout << "[游戏] 第" << turnCount << "个回合开始！" << std::endl;
            // 执行当前玩家回合：抽牌方式1（主牌库抽2弃1），放入弃牌堆1
            game.doCurrentPlayerTurn(1, 1);
            // 打印当前游戏状态
            game.printGameState();
        }

        // 4. 若未自然结束，手动结束轮次并判定胜利
        if (!game.isGameOverFlag()) {
            std::cout << "[游戏] 测试回合结束，手动结算本轮！" << std::endl;
            game.roundEnd(false);
        }

    } catch (const std::exception& e) {
        std::cerr << "游戏异常：" << e.what() << std::endl;
        return -1;
    }

    std::cout << "[游戏] 控制台测试结束！" << std::endl;
    return 0;
}
