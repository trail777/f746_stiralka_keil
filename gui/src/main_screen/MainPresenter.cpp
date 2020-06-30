#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{
}

void MainPresenter::activate()
{
    model->setScreenSaverEnabled(true);
}

void MainPresenter::deactivate()
{

}
