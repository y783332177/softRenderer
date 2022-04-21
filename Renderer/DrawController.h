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
    Vector3f startPoint; // 第三维为深度值
    Vector3f endPoint;
    Color lineColor;

    static CLine Instance;
    CLine();
    CLine(const Vector3i&, const Vector3i&, Color color);
    CLine(const Vector3f&, const Vector3f&, Color color);
public:
    
    ~CLine();
    static CLine GetInstance();
    // calculate mistake f(x,y) = y-kx-b
    //float getDistance(float x, float y);
    void LineTo(Image&);
    void LineTo(Device &d);
    void LineToOptimization(Image&);
    void LineToOptimization(Device &d);
    void SetStartPoint(const Vector3f &);
    Vector3f GetStartPoint() const;
    void SetEndPoint(const Vector3f&);
    Vector3f GetEndPoint() const;
    void SetLineK();
    void SetLineB();
    void SetColor(Color);
    void Init(const Vector3f&, const Vector3f&, Color color);
    void Init(const Vector3i&, const Vector3i&, Color color);
};

class CTriangle
{
public:
    CTriangle();
    ~CTriangle();
    static void DrawTriangle(Device& d, Vector3f& p1, Vector3f& p2, Vector3f& p3, Color color);
};


#endif