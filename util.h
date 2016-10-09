//
// Created by coderobe on 09.10.16.
//

#ifndef INC_0XNATIVE_UTIL_H
#define INC_0XNATIVE_UTIL_H
static void die(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

static struct nk_image icon_load(const char *filename) {
    int x, y, n;
    GLuint tex;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    if (!data) die("[GLEW]: failed to load image: %s", filename);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // FIXME: figure out why this segfaults
    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return nk_image_id((int) tex);
}

static void error_callback(int e, const char *d) {
    printf("Error %d: %s\n", e, d);
}
#endif //INC_0XNATIVE_UTIL_H
