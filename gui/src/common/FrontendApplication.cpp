#include <gui/common/FrontendApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
}

void FrontendApplication::handleTickEvent()
{
    model.tick();
    FrontendApplicationBase::handleTickEvent();
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    MVPApplication::handleClickEvent(evt);

    // A click event has been registered so reset last click time
    model.resetScreenSaverCounter();
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{
    MVPApplication::handleDragEvent(evt);

    // A drag event has been registered so reset last click time
    model.resetScreenSaverCounter();
}


int FrontendApplication::getCurrentScreen()
{
    return model.getCurrentScreen();
}

void FrontendApplication::setCurrentScreen(int screen)
{
    model.setCurrentScreen(screen);
}
