#include "vdp1.h"
#include "vdp2.h"


GLuint VAO, VBO, EBO;
bool debugMode = false; // This will call the displayGraphicsInfo function if set to true.

static void setupWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void validateVersion() {
    const char* version = (const char*) glGetString(GL_VERSION);
    int major, minor;
    sscanf(version, "%d.%d", &major, &minor);
    if (major * 10 + minor < 43) {
        printf("The opengl version is not at least 4.3. Please install a newer version of opengl.");
        glfwTerminate();
        closeWindow();
        exit(EXIT_FAILURE);
    }
}

static void enableCuda() {

}

char* getGraphicsROM() {
    return NULL;
}

void graphicsManager() {
    if (glfwInit() == GLFW_FALSE) {
        printf("An error occurred preventing GLFW from working.");
        exit(EXIT_FAILURE);
    }

    setupWindowHints();
    createWindow(800, 600);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.");
        closeWindow();
        exit(EXIT_FAILURE);
    }
    glViewport(0, 0, 800, 600);
    validateVersion();
    windowLoop();
    closeWindow();
    glfwTerminate();
}

int main() {
    graphicsManager();
    return 0;
}