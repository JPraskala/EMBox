#include "vdp2.h"

GLFWwindow* window = NULL;

void createWindow(GLint width, GLint height) {
    window = glfwCreateWindow(width, height, "Test Screen (Temporary)", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create the window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
}

static void framebuffer_size_callback(GLFWwindow* windowCopy, int width, int height) {
    glViewport(0, 0, width, height);
}

void windowLoop() {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        glClearColor(0.4f, 0.25f, 0.8f, 0.75f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void closeWindow() {
    glfwDestroyWindow(window);
}