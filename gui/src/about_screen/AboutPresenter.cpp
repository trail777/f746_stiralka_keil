#include <gui/about_screen/AboutView.hpp>
#include <gui/about_screen/AboutPresenter.hpp>

AboutPresenter::AboutPresenter(AboutView& v)
    : view(v)
{
}

void AboutPresenter::activate()
{
    model->setScreenSaverEnabled(true);
}

void AboutPresenter::deactivate()
{

}
