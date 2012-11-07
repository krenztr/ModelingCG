//
//  button.cpp
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/18/12.
//
//

#include "button.h"
#include <SFML/Window.hpp>
#include "text.h"
#include "draw.h"

void Button::flip()
{
    isFlipped = !isFlipped;
}

float Button::height()
{
    return Text::height(text) + padding.y + padding.w;
}

float Button::width()
{
    return Text::width(text) + padding.x + padding.z;
}

vec2 Button::getLoc()
{
    if(isRelative)
    {
        vec4 vp = getViewport();
        return vec2(isLeft ? loc.x : vp[2] - loc.x, isBottom ? loc.y : vp[3] - loc.y);
    }
    else
        return loc;
}

float Button::top()
{
    float top = getLoc().y;
    if(isBottom)
        top += height();
    return top;
}

float Button::bottom()
{
    float bottom = getLoc().y;
    if(!isBottom)
        bottom -= height();
    return bottom;
}

float Button::left()
{
    float left = getLoc().x;
    if(!isLeft)
        left -= width();
    return left;
}

float Button::right()
{
    float right = getLoc().x;
    if(isLeft)
        right += width();
    return right;
}

vec4 Button::bounds()
{
    return vec4(left(), top(), right(), bottom());
}

bool Button::contains(float x, float y)
{
    return (x >= left() && x <= right() && y >= bottom() && y <= top());
}

bool Button::contains(vec2 pos)
{
    return contains(pos.x, pos.y);
}

void Button::drawButton()
{
    vec4 bounds = this->bounds();
    vec2 textPos = bounds.get(0, 3) + padding.get(0, 3);
    Text(text, textPos, tcolor).drawText();
    glBegin(GL_QUADS);
    {
        if(!isFlipped)
            glColor3fv(colorT);
        else
            glColor3fv(colorB);
        glVertex2fv(bounds.get(0, 1));
        glVertex2fv(bounds.get(2, 1));
        if(!isFlipped)
            glColor3fv(colorB);
        else
            glColor3fv(colorT);
        glVertex2fv(bounds.get(2, 3));
        glVertex2fv(bounds.get(0, 3));
    }
    glEnd();
}