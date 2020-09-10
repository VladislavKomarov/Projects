#include "point3D.h"


point3D::point3D()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;

    t = 0.0;
    tau = 0.0;

    winx = 0.0;
    winz = 0.0;
    winy = 0.0;
    select = false;
}


point3D::~point3D()
{
}

void point3D::SetRect(int x1, int y1, int x2, int y2)
{
    rect.setLeft(x1);
    rect.setRight(x2);
    rect.setTop(y2);
    rect.setBottom(y1);
}


void point3D::CalculateWindowCoordinates(const QMatrix4x4 &modelView,
                                        const QMatrix4x4 &projection,
                                        const QRect &viewport)
{
    QVector3D worldCoord;
    QVector3D winCoord;

    worldCoord.setX(x);
    worldCoord.setY(y);
    worldCoord.setZ(z);


    //------------ѕолучаем видовые координаты точки-----------
    winCoord = worldCoord.project(modelView, projection, viewport);
    winCoord.setY(viewport.height() - (int)winCoord.y() - 1);

    winx = winCoord.x();
    winy = winCoord.y();
    winz = winCoord.z();

    SetRect((int)winCoord.x() - 5, (int)winCoord.y() - 5, (int)winCoord.x() + 5, (int)winCoord.y() + 5);//создаем пр€моугольник вокруг точки
}


bool point3D::PtInRect(int x, int y)
{
    return rect.contains(x,y);
}
