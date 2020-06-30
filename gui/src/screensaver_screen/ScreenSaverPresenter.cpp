#include <gui/screensaver_screen/ScreenSaverView.hpp>
#include <gui/screensaver_screen/ScreenSaverPresenter.hpp>

ScreenSaverPresenter::ScreenSaverPresenter(ScreenSaverView& v)
    : view(v)
{
}

void ScreenSaverPresenter::activate()
{
    model->setScreenSaverEnabled(true);
}

void ScreenSaverPresenter::deactivate()
{

}
