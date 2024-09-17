#include "vdp2.h"

// GLFWwindow* window = NULL;
bool colorChanged = false;

GLFWwindow* createWindow(GLint width, GLint height) {
    GLFWwindow* window = glfwCreateWindow(width, height, "Test Screen (Temporary)", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create the window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    return window;
}

static void framebuffer_size_callback(GLFWwindow* windowCopy, int width, int height) {
    glViewport(0, 0, width, height);
}

static void testInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        glClearColor(0.65f, 0.65f, 0.7f, 0.75f);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        glClearColor(0.67f, 0.9f, 0.75f, 0.6f);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glClearColor(0.4f, 0.25f, 0.8f, 0.75f);
    }

    colorChanged = true;
}

void windowLoop(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while (!glfwWindowShouldClose(window)) {

        if (!colorChanged)
            glClearColor(0.4f, 0.25f, 0.8f, 0.75f);
        testInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void closeWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
}