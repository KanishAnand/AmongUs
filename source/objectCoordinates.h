#ifndef OBJECT_COORDINATES_H
#define OBJECT_COORDINATES_H

#include "template.h"

class ObjectCoordinates {
   public:
    pair<float, float> bottomLeft, topRight;

    ObjectCoordinates() {}
    ObjectCoordinates(pair<float, float> bottomLeft, pair<float, float> topRight) : bottomLeft(bottomLeft), topRight(topRight) {}
};
#endif