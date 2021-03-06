/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file parent.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/utils.h"
#include "scene/parent.h"

namespace Sgl
{
    Component* Parent::getHitComponent(int32_t x, int32_t y)
    {
        if (!isInteractable()) { return nullptr; }

        for (auto child = m_Children.rbegin(); child != m_Children.rend(); ++child)
        {
            if (!(*child)->isInteractable())
            {
                continue;
            }

            Component* hitted = (*child)->getHitComponent(x - getLayoutX(), y - getLayoutY());

            if (hitted != nullptr)
            {
                return hitted;
            }
        }

        return nullptr;
    }

    void Parent::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (!isVisible())
        {
            return;
        }

        renderShadow(targetRegion);

        Sml::Rectangle<int32_t> translatedTargetRegion = getLayoutBounds();
        translatedTargetRegion.pos += targetRegion.pos;

        if (m_Snapshot != nullptr)
        {
            Sml::Rectangle<int32_t> originBounds = getOriginBounds();
            renderTexture(*m_Snapshot, &translatedTargetRegion, &originBounds);
        }

        for (Component* child : getChildren())
        {
            if (child->isVisible())
            {
                child->render(translatedTargetRegion);
            }
        }
    }

    // TODO: layout only if NEEDS_LAYOUT or DIRTY_BRANCH
    void Parent::layout()
    {
        layoutChildren();

        for (Component* child : m_Children)
        {
            child->layout();
        }

        m_NeedLayoutPass = false;
    }

    void Parent::prerender()
    {
        for (Component* child : m_Children)
        {
            child->prerender();
        }

        updateSnapshotSize();

        Sml::Renderer::getInstance().pushSetTarget(m_Snapshot);

        Sml::Renderer::getInstance().setColor(Sml::COLOR_TRANSPARENT);
        Sml::Renderer::getInstance().clear();
        prerenderSelf();

        Sml::Renderer::getInstance().popTarget();

        updateShadow();
    }

    bool Parent::containsComponent(Component* component)
    {
        assert(component);

        if (component == this)
        {
            return true;
        }
        
        for (auto child : m_Children)
        {
            if (child->containsComponent(component))
            {
                return true;
            }
        }

        return false;
    }

    void Parent::prerenderSelf(){}

    void Parent::layoutChildren()
    {
        for (Component* component : m_Children)
        {
            component->setLayoutWidth(component->computePrefWidth());
            component->setLayoutHeight(component->computePrefHeight());
        }
    }

    bool Parent::isResizable() const { return true; }

    void Parent::addChild(Component* child)
    {
        assert(child);

        for (auto existingChild : m_Children)
        {
            if (existingChild == child)
            {
                return;
            }
        }

        m_Children.push_back(child);
        child->setParent(this);
        child->setScene(getScene());
    }
    
    void Parent::removeChild(Component* child)
    {
        assert(child);
        assert(child->getParent() == this);

        m_Children.remove(child);
        child->setParent(nullptr);
    }

    void Parent::removeChildren()
    {
        m_Children.clear();
    }

    std::list<Component*>& Parent::getChildren() { return m_Children; }
    const std::list<Component*>& Parent::getChildrenReadonly() const { return m_Children; }

    Sml::Texture* Parent::getSnapshot() { return m_Snapshot; }

    bool Parent::needLayoutPass() const { return m_NeedLayoutPass; }
    void Parent::requestLayoutPass() { m_NeedLayoutPass = true; }

    const Background* Parent::getBackground() const { return m_Background; }
    void Parent::setBackground(const Background* background) { m_Background = background; }

    const Border* Parent::getBorder() const { return m_Border; }
    void Parent::setBorder(const Border* border) { m_Border = border; }
    
    Insets Parent::getInsets() const
    {
        return (m_Border == nullptr ? Insets::EMPTY : m_Border->getThickness()) + getPadding();
    }

    Sml::Rectangle<int32_t> Parent::getContentArea() const
    {
        Insets insets = getInsets();
        Sml::Rectangle<int32_t> contentArea = getOriginBounds();
        contentArea.pos    += {insets.left, insets.top};
        contentArea.width  -= insets.left + insets.right;
        contentArea.height -= insets.top + insets.bottom;

        return contentArea;
    }
    
    Insets Parent::getPadding() const { return m_Padding; }
    void Parent::setPadding(const Insets& padding) { m_Padding = padding; }

    void Parent::setPrefWidth (int32_t width)  { m_PrefWidth  = width;  }
    void Parent::setPrefHeight(int32_t height) { m_PrefHeight = height; }

    void Parent::setMinWidth (int32_t width)  { m_MinWidth  = width;  }
    void Parent::setMinHeight(int32_t height) { m_MinHeight = height; }

    void Parent::setMaxWidth (int32_t width)  { m_MaxWidth  = width;  }
    void Parent::setMaxHeight(int32_t height) { m_MaxHeight = height; }

    int32_t Parent::getPrefWidth()  const { return m_PrefWidth;  }
    int32_t Parent::getPrefHeight() const { return m_PrefHeight; }

    int32_t Parent::getMinWidth()  const { return m_MinWidth;  }
    int32_t Parent::getMinHeight() const { return m_MinHeight; }

    int32_t Parent::getMaxWidth()  const { return m_MaxWidth;  }
    int32_t Parent::getMaxHeight() const { return m_MaxHeight; }

    #define CHECK_ENABLED_SIZE(size) if (size != USE_COMPUTED_SIZE) { return size; }

    int32_t Parent::computePrefWidth(int32_t height) const
    {
        CHECK_ENABLED_SIZE(m_PrefWidth);
        return computeCustomPrefWidth(height);
    }

    int32_t Parent::computePrefHeight(int32_t width) const
    {
        CHECK_ENABLED_SIZE(m_PrefHeight);
        return computeCustomPrefHeight(width);
    } 
    
    int32_t Parent::computeMinWidth(int32_t height) const
    {
        CHECK_ENABLED_SIZE(m_MinWidth);
        return computeCustomMinWidth(height);
    }

    int32_t Parent::computeMinHeight(int32_t width) const
    {
        CHECK_ENABLED_SIZE(m_MinHeight);
        return computeCustomMinHeight(width);
    }

    int32_t Parent::computeMaxWidth(int32_t height) const
    { 
        CHECK_ENABLED_SIZE(m_MaxWidth);
        return computeCustomMaxWidth(height);
    }

    int32_t Parent::computeMaxHeight(int32_t width) const
    {
        CHECK_ENABLED_SIZE(m_MaxHeight);
        return computeCustomMaxHeight(width);
    }

    int32_t Parent::computeCustomPrefWidth(int32_t height) const
    {
        COMPONENT_COMPUTE_DIMENSION(Pref, Width, height, X, m_Children, m_PrefWidth,
                                    getInsets().left + getInsets().right);
    }
    int32_t Parent::computeCustomPrefHeight(int32_t width) const
    {
        COMPONENT_COMPUTE_DIMENSION(Pref, Height, width, Y, m_Children, m_PrefHeight,
                                    getInsets().top + getInsets().bottom);
    }
    
    int32_t Parent::computeCustomMinWidth(int32_t height) const { return computePrefWidth(height);  }
    int32_t Parent::computeCustomMinHeight(int32_t width) const { return computePrefHeight(width); }

    int32_t Parent::computeCustomMaxWidth(int32_t height) const { return UNLIMITED_SIZE; }
    int32_t Parent::computeCustomMaxHeight(int32_t width) const { return UNLIMITED_SIZE; }

    void Parent::updateSnapshotSize()
    {
        int32_t width  = m_LayoutBounds.width;
        int32_t height = m_LayoutBounds.height;

        if (width <= 0 || height <= 0)
        {
            return;
        }

        if (m_Snapshot == nullptr)
        {
            m_Snapshot = new Sml::Texture(width, height);
        }
        else if (static_cast<int32_t>(m_Snapshot->getWidth()) < width ||
                 static_cast<int32_t>(m_Snapshot->getHeight()) < height)
        {
            delete m_Snapshot;
            m_Snapshot = new Sml::Texture(width, height);
        }
    }

    void Parent::setSceneInSceneTree(Scene* scene)
    {
        Component::setSceneInSceneTree(scene);

        for (Component* child : m_Children)
        {
            child->setScene(scene);
        }
    }
}