#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "irrwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    IrrWidget *irrWidget;

    QTimer timer;

private slots:
    void irrWidgetUpdate();
    void on_btn_Start_clicked();
    void on_spinBox_seed_valueChanged(int arg1);
    void on_spinBox_Size_valueChanged(int arg1);
    void on_doubleSpinBox_point_scale_valueChanged(double arg1);
    void on_doubleSpinBox_noise_scale_valueChanged(double arg1);
};
#endif // MAINWINDOW_H
