//
//  text.h
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/18/12.
//
//

#ifndef text_h
#define text_h

#include "vector.h"

class Text
{
public:
    // The text to draw
    char* txt;
    // The position of the text start
    vec2 pos;
    // The color of the text
    vec3 color;
    
    Text(char* txt, vec2 pos, vec3 color) : txt(txt), pos(pos), color(color) {}
    
    // Draws the text
    void drawText();
    // The calculated width of the text
    float width();
    // The calculated width of the string
    static float width(char *t);
    // The calculated height of the text
    float height();
    // The calculated height of the string
    static float height(char *t);
};

#endif
