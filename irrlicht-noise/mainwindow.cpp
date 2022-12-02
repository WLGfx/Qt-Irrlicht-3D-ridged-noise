#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    irrWidget = new IrrWidget(ui->renderWidget);

    // use a timer for now to update the widget
    connect(&timer, SIGNAL(timeout()), this, SLOT(irrWidgetUpdate()));
    timer.start(2);
}

MainWindow::~MainWindow()
{
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(irrWidgetUpdate()));
    delete irrWidget;
    delete ui;
}

void MainWindow::irrWidgetUpdate() {
    irrWidget->update();
    irrWidget->draw();
}

void MainWindow::on_btn_Start_clicked()
{
    irrWidget->update_noise(ui->spinBox_seed->value(),
                            ui->spinBox_Size->value(),
                            ui->doubleSpinBox_point_scale->value(),
                            ui->doubleSpinBox_noise_scale->value(),
                            ui->doubleSpinBox_cutoff->value());
}


void MainWindow::on_spinBox_seed_valueChanged(int arg1)
{

}


void MainWindow::on_spinBox_Size_valueChanged(int arg1)
{

}


void MainWindow::on_doubleSpinBox_point_scale_valueChanged(double arg1)
{

}


void MainWindow::on_doubleSpinBox_noise_scale_valueChanged(double arg1)
{

}

