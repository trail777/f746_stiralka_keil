#include <gui/video_screen/VideoView.hpp>
#include <gui/video_screen/VideoPresenter.hpp>

VideoPresenter::VideoPresenter(VideoView& v)
    : view(v)
{
}

void VideoPresenter::activate()
{
    model->setScreenSaverEnabled(false);
}

void VideoPresenter::deactivate()
{

}
