#pragma once
#include "gameFiles/entities/gameObject.h"

class pathfindingComponent : public Component { // flow field pathfinding
public:
	virtual void update(GameObject* owner) {}

	bool computeFlowField(Vec2 target, Vec2 origin) {
        // Reset grid
        mGrid = grid;

        //convert target to target cell
        Vec2 targetCell{ 0,0 };
		targetCell.x = div(target.x, WORLD_TILE_SIZE).quot;
		targetCell.y = div(target.y, WORLD_TILE_SIZE).quot;

		// convert origin to origin cell
        Vec2 originCell{ 0,0 };
        originCell.x = div(origin.x, WORLD_TILE_SIZE).quot;
        originCell.y = div(origin.y, WORLD_TILE_SIZE).quot;

        if(!findOrigin(targetCell, originCell)) return false; // targets not reachable from current position
		calculateFlowDirections();

        return true;
	}

    bool inBounds(Vec2 next) {
        return next.x >= 0 && next.x < WORLD_TILE_COLS && next.y >= 0 && next.y < WORLD_TILE_ROWS; // if in bounds
    }

    bool findOrigin(Vec2 target, Vec2 origin) {
        std::deque<Vec2> q;

        mGrid[target.y][target.x].cost = 0;
        q.push_back(target);

        while (!q.empty()) {
            Vec2 current = q.front();
            q.pop_front();

            int currentCost = mGrid[current.y][current.x].cost;

            for (auto& dir : directions) {
                Vec2 next = { current.x + dir.x, current.y + dir.y };

                if (!inBounds(next) || !mGrid[next.y][next.x].walkable)
                    continue;

                int newCost = currentCost + 1;

                if (newCost < mGrid[next.y][next.x].cost) {
                    mGrid[next.y][next.x].cost = newCost;
                    q.push_back(next);

                    if (next.x == origin.x && next.y == origin.y)
                        return true; // early exit
                }
            }
        }

        return false;
    }


	void calculateFlowDirections() {
        // Compute flow direction for each cell
        for (int y = 0; y < WORLD_TILE_ROWS; y++) {
            for (int x = 0; x < WORLD_TILE_COLS; x++) {
                if (!mGrid[y][x].walkable || mGrid[y][x].cost == INT_MAX) continue; // skip obstacles and cells which would be slower

                int bestCost = mGrid[y][x].cost;
                Vec2 bestDir = { 0, 0 };

                for (auto dir : directions) {
                    Vec2 next = { x + dir.x, y + dir.y };
                    if (next.x >= 0 && next.x < WORLD_TILE_COLS &&
                        next.y >= 0 && next.y < WORLD_TILE_ROWS &&
                        mGrid[next.y][next.x].cost < bestCost) {
                        bestCost = mGrid[next.y][next.x].cost;
                        bestDir = dir;
                    }
                }
                mGrid[y][x].direction = bestDir;
            }
        }
	}

	Vec2 getFlowDirection(Vec2 position) {
        while(position.y >= grid.size()){
            position.y -= 1;
        }
        while (position.x >= grid[position.y].size()) {
            position.x -= 1;
        }
		return grid[position.y][position.x].direction;
	}

	pathfindingComponent(GameObject* obj, vector<vector<Tile>> worldGrid) : Component(obj), mGrid(worldGrid){}
	virtual ~pathfindingComponent() {}

private:
	// The grid representing the world for pathfinding
	vector<vector<Tile>> mGrid;
	// Directions for neighbor tiles (up, down, left, right, and diagonals)
	const vector<Vec2> directions = { {0,-1},{0,1},{-1,0},{1,0},{1,1},{1,-1},{-1,-1},{-1,1} };
};