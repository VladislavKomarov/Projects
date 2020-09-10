#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#define RAD_TO_DEG(rad) ((rad)*180.0/M_PI)
#define DEG_TO_RAD(deg) ((deg)*M_PI/180.0)

class camera
{
public:
    camera();

    //декартовы координаты камеры
    float x0;
    float y0;
    float z0;

    //сферические координаты камеры
    float r;
    float theta;
    float phi;

    //начальные сферические координаты камеры
    float r_0;
    float theta_0;
    float phi_0;

    //декартовы координаты точки наблюдения
    float x_ref;
    float y_ref;
    float z_ref;

    //вектор верха камеры
    float Vx;
    float Vy;
    float Vz;

    //габаритный прямоугольник координат наблюдения
    float xw_min;
    float xw_max;
    float yw_min;
    float yw_max;
    float d_near;
    float d_far;

    void convertFromCartesianToSpherical(void);
    void convertFromCartesianToSphericalCoordinates(float r,
                                                    float theta,
                                                    float phi,
                                                    float &x,
                                                    float &y,
                                                    float &z);
    void normalizeAngle(int &angle);
};

#endif // CAMERA_H
