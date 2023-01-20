#include "shape.h"
// в конструкторе просто присваиваем переданный QList и задаем значение полю m_count
Shape::Shape(QList<GLfloat> data)
{
    m_data = data;
    m_count = m_data.count();
}
