/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <new>
#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <gui/about_screen/AboutView.hpp>
#include <gui/about_screen/AboutPresenter.hpp>
#include <gui/video_screen/VideoView.hpp>
#include <gui/video_screen/VideoPresenter.hpp>
#include <gui/screensaver_screen/ScreenSaverView.hpp>
#include <gui/screensaver_screen/ScreenSaverPresenter.hpp>

using namespace touchgfx;

FrontendApplicationBase::FrontendApplicationBase(Model& m, FrontendHeap& heap)
    : touchgfx::MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m)
{
    touchgfx::HAL::getInstance()->setDisplayOrientation(touchgfx::ORIENTATION_LANDSCAPE);
    touchgfx::Texts::setLanguage(GB);
    reinterpret_cast<touchgfx::LCD16bpp&>(touchgfx::HAL::lcd()).enableTextureMapperAll();
}

/*
 * Screen Transition Declarations
 */

// Main

void FrontendApplicationBase::gotoMainScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoMainScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoMainScreenNoTransitionImpl()
{
    touchgfx::makeTransition<MainView, MainPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// About

void FrontendApplicationBase::gotoAboutScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoAboutScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoAboutScreenNoTransitionImpl()
{
    touchgfx::makeTransition<AboutView, AboutPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Video

void FrontendApplicationBase::gotoVideoScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoVideoScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoVideoScreenNoTransitionImpl()
{
    touchgfx::makeTransition<VideoView, VideoPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// ScreenSaver

void FrontendApplicationBase::gotoScreenSaverScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreenSaverScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreenSaverScreenNoTransitionImpl()
{
    touchgfx::makeTransition<ScreenSaverView, ScreenSaverPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
