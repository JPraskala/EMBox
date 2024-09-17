#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H
#define SUCCESS 1
#define FAILURE 0

static void setupWindowHints();
static int versionNumber(const char* versionInfo);
static void* enableCuda();
char* getGraphicsROM();
int graphicsManager();

#endif