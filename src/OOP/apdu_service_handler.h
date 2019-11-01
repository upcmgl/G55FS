#ifndef __APDU_SERVICE_HANDLER_H__
#define __APDU_SERVICE_HANDLER_H__

INT8U GetRequestNormal(objRequest *pRequest,objResponse *pResp);
INT8U GetRequestNormalList(objRequest *pRequest,objResponse *pResp);
INT8U GetRequestRecord(objRequest *pRequest,objResponse *pResp);
INT8U GetRequestRecordList(objRequest *pRequest,objResponse *pResp);
INT8U GetRequestNext(objRequest *pRequest,objResponse *pResp);
INT8U SetRequestNormal(objRequest *pRequest,objResponse *pResp);
INT8U SetRequestNormalList(objRequest *pRequest,objResponse *pResp);
INT8U SetThenGetRequestNormalList(objRequest *pRequest,objResponse *pResp);
INT8U ActRequestNormal(objRequest *pRequest,objResponse *pResp,INT8U *reset_flag);
INT8U ActThenGetRequestNormalList(objRequest *pRequest,objResponse *pResp);
void ProxyGetRequestList(objRequest *pRequest,objResponse *pResp);
void Proxy_Set_Request_List(objRequest *pRequest,objResponse *pResp);
void ProxySetRequestList(objRequest *pRequest,objResponse *pResp);
void ProxyActionRequestList(objRequest *pRequest,objResponse *pResp);
void ProxyActionThenGetRequestList(objRequest *pRequest,objResponse *pResp);
void ProxySetThenGetRequestList(objRequest *pRequest,objResponse *pResp);
void ProxyGetRequestRecord(objRequest *pRequest,objResponse *pResp);
void ProxyTransCommandRequest(objRequest *pRequest,objResponse *pResp);
INT8U process_get_object_attribute(objRequest *pRequest,objResponse *pResp,SEPARATE_FRAME *separate_frame);
INT8U process_req_record_object_attribute(objRequest *pRequest,objResponse *pResp,INT16U left_len,SEPARATE_FRAME *separate_frame);
INT8U process_set_object_attribute(objRequest *pRequest,objResponse *pResp);
INT8U process_action_dev_method(INT8U m_id,INT8U index,INT8U *data,INT16U len);
INT8U get_custom_info_id(INT8U attri_id);
INT16U get_oad_value(INT16U obis,INT8U attri_id, INT8U index, INT8U *data,tagParamObject *object);
INT8U set_oad_value(INT16U obis, INT8U attri_id, INT8U index_id, INT8U *data, tagParamObject *object, INT8U *buffer);
void load_ertu_param(INT16U obis, INT8U attri_id,INT8U index);
INT16U get_dev_ver_info(INT32U offset,INT8U index,INT8U *data);
BOOLEAN check_meter_doc_param(INT8U *buffer,INT8U index,INT8U *err_flag);
BOOLEAN check_batch_add_task_format(INT8U* param, INT8U count);
BOOLEAN check_batch_add_normal_format(INT8U* param,INT8U scheme_count);
BOOLEAN check_batch_add_event_format(INT8U* param,INT8U scheme_count);
//void exec_object_meter_doc_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);
//void exec_object_acq_ctrl_info_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);
//void exec_esam_oi_table_method(OBJECT_ATTR_DESC *oad,INT8U* param,objResponse *pResp);
void form_hold_state();
void rebuilt_task_plan_index();
BOOLEAN add_acq_ctrl_info(INT8U* param, INT16U len, INT16U max_size, INT8U max_count, uint32_t offset,INT8U acq_ctrl_flag);
void  delete_acq_data(INT16U beginSector,INT16U endSector); //��ʼ��ɾ���洢�Ķ������ߣ�ʵʱ������
#endif