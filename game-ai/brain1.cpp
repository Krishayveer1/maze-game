#include "brain.h"

Brain::Brain()
{
}

int Brain::getNextMove(GameState &gamestate)
{

    int currentStage = gamestate.stage;

    if (stageTypes.find(currentStage) == stageTypes.end())
    {
        int type = detectStageType(gamestate);
        stageTypes[currentStage] = type;
        std::cout << "Detected Stage " << currentStage << " as type " << type << std::endl;
    }

    int type = stageTypes[currentStage];

    if (type == STAGE_MAZE)
        return handleMaze(gamestate);
    else if (type == STAGE_FOOD)
        return handleFood(gamestate);
    else if (type == STAGE_FLAG)
        return handleFlag(gamestate);
    return 0;
}

int Brain::handleFlag(const GameState &gamestate)
{
    std::pair<int, int> currentPos = {gamestate.pos[0], gamestate.pos[1]};
    static bool reachedA = false;
    static bool reachedB = false;
    static std::pair<int, int> targetA = {-1, -1};
    static std::pair<int, int> targetB = {-1, -1};

    if (gamestate.stage != lastStage)
    {
        visitedPositions.clear();
        internalMap.clear();
        while (!pathStack.empty())
            pathStack.pop();
        lastStageEntryPos = {currentPos.first, currentPos.second - 1};
        lastStage = gamestate.stage;
        reachedA = false;
        reachedB = false;
        targetA = {-1, -1};
        targetB = {-1, -1};
    }

    visitedPositions.insert(currentPos);
    updateInternalMap(gamestate);

    int player_h = -1, player_w = -1;
    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            char tile = gamestate.vision[i][j];
            if (tile == 'v' || tile == '<' || tile == '>' || tile == '^')
            {
                player_h = i;
                player_w = j;
                break;
            }
        }
        if (player_h != -1)
            break;
    }

    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            char tile = gamestate.vision[i][j];
            int rel_h = i - player_h;
            int rel_w = j - player_w;

            int abs_h = gamestate.pos[0] + rel_h;
            int abs_w = gamestate.pos[1] + rel_w;
            std::pair<int, int> abs_pos = {abs_h, abs_w};

            if (tile == 'A')
                targetA = abs_pos;
            if (tile == 'B')
                targetB = abs_pos;
        }
    }

    if (!reachedA && currentPos == targetA)
    {
        std::cout << "Reached A at: " << currentPos.first << ", " << currentPos.second << std::endl;
        reachedA = true;
        visitedPositions.clear();
        while (!pathStack.empty())
            pathStack.pop();
    }

    if (reachedA && !reachedB && currentPos == targetB)
    {
        std::cout << "Reached B at: " << currentPos.first << ", " << currentPos.second << std::endl;
        reachedB = true;
        visitedPositions.clear();
        while (!pathStack.empty())
            pathStack.pop();
    }

    std::vector<std::pair<int, int>> directions = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    std::vector<int> directionCodes = {1, 3, 2, 4};

    if (reachedA && !reachedB && targetB != std::pair<int, int>{-1, -1})
    {
        for (size_t i = 0; i < directions.size(); ++i)
        {
            std::pair<int, int> neighbor = {
                currentPos.first + directions[i].first,
                currentPos.second + directions[i].second};
            if (neighbor == targetB)
                return directionCodes[i];
        }
    }

    if (reachedB)
    {
        char right = getVisionCell(gamestate, 0, 1);
        // char up = getVisionCell(gamestate, -1, 0);
        char down = getVisionCell(gamestate, 1, 0);
        std::pair<int, int> rightPos = {currentPos.first, currentPos.second + 1};

        if (right != '+' && !visitedPositions.count(rightPos))
        {
            lastVerticalDir = 0;
            upIsDeadEnd = false;
            downIsDeadEnd = false;
            return 4;
        }

        if (right == '+')
        {
            if (down == '+')
                downIsDeadEnd = true;
            if (downIsDeadEnd)
                return 1;
            if (down != '+')
                return 3;
        }
    }

    for (size_t i = 0; i < directions.size(); ++i)
    {
        int dh = directions[i].first;
        int dw = directions[i].second;
        std::pair<int, int> neighbor = {currentPos.first + dh, currentPos.second + dw};

        if (neighbor == lastStageEntryPos)
            continue;

        if (visitedPositions.count(neighbor))
            continue;

        char tile = internalMap[neighbor];

        if (!reachedA && (tile == 'B' || tile == 'D'))
            continue;

        if (reachedA && !reachedB && tile == 'D')
            continue;

        if (tile != '+')
        {
            pathStack.push(currentPos);
            return directionCodes[i];
        }
    }

    if (!pathStack.empty())
    {
        std::pair<int, int> back = pathStack.top();
        pathStack.pop();

        if (back.first < currentPos.first)
            return 1;
        if (back.first > currentPos.first)
            return 3;
        if (back.second < currentPos.second)
            return 2;
        if (back.second > currentPos.second)
            return 4;
    }

    return (std::rand() % 4) + 1;
}

int Brain::handleFood(const GameState &gamestate)
{
    int player_h = -1;
    int player_w = -1;

    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            char c = gamestate.vision[i][j];
            if (c == 'v' || c == '^' || c == '<' || c == '>')
            {
                player_h = i;
                player_w = j;
                break;
            }
        }
        if (player_h != -1)
            break;
    }

    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            if (gamestate.vision[i][j] == '0')
            {
                dExists = false;
                int dh = i - player_h;
                int dw = j - player_w;

                if (abs(dh) > abs(dw))
                {
                    return (dh < 0) ? 1 : 3;
                }
                else
                {
                    return (dw < 0) ? 2 : 4;
                }
            }
        }
    }

    for (const auto &row : gamestate.vision)
    {
        for (char tile : row)
        {
            if (tile == 'D')
                dExists = true;
        }
    }

    if (dExists)
    {
        return 1 + (std::rand() % 4);
    }

    char right = getVisionCell(gamestate, 0, 1);
    // char up = getVisionCell(gamestate, -1, 0);
    char down = getVisionCell(gamestate, 1, 0);

    if (right == '+')
    {
        if (down == '+')
        {
            downIsDeadEnd = true;
        }
        if (downIsDeadEnd)
        {
            std::cout << "moving up" << std::endl;
            return 1;
        }

        if (down != '+')
        {
            std::cout << "moving down" << std::endl;
            return 3;
        }
    }

    if (right != '+')
    {
        std::cout << "moving right (wall hug)" << std::endl;
        upIsDeadEnd = false;
        downIsDeadEnd = false;
        return 4;
    }

    return (std::rand() % 4) + 1;
}

int Brain::handleMaze(const GameState &gamestate)
{
    std::pair<int, int> currentPos = {gamestate.pos[0], gamestate.pos[1]};

    if (gamestate.stage != lastStage)
    {
        visitedPositions.clear();
        internalMap.clear();
        while (!pathStack.empty())
            pathStack.pop();
        lastStageEntryPos = {currentPos.first, currentPos.second - 1};
        lastStage = gamestate.stage;
    }

    visitedPositions.insert(currentPos);
    updateInternalMap(gamestate);

    std::vector<std::pair<int, int>> directions = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}};

    std::vector<int> directionCodes = {1, 3, 2, 4};

    for (size_t i = 0; i < directions.size(); ++i)
    {
        int dh = directions[i].first;
        int dw = directions[i].second;
        std::pair<int, int> neighbor = {currentPos.first + dh, currentPos.second + dw};

        if (neighbor == lastStageEntryPos)
            continue;

        if (visitedPositions.count(neighbor))
            continue;

        char tile = internalMap[neighbor];
        if (tile != '+')
        {
            pathStack.push(currentPos);
            return directionCodes[i];
        }
    }

    if (!pathStack.empty())
    {
        std::pair<int, int> back = pathStack.top();
        pathStack.pop();

        if (back.first < currentPos.first)
            return 1;
        if (back.first > currentPos.first)
            return 3;
        if (back.second < currentPos.second)
            return 2;
        if (back.second > currentPos.second)
            return 4;
    }

    return 0;
}

char Brain::getVisionCell(const GameState &gamestate, int rel_h, int rel_w)
{
    int player_h = -1;
    int player_w = -1;

    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            char c = gamestate.vision[i][j];
            if (c == 'v' || c == '^' || c == '<' || c == '>')
            {
                player_h = i;
                player_w = j;
                break;
            }
        }
        if (player_h != -1)
            break;
    }

    if (player_h == -1 || player_w == -1)
    {
        std::cerr << "Player not found in vision grid!\n";
        return '\0';
    }

    int h = player_h + rel_h;
    int w = player_w + rel_w;

    if (h < 0 || h >= gamestate.vision.size() ||
        w < 0 || w >= gamestate.vision[0].size())
    {
        return '\0';
    }

    return gamestate.vision[h][w];
}

bool Brain::isFoodStage(const GameState &gamestate)
{
    for (const auto &row : gamestate.vision)
        for (char tile : row)
            if (tile == '0')
                return true;
    return false;
}

bool Brain::isFlagStage(const GameState &gamestate)
{
    for (const auto &row : gamestate.vision)
        for (char tile : row)
            if (tile == 'A' || tile == 'B')
                return true;
    return false;
}

int Brain::detectStageType(const GameState &gamestate)
{
    for (const auto &row : gamestate.vision)
    {
        for (char tile : row)
        {
            if (tile == '0')
                return STAGE_FOOD;
            if (tile == 'A' || tile == 'B')
                return STAGE_FLAG;
            if (tile == 'E')
                return STAGE_ENEMY;
        }
    }
    return STAGE_MAZE;
}

void Brain::updateInternalMap(const GameState &gamestate)
{
    int ph = -1, pw = -1;
    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            char c = gamestate.vision[i][j];
            if (c == 'v' || c == '<' || c == '>' || c == '^')
            {
                ph = i;
                pw = j;
            }
        }
    }

    for (int i = 0; i < gamestate.vision.size(); ++i)
    {
        for (int j = 0; j < gamestate.vision[i].size(); ++j)
        {
            int abs_h = gamestate.pos[0] + (i - ph);
            int abs_w = gamestate.pos[1] + (j - pw);
            internalMap[{abs_h, abs_w}] = gamestate.vision[i][j];
        }
    }
}
