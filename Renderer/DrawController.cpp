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
    startPoint = { 0, 0 };
    endPoint = { 0, 0 };
    startPointZ = 0;
    endPointZ = 0;
    this->lineColor = Color(0.0f, 0.0f, 0.0f);
    SetLineK();
    SetLineB();
}
CLine::CLine(const Vector2i &p0, const Vector2i &p1, const float _startPointZ, const float _endPointZ, const Color &color)
    : startPoint(p0), endPoint(p1),startPointZ(_startPointZ), endPointZ(_endPointZ), lineColor(color) { SetLineK(); SetLineB(); }
CLine::CLine(const Vector2f &p0, const Vector2f &p1, const float _startPointZ, const float _endPointZ, const Color &color)
{
    startPoint[0] = (int)p0[0], startPoint[1] = (int)p0[1];
    endPoint[0] = (int)p1[0], endPoint[1] = (int)p1[1];
    startPointZ = _startPointZ;
    endPointZ = _endPointZ;
    this->lineColor = color;
    //SetLineK();
    //SetLineB();
}
CLine::~CLine() { }

CLine CLine::GetInstance()
{
    static CLine Instance;
    return Instance;
}

void CLine::Init(const Vector2i &p0, const Vector2i &p1, const float _startPointZ, const float _endPointZ, const Color color)
{
    startPoint = p0;
    endPoint = p1;
    startPointZ = _startPointZ;
    endPointZ = _endPointZ;
    lineColor = color;
    //setLineK();
    //setLineB();
}
void CLine::Init(const Vector2f &p0, const Vector2f &p1, const float _startPointZ, const float _endPointZ, const Color color)
{
    startPoint[0] = (int)p0[0], startPoint[1] = (int)p0[1];
    endPoint[0] = (int)p1[0], endPoint[1] = (int)p1[1];
    startPointZ = _startPointZ;
    endPointZ = _endPointZ;
    this->lineColor = color;
}

void CLine::SetStartPoint(const Vector2i &point)
{
    this->startPoint = point;
    //setLineK();
    //setLineB();
}
Vector2i CLine::GetStartPoint() const
{
    return this->startPoint;
}

void CLine::SetEndPoint(const Vector2i &point)
{
    this->endPoint = point;
    //setLineK();
    //setLineB();
}

Vector2i CLine::GetEndPoint() const
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
        int y = startPoint[1] * (1.0 - t) + endPoint[1] * t;
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
        float t = (x - startPoint[0]) / (endPoint[0] - startPoint[0]);
        int y = startPoint[1] * (1.0 - t) + endPoint[1] * t;
        float z = startPointZ * (1.0 - t) + endPointZ * t;
        if (steep)
        {
            d.DrawPoint({ (float)y, (float)x ,float(z)});
        }
        else
        {
            d.DrawPoint({ (float)x, (float)y, float(z) });
        }
    }
}


void CLine::LineToOptimization(Image& image)
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
    int dx = endPoint[0] - startPoint[0];
    int dy = std::abs(endPoint[1] - startPoint[1]);
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
    int dx = endPoint[0] - startPoint[0];
    int dy = std::abs(endPoint[1] - startPoint[1]);
    float dz = endPointZ - startPointZ;
    int y = startPoint[1];
    int eps = 0;
    for (int x = startPoint[0]; x <= endPoint[0]; x++)
    {
        float z = (float)(x - startPoint[0]) / (float)dx * dz + startPointZ;
        if (steep)
        {
            //d.PutPixel(y, x, lineColor);
            d.DrawPoint({ (float)y, (float)x, z }, lineColor);
        }
        else
        {
            //d.PutPixel(x, y, lineColor);
            d.DrawPoint({ (float)x, (float)y, z }, lineColor);
        }
        eps += dy;
        if ((eps << 1) >= dx)
        {
            y += (endPoint[1] > startPoint[1] ? 1 : -1);
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

void CTriangle::DrawTriangle(Device& d, Vector3f _p0, Vector3f _p1, Vector3f _p2, Color color)
{
    Vector2i p0 = Vector2i(RoundF2I(_p0.x), RoundF2I(_p0.y));
    Vector2i p1 = Vector2i(RoundF2I(_p1.x), RoundF2I(_p1.y));
    Vector2i p2 = Vector2i(RoundF2I(_p2.x), RoundF2I(_p2.y));

    if (p0.y == p1.y && p0.y == p2.y) return;

    if (p0.y > p1.y) std::swap(p0, p1);
    if (p0.y > p2.y) std::swap(p0, p2);
    if (p1.y > p2.y) std::swap(p1, p2);
    int totalHeight = p2.y - p0.y;
    for (int i = 0; i < totalHeight; i++)
    {
        bool secondHalf = (i > (p1.y - p0.y)) || (p0.y == p1.y);
        int segmentHeight = secondHalf ? (p2.y - p1.y) : (p1.y - p0.y);
        float alpha = (float)i / totalHeight;
        float beta = (float)(i - (secondHalf ? (p1.y - p0.y) : 0)) / segmentHeight;
        
        Vector2i A = p0 + Vector2i(RoundF2I((p2 - p0).x * alpha), RoundF2I((p2 - p0).y * alpha));
        Vector2i B = secondHalf ? (p1 + Vector2i(RoundF2I((p2 - p1).x * beta), RoundF2I((p2 - p1).y * beta))) : (p0 + Vector2i(RoundF2I((p1 - p0).x * beta), RoundF2I((p1 - p0).y * beta)));

        float z0 = _p0.z + (_p2.z - _p0.z) * alpha;
        float z1 = secondHalf ? (_p1.z + (_p2.z - _p1.z) * beta) : (_p0.z + (_p1.z - _p0.z) * beta);
        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(z0, z1);
        }
        d.DrawLine(A, B, z0, z1, color);
    }
}