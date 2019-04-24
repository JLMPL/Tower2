#include "Waynet.hpp"
#include "Core/Utils.hpp"
#include "Debug/Log.hpp"
#include <fstream>

void Waynet::loadFromFile(const std::string& path)
{
    std::ifstream file(path.c_str());

    if (!file.good())
    {
        Log::error("Could not load waynet %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);

        if (line[0] == 'v')
        {
            Waypoint w;
            sscanf(line.c_str(), "v %f %f %f", &w.pos.x, &w.pos.y, &w.pos.z);

            m_waypoints.push_back(w);
        }
        else if (line[0] == 'l')
        {
            u32 a, b;
            sscanf(line.c_str(), "l %d %d", &a, &b);

            a--;
            b--;

            Edge p;
            p.a = &m_waypoints[a];
            p.b = &m_waypoints[b];

            m_waypoints[a].connect(&m_waypoints[b]);

            m_edges.push_back(p);
        }
    }

    file.close();
}

void Waynet::resetWaypoints()
{
    for (auto& wp : m_waypoints)
    {
        wp.gCost = 0;
        wp.hCost = 0;
        wp.fCost = 0;
        wp.cameFrom = nullptr;
    }
}

Waypoint* Waynet::findClosestToPosition(const vec3& pos)
{
    Waypoint* closest = nullptr;
    f32 dist = 0.f;

    for (auto& i : m_waypoints)
    {
        if (!closest)
        {
            closest = &i;
            dist = math::distance(i.pos, pos);
        }
        else if (f32 nudist = math::distance(i.pos, pos); nudist < dist)
        {
            closest = &i;
            dist = nudist;
        }
    }

    return closest;
}

Waypoint* Waynet::pickOneWithLowestFCost(const std::vector<Waypoint*>& vec)
{
    Waypoint* theOne = nullptr;
    f32 lowestf = 100000;

    for (auto& i : vec)
    {
        if (!theOne)
        {
            theOne = i;
            lowestf = theOne->fCost;
        }
        else if (i->fCost < lowestf)
        {
            theOne = i;
            lowestf = i->fCost;
        }
    }

    return theOne;
}

Waynet::Route Waynet::findWay(const vec3& from, const vec3& to)
{
    resetWaypoints();

    auto start = findClosestToPosition(from);
    auto end = findClosestToPosition(to);

    std::vector<Waypoint*> open;
    std::vector<Waypoint*> closed;

    start->gCost = 0;
    start->hCost = math::distance(start->pos, end->pos);
    start->fCost = start->gCost + start->hCost;
    open.push_back(start);

    while (true)
    {
        Waypoint* current = pickOneWithLowestFCost(open);

        Utils::removeFromVector(open, current);
        closed.push_back(current);

        if (current == end)
            break;

        for (u32 hf = 0; hf < current->numNeighbors; hf++)
        {
            auto& i = current->neighbors[hf];

            if (Utils::isInVector(closed, i))
                continue;

            if (f32 newCost = current->gCost + math::distance(current->pos, i->pos);
                newCost < i->gCost || !Utils::isInVector(open, i))
            {
                i->gCost = math::distance(i->pos, start->pos);
                i->hCost = math::distance(i->pos, end->pos);
                i->fCost = i->gCost + i->hCost;

                i->cameFrom = current;

                if (!Utils::isInVector(open, i))
                    open.push_back(i);
            }
        }
    }

    Route route;
    Waypoint* wp = end;

    route.push_back(to);

    while (wp != nullptr)
    {
        route.push_back(wp->pos);
        wp = wp->cameFrom;
    }

    return route;
}

void Waynet::debugDraw()
{
    // for (auto& edge : m_edges)
        // gfx::g_Renderer3D.addLine(edge.a->pos, edge.b->pos, vec3(1,0,0));
}
