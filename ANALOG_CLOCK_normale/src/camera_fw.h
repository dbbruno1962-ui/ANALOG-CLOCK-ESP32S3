#ifndef CAMERA_FW_H
#define CAMERA_FW_H

#include <Arduino.h>

bool cameraStart(void);
void cameraStop(void);
void cameraHandle(void);
bool cameraIsActive(void);

#endif
