#pragma once
#include "HellTypes.h"
#include "Math/AABB.h"
#include "HellGlm.h"
#include <vector>

struct FrustumPlane {
    glm::vec3 normal;
    float offset;
};

struct Frustum {

public:
    void Update(const glm::mat4& projectionView);
    bool IntersectsAABB(const AABB& aabb);
    //bool IntersectsAABB(const RenderItem& renderItem);
    bool IntersectsAABBFast(const AABB& aabb);
    bool IntersectsAABBFast(const RenderItem& renderItem);
    bool IntersectsAABBFast(const HouseRenderItem& renderItem);
    bool IntersectsPoint(const glm::vec3 point);

    glm::vec3 GetBoundsMin() { return m_boundsMin; }
    glm::vec3 GetBoundsMax() { return m_boundsMax; }
    glm::vec3 GetCorner(int index) { return m_corners[index]; } // sketchy
    glm::vec4 GetPlane(int index);

private:
    FrustumPlane CreatePlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    float SignedDistance(const glm::vec3& point, const FrustumPlane& plane) const;
    static glm::vec3 IntersectPlanes(const glm::vec3& n1, float d1, const glm::vec3& n2, float d2, const glm::vec3& n3, float d3);

    FrustumPlane m_planes[6];
    glm::vec3 m_corners[8];
    glm::vec3 m_boundsMin = glm::vec3(0);
    glm::vec3 m_boundsMax = glm::vec3(0);
};

