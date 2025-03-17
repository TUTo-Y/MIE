#ifndef PATIENT_H
#define PATIENT_H

#include "gui.h"
#include "pack.h"
#include "core/RDH.h"

// 患者界面初始化, 用于添加和初始化控件
void patientInit();

bool patientChoiceImage(size_t wh, void *data);

// 患者点击选择按钮的回调函数
void patientButtonChoiceBackcall(GUIid id, size_t flag, void *data);

#endif // PATIENT_H