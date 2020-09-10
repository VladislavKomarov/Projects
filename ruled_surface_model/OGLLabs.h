// ======================================================================
//  OGLLabs.h
// ======================================================================

#ifndef _OGLLabs_h_
#define _OGLLabs_h_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include "data.h"
#include "point3D.h"
#include "camera.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

extern GLint winWidth;	// Исходный размер окна на экране.
extern GLint winHeight;

// ======================================================================
class OGLLabs : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
protected:
    virtual void initializeGL(                       );
    virtual void resizeGL    (int nWidth, int nHeight);
    virtual void paintGL     (                       );

    void setupVertexAttribs(QOpenGLBuffer *buf, GLint countAttribData);

    void c1(float t, float &x, float &y, float &z);
    void c2(float t, float &x, float &y, float &z);

    Data m_data_c1;
    Data m_data_c2;
    Data m_data;
    QOpenGLVertexArrayObject m_vao_c1;
    QOpenGLVertexArrayObject m_vao_c2;
    QOpenGLBuffer *m_data_c1Vbo;
    QOpenGLBuffer *m_data_c2Vbo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer *m_dataVbo;
    QOpenGLShaderProgram *m_program;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    QRect m_viewport;

    void addPoint(GLint xMouse, GLint yMouse);

    void GenerateBoundaryCurves();
    void calculateRuledSurface();
    void plotMode(GLint selectedOption);
    void update_c1Vbo();
    void update_c2Vbo();

    point3D** CreateMatrix(GLuint size1, GLuint size2); // создание матрицы в памяти
    void DeleteMatrix(point3D **p, GLuint size1); // удаление матрицы из памяти

	bool isMovePoint;
	
    bool drawRuledSurface;

    //Размеры области
    GLdouble Xmin;
    GLdouble Xmax;
    GLdouble Ymin;
    GLdouble Ymax;
    GLdouble Z;

    GLdouble Xmid;
    GLdouble Ymid;

    GLdouble XMAX;
    GLdouble DX;

    GLuint N_ctr; //Размерность матрицы контрольных точек
    GLuint N;  //Размерность матрицы точек сплайна
    GLuint M;

    point3D **Pt;

    GLuint i, j;

    unsigned int maxCountBoundaryPoints;
    unsigned int maxCountRuledSurfacePoints;

    void cleanup();

    //настройки камеры
    camera cam;
    void resetCamera();

    int xRot;
    int yRot;

    QPoint lastPos;

    void setXRotation(int angle);
    void setYRotation(int angle);

    float wheelDelta;

    void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

#ifndef QT_NO_WHEELEVENT
	void wheelEvent(QWheelEvent *event);
#endif

public slots:
void slotRuledSurfaceChecked();
public:
    OGLLabs(QWidget* pwgt = 0);
    ~OGLLabs();
signals:
	void slotCloseEvent();
protected:
    virtual void closeEvent(QCloseEvent * event);
};
#endif  //_OGLLabs_h_
