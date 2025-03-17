#ifndef DOCTOR_H
#define DOCTOR_H

#include "gui.h"
#include <stb_image_write.h>

void *doctorWaitThread(void *data);
void doctorSend(GUIid id, size_t flag, void *data);
#endif // DOCTOR_H