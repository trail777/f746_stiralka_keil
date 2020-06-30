#ifndef ABOUT_VIEW_HPP
#define ABOUT_VIEW_HPP

#include <gui_generated/about_screen/AboutViewBase.hpp>
#include <gui/about_screen/AboutPresenter.hpp>

#include <touchgfx/hal/Types.hpp>

class AboutView : public AboutViewBase
{
public:
    AboutView();
    virtual ~AboutView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

protected:
    enum
    {
        POINTER_Y = 50,
        POINTER_X_TGFX = 33,
        ANIMATION_TIME = 60
    };

    Image pointer;
    Image text;

    uint32_t tickCounter;
    bool animate;

    uint32_t dist(uint32_t x0, uint32_t y0, uint32_t x, uint32_t y);
    void setStateTGFX();

};

#endif // ABOUT_VIEW_HPP
