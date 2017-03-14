#include "mainview.h"
#include <cmath>

void MainView::renderSphere(GLint type, GLint theta, QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    model = QMatrix4x4();
    float isSun;
    if (type == 1) isSun = 1.0; else isSun = 0.0;
    if (type == 1) {
        //Center
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sun);
        model.scale(QVector3D(scale, scale, scale));
    }
    else if (type == 2) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mars);

        model.rotate(theta, 0, 1, 0);
        model.translate(pos);
        model.rotate(theta, 0, 1, 0);
        model.scale(QVector3D(scale, scale, scale));
    }
    else if (type == 3) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earth);

        model.rotate(theta, 0, 1, 0);
        model.translate(pos);
        model.rotate(theta, 0, 1, 0);
        model.scale(QVector3D(scale, scale, scale));
    }
    else if (type == 4) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, jupiter);

        model.rotate(theta, 0, 1, 0);
        model.translate(pos);
        model.rotate(theta, 0, 1, 0);
        model.scale(QVector3D(scale, scale, scale));
    }
    else if (type == 5) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, saturn);

        model.rotate(theta, 0, 1, 0);
        model.translate(pos);
        model.rotate(theta, 0, 1, 0);
        model.scale(QVector3D(scale, scale, scale));
    }

    normal = (view * model).normalMatrix();

    //send data to shader program
    glUniformMatrix4fv(glModel, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(glView, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(glProjection, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(glNormal, 1, GL_FALSE, normal.data());

    glUniform3f(glLightPosition, lightpos.x(), lightpos.y(), lightpos.z());
    glUniform3f(glColorFrag, color.x(), color.y(), color.z());
    glUniform4f(glMaterial, material.x(), material.y(), material.z(), material.w());
    glUniform1f(glIsSun, isSun);

    //draw sphere
    glDrawArrays(GL_TRIANGLES, 0, numTris * 3);
}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainView::renderRaytracerScene()
{

    QVector3D lightpos = QVector3D(0,0,0);

    // Sun here
    renderSphere(1,0,QVector3D(0,0,0),QVector3D(0,0,1),QVector4D(0.2f,0.7f,0.5f,64),lightpos);

    // Second planet
    angle2 = fmod(angle2 + 0.7, 360);
    renderSphere(2,angle2,QVector3D(200,0,0),QVector3D(0,0,1),QVector4D(0.2f,0.7f,0.5f,64),lightpos);

    // Third planet
    angle3 = fmod(angle3 + 0.6, 360);
    renderSphere(3,angle3,QVector3D(-300,0,0),QVector3D(1,0,0),QVector4D(0.2f,0.7f,0.8f,32),lightpos);

    // Fourth planet
    angle4 = fmod(angle4 + 0.5, 360);
    renderSphere(4,angle4,QVector3D(450,0,0),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos);

    // Fifth sphere
    angle5 = fmod(angle5 + 0.6, 360);
    renderSphere(5, angle5, QVector3D(570,100,0),QVector3D(1,0.5f,0),QVector4D(0.2f,0.8f,0.5f,32),lightpos);
}
