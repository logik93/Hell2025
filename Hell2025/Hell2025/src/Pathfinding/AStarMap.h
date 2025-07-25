#pragma once
#include "Types/Renderer/MeshBuffer.h"
#include <vector>

namespace AStarMap {
    void Init();
    void Update();
    void UpdateDebugMeshesFromHeightField();
    void MarkCellAsObstacle(int x, int y);
    void MarkCellAsNotObstacle(int x, int y);
    bool IsInBounds(int x, int y);
    bool IsCellObstacle(int x, int y);

    std::vector<glm::ivec2> GetWallCells();
    int GetMapWidth();
    int GetMapHeight();
    int GetCellCount();

    glm::ivec2 GetCellCoordsFromWorldSpacePosition(glm::vec3 position);
    glm::vec3 GetWorldSpacePositionFromCellCoords(glm::ivec2 cellCoords);

    MeshBuffer& GetDebugGridMeshBuffer(); 
    MeshBuffer& GetDebugSolidMeshBuffer();
}