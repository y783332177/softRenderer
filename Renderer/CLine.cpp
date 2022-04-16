#include "CLine.h"
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

std::uint32_t FLOAT_MAX_BIT = 0xef800000;

const float FLOAT_MAX = *reinterpret_cast<float*>(&FLOAT_MAX_BIT);

CLine::CLine()
{
    startPoint = {0, 0};
    endPoint = { 0, 0 };
    this->lineColor = Color(0.0f, 0.0f, 0.0f);
    setLineK();
    setLineB();
}
CLine::CLine(const Vector2i& p0, const Vector2i& p1, const Color color) : startPoint(p0), endPoint(p1), lineColor(color) { setLineK(); setLineB(); }
CLine::CLine(const Vector2f &p0, const Vector2f &p1, const Color color)
{
    startPoint[0] = (int)p0[0], startPoint[1] = (int)p0[1];
    endPoint[0] = (int)p1[0], endPoint[1] = (int)p1[1];
    this->lineColor = color;
    setLineK();
    setLineB();
}
CLine::~CLine() { }

void CLine::setStartPoint(const Vector2i &point)
{
    this->startPoint = point;
    setLineK();
    setLineB();
}
Vector2i CLine::getStartPoint() const
{
    return this->startPoint;
}

void CLine::setEndPoint(const Vector2i &point)
{
    this->endPoint = point;
    setLineK();
    setLineB();
}

Vector2i CLine::getEndPoint() const
{
    return this->endPoint;
}

void CLine::setColor(const Color color)
{
    this->lineColor = color;
}

void CLine::setLineK()
{
    if (endPoint[0] == startPoint[0])
    {
        this->lineK = FLOAT_MAX;
        return;
    }
    this->lineK = (float)(endPoint[1] - startPoint[1]) / (endPoint[0] - startPoint[0]);
}

void CLine::setLineB()
{
    if (lineK == FLOAT_MAX)
    {
        this->lineB = FLOAT_MAX;
        return;
    }
    this->lineB = startPoint[1] - lineK * startPoint[0];
}

void CLine::lineTo(Image& image)
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

void CLine::lineTo(Device &d)
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
            d.DrawPoint({ (float)y, (float)x });
        }
        else
        {
            d.DrawPoint({ (float)x, (float)y });
        }
    }
}


void CLine::lineToOptimization(Image& image)
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

void CLine::lineToOptimization(Device& d)
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
            //d.PutPixel(y, x, lineColor);
            d.DrawPoint({ (float)y, (float)x });
        }
        else
        {
            //d.PutPixel(x, y, lineColor);
            d.DrawPoint({ (float)x, (float)y });
        }
        eps += dy;
        if ((eps << 1) >= dx)
        {
            y += (endPoint[1] > startPoint[1] ? 1 : -1);
            eps -= dx;
        }
    }
}