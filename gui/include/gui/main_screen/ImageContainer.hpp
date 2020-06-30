#ifndef IMAGECONTAINER_HPP
#define IMAGECONTAINER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>

class ImageContainer : public Container
{
public:
    ImageContainer()
    {
        add(image);
    }
    void setBitmap(Bitmap b)
    {
        image.setBitmap(b);
        invalidate();
    }

    Image image;
};

#endif
