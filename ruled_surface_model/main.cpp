// ======================================================================
//  main.cpp
// ======================================================================

#include <QApplication>
#include <QtWidgets>
#include "OGLLabs.h"
#include "OShowOptions.h"

GLint winWidth = 1024, winHeight = 768; // Исходный размер окна на экране.

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    OGLLabs      oglLabs;

    oglLabs.move(0,0);
    /* Задает верхнее левое положение окна изображения. */
	oglLabs.resize(winWidth, winHeight);   // Задает ширину и высоту
                                // окна изображения
    oglLabs.setWindowTitle("Ruled_Surface");
    oglLabs.show();

    OShowOptions wgt;

	wgt.move(winWidth + 10, 0);
	wgt.resize(500, 10);
	wgt.setWindowTitle("Опции показа");
	wgt.show();

    QObject::connect(wgt.pchkRuledSurface, SIGNAL(toggled(bool)), &oglLabs, SLOT(slotRuledSurfaceChecked()));
//    QObject::connect(wgt.pchkLineSegments, SIGNAL(toggled(bool)), &oglLabs, SLOT(slotLineSegmentsChecked()));
	QObject::connect(&oglLabs, SIGNAL(slotCloseEvent()), &wgt, SLOT(close()));
    QObject::connect(&wgt, SIGNAL(slotCloseEvent()), &oglLabs, SLOT(close()));

    return app.exec();
}
