#include <gui/video_screen/VideoView.hpp>
#include <Bitmapdatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

extern "C" {
#if 1
    extern const unsigned char wash_300x180px_avi[];
    static unsigned int wash_300x180px_avi_len = 5817226;
#endif
}

#if !defined(USE_BPP) || USE_BPP==16
#ifdef SIMULATOR
uint8_t pixeldata[354 * 212 * 2];
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
#else
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
_Pragma("data_alignment = 64")
uint8_t pixeldata[354 * 212 * 2];/*@"NonCacheable"*/;
#endif

#elif USE_BPP==24
#ifdef SIMULATOR
uint8_t pixeldata[354 * 212 * 3];
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
#else
uint8_t lineBuffer[480 * 3]; //length must at least match width of video, RGB888
_Pragma("data_alignment = 64")
uint8_t pixeldata[354 * 212 * 3];/*@"NonCacheable"*/;
#endif
#endif

VideoView::VideoView()
    : fadeCallback(this, &VideoView::buttonFadeDone),
      tickCounter(0), playing(true)
{
}

void VideoView::setupScreen()
{
    HAL::getInstance()->setFrameRateCompensation(false);

#if 1
    //Configure MJPEG widget
    mjpgWidget.setPosition(147, 54, 300, 180);
    mjpgWidget.setLineBuffer(lineBuffer);

#if !defined(USE_BPP) || USE_BPP==16
    mjpgWidget.setBitmapFormat(Bitmap::RGB565);
#elif USE_BPP==24
    mjpgWidget.setBitmapFormat(Bitmap::RGB888);
#endif
    mjpgWidget.setMovie(wash_300x180px_avi, wash_300x180px_avi_len);
    mjpgWidget.setVisible(true);
    mjpgWidget.setPixelData(pixeldata);
    mjpgWidget.gotoFrame(22);
    add(mjpgWidget);
#endif

    //add callback
    imageIndication.setFadeAnimationEndedAction(fadeCallback);
    imageIndication.setAlpha(0);
    //move indicator to front
    remove(imageIndication);
    add(imageIndication);
}

void VideoView::tearDownScreen()
{
    HAL::getInstance()->setFrameRateCompensation(true);
}

void VideoView::handleClickEvent(const ClickEvent& evt)
{
    View::handleClickEvent(evt);

    if (evt.getType() == ClickEvent::RELEASED &&
            evt.getX() > 146 && evt.getX() < 446 && evt.getY() > 53 && evt.getY() < 233)
    {
        if (playing)
        {
            playing = false;
            imageIndication.setBitmap(Bitmap(BITMAP_VIDEO_PAUSE_BUTTON_ID));
            imageIndication.setAlpha(255);
            imageIndication.invalidate();
            imageIndication.startFadeAnimation(0, 25);
        }
        else
        {
            imageIndication.setBitmap(Bitmap(BITMAP_VIDEO_PLAY_BUTTON_ID));
            imageIndication.setAlpha(255);
            imageIndication.invalidate();
            imageIndication.startFadeAnimation(0, 25);
            playing = true;
        }
    }
}

void VideoView::handleTickEvent()
{
    ++tickCounter;
    if (tickCounter & 0x01 && playing)
    {
        TypedText headline, body;
        if (getVideoTexts(mjpgWidget.getFrameNo(), headline, body))
        {
            textAreaHeadline.setTypedText(headline);
            textAreaHeadline.invalidate();
            textAreaBodyText.setTypedText(body);
            textAreaBodyText.invalidate();
        }

        mjpgWidget.tick();
    }
}

bool VideoView::getVideoTexts(int frameno, TypedText& headline, TypedText& body)
{
    switch (frameno)
    {
    case 1:
        headline = TypedText(T_VIDEOHEADLINE1);
        body = TypedText(T_VIDEOTEXT1);
        return true;
    case 180:
        headline = TypedText(T_VIDEOHEADLINE2);
        body = TypedText(T_VIDEOTEXT2);
        return true;
    case 320:
        headline = TypedText(T_VIDEOHEADLINE3);
        body = TypedText(T_VIDEOTEXT3);
        return true;
    case 440:
        headline = TypedText(T_VIDEOHEADLINE4);
        body = TypedText(T_VIDEOTEXT4);
        return true;
    default:
        return false;
    }
}

void VideoView::buttonFadeDone(const FadeAnimator<Image>& img)
{
}

namespace touchgfx
{
namespace fileinput
{
int readFile(fileinput::FileHdl hdl, uint8_t* const buffer, uint32_t length)
{
    return 0;
}
}
}
