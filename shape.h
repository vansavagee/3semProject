#ifndef SHAPE_H
#define SHAPE_H

#include <qopengl.h>
#include <QList>

/*!
    \brief класс хранит список точек графика и возвращает их в виде массива 
    Класс хранит координаты точек графика в виде QList отдельных чисел,
    позволяет получить данные в виде массива, полное количество чисел,
    количество координат точек которое хранится в массиве
*/
class Shape
{
public:
    // конструкторы
    /// конструктор по умолчанию
    Shape(){}
    /*!
     \brief конструктор
     \param data - QList координат точек графика
    */
    Shape(QList<GLfloat> data);
    // возвращает QList в виде простого массива
    /*!
     \brief возвращает указатель на константный массив GLfloat
     \return GLfloat*
    */
    const GLfloat *constData() const { return m_data.constData(); }
    // возвращает количество элементов
    /*!
     \brief возвращает количество элементов в QList
     \return int
    */
    int count() const { return m_count; }
    // для каждой точки нужны 3 координаты, поэтому точек в 3 раза меньше
    /*!
     \brief возвращает количество точек координаты которых хранятся в QList
     \return int
    */
    int vertexCount() const { return m_count / 3; }
private:
    QList<GLfloat> m_data;
    int m_count = 0;
};

#endif // SHAPE_H
