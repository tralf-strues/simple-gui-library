/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file anchor_pane.h
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <unordered_map>
#include "container.h"

namespace Sgl
{
    /**
     * @brief Allows the edges of components to be anchored to anchor pane's edges.
     * 
     * You can set constraints for a child using set<Side>Anchor() method, which accepts
     * an integer offset. This child's <Side> will be anchored to the pane's <Side> with
     * this offset.
     * 
     * If offset is equal to @ref ANCHOR_NOT_ENABLED, then it is interpreted as
     * there's no anchor attached to this edge.
     * 
     * You can use @ref setHorizontalRelative() or @ref setVerticalRelative() methods to
     * enable relative offsets. This means that the enabled offsets (if one of the opposite
     * offsets is not enabled, then does nothing) are interpreted as integer weights of
     * respective offsets. In this case the child is set to its preffered size in this
     * direction, and the free space in this direction is split according to weighted
     * offsets. For example, imagine that anchor pane's height is set to 1280, child's
     * preffered height is 280 and the top and bottom anchors are equal to 1 and 4
     * respectively. In this case the free vertical 1000 pixels will be split as following:
     * absolute top offset = 1000 * 1 / (1 + 4) = 200,
     * absolute bottom offset = 1000 * 4 / (1 + 4) = 800.
     */
    class AnchorPane : public Container
    {
    public:
        static constexpr int32_t ANCHOR_NOT_ENABLED = -1;

        struct Anchors
        {
            int32_t topAnchor    = ANCHOR_NOT_ENABLED;
            int32_t rightAnchor  = ANCHOR_NOT_ENABLED;
            int32_t bottomAnchor = ANCHOR_NOT_ENABLED;
            int32_t leftAnchor   = ANCHOR_NOT_ENABLED;
        };

        struct Constraints
        {
            Anchors anchors;
            bool    isHorizontalRelative = false;
            bool    isVerticalRelative   = false;
        };

    public:
        virtual ~AnchorPane() override = default;

        Constraints getConstraints(Component* child) const;
        bool hasConstraints(Component* child) const;

        void setHorizontalRelativePositioning(Component* child, bool enabled);
        void setVerticalRelativePositioning(Component* child, bool enabled);

        bool isHorizontalRelativePositioning(Component* child) const;
        bool isVerticalRelativePositioning(Component* child) const;

        void setTopAnchor(Component* child, int32_t anchor);
        void setRightAnchor(Component* child, int32_t anchor);
        void setBottomAnchor(Component* child, int32_t anchor);
        void setLeftAnchor(Component* child, int32_t anchor);
        
        int32_t getTopAnchor(Component* child) const;
        int32_t getRightAnchor(Component* child) const;
        int32_t getBottomAnchor(Component* child) const;
        int32_t getLeftAnchor(Component* child) const;

    private:
        std::unordered_map<Component*, Constraints> m_Constraints; 

    private:
        virtual void layoutChildren() override;

        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
        
        virtual int32_t computeCustomMinWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomMinHeight(int32_t width = -1) const override;
    };
}