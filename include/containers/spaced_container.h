//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file spaced_container.h
//! @date 2021-10-31
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SPACED_CONTAINER_H
#define SPACED_CONTAINER_H

#include "../container.h"

// TODO: 1) Merge several Spacers at the same position
// TODO: 2) Add the ability to remove spacers
// TODO: 3) Add removeInvalidSpacers()
// TODO: 4) Add vertically spaced container

class SpacedContainer : public Container
{
public:
    struct Spacer
    {
        Spacer(Component* prevComponent = nullptr, uint32_t weight = 1)
            : prevComponent(prevComponent), weight(weight) {}

        Component* prevComponent; ///< If nullptr than at the beginning.
        uint32_t   weight;
    };

    SpacedContainer(Renderer* renderer) : Container(renderer) {}
    virtual ~SpacedContainer() override = default;

    void insertSpacerAfter(Component* component, uint32_t weight = 1);
    void addSpacer(uint32_t weight = 1);
    void pushFrontSpacer(uint32_t weight = 1);

protected:
    std::list<Spacer> m_Spacers;

    virtual void layOutComponents() override;
    // virtual Vec2<int32_t> getPrefferedLayoutSize() override;
    // virtual Vec2<int32_t> getMinLayoutSize() override;

    int32_t getSpacerWidth(Component* prev, int32_t totalWeight, int32_t totalSpacersWidth);
    bool getSpacer(Component* prev, Spacer* spacer);
    int32_t weightToWidth(const Spacer& spacer, int32_t totalWeight, int32_t totalSpacersWidth);
    uint32_t getTotalWeight();
    int32_t getTotalWidth();
};

#endif // SPACED_CONTAINER_H