#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mathexpr.h"

// функция для создания формы ввода формулы
void MainWindow::createInputForm(QOpenGLWidget *wid)
{
    QGroupBox *groupBox = new QGroupBox(tr("Форма ввода формулы"), wid);
    groupBox->setGeometry(10, 10, 240, 60);
    groupBox->setStyleSheet("QGroupBox { color : white;}");
    QHBoxLayout *vbox = new QHBoxLayout;
    QLabel *lbltop = new QLabel("z=");
    lbltop->setStyleSheet("QLabel { background-color : black; color : white; }");
    vbox->addWidget(lbltop);
    lineEdit = new QLineEdit();
    vbox->addWidget(lineEdit);
    QPushButton *button = new QPushButton("Нарисовать");
    vbox->addWidget(button);
    groupBox->setLayout(vbox);
    groupBox->show();
    connect(button, &QPushButton::released, this, &MainWindow::handleButton);
}

// конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createInputForm(ui->openGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// обработчик кнопки на форме ввода формулы
void MainWindow::handleButton()
{
    // получаем текст формулы и убираем пробелы в начале и конце
    QString text = lineEdit->text().trimmed();
    // если текст пустой ничего не делаем
    if(text != "")
    {
        // используем библиотеку для обработки строки

        // регистрируем переменные
        double x;
        RVar xvar ( "x" , &x );
        double y;
        RVar yvar ( "y" , &y );

        RVar* vararray[2];
        vararray[0]=&xvar;
        vararray[1]=&yvar;

        char* b;
        b = (char*)text.toStdString().c_str();
        // обрабатываем выражение
        ROperation op ( b, 2, vararray );
        // для проверки задаем значения
        x = 1;
        y = 1;
        // если формулу не удается обработать выдаем сообщение об ошибке и выходим
        if(strcmp(op.Expr(), "Error") == 0)
        {
            QMessageBox msgBox;
            msgBox.setText("Не удается решить формулу! Проверьте запись");
            msgBox.exec();
            return;
        }
        // тут будут хранится точки графика
        QList<GLfloat> data;
        // вычисляем в циклах по х и по y
        for( x = -1; x <= 1; x += 0.003){
            for( y = -1; y <= 1; y += 0.003){
                // вычисляем z
                double z = op.Val();
                if(z >= -1 && z <= 1)
                {
                    // добавляем в список x, y и z
                    data.append(x);
                    data.append(y);
                    data.append(z);
                }
            }
        }
        // создаем объект в данными графика
        Shape sh(data);
        // создаем opengl виджет
        myglwidget* w = new myglwidget(sh);
        // форма вводы формулы привязывается к opengl виджет
        // и при его замене она будет уничтожена,
        // поэтому создаем новую форму ввода формулы для эого виджета
        createInputForm(w);
        // удаляем сиарый виджет
        QLayoutItem *wItem;
        while ((wItem = ui->centralwidget->layout()->takeAt(0)) != 0)
        {
            if (wItem->widget())
                wItem->widget()->setParent(NULL);
            delete wItem;
        }
        // ствим новый виджет
        ui->centralwidget->layout()->addWidget(w);
    }
}

