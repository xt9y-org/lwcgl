#define LWCGL_ENABLE_LWJGL2_COMPAT 1
#include <lwcgl/lwcgl.h>
#include <lwcgl/context.h>
#include <lwcgl/glmodern.h>
#include <stdio.h>
int main(void) {
    DisplayMode mode = {320, 200, 0, 0, LWCGL_FALSE};
    if (Display.setDisplayMode(&mode) != 0) return 1;
    Display.setTitle("lwcgl runtime smoke: titles are dynamically stored and are not truncated to the old fixed buffer");
#ifdef __APPLE__
    /*
     * macOS exposes OpenGL 2.1 compatibility and up to 4.1 core, but no 4.3
     * compatibility context. A high compatibility request must degrade to a
     * usable platform context instead of making Display.create() fail.
     */
    lwcglSetContextVersion(4, 3);
    lwcglSetContextProfile(LWCGL_CONTEXT_COMPATIBILITY_PROFILE);
#else
    lwcglSetContextVersion(2, 1);
    lwcglSetContextProfile(LWCGL_CONTEXT_COMPATIBILITY_PROFILE);
#endif
    if (Display.create() != 0) { fprintf(stderr, "%s\n", lwcglGetLastError()); return 2; }
    if (!lwcglModernGLAvailable()) return 3;
#ifdef __APPLE__
    if (lwcglModernGLMajorVersion() < 2) return 9;
#endif
    if (Display.getWidth() <= 0 || Display.getHeight() <= 0) return 4;
    if (lwcglDisplayGetWindowWidth() <= 0) return 10;
    if (lwcglDisplayGetWindowHeight() <= 0) return 11;

    (void)glGetInteger(GL_VIEWPORT);
    IntBuffer *viewport = BufferUtils.createIntBuffer(4);
    if (!viewport) return 5;
    glGetInteger(GL_VIEWPORT, viewport);
    BufferUtils.destroy(viewport);

    glViewport(0, 0, Display.getWidth(), Display.getHeight());
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
    unsigned char pixel[4] = {0, 0, 0, 0};
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    if (pixel[0] < 40 || pixel[1] < 90 || pixel[2] < 140 || pixel[3] < 200) return 6;

    if (Keyboard.create() != 0 || Mouse.create() != 0) return 7;
    Keyboard.poll(); Mouse.poll(); Display.processMessages();
    Mouse.setGrabbed(LWCGL_TRUE); Mouse.setGrabbed(LWCGL_FALSE);
    Display.updateNoMessages();
    Display.destroy();
    if (Keyboard.isCreated() || Mouse.isCreated() || Display.isCreated()) return 8;
    return 0;
}
