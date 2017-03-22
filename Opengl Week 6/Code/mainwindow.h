#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_xSlider_valueChanged(int value);

    void on_ySlider_valueChanged(int value);

    void on_zSlider_valueChanged(int value);

    void on_shaderSelect_currentIndexChanged(const QString &arg1);

    void on_modelSelect_currentIndexChanged(const QString &arg1);

    void on_scaleSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    void updateRotation();
};

#endif // MAINWINDOW_H
