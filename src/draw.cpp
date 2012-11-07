//
//  2d.cpp
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/19/12.
//
//

#include "draw.h"
#include <SFML/Window.hpp>

void setup2D()
{
    vec4 vp = getViewport();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    gluOrtho2D(0, vp[2]-1, 0, vp[3]-1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void tearDown2D()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

vec4 getViewport()
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    return vec4(vp[0], vp[1], vp[2], vp[3]);
}