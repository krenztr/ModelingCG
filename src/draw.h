//
//  2d.h
//  GLBox-XCode
//
//  Created by Eric Henderson on 9/19/12.
//
//

#ifndef draw_h
#define draw_h

#include "vector.h"

// Initializes the matricies for drawing in 2D
void setup2D();

// Returns the matricies to the previous state
void tearDown2D();

// Gets the viewport.  Order is [left, top, right, bottom]
vec4 getViewport();

#endif
