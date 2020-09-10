#pragma once

#include <QRect>
#include <QVector3D>

class point3D
{
public:
    point3D();
    virtual ~point3D();

    //Point Selection
    bool select;//���� ���������? ��/���

    float winx;
    float winy;
    float winz;
    QRect rect;

    float x;		//x-���������� �����
    float y;		//y-���������� �����
    float z;		//z-���������� �����

    //��������������� ���������� t � tau
    float t, tau;

    void SetRect(int x1, int y1, int x2, int y2);
    void CalculateWindowCoordinates(const QMatrix4x4 &modelView,
                                    const QMatrix4x4 &projection,
                                    const QRect &viewport);
    bool PtInRect(int x, int y);
};
