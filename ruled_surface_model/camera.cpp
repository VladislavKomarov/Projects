#include "camera.h"

camera::camera()
{
    r = 0.0;
    phi = 0.0;
    theta = 0.0;

    r_0 = 0.0;
    phi_0 = 0.0;
    theta_0 = 0.0;

    x_ref = 0.0;
    y_ref = 0.0;
    z_ref = 0.0;
}

void camera::convertFromCartesianToSpherical()
{
    double R;
    double Theta;
    double Phi;

    R = r + r_0;
    Theta = theta + theta_0;
    Phi = phi + phi_0;

    convertFromCartesianToSphericalCoordinates(R, Theta, Phi, x0, y0, z0);

    Vx = -R * cos(Theta) * cos(Phi);
    Vy = -R * cos(Theta) * sin(Phi);
    Vz = R * sin(Theta);

    xw_min = -R;
    xw_max = R;
    yw_min = -R;
    yw_max = R;
    d_near = 0.0;
    d_far = 2*R;
}

void camera::convertFromCartesianToSphericalCoordinates(float r,
                                                        float theta,
                                                        float phi,
                                                        float &x,
                                                        float &y,
                                                        float &z)
{
    x = r*sin(theta)*cos(phi);
    y = r*sin(theta)*sin(phi);
    z = r*cos(theta);
}

void camera::normalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}
