#ifndef __TASKRECORDMETER_H__
#define __TASKRECORDMETER_H__

//交采编程按键通知类型
#define DIS_CYPROGNOTE   0x00
#define EN_CYPROGNOTE    0x01

#define BTN_CYPROG_MASK  0x02

//==============================================================================
//
//          485电表数据安排
//
//==============================================================================


//测量点曲线数据类别定义
#define  SPOT_CURVE_PQUI        1          //测量点功率，电压，电流曲线
#define  SPOT_CURVE_ZFXYWG      2          //测量点电能量示值曲线
#define  SPOT_CURVE_AMOUNTS     3          //测量点电能量曲线
#define  RTUA_CURVE_NOISE       4          //载波主节点噪声曲线
#define  SPOT_CURVE_FL  		5          //测量点费率电能量曲线

#define REC_DATE_MINUTE   0
#define REC_DATE_HOUR     1
#define REC_DATE_DAY      2
#define REC_DATE_MONTH    3
#define REC_DATE_YEAR     4


//当前数据文件安排  FILEID_485_CUR
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
#define PIM_C1_F35         (PIM_C1_F34+91)             //76, 当月正向有、无功最大需量及发生时间
#define PIM_C1_F36         (PIM_C1_F35+76)             //76, 当月反向有、无功最大需量及发生时间
#define PIM_C1_F37         (PIM_C1_F36+76)             //91,
#define PIM_C1_F38         (PIM_C1_F37+91)             //91,
#define PIM_C1_F33_DAY     (PIM_C1_F38+91)             //91, 电能量日初值
#define PIM_C1_F34_DAY     (PIM_C1_F33_DAY+91)         //91, 电能量日初值
#define PIM_C1_F33_MONTH   (PIM_C1_F34_DAY+91)          //91, 电能量月初值
#define PIM_C1_F34_MONTH   (PIM_C1_F33_MONTH+91)       //91, 电能量月初值
#define PIM_C1_F58         (PIM_C1_F34_MONTH+91)       //223,谐波含有率
#define PIM_C1_F57         (PIM_C1_F58+223)            //217,谐波有效值
#define PIM_C1_F49         (PIM_C1_F57+217)            //12, 当前电压，电流相位角
#define PIM_C1_F161        (PIM_C1_F49+12)             //C1_F161_LEN, 电能表远程控制通断电状态及记录
#define PIM_C1_F165        (PIM_C1_F161+C1_F161_LEN)   //C1_F165_LEN, 电能表开关操作次数及时间
#define PIM_C1_F166        (PIM_C1_F165+C1_F165_LEN)   //C1_F166_LEN, 电能表参数修改次数及时间
#define PIM_C1_F167        (PIM_C1_F166+C1_F166_LEN)   //41, 电能表购，用电信息
#define PIM_C1_F168        (PIM_C1_F167+41)            //56, 电能表结算信息
#define PIM_C1_F31         (PIM_C1_F168+56)            //59, 当前ABC三相正反向有功电能示值，组合无功1，2电能示值
#define PIM_C1_F32         (PIM_C1_F31+59)             //59, 上一结算日A，B，C三相正反向有功电能示值，组合无功1，2电能示值
#define PIM_LAST_ZXYGZ     (PIM_C1_F32+59)             //11, 上次抄表时间+费率+正向有功总电能示值
#define PIM_ZXYGZ_STOP     (PIM_LAST_ZXYGZ+11)         //2,  电表不走持续时间.
#define PIM_TIMESEG_97     (PIM_ZXYGZ_STOP+2)          //84, 97电表的费率时段信息C33F(42),C34F(42)
#define PIM_C1_F39         (PIM_TIMESEG_97+84)         //76, 上一月正向有、无功最大需量及发生时间
#define PIM_C1_F40         (PIM_C1_F39+76)             //76, 上一月反向有、无功最大需量及发生时间
#define PIM_C1_F33_MONTH2  (PIM_C1_F40+76)             //91, 上次电能量月初值
#define PIM_C1_F34_MONTH2  (PIM_C1_F33_MONTH2+91)      //91, 上次电能量月初值
#define PIM_C1_F162        (PIM_C1_F34_MONTH2+91)      //12, 电能表日历时钟
#define PIM_C1_F177        (PIM_C1_F162+12)            //31, 当前组合有功电能示值
#define PIM_C1_F178        (PIM_C1_F177+31)            //31, 上1结算日组合有功电能示值
#define PIM_C1_F246        (PIM_C1_F178+31)            //103, 当前掉电记录
/////山东使用  begin
#define PIM_REC_DAY_PARAM  (PIM_C1_F246+103)           //6,存储3个抄表日
#define PIM_I_BREAK_COUNT  (PIM_REC_DAY_PARAM+6)       //9,a、b、c相断流次数
#define PIM_V_LOST_COUNT   (PIM_I_BREAK_COUNT+9)       //6=3+3,失压总次数、全失压总次数
#define PIM_P_SUM_DAY      (PIM_V_LOST_COUNT+6)        //4,日功率总和
#define PIM_P_COUNT_DAY    (PIM_P_SUM_DAY+4)           //2,日功率抄读次数
#define PIM_P_AVERAGE_STOP (PIM_P_COUNT_DAY+2)         //4,判断停走的平均功率

/////山东使用  end
#define PIM_C1_F33_DAY2     (PIM_P_AVERAGE_STOP+4)       //91, 上次电能量日初值
#define PIM_C1_F34_DAY2     (PIM_C1_F33_DAY2+91)         //91, 上次电能量日初值



//日冻结数据   FILEID_485_DH
#define PIM_C2_MONTH      (INT8U)2                  //二类数据保存月数，1月=31天，2月=62天......
#define PIM_C2_F1         0                                         //91*31 = 2821
#define PIM_C2_F2      (PIM_C2_F1+2821*PIM_C2_MONTH)              //91*31 = 2821
#define PIM_C2_F32     (PIM_C2_F2+2821*PIM_C2_MONTH)              //C2_F32_LEN*31 = 1767
#define PIM_C2_F25     (PIM_C2_F32+C2_F32_LEN*31*PIM_C2_MONTH)    //32*31 =  992    日总及分相最大有功功率统计
#define PIM_C2_F26     (PIM_C2_F25+992*PIM_C2_MONTH)              //24*31 =  744    日总及分相最大需量统计
#if defined (__QGDW_376_2013_PROTOCOL__) || defined (__QGDW2013_VRATE__)
#define PIM_C2_F27     (PIM_C2_F26+744*PIM_C2_MONTH)              //93*31 = 2883    日电压统计
#define PIM_C2_F28     (PIM_C2_F27+2883*PIM_C2_MONTH)             //16*31 =  496    日不平衡统计
#else
#define PIM_C2_F27     (PIM_C2_F26+744*PIM_C2_MONTH)              //66*31 = 2046    日电压统计
#define PIM_C2_F28     (PIM_C2_F27+2046*PIM_C2_MONTH)             //16*31 =  496    日不平衡统计
#endif
#define PIM_C2_F29     (PIM_C2_F28+496*PIM_C2_MONTH)              //38*31 = 1178    日电流统计
#define PIM_C2_F30     (PIM_C2_F29+1178*PIM_C2_MONTH)             //4 *31 =  124    日视在功率统计
#define PIM_C2_F43     (PIM_C2_F30+124*PIM_C2_MONTH)              //6*31  =  186    日功率因数统计
#define PIM_C2_F3      (PIM_C2_F43+186*PIM_C2_MONTH)              //76*31 = 2356    日冻结最大需量及发生时间(费率)
#define PIM_C2_F4      (PIM_C2_F3+2356*PIM_C2_MONTH)              //76*31 = 2356    日冻结最大需量及发生时间(费率)
#define PIM_C2_F153    (PIM_C2_F4+2356*PIM_C2_MONTH)              //59*31 = 1829    日冻结分相电量
#define PIM_C2_F209    (PIM_C2_F153+1829*PIM_C2_MONTH)            //C2_F209_LEN*31 =  496    日冻结电能表远程控制状态及信息
#define PIM_C2_F31     (PIM_C2_F209+C2_F209_LEN*31*PIM_C2_MONTH)  //10*31 =  310    日负载率统计
#ifdef __REC_DH_485_EXT__
#define PIM_C2_F300   (PIM_C2_F31  + 310*PIM_C2_MONTH)            //29*31 =  899      日冻结电能表欠压数据
#define PIM_C2_F301   (PIM_C2_F300 + 899*PIM_C2_MONTH)            //168*31 =  5208    日冻结电能表失流数据
#define PIM_C2_F302   (PIM_C2_F301 + 5208*PIM_C2_MONTH)           //168*31 =  5208    日冻结电能表过流数据
#define PIM_C2_F303   (PIM_C2_F302 + 5208*PIM_C2_MONTH)           //129*31 =  3999    日冻结电能表潮流数据
#define PIM_C2_F304   (PIM_C2_F303 + 3999*PIM_C2_MONTH)           //168*31 =  5208    日冻结电能表断流数据
#define PIM_C2_F305   (PIM_C2_F304 + 5208*PIM_C2_MONTH)           //20*31 =  620    日冻结电能表掉电
#define PIM_C2_F211   (PIM_C2_F305 + 5828*PIM_C2_MONTH)           //9*31 =  279    日冻结三相电压、电流不平衡度95%概率值
#define PIM_C2_F210   (PIM_C2_F211 + 279*PIM_C2_MONTH)     //41*31 =  1271    日冻结够用点信息
#else
#define PIM_C2_F211   (PIM_C2_F31  + 310*PIM_C2_MONTH)           //9*31 =  279    日冻结三相电压、电流不平衡度95%概率值
#define PIM_C2_F210   (PIM_C2_F211 + 279*PIM_C2_MONTH)     //41*31 =  1271    日冻结够用点信息
#endif
#define PIM_C2_F246   (PIM_C2_F210 + 1271*PIM_C2_MONTH)    //103*31 = 3193   日冻结掉电记录
#ifdef __GET_DH_AMOUNT_FROM_REAL__
#define PIM_C2_F1_REAL (PIM_C2_F246 + 3193*PIM_C2_MONTH)  // 91*31 = 2821
#define PIM_C2_F2_REAL (PIM_C2_F1_REAL + 2821*PIM_C2_MONTH) ////91*31 = 2821
#endif
#define PIM_C2_F240	  (PIM_C2_F246 + 465*PIM_C2_MONTH)			//15*31 = 465 联东地产项目扩展 日冻结用电金额


//测量点瞬时量曲线数据(从C1F25中得到,保存最小间隔为15分钟,每日96点，连续7天)
//FILEID_485_CURVE
#define PIM_CURVE_C1F25  0              //67*96*7 = 45024



//测量点正反向有无功电能示值曲线       
//FILEID_485_CURVE_YWG                 53字节*96个点*10天=50880  zy 2011.3.24 更新
// 保存格式｛抄表时间/5 ZXYGZ/4   ZXWGZ/4  FXYGZ/4  FXWGZ/4｝ = 21字节


//测量点抄表日冻结数据文件
//FILEID_485_RECDAY
#define PIM_C2_F9        0              //91*31 = 2821
#define PIM_C2_F10    2821              //91*31 = 2821
#define PIM_C2_F11    5642              //76*31 = 2356
#define PIM_C2_F12    7998              //76*31 = 2356

//测量点月冻结数据文件
//FILEID_485_MH
#define PIM_C2_F17         0              //91*12 = 1092
#define PIM_C2_F18      1092              //91*12 = 1092
#define PIM_C2_F19      2184              //76*12 =  912
#define PIM_C2_F20      3096              //76*12 =  912
#define PIM_C2_F35      4008              //93*12 =  1116,792   月电压统计
#if defined (__QGDW_376_2013_PROTOCOL__) || defined (__QGDW2013_VRATE__)
#define PIM_C2_F36      5124              //16*12 =  192   月不平衡统计
#else
#define PIM_C2_F36      4800              //16*12 =  192   月不平衡统计
#endif
#define PIM_C2_F37      (PIM_C2_F36+192)              //38*12 =  456   月电流统计
#define PIM_C2_F38      (PIM_C2_F37+456)              //4*12  =   48   月视在功率统计
#define PIM_C2_F44      (PIM_C2_F38+48)              //6*12  =   72   月功率因数区间统计
#define PIM_C2_F33      (PIM_C2_F44+72)              //32*12 =  384  月总及分相最大有功功率统计
#define PIM_C2_F34      (PIM_C2_F33+384)              //24*12 =  288  月总及分相最大需量统计
#define PIM_C2_F157     (PIM_C2_F34+288)              //59*12 =  708  月冻结分相电能示值
#if defined (__QGDW_376_2013_PROTOCOL__)
    #define C2_F213_LEN     23            //月冻结电能表开关操作次数及时间长度
    #define C2_F214_LEN     29            //月冻结电能表参数修改次数及时间长度
#else
    #define C2_F213_LEN     19            //月冻结电能表开关操作次数及时间长度
    #define C2_F214_LEN     19            //月冻结电能表参数修改次数及时间长度
#endif
#define PIM_C2_F213     (PIM_C2_F157+708)              //C2_F213_LEN*12 =276或228  月冻结电能表开关操作次数及时间

#define PIM_C2_F214     (PIM_C2_F213+C2_F213_LEN*12)              //19*12 =  228  月冻结电能表参数修改次数及时间
#define PIM_C2_F39      (PIM_C2_F214+C2_F214_LEN*12)              //12*12 =  144  月负载率统计
#ifdef __REC_DH_485_EXT__
#define PIM_C2_F308   (PIM_C2_F39  + 144)     //29*12 =  348      月冻结电能表欠压数据
#define PIM_C2_F309   (PIM_C2_F308 + 348)     //168*12 =  2016    月冻结电能表失流数据
#define PIM_C2_F310   (PIM_C2_F309 + 2016)    //168*12 =  2016    月冻结电能表过流数据
#define PIM_C2_F311   (PIM_C2_F310 + 2016)    //129*12 =  1548    月冻结电能表潮流数据
#define PIM_C2_F312   (PIM_C2_F311 + 1548)    //168*12 =  2016    月冻结电能表断流数据
#define PIM_C2_F313   (PIM_C2_F312 + 2016)    //20*12 =  240    月冻结电能表掉电
#define PIM_C2_F212   (PIM_C2_F313 + 240)    //9*12 =  108    月冻结三相电压、电流不平衡度95%概率值
#define PIM_C2_F215   (PIM_C2_F212 + 108)     //41*12 = 492     月冻结够用点信息
#else
#define PIM_C2_F212   (PIM_C2_F39  + 144)    //9*12 =  108    月冻结三相电压、电流不平衡度95%概率值
#define PIM_C2_F215   (PIM_C2_F212 + 108)     //41*12 = 492     月冻结够用点信息
//#define PIM_C2_F216   (PIM_C2_F215 + 492)     //51*12 = 612     月冻结电能表结算信息
#endif


//终端数据文件FILEID_485_ERTU
//F49 日冻结终端日供电时间，日复位累计次数
//F50 日冻结终端日控制统计
//F53 日通信流量
#define PIM_ERTU_C2_F495053          0              //15*31 =  465       //年-月-日
//F51 月冻结月供电时间，月复位累计次数
//F52 月冻结终端月控制次数
//F54 终端与主站月通信流量                               6
#define PIM_ERTU_C2_F51F52F54        (PIM_ERTU_C2_F495053 + 465)              //15*12 =  180
//自己扩展的温度曲线数据，按照15分钟采集一次
#define PIM_ERTU_TEMPERATURE_CURVE   (PIM_ERTU_C2_F51F52F54 + 180)            //7字节*3天*96点 = 2016

//线损数据 2014.08.04 by rxp
#define PIM_ERTU_XLOST               (PIM_ERTU_TEMPERATURE_CURVE + 2016)      //30天*15[(线损率2 + 符号1)*4(总,A,B,C)+时间3]=450



//统计数据 , 按照测量点分块保存，每块内容安排如下
//FILEID_STAT

#ifdef __STAT_LOADRATE__
#define  PIM_STAT_C2_TRANSIT            0   //300*2=600  瞬时量日月统计
#define  PIM_STAT_C2_ZDXL             600   //24*2=48    最大需量日月统计 (分相)
#define  PIM_STAT_C2_ZDXL_FL_ZX       648   //76*2=152   正向最大需量日月统计（费率）
#define  PIM_STAT_C2_ZDXL_FL_FX       800   //76*2=152   反向最大需量日月统计（费率）
#define  PIM_STAT_C2_LOADRATE         952   //12*2=24    负载率日月统计

#define  LEN_STAT_C2_BLOCK            (944+24)

#else

#define  PIM_STAT_C2_TRANSIT            0   //296*2=592  瞬时量日月统计
#define  PIM_STAT_C2_ZDXL             592   //24*2=48    最大需量日月统计 (分相)
#define  PIM_STAT_C2_ZDXL_FL_ZX       640   //76*2=152   正向最大需量日月统计（费率）
#define  PIM_STAT_C2_ZDXL_FL_FX       792   //76*2=152   反向最大需量日月统计（费率）
#define  PIM_STAT_C2_LOADRATE         944   //12*2=24    负载率日月统计

#define  LEN_STAT_C2_BLOCK            944
#endif

//参数：F33 终端台区集中抄表运行参数设置
typedef union{
    struct{
    INT8U    port;          // 终端通信端口号	BIN	1	本次设置的
    INT8U    run_ctrl[2];   // 台区集中抄表运行控制字	BS16	2
    INT8U    rec_days[4];   // 抄表日-日期	BS32	4
    INT8U    rec_time[2];   // 抄表日-时间	数据格式19	2
    INT8U    cycle;         // 抄表间隔         BIN     1
    INT8U    cast_time[3];  // 对电表广播校时定时时间	数据格式18	3
    INT8U    seg_count;       // 抄表时段数量
    INT8U    rec_timeseg[96]; // 抄表时段信息   时-分 
    };
    struct{
      INT8U    tmp[13];            //其他
      INT8U    seg_count;          // 抄表时段数量
      INT8U    rec_timeseg[24][4]; //24个时段
    }seg;
}JCSET_F33;

#ifdef __PROVICE_HUBEI__
typedef struct{
    INT8U    rec_days[4];        // 抄表日-日期	BS32	4
    INT8U    seg_count;          // 抄表时段数量
    INT8U    rec_timeseg[24][4]; //24个时段
}JCSET_F801;

typedef union{
    INT8U value[4];  //做啥用？？
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
//参数： F33，  DL698_41_0928 版本定义
typedef struct{
    INT8U    port;            // 终端通信端口号	BIN	1	本次设置的
    INT8U    run_ctrl[2];     // 台区集中抄表运行控制字	BS16	2
    INT8U    rec_timeseg[12]; // 抄表时段信息  15分钟间隔，用96个位标记禁止或允许，1==禁止
    INT8U    rec_days[4];     // 抄表日-日期	BS32	4
    INT8U    rec_time[2];     // 抄表日-时间	数据格式19	2
    INT8U    cycle;           // 抄表间隔         BIN     1
    INT8U    cast_time[3];    // 对电表广播校时定时时间	数据格式18	3   
}JCSET_F33_DL698_41_0928;


//参数: 终端支持三个可以设置的抄表端口: 1--载波口   2--抄表485口   3--级联485口(如果设置为级联,则不可用)
typedef struct{
    JCSET_F33  port_set[4];
}JCSET_F33_ALL;


//数据格式14:  XXXXXX.XXXX
typedef struct{
     INT8U   value[5];
}DATAFMT_14;


//数据格式11:  XXXXXX.XX
typedef struct{
     INT8U   value[4];
}DATAFMT_11;


//数据格式15
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

//终端抄表时间
typedef  DATAFMT_15 REC_DATETIME;


typedef union{
    INT8U   value[10];
    struct{
        INT8U flag;              //0,对时开关：0：关闭；1：开启
        INT8U cycle;             //1,对时周期：每隔一个周期自动开始一次完整终端对时操作，单位是天。
        INT8U bcd_begin_minute;  //2,对时开始时间
        INT8U bcd_begin_hour;    //3,对时开始时间
        INT8U bcd_end_minute;    //4,对时结束时间
        INT8U bcd_end_hour;      //5,对时结束时间
        INT8U interval;          //6,对时最短间隔 单位：分钟
        INT8U max_count;         //7,允许对时次数
        INT8U error_threshold;   //8,时钟误差阈值 单位：秒
        INT8U power_max_delay;   //9,上电事件生成最大延时时间 单位：分钟
    };
}SET_F297; //__PRECISE_TIME__

typedef union{
    INT8U   value[8];
    struct{
        INT8U flag;              //0,对时开关：0：关闭；1：采用指定电表地址的广播对时命令；2：采用指定采集器的广播对时命令；3：采用全终端的广播对时命令；当选择全终端的广播对时命令时，根据时钟误差最大电表的误差情况决定是否对时。
        INT8U cycle;             //1,对时周期：每隔一个周期自动开始一次完整终端对时操作，单位是天。
        INT8U bcd_begin_minute;  //2,对时开始时间
        INT8U bcd_begin_hour;    //3,对时开始时间
        INT8U bcd_end_minute;    //4,对时结束时间
        INT8U bcd_end_hour;      //5,对时结束时间
        INT8U max_count;         //6,允许对时次数
        INT8U error_threshold;   //7,时钟误差阈值 单位：秒
    };
}SET_F298; //__PRECISE_TIME__


#define BATCH_TRANS_METER_TASK_FLAG_NO_EXEC             0     //有任务,未执行
#define BATCH_TRANS_METER_TASK_FLAG_CANT_EXEC           1     //不能执行
#define BATCH_TRANS_METER_TASK_FLAG_EXEC_NO_RESP        2     //已执行，未响应
#define BATCH_TRANS_METER_TASK_FLAG_FINISH              3     //执行完成
#define BATCH_TRANS_METER_TASK_FLAG_BEFORE_EXCEPTION    4     //前面任务回复异常应答
#define BATCH_TRANS_METER_TASK_FLAG_RESP_LONG           5     //执行完成,回复报文太长
#define BATCH_TRANS_METER_TASK_FLAG_TIME_OUT            6     //执行完成,超时

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
        INT8U task_level;   //0：保留；1：高于日冻结抄读；2：低于日冻结抄读
        BATCH_TRANSPARENT_METER_TASK_FALG ctrl_flag;  //D0-上报标志位  D1-抄读等待标志位  D2~D7备用
        #ifdef __BATCH_TRANSPARENT_METER_TASK_TMP1__
        INT8U vaild_minute[2];  // 0-不限制有效时长  其它：任务执行有效时长，单位为分钟
        #else
        INT8U vaild_minute;  // 0-不限制有效时长  其它：任务执行有效时长，单位为分钟
        #endif
        INT8U z_count;
        INT8U count;
        INT8U begin_time[6];
        INT8U end_time[6];
        INT8U flag;         //0xAA 有任务
    };
}BATCH_TRANSPARENT_METER_TASK_HEADER;       //__BATCH_TRANSPARENT_METER_TASK__

typedef union{
    INT8U value[4];
    struct{
        INT8U flag;  //FE:有任务；未执行  FC：不能执行  F8：执行完成
        INT8U frame_seq;
        INT8U req_frame_len;
        INT8U req_frame[BATCH_TRANSPARENT_METER_TASK_FRAME_LEN];
        INT8U resp_frame_len;
        INT8U resp_date[6];
        INT8U resp_frame[BATCH_TRANSPARENT_METER_TASK_FRAME_LEN];
    };
}BATCH_TRANSPARENT_METER_TASK;       //__BATCH_TRANSPARENT_METER_TASK__

//数据格式20
typedef union{
    INT8U value[3];
    struct{
        INT8U bcd_day;
        INT8U bcd_month;
        INT8U bcd_year;
    };
}DATAFMT_20;
//电表冻结时间，暂定安徽使用..
typedef DATAFMT_20  METER_DAYHOLD_TIME;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//          一类数据
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

//F170：集中抄表电表抄读信息
typedef union{
    INT8U  value[18];
    struct{
       INT8U           port;            //所属终端通信端口号
       INT8U           depth;           //中继路由级数
       DC1_PLC_PHASE   phase;           //载波抄读通信相位
       DC1_PLC_Q           plc_q;           //载波信号品质
       INT8U           rec_flag;        //最近一次抄表成功/失败标志
       DATAFMT_01      last_rec_date;   //最近一次抄表成功时间	数据格式01      6
       DATAFMT_01      last_fail_date;  //最近一次抄表失败时间	数据格式01	6
       INT8U           fail_count;      //最近连续失败累计次数
    };
}DC1_F170;

//5.12.2.4.10	F11：终端集中抄表状态信息
//本项数据块个数n  BIN 1
//    终端通信端口号      	BIN	1
//    要抄电表总数	        BIN	2
//    当前抄表工作状态标志	BS8	1
//    抄表成功块数	        BIN	2
//    抄重点表成功块数	        BIN	1
//    抄表开始时间	    数据格式1	6
//    抄表结束时间	    数据格式1	6

//当前抄表工作状态标志
typedef union{
   INT8U  value;
   struct{
      INT8U    recording:1;    //置"1"正在抄表，置"0" 未抄表
      INT8U    finish:1;       //置"1"时段内完成，置"0"时段内未完成
      INT8U    reserved:6;
   };
}LLVC_CURREC_FLAG;

//集中抄表状态
typedef  struct{
     INT8U             port;
     INT8U             total_count[2];
     LLVC_CURREC_FLAG  currec_flag;
     INT8U             read_count[2];
     INT8U             read_vip_count;
     DATAFMT_01        begin_datetime;
     DATAFMT_01        end_datetime;
}LLVC_REC_STATE ;

//一类 F11
typedef union{
  INT8U  value[39];
  struct{
     INT8U             port_count;    //恒取2
     LLVC_REC_STATE    rec_state[2];  //集中抄表状态,2个端口
  };
}DC1_F11;

typedef union{
    INT8U  value[7];
    struct{
        INT8U Item[4];      //事件代码
        INT8U Level;        //事件级别
        INT8U Data[2];       //CRC数据
    };
}METER_EVENT_F106;     //#ifdef __METER_EVENT_GRADE__

typedef union{
    INT16U  value;
    struct{
        INT16U offset:15;      //事件代码
        INT16U is_report:1;    //事件级别
    };
}METER_EVENT_REC_HEAD;     //#ifdef __METER_EVENT_GRADE__

//F50:当前剩余电量、剩余金额
typedef union{
   INT8U  value[8];
   struct{
     INT8U   left_amount[4];
     INT8U   left_fee[4];
   };
}DC1_F50;
#if !defined (__QGDW_376_2013_PROTOCOL__)
//F177: 当日电压合格率统计
typedef union{
   INT8U value[32];
   struct{
      INT8U  read_date[5];
      INT8U  mon_time[3];   //电压监测时间	数据格式10	分	3
      INT8U  ok_rate[3];    //电压合格率	数据格式28	%	3
      INT8U  over_rate[3];  //电压超限率	数据格式28	%	3
      INT8U  over_time[3];  //电压超上限时间	数据格式10	分	3
      INT8U  low_time[3];   //电压超下限时间	数据格式10	分	3
      INT8U  max_v[2];      //最高电压	数据格式5	V	2
      INT8U  max_v_time[4]; //最高电压出现时间	数据格式17		4
      INT8U  min_v[2];      //最低电压	数据格式5	V	2
      INT8U  min_v_time[4]; //最低电压出现时间	数据格式17		4
   };
   struct{
      INT8U  read_date[5];
      INT8U  meter_data[27];
   }d;
}DC1_F177;
#endif
//有功电能示值（总，费率1~M)
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

//无功电能示值
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

//一类数据当日F41,F42,F43,F44,F45,F46,F47,F48
typedef union{
    INT8U value[21];
    struct{
        INT8U   fl_number;              //费率数M
        INT8U   read_data[20];
    };
    struct{                             //当日
        INT8U   fl_number;              //费率数M
        INT8U   zx_ygz[5][4];           //正向有功电能量    总，费率1~M     数据格式A.13    5*4
    }d;
}DC1_F41, DC1_F42, DC1_F43, DC1_F44, DC1_F45, DC1_F46, DC1_F47, DC1_F48;


//载波表当前正向有功电能量示值数据
typedef union{
   INT8U  value[31];
   struct{
       REC_DATETIME   rec_datetime;
       INT8U          fl_count;
       DATAFMT_14     zxygz;
       DATAFMT_14     zxyg_fl[4];
   };
}PLCDATA_CUR_ZXYG;



//小时冻结类电能示值或电能量
typedef union{
    INT8U  value[6];
    struct{
       INT8U  td_h[2];  //小时冻结类时标
       INT8U  amount[4];
    };
}DC1_HOURHOLDAMOUNT;

//多功能电表失压实流反极性编程等告警存储数据结构
typedef union{
    INT8U  value[14];
    struct{
        INT8U  rec_date[5];  //年-月-日-时-分 BCD码
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
//          二类数据
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

//二类：F161：日冻结正向有功电能示值（总、费率1~M）
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

//二类：F177：月冻结正向有功电能示值（总、费率1~M）
typedef  DC2_F161  DC2_F177;

//二类：F179: 月冻结反向有功电能示值（总、费率1~M）
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

//二类  F162: 日冻结正向无功
typedef struct{
     REC_DATETIME   rec_datetime;
     INT8U          fl_count;
     INT8U          z[4];
     INT8U          fl[4][4];
}DC2_F162,DC2_F164,DC2_F165,DC2_F166,DC2_F167,DC2_F168;

//二类：F1：重庆测试需要

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

//F220: 购用电信息
typedef union{
   INT8U value[17];
   struct{
      INT8U  read_date[5];
      INT8U  buy_count[2];   //购电次数
      INT8U  left_buy[5];    //剩余金额
      INT8U  total_buy_fee[5];  //累计购电金额
   };
   struct{
      INT8U  read_date[5];
      INT8U  meter_data[12];
   }d;
}DC2_F220;


//1类数据F25
typedef union {
      INT8U  value[67];
      struct{
      unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年    
      unsigned char p_yg[12];      //当前总,A,B,C相有功功率  数据格式09  4*3
      unsigned char p_wg[12];      //当前总,A,B,C相无功功率  数据格式09  4*3
      unsigned char Q[8];          //当前总,A,B,C相功率因数  数据格式05  4*2
      unsigned char Voltage[6];    //当前A,B,C相电压  数据格式07   3*2
         unsigned char I[12];          //当前A,B,C,N相电流  数据格式25   4*3
         unsigned char p_uv[12];      //当前总,A,B,C相视在功率  数据格式09  4*3
     };
  }C1F25;

typedef struct{
    C1F25 cur_F25;
    C1F49 cur_F49;
    #ifdef __GB_645_YUNNAN__
    DC2_F96 c2F96;
    #endif
}C1SPORTCURVE;

//1类数据F26
typedef  union {
     INT8U  value[57];
     struct
     {
        INT8U read_date[5];     //终端抄表时间  数据格式15 分-时-日-月-年
      #if defined(__QGDW_376_2013_PROTOCOL__)
        INT8U V_BreakCount[12];  //总,A,B,C断相次数  4*3
      #else
        INT8U V_BreakCount[8];  //总,A,B,C断相次数  4*2
      #endif
        INT8U V_BreakTime[12];  //总,A,B,C断相累计时间 4*3
        INT8U V_BreakLastStart[16];  //总,A,B,C最后一次断相起始时间 4*4
        INT8U V_BreakLastStop[16];   //总,A,B,C最后一次断相结束时间 4*4
     };
   }C1F26;




   //1类数据F27
typedef union {
     INT8U  value[60];
     struct{
      unsigned char read_date[5];     //终端抄表时间  数据格式15 分-时-日-月-年    
      unsigned char meter_date[6];    //电能表日历时钟
       unsigned char battery_time[4];       //电池工作时间
       unsigned char program_count[3];      //编程次数
       unsigned char last_program_date[6];       //最近一次编程时间
       unsigned char zero_init_count[3];         //电表清零次数
       unsigned char last_zero_init_date[6];     //最近一次清零时间
       unsigned char xlclear_count[3];           //最大需量清零次数
       unsigned char last_xlclear_date[6];       //最近一次最大需量清零时间
       unsigned char event_init_count[3];        //事件清零次数
       unsigned char last_event_init_date[6];    //最近一次事件清零时间
       unsigned char adj_time_count[3];         //校时次数
       unsigned char last_adj_time_date[6];     //最近一次校时时间
     };
   }C1F27;


//一类数据F28
typedef union{
     INT8U   value[33];
     struct{
        unsigned char read_date[5];     //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char flag_chg[14];     //电表运行状态变位字1-7
        unsigned char status[14];       //电表运行状态字1-7
     };
}C1F28;

//1类数据F31,F32
typedef union{
     INT8U   value[59];
     struct{
          INT8U read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
          INT8U zxyg_A[5];     //A相正向有功电能示值
          INT8U fxyg_A[5];     //A相反向有功电能示值
          INT8U zhwg1_A[4];    //A相组合无功1电能示值
          INT8U zhwg2_A[4];    //A相组合无功2电能示值
          INT8U zxyg_B[5];     //B相正向有功电能示值
          INT8U fxyg_B[5];     //B相反向有功电能示值
          INT8U zhwg1_B[4];    //B相组合无功1电能示值
          INT8U zhwg2_B[4];    //B相组合无功2电能示值
          INT8U zxyg_C[5];     //C相正向有功电能示值
          INT8U fxyg_C[5];     //C相反向有功电能示值
          INT8U zhwg1_C[4];    //C相组合无功1电能示值
          INT8U zhwg2_C[4];    //C相组合无功2电能示值
     }b;
}C1F31,C1F32;

//1类数据F29,F30
typedef union{
     INT8U   value[13];
     struct{
          INT8U read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
          INT8U copper_loss[5];     //A相正向有功电能示值
          INT8U iron_loss[5];     //A相反向有功电能示值
     }b;
}C1F29,C1F30;

//1类数据F33,F34,F37,F38
typedef union {
      INT8U  value[91];
      struct{
      unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年    
      unsigned char fl_number;     //费率数M
      unsigned char zx_ygz[25];    //正向有功总电能示值  总，费率1~M   数据格式14    5*5
      unsigned char zx_wgz[20];    //正向无功总电能示值  总，费率1~M   数据格式11    5*4
      unsigned char zx_wg1[20];    //正向1象限无功总电能示值  总，费率1~M   数据格式11    5*4
      unsigned char zx_wg4[20];    //正向4象限无功总电能示值  总，费率1~M   数据格式11    5*4   
      };
      struct{
        unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;     //费率数M
        unsigned char zx_ygz[5][5];    //正向有功总电能示值  总，费率1~M   数据格式14    5*5
        unsigned char zx_wgz[5][4];    //正向无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg1[5][4];    //正向1象限无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg4[5][4];    //正向4象限无功总电能示值  总，费率1~M   数据格式11    5*4
      }d;
      struct{
        unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;     //费率数M = 1
        unsigned char zx_ygz[10];    //正向有功总电能示值  总，费率1~M   数据格式14    5*5
        unsigned char zx_wgz[8];    //正向无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg1[8];    //正向1象限无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg4[8];    //正向4象限无功总电能示值  总，费率1~M   数据格式11    5*4
      }d1;
      struct{
        unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;     //费率数M = 2
        unsigned char zx_ygz[15];    //正向有功总电能示值  总，费率1~M   数据格式14    5*5
        unsigned char zx_wgz[12];    //正向无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg1[12];    //正向1象限无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg4[12];    //正向4象限无功总电能示值  总，费率1~M   数据格式11    5*4
      }d2;
      struct{
        unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;     //费率数M = 3
        unsigned char zx_ygz[20];    //正向有功总电能示值  总，费率1~M   数据格式14    5*5
        unsigned char zx_wgz[16];    //正向无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg1[16];    //正向1象限无功总电能示值  总，费率1~M   数据格式11    5*4
        unsigned char zx_wg4[16];    //正向4象限无功总电能示值  总，费率1~M   数据格式11    5*4
      }d3;
      struct{
        unsigned char meter_no[6];
        unsigned char read_date[5];
        unsigned char fl_count;
        unsigned char value[20];
      }f;  //载波表的其他日冻结数据头信息
}C1F33,C1F34,C1F37,C1F38;


typedef union {
      INT8U  value[86];
      struct{
      unsigned char read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年    
      unsigned char fl_number;     //费率数M
      unsigned char zx_yg[20];    //正向有功总电能示值  总，费率1~M   数据格式14    5*4
      unsigned char zx_wg[20];    //正向无功总电能示值  总，费率1~M   数据格式11    5*4
      unsigned char fx_yg[20];    //正向1象限无功总电能示值  总，费率1~M   数据格式11    5*4
      unsigned char fx_wg[20];    //正向4象限无功总电能示值  总，费率1~M   数据格式11    5*4   
      };
}ZFYW_FL;


//交采每分钟数据缓存
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

//1类数据F35,F36
typedef  union{
      INT8U  value[76];
      struct{
      unsigned char read_date[5];      //终端抄表时间  数据格式15 分-时-日-月-年    
      unsigned char fl_number;         //费率数M
      unsigned char zx_ygxl[15];       //正向有功总最大需量  总，费率1~M   数据格式23    5*3
      unsigned char zx_ygxl_date[20];  //正向有功总最大需量发生时间  总，费率1~M   数据格式17    5*4
      unsigned char zx_wgxl[15];       //正向无功总最大需量  总，费率1~M   数据格式23    5*3
      unsigned char zx_wgxl_date[20];  //正向无功总最大需量发生时间  总，费率1~M   数据格式17    5*4
      };
    struct{
        unsigned char read_date[5];      //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;         //费率数M = 1
        unsigned char zx_ygxl[6];       //正向有功总最大需量  总，费率1~M   数据格式23    5*3
        unsigned char zx_ygxl_date[8];  //正向有功总最大需量发生时间  总，费率1~M   数据格式17    5*4
        unsigned char zx_wgxl[6];       //正向无功总最大需量  总，费率1~M   数据格式23    5*3
        unsigned char zx_wgxl_date[8];  //正向无功总最大需量发生时间  总，费率1~M   数据格式17    5*4
      }d1;
    struct{
        unsigned char read_date[5];      //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;         //费率数M = 2
        unsigned char zx_ygxl[9];       //正向有功总最大需量  总，费率1~M   数据格式23    5*3
        unsigned char zx_ygxl_date[12];  //正向有功总最大需量发生时间  总，费率1~M   数据格式17    5*4
        unsigned char zx_wgxl[9];       //正向无功总最大需量  总，费率1~M   数据格式23    5*3
        unsigned char zx_wgxl_date[12];  //正向无功总最大需量发生时间  总，费率1~M   数据格式17    5*4
      }d2;
    struct{
        unsigned char read_date[5];      //终端抄表时间  数据格式15 分-时-日-月-年
        unsigned char fl_number;         //费率数M = 3
        unsigned char zx_ygxl[12];       //正向有功总最大需量  总，费率1~M   数据格式23    5*3
        unsigned char zx_ygxl_date[16];  //正向有功总最大需量发生时间  总，费率1~M   数据格式17    5*4
        unsigned char zx_wgxl[12];       //正向无功总最大需量  总，费率1~M   数据格式23    5*3
        unsigned char zx_wgxl_date[16];  //正向无功总最大需量发生时间  总，费率1~M   数据格式17    5*4
      }d3;
}C1F35,C1F36;

//1类：F145-F152  最大需量数据结构
typedef union{
   INT8U  value[7];
   struct{
       INT8U  zdxl[3];     //A.23
       INT8U  xl_time[4];  //分时日月
   };
}DC1_ZDXL;
typedef union{
    INT8U  value[41];
    struct{
       INT8U  read_date[5];
       INT8U  fl_count;
       DC1_ZDXL  zdxl[5];   //总，费率1，2，3，4
    };
}DC1_F145,DC1_F146,DC1_F147,DC1_F148,DC1_F149,DC1_F150,DC1_F151,DC1_F152;


//1类F161：电能表远程控制通断电状态及记录
#if defined (__QGDW_376_2013_PROTOCOL__)
typedef union{
      INT8U  value[18];
      struct{
         INT8U    read_date[5];        //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    meter_switch_state;  //电表通断电状态
         INT8U    last_ctrl_on_time[6]; //最近1次控制通电时间
         INT8U    last_ctrl_off_time[6]; //最近1次控制断电时间
      }b;
}DC1_F161,DC2_F209;
#else
typedef union{
      INT8U  value[16];
      struct{
         INT8U    read_date[5];        //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    meter_switch_state;  //电表通断电状态
         INT8U    last_ctrl_on_time[5]; //最近1次控制通电时间
         INT8U    last_ctrl_off_time[5]; //最近1次控制断电时间
      }b;
}DC1_F161,DC2_F209;
#endif

#if defined (__QGDW_376_2013_PROTOCOL__)
//1类F162,电能表日历时钟
typedef union{
      INT8U  value[12];
      struct{
         INT8U    read_date[6];        //终端抄表时间  数据格式15 秒-分-时-日-月-年
         INT8U    meter_time[6];       //电能表日历时钟
      }b;
}DC1_F162;
#endif

//1类F165：电能表开关操作次数及时间
#if defined (__QGDW_376_2013_PROTOCOL__)
typedef union{
      INT8U  value[23];
      struct{
         INT8U    read_date[5];         //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    op_count[3];          //编程开关操作次数  FMT08
         INT8U    last_prog_time[6];    //最近1次编程开关操作时间
         INT8U    endcap_open_count[3];  //尾盖打开次数
         INT8U    endcap_open_time[6];    //最近1次尾盖打开时间
		 #ifdef __PROVICE_SHANXI__
         INT8U    meter_open_count[3];  //电能表表盖打开次数
         INT8U    last_open_time[6];    //最近一次表盖打开时间
		 #endif
      }b;
}DC1_F165,DC2_F213;
#else
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];         //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    op_count[2];          //编程开关操作次数  FMT08
         INT8U    last_prog_time[5];    //最近1次编程开关操作时间
         INT8U    endcap_open_count[2];  //尾盖打开次数
         INT8U    endcap_open_time[5];    //最近1次尾盖打开时间
      }b;
}DC1_F165,DC2_F213;
#endif
//1类F166：电能表参数修改次数及时间
#if defined (__QGDW_376_2013_PROTOCOL__)
typedef union{
      INT8U  value[29];
      struct{
         INT8U    read_date[5];              //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    op_time_count[3];          //时钟修改次数  FMT08
         INT8U    op_before_time[6];         //校时前时间
         INT8U    op_after_time[6];          //校时后时间
         INT8U    op_timeseg_count[3];       //时段参数修改次数
         INT8U    last_op_timeseg_time[6];   //最近1次时段参数修改时间
      }b;
}DC1_F166,DC2_F214;
#else
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];              //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    op_time_count[2];          //时钟修改次数  FMT08
         INT8U    last_op_time[5];           //最近1次时钟修改时间
         INT8U    op_timeseg_count[2];       //时段参数修改次数
         INT8U    last_op_timeseg_time[5];   //最近1次时段参数修改时间
      }b;
}DC1_F166,DC2_F214;
#endif
//1类F167： 电能表购用电信息
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];               //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    buy_count[2];               //购电次数      FMT08
         INT8U    left_fee[5];                //剩余金额      FMT14
         INT8U    total_buy_fee[5];           //累计购电金额  FMT14
         INT8U    left_amount[4];             //剩余电量      FMT11
         INT8U    overuse_amount[4];          //透支电量      FMT11
         INT8U    total_buy_amount[4];        //累计购电量    FMT11
         INT8U    canoveruse_amount[4];       //赊欠门限电量      FMT11
         INT8U    alarm_amount[4];            //报警电量    FMT11
         INT8U    err_amount[4];              //故障电量      FMT11
      }b;
     struct{
     REC_DATETIME   rec_datetime;
     INT8U meter_data[36];
     };
}DC1_F167,DC2_F210;

//1类F168： 电能表结算信息
typedef union{
      INT8U  value[19];
      struct{
         INT8U    read_date[5];         //终端抄表时间  数据格式15 分-时-日-月-年
         INT8U    fl_count;             //费率个数
         INT8U    billed_amount[5][5];  //已结有功总，费率1-4电能    FMT14
         INT8U    unbill_amount[5][5];  //未结有功总，费率1-4电能
      }b;
}DC1_F168;


typedef struct{
     unsigned int eeAddr_Data;
     unsigned int eeAddr_Dest;
     unsigned int datavaluelen;
     unsigned int holds_dens;
     unsigned char Td_Flag;
     unsigned char fn_idx;   //冻结标识序号
     unsigned char datavalue[20];
    }C1Hold;




//终端供电统计,如果date为3，结构体的尺寸会变为8，而不是7，所以date使用2字节，月-日
typedef union{
     INT8U  vlaue[6];
     struct{
        INT16U  power_time;
        INT16U  reset_count;
        INT8U   date[2];
     };
}STAT_ERTU_PWR;

//统计限值设定值
typedef struct{
   INT16U   v_maxmax;        //电压上上限
   INT16U   v_max;
   INT16U   v_min;
   INT16U   v_minmin;
   INT32U   i_maxmax;
   INT32U   i_max;
   INT32U   in_max;
   INT32U   s_maxmax;
   INT32U   s_max;
   INT16U   u_unb_max;       //电压不平衡限值
   INT16U   i_unb_max;
   INT16U   v_lost;		    //断相门限

   INT16S   v_maxmax_restore_coff;       //电压上上限越限恢复系数
   INT16S   v_minmin_restore_coff;       //电压下下限越限恢复系数
   INT16S   i_maxmax_restore_coff;       //电流上上限越限恢复系数
   INT16S   i_max_restore_coff;       //电流上限越限恢复系数
   INT16S   in_max_restore_coff;       //零序电流上限越限恢复系数
   INT16S   u_unb_restore_coff;       //电压不平衡越限恢复系数
   INT16S   i_unb_restore_coff;       //电流不平衡越限恢复系数
   INT16S    s_maxmax_restore_coff;       //视在功率越上上限恢复系数
   INT16S    s_max_restore_coff;       //视在功率越上限恢复系数

   INT8U    v_maxmax_last_time;          //电压上上限越限持续时间
   INT8U    v_minmin_last_time;          //电压下下限越限持续时间
   INT8U    i_maxmax_last_time;          //电流上上限越限持续时间
   INT8U    i_max_last_time;             //电流上限越限持续时间
   INT8U    in_max_last_time;            //零序电流上限越限持续时间
   INT8U    u_unb_last_time;             //电压不平衡越限持续时间
   INT8U    i_unb_last_time;             //电流不平衡越限持续时间
   INT8U    s_maxmax_last_time;    //视在功率越上上限持续时间
   INT8U    s_max_last_time;       //视在功率越上限持续时间
   INT8U    v_lost_last_time;      //连续失压时间限值

}STAT_SET;


//越限检查器,用于需要一个持续时间才能判定发生或恢复的越限类型
typedef struct{
    INT8U  over_last_time;       //越限持续时间
    #ifdef __PROVICE_JIBEI_CHECK__
    struct{
        INT8U  over_occur_flag:1;      //越限发生标志
        INT8U  over_occur_start:1;     //刚开始发生
        INT8U  res:6;
        };
    #else
    INT8U  over_occur_flag;      //越限发生标志
    #endif
    INT8U  not_over_last_time;   //不越限持续时间
    INT8U  over_restore_flag;    //越限恢复标志 
}tagOverMaxChecker;

//电压越限统计值
typedef union{
    INT8U  value[10];
    struct{
      INT16U v_over_maxmax_time;   //越电压上上限时间
      INT16U v_over_minmin_time;   //越电压下下限时间
      INT16U v_over_max_time;
      INT16U v_over_min_time;
      INT16U v_ok_time;      //电压合格时间

      //-------------限值检查变量-------------
      tagOverMaxChecker   chk_over_maxmax;     //越上上限检查器
      tagOverMaxChecker   chk_over_max;        //越上限检查器
      tagOverMaxChecker   chk_over_minmin;     //越下下限检查器
    };
}STAT_V_OVER;

//电压最大最小值统计
typedef union{
    INT8U  value[12];
    struct{
        INT16U  v_max;
        INT8U   v_max_time[4];   //分-时-日-月
        INT16U  v_min;
        INT8U   v_min_time[4];
    };
}STAT_V_MAXMIN;


//电压统计数据
typedef union
{
   INT8U  value[78];
   struct{
      STAT_V_OVER    v_over_time[3];   //A,B,C 三相 电压越限累计时间
      STAT_V_MAXMIN  v_maxmin[3];      //A,B,C 三相 极值统计
      INT16U         v_avg[3];         //A,B,C 三相电压平均值 (转为日冻结数据前，存放计算次数)
      INT32U         v_sum[3];         //存放A,B,C三相电压的累加和。

   };
}STAT_V;

//不平衡统计数据
typedef union
{
   INT8U  value[16];
   struct
   {
       INT16U         i_unbalance_time; //电流不平衡累计时间
       INT16U         v_unbalance_time; //电压不平衡累计时间
       INT16U         i_unbalance_max;          //电流不平衡最大值
       INT8U          i_unbalance_max_time[4];  //分-时-日-月
       INT16U         u_unbalance_max;          //电压不平衡最大值
       INT8U          u_unbalance_max_time[4];  //分-时-日-月

      //-------------限值检查变量-------------
      tagOverMaxChecker   chk_over_max_u;     //（电压）越限检查器
            //-------------限值检查变量-------------
      tagOverMaxChecker   chk_over_max_i;     //（电流）越限检查器
   };
}STAT_UNBALANCE;


//电流越限
typedef union
{
   INT8U value[4];
   struct{
      INT16U   i_over_maxmax_time;
      INT16U   i_over_max_time;

      //-------------限值检查变量-------------
      tagOverMaxChecker   chk_over_maxmax;     //越上上限检查器
            //-------------限值检查变量-------------
      tagOverMaxChecker   chk_over_max;        //越上限检查器
   };
}STAT_I_OVER;


//电流极值
typedef union{
    INT8U  value[8];
    struct{
        INT32U  i_max;
        INT8U   i_max_time[4];   //分-时-日-月
    };
}STAT_I_MAX;

//电流越限统计
typedef union{
     INT8U  value[34];
     struct{
       STAT_I_OVER  i_over_time[4];   //A,B,C,N 电流越限时间
       STAT_I_MAX   i_max[4];        //A,B,C,N 极值统计
     };
}STAT_I;



//2类： 冻结电压统计数据
typedef union{
    INT8U  value[66];
    struct{
       INT8U  va_over_time[10];   //A相电压越上上限累计时间，越下下限累计时间，越上限累计时间，越下限累计时间，合格时间
       INT8U  vb_over_time[10];
       INT8U  vc_over_time[10];
       INT8U  va_max[2];          //A相电压最大值
       INT8U  va_max_time[3];     //A相电压最大值发生时间  分时日
       INT8U  va_min[2];          //A相电压最小值
       INT8U  va_min_time[3];     //A相电压最小值发生时间  分时日
       INT8U  vb_max[2];
       INT8U  vb_max_time[3];
       INT8U  vb_min[2];
       INT8U  vb_min_time[3];
       INT8U  vc_max[2];
       INT8U  vc_max_time[3];
       INT8U  vc_min[2];
       INT8U  vc_min_time[3];
       INT8U  v_avg[6];           //A相平均电压，B相平均电压，C相平均电压
       #if defined (__QGDW_376_2013_PROTOCOL__) || defined (__QGDW2013_VRATE__)
       INT8U  v_rate[27];
       #endif
    };
}C2_F27F35;

typedef struct{
    INT8U rec_date[5];
    C2_F27F35 c2f27f35;
}C2_F27F35_PLC;

//2类：冻结不平衡越限数据
typedef union{
   INT8U value[16];
   struct{
      INT8U   i_over_time[2];  //电流不平衡越限时间
      INT8U   u_over_time[2];  //电压不平衡
      INT8U   i_max[2];         //电流不平衡最大值
      INT8U   i_max_time[4];
      INT8U   u_max[2];        //电压不平衡最大值
      INT8U   u_max_time[4];
   };
}C2_F28F36;

typedef union{
   INT8U value[14];
   struct{
      INT8U   i_over_time[2];  //电流不平衡越限时间
      INT8U   u_over_time[2];  //电压不平衡
      INT8U   i_max[2];         //电流不平衡最大值
      INT8U   i_max_time[3];
      INT8U   u_max[2];        //电压不平衡最大值
      INT8U   u_max_time[3];
   };
}C2_F28;

//2类：冻结电流越限数据
typedef union{
      INT8U value[38];
      struct{
         INT8U i_overtime[14];  //A,B,C越上上限，越上限累计时间，N 越上限累计时间
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


//2类：日负载率统计
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





//2类：冻结视在功率越限累计时间
typedef union{
    INT8U  value[4];
    struct{
        INT8U  over_maxmax[2];
        INT8U  over_max[2];
    };
    struct{
        INT16U  s_over_maxmax;            //越上上限累计时间
        INT16U  s_over_max;               //越上限累计时间

        //-------------用于统计的变量-------------
        tagOverMaxChecker   chk_over_maxmax;   //越上上限检查器
        tagOverMaxChecker   chk_over_max;      //越上限检查器
    };
}C2_F30F38;

//2类  电容器投入累计时间和次数
typedef union{
   INT8U  value[72];
   struct{
     INT8U   run_time[9][4];   //第1-9组电容器累计投入时间
     INT8U   run_count[9][4];  //第1-9组电容器累计投入次数
   };
}C2_F41,C1_F66;

//2类  日月电容累计补偿无功电能量
typedef union{
    INT8U value[8];
    struct{
        INT8U  day_wg_cap[4];
        INT8U  month_wg_cap[4];
    };
}C2_F42,C1_F67;

//2类：冻结功率因数区段累计时间
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

//2类：冻结总及分相最大有功功率及发生时间
typedef union{
    INT8U  value[6];
    struct{
        INT8U  p_max[3];
        INT8U  p_max_time[3]; //分时日
    };
}C2_F25F33_PYG;
typedef union{
    INT8U   value[32];
    struct{
        C2_F25F33_PYG   pyg_max[4];       //总，A,B,C最大有功功率及发生时间
        INT16U          pyg_zero_time[4]; //总，A,B,C功率为零累计时间
    };
}C2_F25F33;

//2类：冻结总及分相有功最大需量及发生时间
// 这个统计数据单独存放，并且只有在变化时才考虑写入。
typedef union{
    INT8U  value[24];
    struct{
      C2_F25F33_PYG  xl_max[4];
    };
}C2_F26F34;


//2类，日冻结总加组日最大，最小有功功率及其发生时间，有功功率为零日累计时间
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

//2类，日，月冻结直流模拟量越限累计时间，最大最小值发生时间
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

//2类：F153
typedef union{
     INT8U value[20];
     struct{
          INT8U read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
          INT8U yg_a[5];      //A,B,C有功
          INT8U yg_b[5];
          INT8U yg_c[5];
     };
}C2_F153,C2_F155,C2_F157,C2_F159;

//2类：F154
typedef union{
     INT8U value[17];
     struct{
          INT8U read_date[5];  //终端抄表时间  数据格式15 分-时-日-月-年
          INT8U wg_a[4];      //A,B,C无功
          INT8U wg_b[4];
          INT8U wg_c[4];        
     };
}C2_F154,C2_F156,C2_F158,C2_F160;

//2类： 冻结最大需量及发生时间
typedef union{
     INT8U  value[41];
     struct{
        INT8U   read_date[5];
        INT8U   fl_count;
        INT8U   zdxl[35];
     };
}C2_F185_188;

//测量点事件标志
typedef union{
     INT8U  value;
     struct{
        INT8U   v_over_maxmax:1;        //电压越上上限   当前状态
        INT8U   v_over_maxmax_evt:1;    //电压越上上限   上次标志
        INT8U   v_over_minmin:1;        //电压越下下限   当前状态
        INT8U   v_over_minmin_evt:1;    //电压越下下限   上次标志
        INT8U   v_lost:1;               //断相           当前状态
        INT8U   v_lost_evt:1;           //断相           上次标志
        INT8U   v_low:1;                //失压           当前状态
        INT8U   v_low_evt:1;            //失压           上次标志
     };
}SPOT_V_EVENT_FLAG;


typedef union{
     INT8U  value;
     struct{
        INT8U   i_over_maxmax:1;        //电流越上上限   当前状态
        INT8U   i_over_maxmax_evt:1;    //电流越上上限   上次标志
        INT8U   i_over_max:1;           //电流越上限     当前状态
        INT8U   i_over_max_evt:1;       //电流越上限     上次标志
     };
}SPOT_I_EVENT_FLAG;

typedef union{
       INT8U  value;
       struct{
        INT8U   s_over_maxmax:1;        //视在功率越上上限    当前状态
        INT8U   s_over_maxmax_evt:1;    //视在功率越上上限    上次标志
        INT8U   s_over_max:1;           //视在功率越上限       当前状态
        INT8U   s_over_max_evt:1;       //视在功率越上限      上次标志
        INT8U   v_unbalance:1;          //电压不平衡          当前状态
        INT8U   v_unbalance_evt:1;      //电压不平衡          上次标志
        INT8U   i_unbalance:1;          //电流不平衡         当前状态
        INT8U   i_unbalance_evt:1;      //电流不平衡         上次标志
       };
}SPOT_S_UNB_EVENT_FLAG;


typedef union{
      INT8U  value[4];
      struct{
         SPOT_V_EVENT_FLAG      phase_v[3];  //a,b,c三相的电压事件状态
         SPOT_I_EVENT_FLAG      phase_i[3];   //a,b,c三相的电流事件状态,还有零序电流，但是没生成事件
         SPOT_S_UNB_EVENT_FLAG   s_unb;        //视在功率及不平衡
      };
}SPOT_EVENT_FLAG;

//测量点统计数据文件：
//
//测量点瞬时量统计数据
typedef union{
      INT8U  value[296];
      struct{
         INT8U            stat_date[4]; //统计日期，BIN 时-日-月-年
         STAT_V           stat_v;    //电压统计
         STAT_I           stat_i;    //电流统计
         STAT_UNBALANCE   stat_unb;  //不平衡统计
         C2_F30F38        stat_s;    //视在功率统计
         C2_F43F44        stat_q;    //功率因数分段统计
         C2_F25F33        stat_pyg;  //总及分相最大有功功率及发生时间
         SPOT_EVENT_FLAG  event_flag;  //事件生成标志
         #ifdef __STAT_LOADRATE__
         C2_F31F39        loadrate;    //负载率统计
         #else
         INT8U            reserved[8];
         #endif
      };
      SET_F26             set_f26;
}SPOT_STAT;

//2类： F101 F102 F103 F104  测量点正反向有无功总电能示值曲线
//      F145,F146,F147,F148  四象限无功电能示值曲线
typedef union{
     INT8U  value[53];
     struct{
        INT8U  read_date[5];
        INT8U  zxygz[4];
        INT8U  zxwgz[4];
        INT8U  fxygz[4];
        INT8U  fxwgz[4];
        INT8U  zxyg_fl[16];   //正向有功电能 费率1，2，3，4
        INT8U  wg1[4];  //一象限无功
        INT8U  wg2[4];  //二象限无功
        INT8U  wg3[4];  //三象限无功
        INT8U  wg4[4];  //四象限无功
     };
}C2_SPOT_CURVE_ZFXYWG;

// 测量点电能量数据
typedef union{
    INT8U  value[21];
    struct{
       INT8U  fl_count;
       INT8U  amount[20];
    };
}SPOT_AMOUNT;



//终端的日月统计数据保存数据
typedef union{
   INT8U   value[15];
   struct{
      INT8U  date[3];
      INT8U  F49[4];
      INT8U  F50[4];
      INT8U  F53[4];
   }d;
   struct{
      INT8U  year;      //HEX格式，
      INT8U  month;
      INT8U  day;
      INT8U  F51[4];
      INT8U  F52[4];
      INT8U  F54[4];
   }e;
}C2_ERTU_DATA;



//F65: 当前电容器投切状态
typedef union{
     INT8U value[3];
     struct{
         INT8U  run_mode;        //BS8 运行方式
         INT8U  run_state[2];    //BS16 电容器投切状态
     };
}C1_CAP_RUNSTATE;


//F66: 当前电容器累计补偿投入时间和次数
typedef union{
    INT8U value[72];
    struct{
       INT32U    run_time[9];    //第1-9组累计投入时间，单位：秒
       INT32U    run_count[9];   //第1-9组电容器累计投入次数.
    };
}C1_CAP_RUNTIME;

//F67: 当日，当月电容器累计补偿的无功电能量
typedef union{
     INT8U  value[8];
     struct{
        INT8U   day_wg[4];        //当日补偿的无功电能量    XXXX.XXXX  KVar
        INT8U   month_wg[4];      //当月补偿的无功电能量
     };
}C1_CAP_WG;

typedef union{
    INT8U  value[5];
    struct{
       INT8U  read_date[5];
       INT8U  qdcs[2];
           };
}C1_F187;

//F162：请求三相电能表最近十次失压数据   //广西扩展
/*typedef union{
     INT8U  value[121];
     struct{
        INT8U   count;        //本次记录次数
        INT8U   lost_vi[10][3];      //失压累计次数   z a b c
     };
     struct{
        INT8U   count;        //本次记录次数
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
//掉电记录
typedef union{
   INT8U value[103];
   struct{
     INT8U           count[3];
     POWEROFF_RECORD record[10];
   };
}C1_F246,C2_F246;

//噪声曲线
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
     INT8U    read_date[5];    //年-月-日-时-分
     INT8U    white_noise[3];
     C2_F218   color_noise[3]; 
   }b;
}C2_PLC_NOISE;

typedef union{
    INT8U  value[29];
    struct{
        INT8U rec_date[5];       //年-月-日-时-分 BCD码
        INT8U under_v_z[3];      //总欠压次数
        INT8U under_v_a[3];      //A相欠压总次数
        INT8U under_v_b[3];      //B相欠压总次数
        INT8U under_v_c[3];      //C相欠压总次数
        INT8U under_v_z_time[3]; //欠相累计时间
        INT8U under_v_a_time[3]; //A欠压累计时间
        INT8U under_v_b_time[3]; //B欠压累计时间
        INT8U under_v_c_time[3]; //C欠压累计时间
    };
}C2_F300;

typedef union{
    INT8U  value[168];
    struct{
        INT8U rec_date[5];       //年-月-日-时-分 BCD码
        INT8U lost_i_z[3];       //失流相次数
        INT8U lost_i_a[3];       //A相失流总次数
        INT8U lost_i_b[3];       //B相失流总次数
        INT8U lost_i_c[3];       //C相失流总次数
        INT8U lost_i_z_time[3];  //失流累计时间
        INT8U lost_i_a_time[3];  //A失流累计时间
        INT8U lost_i_b_time[3];  //B失流累计时间
        INT8U lost_i_c_time[3];  //C失流累计时间

        INT8U lost_i_a_begin[6]; //(上1次)A相失流发生时刻
        INT8U lost_i_a_end[6];   //(上1次)A相失流结束时刻
        INT8U lost_i_z_zxyg[4];  //失流期间正向有功总电能增量
        INT8U lost_i_z_fxyg[4];  //失流期间反向有功总电能增量
        INT8U lost_i_z_zxwg[4];  //失流期间组合无功1总电能增量
        INT8U lost_i_z_fxwg[4];  //失流期间组合无功2总电能增量
        INT8U lost_i_a_zxyg[4];  //失流期间A相正向有功电能增量
        INT8U lost_i_a_fxyg[4];  //失流期间A相反向有功电能增量
        INT8U lost_i_a_zxwg[4];  //失流期间A相组合无功1电能增量
        INT8U lost_i_a_fxwg[4];  //失流期间A相组合无功2电能增量
        INT8U lost_i_a_v[2];     //失流时刻A相电压
        INT8U lost_i_a_i[3];     //失流时刻A相电流
        INT8U lost_i_a_yggl[3];  //失流时刻A相有功功率
        INT8U lost_i_a_wggl[3];  //失流时刻A相无功功率
        INT8U lost_i_a_glys[2];  //失流时刻A相功率因数

        INT8U lost_i_b_begin[6]; //(上1次)B相失流发生时刻
        INT8U lost_i_b_end[6];   //(上1次)B相失流结束时刻
        INT8U lost_i_b_zxyg[4];  //失流期间B相正向有功电能增量
        INT8U lost_i_b_fxyg[4];  //失流期间B相反向有功电能增量
        INT8U lost_i_b_zxwg[4];  //失流期间B相组合无功1电能增量
        INT8U lost_i_b_fxwg[4];  //失流期间B相组合无功2电能增量
        INT8U lost_i_b_v[2];     //失流时刻B相电压
        INT8U lost_i_b_i[3];     //失流时刻B相电流
        INT8U lost_i_b_yggl[3];  //失流时刻B相有功功率
        INT8U lost_i_b_wggl[3];  //失流时刻B相无功功率
        INT8U lost_i_b_glys[2];  //失流时刻B相功率因数

        INT8U lost_i_c_begin[6]; //(上1次)C相失流发生时刻
        INT8U lost_i_c_end[6];   //(上1次)C相失流结束时刻
        INT8U lost_i_c_zxyg[4];  //失流期间C相正向有功电能增量
        INT8U lost_i_c_fxyg[4];  //失流期间C相反向有功电能增量
        INT8U lost_i_c_zxwg[4];  //失流期间C相组合无功1电能增量
        INT8U lost_i_c_fxwg[4];  //失流期间C相组合无功2电能增量
        INT8U lost_i_c_v[2];     //失流时刻C相电压
        INT8U lost_i_c_i[3];     //失流时刻C相电流
        INT8U lost_i_c_yggl[3];  //失流时刻C相有功功率
        INT8U lost_i_c_wggl[3];  //失流时刻C相无功功率
        INT8U lost_i_c_glys[2];  //失流时刻C相功率因数
    };
}C2_F301;

typedef union{
    INT8U  value[129];
    struct{
        INT8U rec_date[5];       //年-月-日-时-分 BCD码
        INT8U lost_i_z[3];       //失流相次数
        INT8U lost_i_a[3];       //A相失流总次数
        INT8U lost_i_b[3];       //B相失流总次数
        INT8U lost_i_c[3];       //C相失流总次数
        INT8U lost_i_z_time[3];  //失流累计时间
        INT8U lost_i_a_time[3];  //A失流累计时间
        INT8U lost_i_b_time[3];  //B失流累计时间
        INT8U lost_i_c_time[3];  //C失流累计时间

        INT8U lost_i_a_begin[6]; //(上1次)A相失流发生时刻
        INT8U lost_i_a_end[6];   //(上1次)A相失流结束时刻
        INT8U lost_i_z_zxyg[4];  //失流期间正向有功总电能增量
        INT8U lost_i_z_fxyg[4];  //失流期间反向有功总电能增量
        INT8U lost_i_z_zxwg[4];  //失流期间组合无功1总电能增量
        INT8U lost_i_z_fxwg[4];  //失流期间组合无功2总电能增量
        INT8U lost_i_a_zxyg[4];  //失流期间A相正向有功电能增量
        INT8U lost_i_a_fxyg[4];  //失流期间A相反向有功电能增量
        INT8U lost_i_a_zxwg[4];  //失流期间A相组合无功1电能增量
        INT8U lost_i_a_fxwg[4];  //失流期间A相组合无功2电能增量

        INT8U lost_i_b_begin[6]; //(上1次)B相失流发生时刻
        INT8U lost_i_b_end[6];   //(上1次)B相失流结束时刻
        INT8U lost_i_b_zxyg[4];  //失流期间B相正向有功电能增量
        INT8U lost_i_b_fxyg[4];  //失流期间B相反向有功电能增量
        INT8U lost_i_b_zxwg[4];  //失流期间B相组合无功1电能增量
        INT8U lost_i_b_fxwg[4];  //失流期间B相组合无功2电能增量

        INT8U lost_i_c_begin[6]; //(上1次)C相失流发生时刻
        INT8U lost_i_c_end[6];   //(上1次)C相失流结束时刻
        INT8U lost_i_c_zxyg[4];  //失流期间C相正向有功电能增量
        INT8U lost_i_c_fxyg[4];  //失流期间C相反向有功电能增量
        INT8U lost_i_c_zxwg[4];  //失流期间C相组合无功1电能增量
        INT8U lost_i_c_fxwg[4];  //失流期间C相组合无功2电能增量
    };
}C2_F303;

typedef union{
    INT8U  value[20];
    struct{
        INT8U rec_date[5];            //年-月-日-时-分 BCD码
        INT8U lost_power[3];          //掉电总次数
        INT8U lost_power_begin[6];    //最近一次掉电发生时刻
        INT8U lost_power_end[6];      //最近一次掉电结束时刻
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
        INT8U biti:3;    //记录缩位位置
        INT8U protocol:1;   //规约类型 0:97  1:07
        INT8U flag:1;      //是否使用全A或全9 搜索
        INT8U start:2;   //启停标志 0: 启动搜表  >=2完成搜表
        INT8U isaddF10:1;   //事发后添加F10     0 ：未添加 1：已填加
    };
} SEARCH_485;

#ifdef __SHANGHAI_CLASS_53_54_485_CURVE__
typedef union{
    INT8U value[2];
    struct{
        INT8U read_date[5];   //冻结时标 bcd
        INT8U zxygz[4];       //正向有功总
        INT8U v[6];           //电压
    };
}CLASS_53_54_CURVE;
#endif

typedef union{
   INT8U value[7];
   struct{
     INT8U rec_date[5];       //年-月-日-时-分 BCD码
     INT8U  v[2];
     INT8U  i[2];
   };
}C2_F211;

//轮显数据
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
        INT8U length;                  //数据长度+4
        INT8U item[4];                 //数据项
        INT8U meter_state_word[110];   //规约类型 12+96+2
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
        INT8U ctrl_flag;               //控制标识
        INT8U meter_state_word[12];    //状态字
        EVENT_COUNT event_count;       //事件发生次数及掩码
        INT8U event_mask[1];           //事件记录掩码
    };
}EVENT_READ_CTRL;
#endif

typedef union{
    INT8U    value[15];
    struct{
	    INT8U datetime[5];
        INT8U cur_day_fee[5];		//当日实时用电金额
        INT8U cur_month_fee[5];		//当月累计用电金额
    };
}C1F181,C2F240;
 
#endif
