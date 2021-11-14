/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file button.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/controls/button.h"

namespace Sgl
{
    Button::Button(const char* label) : m_Label(label)
    {
        m_DefaultSkin = new DefaultSkins::ButtonSkin(this);
        setSkin(m_DefaultSkin);
    }

    Button::Button(BaseSkin<Button>* skin, const char* label) : m_Label(label)
    {
        assert(skin);
        skin->attach(this);
        setSkin(skin);
    }


    const char* Button::getLabel() const { return m_Label; }
    void Button::setLabel(const char* label) { m_Label = label; }

    const Image* Button::getIcon() const { return m_Icon; }
    void Button::setIcon(const Image* icon) { m_Icon = icon; }

    void Button::setOnAction(ActionListener<Button>* listener)
    {
        assert(listener);
        m_ActionListener = listener;
        m_ActionListener->setControl(this);
        m_Dispatcher.attachHandler({ActionEvent::getStaticType()}, listener);
    }

    ActionListener<Button>* Button::getOnAction() { return m_ActionListener; }
}