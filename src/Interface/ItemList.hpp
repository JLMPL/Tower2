#include "Render/2D/Sprite.hpp"
#include "Render/2D/Text.hpp"
#include "Core/Timer.hpp"
#include <vector>

struct Event;

namespace ui
{

class ItemList
{
public:
    ItemList() = default;
    ~ItemList() = default;

    void init();
    void sendSystemEvent(const Event& event);
    void update();
    void draw();

    void setItems(const std::vector<i32>& items);

private:
    void startScroll();
    void performScroll();
    void endScroll();

private:
    static constexpr i32 NumShownItems = 4;

    gfx::Sprite m_spr[NumShownItems];
    gfx::Sprite m_select;
    gfx::Text m_name;
    vec2 m_pos;

    vec2 m_prevPos[NumShownItems];
    vec2 m_targetPos[NumShownItems];
    f32 m_lerp = 0;
    i32 m_scrollIndex = 0;

    const std::vector<i32>* m_items = nullptr;

    core::Timer m_timer;
    bool m_visible = false;

    bool m_isScrolling = false;
};

}
