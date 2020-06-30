#include <gui/main_screen/MainView.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/Color.hpp>
#include <stdio.h>

MainView::MainView() :
    animationState(WAITING),
    initializeCenterMenuItemCallback(this, &MainView::initializeCenterMenuItem),
    initializeBackMenuItemCallback(this, &MainView::initializeBackMenuItem),
    touchOverlay(centerMenu, backMenu),
    centerAnimatedImageCallback(this, &MainView::centerAnimatedImageDone),
    centerMenuAnimationEndedCallback(this, &MainView::centerMenuAnimationEnded),
    textDashFadeCallback(this, &MainView::textDashFadeDone),
    program(PRG_ECOWASH), buttonClicked(false), wasAnimating(false),
    paused(false), touchTimer(0)
{
}

void MainView::setupScreen()
{
    const int margin = 22;
    //Center Menu
    centerMenu.setXY(imageWhiteCenter.getX() + 40 - margin, imageWhiteCenter.getY() + 40);
    centerMenu.setup(true,              //horizontal
                     true,              //isCircular
                     100 + margin,      //width
                     100,               //height
                     0,                 //selectedItemOffset
                     100,               //containerSize
                     2 * margin,        //containerSpacing
                     NUM_ICONS_IN_MENU, //numItems
                     2,                 //numContainers
                     centerMenuItems.getElementCallback,
                     initializeCenterMenuItemCallback);
    centerMenu.setAnimationEndedCallback(centerMenuAnimationEndedCallback);
    centerMenu.setSwipeAcceleration(60);

    container.insert(&topbar, centerMenu);

    //Back menu
    //must match center menu to swipe nice, 90+54 = 100+2*margin
    backMenu.setXY(46, 82);
    backMenu.setup(true,              //horizontal
                   true,              //isCircular
                   388,               //width
                   110,               //height
                   -20,                 //selectedItemOffset
                   90,                //containerSize
                   54,                //containerSpacing
                   NUM_ICONS_IN_MENU, //numItems
                   5,                 //numContainers
                   backMenuItems.getElementCallback,
                   initializeBackMenuItemCallback);
    backMenu.setSwipeAcceleration(60);

    container.insert(&background, backMenu);

    //center animation
    centerAnimation.setXY(104, 28);
    centerAnimation.setBitmaps(BITMAP_COMP_00000_ID, BITMAP_COMP_00026_ID);
    centerAnimation.setUpdateTicksInterval(2);
    centerAnimation.setDoneAction(centerAnimatedImageCallback);
    centerAnimation.setVisible(false);
    container.insert(&centerMenu, centerAnimation);

    container.insert(&centerAnimation, leftReplacementImage);
    container.insert(&centerAnimation, rightReplacementImage);

    add(icon1);
    add(icon2);
    add(icon3);
    add(icon4);
    add(icon5);
    add(icon6);

    prewashIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_PREWASH_INACTIVE_ID));
    prewashIcon.setXY(35, 15);
    prewashIcon.setVisible(false);
    add(prewashIcon);
    washingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_WASHING_INACTIVE_ID));
    washingIcon.setXY(113, 15);
    washingIcon.setVisible(false);
    add(washingIcon);
    rinsingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_RINSING_INACTIVE_ID));
    rinsingIcon.setXY(293, 15);
    rinsingIcon.setVisible(false);
    add(rinsingIcon);
    spinCycleIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_SPIN_CYCLE_INACTIVE_ID));
    spinCycleIcon.setXY(360, 15);
    spinCycleIcon.setVisible(false);
    add(spinCycleIcon);

    //select items
    centerMenu.setSelectedItem(PRG_ECOWASH);
    backMenu.setSelectedItem(centerMenu.getSelectedItem() - 1);
    program = PRG_ECOWASH;

    touchOverlay.setPosition(46, 90, 388, 110);
    add(touchOverlay);

    //button start on top
    remove(buttonStart);
    add(buttonStart);

    //wash animation
    washDoneBackground.setXY(190, 86);
    washDoneBackground.setBitmap(Bitmap(BITMAP_BLUE_DONE_ID));
    washDoneBackground.setVisible(false);
    add(washDoneBackground);
    washAnimation.setXY(174, 70);
    washAnimation.setAlpha(0);
    washAnimation.setBitmaps(BITMAP_WASH_FRAME_01_ID,  BITMAP_WASH_FRAME_11_ID);
    washAnimation.setUpdateTicksInterval(4);
    add(washAnimation);

    colorPainter.setColor(Color::getColorFrom24BitRGB(0x45, 0x4d, 0x56), 255);
    progressBar.setPosition(134, 30, 210, 210);
    progressBar.setCenter((progressBar.getWidth() + 1) / 2.0f, progressBar.getHeight() / 2.0f);
    progressBar.setRadius((progressBar.getWidth() - 15) / 2.0f);
    progressBar.setLineWidth(15);
    progressBar.setPainter(colorPainter);
    progressBar.setCapPrecision(15);
    progressBar.setArc(0, 0);
    progressBar.setVisible(false);
    add(progressBar);

    remove(buttonUp);

    //Headline text
    updateProgramTexts();
    static_cast<FrontendApplication*>(Application::getInstance())->setCurrentScreen(Model::VIEW_MAIN);
}

void MainView::tearDownScreen()
{
}

void MainView::initializeCenterMenuItem(int containerIndex, int itemIndex)
{
    centerMenuItems.element[containerIndex].setBitmap(Bitmap(getBitmapForProgram(itemIndex, true)));
}

void MainView::centerMenuAnimationEnded(int index)
{
}

void MainView::initializeBackMenuItem(int containerIndex, int itemIndex)
{
    backMenuItems.element[containerIndex].setBitmap(Bitmap(getBitmapForProgram(itemIndex, false)));
}

void MainView::handleTickEvent()
{
    if (touchOverlay.touchIsDown())
    {
        touchTimer = 0;
    }
    else
    {
        touchTimer++;
    }

    switch (animationState)
    {
    case WAITING:
        {
            WasherProgram old = program;
            program = (WasherProgram)centerMenu.getSelectedItem();
            if (old != program)
            {
                //touchgfx_printf("changed to program=%d %d\n", program, touchOverlay.touchIsDown());
                if (touchOverlay.touchIsDown())
                {
                    //update immediately when dragging
                    updateProgramTexts();
                }
                else
                {
                    //fade out without delay + fade in
                    fadeOutProgramTextsNow();
                }
            }

            //evaluate go to start
            if (touchTimer == TOUCH_START_TIMEOUT)
            {
                gotoWaitingStart();
            }

            break;
        }
    case WAITING_START:
        break;
    case WASHING_PREWASH:
    case WASHING_WASHING:
    case WASHING_RINSING:
    case WASHING_SPINNING:
        {
            if (paused)
            {
                break;
            }
            if (++animationTimer < 30)
            {
                break;
            }
            float angle = (animationTimer - 30) * 0.6f;
            progressBar.setArc(0, static_cast<int16_t>(angle));
            progressBar.invalidate();
            if (angle > 360)
            {
                gotoWashingDone();
            }
            else if (angle > 270)
            {
                gotoWashingSpinning();
            }
            else if (angle > 180)
            {
                gotoWashingRinsing();
            }
            else if (angle > 90)
            {
                gotoWashingWashing();
            }

            break;
        }
    case WASHING_DONE:
        {
            if (++animationTimer < 30)
            {
                break;
            }

            if (animationTimer == 30)
            {
                progressBar.setVisible(false);
                progressBar.invalidate();
            }

            if (animationTimer == 60)
            {
                centerAnimation.startAnimation(true, false, false);
            }

            if (animationTimer == 180)
            {
                static_cast<FrontendApplication*>(Application::getInstance())->gotoMainScreenNoTransition();
            }
        }
    default:
        break;
    }
}

void MainView::handleDragEvent(const DragEvent& evt)
{
    View::handleDragEvent(evt);
    switch (animationState)
    {
    case WAITING_START:
        gotoWaiting();
        break;
    default:
        break;
    }
}

void MainView::rightButtonClicked()
{
    centerMenu.setSelectedItem(centerMenu.getSelectedItem() + 1);
    backMenu.setSelectedItem(centerMenu.getSelectedItem() - 1); //back is one behind
    program = (WasherProgram)centerMenu.getSelectedItem();
    fadeOutProgramTexts();
    gotoWaiting();
}

void MainView::leftButtonClicked()
{
    centerMenu.setSelectedItem(centerMenu.getSelectedItem() - 1);
    backMenu.setSelectedItem(centerMenu.getSelectedItem() - 1); //back is one behind
    program = (WasherProgram)centerMenu.getSelectedItem();
    fadeOutProgramTexts();
    gotoWaiting();
}

void MainView::buttonUpClicked()
{
}

void MainView::buttonInstructionsClicked()
{
}

BitmapId MainView::getBitmapForProgram(int index, bool isMedium)
{
    if (isMedium)
    {
        switch (index)
        {
        case 0:
            return BITMAP_ICON_CENTER_ACTIVEWEAR_MEDIUM_ID;
        case 1:
            return BITMAP_ICON_CENTER_DELICATES_MEDIUM_ID;
        case 2:
            return BITMAP_ICON_CENTER_ECOWASH_MEDIUM_ID;
        case 3:
            return BITMAP_ICON_CENTER_HEAVYDUTY_MEDIUM_ID;
        case 4:
            return BITMAP_ICON_CENTER_NORMAL_MEDIUM_ID;
        case 5:
            return BITMAP_ICON_CENTER_QUICKWASH_MEDIUM_ID;
        case 6:
            return BITMAP_ICON_CENTER_SMARTWASH_MEDIUM_ID;
        }
    }
    else
    {
        switch (index)
        {
        case 0:
            return BITMAP_ICON_CENTER_ACTIVEWEAR_ID;
        case 1:
            return BITMAP_ICON_CENTER_DELICATES_ID;
        case 2:
            return BITMAP_ICON_CENTER_ECOWASH_ID;
        case 3:
            return BITMAP_ICON_CENTER_HEAVYDUTY_ID;
        case 4:
            return BITMAP_ICON_CENTER_NORMAL_ID;
        case 5:
            return BITMAP_ICON_CENTER_QUICKWASH_ID;
        case 6:
            return BITMAP_ICON_CENTER_SMARTWASH_ID;
        }
    }
    return BITMAP_INVALID;
}

void MainView::updateProgramTexts()
{
    Unicode::strncpy(textAreaProgramNameBuffer, getProgramName(program), TEXTAREAPROGRAMNAME_SIZE);
    textAreaProgramName.invalidate();

    char time[5];
    int totalTime = getProgramTime(program);
    sprintf(time, "%d:%02d", totalTime / 60, totalTime % 60);
    Unicode::strncpy(textAreaProgramLengthBuffer, time, 5);

    textAreaProgramName.setFadeAnimationDelay(0);
    textAreaProgramName.startFadeAnimation(255, 5);
    textAreaDash.setFadeAnimationDelay(0);
    textAreaDash.startFadeAnimation(255, 5);
    textAreaProgramLength.setFadeAnimationDelay(0);
    textAreaProgramLength.startFadeAnimation(255, 5);
}

void MainView::textDashFadeDone(const FadeAnimator<TextArea>& txt)
{
    switch (animationState)
    {
    case WAITING:
        if (textAreaDash.getAlpha() == 0)
        {
            updateProgramTexts();
        }
        break;
    case WAITING_START:
        break;
    case STARTING:
        prewashIcon.setVisible(true);
        prewashIcon.invalidate();
        washingIcon.setVisible(true);
        washingIcon.invalidate();
        rinsingIcon.setVisible(true);
        rinsingIcon.invalidate();
        spinCycleIcon.setVisible(true);
        spinCycleIcon.invalidate();
        break;
    case WASHING_PREWASH:
    case WASHING_WASHING:
    case WASHING_RINSING:
    case WASHING_SPINNING:
    case WASHING_DONE:
        break;
    }
}

void MainView::fadeOutProgramTextsNow()
{
    const int duration = 10;
    const int delay = 0;
    textAreaProgramName.setFadeAnimationDelay(delay);
    textAreaProgramName.startFadeAnimation(0, duration);
    textAreaDash.setFadeAnimationDelay(delay);
    textAreaDash.startFadeAnimation(0, duration);
    textAreaProgramLength.setFadeAnimationDelay(delay);
    textAreaProgramLength.startFadeAnimation(0, duration);

    textAreaDash.setFadeAnimationEndedAction(textDashFadeCallback);
}

void MainView::fadeOutProgramTexts()
{
    //only start fade once
    if (!textAreaDash.isRunning())
    {
        const int duration = 10;
        const int delay = 15;
        textAreaProgramName.setFadeAnimationDelay(delay);
        textAreaProgramName.startFadeAnimation(0, duration);
        textAreaDash.setFadeAnimationDelay(delay);
        textAreaDash.startFadeAnimation(0, duration);
        textAreaProgramLength.setFadeAnimationDelay(delay);
        textAreaProgramLength.startFadeAnimation(0, duration);

        textAreaDash.setFadeAnimationEndedAction(textDashFadeCallback);
    }
}

void MainView::gotoWaiting()
{
    AnimationState old = animationState;
    touchgfx_printf("gotoWaiting\n");
    animationState = WAITING;
    touchTimer = 0;
    touchOverlay.setDragable(true);
    buttonStart.startFadeAnimation(0, 10);

    if (old != WAITING_START)
    {
        backMenu.setVisible(true);
        backMenu.invalidate();
        leftShade.setVisible(true);
        leftShade.invalidate();
        rightShade.setVisible(true);
        rightShade.invalidate();
    }
}

void MainView::gotoWaitingStart()
{
    touchgfx_printf("gotoWaitingStart\n");
    animationState = WAITING_START;
    buttonStart.setAlpha(0);
    buttonStart.startFadeAnimation(255, 10);
}

void MainView::buttonStartClicked()
{
    gotoStarting();
}

void MainView::buttonPauseClicked()
{
    paused = true;
    washAnimation.stopAnimation();
}

void MainView::buttonRunClicked()
{
    paused = false;
    washAnimation.startAnimation(false, false, true);
}

void MainView::gotoStarting()
{
    touchgfx_printf("gotoStarting\n");
    animationState = STARTING;
    touchOverlay.setDragable(false);

    //top headline
    fadeOutProgramTextsNow();

    //insert left replacement image
    ImageContainer* selected;
    backMenuItems.getElement(backMenu.getSelectedItem(), (Container**)&selected);
    leftReplacementImage.setBitmap(Bitmap(selected->image.getBitmap()));
    leftReplacementImage.setX(selected->getX() + backMenu.getX());
    leftReplacementImage.setY(backMenu.getY());
    leftReplacementImage.startMoveAnimation(leftReplacementImage.getX() - START_MOVE_ANIMATION_LENGTH, leftReplacementImage.getY(),
                                            START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);

    //right replacement
    backMenuItems.getElement((backMenu.getSelectedItem() + 2) % NUM_ICONS_IN_MENU, (Container**)&selected);
    rightReplacementImage.setBitmap(Bitmap(selected->image.getBitmap()));
    rightReplacementImage.setX(selected->getX() + backMenu.getX());
    rightReplacementImage.setY(backMenu.getY());
    rightReplacementImage.startMoveAnimation(rightReplacementImage.getX() + START_MOVE_ANIMATION_LENGTH, rightReplacementImage.getY(),
                                             START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);

    //buttons
    leftButton.startMoveAnimation(leftButton.getX() - START_MOVE_ANIMATION_LENGTH, leftButton.getY(),
                                  START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);
    rightButton.startMoveAnimation(rightButton.getX() + START_MOVE_ANIMATION_LENGTH, leftButton.getY(),
                                   START_MOVE_ANIMATION_DURATION, EasingEquations::cubicEaseIn);

    //remove backMenu and shades
    backMenu.setVisible(false);
    backMenu.invalidate();
    leftShade.setVisible(false);
    leftShade.invalidate();
    rightShade.setVisible(false);
    rightShade.invalidate();

    //remove low elements
    imageRPM.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
    imageHeat.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
    imageLevel.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
    imageOption1.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
    imageOption2.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);
    imageOption3.startFadeAnimation(0, START_MOVE_FADE_OUT_DURATION);

    //running & Pause
    buttonRun.setFadeAnimationDelay(START_MOVE_ANIMATION_DURATION);
    buttonRun.startFadeAnimation(255, START_MOVE_FADE_IN_DURATION);
    buttonPause.setFadeAnimationDelay(START_MOVE_ANIMATION_DURATION);
    buttonPause.startFadeAnimation(255, START_MOVE_FADE_IN_DURATION);

    //fly in elements
    icon1.setBitmap(Bitmap(BITMAP_SOIL_LEVEL_MEDIUM_ID));
    icon1.setXY(-120, 70);
    icon1.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY);
    icon1.startMoveAnimation(0, 70, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

    icon2.setBitmap(Bitmap(BITMAP_HEAT_60_ID));
    icon2.setXY(-120, 106);
    icon2.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL);
    icon2.startMoveAnimation(0, 106, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

    icon3.setBitmap(Bitmap(BITMAP_SPIN_1600_ID));
    icon3.setXY(-120, 142);
    icon3.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 2);
    icon3.startMoveAnimation(0, 142, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

    icon4.setBitmap(Bitmap(BITMAP_ALLERGY_ID));
    icon4.setXY(480, 71);
    icon4.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 0);
    icon4.startMoveAnimation(358, 70, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

    icon5.setBitmap(Bitmap(BITMAP_ECO_WASH_ID));
    icon5.setXY(480, 105);
    icon5.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 1);
    icon5.startMoveAnimation(358, 106, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

    icon6.setBitmap(Bitmap(BITMAP_PREWASH_ID));
    icon6.setXY(480, 140);
    icon6.setMoveAnimationDelay(START_MOVE_FLYIN_DELAY + START_MOVE_FLYIN_INTERVAL * 2);
    icon6.startMoveAnimation(358, 142, START_MOVE_FLYIN_DURATION, EasingEquations::cubicEaseIn);

    centerAnimation.setVisible(true);
    centerAnimation.invalidate();
    centerAnimation.startAnimation(false, false, false);

    centerMenu.setVisible(false);
    centerMenu.invalidate();

    imageWhiteCenter.setVisible(false);
    imageWhiteCenter.invalidate();

    buttonStart.setVisible(false);
    buttonStart.invalidate();
}

void MainView::centerAnimatedImageDone(const AnimatedImage&)
{
    touchgfx_printf("centerAnimatedImageDone\n");
    if (animationState == STARTING)
    {
        gotoWashingPrewash();
    }

    if (animationState == WASHING_DONE)
    {
        washDoneBackground.setVisible(true);
        washDoneBackground.invalidate();

        washAnimation.stopAnimation();
        washAnimation.startFadeAnimation(0, 30);
    }
}

void MainView::gotoWashingPrewash()
{
    animationState = WASHING_PREWASH;
    animationTimer = 0;

    washAnimation.startFadeAnimation(255, 30);
    washAnimation.startAnimation(false, true, true);

    prewashIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_PREWASH_ACTIVE_ID));
    prewashIcon.invalidate();

    progressBar.setVisible(true);
    progressBar.invalidate();
}

void MainView::gotoWashingWashing()
{
    animationState = WASHING_WASHING;
    prewashIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_PREWASH_DONE_ID));
    prewashIcon.invalidate();
    washingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_WASHING_ACTIVE_ID));
    washingIcon.invalidate();
}

void MainView::gotoWashingRinsing()
{
    animationState = WASHING_RINSING;
    washingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_WASHING_DONE_ID));
    washingIcon.invalidate();
    rinsingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_RINSING_ACTIVE_ID));
    rinsingIcon.invalidate();
}

void MainView::gotoWashingSpinning()
{
    animationState = WASHING_SPINNING;
    rinsingIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_RINSING_DONE_ID));
    rinsingIcon.invalidate();
    spinCycleIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_SPIN_CYCLE_ACTIVE_ID));
    spinCycleIcon.invalidate();
}

void MainView::gotoWashingDone()
{
    animationState = WASHING_DONE;
    spinCycleIcon.setBitmap(Bitmap(BITMAP_TOP_WASH_SPIN_CYCLE_DONE_ID));
    spinCycleIcon.invalidate();


    progressBar.startFadeAnimation(0, 30);

    animationTimer = 0;
}
