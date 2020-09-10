#ifndef DATA_H
#define DATA_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>
//#include <math.h>

//extern GLint winWidth, winHeight; // Исходный размер окна на экране.

class Data
{
public:
    Data();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / m_countAttribData; }
    int attribDataCount() const { return m_countAttribData; }

    const GLuint *constIndices() const { return m_indices.constData(); }
    int countIndices() const { return m_countIndices; }

    void add_coord(const QVector3D &v);
    void set_coord(const QVector3D &v, unsigned int  i);
    void set_select(bool isSelect, unsigned int  i);
    void allocate(unsigned int maxCountPoints, unsigned int countAttribData);
    void clear();
    void create_indexes(GLuint n, GLuint m);
private:
    QVector<GLfloat> m_data;
    int m_count;
    GLint m_countAttribData;
    QVector<GLuint> m_indices;
    int m_countIndices;
};

#endif // DATA_H
