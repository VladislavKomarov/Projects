// ======================================================================
//  OGLQuad.cpp
// ======================================================================

#include "OGLLabs.h"

// ----------------------------------------------------------------------
OGLLabs::OGLLabs(QWidget* pwgt/*= 0*/) : QOpenGLWidget(pwgt),
  m_program(0)
{
    N_ctr = 100;

    N = 50;
    M = 50;

    //Размеры области
    Xmin = 0;
    Xmax = 3 * M_PI;
    Ymin = 0;
    Ymax = 3 * M_PI;
    Z = 1.5;

    Xmid = Xmin + (Xmax - Xmin) / 2.0;
    Ymid = Ymin + (Ymax - Ymin) / 2.0;

    XMAX = sqrt(Xmid*Xmid + Ymid*Ymid);
    DX = XMAX - Xmid + 1;

    maxCountBoundaryPoints = N_ctr;
    maxCountRuledSurfacePoints = N*M;

    Pt = CreateMatrix(N,M);

    resetCamera();
}

OGLLabs::~OGLLabs()
{
    cleanup();
}

void OGLLabs::cleanup()
{
    m_data_c1Vbo->destroy();
    m_data_c2Vbo->destroy();
    m_dataVbo->destroy();
    delete m_data_c1Vbo;
    delete m_data_c2Vbo;
    delete m_dataVbo;
    delete m_program;
    DeleteMatrix(Pt, N);
    m_program = 0;
}

void OGLLabs::resetCamera()
{
    xRot = 0;
    yRot = 0;

    wheelDelta = 0.0;
}

void OGLLabs::setXRotation(int angle)
{
    cam.normalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        update();
    }
}

void OGLLabs::setYRotation(int angle)
{
    cam.normalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        update();
    }
}

// ----------------------------------------------------------------------
/*virtual*/void OGLLabs::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OGLLabs::cleanup);

    initializeOpenGLFunctions();

    glClearColor(1.0, 1.0, 1.0, 1.0); // Цвет окна выбран белым.
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glClearDepthf(1.0f);    // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing (Less Or Equal)
    glEnable(GL_DEPTH_TEST);

    m_program = new QOpenGLShaderProgram;

    // Compile vertex shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("select", 1);
    m_program->link();

    m_program->bind();

    m_data_c1.allocate(maxCountBoundaryPoints, 4);
    m_data_c2.allocate(maxCountBoundaryPoints, 4);
    m_data.allocate(maxCountRuledSurfacePoints, 4);


    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao_c1.create();
    QOpenGLVertexArrayObject::Binder vaoBinder_c1(&m_vao_c1);

    // Setup our vertex buffer object.
    m_data_c1Vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_data_c1Vbo->create();
    m_data_c1Vbo->bind();
    m_data_c1Vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_data_c1Vbo->allocate(maxCountBoundaryPoints * m_data_c1.attribDataCount() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs(m_data_c1Vbo, m_data_c1.attribDataCount());

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao_c2.create();
    QOpenGLVertexArrayObject::Binder vaoBinder_c2(&m_vao_c2);

    // Setup our vertex buffer object.
    m_data_c2Vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_data_c2Vbo->create();
    m_data_c2Vbo->bind();
    m_data_c2Vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_data_c2Vbo->allocate(maxCountBoundaryPoints * m_data_c2.attribDataCount() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs(m_data_c2Vbo, m_data_c2.attribDataCount());

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_dataVbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_dataVbo->create();
    m_dataVbo->bind();
    m_dataVbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_dataVbo->allocate(maxCountRuledSurfacePoints *  m_data.attribDataCount() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs(m_dataVbo, m_data.attribDataCount());

    QString vendor(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
    qDebug() << "Vendor: " << vendor << endl;

    QString renderer(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    qDebug() << "Renderer: " << renderer << endl;

    QString versionOfGL(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    qDebug() << "Version: " << versionOfGL << endl;

    QString versionGLSL(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));
    qDebug() << "GLSL: " << versionGLSL << endl;

    m_program->release();

    drawRuledSurface = false;

    isMovePoint = false;

    m_camera.setToIdentity();
    m_world.setToIdentity();

	setMouseTracking(true);

    GenerateBoundaryCurves();

    cam.r_0 = sqrt(pow((Xmax - Xmin) / 2.0, 2) +
                   pow((Ymax - Ymin) / 2.0, 2) +
                   pow(Z, 2));
}

// ----------------------------------------------------------------------
/*virtual*/void OGLLabs::resizeGL(int nWidth, int nHeight)
{
	// Обновление параметров размера окна.
	winWidth = nWidth;
	winHeight = nHeight;

	// Обновление точки обзора и проекционных параметров.
    m_viewport.setRect(0, 0, winWidth, winHeight);
	glViewport(0, 0, winWidth, winHeight);
}

void OGLLabs::setupVertexAttribs(QOpenGLBuffer *buf, GLint countAttribData)
{
    buf->bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, countAttribData * sizeof(GLfloat), 0);
    if (countAttribData > 3)
    {
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, countAttribData * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    }
    buf->release();
}

// ----------------------------------------------------------------------
/*virtual*/void OGLLabs::paintGL()
{
    /* Очищает окно изображения */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    cam.r = wheelDelta;
    cam.phi = DEG_TO_RAD(yRot / 16.0);
    cam.theta = DEG_TO_RAD(xRot / 16.0);
    cam.convertFromCartesianToSpherical();
    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(cam.x0, cam.y0, cam.z0), QVector3D(cam.x_ref, cam.y_ref, cam.z_ref), QVector3D(cam.Vx, cam.Vy, cam.Vz));

    m_proj.setToIdentity();
    m_proj.ortho(cam.xw_min, cam.xw_max, cam.yw_min, cam.yw_max, cam.d_near, cam.d_far);

    m_program->setUniformValue("mvMatrix", m_camera * m_world);
    m_program->setUniformValue("projMatrix", m_proj);
    m_program->setUniformValue("color", QVector4D(0.0f, 0.0f, 0.0f, 1.0f)); // Цвет рисования выбран черным
    m_program->setUniformValue("colorSelect", QVector4D(0.5f, 0.5f, 0.0f, 1.0f));
    m_program->setUniformValue("pointSizeSelect", (GLfloat) 7.0);//размер выделенной точки
    m_program->setUniformValue("pointSize", (GLfloat) 3.0);//размер обычной точки

    glLineWidth(3);
    QOpenGLVertexArrayObject::Binder vaoBinder1(&m_vao_c1);
    glDrawArrays(GL_LINE_STRIP, 0, m_data_c1.vertexCount());
    QOpenGLVertexArrayObject::Binder vaoBinder2(&m_vao_c2);
    glDrawArrays(GL_LINE_STRIP, 0, m_data_c2.vertexCount());

    if (drawRuledSurface)
    {
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

        m_program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f)); // Цвет рисования выбран красным
        m_program->setUniformValue("pointSize", (GLfloat) 5.0);

        glDrawArrays(GL_POINTS, 0, m_data.vertexCount());
    }

    m_program->release();
}

void OGLLabs::c1(float t, float &x, float &y, float &z)
{
    x = Xmin + t * (Xmax - Xmin) - Xmid;
    y = Ymin - Ymid;
    z = Z * sin(x) * sin(y);
}

void OGLLabs::c2(float t, float &x, float &y, float &z)
{
    x = Xmin + t * (Xmax - Xmin) - Xmid;
    y = Ymax - Ymid;
    z = Z * sin(x) * sin(y);
}

void OGLLabs::GenerateBoundaryCurves()
{
    float t,x,y,z;


    for (i = 0; i < N_ctr; i++)
    {
        t = (float) i / (N_ctr - 1);

        c1(t, x, y, z);
        m_data_c1.add_coord(QVector3D(x, y, z));

        c2(t, x, y, z);
        m_data_c2.add_coord(QVector3D(x, y, z));
    }
    update_c1Vbo();
    update_c2Vbo();
}

void OGLLabs::update_c1Vbo()
{
    m_data_c1Vbo->bind();
    m_data_c1Vbo->write(0, m_data_c1.constData(),  m_data_c1.count() * sizeof(GLfloat));
    m_data_c1Vbo->release();
}

void OGLLabs::update_c2Vbo()
{
    m_data_c2Vbo->bind();
    m_data_c2Vbo->write(0, m_data_c2.constData(),  m_data_c2.count() * sizeof(GLfloat));
    m_data_c2Vbo->release();
}

void OGLLabs::DeleteMatrix(point3D **p, GLuint size1)
{
    for (i=0; i<size1; i++)
        delete [] p[i];

    delete [] p;

    p = NULL;
}

point3D **OGLLabs::CreateMatrix(GLuint size1, GLuint size2)
{
    point3D **p = new point3D* [size1];

    for (i=0; i<size1; i++)
        p[i] = new point3D[size2];

    return p;
}

void OGLLabs::calculateRuledSurface()
{
    float t;
    float tau;
    float x1,y1,z1;
    float x2,y2,z2;

    m_data.clear();

//    //КОД РАСЧЕТА ТОЧЕК ЛИНЕЙЧАТОЙ ПОВЕРХНОСТИ
    int i=0;
    do{
        t=(float)i/(N-1);
        int j=0;
        while (j<M)
        {
            tau=(float)j/(M-1);
            c1(t, x1, y1, z1);
            c2(t, x2, y2, z2);
            Pt[i][j].x =(1-tau)*x1+tau*x2 ;
            Pt[i][j].y = (1-tau)*y1+tau*y2;
            Pt[i][j].z =(1-tau)*z1+tau*z2 ;
            m_data.add_coord(QVector3D(Pt[i][j].x, Pt[i][j].y, Pt[i][j].z));
            j++;
        }
        i++;
    }
    while (i<N);

    m_dataVbo->bind();
    m_dataVbo->write(0, m_data.constData(),  m_data.count() * sizeof(GLfloat));
    m_dataVbo->release();
}

void OGLLabs::plotMode(GLint selectedOption)
{
	switch (selectedOption)
	{
	case 1:
        drawRuledSurface = !drawRuledSurface;
        if (drawRuledSurface)
		{
            calculateRuledSurface();
		}
		break;
	}

    update();
}

void OGLLabs::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
	{
		isMovePoint = false;

		if (!isMovePoint)
		{
            lastPos = event->pos();
		}

		break;
	}
	case Qt::RightButton:
        resetCamera();
		break;
    default:
        break;
	}

    update();
}

void OGLLabs::mouseMoveEvent(QMouseEvent *event)
{
    QVector3D worldCoord;
    QVector3D winCoord;

	switch (event->buttons())
	{
	case Qt::LeftButton:
	{
		if (isMovePoint)
		{
		}
        else
        {
            int dx = event->x() - lastPos.x();
            int dy = event->y() - lastPos.y();

            setXRotation(xRot + 8 * dy);
            setYRotation(yRot + 8 * dx);

            lastPos = event->pos();
        }

		break;
	}
	}

    update();
}

#ifndef QT_NO_WHEELEVENT
void OGLLabs::wheelEvent(QWheelEvent *event)
{
    double delta;
    delta = cam.r_0 * (-1.) * event->delta() / 1000.0f;
    if ((wheelDelta + delta >= -cam.r_0) && (wheelDelta + delta <= cam.r_0*3.0f))
    {
        wheelDelta += delta;
    }

    update();
}
#endif

void OGLLabs::slotRuledSurfaceChecked()
{
    plotMode(1);
}

void OGLLabs::closeEvent(QCloseEvent *event)
{
	QWidget::closeEvent(event);
	emit slotCloseEvent();
}


