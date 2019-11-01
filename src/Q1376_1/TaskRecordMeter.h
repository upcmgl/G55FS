#ifndef __TASKRECORDMETER_H__
#define __TASKRECORDMETER_H__

//���ɱ�̰���֪ͨ����
#define DIS_CYPROGNOTE   0x00
#define EN_CYPROGNOTE    0x01

#define BTN_CYPROG_MASK  0x02

//==============================================================================
//
//          485������ݰ���
//
//==============================================================================


//�������������������
#define  SPOT_CURVE_PQUI        1          //�����㹦�ʣ���ѹ����������
#define  SPOT_CURVE_ZFXYWG      2          //�����������ʾֵ����
#define  SPOT_CURVE_AMOUNTS     3          //���������������
#define  RTUA_CURVE_NOISE       4          //�ز����ڵ���������
#define  SPOT_CURVE_FL  		5          //��������ʵ���������

#define REC_DATE_MINUTE   0
#define REC_DATE_HOUR     1
#define REC_DATE_DAY      2
#define REC_DATE_MONTH    3
#define REC_DATE_YEAR     4


//��ǰ�����ļ�����  FILEID_485_CUR
#if defined (__QGDW_376_2013_PROTOCOL__)
#define     C1_F26_LEN          61
#define     C1_F161_LEN          18

#ifdef __PROVICE_SHANXI__
	#define     C1_F165_LEN          32
#else
	#define     C1_F165_LEN          23
#endif
#define     C1_F166_LEN          29
#else
#define     C1_F26_LEN          57
#define     C1_F161_LEN          16
#define     C1_F165_LEN          19
#define     C1_F166_LEN          19
#endif
#define     C2_F209_LEN         C1_F161_LEN
#define     C2_F32_LEN          C1_F26_LEN
#define PIM_C1_F25           0                         //67
#define    PIM_C1_F25_V     37 
#define PIM_C1_F26          67                         //C1_F26_LEN,
#define PIM_C1_F27         (PIM_C1_F26+C1_F26_LEN)     //60,
#define PIM_C1_F28         (PIM_C1_F27+60)             //33
#define PIM_C1_F33         (PIM_C1_F28+33)             //91,
#define PIM_C1_F34         (PIM_C1_F33+91)             //91,
#define PIM_C1_F35         (PIM_C1_F34+91)             //76, ���������С��޹��������������ʱ��
#define PIM_C1_F36         (PIM_C1_F35+76)             //76, ���·����С��޹��������������ʱ��
#define PIM_C1_F37         (PIM_C1_F36+76)             //91,
#define PIM_C1_F38         (PIM_C1_F37+91)             //91,
#define PIM_C1_F33_DAY     (PIM_C1_F38+91)             //91, �������ճ�ֵ
#define PIM_C1_F34_DAY     (PIM_C1_F33_DAY+91)         //91, �������ճ�ֵ
#define PIM_C1_F33_MONTH   (PIM_C1_F34_DAY+91)          //91, �������³�ֵ
#define PIM_C1_F34_MONTH   (PIM_C1_F33_MONTH+91)       //91, �������³�ֵ
#define PIM_C1_F58         (PIM_C1_F34_MONTH+91)       //223,г��������
#define PIM_C1_F57         (PIM_C1_F58+223)            //217,г����Чֵ
#define PIM_C1_F49         (PIM_C1_F57+217)            //12, ��ǰ��ѹ��������λ��
#define PIM_C1_F161        (PIM_C1_F49+12)             //C1_F161_LEN, ���ܱ�Զ�̿���ͨ�ϵ�״̬����¼
#define PIM_C1_F165        (PIM_C1_F161+C1_F161_LEN)   //C1_F165_LEN, ���ܱ��ز���������ʱ��
#define PIM_C1_F166        (PIM_C1_F165+C1_F165_LEN)   //C1_F166_LEN, ���ܱ�����޸Ĵ�����ʱ��
#define PIM_C1_F167        (PIM_C1_F166+C1_F166_LEN)   //41, ���ܱ����õ���Ϣ
#define PIM_C1_F168        (PIM_C1_F167+41)            //56, ���ܱ������Ϣ
#define PIM_C1_F31         (PIM_C1_F168+56)            //59, ��ǰABC�����������й�����ʾֵ������޹�1��2����ʾֵ
#define PIM_C1_F32         (PIM_C1_F31+59)             //59, ��һ������A��B��C�����������й�����ʾֵ������޹�1��2����ʾֵ
#define PIM_LAST_ZXYGZ     (PIM_C1_F32+59)             //11, �ϴγ���ʱ��+����+�����й��ܵ���ʾֵ
#define PIM_ZXYGZ_STOP     (PIM_LAST_ZXYGZ+11)         //2,  ����߳���ʱ��.
#define PIM_TIMESEG_97     (PIM_ZXYGZ_STOP+2)          //84, 97���ķ���ʱ����ϢC33F(42),C34F(42)
#define PIM_C1_F39         (PIM_TIMESEG_97+84)         //76, ��һ�������С��޹��������������ʱ��
#define PIM_C1_F40         (PIM_C1_F39+76)             //76, ��һ�·����С��޹��������������ʱ��
#define PIM_C1_F33_MONTH2  (PIM_C1_F40+76)             //91, �ϴε������³�ֵ
#define PIM_C1_F34_MONTH2  (PIM_C1_F33_MONTH2+91)      //91, �ϴε������³�ֵ
#define PIM_C1_F162        (PIM_C1_F34_MONTH2+91)      //12, ���ܱ�����ʱ��
#define PIM_C1_F177        (PIM_C1_F162+12)            //31, ��ǰ����й�����ʾֵ
#define PIM_C1_F178        (PIM_C1_F177+31)            //31, ��1����������й�����ʾֵ
#define PIM_C1_F246        (PIM_C1_F178+31)            //103, ��ǰ�����¼
/////ɽ��ʹ��  begin
#define PIM_REC_DAY_PARAM  (PIM_C1_F246+103)           //6,�洢3��������
#define PIM_I_BREAK_COUNT  (PIM_REC_DAY_PARAM+6)       //9,a��b��c���������
#define PIM_V_LOST_COUNT   (PIM_I_BREAK_COUNT+9)       //6=3+3,ʧѹ�ܴ�����ȫʧѹ�ܴ���
#define PIM_P_SUM_DAY      (PIM_V_LOST_COUNT+6)        //4,�չ����ܺ�
#define PIM_P_COUNT_DAY    (PIM_P_SUM_DAY+4)           //2,�չ��ʳ�������
#define PIM_P_AVERAGE_STOP (PIM_P_COUNT_DAY+2)         //4,�ж�ͣ�ߵ�ƽ������

/////ɽ��ʹ��  end
#define PIM_C1_F33_DAY2     (PIM_P_AVERAGE_STOP+4)       //91, �ϴε������ճ�ֵ
#define PIM_C1_F34_DAY2     (PIM_C1_F33_DAY2+91)         //91, �ϴε������ճ�ֵ



//�ն�������   FILEID_485_DH
#define PIM_C2_MONTH      (INT8U)2                  //�������ݱ���������1��=31�죬2��=62��......
#define PIM_C2_F1         0                                         //91*31 = 2821
#define PIM_C2_F2      (PIM_C2_F1+2821*PIM_C2_MONTH)              //91*31 = 2821
#define PIM_C2_F32     (PIM_C2_F2+2821*PIM_C2_MONTH)              //C2_F32_LEN*31 = 1767
#define PIM_C2_F25     (PIM_C2_F32+C2_F32_LEN*31*PIM_C2_MONTH)    //32*31 =  992    ���ܼ���������й�����ͳ��
#define PIM_C2_F26     (PIM_C2_F25+992*PIM_C2_MONTH)              //24*31 =  744    ���ܼ������������ͳ��
#if defined (__QGDW_376_2013_PROTOCOL__) || defined (__QGDW2013_VRATE__)
#define PIM_C2_F27     (PIM_C2_F26+744*PIM_C2_MONTH)              //93*31 = 2883    �յ�ѹͳ��
#define PIM_C2_F28     (PIM_C2_F27+2883*PIM_C2_MONTH)             //16*31 =  496    �ղ�ƽ��ͳ��
#else
#define PIM_C2_F27     (PIM_C2_F26+744*PIM_C2_MONTH)              //66*31 = 2046    �յ�ѹͳ��
#define PIM_C2_F28     (PIM_C2_F27+2046*PIM_C2_MONTH)             //16*31 =  496    �ղ�ƽ��ͳ��
#endif
#define PIM_C2_F29     (PIM_C2_F28+496*PIM_C2_MONTH)              //38*31 = 1178    �յ���ͳ��
#define PIM_C2_F30     (PIM_C2_F29+1178*PIM_C2_MONTH)             //4 *31 =  124    �����ڹ���ͳ��
#define PIM_C2_F43     (PIM_C2_F30+124*PIM_C2_MONTH)              //6*31  =  186    �չ�������ͳ��
#define PIM_C2_F3      (PIM_C2_F43+186*PIM_C2_MONTH)              //76*31 = 2356    �ն����������������ʱ��(����)
#define PIM_C2_F4      (PIM_C2_F3+2356*PIM_C2_MONTH)              //76*31 = 2356    �ն����������������ʱ��(����)
#define PIM_C2_F153    (PIM_C2_F4+2356*PIM_C2_MONTH)              //59*31 = 1829    �ն���������
#define PIM_C2_F209    (PIM_C2_F153+1829*PIM_C2_MONTH)            //C2_F209_LEN*31 =  496    �ն�����ܱ�Զ�̿���״̬����Ϣ
#define PIM_C2_F31     (PIM_C2_F209+C2_F209_LEN*31*PIM_C2_MONTH)  //10*31 =  310    �ո�����ͳ��
#ifdef __REC_DH_485_EXT__
#define PIM_C2_F300   (PIM_C2_F31  + 310*PIM_C2_MONTH)            //29*31 =  899      �ն�����ܱ�Ƿѹ����
#define PIM_C2_F301   (PIM_C2_F300 + 899*PIM_C2_MONTH)            //168*31 =  5208    �ն�����ܱ�ʧ������
#define PIM_C2_F302   (PIM_C2_F301 + 5208*PIM_C2_MONTH)           //168*31 =  5208    �ն�����ܱ��������
#define PIM_C2_F303   (PIM_C2_F302 + 5208*PIM_C2_MONTH)           //129*31 =  3999    �ն�����ܱ�������
#define PIM_C2_F304   (PIM_C2_F303 + 3999*PIM_C2_MONTH)           //168*31 =  5208    �ն�����ܱ��������
#define PIM_C2_F305   (PIM_C2_F304 + 5208*PIM_C2_MONTH)           //20*31 =  620    �ն�����ܱ����
#define PIM_C2_F211   (PIM_C2_F305 + 5828*PIM_C2_MONTH)           //9*31 =  279    �ն��������ѹ��������ƽ���95%����ֵ
#define PIM_C2_F210   (PIM_C2_F211 + 279*PIM_C2_MONTH)     //41*31 =  1271    �ն��ṻ�õ���Ϣ
#else
#define PIM_C2_F211   (PIM_C2_F31  + 310*PIM_C2_MONTH)           //9*31 =  279    �ն��������ѹ��������ƽ���95%����ֵ
#define PIM_C2_F210   (PIM_C2_F211 + 279*PIM_C2_MONTH)     //41*31 =  1271    �ն��ṻ�õ���Ϣ
#endif
#define PIM_C2_F246   (PIM_C2_F210 + 1271*PIM_C2_MONTH)    //103*31 = 3193   �ն�������¼
#ifdef __GET_DH_AMOUNT_FROM_REAL__
#define PIM_C2_F1_REAL (PIM_C2_F246 + 3193*PIM_C2_MONTH)  // 91*31 = 2821
#define PIM_C2_F2_REAL (PIM_C2_F1_REAL + 2821*PIM_C2_MONTH) ////91*31 = 2821
#endif
#define PIM_C2_F240	  (PIM_C2_F246 + 465*PIM_C2_MONTH)			//15*31 = 465 �����ز���Ŀ��չ �ն����õ���


//������˲ʱ����������(��C1F25�еõ�,������С���Ϊ15����,ÿ��96�㣬����7��)
//FILEID_485_CURVE
#define PIM_CURVE_C1F25  0              //67*96*7 = 45024



//���������������޹�����ʾֵ����       
//FILEID_485_CURVE_YWG                 53�ֽ�*96����*10��=50880  zy 2011.3.24 ����
// �����ʽ������ʱ��/5 ZXYGZ/4   ZXWGZ/4  FXYGZ/4  FXWGZ/4�� = 21�ֽ�


//�����㳭���ն��������ļ�
//FILEID_485_RECDAY
#define PIM_C2_F9        0              //91*31 = 2821
#define PIM_C2_F10    2821              //91*31 = 2821
#define PIM_C2_F11    5642              //76*31 = 2356
#define PIM_C2_F12    7998              //76*31 = 2356

//�������¶��������ļ�
//FILEID_485_MH
#define PIM_C2_F17         0              //91*12 = 1092
#define PIM_C2_F18      1092              //91*12 = 1092
#define PIM_C2_F19      2184              //76*12 =  912
#define PIM_C2_F20      3096              //76*12 =  912
#define PIM_C2_F35      4008              //93*12 =  1116,792   �µ�ѹͳ��
#if defined (__QGDW_376_2013_PROTOCOL__) || defined (__QGDW2013_VRATE__)
#define PIM_C2_F36      5124              //16*12 =  192   �²�ƽ��ͳ��
#else
#define PIM_C2_F36      4800              //16*12 =  192   �²�ƽ��ͳ��
#endif
#define PIM_C2_F37      (PIM_C2_F36+192)              //38*12 =  456   �µ���ͳ��
#define PIM_C2_F38      (PIM_C2_F37+456)              //4*12  =   48   �����ڹ���ͳ��
#define PIM_C2_F44      (PIM_C2_F38+48)              //6*12  =   72   �¹�����������ͳ��
#define PIM_C2_F33      (PIM_C2_F44+72)              //32*12 =  384  ���ܼ���������й�����ͳ��
#define PIM_C2_F34      (PIM_C2_F33+384)              //24*12 =  288  ���ܼ������������ͳ��
#define PIM_C2_F157     (PIM_C2_F34+288)              //59*12 =  708  �¶���������ʾֵ
#if defined (__QGDW_376_2013_PROTOCOL__)
    #define C2_F213_LEN     23            //�¶�����ܱ��ز���������ʱ�䳤��
    #define C2_F214_LEN     29            //�¶�����ܱ�����޸Ĵ�����ʱ�䳤��
#else
    #define C2_F213_LEN     19            //�¶�����ܱ��ز���������ʱ�䳤��
    #define C2_F214_LEN     19            //�¶�����ܱ�����޸Ĵ�����ʱ�䳤��
#endif
#define PIM_C2_F213     (PIM_C2_F157+708)              //C2_F213_LEN*12 =276��228  �¶�����ܱ��ز���������ʱ��

#define PIM_C2_F214     (PIM_C2_F213+C2_F213_LEN*12)              //19*12 =  228  �¶�����ܱ�����޸Ĵ�����ʱ��
#define PIM_C2_F39      (PIM_C2_F214+C2_F214_LEN*12)              //12*12 =  144  �¸�����ͳ��
#ifdef __REC_DH_485_EXT__
#define PIM_C2_F308   (PIM_C2_F39  + 144)     //29*12 =  348      �¶�����ܱ�Ƿѹ����
#define PIM_C2_F309   (PIM_C2_F308 + 348)     //168*12 =  2016    �¶�����ܱ�ʧ������
#define PIM_C2_F310   (PIM_C2_F309 + 2016)    //168*12 =  2016    �¶�����ܱ��������
#define PIM_C2_F311   (PIM_C2_F310 + 2016)    //129*12 =  1548    �¶�����ܱ�������
#define PIM_C2_F312   (PIM_C2_F311 + 1548)    //168*12 =  2016    �¶�����ܱ��������
#define PIM_C2_F313   (PIM_C2_F312 + 2016)    //20*12 =  240    �¶�����ܱ����
#define PIM_C2_F212   (PIM_C2_F313 + 240)    //9*12 =  108    �¶��������ѹ��������ƽ���95%����ֵ
#define PIM_C2_F215   (PIM_C2_F212 + 108)     //41*12 = 492     �¶��ṻ�õ���Ϣ
#else
#define PIM_C2_F212   (PIM_C2_F39  + 144)    //9*12 =  108    �¶��������ѹ��������ƽ���95%����ֵ
#define PIM_C2_F215   (PIM_C2_F212 + 108)     //41*12 = 492     �¶��ṻ�õ���Ϣ
//#define PIM_C2_F216   (PIM_C2_F215 + 492)     //51*12 = 612     �¶�����ܱ������Ϣ
#endif


//�ն������ļ�FILEID_485_ERTU
//F49 �ն����ն��չ���ʱ�䣬�ո�λ�ۼƴ���
//F50 �ն����ն��տ���ͳ��
//F53 ��ͨ������
#define PIM_ERTU_C2_F495053          0              //15*31 =  465       //��-��-��
//F51 �¶����¹���ʱ�䣬�¸�λ�ۼƴ���
//F52 �¶����ն��¿��ƴ���
//F54 �ն�����վ��ͨ������                               6
#define PIM_ERTU_C2_F51F52F54        (PIM_ERTU_C2_F495053 + 465)              //15*12 =  180
//�Լ���չ���¶��������ݣ�����15���Ӳɼ�һ��
#define PIM_ERTU_TEMPERATURE_CURVE   (PIM_ERTU_C2_F51F52F54 + 180)            //7�ֽ�*3��*96�� = 2016

//�������� 2014.08.04 by rxp
#define PIM_ERTU_XLOST               (PIM_ERTU_TEMPERATURE_CURVE + 2016)      //30��*15[(������2 + ����1)*4(��,A,B,C)+ʱ��3]=450



//ͳ������ , ���ղ�����ֿ鱣�棬ÿ�����ݰ�������
//FILEID_STAT

#ifdef __STAT_LOADRATE__
#define  PIM_STAT_C2_TRANSIT            0   //300*2=600  ˲ʱ������ͳ��
#define  PIM_STAT_C2_ZDXL             600   //24*2=48    �����������ͳ�� (����)
#define  PIM_STAT_C2_ZDXL_FL_ZX       648   //76*2=152   ���������������ͳ�ƣ����ʣ�
#define  PIM_STAT_C2_ZDXL_FL_FX       800   //76*2=152   ���������������ͳ�ƣ����ʣ�
#define  PIM_STAT_C2_LOADRATE         952   //12*2=24    ����������ͳ��

#define  LEN_STAT_C2_BLOCK            (944+24)

#else

#define  PIM_STAT_C2_TRANSIT            0   //296*2=592  ˲ʱ������ͳ��
#define  PIM_STAT_C2_ZDXL             592   //24*2=48    �����������ͳ�� (����)
#define  PIM_STAT_C2_ZDXL_FL_ZX       640   //76*2=152   ���������������ͳ�ƣ����ʣ�
#define  PIM_STAT_C2_ZDXL_FL_FX       792   //76*2=152   ���������������ͳ�ƣ����ʣ�
#define  PIM_STAT_C2_LOADRATE         944   //12*2=24    ����������ͳ��

#define  LEN_STAT_C2_BLOCK            944
#endif

//������F33 �ն�̨�����г������в�������
typedef union{
    struct{
    INT8U    port;          // �ն�ͨ�Ŷ˿ں�	BIN	1	�������õ�
    INT8U    run_ctrl[2];   // ̨�����г������п�����	BS16	2
    INT8U    rec_days[4];   // ������-����	BS32	4
    INT8U    rec_time[2];   // ������-ʱ��	���ݸ�ʽ19	2
    INT8U    cycle;         // ������         BIN     1
    INT8U    cast_time[3];  // �Ե��㲥Уʱ��ʱʱ��	���ݸ�ʽ18	3
    INT8U    seg_count;       // ����ʱ������
    INT8U    rec_timeseg[96]; // ����ʱ����Ϣ   ʱ-�� 
    };
    struct{
      INT8U    tmp[13];            //����
      INT8U    seg_count;          // ����ʱ������
      INT8U    rec_timeseg[24][4]; //24��ʱ��
    }seg;
}JCSET_F33;

#ifdef __PROVICE_HUBEI__
typedef struct{
    INT8U    rec_days[4];        // ������-����	BS32	4
    INT8U    seg_count;          // ����ʱ������
    INT8U    rec_timeseg[24][4]; //24��ʱ��
}JCSET_F801;

typedef union{
    INT8U value[4];  //��ɶ�ã���
    struct{
        INT8U    year;
        INT8U    month;
        INT8U    day;
        INT8U    hour;
        INT8U    p[12];
    };
}CURVE_POWER;

#endif
typedef union{
    INT8U value[4];
    struct{
        INT8U    year;
        INT8U    month;
        INT8U    day;
        INT8U    hour;
        INT8U    v[6];
        INT8U    i[9];
    };
}CURVE_V_I;
//������ F33��  DL698_41_0928 �汾����
typedef struct{
    INT8U    port;            // �ն�ͨ�Ŷ˿ں�	BIN	1	�������õ�
    INT8U    run_ctrl[2];     // ̨�����г������п�����	BS16	2
    INT8U    rec_timeseg[12]; // ����ʱ����Ϣ  15���Ӽ������96��λ��ǽ�ֹ������1==��ֹ
    INT8U    rec_days[4];     // ������-����	BS32	4
    INT8U    rec_time[2];     // ������-ʱ��	���ݸ�ʽ19	2
    INT8U    cycle;           // ������         BIN     1
    INT8U    cast_time[3];    // �Ե��㲥Уʱ��ʱʱ��	���ݸ�ʽ18	3   
}JCSET_F33_DL698_41_0928;


//����: �ն�֧�������������õĳ���˿�: 1--�ز���   2--����485��   3--����485��(�������Ϊ����,�򲻿���)
typedef struct{
    JCSET_F33  port_set[4];
}JCSET_F33_ALL;


//���ݸ�ʽ14:  XXXXXX.XXXX
typedef struct{
     INT8U   value[5];
}DATAFMT_14;


//���ݸ�ʽ11:  XXXXXX.XX
typedef struct{
     INT8U   value[4];
}DATAFMT_11;


//���ݸ�ʽ15
typedef union{
    INT8U   value[5];
    struct{
       INT8U   bcd_minute;
       INT8U   bcd_hour;
       INT8U   bcd_day;
       INT8U   bcd_month;
       INT8U   bcd_year;
    };
}DATAFMT_15;

//�ն˳���ʱ��
typedef  DATAFMT_15 REC_DATETIME;


typedef union{
    INT8U   value[10];
    struct{
        INT8U flag;              //0,��ʱ���أ�0���رգ�1������
        INT8U cycle;             //1,��ʱ���ڣ�ÿ��һ�������Զ���ʼһ�������ն˶�ʱ��������λ���졣
        INT8U bcd_begin_minute;  //2,��ʱ��ʼʱ��
        INT8U bcd_begin_hour;    //3,��ʱ��ʼʱ��
        INT8U bcd_end_minute;    //4,��ʱ����ʱ��
        INT8U bcd_end_hour;      //5,��ʱ����ʱ��
        INT8U interval;          //6,��ʱ��̼�� ��λ������
        INT8U max_count;         //7,�����ʱ����
        INT8U error_threshold;   //8,ʱ�������ֵ ��λ����
        INT8U power_max_delay;   //9,�ϵ��¼����������ʱʱ�� ��λ������
    };
}SET_F297; //__PRECISE_TIME__

typedef union{
    INT8U   value[8];
    struct{
        INT8U flag;              //0,��ʱ���أ�0���رգ�1������ָ������ַ�Ĺ㲥��ʱ���2������ָ���ɼ����Ĺ㲥��ʱ���3������ȫ�ն˵Ĺ㲥��ʱ�����ѡ��ȫ�ն˵Ĺ㲥��ʱ����ʱ������ʱ����������������������Ƿ��ʱ��
        INT8U cycle;             //1,��ʱ���ڣ�ÿ��һ�������Զ���ʼһ�������ն˶�ʱ��������λ���졣
        INT8U bcd_begin_minute;  //2,��ʱ��ʼʱ��
        INT8U bcd_begin_hour;    //3,��ʱ��ʼʱ��
        INT8U bcd_end_minute;    //4,��ʱ����ʱ��
        INT8U bcd_end_hour;      //5,��ʱ����ʱ��
        INT8U max_count;         //6,�����ʱ����
        INT8U error_threshold;   //7,ʱ�������ֵ ��λ����
    };
}SET_F298; //__PRECISE_TIME__


#define BATCH_TRANS_METER_TASK_FLAG_NO_EXEC             0     //������,δִ��
#define BATCH_TRANS_METER_TASK_FLAG_CANT_EXEC           1     //����ִ��
#define BATCH_TRANS_METER_TASK_FLAG_EXEC_NO_RESP        2     //��ִ�У�δ��Ӧ
#define BATCH_TRANS_METER_TASK_FLAG_FINISH              3     //ִ�����
#define BATCH_TRANS_METER_TASK_FLAG_BEFORE_EXCEPTION    4     //ǰ������ظ��쳣Ӧ��
#define BATCH_TRANS_METER_TASK_FLAG_RESP_LONG           5     //ִ�����,�ظ�����̫��
#define BATCH_TRANS_METER_TASK_FLAG_TIME_OUT            6     //ִ�����,��ʱ

typedef union{
    INT8U value;
    struct{
        INT8U report:1;
        INT8U wait_read:1;
        INT8U task_end:1;
        INT8U deny:1;
        INT8U tmp:4;
    };
}BATCH_TRANSPARENT_METER_TASK_FALG;

typedef union{
    INT8U value[4];
    struct{
        INT8U task_id[2];
        INT8U task_level;   //0��������1�������ն��᳭����2�������ն��᳭��
        BATCH_TRANSPARENT_METER_TASK_FALG ctrl_flag;  //D0-�ϱ���־λ  D1-�����ȴ���־λ  D2~D7����
        #ifdef __BATCH_TRANSPARENT_METER_TASK_TMP1__
        INT8U vaild_minute[2];  // 0-��������Чʱ��  ����������ִ����Чʱ������λΪ����
        #else
        INT8U vaild_minute;  // 0-��������Чʱ��  ����������ִ����Чʱ������λΪ����
        #endif
        INT8U z_count;
        INT8U count;
        INT8U begin_time[6];
        INT8U end_time[6];
        INT8U flag;         //0xAA ������
    };
}BATCH_TRANSPARENT_METER_TASK_HEADER;       //__BATCH_TRANSPARENT_METER_TASK__

typedef union{
    INT8U value[4];
    struct{
        INT8U flag;  //FE:������δִ��  FC������ִ��  F8��ִ�����
        INT8U frame_seq;
        INT8U req_frame_len;
        INT8U req_frame[BATCH_TRANSPARENT_METER_TASK_FRAME_LEN];
        INT8U resp_frame_len;
        INT8U resp_date[6];
        INT8U resp_frame[BATCH_TRANSPARENT_METER_TASK_FRAME_LEN];
    };
}BATCH_TRANSPARENT_METER_TASK;       //__BATCH_TRANSPARENT_METER_TASK__

//���ݸ�ʽ20
typedef union{
    INT8U value[3];
    struct{
        INT8U bcd_day;
        INT8U bcd_month;
        INT8U bcd_year;
    };
}DATAFMT_20;
//�����ʱ�䣬�ݶ�����ʹ��..
typedef DATAFMT_20  METER_DAYHOLD_TIME;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//          һ������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////


typedef union{
   INT8U  value;
   struct{
      INT8U   act_A:1;
      INT8U   act_B:1;
      INT8U   act_C:1;
      INT8U   reserved1:1;
      INT8U   rec_A:1;
      INT8U   rec_B:1;
      INT8U   rec_C:1;
      INT8U   reserved2:1;
   };
}DC1_PLC_PHASE;

typedef union{
   INT8U  value;
   struct{
      INT8U   resp_q:4;
      INT8U   send_q:4;
   };
}DC1_PLC_Q;

//F170�����г���������Ϣ
typedef union{
    INT8U  value[18];
    struct{
       INT8U           port;            //�����ն�ͨ�Ŷ˿ں�
       INT8U           depth;           //�м�·�ɼ���
       DC1_PLC_PHASE   phase;           //�ز�����ͨ����λ
       DC1_PLC_Q           plc_q;           //�ز��ź�Ʒ��
       INT8U           rec_flag;        //���һ�γ���ɹ�/ʧ�ܱ�־
       DATAFMT_01      last_rec_date;   //���һ�γ���ɹ�ʱ��	���ݸ�ʽ01      6
       DATAFMT_01      last_fail_date;  //���һ�γ���ʧ��ʱ��	���ݸ�ʽ01	6
       INT8U           fail_count;      //�������ʧ���ۼƴ���
    };
}DC1_F170;

//5.12.2.4.10	F11���ն˼��г���״̬��Ϣ
//�������ݿ����n  BIN 1
//    �ն�ͨ�Ŷ˿ں�      	BIN	1
//    Ҫ���������	        BIN	2
//    ��ǰ������״̬��־	BS8	1
//    ����ɹ�����	        BIN	2
//    ���ص��ɹ�����	        BIN	1
//    ����ʼʱ��	    ���ݸ�ʽ1	6
//    �������ʱ��	    ���ݸ�ʽ1	6

//��ǰ������״̬��־
typedef union{
   INT8U  value;
   struct{
      INT8U    recording:1;    //��"1"���ڳ�����"0" δ����
      INT8U    finish:1;       //��"1"ʱ������ɣ���"0"ʱ����δ���
      INT8U    reserved:6;
   };
}LLVC_CURREC_FLAG;

//���г���״̬
typedef  struct{
     INT8U             port;
     INT8U             total_count[2];
     LLVC_CURREC_FLAG  currec_flag;
     INT8U             read_count[2];
     INT8U             read_vip_count;
     DATAFMT_01        begin_datetime;
     DATAFMT_01        end_datetime;
}LLVC_REC_STATE ;

//һ�� F11
typedef union{
  INT8U  value[39];
  struct{
     INT8U             port_count;    //��ȡ2
     LLVC_REC_STATE    rec_state[2];  //���г���״̬,2���˿�
  };
}DC1_F11;

typedef union{
    INT8U  value[7];
    struct{
        INT8U Item[4];      //�¼�����
        INT8U Level;        //�¼�����
        INT8U Data[2];       //CRC����
    };
}METER_EVENT_F106;     //#ifdef __METER_EVENT_GRADE__

typedef union{
    INT16U  value;
    struct{
        INT16U offset:15;      //�¼�����
        INT16U is_report:1;    //�¼�����
    };
}METER_EVENT_REC_HEAD;     //#ifdef __METER_EVENT_GRADE__

//F50:��ǰʣ�������ʣ����
typedef union{
   INT8U  value[8];
   struct{
     INT8U   left_amount[4];
     INT8U   left_fee[4];
   };
}DC1_F50;
#if !defined (__QGDW_376_2013_PROTOCOL__)
//F177: ���յ�ѹ�ϸ���ͳ��
typedef union{
   INT8U value[32];
   struct{
      INT8U  read_date[5];
      INT8U  mon_time[3];   //��ѹ���ʱ��	���ݸ�ʽ10	��	3
      INT8U  ok_rate[3];    //��ѹ�ϸ���	���ݸ�ʽ28	%	3
      INT8U  over_rate[3];  //��ѹ������	���ݸ�ʽ28	%	3
      INT8U  over_time[3];  //��ѹ������ʱ��	���ݸ�ʽ10	��	3
      INT8U  low_time[3];   //��ѹ������ʱ��	���ݸ�ʽ10	��	3
      INT8U  max_v[2];      //��ߵ�ѹ	���ݸ�ʽ5	V	2
      INT8U  max_v_time[4]; //��ߵ�ѹ����ʱ��	���ݸ�ʽ17		4
      INT8U  min_v[2];      //��͵�ѹ	���ݸ�ʽ5	V	2
      INT8U  min_v_time[4]; //��͵�ѹ����ʱ��	���ݸ�ʽ17		4
   };
   struct{
      INT8U  read_date[5];
      INT8U  meter_data[27];
   }d;
}DC1_F177;
#endif
//�й�����ʾֵ���ܣ�����1~M)
typedef union{
    INT8U  value[31];
    struct{
       INT8U  read_date[5];
       INT8U  fl_count;
       INT8U  ygz[5];
       INT8U  yg1[5];
       INT8U  yg2[5];
       INT8U  yg3[5];
       INT8U  yg4[5];
    };
    struct{
       INT8U  read_date[5];
       INT8U  fl_count;
       INT8U  yg_data[5][5];
    }d;
#if  defined (__QGDW_376_2013_PROTOCOL__)
}DC1_F129,DC1_F131,DC1_F137,DC1_F139,DC1_F177,DC1_F178;
#else
}DC1_F129,DC1_F131,DC1_F137,DC1_F139,DC1_F178;
#endif

//�޹�����ʾֵ
typedef union{
    INT8U  value[26];
    struct{
       INT8U  read_date[5];
       INT8U  fl_count;
       INT8U  wgz[4];
       INT8U  wg1[4];
       INT8U  wg2[4];
       INT8U  wg3[4];
       INT8U  wg4[4];
    };
}DC1_F130,DC1_F132,DC1_F133,DC1_F134,DC1_F135,DC1_F136,DC1_F138,DC1_F140,DC1_F141,DC1_F142,DC1_F143,DC1_F144;

//һ�����ݵ���F41,F42,F43,F44,F45,F46,F47,F48
typedef union{
    INT8U value[21];
    struct{
        INT8U   fl_number;              //������M
        INT8U   read_data[20];
    };
    struct{                             //����
        INT8U   fl_number;              //������M
        INT8U   zx_ygz[5][4];           //�����й�������    �ܣ�����1~M     ���ݸ�ʽA.13    5*4
    }d;
}DC1_F41, DC1_F42, DC1_F43, DC1_F44, DC1_F45, DC1_F46, DC1_F47, DC1_F48;


//�ز���ǰ�����й�������ʾֵ����
typedef union{
   INT8U  value[31];
   struct{
       REC_DATETIME   rec_datetime;
       INT8U          fl_count;
       DATAFMT_14     zxygz;
       DATAFMT_14     zxyg_fl[4];
   };
}PLCDATA_CUR_ZXYG;



//Сʱ���������ʾֵ�������
typedef union{
    INT8U  value[6];
    struct{
       INT8U  td_h[2];  //Сʱ������ʱ��
       INT8U  amount[4];
    };
}DC1_HOURHOLDAMOUNT;

//�๦�ܵ��ʧѹʵ�������Ա�̵ȸ澯�洢���ݽṹ
typedef union{
    INT8U  value[14];
    struct{
        INT8U  rec_date[5];  //��-��-��-ʱ-�� BCD��
        INT8U  lost_v_z[3];
        //INT8U  lost_v_a[3];
        //INT8U  lost_v_b[3];
        //INT8U  lost_v_c[3];
        INT8U  lost_i_z[3];
        //INT8U  lost_i_a[3];
        //INT8U  lost_i_b[3];
        //INT8U  lost_i_c[3];
        INT8U  program_count[3];
        //INT8U  program_time[5];
    };
}MFM_COMMON_ALARM;


typedef union{
    INT8U  value[22];
    struct{
        INT8U node_no[6];  
        INT8U event_count[3];
        INT8U first_begin_time[6];
        INT8U first_end_time[6];
        INT8U flag;
    };
}EVENT_OPEN_DOOR;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//          ��������
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

//���ࣺF161���ն��������й�����ʾֵ���ܡ�����1~M��
typedef union{
    INT8U value[20];
     struct{
     REC_DATETIME   rec_datetime;
     INT8U          fl_count;
     DATAFMT_14     zxygz;
     DATAFMT_14     zxyg_fl[4];
       #ifdef  __COUNTRY_SOUTH_AFRICA__
       INT8U  meter_event[2];
       INT8U  wg_dl[4][4];
       #endif
     };
     struct{
       REC_DATETIME   rec_datetime;
       INT8U          fl_count;
       INT8U  yg_data[5][5];
       #ifdef  __COUNTRY_SOUTH_AFRICA__
       INT8U  meter_event[2];
       INT8U  wg_dl[4][4];
       #endif
     }d;
     struct{
       REC_DATETIME   rec_datetime;
       INT8U          fl_count;
       INT8U          data[5][4]; 
     }e;
}DC2_F161,DC2_F163;

//���ࣺF177���¶��������й�����ʾֵ���ܡ�����1~M��
typedef  DC2_F161  DC2_F177;

//���ࣺF179: �¶��ᷴ���й�����ʾֵ���ܡ�����1~M��
typedef  DC2_F161  DC2_F179;


typedef union{
    INT8U value[20];
    struct{
        REC_DATETIME   rec_datetime;
        INT8U          fl_count;
        INT8U          zxyg[20];
        INT8U          fxyg[20];
        INT8U          zxwg[20];
        INT8U          fxwg[20];
    };
    struct{
        REC_DATETIME   rec_datetime;
        INT8U          fl_count;
        INT8U          data[4][20];
    }d;
}BRAZIL_DAYHOLD;

//����  F162: �ն��������޹�
typedef struct{
     REC_DATETIME   rec_datetime;
     INT8U          fl_count;
     INT8U          z[4];
     INT8U          fl[4][4];
}DC2_F162,DC2_F164,DC2_F165,DC2_F166,DC2_F167,DC2_F168;

//���ࣺF1�����������Ҫ

typedef union{
    INT8U value[20];
    struct{
      REC_DATETIME   rec_datetime;
      INT8U          fl_count;
      DATAFMT_14     zxygz;
      DATAFMT_11     zxwgz;
      DATAFMT_11     wg1;
      DATAFMT_11     wg4;
    };
    struct{
      REC_DATETIME   rec_datetime;
      INT8U          fl_count;
      DATAFMT_14     zxygz[5];
      DATAFMT_11     zxwgz[5];
      DATAFMT_11     wg1[5];
      DATAFMT_11     wg4[5];
    }f;
}DC2_F1;

//F220: ���õ���Ϣ
typedef union{
   INT8U value[17];
   struct{
      INT8U  read_date[5];
      INT8U  buy_count[2];   //�������
      INT8U  left_buy[5];    //ʣ����
      INT8U  total_buy_fee[5];  //�ۼƹ�����
   };
   struct{
      INT8U  read_date[5];
      INT8U  meter_data[12];
   }d;
}DC2_F220;


//1������F25
typedef union {
      INT8U  value[67];
      struct{
      unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��    
      unsigned char p_yg[12];      //��ǰ��,A,B,C���й�����  ���ݸ�ʽ09  4*3
      unsigned char p_wg[12];      //��ǰ��,A,B,C���޹�����  ���ݸ�ʽ09  4*3
      unsigned char Q[8];          //��ǰ��,A,B,C�๦������  ���ݸ�ʽ05  4*2
      unsigned char Voltage[6];    //��ǰA,B,C���ѹ  ���ݸ�ʽ07   3*2
         unsigned char I[12];          //��ǰA,B,C,N�����  ���ݸ�ʽ25   4*3
         unsigned char p_uv[12];      //��ǰ��,A,B,C�����ڹ���  ���ݸ�ʽ09  4*3
     };
  }C1F25;

typedef struct{
    C1F25 cur_F25;
    C1F49 cur_F49;
    #ifdef __GB_645_YUNNAN__
    DC2_F96 c2F96;
    #endif
}C1SPORTCURVE;

//1������F26
typedef  union {
     INT8U  value[57];
     struct
     {
        INT8U read_date[5];     //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
      #if defined(__QGDW_376_2013_PROTOCOL__)
        INT8U V_BreakCount[12];  //��,A,B,C�������  4*3
      #else
        INT8U V_BreakCount[8];  //��,A,B,C�������  4*2
      #endif
        INT8U V_BreakTime[12];  //��,A,B,C�����ۼ�ʱ�� 4*3
        INT8U V_BreakLastStart[16];  //��,A,B,C���һ�ζ�����ʼʱ�� 4*4
        INT8U V_BreakLastStop[16];   //��,A,B,C���һ�ζ������ʱ�� 4*4
     };
   }C1F26;




   //1������F27
typedef union {
     INT8U  value[60];
     struct{
      unsigned char read_date[5];     //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��    
      unsigned char meter_date[6];    //���ܱ�����ʱ��
       unsigned char battery_time[4];       //��ع���ʱ��
       unsigned char program_count[3];      //��̴���
       unsigned char last_program_date[6];       //���һ�α��ʱ��
       unsigned char zero_init_count[3];         //����������
       unsigned char last_zero_init_date[6];     //���һ������ʱ��
       unsigned char xlclear_count[3];           //��������������
       unsigned char last_xlclear_date[6];       //���һ�������������ʱ��
       unsigned char event_init_count[3];        //�¼��������
       unsigned char last_event_init_date[6];    //���һ���¼�����ʱ��
       unsigned char adj_time_count[3];         //Уʱ����
       unsigned char last_adj_time_date[6];     //���һ��Уʱʱ��
     };
   }C1F27;


//һ������F28
typedef union{
     INT8U   value[33];
     struct{
        unsigned char read_date[5];     //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char flag_chg[14];     //�������״̬��λ��1-7
        unsigned char status[14];       //�������״̬��1-7
     };
}C1F28;

//1������F31,F32
typedef union{
     INT8U   value[59];
     struct{
          INT8U read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
          INT8U zxyg_A[5];     //A�������й�����ʾֵ
          INT8U fxyg_A[5];     //A�෴���й�����ʾֵ
          INT8U zhwg1_A[4];    //A������޹�1����ʾֵ
          INT8U zhwg2_A[4];    //A������޹�2����ʾֵ
          INT8U zxyg_B[5];     //B�������й�����ʾֵ
          INT8U fxyg_B[5];     //B�෴���й�����ʾֵ
          INT8U zhwg1_B[4];    //B������޹�1����ʾֵ
          INT8U zhwg2_B[4];    //B������޹�2����ʾֵ
          INT8U zxyg_C[5];     //C�������й�����ʾֵ
          INT8U fxyg_C[5];     //C�෴���й�����ʾֵ
          INT8U zhwg1_C[4];    //C������޹�1����ʾֵ
          INT8U zhwg2_C[4];    //C������޹�2����ʾֵ
     }b;
}C1F31,C1F32;

//1������F29,F30
typedef union{
     INT8U   value[13];
     struct{
          INT8U read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
          INT8U copper_loss[5];     //A�������й�����ʾֵ
          INT8U iron_loss[5];     //A�෴���й�����ʾֵ
     }b;
}C1F29,C1F30;

//1������F33,F34,F37,F38
typedef union {
      INT8U  value[91];
      struct{
      unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��    
      unsigned char fl_number;     //������M
      unsigned char zx_ygz[25];    //�����й��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ14    5*5
      unsigned char zx_wgz[20];    //�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      unsigned char zx_wg1[20];    //����1�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      unsigned char zx_wg4[20];    //����4�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4   
      };
      struct{
        unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;     //������M
        unsigned char zx_ygz[5][5];    //�����й��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ14    5*5
        unsigned char zx_wgz[5][4];    //�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg1[5][4];    //����1�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg4[5][4];    //����4�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      }d;
      struct{
        unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;     //������M = 1
        unsigned char zx_ygz[10];    //�����й��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ14    5*5
        unsigned char zx_wgz[8];    //�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg1[8];    //����1�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg4[8];    //����4�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      }d1;
      struct{
        unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;     //������M = 2
        unsigned char zx_ygz[15];    //�����й��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ14    5*5
        unsigned char zx_wgz[12];    //�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg1[12];    //����1�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg4[12];    //����4�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      }d2;
      struct{
        unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;     //������M = 3
        unsigned char zx_ygz[20];    //�����й��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ14    5*5
        unsigned char zx_wgz[16];    //�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg1[16];    //����1�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
        unsigned char zx_wg4[16];    //����4�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      }d3;
      struct{
        unsigned char meter_no[6];
        unsigned char read_date[5];
        unsigned char fl_count;
        unsigned char value[20];
      }f;  //�ز���������ն�������ͷ��Ϣ
}C1F33,C1F34,C1F37,C1F38;


typedef union {
      INT8U  value[86];
      struct{
      unsigned char read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��    
      unsigned char fl_number;     //������M
      unsigned char zx_yg[20];    //�����й��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ14    5*4
      unsigned char zx_wg[20];    //�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      unsigned char fx_yg[20];    //����1�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4
      unsigned char fx_wg[20];    //����4�����޹��ܵ���ʾֵ  �ܣ�����1~M   ���ݸ�ʽ11    5*4   
      };
}ZFYW_FL;


//����ÿ�������ݻ���
typedef union{
   INT8U value[308];
   struct{
       C1F25 cur_F25;
       C1F26 cur_F26;
       C1F27 cur_F27;
       C1F28 cur_F28;
       C1F33 cur_F33;
   };
}ACDATA;

//1������F35,F36
typedef  union{
      INT8U  value[76];
      struct{
      unsigned char read_date[5];      //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��    
      unsigned char fl_number;         //������M
      unsigned char zx_ygxl[15];       //�����й����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
      unsigned char zx_ygxl_date[20];  //�����й��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
      unsigned char zx_wgxl[15];       //�����޹����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
      unsigned char zx_wgxl_date[20];  //�����޹��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
      };
    struct{
        unsigned char read_date[5];      //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;         //������M = 1
        unsigned char zx_ygxl[6];       //�����й����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
        unsigned char zx_ygxl_date[8];  //�����й��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
        unsigned char zx_wgxl[6];       //�����޹����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
        unsigned char zx_wgxl_date[8];  //�����޹��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
      }d1;
    struct{
        unsigned char read_date[5];      //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;         //������M = 2
        unsigned char zx_ygxl[9];       //�����й����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
        unsigned char zx_ygxl_date[12];  //�����й��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
        unsigned char zx_wgxl[9];       //�����޹����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
        unsigned char zx_wgxl_date[12];  //�����޹��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
      }d2;
    struct{
        unsigned char read_date[5];      //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
        unsigned char fl_number;         //������M = 3
        unsigned char zx_ygxl[12];       //�����й����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
        unsigned char zx_ygxl_date[16];  //�����й��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
        unsigned char zx_wgxl[12];       //�����޹����������  �ܣ�����1~M   ���ݸ�ʽ23    5*3
        unsigned char zx_wgxl_date[16];  //�����޹��������������ʱ��  �ܣ�����1~M   ���ݸ�ʽ17    5*4
      }d3;
}C1F35,C1F36;

//1�ࣺF145-F152  ����������ݽṹ
typedef union{
   INT8U  value[7];
   struct{
       INT8U  zdxl[3];     //A.23
       INT8U  xl_time[4];  //��ʱ����
   };
}DC1_ZDXL;
typedef union{
    INT8U  value[41];
    struct{
       INT8U  read_date[5];
       INT8U  fl_count;
       DC1_ZDXL  zdxl[5];   //�ܣ�����1��2��3��4
    };
}DC1_F145,DC1_F146,DC1_F147,DC1_F148,DC1_F149,DC1_F150,DC1_F151,DC1_F152;


//1��F161�����ܱ�Զ�̿���ͨ�ϵ�״̬����¼
#if defined (__QGDW_376_2013_PROTOCOL__)
typedef union{
      INT8U  value[18];
      struct{
         INT8U    read_date[5];        //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    meter_switch_state;  //���ͨ�ϵ�״̬
         INT8U    last_ctrl_on_time[6]; //���1�ο���ͨ��ʱ��
         INT8U    last_ctrl_off_time[6]; //���1�ο��ƶϵ�ʱ��
      }b;
}DC1_F161,DC2_F209;
#else
typedef union{
      INT8U  value[16];
      struct{
         INT8U    read_date[5];        //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    meter_switch_state;  //���ͨ�ϵ�״̬
         INT8U    last_ctrl_on_time[5]; //���1�ο���ͨ��ʱ��
         INT8U    last_ctrl_off_time[5]; //���1�ο��ƶϵ�ʱ��
      }b;
}DC1_F161,DC2_F209;
#endif

#if defined (__QGDW_376_2013_PROTOCOL__)
//1��F162,���ܱ�����ʱ��
typedef union{
      INT8U  value[12];
      struct{
         INT8U    read_date[6];        //�ն˳���ʱ��  ���ݸ�ʽ15 ��-��-ʱ-��-��-��
         INT8U    meter_time[6];       //���ܱ�����ʱ��
      }b;
}DC1_F162;
#endif

//1��F165�����ܱ��ز���������ʱ��
#if defined (__QGDW_376_2013_PROTOCOL__)
typedef union{
      INT8U  value[23];
      struct{
         INT8U    read_date[5];         //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    op_count[3];          //��̿��ز�������  FMT08
         INT8U    last_prog_time[6];    //���1�α�̿��ز���ʱ��
         INT8U    endcap_open_count[3];  //β�Ǵ򿪴���
         INT8U    endcap_open_time[6];    //���1��β�Ǵ�ʱ��
		 #ifdef __PROVICE_SHANXI__
         INT8U    meter_open_count[3];  //���ܱ��Ǵ򿪴���
         INT8U    last_open_time[6];    //���һ�α�Ǵ�ʱ��
		 #endif
      }b;
}DC1_F165,DC2_F213;
#else
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];         //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    op_count[2];          //��̿��ز�������  FMT08
         INT8U    last_prog_time[5];    //���1�α�̿��ز���ʱ��
         INT8U    endcap_open_count[2];  //β�Ǵ򿪴���
         INT8U    endcap_open_time[5];    //���1��β�Ǵ�ʱ��
      }b;
}DC1_F165,DC2_F213;
#endif
//1��F166�����ܱ�����޸Ĵ�����ʱ��
#if defined (__QGDW_376_2013_PROTOCOL__)
typedef union{
      INT8U  value[29];
      struct{
         INT8U    read_date[5];              //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    op_time_count[3];          //ʱ���޸Ĵ���  FMT08
         INT8U    op_before_time[6];         //Уʱǰʱ��
         INT8U    op_after_time[6];          //Уʱ��ʱ��
         INT8U    op_timeseg_count[3];       //ʱ�β����޸Ĵ���
         INT8U    last_op_timeseg_time[6];   //���1��ʱ�β����޸�ʱ��
      }b;
}DC1_F166,DC2_F214;
#else
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];              //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    op_time_count[2];          //ʱ���޸Ĵ���  FMT08
         INT8U    last_op_time[5];           //���1��ʱ���޸�ʱ��
         INT8U    op_timeseg_count[2];       //ʱ�β����޸Ĵ���
         INT8U    last_op_timeseg_time[5];   //���1��ʱ�β����޸�ʱ��
      }b;
}DC1_F166,DC2_F214;
#endif
//1��F167�� ���ܱ��õ���Ϣ
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];               //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    buy_count[2];               //�������      FMT08
         INT8U    left_fee[5];                //ʣ����      FMT14
         INT8U    total_buy_fee[5];           //�ۼƹ�����  FMT14
         INT8U    left_amount[4];             //ʣ�����      FMT11
         INT8U    overuse_amount[4];          //͸֧����      FMT11
         INT8U    total_buy_amount[4];        //�ۼƹ�����    FMT11
         INT8U    canoveruse_amount[4];       //��Ƿ���޵���      FMT11
         INT8U    alarm_amount[4];            //��������    FMT11
         INT8U    err_amount[4];              //���ϵ���      FMT11
      }b;
     struct{
     REC_DATETIME   rec_datetime;
     INT8U meter_data[36];
     };
}DC1_F167,DC2_F210;

//1��F168�� ���ܱ������Ϣ
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];         //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
         INT8U    fl_count;             //���ʸ���
         INT8U    billed_amount[5][5];  //�ѽ��й��ܣ�����1-4����    FMT14
         INT8U    unbill_amount[5][5];  //δ���й��ܣ�����1-4����
      }b;
}DC1_F168;


typedef struct{
     unsigned int eeAddr_Data;
     unsigned int eeAddr_Dest;
     unsigned int datavaluelen;
     unsigned int holds_dens;
     unsigned char Td_Flag;
     unsigned char fn_idx;   //�����ʶ���
     unsigned char datavalue[20];
    }C1Hold;




//�ն˹���ͳ��,���dateΪ3���ṹ��ĳߴ���Ϊ8��������7������dateʹ��2�ֽڣ���-��
typedef union{
     INT8U  vlaue[6];
     struct{
        INT16U  power_time;
        INT16U  reset_count;
        INT8U   date[2];
     };
}STAT_ERTU_PWR;

//ͳ����ֵ�趨ֵ
typedef struct{
   INT16U   v_maxmax;        //��ѹ������
   INT16U   v_max;
   INT16U   v_min;
   INT16U   v_minmin;
   INT32U   i_maxmax;
   INT32U   i_max;
   INT32U   in_max;
   INT32U   s_maxmax;
   INT32U   s_max;
   INT16U   u_unb_max;       //��ѹ��ƽ����ֵ
   INT16U   i_unb_max;
   INT16U   v_lost;		    //��������

   INT16S   v_maxmax_restore_coff;       //��ѹ������Խ�޻ָ�ϵ��
   INT16S   v_minmin_restore_coff;       //��ѹ������Խ�޻ָ�ϵ��
   INT16S   i_maxmax_restore_coff;       //����������Խ�޻ָ�ϵ��
   INT16S   i_max_restore_coff;       //��������Խ�޻ָ�ϵ��
   INT16S   in_max_restore_coff;       //�����������Խ�޻ָ�ϵ��
   INT16S   u_unb_restore_coff;       //��ѹ��ƽ��Խ�޻ָ�ϵ��
   INT16S   i_unb_restore_coff;       //������ƽ��Խ�޻ָ�ϵ��
   INT16S    s_maxmax_restore_coff;       //���ڹ���Խ�����޻ָ�ϵ��
   INT16S    s_max_restore_coff;       //���ڹ���Խ���޻ָ�ϵ��

   INT8U    v_maxmax_last_time;          //��ѹ������Խ�޳���ʱ��
   INT8U    v_minmin_last_time;          //��ѹ������Խ�޳���ʱ��
   INT8U    i_maxmax_last_time;          //����������Խ�޳���ʱ��
   INT8U    i_max_last_time;             //��������Խ�޳���ʱ��
   INT8U    in_max_last_time;            //�����������Խ�޳���ʱ��
   INT8U    u_unb_last_time;             //��ѹ��ƽ��Խ�޳���ʱ��
   INT8U    i_unb_last_time;             //������ƽ��Խ�޳���ʱ��
   INT8U    s_maxmax_last_time;    //���ڹ���Խ�����޳���ʱ��
   INT8U    s_max_last_time;       //���ڹ���Խ���޳���ʱ��
   INT8U    v_lost_last_time;      //����ʧѹʱ����ֵ

}STAT_SET;


//Խ�޼����,������Ҫһ������ʱ������ж�������ָ���Խ������
typedef struct{
    INT8U  over_last_time;       //Խ�޳���ʱ��
    #ifdef __PROVICE_JIBEI_CHECK__
    struct{
        INT8U  over_occur_flag:1;      //Խ�޷�����־
        INT8U  over_occur_start:1;     //�տ�ʼ����
        INT8U  res:6;
        };
    #else
    INT8U  over_occur_flag;      //Խ�޷�����־
    #endif
    INT8U  not_over_last_time;   //��Խ�޳���ʱ��
    INT8U  over_restore_flag;    //Խ�޻ָ���־ 
}tagOverMaxChecker;

//��ѹԽ��ͳ��ֵ
typedef union{
    INT8U  value[10];
    struct{
      INT16U v_over_maxmax_time;   //Խ��ѹ������ʱ��
      INT16U v_over_minmin_time;   //Խ��ѹ������ʱ��
      INT16U v_over_max_time;
      INT16U v_over_min_time;
      INT16U v_ok_time;      //��ѹ�ϸ�ʱ��

      //-------------��ֵ������-------------
      tagOverMaxChecker   chk_over_maxmax;     //Խ�����޼����
      tagOverMaxChecker   chk_over_max;        //Խ���޼����
      tagOverMaxChecker   chk_over_minmin;     //Խ�����޼����
    };
}STAT_V_OVER;

//��ѹ�����Сֵͳ��
typedef union{
    INT8U  value[12];
    struct{
        INT16U  v_max;
        INT8U   v_max_time[4];   //��-ʱ-��-��
        INT16U  v_min;
        INT8U   v_min_time[4];
    };
}STAT_V_MAXMIN;


//��ѹͳ������
typedef union
{
   INT8U  value[78];
   struct{
      STAT_V_OVER    v_over_time[3];   //A,B,C ���� ��ѹԽ���ۼ�ʱ��
      STAT_V_MAXMIN  v_maxmin[3];      //A,B,C ���� ��ֵͳ��
      INT16U         v_avg[3];         //A,B,C �����ѹƽ��ֵ (תΪ�ն�������ǰ����ż������)
      INT32U         v_sum[3];         //���A,B,C�����ѹ���ۼӺ͡�

   };
}STAT_V;

//��ƽ��ͳ������
typedef union
{
   INT8U  value[16];
   struct
   {
       INT16U         i_unbalance_time; //������ƽ���ۼ�ʱ��
       INT16U         v_unbalance_time; //��ѹ��ƽ���ۼ�ʱ��
       INT16U         i_unbalance_max;          //������ƽ�����ֵ
       INT8U          i_unbalance_max_time[4];  //��-ʱ-��-��
       INT16U         u_unbalance_max;          //��ѹ��ƽ�����ֵ
       INT8U          u_unbalance_max_time[4];  //��-ʱ-��-��

      //-------------��ֵ������-------------
      tagOverMaxChecker   chk_over_max_u;     //����ѹ��Խ�޼����
            //-------------��ֵ������-------------
      tagOverMaxChecker   chk_over_max_i;     //��������Խ�޼����
   };
}STAT_UNBALANCE;


//����Խ��
typedef union
{
   INT8U value[4];
   struct{
      INT16U   i_over_maxmax_time;
      INT16U   i_over_max_time;

      //-------------��ֵ������-------------
      tagOverMaxChecker   chk_over_maxmax;     //Խ�����޼����
            //-------------��ֵ������-------------
      tagOverMaxChecker   chk_over_max;        //Խ���޼����
   };
}STAT_I_OVER;


//������ֵ
typedef union{
    INT8U  value[8];
    struct{
        INT32U  i_max;
        INT8U   i_max_time[4];   //��-ʱ-��-��
    };
}STAT_I_MAX;

//����Խ��ͳ��
typedef union{
     INT8U  value[34];
     struct{
       STAT_I_OVER  i_over_time[4];   //A,B,C,N ����Խ��ʱ��
       STAT_I_MAX   i_max[4];        //A,B,C,N ��ֵͳ��
     };
}STAT_I;



//2�ࣺ �����ѹͳ������
typedef union{
    INT8U  value[66];
    struct{
       INT8U  va_over_time[10];   //A���ѹԽ�������ۼ�ʱ�䣬Խ�������ۼ�ʱ�䣬Խ�����ۼ�ʱ�䣬Խ�����ۼ�ʱ�䣬�ϸ�ʱ��
       INT8U  vb_over_time[10];
       INT8U  vc_over_time[10];
       INT8U  va_max[2];          //A���ѹ���ֵ
       INT8U  va_max_time[3];     //A���ѹ���ֵ����ʱ��  ��ʱ��
       INT8U  va_min[2];          //A���ѹ��Сֵ
       INT8U  va_min_time[3];     //A���ѹ��Сֵ����ʱ��  ��ʱ��
       INT8U  vb_max[2];
       INT8U  vb_max_time[3];
       INT8U  vb_min[2];
       INT8U  vb_min_time[3];
       INT8U  vc_max[2];
       INT8U  vc_max_time[3];
       INT8U  vc_min[2];
       INT8U  vc_min_time[3];
       INT8U  v_avg[6];           //A��ƽ����ѹ��B��ƽ����ѹ��C��ƽ����ѹ
       #if defined (__QGDW_376_2013_PROTOCOL__) || defined (__QGDW2013_VRATE__)
       INT8U  v_rate[27];
       #endif
    };
}C2_F27F35;

typedef struct{
    INT8U rec_date[5];
    C2_F27F35 c2f27f35;
}C2_F27F35_PLC;

//2�ࣺ���᲻ƽ��Խ������
typedef union{
   INT8U value[16];
   struct{
      INT8U   i_over_time[2];  //������ƽ��Խ��ʱ��
      INT8U   u_over_time[2];  //��ѹ��ƽ��
      INT8U   i_max[2];         //������ƽ�����ֵ
      INT8U   i_max_time[4];
      INT8U   u_max[2];        //��ѹ��ƽ�����ֵ
      INT8U   u_max_time[4];
   };
}C2_F28F36;

typedef union{
   INT8U value[14];
   struct{
      INT8U   i_over_time[2];  //������ƽ��Խ��ʱ��
      INT8U   u_over_time[2];  //��ѹ��ƽ��
      INT8U   i_max[2];         //������ƽ�����ֵ
      INT8U   i_max_time[3];
      INT8U   u_max[2];        //��ѹ��ƽ�����ֵ
      INT8U   u_max_time[3];
   };
}C2_F28;

//2�ࣺ�������Խ������
typedef union{
      INT8U value[38];
      struct{
         INT8U i_overtime[14];  //A,B,CԽ�����ޣ�Խ�����ۼ�ʱ�䣬N Խ�����ۼ�ʱ��
         INT8U ia_max[3];
         INT8U ia_max_time[3];
         INT8U ib_max[3];
         INT8U ib_max_time[3];
         INT8U ic_max[3];
         INT8U ic_max_time[3];
         INT8U in_max[3];
         INT8U in_max_time[3];
      };
}C2_F29F37;


//2�ࣺ�ո�����ͳ��
typedef union{
      INT8U value[12];
      struct{
         INT8U fzl_max[2];
         INT8U fzl_max_time[3];
         INT8U fzl_min[2];
         INT8U fzl_min_time[3];
      };
      struct{
         INT8U fzl_max[2];
         INT8U fzl_max_time[4];
         INT8U fzl_min[2];
         INT8U fzl_min_time[4];
      }e;
}C2_F31F39;





//2�ࣺ�������ڹ���Խ���ۼ�ʱ��
typedef union{
    INT8U  value[4];
    struct{
        INT8U  over_maxmax[2];
        INT8U  over_max[2];
    };
    struct{
        INT16U  s_over_maxmax;            //Խ�������ۼ�ʱ��
        INT16U  s_over_max;               //Խ�����ۼ�ʱ��

        //-------------����ͳ�Ƶı���-------------
        tagOverMaxChecker   chk_over_maxmax;   //Խ�����޼����
        tagOverMaxChecker   chk_over_max;      //Խ���޼����
    };
}C2_F30F38;

//2��  ������Ͷ���ۼ�ʱ��ʹ���
typedef union{
   INT8U  value[72];
   struct{
     INT8U   run_time[9][4];   //��1-9��������ۼ�Ͷ��ʱ��
     INT8U   run_count[9][4];  //��1-9��������ۼ�Ͷ�����
   };
}C2_F41,C1_F66;

//2��  ���µ����ۼƲ����޹�������
typedef union{
    INT8U value[8];
    struct{
        INT8U  day_wg_cap[4];
        INT8U  month_wg_cap[4];
    };
}C2_F42,C1_F67;

//2�ࣺ���Ṧ�����������ۼ�ʱ��
typedef union{
    INT8U   value[6];
    struct{
        INT8U  seg1[2];
        INT8U  seg2[2];
        INT8U  seg3[2];
    };
    struct{
        INT16U  seg1_time;
        INT16U  seg2_time;
        INT16U  seg3_time;
    };
}C2_F43F44;

//2�ࣺ�����ܼ���������й����ʼ�����ʱ��
typedef union{
    INT8U  value[6];
    struct{
        INT8U  p_max[3];
        INT8U  p_max_time[3]; //��ʱ��
    };
}C2_F25F33_PYG;
typedef union{
    INT8U   value[32];
    struct{
        C2_F25F33_PYG   pyg_max[4];       //�ܣ�A,B,C����й����ʼ�����ʱ��
        INT16U          pyg_zero_time[4]; //�ܣ�A,B,C����Ϊ���ۼ�ʱ��
    };
}C2_F25F33;

//2�ࣺ�����ܼ������й��������������ʱ��
// ���ͳ�����ݵ�����ţ�����ֻ���ڱ仯ʱ�ſ���д�롣
typedef union{
    INT8U  value[24];
    struct{
      C2_F25F33_PYG  xl_max[4];
    };
}C2_F26F34;


//2�࣬�ն����ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
typedef union{
    INT8U value[12];
    struct{
      INT8U  p_max[2];
      INT8U  p_max_time[3];
      INT8U  p_min[2];
      INT8U  p_min_time[3];
      INT8U  p_zero_time[2];
    };
}C2_F57;

//2�࣬�գ��¶���ֱ��ģ����Խ���ۼ�ʱ�䣬�����Сֵ����ʱ��
typedef union{
   INT8U value[14];
   struct{
      INT8U   over_max_time[2];
      INT8U   over_min_time[2];
      INT8U   value_max[2];
      INT8U   value_max_time[3];
      INT8U   value_min[2];
      INT8U   value_min_time[3];
   };
}C2_F129,C2_F130;

//2�ࣺF153
typedef union{
     INT8U value[20];
     struct{
          INT8U read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
          INT8U yg_a[5];      //A,B,C�й�
          INT8U yg_b[5];
          INT8U yg_c[5];
     };
}C2_F153,C2_F155,C2_F157,C2_F159;

//2�ࣺF154
typedef union{
     INT8U value[17];
     struct{
          INT8U read_date[5];  //�ն˳���ʱ��  ���ݸ�ʽ15 ��-ʱ-��-��-��
          INT8U wg_a[4];      //A,B,C�޹�
          INT8U wg_b[4];
          INT8U wg_c[4];        
     };
}C2_F154,C2_F156,C2_F158,C2_F160;

//2�ࣺ �����������������ʱ��
typedef union{
     INT8U  value[41];
     struct{
        INT8U   read_date[5];
        INT8U   fl_count;
        INT8U   zdxl[35];
     };
}C2_F185_188;

//�������¼���־
typedef union{
     INT8U  value;
     struct{
        INT8U   v_over_maxmax:1;        //��ѹԽ������   ��ǰ״̬
        INT8U   v_over_maxmax_evt:1;    //��ѹԽ������   �ϴα�־
        INT8U   v_over_minmin:1;        //��ѹԽ������   ��ǰ״̬
        INT8U   v_over_minmin_evt:1;    //��ѹԽ������   �ϴα�־
        INT8U   v_lost:1;               //����           ��ǰ״̬
        INT8U   v_lost_evt:1;           //����           �ϴα�־
        INT8U   v_low:1;                //ʧѹ           ��ǰ״̬
        INT8U   v_low_evt:1;            //ʧѹ           �ϴα�־
     };
}SPOT_V_EVENT_FLAG;


typedef union{
     INT8U  value;
     struct{
        INT8U   i_over_maxmax:1;        //����Խ������   ��ǰ״̬
        INT8U   i_over_maxmax_evt:1;    //����Խ������   �ϴα�־
        INT8U   i_over_max:1;           //����Խ����     ��ǰ״̬
        INT8U   i_over_max_evt:1;       //����Խ����     �ϴα�־
     };
}SPOT_I_EVENT_FLAG;

typedef union{
       INT8U  value;
       struct{
        INT8U   s_over_maxmax:1;        //���ڹ���Խ������    ��ǰ״̬
        INT8U   s_over_maxmax_evt:1;    //���ڹ���Խ������    �ϴα�־
        INT8U   s_over_max:1;           //���ڹ���Խ����       ��ǰ״̬
        INT8U   s_over_max_evt:1;       //���ڹ���Խ����      �ϴα�־
        INT8U   v_unbalance:1;          //��ѹ��ƽ��          ��ǰ״̬
        INT8U   v_unbalance_evt:1;      //��ѹ��ƽ��          �ϴα�־
        INT8U   i_unbalance:1;          //������ƽ��         ��ǰ״̬
        INT8U   i_unbalance_evt:1;      //������ƽ��         �ϴα�־
       };
}SPOT_S_UNB_EVENT_FLAG;


typedef union{
      INT8U  value[4];
      struct{
         SPOT_V_EVENT_FLAG      phase_v[3];  //a,b,c����ĵ�ѹ�¼�״̬
         SPOT_I_EVENT_FLAG      phase_i[3];   //a,b,c����ĵ����¼�״̬,�����������������û�����¼�
         SPOT_S_UNB_EVENT_FLAG   s_unb;        //���ڹ��ʼ���ƽ��
      };
}SPOT_EVENT_FLAG;

//������ͳ�������ļ���
//
//������˲ʱ��ͳ������
typedef union{
      INT8U  value[296];
      struct{
         INT8U            stat_date[4]; //ͳ�����ڣ�BIN ʱ-��-��-��
         STAT_V           stat_v;    //��ѹͳ��
         STAT_I           stat_i;    //����ͳ��
         STAT_UNBALANCE   stat_unb;  //��ƽ��ͳ��
         C2_F30F38        stat_s;    //���ڹ���ͳ��
         C2_F43F44        stat_q;    //���������ֶ�ͳ��
         C2_F25F33        stat_pyg;  //�ܼ���������й����ʼ�����ʱ��
         SPOT_EVENT_FLAG  event_flag;  //�¼����ɱ�־
         #ifdef __STAT_LOADRATE__
         C2_F31F39        loadrate;    //������ͳ��
         #else
         INT8U            reserved[8];
         #endif
      };
      SET_F26             set_f26;
}SPOT_STAT;

//2�ࣺ F101 F102 F103 F104  ���������������޹��ܵ���ʾֵ����
//      F145,F146,F147,F148  �������޹�����ʾֵ����
typedef union{
     INT8U  value[53];
     struct{
        INT8U  read_date[5];
        INT8U  zxygz[4];
        INT8U  zxwgz[4];
        INT8U  fxygz[4];
        INT8U  fxwgz[4];
        INT8U  zxyg_fl[16];   //�����й����� ����1��2��3��4
        INT8U  wg1[4];  //һ�����޹�
        INT8U  wg2[4];  //�������޹�
        INT8U  wg3[4];  //�������޹�
        INT8U  wg4[4];  //�������޹�
     };
}C2_SPOT_CURVE_ZFXYWG;

// ���������������
typedef union{
    INT8U  value[21];
    struct{
       INT8U  fl_count;
       INT8U  amount[20];
    };
}SPOT_AMOUNT;



//�ն˵�����ͳ�����ݱ�������
typedef union{
   INT8U   value[15];
   struct{
      INT8U  date[3];
      INT8U  F49[4];
      INT8U  F50[4];
      INT8U  F53[4];
   }d;
   struct{
      INT8U  year;      //HEX��ʽ��
      INT8U  month;
      INT8U  day;
      INT8U  F51[4];
      INT8U  F52[4];
      INT8U  F54[4];
   }e;
}C2_ERTU_DATA;



//F65: ��ǰ������Ͷ��״̬
typedef union{
     INT8U value[3];
     struct{
         INT8U  run_mode;        //BS8 ���з�ʽ
         INT8U  run_state[2];    //BS16 ������Ͷ��״̬
     };
}C1_CAP_RUNSTATE;


//F66: ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
typedef union{
    INT8U value[72];
    struct{
       INT32U    run_time[9];    //��1-9���ۼ�Ͷ��ʱ�䣬��λ����
       INT32U    run_count[9];   //��1-9��������ۼ�Ͷ�����.
    };
}C1_CAP_RUNTIME;

//F67: ���գ����µ������ۼƲ������޹�������
typedef union{
     INT8U  value[8];
     struct{
        INT8U   day_wg[4];        //���ղ������޹�������    XXXX.XXXX  KVar
        INT8U   month_wg[4];      //���²������޹�������
     };
}C1_CAP_WG;

typedef union{
    INT8U  value[5];
    struct{
       INT8U  read_date[5];
       INT8U  qdcs[2];
           };
}C1_F187;

//F162������������ܱ����ʮ��ʧѹ����   //������չ
/*typedef union{
     INT8U  value[121];
     struct{
        INT8U   count;        //���μ�¼����
        INT8U   lost_vi[10][3];      //ʧѹ�ۼƴ���   z a b c
     };
     struct{
        INT8U   count;        //���μ�¼����
        INT8U   program_count[3];      
        INT8U   program_time[10][5];
     }b;
}C1_F162;  */

typedef union{
    INT8U value[10];
    struct {
    INT8U start_time[5];
    INT8U stop_time[5];
    };
}POWEROFF_RECORD;
//�����¼
typedef union{
   INT8U value[103];
   struct{
     INT8U           count[3];
     POWEROFF_RECORD record[10];
   };
}C1_F246,C2_F246;

//��������
typedef union{
   INT8U value[7];
   struct{
     INT8U  noise;
     INT8U  node[6];
   }b;
}C2_F218;

typedef union{
   INT8U  value[24+5];
   struct{
     INT8U    read_date[5];    //��-��-��-ʱ-��
     INT8U    white_noise[3];
     C2_F218   color_noise[3]; 
   }b;
}C2_PLC_NOISE;

typedef union{
    INT8U  value[29];
    struct{
        INT8U rec_date[5];       //��-��-��-ʱ-�� BCD��
        INT8U under_v_z[3];      //��Ƿѹ����
        INT8U under_v_a[3];      //A��Ƿѹ�ܴ���
        INT8U under_v_b[3];      //B��Ƿѹ�ܴ���
        INT8U under_v_c[3];      //C��Ƿѹ�ܴ���
        INT8U under_v_z_time[3]; //Ƿ���ۼ�ʱ��
        INT8U under_v_a_time[3]; //AǷѹ�ۼ�ʱ��
        INT8U under_v_b_time[3]; //BǷѹ�ۼ�ʱ��
        INT8U under_v_c_time[3]; //CǷѹ�ۼ�ʱ��
    };
}C2_F300;

typedef union{
    INT8U  value[168];
    struct{
        INT8U rec_date[5];       //��-��-��-ʱ-�� BCD��
        INT8U lost_i_z[3];       //ʧ�������
        INT8U lost_i_a[3];       //A��ʧ���ܴ���
        INT8U lost_i_b[3];       //B��ʧ���ܴ���
        INT8U lost_i_c[3];       //C��ʧ���ܴ���
        INT8U lost_i_z_time[3];  //ʧ���ۼ�ʱ��
        INT8U lost_i_a_time[3];  //Aʧ���ۼ�ʱ��
        INT8U lost_i_b_time[3];  //Bʧ���ۼ�ʱ��
        INT8U lost_i_c_time[3];  //Cʧ���ۼ�ʱ��

        INT8U lost_i_a_begin[6]; //(��1��)A��ʧ������ʱ��
        INT8U lost_i_a_end[6];   //(��1��)A��ʧ������ʱ��
        INT8U lost_i_z_zxyg[4];  //ʧ���ڼ������й��ܵ�������
        INT8U lost_i_z_fxyg[4];  //ʧ���ڼ䷴���й��ܵ�������
        INT8U lost_i_z_zxwg[4];  //ʧ���ڼ�����޹�1�ܵ�������
        INT8U lost_i_z_fxwg[4];  //ʧ���ڼ�����޹�2�ܵ�������
        INT8U lost_i_a_zxyg[4];  //ʧ���ڼ�A�������й���������
        INT8U lost_i_a_fxyg[4];  //ʧ���ڼ�A�෴���й���������
        INT8U lost_i_a_zxwg[4];  //ʧ���ڼ�A������޹�1��������
        INT8U lost_i_a_fxwg[4];  //ʧ���ڼ�A������޹�2��������
        INT8U lost_i_a_v[2];     //ʧ��ʱ��A���ѹ
        INT8U lost_i_a_i[3];     //ʧ��ʱ��A�����
        INT8U lost_i_a_yggl[3];  //ʧ��ʱ��A���й�����
        INT8U lost_i_a_wggl[3];  //ʧ��ʱ��A���޹�����
        INT8U lost_i_a_glys[2];  //ʧ��ʱ��A�๦������

        INT8U lost_i_b_begin[6]; //(��1��)B��ʧ������ʱ��
        INT8U lost_i_b_end[6];   //(��1��)B��ʧ������ʱ��
        INT8U lost_i_b_zxyg[4];  //ʧ���ڼ�B�������й���������
        INT8U lost_i_b_fxyg[4];  //ʧ���ڼ�B�෴���й���������
        INT8U lost_i_b_zxwg[4];  //ʧ���ڼ�B������޹�1��������
        INT8U lost_i_b_fxwg[4];  //ʧ���ڼ�B������޹�2��������
        INT8U lost_i_b_v[2];     //ʧ��ʱ��B���ѹ
        INT8U lost_i_b_i[3];     //ʧ��ʱ��B�����
        INT8U lost_i_b_yggl[3];  //ʧ��ʱ��B���й�����
        INT8U lost_i_b_wggl[3];  //ʧ��ʱ��B���޹�����
        INT8U lost_i_b_glys[2];  //ʧ��ʱ��B�๦������

        INT8U lost_i_c_begin[6]; //(��1��)C��ʧ������ʱ��
        INT8U lost_i_c_end[6];   //(��1��)C��ʧ������ʱ��
        INT8U lost_i_c_zxyg[4];  //ʧ���ڼ�C�������й���������
        INT8U lost_i_c_fxyg[4];  //ʧ���ڼ�C�෴���й���������
        INT8U lost_i_c_zxwg[4];  //ʧ���ڼ�C������޹�1��������
        INT8U lost_i_c_fxwg[4];  //ʧ���ڼ�C������޹�2��������
        INT8U lost_i_c_v[2];     //ʧ��ʱ��C���ѹ
        INT8U lost_i_c_i[3];     //ʧ��ʱ��C�����
        INT8U lost_i_c_yggl[3];  //ʧ��ʱ��C���й�����
        INT8U lost_i_c_wggl[3];  //ʧ��ʱ��C���޹�����
        INT8U lost_i_c_glys[2];  //ʧ��ʱ��C�๦������
    };
}C2_F301;

typedef union{
    INT8U  value[129];
    struct{
        INT8U rec_date[5];       //��-��-��-ʱ-�� BCD��
        INT8U lost_i_z[3];       //ʧ�������
        INT8U lost_i_a[3];       //A��ʧ���ܴ���
        INT8U lost_i_b[3];       //B��ʧ���ܴ���
        INT8U lost_i_c[3];       //C��ʧ���ܴ���
        INT8U lost_i_z_time[3];  //ʧ���ۼ�ʱ��
        INT8U lost_i_a_time[3];  //Aʧ���ۼ�ʱ��
        INT8U lost_i_b_time[3];  //Bʧ���ۼ�ʱ��
        INT8U lost_i_c_time[3];  //Cʧ���ۼ�ʱ��

        INT8U lost_i_a_begin[6]; //(��1��)A��ʧ������ʱ��
        INT8U lost_i_a_end[6];   //(��1��)A��ʧ������ʱ��
        INT8U lost_i_z_zxyg[4];  //ʧ���ڼ������й��ܵ�������
        INT8U lost_i_z_fxyg[4];  //ʧ���ڼ䷴���й��ܵ�������
        INT8U lost_i_z_zxwg[4];  //ʧ���ڼ�����޹�1�ܵ�������
        INT8U lost_i_z_fxwg[4];  //ʧ���ڼ�����޹�2�ܵ�������
        INT8U lost_i_a_zxyg[4];  //ʧ���ڼ�A�������й���������
        INT8U lost_i_a_fxyg[4];  //ʧ���ڼ�A�෴���й���������
        INT8U lost_i_a_zxwg[4];  //ʧ���ڼ�A������޹�1��������
        INT8U lost_i_a_fxwg[4];  //ʧ���ڼ�A������޹�2��������

        INT8U lost_i_b_begin[6]; //(��1��)B��ʧ������ʱ��
        INT8U lost_i_b_end[6];   //(��1��)B��ʧ������ʱ��
        INT8U lost_i_b_zxyg[4];  //ʧ���ڼ�B�������й���������
        INT8U lost_i_b_fxyg[4];  //ʧ���ڼ�B�෴���й���������
        INT8U lost_i_b_zxwg[4];  //ʧ���ڼ�B������޹�1��������
        INT8U lost_i_b_fxwg[4];  //ʧ���ڼ�B������޹�2��������

        INT8U lost_i_c_begin[6]; //(��1��)C��ʧ������ʱ��
        INT8U lost_i_c_end[6];   //(��1��)C��ʧ������ʱ��
        INT8U lost_i_c_zxyg[4];  //ʧ���ڼ�C�������й���������
        INT8U lost_i_c_fxyg[4];  //ʧ���ڼ�C�෴���й���������
        INT8U lost_i_c_zxwg[4];  //ʧ���ڼ�C������޹�1��������
        INT8U lost_i_c_fxwg[4];  //ʧ���ڼ�C������޹�2��������
    };
}C2_F303;

typedef union{
    INT8U  value[20];
    struct{
        INT8U rec_date[5];            //��-��-��-ʱ-�� BCD��
        INT8U lost_power[3];          //�����ܴ���
        INT8U lost_power_begin[6];    //���һ�ε��緢��ʱ��
        INT8U lost_power_end[6];      //���һ�ε������ʱ��
    };
}C2_F305;

typedef union{
    INT8U value[10];
    struct{
        INT8U rec_date[5];
        INT8U before_err[2];
        INT8U after_err[2];
        INT8U flag;
    };
}PRECISE_TIME_RECORD; //__PRECISE_TIME__

typedef union{
    INT8U value[10];
    struct{
        INT8U flag;
        INT8U cycle;
        INT8U meter_date[4];
        INT8U meter_error[4];
    };
}PRECISE_TIME_METER_CTRL; //__PRECISE_TIME__

typedef union{
    INT8U value[12];
    struct{
        INT8U spot_idx[2];
        PRECISE_TIME_RECORD record;
    };
}PRECISE_TIME_STAT_DATA; //__PRECISE_TIME__

typedef union{
    INT8U value[7];
    struct{
        DATAFMT_15 rec_date;
        INT8U temperature[2];
    };
}TEMP_CURVE;

typedef union{
   INT8U  value[17];
   struct{
     INT8U    depth;
     INT8U    spot_idx[2];
     INT8U    meter_no[6];
     INT8U    parent_spot_idx[2];
     INT8U    parent_meter_no[6];
   };
}PLC_RELAY_INFO;

typedef union{
    INT8U  value[7];
    struct{
        INT8U state;
        INT8U meter_no[6];
    };
}PLC_CTRL_REC_68;

typedef union{
    INT8U    value;
    struct{
        INT8U biti:3;    //��¼��λλ��
        INT8U protocol:1;   //��Լ���� 0:97  1:07
        INT8U flag:1;      //�Ƿ�ʹ��ȫA��ȫ9 ����
        INT8U start:2;   //��ͣ��־ 0: �����ѱ�  >=2����ѱ�
        INT8U isaddF10:1;   //�·������F10     0 ��δ��� 1�������
    };
} SEARCH_485;

#ifdef __SHANGHAI_CLASS_53_54_485_CURVE__
typedef union{
    INT8U value[2];
    struct{
        INT8U read_date[5];   //����ʱ�� bcd
        INT8U zxygz[4];       //�����й���
        INT8U v[6];           //��ѹ
    };
}CLASS_53_54_CURVE;
#endif

typedef union{
   INT8U value[7];
   struct{
     INT8U rec_date[5];       //��-��-��-ʱ-�� BCD��
     INT8U  v[2];
     INT8U  i[2];
   };
}C2_F211;

//��������
typedef union
{
    INT8U value[260];
    struct
    {
        C1F25 LX_f25;
        C1F33 LX_f33;
        C1F34 LX_f34;
    }e;
}tagDATA_LOOP_DIS;

#ifdef __REPORT_NODE_EVENT__
typedef union{
    INT8U value[2];
    struct{
        INT8U length;                  //���ݳ���+4
        INT8U item[4];                 //������
        INT8U meter_state_word[110];   //��Լ���� 12+96+2
    };
}DATA_04001501;

typedef union{
    INT8U value;
    struct{
        INT8U count:7;
        INT8U mask:1;
    };
}EVENT_COUNT;

typedef union{
    INT8U    value[2];
    struct{
        INT8U ctrl_flag;               //���Ʊ�ʶ
        INT8U meter_state_word[12];    //״̬��
        EVENT_COUNT event_count;       //�¼���������������
        INT8U event_mask[1];           //�¼���¼����
    };
}EVENT_READ_CTRL;
#endif

typedef union{
    INT8U    value[15];
    struct{
	    INT8U datetime[5];
        INT8U cur_day_fee[5];		//����ʵʱ�õ���
        INT8U cur_month_fee[5];		//�����ۼ��õ���
    };
}C1F181,C2F240;
 
#endif
