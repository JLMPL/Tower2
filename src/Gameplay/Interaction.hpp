#pragma once
#include "Core/Math3D.hpp"
#include <functional>

class Creature;
class Interactible;

class Interaction
{
public:
    void begin(Creature* creature, Interactible* entity);
    void update();

    void setRequirementFunc(const std::function<bool (void)>& func);
    void setFinishFunc(const std::function<void (Creature*)>& func);
    void setAnimation(const std::string& anim);
    void setCreaturePosition(const vec3& pos, const vec3& dir);

    bool isRunning() const;

private:
    void lerpIn();
    void playAnim();
    void lerpOut();

private:
    enum class Stage
    {
        LerpIn = 0,
        PlayAnim,
        LerpOut
    };

    Stage                           m_stage = Stage::LerpIn;

    Creature*                       m_creature = nullptr;
    Interactible*                   m_entity = nullptr;

    bool                            m_requirementsMet = false;
    std::function<bool (void)>      m_requirementsFunc;
    std::function<void (Creature*)> m_finishFunc;

    std::string                     m_anim;

    f32                             m_lerp = 0;
    vec3                            m_creatureLerpPos[2];
    vec3                            m_creatureLerpDir[2];

    bool                            m_finished = false;
    bool                            m_isRunning = false;
};
