#include <stdio.h>
#include "frame.h"
#include "components/label.h"

// FIXME: Move to some header file!
#define ASSERT_EVENT_TYPE(eventPointer, eventClass) \
    assert(eventPointer->getType() == eventClass::getStaticType());

int main()
{
    initGraphics();

    Font font{"res/OpenSans-Bold.ttf", 16};

    Frame frame{1280, 720, "Hello, Paint!"};
    bool running = true;
    
    Label label{frame.getRenderer(), font, "Oh my god!", COLOR_BLACK};
    label.setX(100);
    label.setY(100);
    label.setSize(label.getPrefSize().width, label.getPrefSize().height);
    label.setForeground(COLOR_GREEN);

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
            ASSERT_EVENT_TYPE(event, WindowCloseEvent);

            printf("WindowCloseEvent\n");

            *m_Running = false;
            event->consume();
        }

    private: 
        bool* m_Running = nullptr;
    };

    LabelOnHover labelOnHoveListener;
    label.getEventDispatcher()->attachHandler({MouseMovedEvent::getStaticType()}, &labelOnHoveListener);

    CloseListener closeListener{&running};
    frame.getEventDispatcher()->attachHandler({WindowCloseEvent::getStaticType()}, &closeListener);

    frame.addComponent(&label);

    frame.updateGraphics();

    while (running)
    {
        frame.proccessSystemEvents();

        frame.show();
    }

    quitGraphics();

    return 0;
}