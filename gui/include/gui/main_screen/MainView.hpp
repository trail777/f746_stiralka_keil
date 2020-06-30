#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <BitmapDatabase.hpp>
#include <gui/common/ScrollingMenu/ScrollingMenu.hpp>
#include <gui/common/Programs.hpp>
#include <gui/main_screen/ImageContainer.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <gui/main_screen/TouchForwarder.hpp>
#include <gui_generated/main_screen/MainViewBase.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>

#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#endif

#include <touchgfx/widgets/AnimatedImage.hpp>


class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void handleDragEvent(const DragEvent& evt);

protected:

    virtual void rightButtonClicked();
    virtual void leftButtonClicked();
    virtual void buttonUpClicked();
    virtual void buttonInstructionsClicked();
    virtual void buttonStartClicked();
    virtual void buttonPauseClicked();
    virtual void buttonRunClicked();

    enum
    {
        NUM_ICONS_IN_MENU = 5,
        TOUCH_START_TIMEOUT = 120,
        START_MOVE_ANIMATION_DURATION = 45,
        START_MOVE_ANIMATION_LENGTH = 440,
        START_MOVE_FADE_OUT_DURATION = 10,
        START_MOVE_FADE_IN_DURATION = 20,
        START_MOVE_FLYIN_DURATION = 12,
        START_MOVE_FLYIN_INTERVAL = 12,
        START_MOVE_FLYIN_DELAY = 60
    };

    enum AnimationState
    {
        WAITING, //default
        WAITING_START,
        STARTING,
        WASHING_PREWASH,
        WASHING_WASHING,
        WASHING_RINSING,
        WASHING_SPINNING,
        WASHING_DONE
    } animationState;

    //center Menu
    ScrollingMenu centerMenu;
    SparseContainerItems<ImageContainer, NUM_ICONS_IN_MENU> centerMenuItems;
    Callback<MainView, int, int> initializeCenterMenuItemCallback;
    void initializeCenterMenuItem(int containerIndex, int itemIndex);

    //back Menu
    ScrollingMenu backMenu;
    SparseContainerItems<ImageContainer, NUM_ICONS_IN_MENU> backMenuItems;
    Callback<MainView, int, int> initializeBackMenuItemCallback;
    void initializeBackMenuItem(int containerIndex, int itemIndex);
    MoveAnimator<Image> leftReplacementImage;
    MoveAnimator<Image> rightReplacementImage;

    //touch overlay
    TouchForwarder touchOverlay;

    //fly in icons
    MoveAnimator<Image> icon1, icon2, icon3, icon4, icon5, icon6;

    //center out and wash animation
    AnimatedImage centerAnimation;
    Image washDoneBackground;
    FadeAnimator<AnimatedImage> washAnimation;
    Callback<MainView, const AnimatedImage&> centerAnimatedImageCallback;
    void centerAnimatedImageDone(const AnimatedImage&);

    //washing status icons
    Image prewashIcon, washingIcon, rinsingIcon, spinCycleIcon;

    //washing progress bar
    FadeAnimator<Circle> progressBar;

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 colorPainter;
#elif USE_BPP==24
    PainterRGB888  colorPainter;
#endif

    //helpers
    BitmapId getBitmapForProgram(int index, bool isMedium);
    void updateProgramTexts();
    void fadeOutProgramTexts();
    void fadeOutProgramTextsNow();
    Callback<MainView, int> centerMenuAnimationEndedCallback;
    void centerMenuAnimationEnded(int index);
    Callback<MainView, const FadeAnimator<TextArea>& > textDashFadeCallback;
    void textDashFadeDone(const FadeAnimator<TextArea>& txt);

    void gotoWaiting();
    void gotoWaitingStart();
    void gotoStarting();
    void gotoWashingPrewash();
    void gotoWashingWashing();
    void gotoWashingRinsing();
    void gotoWashingSpinning();
    void gotoWashingDone();

    //state
    WasherProgram program;
    bool buttonClicked;
    bool wasAnimating;
    bool paused;
    uint32_t touchTimer;
    uint32_t animationTimer;
};

#endif // MAIN_VIEW_HPP
