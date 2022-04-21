#include "DrawController.h"
#include "VectorNT.h"
#include "Matrix.h"
#include "SoftEngine.h"
#include "Image.h"
#include "common.h"

#include<vector>
#include<queue>
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>

std::uint32_t FLOAT_MAX_BIT = 0xef800000;

const float FLOAT_MAX = *reinterpret_cast<float*>(&FLOAT_MAX_BIT);

CLine::CLine()
{
    startPoint = { 0, 0, 0 };
    endPoint = { 0, 0, 0 };
    this->lineColor = Color(0.0f, 0.0f, 0.0f);
    SetLineK();
    SetLineB();
}
CLine::CLine(const Vector3f& p0, const Vector3f& p1, const Color color) : startPoint(p0), endPoint(p1), lineColor(color) { SetLineK(); SetLineB(); }
CLine::CLine(const Vector3i &p0, const Vector3i &p1, const Color color)
{
    startPoint[0] = (float)p0[0], startPoint[1] = (float)p0[1], startPoint[2] = (float)p0[2];
    endPoint[0] = (float)p1[0], endPoint[1] = (float)p1[1], endPoint[2] = (float)p1[2];
    this->lineColor = color;
    SetLineK();
    SetLineB();
}
CLine::~CLine() { }

CLine CLine::GetInstance()
{
    static CLine Instance;
    return Instance;
}

void CLine::Init(const Vector3f& p0, const Vector3f& p1, const Color color)
{
    startPoint = p0;
    endPoint = p1;
    lineColor = color;
    //setLineK();
    //setLineB();
}
void CLine::Init(const Vector3i& p0, const Vector3i& p1, const Color color)
{
    startPoint[0] = (float)p0[0], startPoint[1] = (float)p0[1], startPoint[2] = (float)p0[2];
    endPoint[0] = (float)p1[0], endPoint[1] = (float)p1[1], endPoint[2] = (float)p1[2];
    this->lineColor = color;
}

void CLine::SetStartPoint(const Vector3f &point)
{
    this->startPoint = point;
    //setLineK();
    //setLineB();
}
Vector3f CLine::GetStartPoint() const
{
    return this->startPoint;
}

void CLine::SetEndPoint(const Vector3f &point)
{
    this->endPoint = point;
    //setLineK();
    //setLineB();
}

Vector3f CLine::GetEndPoint() const
{
    return this->endPoint;
}

void CLine::SetColor(const Color color)
{
    this->lineColor = color;
}

void CLine::SetLineK()
{
    if (endPoint[0] == startPoint[0])
    {
        this->lineK = FLOAT_MAX;
        return;
    }
    this->lineK = (float)(endPoint[1] - startPoint[1]) / (endPoint[0] - startPoint[0]);
}

void CLine::SetLineB()
{
    if (lineK == FLOAT_MAX)
    {
        this->lineB = FLOAT_MAX;
        return;
    }
    this->lineB = startPoint[1] - lineK * startPoint[0];
}

void CLine::LineTo(Image& image)
{
    bool steep = false;
    if (std::abs(startPoint[0] - endPoint[0]) < std::abs(startPoint[1] - endPoint[1]))
    {
        std::swap(startPoint[0], startPoint[1]);
        std::swap(endPoint[0], endPoint[1]);
        steep = true;
    }

    if (startPoint[0] > endPoint[0])
    {
        std::swap(startPoint[0], endPoint[0]);
        std::swap(startPoint[1], endPoint[1]);
    }
    for (int x = startPoint[0]; x <= endPoint[0]; x++)
    {
        float t = (x - startPoint[0]) / (float)(endPoint[0] - startPoint[0]);
        int y = startPoint[0] * (1.0 - t) + endPoint[1] * t;
        if (steep)
        {
            image.SetColor(lineColor, y, x);
        }
        else
        {
            image.SetColor(lineColor, x, y);
        }
    }
}

void CLine::LineTo(Device &d)
{
    bool steep = false;

    if (std::fabs(startPoint[0] - endPoint[0]) < std::fabs(startPoint[1] - endPoint[1]))
    {
        std::swap(startPoint[0], startPoint[1]);
        std::swap(endPoint[0], endPoint[1]);
        steep = true;
    }

    if (startPoint[0] > endPoint[0])
    {
        std::swap(startPoint[0], endPoint[0]);
        std::swap(startPoint[1], endPoint[1]);
    }

    for (float x = startPoint[0]; x <= endPoint[0]; x++)
    {
        float t = (x - startPoint[0]) / (endPoint[0] - startPoint[0]);
        float y = startPoint[1] * (1.0 - t) + endPoint[1] * t;
        float z = startPoint[2] * (1.0 - t) + endPoint[2] * t;
        if (steep)
        {
            d.DrawPoint({ (float)y, (float)x ,float(z)});
        }
        else
        {
            d.DrawPoint({ (float)x, (float)y, float() });
        }
    }
}


void CLine::LineToOptimization(Image& image)
{
    bool steep = false;
    if (std::fabs(startPoint[0] - endPoint[0]) < std::fabs(startPoint[1] - endPoint[1]))
    {
        std::swap(startPoint[0], startPoint[1]);
        std::swap(endPoint[0], endPoint[1]);
        steep = true;
    }

    if (startPoint[0] > endPoint[0])
    {
        std::swap(startPoint[0], endPoint[0]);
        std::swap(startPoint[1], endPoint[1]);
    }
    int dx = endPoint[0] - startPoint[0];
    int dy = std::fabs(endPoint[1] - startPoint[1]);
    int y = startPoint[1];
    int eps = 0;
    for (int x = startPoint[0]; x <= endPoint[0]; x++)
    {
        if (steep)
        {
            image.SetColor(lineColor, y, x);
        }
        else
        {
            image.SetColor(lineColor, x, y);
        }
        eps += dy;
        if ((eps << 1) >= dx)
        {
            y += (endPoint[1] > startPoint[1] ? 1 : -1);
            eps -= dx;
        }
    }
}

void CLine::LineToOptimization(Device& d)
{
    bool steep = false;
    if (std::fabs(startPoint[0] - endPoint[0]) < std::fabs(startPoint[1] - endPoint[1]))
    {
        std::swap(startPoint[0], startPoint[1]);
        std::swap(endPoint[0], endPoint[1]);
        steep = true;
    }

    if (startPoint[0] > endPoint[0])
    {
        std::swap(startPoint[0], endPoint[0]);
        std::swap(startPoint[1], endPoint[1]);
    }
    float dx = endPoint[0] - startPoint[0];
    float dy = std::fabs(endPoint[1] - startPoint[1]);
    float dz = endPoint[2] - startPoint[2];
    float y = startPoint[1];
    float eps = 0;
    for (float x = startPoint[0]; x <= endPoint[0]; x++)
    {
        float z = (x - startPoint[0]) / dx * dz + startPoint[2];
        if (steep)
        {
            //d.PutPixel(y, x, lineColor);
            d.DrawPoint({ y, x, z }, lineColor);
        }
        else
        {
            //d.PutPixel(x, y, lineColor);
            d.DrawPoint({ x, y, z }, lineColor);
        }
        eps += dy;
        if ((eps * 2) >= dx)
        {
            y += (endPoint[1] > startPoint[1] ? 1.0f : -1.0f);
            eps -= dx;
        }
    }
}

CTriangle::CTriangle()
{
}

CTriangle::~CTriangle()
{
}

void CTriangle::DrawTriangle(Device& d, Vector3f& p1, Vector3f& p2, Vector3f& p3, Color color)
{
    //Vector2i p1 = Vector2i(RoundF2I(_p1.x), RoundF2I(_p1.y));
    //Vector2i p2 = Vector2i(RoundF2I(_p2.x), RoundF2I(_p2.y));
    //Vector2i p3 = Vector2i(RoundF2I(_p3.x), RoundF2I(_p3.y));

    if (p1.y == p2.y && p1.y == p3.y) return;

    if (p1.y > p2.y) std::swap(p1, p2);
    if (p1.y > p3.y) std::swap(p1, p3);
    if (p2.y > p3.y) std::swap(p2, p3);
    float totalHeight = p3.y - p1.y;
    for (int i = 0; i < totalHeight; i++)
    {
        bool secondHalf = (i >= (p2.y - p1.y)) || (p1.y == p2.y);
        float segmentHeight = secondHalf ? (p3.y - p2.y) : (p2.y - p1.y);
        float alpha = (float)i / totalHeight;
        float beta = (float)(i - (secondHalf ? (p2.y - p1.y) : 0)) / segmentHeight;
        
        Vector3f A = p1 + (p3 - p1) * alpha;
        Vector3f B = secondHalf ? (p2 + (p3 - p2) * beta) : (p1 + (p2 - p1) * beta);

        if (A.x > B.x) std::swap(A, B);
        d.DrawLine(A, B, color);
    }
}