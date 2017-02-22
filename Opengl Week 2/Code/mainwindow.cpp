#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateRotation()
{
    int x = ui->xSlider->value();
    int y = ui->ySlider->value();
    int z = ui->zSlider->value();
    ui->mainView->updateRotation(x,y,z);
}

void MainWindow::on_xSlider_valueChanged(int value)
{
    Q_UNUSED(value)
    updateRotation();
}

void MainWindow::on_ySlider_valueChanged(int value)
{
    Q_UNUSED(value)
    updateRotation();
}

void MainWindow::on_zSlider_valueChanged(int value)
{
    Q_UNUSED(value)
    updateRotation();
}

void MainWindow::on_shaderSelect_currentIndexChanged(const QString &arg1)
{
    ui->mainView->updateShader(arg1);
}

void MainWindow::on_modelSelect_currentIndexChanged(const QString &arg1)
{
    ui->mainView->updateModel(arg1);
}

void MainWindow::on_scaleSlider_valueChanged(int value)
{
    double scale = (double)value / (double)(ui->scaleSlider->maximum()/3);
    ui->mainView->updateScale((float)scale);
}
