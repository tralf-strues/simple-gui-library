#include <stdio.h>
#include "frame.h"
#include "components/button.h"
#include "style/default_skins.h"

// FIXME: Move to some header file!
#define ASSERT_EVENT_TYPE(eventPointer, eventClass) \
    assert(eventPointer->getType() == eventClass::getStaticType());

class LabelOnHover : public Listener
{
    virtual void onEvent(Event* event) override
    {
        ASSERT_EVENT_TYPE(event, MouseMovedEvent);

        MouseMovedEvent* mouseEvent = static_cast<MouseMovedEvent*>(event);
        printf("MouseMovedEvent: <%d, %d>\n", mouseEvent->getX(), mouseEvent->getY());

        mouseEvent->consume();
    }
};

class CloseListener : public Listener
{
public:
    CloseListener(bool* running) { assert(running); m_Running = running; }

    virtual void onEvent(Event* event) override
    {
        printf("WindowCloseEvent\n");

        *m_Running = false;
        event->consume();
    }

private: 
    bool* m_Running = nullptr;
};

int main()
{
    initGraphics();

    Font font{"res/OpenSans-Regular.ttf", 14};

    Frame frame{1280, 720, "Hello, Paint!"};
    frame.setBackground(COLOR_WHITE);
    bool running = true;
    
    Label label{frame.getRenderer(), font, "Oh my god!", COLOR_BLACK};
    // label.setBackground(COLOR_YELLOW);
    label.setX(100);
    label.setY(100);
    label.setSize(label.getPrefSize().x, label.getPrefSize().y);

    LabelOnHover labelOnHoveListener;
    label.getEventDispatcher()->attachHandler({MouseMovedEvent::getStaticType()}, &labelOnHoveListener);

    Button button{frame.getRenderer(), font};
    button.setSkin(&DEFAULT_SKIN_BUTTON);
    button.setX(300);
    button.setY(100);
    button.setSize(128, 32);
    button.setLabel("Quit");

    CloseListener closeListener{&running};
    button.getEventDispatcher()->attachHandler({MouseButtonPressedEvent::getStaticType()}, &closeListener);
    frame.getEventDispatcher()->attachHandler({WindowCloseEvent::getStaticType()}, &closeListener);

    frame.addComponent(&label);
    frame.addComponent(&button);

    frame.updateGraphics();

    while (running)
    {
        frame.proccessSystemEvents();
        frame.show();

        // frame.render(nullptr, Rectangle<int32_t>{Vec2<int32_t>{0, 0}, frame.getPos().x, frame.getPos().y});

        // frame.getRenderer()->setColor(COLOR_WHITE);
        // frame.getRenderer()->clear();
        // text.render(*frame.getRenderer(), Vec2<int32_t>{100, 100});
        // label.render(nullptr, Rectangle<int32_t>{Vec2<int32_t>{0, 0}, frame.getPos().x, frame.getPos().y});

        // frame.getRenderer()->present();
    }

    quitGraphics();

    return 0;
}