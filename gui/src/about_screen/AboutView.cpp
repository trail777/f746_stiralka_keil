#include <gui/about_screen/AboutView.hpp>
#include <Bitmapdatabase.hpp>

AboutView::AboutView()
    : tickCounter(0), animate(true)
{

}

void AboutView::setupScreen()
{
    text.setXY(0, 272 - 90);
    text.setBitmap(Bitmap(BITMAP_TOUCHGFX_TXT_ID));
    add(text);
    remove(box);
    add(box);
    box.setVisible(false);
    pointer.setBitmap(Bitmap(BITMAP_ABOUT_SELECTED_ID));
    pointer.setXY(POINTER_X_TGFX, POINTER_Y);
    add(pointer);
    setStateTGFX();
}

void AboutView::tearDownScreen()
{

}

uint32_t AboutView::dist(uint32_t x0, uint32_t y0, uint32_t x, uint32_t y)
{
    return (x - x0) * (x - x0) + (y - y0) * (y - y0);
}

void AboutView::setStateTGFX()
{
    tickCounter = 0;
    box.setVisible(false);
    animate = false;
    text.setBitmap(Bitmap(BITMAP_TOUCHGFX_TXT_ID));
    text.invalidate();
    pointer.moveTo(POINTER_X_TGFX, POINTER_Y);
}
