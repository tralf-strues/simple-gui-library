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

namespace Sgl
{
    class Parent : public Component
    {
    public:
        Parent() = default;
        virtual ~Parent() override = default;

        virtual void layout() override final;
        virtual bool isResizable() const override;

        std::list<Component*> getChildren();

        bool needLayoutPass() const;
        void requestLayoutPass();

        const Background* getBackground() const;
        void setBackground(const Background* background);

        const Border* getBorder() const;
        void setBorder(const Border* border);

        Insets getInsets() const;
        
        Insets getPadding() const;
        void setPadding(const Insets& padding);

        void setPrefWidth(int32_t width);
        void setPrefHeight(int32_t height);

        void setMinWidth(int32_t width);
        void setMinHeight(int32_t height);

        void setMaxWidth(int32_t width);
        void setMaxHeight(int32_t height);

        /* Default computeSize functions don't account for insets! */
        virtual int32_t computePrefWidth(int32_t height = -1) override;
        virtual int32_t computePrefHeight(int32_t width = -1) override;
        
        virtual int32_t computeMinWidth(int32_t height = -1) override;
        virtual int32_t computeMinHeight(int32_t width = -1) override;

        virtual int32_t computeMaxWidth(int32_t height = -1) override;
        virtual int32_t computeMaxHeight(int32_t width = -1) override;

    protected:
        std::list<Component*> m_Children;

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

        virtual void layoutChildren();
    };
}