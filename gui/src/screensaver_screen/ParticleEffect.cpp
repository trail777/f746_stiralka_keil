#include <gui/screensaver_screen/ParticleEffect.hpp>
#include <stdlib.h>
#include <touchgfx/Utils.hpp>
#include <math.h>
#include <touchgfx/hal/HAL.hpp>

using namespace touchgfx;

void Particle::init(int cx, int cy, int counter, bool fadeEnabled, BitmapId bmp)
{
    float r = ((float)rand() / RAND_MAX) * ((float)rand() / RAND_MAX) * 11;
    setBitmap(touchgfx::Bitmap(bmp - 1 * (int)r));
    setVisible(true);

    dvdt = 0;
    vx = ((float)rand() / RAND_MAX) * 10.0f * cosf(counter / 50.0f);
    vy = ((float)rand() / RAND_MAX) * 10.0f * sinf(counter / 50.0f);
    if (((float)rand() / RAND_MAX) < 0.5f)
    {
        vx = -vx;
        vy = -vy;
    }
    x = (float)cx - getWidth() / 2;
    y = (float)cy - getHeight() / 2;
    alpha = 255;

    if (fadeEnabled)
    {
        dalpha = ((float)rand() / RAND_MAX) * 0.5f + 0.5f;
    }
    else
    {
        dalpha = 0;
    }

    setAlpha((uint8_t)alpha);
    moveTo((int16_t)x, (int16_t)y);
}

void Particle::update()
{
    dvdt *= 0.9f;
    vx += dvdt;
    vy += dvdt;
    x += vx;
    y += vy;
    alpha -= dalpha;

    if (alpha <= 0.0)
    {
        alpha = 0;
    }

    setAlpha((uint8_t)alpha);
    moveTo((int16_t)x, (int16_t)y);
}

bool Particle::done(int maxX, int maxY)
{
    return alpha <= 0.0f || x + getWidth() < 0 || x > maxX || y + getHeight() < 0 || y > maxY;
}

ParticleEffect::ParticleEffect() :
    numberOfParticles(MAX_NUMBER_OF_PARTICLES),
    originX(0),
    originY(0),
    snapToOrigin(false),
    spawnActive(true),
    fadeParticles(true),
    endParticleBitmap(BITMAP_PARTICLE10_ID),
    cx(0),
    cy(0),
    counter(0)
{
    Application::getInstance()->registerTimerWidget(this);
    setTouchable(true);

}

ParticleEffect::~ParticleEffect()
{
    Application::getInstance()->unregisterTimerWidget(this);
}

touchgfx::Rect ParticleEffect::getSolidRect() const
{
    return touchgfx::Rect();
}

void ParticleEffect::draw(const touchgfx::Rect& invalidatedArea) const
{
    //    for (int i = 0; i < numberOfParticles; i++)
    //    {
    //        Rect invalid = invalidatedArea & particles[i].getRect();
    //        if (!invalid.isEmpty())
    //        {
    //            invalid.x -= particles[i].getX();
    //            invalid.y -= particles[i].getY();
    //
    //            HAL::lcd().drawPartialBitmap(Bitmap(particles[i].getBitmap()), particles[i].getX(), particles[i].getY(), invalid, (uint8_t)particles[i].alpha);
    //        }
    //    }

    Rect meAbs;
    translateRectToAbsolute(meAbs); //To find our x and y coords in absolute.

    for (int i = 0; i < numberOfParticles; i++)
    {
        Rect invalid = particles[i].getRect() & invalidatedArea;

        if (!invalid.isEmpty())
        {
            invalid.x -= particles[i].getX();
            invalid.y -= particles[i].getY();
            HAL::lcd().drawPartialBitmap(Bitmap(particles[i].getBitmap()), meAbs.x + particles[i].getX(), meAbs.y + particles[i].getY(), invalid, (uint8_t)particles[i].alpha);
        }
    }
}

void ParticleEffect::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        setCenter(evt.getX(), evt.getY());
    }
    else
    {
        if (snapToOrigin)
        {
            setCenter(originX, originY);
        }
    }
}

void ParticleEffect::handleDragEvent(const DragEvent& evt)
{
    setCenter(evt.getNewX(), evt.getNewY());
}

void ParticleEffect::handleTickEvent()
{
    counter++;

    invalidate();
    static int count = 0;
    if (++count > numberOfParticles)
    {
        count = numberOfParticles;
    }
    int32_t sum = 0;
    static int32_t maxsum = 0;

    for (int i = 0; i < count; i++)
    {
        if (spawnActive && particles[i].done(getWidth(), getHeight()))
        {
            particles[i].init(cx, cy, counter, fadeParticles, endParticleBitmap);
        }
        else
        {
            particles[i].update();
        }
        sum += particles[i].getWidth() * particles[i].getHeight();
        if (sum > maxsum)
        {
            maxsum = sum;
        }
    }
}

void ParticleEffect::setNumberOfParticles(int particles)
{
    numberOfParticles = particles;

    if (numberOfParticles > MAX_NUMBER_OF_PARTICLES)
    {
        numberOfParticles = MAX_NUMBER_OF_PARTICLES;
    }
}

void ParticleEffect::setCenter(int x, int y)
{
    cx = x;
    cy = y;
}

void ParticleEffect::setSnapToOrigin(bool snap)
{
    snapToOrigin = snap;
}

void ParticleEffect::setOrigin(int oX, int oY)
{
    originX = oX;
    originY = oY;
}

void ParticleEffect::spawnNewParticles(bool spawn)
{
    spawnActive = spawn;
}

void ParticleEffect::setParticleFade(bool fade)
{
    fadeParticles = fade;
}

void ParticleEffect::setParticleBitmap(BitmapId bmp)
{
    endParticleBitmap = bmp;
}
