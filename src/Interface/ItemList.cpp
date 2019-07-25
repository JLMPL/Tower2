#include "ItemList.hpp"
#include "Core/Config.hpp"
#include "Core/FrameInfo.hpp"
#include "Events/Event.hpp"
#include "Render/FontManager.hpp"
#include "Render/Renderer2D.hpp"
#include "Render/TextureManager.hpp"
#include "Gameplay/ItemManager.hpp"

namespace ui
{

constexpr i32 spriteSize = 48;
constexpr i32 interOffset = 32;
constexpr f32 timeout = 2.f;

void ItemList::init()
{
    auto display = core::getDisplayConfig();

    m_pos = vec2(
        display.halfWidth - (((NumShownItems - 1) * spriteSize) + ((NumShownItems - 2) * interOffset)) / 2,
        display.height - (spriteSize + 32)
    );

    for (auto i = 0; i < NumShownItems; i++)
    {
        m_spr[i].init(gfx::g_TexMgr.getTexture("sword.png"), {0, 0, spriteSize, spriteSize}, {0,0,32,32});
        m_spr[i].setPosition(m_pos + vec2(i * (spriteSize + interOffset), 0));
        // m_spr[i].setColor(m_colors[(m_scrollIndex - i) % m_numItems]);
    }

    m_select.init(gfx::g_TexMgr.getTexture("select.png"));
    m_select.setPosition({display.halfWidth - 32, display.height - (56 + 32)});

    m_name.init(gfx::g_FontMgr.getFont("DejaVuSans-Bold.ttf"));
    m_name.setCharacterSize(18);
}

void ItemList::setItems(const std::vector<i32>& items)
{
    m_items = &items;
}

void ItemList::sendSystemEvent(const Event& event)
{
    if (event.type == Event::Type::ButtonPressed &&
        event.button == Button::K_F)
    {
        if (!m_isScrolling)
            startScroll();
    }
}

void ItemList::startScroll()
{
    m_isScrolling = true;
    m_lerp = 0;
    m_scrollIndex++;

    if (m_scrollIndex == m_items->size())
        m_scrollIndex = 0;

    resetTimer(m_timer);
    m_visible = true;

    for (auto i = 0; i < NumShownItems; i++)
    {
        m_spr[i].setPosition(m_spr[i].getPosition() + -vec2(spriteSize + interOffset, 0));
        m_prevPos[i] = m_spr[i].getPosition();
        m_targetPos[i] = m_spr[i].getPosition() + vec2(spriteSize + interOffset, 0);
    }
}

void ItemList::endScroll()
{
    m_isScrolling = false;

    for (auto i = 0; i < NumShownItems; i++)
    {
        m_spr[i].setPosition(math::lerp(m_prevPos[i], m_targetPos[i], 1.f));

        auto itemIndex = m_scrollIndex - i - 1;
        if (itemIndex < 0)
            itemIndex += (m_items->size()-1);

        const Item* item = g_ItemMgr.getItem((*m_items)[itemIndex]);
        m_spr[i].setTexture(item->m_image);
    }

    m_spr[NumShownItems-1].setColor({1,1,1,0});
}

void ItemList::performScroll()
{
    if (!m_isScrolling)
        return;

    m_lerp += core::g_FInfo.delta * 1;

    if (m_lerp >= 1)
    {
        m_lerp = 1;
        endScroll();
        return;
    }

    for (auto i = 0; i < NumShownItems; i++)
    {
        m_spr[i].setPosition(math::lerp(m_prevPos[i], m_targetPos[i], m_lerp));

        auto itemIndex = m_scrollIndex - i;
        if (itemIndex < 0)
            itemIndex += (m_items->size()-1);

        const Item* item = g_ItemMgr.getItem((*m_items)[itemIndex]);
        m_spr[i].setTexture(item->m_image);
    }

    m_spr[0].setColor(math::lerp(Color(1,1,1,0), Color(1,1,1,1), m_lerp));
    m_spr[NumShownItems-1].setColor(math::lerp(Color(1,1,1,1), Color(1,1,1,0), m_lerp));
}

void ItemList::update()
{
    performScroll();

    if (core::getElapsedTime(m_timer) > timeout)
        m_visible = false;

    i32 itemid = (*m_items)[(m_scrollIndex + 5) % m_items->size()];
    m_name.setString(g_ItemMgr.getItem(itemid)->m_name);

    auto display = core::getDisplayConfig();
    m_name.setPosition({display.halfWidth - (m_name.getWidth() / 2), display.height - 96});
}

void ItemList::draw()
{
    if (!m_visible)
        return;

    for (auto i = 0; i < NumShownItems; i++)
        gfx::g_Renderer2D.draw(m_spr[i]);

    gfx::g_Renderer2D.draw(m_select);
    gfx::g_Renderer2D.draw(m_name);
}

}
