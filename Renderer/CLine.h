#pragma once
#ifndef _CLINE_H
#define _CLINE_H

#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "Image.h"

#include<vector>
#include<queue>
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>

class CLine
{
private:
    float lineK;
    float lineB;
    Vector2i startPoint;
    Vector2i endPoint;
    Color lineColor;

public:
    CLine();
    CLine(const Vector2i&, const Vector2i&, Color color);
    CLine(const Vector2f&, const Vector2f&, Color color);
    ~CLine();
    // calculate mistake f(x,y) = y-kx-b
    //float getDistance(float x, float y);
    void lineTo(Image&);
    void lineTo(Device &d);
    void lineToOptimization(Image&);
    void lineToOptimization(Device &d);
    void setStartPoint(const Vector2i &);
    Vector2i getStartPoint() const;
    void setEndPoint(const Vector2i&);
    Vector2i getEndPoint() const;
    void setLineK();
    void setLineB();
    void setColor(Color);
};

#endif