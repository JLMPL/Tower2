#pragma once
#include "Waypoint.hpp"

class Waynet
{
public:
    using Route = std::vector<vec3>;

    Waynet() = default;

    void loadFromFile(const std::string& path);
    void debugDraw();

    Route findWay(const vec3& from, const vec3& to);

private:
    void resetWaypoints();
    Waypoint* findClosestToPosition(const vec3& pos);
    Waypoint* pickOneWithLowestFCost(const std::vector<Waypoint*>& vec);

private:
    struct Edge
    {
        Waypoint* a = nullptr;
        Waypoint* b = nullptr;
    };

    std::vector<Waypoint> m_waypoints;
    std::vector<Edge>     m_edges;
};
