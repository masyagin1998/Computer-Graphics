#include "box.hpp"
#include <iostream>

// Я обленился.
#define pb(polygon, vertex) polygon.push_back(vertex)

Box::Box(Vertex ltn_, Vertex rbf_, Color color, GLuint texture,
         GLuint box_far_without_texture, GLuint box_near_without_texture,
         GLuint box_far_with_texture, GLuint box_near_with_texture) :
    color(color), texture(texture),
    box_far_without_texture(box_far_without_texture), box_near_without_texture(box_near_without_texture),
    box_far_with_texture(box_far_with_texture), box_near_with_texture(box_near_with_texture) {
    // Все "левые" вершины.
    Vertex ltn = ltn_;
    Vertex ltf = ltn_; ltf.Z = rbf_.Z;
    Vertex lbf = rbf_; lbf.X = ltn_.X;
    Vertex lbn = ltn_; lbn.Y = rbf_.Y;

    // Все "правые вершины."
    Vertex rbf = rbf_;
    Vertex rbn = rbf_; rbn.Z = ltn.Z;
    Vertex rtn = ltn_; rtn.X = rbf_.X;
    Vertex rtf = rbf_; rtf.Y = ltn_.Y;

    top.reserve(4);
    pb(top, ltn); pb(top, ltf); pb(top, rtf); pb(top, rtn);
    bottom.reserve(4);
    pb(bottom, lbn); pb(bottom, lbf); pb(bottom, rbf); pb(bottom, rbn);
    left.reserve(4);
    pb(left, lbn); pb(left, ltn); pb(left, ltf); pb(left, lbf);
    right.reserve(4);
    pb(right, rbn); pb(right, rtn); pb(right, rtf); pb(right, rbf);
    near.reserve(4);
    pb(near, lbn); pb(near, ltn); pb(near, rtn); pb(near, rbn);
    far.reserve(4);
    pb(far, lbf); pb(far, ltf); pb(far, rtf); pb(far, rbf);

    // Генерация массивов вершин, нормалей и текстур.

    Compile();
}

void Box::Compile() {
    glNewList(box_far_without_texture, GL_COMPILE);
    // Отрисовываем заднюю стенку.
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(color.R, color.G, color.B, color.A);
    glBegin(GL_QUADS);
        glVertex3f(far[0].X, far[0].Y, far[0].Z);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(far[1].X, far[1].Y, far[1].Z);
        glVertex3f(far[2].X, far[2].Y, far[2].Z);
        glVertex3f(far[3].X, far[3].Y, far[3].Z);

        glVertex3f(left[0].X, left[0].Y, left[0].Z);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(left[1].X, left[1].Y, left[1].Z);
        glVertex3f(left[2].X, left[2].Y, left[2].Z);
        glVertex3f(left[3].X, left[3].Y, left[3].Z);
        
        glVertex3f(top[0].X, top[0].Y, top[0].Z);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(top[1].X, top[1].Y, top[1].Z);
        glVertex3f(top[2].X, top[2].Y, top[2].Z);
        glVertex3f(top[3].X, top[3].Y, top[3].Z);

        glVertex3f(right[0].X, right[0].Y, right[0].Z);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(right[1].X, right[1].Y, right[1].Z);
        glVertex3f(right[2].X, right[2].Y, right[2].Z);
        glVertex3f(right[3].X, right[3].Y, right[3].Z);
        
        glVertex3f(bottom[0].X, bottom[0].Y, bottom[0].Z);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(bottom[1].X, bottom[1].Y, bottom[1].Z);
        glVertex3f(bottom[2].X, bottom[2].Y, bottom[2].Z);
        glVertex3f(bottom[3].X, bottom[3].Y, bottom[3].Z);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEndList();

    glNewList(box_near_without_texture, GL_COMPILE);
    // Отрисовка передней грани.
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(color.R, color.G, color.B, color.A);
    glBegin(GL_QUADS);
    glVertex3f(near[0].X, near[0].Y, near[0].Z);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(near[1].X, near[1].Y, near[1].Z);
        glVertex3f(near[2].X, near[2].Y, near[2].Z);
        glVertex3f(near[3].X, near[3].Y, near[3].Z);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEndList();

    glNewList(box_far_with_texture, GL_COMPILE);
    // Отрисовываем заднюю стенку.
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(color.R, color.G, color.B, color.A);
    glBegin(GL_QUADS);
        glTexCoord2f(0, -1);
        glVertex3f(far[0].X, far[0].Y, far[0].Z);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0, 0);
        glVertex3f(far[1].X, far[1].Y, far[1].Z);
        glTexCoord2f(1, 0);
        glVertex3f(far[2].X, far[2].Y, far[2].Z);
        glTexCoord2f(1, -1);
        glVertex3f(far[3].X, far[3].Y, far[3].Z);

        glTexCoord2f(0, -1);
        glVertex3f(left[0].X, left[0].Y, left[0].Z);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0);
        glVertex3f(left[1].X, left[1].Y, left[1].Z);
        glTexCoord2f(1, 0);
        glVertex3f(left[2].X, left[2].Y, left[2].Z);
        glTexCoord2f(1, -1);
        glVertex3f(left[3].X, left[3].Y, left[3].Z);
        
        glTexCoord2f(0, -1);
        glVertex3f(top[0].X, top[0].Y, top[0].Z);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0, 0);
        glVertex3f(top[1].X, top[1].Y, top[1].Z);
        glTexCoord2f(1, 0);
        glVertex3f(top[2].X, top[2].Y, top[2].Z);
        glTexCoord2f(1, -1);
        glVertex3f(top[3].X, top[3].Y, top[3].Z);

        glTexCoord2f(0, -1);
        glVertex3f(right[0].X, right[0].Y, right[0].Z);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0, 0);
        glVertex3f(right[1].X, right[1].Y, right[1].Z);
        glTexCoord2f(1, 0);
        glVertex3f(right[2].X, right[2].Y, right[2].Z);
        glTexCoord2f(1, -1);
        glVertex3f(right[3].X, right[3].Y, right[3].Z);
        
        glTexCoord2f(0, -1);
        glVertex3f(bottom[0].X, bottom[0].Y, bottom[0].Z);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0, 0);
        glVertex3f(bottom[1].X, bottom[1].Y, bottom[1].Z);
        glTexCoord2f(1, 0);
        glVertex3f(bottom[2].X, bottom[2].Y, bottom[2].Z);
        glTexCoord2f(1, -1);
        glVertex3f(bottom[3].X, bottom[3].Y, bottom[3].Z);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEndList();

    glNewList(box_near_with_texture, GL_COMPILE);
    // Отрисовка передней грани.
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(color.R, color.G, color.B, color.A);
    glBegin(GL_QUADS);
        glTexCoord2f(0, -1);
        glVertex3f(near[0].X, near[0].Y, near[0].Z);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 0);
        glVertex3f(near[1].X, near[1].Y, near[1].Z);
        glTexCoord2f(1, 0);
        glVertex3f(near[2].X, near[2].Y, near[2].Z);
        glTexCoord2f(1, -1);
        glVertex3f(near[3].X, near[3].Y, near[3].Z);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEndList();
}
