#include "mainview.h"
#include "math.h"

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}

/**
 * @brief MainView::convertHSLtoRGB
 *
 * Convert Hue Saturation Lightness to RGB value
 *
 * @param H Hue
 * @param S Saturation
 * @param L Lightness
 * @return QVector3D with the RGB values
 */
QVector3D MainView::convertHSLtoRGB(float H, float S, float L) {

    float C, Hp, X, m;
    QVector3D preRGB;

    // Grey values, S=0.0, so C=0.0
    C = (1.0 - abs(2.0*L - 1.0)) * S;
    m = L - (C / 2.0);
    Hp = (H * 360.0) / 60.0;
    X = C * (1.0 - abs(fmod(Hp, 2.0) - 1.0));

    if (Hp > 5.0) {
        preRGB = QVector3D(C,0.0,X);
    }
    else if (Hp > 4.0) {
        preRGB = QVector3D(X,0.0,C);
    }
    else if (Hp > 3.0) {
        preRGB = QVector3D(0.0,X,C);
    }
    else if (Hp > 2.0) {
        preRGB = QVector3D(0.0,C,X);
    }
    else if (Hp > 1.0) {
        preRGB = QVector3D(X,C,0.0);
    }
    else if (Hp >= 0.0) {
        preRGB = QVector3D(C,X,0.0);
    }

    return preRGB + QVector3D(m,m,m);
}
