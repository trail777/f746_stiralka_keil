#ifndef VIDEO_PRESENTER_HPP
#define VIDEO_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class VideoView;

class VideoPresenter : public Presenter, public ModelListener
{
public:
    VideoPresenter(VideoView& v);

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

    virtual ~VideoPresenter() {};

private:
    VideoPresenter();

    VideoView& view;
};


#endif // VIDEO_PRESENTER_HPP
