#ifndef __PLMS_FTP_H__
#define __PLMS_FTP_H__

//��������壬Ŀǰ����376.1�ļ�������ļ���ʶ����
#define FILEFLAG_CLEAR            0x00    //��������ļ�
#define FILEFLAG_DEV_PROG         0x01    //�ն������ļ�

BOOLEAN read_datfile_header(DATHEADER *header, INT8U *buffer);
BOOLEAN check_update_header(DATHEADER *header,INT8U fileflag);
void update_download_record(INT16U sec_count,INT16U cur_sec,INT16U sec_len);
BOOLEAN check_download_record(INT8U *buffer);
BOOLEAN datfile_update(DATHEADER *header, INT8U *resetFlag,INT8U *buffer);
BOOLEAN check_datfile(DATHEADER *header, INT8U *buffer);

#endif



