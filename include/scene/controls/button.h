/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file button.h
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "control.h"
#include "../style/default_skins.h"

namespace Sgl
{
    class Button : public Control
    {
    public:
        Button(const char* label);
        Button(Skin* skin, const char* label);

        const char* getLabel() const;
        void setLabel(const char* label);

        const Image* getIcon() const;
        void setIcon(const Image* icon);

        void setOnAction(ActionListener* listener);
        ActionListener* getOnAction(); // TODO: get rid of

    private:
        const char*     m_Label          = nullptr;
        const Image*    m_Icon           = nullptr;

        ActionListener* m_ActionListener = nullptr;
    };
}