//
//  button.h
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/18/12.
//
//

#ifndef button_h
#define button_h

#include "vector.h"

class Button
{
public:
    // The text on the button
    char* text;
    // The location of the button
    vec2 loc;
    // if isLeft is true, loc.x will refer to the left side of the button; if it is false, loc.x will refer to the right side of the button
    bool isLeft;
    // if isBottom is true, loc.y will refer to the bottom side of the button; if it is false, loc.y will refer to the top side of the button
    bool isBottom;
    // The color of the text
    vec3 tcolor;
    // The padding around the text (controls button size).  Order is [left, top, right, bottom]
    vec4 padding;
    // The color of the top of the button
    vec3 colorT;
    // The color of the bottom of the button
    vec3 colorB;
    // if isFlipped is true, the button is in a pressed state and the top and bottom colors are switched; if it is false, the button is not pressed and renders normally
    bool isFlipped;
    // if isRelative is true, the position of the corner for loc will be relative to the corresponding corner of the window; if it is false, the position of the corner for loc will be relative to the origin.
    bool isRelative;
    
    Button(char* text, vec2 loc, bool isLeft, bool isBottom, vec3 tcolor, vec4 paddingLTRB, vec3 colorT, vec3 colorB, bool isFlipped, bool isRelative) : text(text), loc(loc), isLeft(isLeft), isBottom(isBottom), tcolor(tcolor), padding(paddingLTRB), colorT(colorT), colorB(colorB), isFlipped(isFlipped), isRelative(isRelative) {}
    
    // Flip the button
    void flip();
    // The calculated height of the button
    float height();
    // The calculated width of the button
    float width();
    // The calculated top of the button
    float top();
    // The calculated bottom of the button
    float bottom();
    // The calculated left of the button
    float left();
    // The calculated right of the button
    float right();
    // The bounds of the button.  Order is [left, top, right, bottom]
    vec4 bounds();
    // Returns true if (x, y) is inside the button, false otherwise.
    bool contains(float x, float y);
    // Returns true if (vec2.x, vec2.y) is inside the button, false otherwise
    bool contains(vec2 pos);
    // Draws the button
    void drawButton();
private:
    // Gets the location taking isRelative into account
    vec2 getLoc();
};

#endif
