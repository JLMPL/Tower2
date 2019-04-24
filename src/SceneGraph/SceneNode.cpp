#include "SceneNode.hpp"

void SceneNode::attachNode(SceneNode* node)
{
    m_children.push_back(node);
}

void SceneNode::detachNode(SceneNode* node)
{
    for (auto i = 0; i < m_children.size(); i++)
    {
        if (m_children[i] == node)
        {
            m_children.erase(m_children.begin() + i);
            return;
        }
    }
}

void SceneNode::setPosition(const vec3& pos)
{
    m_pos = pos;
}

void SceneNode::calcLocalTransform()
{
    m_localTransform = math::translate(m_pos) * math::mat4_cast(m_rot);
}

void SceneNode::calcsUsingGlobal()
{
}

void SceneNode::updateTransforms(mat4 parent)
{
    calcLocalTransform();
    m_globalTransform = parent * m_localTransform;

    calcsUsingGlobal();

    for (auto& i : m_children)
        i->updateTransforms(m_globalTransform);
}

void SceneNode::setRotation(const quat& qt)
{
    m_rot = qt;
}

const quat& SceneNode::getRotation() const
{
    return m_rot;
}

const vec3& SceneNode::getPosition() const
{
    return m_pos;
}

void SceneNode::setTransform(const mat4& tr)
{
    m_localTransform = tr;
}

const mat4& SceneNode::getTransform() const
{
    return m_localTransform;
}

const mat4& SceneNode::getGlobalTransform() const
{
    return m_globalTransform;
}

void SceneNode::show()
{
    m_visible = true;
}

void SceneNode::hide()
{
    m_visible = false;
}

bool SceneNode::isVisible() const
{
    return m_visible;
}

bool SceneNode::hasChildren() const
{
    return !m_children.empty();
}

SceneNode::Type SceneNode::getType() const
{
    return Type::Empty;
}
