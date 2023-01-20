#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>
#include "shape.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
// для работы с opengl создаем свой виджет на основе QOpenGLWidget и protected QOpenGLFunctions

/*!
    \brief класс отрисовывает график на форме
    Класс - наследник QOpenGLWidget и QOpenGLFunctions,
    позволяет работать с функциями OpenGL
*/
class myglwidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    // конструтор и деструктор
    /*!
     \brief конструктор
     \param shape - объект класса Shape
     \param parent - родительский виджет
    */
    myglwidget( Shape shape, QWidget *parent = nullptr);
    ~myglwidget();
// методы унаследованные от QOpenGLWidget
protected:
    /// создание контекста и инициализация шейдеров
    void initializeGL() override;
    /// вывод на экран
    void paintGL() override;
    /// корретировка вида при изменении окна
    void resizeGL(int w, int h) override;
    /// обработчик нажатия кнопок мыши
    void mousePressEvent(QMouseEvent *event) override;
    /// обработчик движения мыши
    void mouseMoveEvent(QMouseEvent *event) override;
    /// обработчик нажатия кнопок клавиатуры
    void keyPressEvent(QKeyEvent *e) override;
    /// обработчик колесика мыши
    void wheelEvent(QWheelEvent *event) override;
// функциия-слот вызывается при уничтожении виджета
public slots:
    void cleanup();

private:
    // объект класса с данными графика
    Shape m_shape;
    // Vertex Array Object (или VAO) - специальный тип объектов, который инкапсулирует все данные,
    // связанные с вершинным процессором. Вместо хранения текущих данных они содержат ссылки
    // на вершинный буфер, буфер индексов и указания для слоев самих вершин. Преимущество в том, что
    // единожды настроив VAO для меша вы можете привести внутренние состояния меша просто привязав VAO.
    QOpenGLVertexArrayObject m_vao;
    // буфер для передачи данных графика в OpenGL
    QOpenGLBuffer m_vbo;
    // программа которая обрабатывает шейдер
    QOpenGLShaderProgram *m_program = nullptr;
    // матрица отвечает за перспективную проекцию
    QMatrix4x4 m_projection;
    // матрица отвечает за расположение модели
    QMatrix4x4 m_model;
    // буферы и списки точек для осей
    QOpenGLBuffer m_xvbo;
    QList<GLfloat> m_x_axis_data;
    QOpenGLBuffer m_yvbo;
    QList<GLfloat> m_y_axis_data;
    QOpenGLBuffer m_zvbo;
    QList<GLfloat> m_z_axis_data;
    // работа с камерой, где находится. куда смотрит, где у нее верх
    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;
    // переменные для доступа к переменным в шейдерах
    int m_projectionMatrixLoc = 0;
    int m_viewMatrixLoc = 0;
    int m_modelMatrixLoc = 0;
    int m_colorLoc = 0;
    // размеры виджета
    int width;
    int height;
    // переменные для работы с мышью
    bool firstMouse = true;
    float yaw   = -90.0f;
    float pitch =  0.0f;
    float lastX =  0.0;
    float lastY =  0.0;
    float fov   =  45.0f;
    // шейдеры, программы для opengl, нужны для вывода на экран и окрашивания в указанный цвет
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in highp vec3 color;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
        "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
        "layout(location=0) out vec4 fragColor;\n"
        "uniform vec3 color;\n"
        "void main() {\n"
        "   fragColor = vec4(color, 1.0);\n"
        "}\n";
};

#endif // MYGLWIDGET_H
