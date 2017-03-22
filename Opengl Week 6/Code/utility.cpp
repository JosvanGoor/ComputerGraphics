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

QVector<quint8> MainView::imageToBytes(QImage image) {
    // needed since (0,0) is bottom left in OpenGL
    QImage im = image.mirrored();
    
    QVector<quint8> pixelData;
    pixelData.reserve(im.width()*im.height()*4);
    
    quint8 r,g,b,a; // Use an unsigned byte
    int i, j;
    for (i = 0; i != im.height(); ++i) {
        for (j = 0; j != im.width(); ++j) {
            QRgb pixel = im.pixel(j,i);
            
            // pixel is of format #AARRGGBB (in hexadecimal notation)
            // so with bitshifting and binary AND you can get
            // the values of the different components
            r = (quint8)((pixel >> 16) & 0xFF); // Red component
            g = (quint8)((pixel >> 8) & 0xFF); // Green component
            b = (quint8)(pixel & 0xFF); // Blue component
            a = (quint8)((pixel >> 24) & 0xFF); // Alpha component
            
            // Add them to the Vector
            pixelData.append(r);
            pixelData.append(g);
            pixelData.append(b);
            pixelData.append(a);
        }
    }
    return pixelData;
}
