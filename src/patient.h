#ifndef PATIENT_H
#define PATIENT_H

#include "gui.h"
#include "pack.h"
#include "core/RDH.h"

bool patientChoiceImage(size_t wh, void *data);

// 患者点击选择按钮的回调函数
void patientButtonChoiceBackcall(GUIid id, size_t flag, void *data);

// 患者点击上传按钮的回调函数
void patientButtonUploadBackcall(GUIid id, size_t flag, void *data);


#endif // PATIENT_H