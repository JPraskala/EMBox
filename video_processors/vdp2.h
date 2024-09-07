#ifndef VDP2_H
#define VDP2_H
#include "vdp1.h"



typedef enum {NGB0, NGB1, NGB2, NGB3} ScrollScreens;
typedef enum {RGB0, RGB1} RotationScreens; // If RGB1 is enabled, we can't use NGB0-NGB3
typedef enum {BACK, LINE_COLOR} LineScreens;

void createWindow(GLint width, GLint height);
static void framebuffer_size_callback(GLFWwindow* windowCopy, int width, int height);
void windowLoop();
static void testInput();
void closeWindow();


#endif