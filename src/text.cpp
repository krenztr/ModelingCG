//
//  text.cpp
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/18/12.
//
//

#include "text.h"
#include <SFML/Window.hpp>
#include "glyphs.h"

void Text::drawText()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glColor3fv(color);
    glRasterPos2fv(pos);
    for(int i = 0; i < strlen(txt); i++)
    {
        glBitmap(GLYPH_WIDTH, GLYPH_HEIGHT, 0, 0, GLYPH_WIDTH, 0, (GLubyte*)glyphs[txt[i]]);
    }
}

float Text::width()
{
    return strlen(txt)*GLYPH_WIDTH;
}

float Text::width(char *t)
{
    return strlen(t)*GLYPH_WIDTH;
}

float Text::height()
{
    return GLYPH_HEIGHT;
}

float Text::height(char *t)
{
    return GLYPH_HEIGHT;
}