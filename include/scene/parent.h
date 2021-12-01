/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file parent.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "component.h"
#include "background.h"
#include "border.h"
#include "style/shadow.h"

namespace Sgl
{
    class Parent : public Component
    {
    public:
        Parent() = default;
        virtual ~Parent() override = default;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;
        virtual void layout() override final;
        virtual void prerender() override final;

        virtual bool isResizable() const override;

        /**
         * @brief Convenience method for adding a child to @ref Parent.
         * 
         * @attention For @ref Control this should only be used by custom skins! It's undefined behavior
         *            if you simply add children to a @ref Control. For @ref Container this method can
         *            easily be used for adding any number of custom components.
         * 
         * @param child
         */
        void addChild(Component* child);

        /**
         * @brief Convenience method for removing a child to @ref Parent.
         * 
         * @attention For @ref Control this should only be used by custom skins! It's undefined behavior
         *            if you simply remove children from a @ref Control. For @ref Container this method
         *            can easily be used for removing children from it.
         * 
         * @param child
         */
        void removeChild(Component* child);

        std::list<Component*>& getChildren();
        const std::list<Component*>& getChildrenReadonly() const;

        Sml::Texture* getSnapshot();

        bool needLayoutPass() const;
        void requestLayoutPass();

        const Background* getBackground() const;
        void setBackground(const Background* background);

        const Border* getBorder() const;
        void setBorder(const Border* border);

        Insets getInsets() const;
        Sml::Rectangle<int32_t> getContentArea() const;
        
        Insets getPadding() const;
        void setPadding(const Insets& padding);

        void setPrefWidth(int32_t width);
        void setPrefHeight(int32_t height);

        void setMinWidth(int32_t width);
        void setMinHeight(int32_t height);

        void setMaxWidth(int32_t width);
        void setMaxHeight(int32_t height);

        int32_t getPrefWidth() const;
        int32_t getPrefHeight() const;

        int32_t getMinWidth() const;
        int32_t getMinHeight() const;

        int32_t getMaxWidth() const;
        int32_t getMaxHeight() const;

        virtual int32_t computePrefWidth(int32_t height = -1) const override final;
        virtual int32_t computePrefHeight(int32_t width = -1) const override final;
        
        virtual int32_t computeMinWidth(int32_t height = -1) const override final;
        virtual int32_t computeMinHeight(int32_t width = -1) const override final;

        virtual int32_t computeMaxWidth(int32_t height = -1) const override final;
        virtual int32_t computeMaxHeight(int32_t width = -1) const override final;

    protected:
        std::list<Component*> m_Children;
        Sml::Texture*         m_Snapshot       = nullptr;

        bool                  m_NeedLayoutPass = false;
        const Background*     m_Background     = nullptr;
        const Border*         m_Border         = nullptr;
        Insets                m_Padding        = Insets::EMPTY;

        int32_t               m_PrefWidth      = USE_COMPUTED_SIZE;
        int32_t               m_PrefHeight     = USE_COMPUTED_SIZE;
        int32_t               m_MinWidth       = USE_COMPUTED_SIZE;
        int32_t               m_MinHeight      = USE_COMPUTED_SIZE;
        int32_t               m_MaxWidth       = USE_COMPUTED_SIZE;
        int32_t               m_MaxHeight      = USE_COMPUTED_SIZE;

        virtual void setSceneInSceneTree(Scene* scene) override final;

        virtual void layoutChildren();
        virtual void prerenderSelf();
        void updateSnapshotSize();

        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const;
        
        virtual int32_t computeCustomMinWidth(int32_t height = -1) const;
        virtual int32_t computeCustomMinHeight(int32_t width = -1) const;

        virtual int32_t computeCustomMaxWidth(int32_t height = -1) const;
        virtual int32_t computeCustomMaxHeight(int32_t width = -1) const;
    };
}