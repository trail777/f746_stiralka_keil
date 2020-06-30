#ifndef VIDEO_VIEW_HPP
#define VIDEO_VIEW_HPP

#include <gui_generated/video_screen/VideoViewBase.hpp>
#include <gui/video_screen/VideoPresenter.hpp>
#include <widgets/MJPEGWidget.hpp>

class VideoView : public VideoViewBase
{
public:
    VideoView();
    virtual ~VideoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);

protected:
    Callback<VideoView, const FadeAnimator<Image>& > fadeCallback;
    void buttonFadeDone(const FadeAnimator<Image>& img);
    bool getVideoTexts(int frameno, TypedText& headline, TypedText& body);

    uint32_t tickCounter;
    bool playing;
    MJPEGWidget mjpgWidget;
};

#endif // VIDEO_VIEW_HPP
