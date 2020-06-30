#include <gui/screensaver_screen/ScreenSaverView.hpp>

ScreenSaverView::ScreenSaverView() :
    particleEffect()
{
    HAL::getInstance()->setFrameRateCompensation(true);
}

void ScreenSaverView::setupScreen()
{
    particleEffect.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    particleEffect.setSnapToOrigin(true);
    particleEffect.setNumberOfParticles(180);
    particleEffect.setOrigin(particleEffect.getWidth() / 2, particleEffect.getHeight() / 2);
    particleEffect.setCenter(particleEffect.getWidth() / 2, particleEffect.getHeight() / 2);

    // Insert particleEffect behind the logo and quitButton (z-order)
    container.insert(&background,  particleEffect);
    static_cast<FrontendApplication*>(Application::getInstance())->setCurrentScreen(Model::VIEW_SAVER);
}

void ScreenSaverView::tearDownScreen()
{

}

void ScreenSaverView::endScreenSaverAnimation()
{
    particleEffect.spawnNewParticles(false);
    quitButton.setTouchable(false);
}
