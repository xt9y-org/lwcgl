#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static GLFWwindow *display_window(void)
{
    return (GLFWwindow *)Display.getNativeWindow();
}

int lwcglDisplayGetWindowWidth(void)
{
    const DisplayMode mode = Display.getDisplayMode();
    GLFWwindow *window = display_window();
    int width = mode.width;

    if (window && !Display.isFullscreen())
        glfwGetWindowSize(window, &width, NULL);

    return width > 0 ? width : mode.width;
}

int lwcglDisplayGetWindowHeight(void)
{
    const DisplayMode mode = Display.getDisplayMode();
    GLFWwindow *window = display_window();
    int height = mode.height;

    if (window && !Display.isFullscreen())
        glfwGetWindowSize(window, NULL, &height);

    return height > 0 ? height : mode.height;
}
