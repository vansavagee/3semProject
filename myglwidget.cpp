#include "myglwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>

// конструктор, тут задаем начальное положение камеры, координаты осей и объект с данными графика
myglwidget::myglwidget(Shape shape, QWidget *parent)
{
    setFocusPolicy(Qt::StrongFocus);
    cameraPos   = QVector3D(0.0f, 0.0f, 20.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp    = QVector3D(0.0f, 1.0f, 0.0f);

    m_x_axis_data = {-3.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f};
    m_y_axis_data = {0.0f, -3.0f, 0.0f, 0.0f, 3.0f, 0.0f};
    m_z_axis_data = {0.0f, 0.0f, -3.0f, 0.0f, 0.0f, 3.0f};

    m_shape = shape;
}
// деструктор
myglwidget::~myglwidget()
{
    cleanup();
}

// функция вызывает один раз при создании виджета
void myglwidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &myglwidget::cleanup);

    initializeOpenGLFunctions();
    // заливаем черным цветом
    glClearColor(0, 0, 0, 1);
    // создаем шейдерную программу
    m_program = new QOpenGLShaderProgram;
    // передаем ей наши шейдера
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    // привязываем переменную в шейдере aPos, в ней будет хранится объект который будет отрисовыватся
    m_program->bindAttributeLocation("aPos", 0);
    m_program->link();
    // привязываемся к переменным в шейдере, матрицы и цвет
    m_program->bind();
    m_projectionMatrixLoc = m_program->uniformLocation("projection");
    m_viewMatrixLoc = m_program->uniformLocation("view");
    m_modelMatrixLoc = m_program->uniformLocation("model");
    m_colorLoc = m_program->uniformLocation("color");
    // создаем Vertex Array Object
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    // тут создаем все буфера и выделяем под них память
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_shape.constData(), m_shape.count() * sizeof(GLfloat));

    m_xvbo.create();
    m_xvbo.bind();
    m_xvbo.allocate(m_x_axis_data.constData(), m_x_axis_data.count() * sizeof(GLfloat));

    m_yvbo.create();
    m_yvbo.bind();
    m_yvbo.allocate(m_y_axis_data.constData(), m_y_axis_data.count() * sizeof(GLfloat));

    m_zvbo.create();
    m_zvbo.bind();
    m_zvbo.allocate(m_z_axis_data.constData(), m_z_axis_data.count() * sizeof(GLfloat));

    m_program->release();
}
// функция выводит на экран график
void myglwidget::paintGL()
{
    // тут указываем опции отрисовки, так как график трехмерный
    // нужно проверять положение по оси z, не отрисовывать невидимое
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // матрица перспективной проекции
    m_projection.setToIdentity();
    m_projection.perspective(fov, GLfloat(width) / height, 0.01f, 1000.0f);
    // матрица вида, выщитывается по позиции камеры
    QMatrix4x4 view;
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // модель ставим просто в центр
    m_model.setToIdentity();
    // отправляем матрицы и цвет графика в шейдер
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projectionMatrixLoc, m_projection);
    m_program->setUniformValue(m_viewMatrixLoc, view);
    m_program->setUniformValue(m_modelMatrixLoc, m_model);
    m_program->setUniformValue(m_colorLoc, QVector3D(1.0f, 1.0f, 0.0f));

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    // работаем с буфером графика, указываем, что нужно брать по 3 точки
    m_vbo.bind();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_vbo.release();
    // выводим график на экран
    glDrawArrays(GL_POINTS, 0, m_shape.vertexCount());
    // тут работаем с буфером оси х
    m_xvbo.bind();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_xvbo.release();
    // меняем цвет
    m_program->setUniformValue(m_colorLoc, QVector3D(0.0f, 1.0f, 0.0f));

    // для линии нужны только 2 точки
    glDrawArrays(GL_LINES, 0, 2);
    // остальные оси обрабатываем так же как х
    m_yvbo.bind();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_yvbo.release();

    m_program->setUniformValue(m_colorLoc, QVector3D(1.0f, 0.0f, 0.0f));

    glDrawArrays(GL_LINES, 0, 2);

    m_zvbo.bind();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    m_zvbo.release();

    m_program->setUniformValue(m_colorLoc, QVector3D(0.0f, 0.0f, 1.0f));

    glDrawArrays(GL_LINES, 0, 2);

    m_program->release();
}
// при изменении размеров окна пересчитываем перспективную проекцию
void myglwidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
    m_projection.setToIdentity();
    m_projection.perspective(fov, GLfloat(width) / height, 0.01f, 1000.0f);
}
// при нажатии на кнопки мыши устанавливаем флаг, так избегаем резких перемещений вида
void myglwidget::mousePressEvent(QMouseEvent *event)
{
    firstMouse = true;
}
// обработчик мыши, позволяет изменять направление взгляда камеры,
// работает если нажата какая-нибудь кнопка мыши
void myglwidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->position().toPoint().x();
    int dy = event->position().toPoint().y();

    float xpos = dx;
    float ypos = dy;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    QVector3D front;
    front.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.setY(sin(qDegreesToRadians(pitch)));
    front.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    cameraFront = front.normalized();
    update();
}
// обработчик кнопок клавиатуры
void myglwidget::keyPressEvent(QKeyEvent *e)
{
    float cameraSpeed = 0.1f;
    bool changed = false;
    switch (e->key()) {
    case Qt::Key_A:
        cameraPos -= (QVector3D::crossProduct(cameraFront, cameraUp)).normalized() * cameraSpeed;
        changed = true;
        break;
    case Qt::Key_S:
        cameraPos -= cameraFront * cameraSpeed;
        changed = true;
        break;
    case Qt::Key_W:
        cameraPos += cameraFront * cameraSpeed;
        changed = true;
        break;
    case Qt::Key_D:
        cameraPos += (QVector3D::crossProduct(cameraFront, cameraUp)).normalized() * cameraSpeed;
        changed = true;
        break;
    }
    if(changed)
        update();
    QOpenGLWidget::keyPressEvent(e);
}
// обработчик колесика мыши, изменяет проекцию
void myglwidget::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->angleDelta();
    fov -= (float)(numPixels.y() / 120);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
    event->accept();
    update();
}
// функция очистки
void myglwidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    m_vbo.destroy();
    m_xvbo.destroy();
    m_yvbo.destroy();
    m_zvbo.destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}
