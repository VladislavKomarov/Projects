#include "data.h"

Data::Data()
    : m_count(0),
      m_countAttribData(0)
{

//    add(QVector4D(0.0, 0.0, 0.0, 1.0), QVector4D(1.0, 0.0, 0.0, 1.0));
//    add(QVector4D(1024, 768, 0.0, 1.0), QVector4D(0.0, 1.0, 0.0, 1.0));
//    add(QVector4D(1024, 0.0, 0.0, 1.0), QVector4D(0.0, 0.0, 1.0, 1.0));
}

void Data::allocate(unsigned int maxCountPoints, unsigned int countAttribData)
{
    m_countAttribData = countAttribData;
    m_data.resize(maxCountPoints * m_countAttribData);
}

void Data::add_coord(const QVector3D &v)
{
    GLfloat *p = m_data.data() + m_count;
	*p++ = v.x();
	*p++ = v.y();
    *p++ = v.z();
    *p++ = false;
    m_count += m_countAttribData;
}

//void Data::add_select(bool isSelect)
//{
//    GLfloat *p = m_data.data() + m_count;
//    *p++ = isSelect;
//    m_count += 1;
//}

void Data::set_coord(const QVector3D &v, unsigned int i)
{
    GLfloat *p = m_data.data() + i*m_countAttribData;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
}

void Data::set_select(bool isSelect, unsigned int i)
{
    GLfloat *p = m_data.data() + i*m_countAttribData + (m_countAttribData - 1);
    *p++ = isSelect;
}

void Data::clear()
{
    m_count = 0;
}

void Data::create_indexes(GLuint n, GLuint m)
{
    GLuint i,j,k;

    k = 0;
    m_countIndices = 2*n*m;
    m_indices.resize(m_countIndices);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
            m_indices[k++] = i*m+j;
    }
    for (j = 0; j < m; j++)
    {
        for (i = 0; i < n; i++)
            m_indices[k++] = i*m+j;
    }
}

