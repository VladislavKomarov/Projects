#pragma once

#include <QRect>
#include <QVector3D>

class point3D
{
public:
    point3D();
    virtual ~point3D();

    //Point Selection
    bool select;//есть выделение? Да/Нет

    float winx;
    float winy;
    float winz;
    QRect rect;

    float x;		//x-координата точки
    float y;		//y-координата точки
    float z;		//z-координата точки

    //ПАРАМЕТРИЧЕСКИЕ КООРДИНАТЫ t и tau
    float t, tau;

    void SetRect(int x1, int y1, int x2, int y2);
    void CalculateWindowCoordinates(const QMatrix4x4 &modelView,
                                    const QMatrix4x4 &projection,
                                    const QRect &viewport);
    bool PtInRect(int x, int y);
};
