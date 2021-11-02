#include <stdio.h>
#include "frame.h"
#include "components/button.h"
#include "style/default_skins.h"
#include "containers/spaced_container.h"
#include "containers/list_menu.h"
#include "containers/menu_bar.h"

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

    Font font{"res/OpenSans-Regular.ttf", 16};

    Frame frame{1280, 720, "Hello, Paint!"};
    frame.getWindow()->setBorderless();
    frame.setBackground(COLOR_BLUE);
    bool running = true;

    Texture iconTextureClose{frame.getRenderer()};
    iconTextureClose.loadFromImage("res/icons8-macos-close-96.png");
    IconSkin iconClose{&iconTextureClose};
    
    Label label{frame.getRenderer(), font, "Oh my god!", COLOR_BLACK};
    label.setY(16);

    Label label2{frame.getRenderer(), font, "Spacers work!", COLOR_BLUE};

    Button button{frame.getRenderer(), font};
    button.setSize(128, 22);
    button.setLabel("Quit");

    Button closeButton{frame.getRenderer(), font};
    closeButton.setX(250);
    closeButton.setY(250);
    closeButton.setSize(50, 50);
    closeButton.setSkin(&iconClose);

    SpacedContainer spacedContainer{frame.getRenderer()};
    spacedContainer.setY(400);
    spacedContainer.setSize(1280, 12);
    spacedContainer.setBackground(COLOR_GREEN);

    label.attachHandler({MouseMovedEvent::getStaticType()}, new LabelOnHover{});

    button.attachHandler({MouseButtonPressedEvent::getStaticType()}, new CloseListener{&running});
    frame.attachHandler({WindowCloseEvent::getStaticType()}, new CloseListener{&running});

    spacedContainer.addComponent(&label2);
    spacedContainer.addComponent(&label);
    spacedContainer.addSpacer();
    // spacedContainer.addComponent(&button);
    spacedContainer.insertSpacerAfter(&label2, 3);

    // ListMenu listMenu{frame.getRenderer()};
    // listMenu.addComponent(new Label(frame.getRenderer(), font, "Item 1"));
    // listMenu.addComponent(new Label(frame.getRenderer(), font, "Item 2"));
    // listMenu.addComponent(new Label(frame.getRenderer(), font, "Item 3"));
    // listMenu.addComponent(new Label(frame.getRenderer(), font, "Item 4"));
    // listMenu.setSize(listMenu.getPrefSize().x, listMenu.getPrefSize().y);
    // listMenu.setX(50);
    // listMenu.setY(250);

    MenuBar menuBar{frame.getRenderer(), font};
    menuBar.setSize(1280, button.getHeight());
    Menu* fileMenu = menuBar.addMenu("File");
    fileMenu->getMenuItems()->addComponent(new Label{frame.getRenderer(), font, "Open"});
    fileMenu->getMenuItems()->addComponent(new Label{frame.getRenderer(), font, "New"});
    fileMenu->getMenuItems()->addComponent(new Label{frame.getRenderer(), font, "Close"});

    menuBar.addMenu("Edit");
    menuBar.addMenu("View");

    menuBar.addSpacer();
    menuBar.addComponent(&button);

    frame.addComponent(&closeButton);
    frame.addComponent(&spacedContainer);
    frame.addComponent(&menuBar);
    // frame.addComponent(&listMenu);

    frame.updateGraphics();
    // frame.updateGraphics();

    while (running)
    {
        frame.proccessSystemEvents();
        frame.show();
    }

    quitGraphics();

    return 0;
}