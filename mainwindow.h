#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "myglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
// основное окно программы
/*!
    \brief основное окно программы
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// конструктор
    MainWindow(QWidget *parent = nullptr);
    /// деструктор
    ~MainWindow();

private slots:
  /// обработчик нажатия кнопки
  void handleButton();

private:
    Ui::MainWindow *ui;
    QLineEdit *lineEdit;
    void createInputForm(QOpenGLWidget* wid);
};
#endif // MAINWINDOW_H
