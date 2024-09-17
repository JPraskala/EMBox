#include "vdp1.h"
#include "vdp2.h"
#include "graphicManager.h"
#include <string.h>

GLuint VAO, VBO, EBO;
bool debugMode = false; // This will call the displayGraphicsInfo function if set to true.

static void setupWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void* enableCuda() {
    return NULL;
}

static int versionNumber(const char* versionInfo) {
    // const char* version = (const char*) glGetString(GL_VERSION);
    // printf("%s\n", version);
    int major, minor;
    sscanf(versionInfo, "%d.%d", &major, &minor);
    /*
    if (major * 10 + minor < 43) {
        printf("The opengl version is not at least 4.3. Please install a newer version of opengl.");
        glfwTerminate();
        closeWindow();
        exit(EXIT_FAILURE);
    }

    if (strstr(version, "NVIDIA") != NULL) {
        enableCuda();
    }
     */
    return major * 10 + minor;
}

char* getGraphicsROM() {
    return NULL;
}

int graphicsManager() {
    if (glfwInit() == GLFW_FALSE) {
        printf("An error occurred preventing GLFW from working.");
        return FAILURE;
    }

    setupWindowHints();
    GLFWwindow* window = createWindow(800, 600);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.");
        closeWindow(window);
        return FAILURE;
    }
    glViewport(0, 0, 800, 600);
    const char* versionStr = (const char*) glGetString(GL_VERSION);
    if (versionNumber(versionStr) < 43) {
        printf("The version of Opengl is below 4.3. Please install a newer version.");
        return FAILURE;
    }
    // validateVersion();
    windowLoop(window);
    closeWindow(window);
    glfwTerminate();

    VDP1* test = initVDP1();
    if (test != NULL) {
        printf("Successfully created vdp1\n");
        freeVDP1(test);
        printf("Freed memory");
    }

    return SUCCESS;
}