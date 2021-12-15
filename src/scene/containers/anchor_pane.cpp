/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file anchor_pane.cpp
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/containers/anchor_pane.h"

namespace Sgl
{
    #define CONSTRAINTS_SET(property, value)          \
        assert(child);                                \
        auto constraints = m_Constraints.find(child); \
        if (constraints != m_Constraints.end())       \
        {                                             \
            (*constraints).second.property = value;   \
        }                                             \
        else                                          \
        {                                             \
            m_Constraints[child] = Constraints{};     \
            m_Constraints[child].property = value;    \
        }

    #define CONSTRAINTS_GET(property, defaultValue)                                    \
        assert(child);                                                                 \
        auto constraints = m_Constraints.find(child);                                  \
        return (constraints != m_Constraints.end()) ? (*constraints).second.property : \
                                                      defaultValue;

    AnchorPane::Constraints AnchorPane::getConstraints(Component* child) const
    {
        assert(child);

        auto constraints = m_Constraints.find(child);
        if (constraints != m_Constraints.end())
        {
            return (*constraints).second;
        }

        return Constraints{};
    }

    bool AnchorPane::hasConstraints(Component* child) const
    {
        assert(child);

        auto constraints = m_Constraints.find(child);
        if (constraints == m_Constraints.end())
        {
            return false;
        }

        const Anchors& anchors = (*constraints).second.anchors;

        return anchors.topAnchor    != ANCHOR_NOT_ENABLED ||
               anchors.rightAnchor  != ANCHOR_NOT_ENABLED ||
               anchors.bottomAnchor != ANCHOR_NOT_ENABLED ||
               anchors.leftAnchor   != ANCHOR_NOT_ENABLED;
    }

    void AnchorPane::setHorizontalRelativePositioning(Component* child, bool enabled)
    {
        CONSTRAINTS_SET(isHorizontalRelative, enabled);
    }

    void AnchorPane::setVerticalRelativePositioning(Component* child, bool enabled)
    {
        CONSTRAINTS_SET(isVerticalRelative, enabled);
    }

    bool AnchorPane::isHorizontalRelativePositioning(Component* child) const
    {
        CONSTRAINTS_GET(isHorizontalRelative, false);
    }

    bool AnchorPane::isVerticalRelativePositioning(Component* child) const
    {
        CONSTRAINTS_GET(isVerticalRelative, false);
    }

    void AnchorPane::setTopAnchor(Component* child, int32_t anchor)
    {
        CONSTRAINTS_SET(anchors.topAnchor, anchor);
    }

    void AnchorPane::setRightAnchor(Component* child, int32_t anchor)
    {
        CONSTRAINTS_SET(anchors.rightAnchor, anchor);
    }

    void AnchorPane::setBottomAnchor(Component* child, int32_t anchor)
    {
        CONSTRAINTS_SET(anchors.bottomAnchor, anchor);
    }

    void AnchorPane::setLeftAnchor(Component* child, int32_t anchor)
    {
        CONSTRAINTS_SET(anchors.leftAnchor, anchor);
    }
    
    int32_t AnchorPane::getTopAnchor(Component* child) const
    {
        CONSTRAINTS_GET(anchors.topAnchor, ANCHOR_NOT_ENABLED);
    }

    int32_t AnchorPane::getRightAnchor(Component* child) const
    {
        CONSTRAINTS_GET(anchors.rightAnchor, ANCHOR_NOT_ENABLED);
    }

    int32_t AnchorPane::getBottomAnchor(Component* child) const
    {
        CONSTRAINTS_GET(anchors.bottomAnchor, ANCHOR_NOT_ENABLED);
    }

    int32_t AnchorPane::getLeftAnchor(Component* child) const
    {
        CONSTRAINTS_GET(anchors.leftAnchor, ANCHOR_NOT_ENABLED);
    }

    void AnchorPane::layoutChildren()
    {
        Sml::Rectangle<int32_t> contentArea = getContentArea();

        for (auto child : m_Children)
        {
            Constraints constraints = getConstraints(child);

            if (!hasConstraints(child))
            {
                child->setLayoutWidth(child->computePrefWidth());
                child->setLayoutHeight(child->computePrefHeight());
                continue;
            }

            // Horizontal layout
            if (isHorizontalRelativePositioning(child))
            {
                int32_t prefWidth = child->computePrefWidth();
                int32_t freeWidth = contentArea.width - prefWidth;
             
                child->setLayoutWidth(prefWidth);

                int32_t totalAnchors = constraints.anchors.leftAnchor + constraints.anchors.rightAnchor;
                int32_t leftOffset   = freeWidth * (static_cast<float>(constraints.anchors.leftAnchor) / totalAnchors);

                child->setLayoutX(contentArea.pos.x + leftOffset);
            }
            else if (constraints.anchors.leftAnchor  != ANCHOR_NOT_ENABLED &&
                     constraints.anchors.rightAnchor != ANCHOR_NOT_ENABLED)
            {
                child->setLayoutX(contentArea.pos.x + constraints.anchors.leftAnchor);
                child->setLayoutWidth(contentArea.width -
                                      constraints.anchors.leftAnchor -
                                      constraints.anchors.rightAnchor);
            }
            else if (constraints.anchors.leftAnchor  != ANCHOR_NOT_ENABLED &&
                     constraints.anchors.rightAnchor == ANCHOR_NOT_ENABLED)
            {
                child->setLayoutX(contentArea.pos.x + constraints.anchors.leftAnchor);
                child->setLayoutWidth(child->computePrefWidth());
            }
            else if (constraints.anchors.leftAnchor  == ANCHOR_NOT_ENABLED &&
                     constraints.anchors.rightAnchor != ANCHOR_NOT_ENABLED)
            {
                child->setLayoutWidth(child->computePrefWidth());
                child->setLayoutX(contentArea.pos.x + contentArea.width - child->getLayoutWidth() -
                                  constraints.anchors.rightAnchor);
            }
            else
            {
                child->setLayoutWidth(child->computePrefWidth());
            }

            // Vertical layout
            if (isVerticalRelativePositioning(child))
            {
                int32_t prefHeight = child->computePrefHeight();
                int32_t freeHeight = contentArea.height - prefHeight;
             
                child->setLayoutHeight(prefHeight);

                int32_t totalAnchors = constraints.anchors.topAnchor + constraints.anchors.bottomAnchor;
                int32_t topOffset    = freeHeight * (static_cast<float>(constraints.anchors.topAnchor) / totalAnchors);

                child->setLayoutY(contentArea.pos.y + topOffset);
            }
            else if (constraints.anchors.topAnchor    != ANCHOR_NOT_ENABLED &&
                     constraints.anchors.bottomAnchor != ANCHOR_NOT_ENABLED)
            {
                child->setLayoutY(contentArea.pos.y + constraints.anchors.topAnchor);
                child->setLayoutHeight(contentArea.height -
                                      constraints.anchors.topAnchor -
                                      constraints.anchors.bottomAnchor);
            }
            else if (constraints.anchors.topAnchor    != ANCHOR_NOT_ENABLED &&
                     constraints.anchors.bottomAnchor == ANCHOR_NOT_ENABLED)
            {
                child->setLayoutY(contentArea.pos.y + constraints.anchors.topAnchor);
                child->setLayoutHeight(child->computePrefHeight());
            }
            else if (constraints.anchors.topAnchor    == ANCHOR_NOT_ENABLED &&
                     constraints.anchors.bottomAnchor != ANCHOR_NOT_ENABLED)
            {
                child->setLayoutHeight(child->computePrefHeight());
                child->setLayoutY(contentArea.pos.y + contentArea.height - child->getLayoutHeight() -
                                  constraints.anchors.bottomAnchor);
            }
            else
            {
                child->setLayoutHeight(child->computePrefHeight());
            }
        }
    }

    int32_t AnchorPane::computeCustomPrefWidth(int32_t height) const
    {
        int32_t maxNeededWidth = 0;
        for (auto child : m_Children)
        {
            int32_t leftOffset  = getLeftAnchor(child);
            int32_t rightOffset = getRightAnchor(child);

            if (leftOffset == ANCHOR_NOT_ENABLED && rightOffset == ANCHOR_NOT_ENABLED)
            {
                continue;
            }

            int32_t neededWidth = child->computePrefWidth();
            if (!isHorizontalRelativePositioning(child))
            {
                if (leftOffset  != ANCHOR_NOT_ENABLED) { neededWidth += leftOffset;  }
                if (rightOffset != ANCHOR_NOT_ENABLED) { neededWidth += rightOffset; }
            }

            if (neededWidth > maxNeededWidth)
            {
                maxNeededWidth = neededWidth;
            }
        }

        int32_t defaultPrefWidth = Container::computeCustomPrefWidth(height);

        return std::max(defaultPrefWidth, getInsets().left + getInsets().right + maxNeededWidth);
    }

    int32_t AnchorPane::computeCustomPrefHeight(int32_t width) const
    {
        int32_t maxNeededHeight = 0;
        for (auto child : m_Children)
        {
            int32_t topOffset    = getTopAnchor(child);
            int32_t bottomOffset = getBottomAnchor(child);

            if (topOffset == ANCHOR_NOT_ENABLED && bottomOffset == ANCHOR_NOT_ENABLED)
            {
                continue;
            }

            int32_t neededHeight = child->computePrefHeight();
            if (!isVerticalRelativePositioning(child))
            {
                if (topOffset    != ANCHOR_NOT_ENABLED) { neededHeight += topOffset;    }
                if (bottomOffset != ANCHOR_NOT_ENABLED) { neededHeight += bottomOffset; }
            }

            if (neededHeight > maxNeededHeight)
            {
                maxNeededHeight = neededHeight;
            }
        }

        int32_t defaultPrefHeight = Container::computeCustomPrefHeight(width);

        return std::max(defaultPrefHeight, getInsets().top + getInsets().bottom + maxNeededHeight);
    }

    // TODO: implement
    int32_t AnchorPane::computeCustomMinWidth(int32_t height) const
    {
        return 0;
    }

    // TODO: implement
    int32_t AnchorPane::computeCustomMinHeight(int32_t width) const
    {
        return 0;
    }
}