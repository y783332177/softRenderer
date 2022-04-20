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

    static CLine Instance;
    CLine();
    CLine(const Vector2i&, const Vector2i&, Color color);
    CLine(const Vector2f&, const Vector2f&, Color color);
public:
    
    ~CLine();
    static CLine GetInstance();
    // calculate mistake f(x,y) = y-kx-b
    //float getDistance(float x, float y);
    void LineTo(Image&);
    void LineTo(Device &d);
    void LineToOptimization(Image&);
    void LineToOptimization(Device &d);
    void SetStartPoint(const Vector2i &);
    Vector2i GetStartPoint() const;
    void SetEndPoint(const Vector2i&);
    Vector2i GetEndPoint() const;
    void SetLineK();
    void SetLineB();
    void SetColor(Color);
    void Init(const Vector2f&, const Vector2f&, Color color);
    void Init(const Vector2i&, const Vector2i&, Color color);
};

class CTriangle
{
public:
    CTriangle();
    ~CTriangle();
    static void DrawTriangle(Device& d, Vector2f& p1, Vector2f& p2, Vector2f& p3, Color color);
};


#endif