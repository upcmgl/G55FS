
#ifndef CJSONFRAMEPACK_H
#define CJSONFRAMEPACK_H
#include "cJSON.h"
#include "../main_include.h"
#ifdef __cplusplus
extern "C" {
#endif

#define __SIMULATION__
#define __BEIJING_ONENET_EDP__
//#define __ZHEJIANG_ONENET_EDP__
#ifdef __BEIJING_ONENET_EDP__
//�豸������Ϣ
INT16U  cjsonDeviceConfigPack(INT8U * buf);
//�¼��澯��Ϣ
INT16U cjsonEventWarningPack(INT8U *buf,INT8U flag);
//�̼�������Ϣ
INT16U cjsonFirmwareUpdatePack(INT8U *buf,INT8U flag);
//ʵʱ������Ϣ
INT16U cjsonEnergyInfoPack(char dayhold,INT8U *buf);
//��������Ϣ���ݿ�
INT16U cjsonEnergyBlockPack(INT8U *buf);
INT16U cjsonReadMeter(INT8U *buf);
#endif

#ifdef __ZHEJIANG_ONENET_EDP__
//�豸������Ϣ
void  cjsonDeviceConfigPack();
//�¼��澯��Ϣ
void cjsonEventWarningPack();
//�̼�������Ϣ
void cjsonFirmwareUpdatePack();
//ʵʱ������Ϣ
void cjsonEnergyInfoPack(char dayhold);
//��������Ϣ���ݿ�
void cjsonEnergyBlockPack();
#endif

#ifdef __cplusplus
}
#endif

#endif