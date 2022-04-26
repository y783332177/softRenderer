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
    Vector2i startPoint; // 第三维为深度值
    Vector2i endPoint;
    float startPointZ;
    float endPointZ;
    Color lineColor;

    static CLine Instance;
    CLine();
    CLine(const Vector2i&, const Vector2i&, const float, const float, const Color &color);
    CLine(const Vector2f&, const Vector2f&, const float, const float, const Color &color);
public:
    
    ~CLine();
    static CLine GetInstance();
    // calculate mistake f(x,y) = y-kx-b
    //float getDistance(float x, float y);
    void LineTo(Image&);
    void LineTo(Device &d);
    void LineToOptimization(Image&);
    void LineToOptimization(Device &d);
    void LineToOptimization(Device &d, Vector2f , Vector2f);
    void SetStartPoint(const Vector2i &);
    Vector2i GetStartPoint() const;
    void SetEndPoint(const Vector2i &);
    Vector2i GetEndPoint() const;
    void SetLineK();
    void SetLineB();
    void SetColor(Color);
    void Init(const Vector2f&, const Vector2f&, const float, const float, Color color);
    void Init(const Vector2i&, const Vector2i&, const float, const float, Color color);
};

class CTriangle
{
public:
    CTriangle();
    ~CTriangle();
    static void DrawTriangle(Device& d, Vector3f _p0, Vector3f _p1, Vector3f _p2, Color color);
    static void DrawTriangleBox(Device& d, Vector3f _p0, Vector3f _p1, Vector3f _p2, Color color);
};


#endif