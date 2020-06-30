#ifndef SCREENSAVER_PRESENTER_HPP
#define SCREENSAVER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenSaverView;

class ScreenSaverPresenter : public Presenter, public ModelListener
{
public:
    ScreenSaverPresenter(ScreenSaverView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenSaverPresenter() {};

    // Ignore goto screen saver mode
    virtual void gotoScreenSaverMode() {}

private:
    ScreenSaverPresenter();

    ScreenSaverView& view;
};


#endif // SCREENSAVER_PRESENTER_HPP
