#ifndef BRAIN_H
#define BRAIN_H

#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include "../Game/game.h"
#include <set>
#include <stack>
#include <map>

class Brain
{
public:
    Brain();
    int getNextMove(GameState &gamestate);
    bool upIsDeadEnd = false;
    bool downIsDeadEnd = false;

private:
    std::pair<int, int> posA = {-1, -1};
    std::pair<int, int> posB = {-1, -1};
    std::pair<int, int> posD = {-1, -1};
    int flagStagePhase = 0;
    std::pair<int, int> targetA = {-1, -1};

    std::set<std::pair<int, int>> visitedPositions;
    bool triedUp = false;
    bool triedDown = false;
    int lastVerticalDir = 0;
    const int STAGE_MAZE = 1;
    const int STAGE_FOOD = 2;
    const int STAGE_FLAG = 3;
    const int STAGE_ENEMY = 4;
    std::unordered_map<int, int> stageTypes;
    bool dExists = false;
    std::map<std::pair<int, int>, char> internalMap;
    std::set<std::pair<int, int>> visited;
    std::stack<std::pair<int, int>> pathStack;
    int lastStage = -1; // Last stage seen (used to clear stack when entering new stage)
    std::pair<int, int> lastStageEntryPos = {-1, -1};

    int detectStageType(const GameState &gamestate);
    int handleMaze(const GameState &gamestate);
    char getVisionCell(const GameState &gamestate, int rel_h, int rel_w);
    bool isFoodStage(const GameState &gamestate);
    bool isFlagStage(const GameState &gamestate);
    int handleFood(const GameState &gamestate);
    void updateInternalMap(const GameState &gamestate);
    int getStageForPosition(const std::pair<int, int> &pos);
    int handleFlag(const GameState &gamestate);
};

#endif