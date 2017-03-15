/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <mainview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *settingsBox;
    QFormLayout *formLayout;
    QLabel *xRotationLabel;
    QSlider *xSlider;
    QLabel *yRotationLabel;
    QSlider *ySlider;
    QLabel *zRotationLabel;
    QSlider *zSlider;
    QSpacerItem *vSpacer;
    QLabel *shaderLabel;
    QComboBox *shaderSelect;
    QLabel *modelLabel;
    QComboBox *modelSelect;
    QLabel *scaleLabel;
    QSlider *scaleSlider;
    MainView *mainView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        settingsBox = new QGroupBox(centralWidget);
        settingsBox->setObjectName(QStringLiteral("settingsBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settingsBox->sizePolicy().hasHeightForWidth());
        settingsBox->setSizePolicy(sizePolicy);
        settingsBox->setMinimumSize(QSize(200, 0));
        formLayout = new QFormLayout(settingsBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        xRotationLabel = new QLabel(settingsBox);
        xRotationLabel->setObjectName(QStringLiteral("xRotationLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, xRotationLabel);

        xSlider = new QSlider(settingsBox);
        xSlider->setObjectName(QStringLiteral("xSlider"));
        xSlider->setMaximum(360);
        xSlider->setOrientation(Qt::Horizontal);

        formLayout->setWidget(0, QFormLayout::FieldRole, xSlider);

        yRotationLabel = new QLabel(settingsBox);
        yRotationLabel->setObjectName(QStringLiteral("yRotationLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, yRotationLabel);

        ySlider = new QSlider(settingsBox);
        ySlider->setObjectName(QStringLiteral("ySlider"));
        ySlider->setMaximum(360);
        ySlider->setOrientation(Qt::Horizontal);

        formLayout->setWidget(1, QFormLayout::FieldRole, ySlider);

        zRotationLabel = new QLabel(settingsBox);
        zRotationLabel->setObjectName(QStringLiteral("zRotationLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, zRotationLabel);

        zSlider = new QSlider(settingsBox);
        zSlider->setObjectName(QStringLiteral("zSlider"));
        zSlider->setMaximum(360);
        zSlider->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, zSlider);

        vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(3, QFormLayout::LabelRole, vSpacer);

        shaderLabel = new QLabel(settingsBox);
        shaderLabel->setObjectName(QStringLiteral("shaderLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, shaderLabel);

        shaderSelect = new QComboBox(settingsBox);
        shaderSelect->setObjectName(QStringLiteral("shaderSelect"));

        formLayout->setWidget(4, QFormLayout::FieldRole, shaderSelect);

        modelLabel = new QLabel(settingsBox);
        modelLabel->setObjectName(QStringLiteral("modelLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, modelLabel);

        modelSelect = new QComboBox(settingsBox);
        modelSelect->setObjectName(QStringLiteral("modelSelect"));

        formLayout->setWidget(5, QFormLayout::FieldRole, modelSelect);

        scaleLabel = new QLabel(settingsBox);
        scaleLabel->setObjectName(QStringLiteral("scaleLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, scaleLabel);

        scaleSlider = new QSlider(settingsBox);
        scaleSlider->setObjectName(QStringLiteral("scaleSlider"));
        scaleSlider->setMinimum(1);
        scaleSlider->setMaximum(3000);
        scaleSlider->setSingleStep(30);
        scaleSlider->setPageStep(300);
        scaleSlider->setValue(1000);
        scaleSlider->setOrientation(Qt::Horizontal);
        scaleSlider->setInvertedAppearance(false);
        scaleSlider->setInvertedControls(false);
        scaleSlider->setTickPosition(QSlider::NoTicks);
        scaleSlider->setTickInterval(100);

        formLayout->setWidget(6, QFormLayout::FieldRole, scaleSlider);


        horizontalLayout->addWidget(settingsBox);

        mainView = new MainView(centralWidget);
        mainView->setObjectName(QStringLiteral("mainView"));

        horizontalLayout->addWidget(mainView);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        settingsBox->setTitle(QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
        xRotationLabel->setText(QApplication::translate("MainWindow", "x rotation", Q_NULLPTR));
        yRotationLabel->setText(QApplication::translate("MainWindow", "y rotation", Q_NULLPTR));
        zRotationLabel->setText(QApplication::translate("MainWindow", "z rotation", Q_NULLPTR));
        shaderLabel->setText(QApplication::translate("MainWindow", "Renderer", Q_NULLPTR));
        shaderSelect->clear();
        shaderSelect->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Flat", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Goraud", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Phong", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Toon", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Normal", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Textured", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Deferred blitted", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Deferred", Q_NULLPTR)
        );
        modelLabel->setText(QApplication::translate("MainWindow", "Model", Q_NULLPTR));
        modelSelect->clear();
        modelSelect->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "cube.obj", Q_NULLPTR)
         << QApplication::translate("MainWindow", "sphere.obj", Q_NULLPTR)
         << QApplication::translate("MainWindow", "flat_surface.obj", Q_NULLPTR)
         << QApplication::translate("MainWindow", "cat.obj", Q_NULLPTR)
        );
        scaleLabel->setText(QApplication::translate("MainWindow", "Scale", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
