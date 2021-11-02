//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file container.h
//! @date 2021-10-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef CONTAINER_H
#define CONTAINER_H

#include <list>
#include "component.h"

class Container : public Component
{
public:
    Container() = default;
    virtual ~Container() override = default;

    Container(Renderer* renderer, int32_t width = 1, int32_t height = 1)
        : Component(renderer, width, height) {}

    virtual Component* getHitComponent(int32_t x, int32_t y) override;
    virtual void updateGraphics() override;
    virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;

    // virtual Vec2<int32_t> getPrefferedLayoutSize();
    // virtual Vec2<int32_t> getMinLayoutSize();

    void addComponent(Component* component);
    void removeComponent(Component* component);

    const std::list<Component*>& getComponents() const;

protected:
    std::list<Component*> m_Components;

    //--------------------------------------------------------------------------
    //! Go through components and set layout properties (i.e. position, size,
    //! etc.) of these components according to some rule implemented in a
    //! specific LayoutManager.
    //--------------------------------------------------------------------------
    virtual void layOutComponents();
};

#endif // CONTAINER_H