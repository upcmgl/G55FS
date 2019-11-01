#ifndef __OPTION_H__
#define __OPTION_H__
//编译选型的定义文件
//GPRS表模块
#ifdef __GMETER__
#define STACK_UNUSE_ENC24J600
#define __ENABLE_INFRA__
#define __REPORT_NODE_EVENT__ //全事件
#define __GMETER_CHECK__ //工厂检测U盘 usb.dat会生成ERC64
//#define __FIX_METERNUM_TO_ERTU_ADDR__ //上海电表表号设置为终端地址
//#define __FIX_METERNUM_TO_ERTU_ADDR_JILIN__ //吉林电表表号设置为终端地址
//#define __TEST_CTRL__ //载波转GPRS power_ctrl电平取反 上海外置GPRS需要打开
//#define __CURVE__  //曲线取最近一个有效值
//#define __HEBEI_HUANBAO__//河北邯郸环保局 645上行 心跳FF+表号（正序）
//#define __APN32__
//#define __BEIJING_CURVE__
//#define __LVD_5V__
//#define __ZB2GPRS_485_NOT_OD__ //载波转GPRS 485转接方式 不能开漏 （直接和电表接口方式需要开漏）
#endif


//#define __SHANGHAI_WAIZHI__
#ifdef  __SHANGHAI_WAIZHI__
#define __FIX_METERNUM_TO_ERTU_ADDR__ //上海电表表号设置为终端地址
#define __TEST_CTRL__ //载波转GPRS power_ctrl电平取反 上海外置GPRS需要打开
#endif

#ifdef __HEBEI_HUANBAO__ 
#define __USE_DUAL_GPRS_CLIENT__
//#define __INFRA_DL645_READ__
#endif

#ifdef __PROVICE_JILIN__
//#define __REPORT_NODE_EVENT__ //全事件
//#define __EVENT_04001501_BEFORE__
#endif

#ifdef __PROVICE_JIANGSU__ //江苏NB使用A9600
//#define __USE_EX_GPRS__ //目前只有A9600使用外部协议  发送管脚开漏 
//#define __GPRS_OD__    //A9600模组发送管脚开漏
#endif

//无源干节点数量 默认为2个
#define  __NOSRC_NODE_COUNT__  4

#define MAX_SW_TURN_COUNT     2
#define MAX_PULSE_PORT_COUNT  2

//实时读取交采F25数据
//#define __READ_485_CY_DIRECT__

//一类数据为实时读取
#define __READ_C1DATA_VIA_MONITOR__ 

#define __ENABLE_CLOCK_FIX_PLC_REC__

//实时读取485一类数据
#define __READ_485_DIRECT__

//#define ENABLE_MSA_FRAME_CACHE //使能主站报文缓存功能
#define MSA_FRAME_CACHE_VALID_SECONDS  600 //缓存的上行主站报文有效时间，单位s

//#define __BATCH_TRANSPARENT_METER_TASK__

#ifdef __POWER_CTRL__
  //自动配置脉冲表档案
  #define __PULSE_METER_AUTO_ADD_F10__
  #undef __NOSRC_NODE_COUNT__
  #define __NOSRC_NODE_COUNT__ 2        //国网遥信支持2路
#endif

#define BAUDRATE_CAS_TO_CY   9600

#define __FILE_TRANSMIT_REVERSE__   //文件传输，反转传输内容

#define  BASE_STATION_CHANGE  //锁频处理已经同步为与3.0一致，可以全部打开，由用户在界面选择启用与否
//#define __USE_SMS_CTRL_ALARM__    //使用短信通知控制告警信息

//#define __USE_DUAL_GPRS_CLIENT__  //使用2个GPRS客户端，不支持以太网的客户端

//#define __METER_EVENT_GRADE__       //国网事件等级抄读

//#define __REPORT_NODE_EVENT__       //国网电表事件主动上报

//#define __METER_CURVE_READ__    //国网曲线抄读

//#define __PRECISE_TIME__      //精确对时

//协议编译选项
//#define __PROVICE_YUNNAN__    //云南省(参数F33不同，1类数据扩展并实时抄读，含负控2004规约)

//#define __PROVICE_HUBEI__       //湖北省

//#define __PROVICE_GUIZHOU__       //贵州省，南网2008

//#define __PROVICE_HUNAN__       //湖南省，重点表抄读电表端整点冻结数据,要求有台区组网功能

//#define __PROVICE_JILIN__      //吉林省(无日冻结05060101,改抄00010000)

//#define __PROVICE_SHANXI__       //山西 要抄载波反向有功

//#define __PROVICE_TIANJIN__      //天津

//#define __PROVICE_ZHEJIANG__     //浙江

//#define __ZHEJIANG_EMERGENCY__     //浙江 应急处理 捞表时，载波表不加入档案，不进行路由查询节点的操作

//#define __PROVICE_CHONGQING__    //重庆

//#define __PROVICE_XINJIANG__     //新疆

//#define __PROVICE_XINJIANGFK__     //新疆负控

//#define __PROVICE_SICHUAN__     //四川

//#define __PROVICE_SICHUAN_BOGAO__     //四川博高

//#define __DAYHOLD_GTOUP_REPEAT_TD__    //请求二类数据组合下发时带有多个时标

//#define __PROVICE_HUBEI__     //湖北

//#define __PROVICE_FUJIAN__     //福建

//#define __PROVICE_JIANGSU__     //江苏

//#define __JIANGSU_EXT_METER_PROTOCOL_11_12__     //江苏

//#define __PROVICE_GUANGXI__     //广西

//#define __PROVICE_ANHUI__       //安徽

//#define __PROVICE_GANSU__       //甘肃

//#define __PROVICE_SHANDONG__       //山东

//#define __COUNTRY_ISRAEL__      //以色列要求（实时电量抄读0000FF00:F129   增加APM数据）

//#define __ISRAEL_CYCLE_REC__      //以色列周期抄表，按照F33的抄表间隔时间进行抄表最小间隔15分钟，保存96个点的数据

//#define __ISRAEL_HOUR_REC__       //不抄日冻结，抄小时冻结，0点的小时冻结转存日冻结，补抄，保存96个点的数据

//#define __COUNTRY_SOUTH_AFRICA__   //南非要求，其他与以色列相同，但是日冻结改抄05063001

//#define __COUNTRY_IRAN__        //伊朗，内部按照公历运行，显示按照伊朗历，

//#define __COUNTRY_BRAZIL__        //巴西

//#define __ROADLAMP_CTRL__      //路灯控制器 （适用于路灯控制的）

//#define __PLC_READ_VIP_F25__    //载波抄读有功无功功率、电压电流

#define __PLC_READ_FXYG__    //载波抄读反向有功

#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //05060101抄不到时，单费率抄读当前00010000,多费率抄读0001FF00,2013.8.6 所有版本打开

#define __PLC_REC_97_901F_DENY_9010__       //97载波表抄读日冻结901F否认时，改抄9010

//#define __REDUCE_FLOW__    //缩减流量，湖南要求。

//#define __TD_IS_TODAY__    //时标是当天，不是前一天,云南昭通主站特色

//#define __PROVICE_SHANGHAI__   //上海市

//#define __SHANGHAI_FOR_TEST_PASS__    //上海测试，抄读1类数据的临时策略。

//#define __485_NET__    //485组网或捞表

//#define __OPEN_PACKAGE_MONITOR__   //开箱监视器 湖南

//#define __485_BIT_SEARCH__   //485缩位搜表

//#define __SET_F25_METER_TYPE__   //根据参数设置F25区别电表是单线和三相，载波抄表

//#define __PLC_READ_METER_TIME__    //载波抄读电表时钟，产生ERC12

//#define  __ENABLE_ESAM__    //使用ESAM模块

//#define  __EN_CAP_CTRL__    //使用无功投切功能

//#define  __PLC_NET_F149_TYPE_1__     //浙江捞表模式1 ：等待+捞表的时间是3、10、17分钟
//#define  __PLC_NET_F149_TYPE_2__     //浙江捞表模式2 ：等待3、10、17分钟，捞表15分钟

//#define  __POWER_CTRL__     //负控

//#define __TRANS_STATION__  //变电站，抄读64块485电表，不用抄读载波电表

#define  __STAT_LOADRATE__     //统计负载率

#define __C1_DATA_FL_IS_4__    //按照默认4费率处理

//#define  __DAYHOLD_F167__     //日冻结电能表购用电信息 存储93天的数据

//#define __PLC_REC_PLAN_FROM_F39__      //载波抄表按照F39方案进行抄表

//#define __REC_DAY_HOLD_READ_SETTLEMENT_DAY_DATA__      //抄表日数据抄读上一结算日数据

//#define __COMPUTE_XLOST__        //计算线损

//#define  __TEMPERATURE_CURVE__   //温度曲线

//#define  __STAT_PLC_REC_FAIL__   //统计载波抄表失败

//#define __TAIL_VOLTAGE_CURVE__    //四川抄读末端电压曲线

//#define __PLC_REC_LOST_VI__      //多功能载波表抄读失压失流变更次数和电能表编程次数变更，存储10次

#define __CAST_NO_CONFIRM__      //组地址不确认

//define __VIP_METER_255__       //重点表支持255个

//defind __PLC_VIP_METER_REC_MODE_2__     //重点表抄读方式2

//defind __PLC_VIP_METER_REC_MODE_2_SUPPLEMENT_REC__     //重点表抄读方式2不抄当天的小时冻结

//define __FTP_REMOTE_UPDATE__       //Ftp远程升级  上海扩展

//define __PWLEN_IS_2BYTE__
//#define __RECONNECT_MSA_AFTER_CHANGE__   //修改通讯参数后是否立即重连主站

//#define __GB_645_YUNNAN__          //按在云南电表规约抄读电表

//#define __GB_645_YUNNAN_CJQ_REC_2__   //按在云南电表规约抄读采集器抄两个数据项，不抄00010000,否则抄读3个数据项，

//#define __GB_645_YUNNAN_CHECK__       //送检临时使用，云南电表规约模式4时，抄读日冻结数据项05060101和05060201，月冻结抄读上一结算日数据，不抄00010000的符合记录

//#define __PROVICE_YUNNAN_PUER__      //------------------云南普洱

//#define __PROVICE_YUNNAN_TEST__      //云南电科院科陆台体送检，1、485抄表为00或FF时，周期按照1分钟设置；2、参数设置F26中的所有超限持续时间都按照1分钟统计

//#define __PLC_REC_CJQ_EVENT__     //如果是载波表，并且采集器地址有效，则需要读采集器事件

//#define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据

//#define __PLC_PRIOR_READ_HIGH__        //载波表预抄一类数据，放在每块电表日冻结后抄读，否则的话，在全部电表日冻结抄完后，才抄

#define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //载波表预抄一类数据保存在文件系统中，否则保存在spansion中

//#define __PLC_PRIOR_READ_PER_HOUR__   //载波表预抄一类数据每小时每小时抄读一次

//#define __PLC_READ_LOAD_RECORD__       //三相载波表抄读负荷记录数据，对应曲线F89-F94,F103,F101,按照60分钟时间间隔，保存10天的数据

//#define __NO_QGDW_TYPE__  //不处理协议类型，统统按照376.1处理

//#define __PLC_REC_CJQ_ERC_36_37__      //载波抄表采集器生成erc36电能表开表盖事件、erc37

//#define __FORGE_METER_TIME__           //主站请求电表时间时，不进行抄读，返回集中器的时间，彬腾使用

#define __OTHER_DAYHOLDS_SAVE_SPOT_FLIE__ //载波多功能表其他日冻结数据保存在测量点文件中，存储2个月数据

//是否允许设备0点复位
//#define __ALLOW_SELF_RESET_AT_ZERO__

//路由选择, 定义使用iAD的路由协议
//#define __ROUTER_iAD__

//#define __TCP_SERVER_MODE__		//tcp 服务器模式
//#define __TCP_SERVER_CLIENT_MODE__  //tcp 混合模式
#define __REMOTE_ETH_PRIORITY__	 //以太网优先原则，不开启为GPRS优先
#define __USE_SMS_HEAD__	//使用短信头
#define __USE_SMS__			//使用短信
//#define __USE_FIXED_HEART_CYCLE__	//使用固定心跳周期

//#define __INFRA_DL645_READ__    //通过红外使用645抄读集中器电表数据
//#define __MAKE_CUTPOWER_EVENT__	//带电池时生成停电事件，四川华立达检测需要
#define __ENABLE_SYS_TOOLS__	//系统调试工具
//#define __USE_2013_UPDATE__	//使用2013协议进行升级


#define __ENABLE_CLOCK_FIX__   //使能时钟修复

#define __AUTO_SYS_INIT__       //使能自动初始化参数
#define  __ROUTER_ADDR_MODE__   //路由载波主节点地址

#if defined(__ROUTER_iAD__)
  #define __ROUTER_iAD_DIRECT_RECORD__   //直接抄读模式
#endif


//请求1类数据的最大等待延时：单位秒
#define MAX_WAITTIME_OF_REQ_C1    60

#define __DEL_REPEAT_METERNO_AT_ADD__       //添加新表档案时如果原表档案中存在相同表号，则删除原表档案    

//如果集中器有液晶显示屏，则定义如下宏，否则禁止定义
#define __LCD__
#define __485_BAUDRATE_USE_DOC__
//#define __POWEROFF_NO_CHECK_CY__		//停上电不检测交采停电纪录，直接来自终端掉电复位

#define __METRE_DOC_INIT_OPTIMIZE__    //启用数据区域初始化时初始化测量点档案优化功能
#define __IMPORTANT_PARAM_BACKUP_RESTORE__   //启动档案备份与恢复，目前支持终端基本参数和电表档案，使用三备份，采用三取二原则
//#if defined __PROVICE_SHANGHAI__
//#elif defined __PROVICE_SHANDONG__
//#elif defined __PROVICE_ZHEJIANG__
//#else
#if defined __POWER_CTRL__
#define __READ_HENGTONG_METER__				 //恒通规约支持
#define __READ_EDMI_METER__					 //红相规约支持
#endif
//编译时指定
//中文显示介面
//#define __DISPLAY_IN_CHINESE__
//#define __DISPLAY_QINGHAI__

//英文显示界面
//#ifndef __DISPLAY_IN_CHINESE__
//#define __DISPLAY_IN_ENGLISH__
//#define __DISPLAY_IN_ENGLISH_GW__
//#endif


#if defined(__ZJWS__)

  #undef __TOPS__ 
  #undef __DISPLAY_IN_ENGLISH__
  #undef __PROVICE_YUNNAN__
  #undef __PROVICE_JILIN__
  #undef __DISPLAY_IN_ENGLISH_GW__
  #undef __DISPLAY_QINGHAI__

  //定义中兴通信ME3000模块 
  #define __GPRS_ME3000__  
  #ifndef __DISPLAY_IN_CHINESE_ZJWS__
  #define __DISPLAY_IN_CHINESE_ZJWS__
  #endif
#endif //__ZJWS

#if defined(__DISPLAY_IN_CHINESE_ZJWS__)
  #ifndef __DISPLAY_IN_CHINESE__
  #define __DISPLAY_IN_CHINESE__
  #endif
#endif


//#define __QGDW_2013_STRUCT__   //在工程里面定义
//#define __REC_DH_485_EXT__
#ifdef __QGDW_2013_STRUCT__
#define __ENABLE_ESAM2__
#else
//#define __ENABLE_ESAM__
#endif

#ifdef __QGDW_376_2013_PROTOCOL__
    #define __376_2_2013__
    #define __FTP_REMOTE_UPDATE__
    #define __ERC_37__                         //erc37：电能表开表盖事件记录
    #define __ERC_38__                         //erc38：电能表开端钮盒事件记录
    #define __ERC_39__                         //ERC39：补抄失败事件记录
    #define __ERC_40__                         //erc40：磁场异常事件记录
   // #define __ERC_41_SHANGHAI__                //erc41：对时事件记录
    #ifndef __POWER_CTRL__
    #define __PLC_NET_F149__
    #define __PLC_NET_F149_TYPE_2__             //F149捞表模式
    #define __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #define __TIMER_PLC_NET_MODE_1__            //浙江模式搜表
    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //载波表预抄一类数据保存在文件系统中，否则保存在spansion中
    #define __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #define __MAX_SPOT_IDX_2040__          //测量点最大支持2040个
    #define __PLC_REC_PLAN_FROM_F39__      //载波抄表按照F39方案进行抄表
    #endif
    #define __METER_DOC_F150__
    #define __PATCH_REC_DH__                    //补抄3天日冻结
    //#define __VIP_PATCH_REC_DATA__              //重点表补抄07表小时冻结
    #define __F39_DAYHOLD_F210__                    //日冻结购用电信息
    //#define __DELETE_30_DAY_NO_REC_SPOT__       //删除30天没有抄到的测量点
    #define __PLC_NET_ADD_FILE__                //新规范定义F149 01时添加档案，不加搜表文件 02添加文件，不加档案
    #define __FILL_DEFAULT_C2_DATA__    //测量点不存在时，请求2类数据是否填充EE
    #define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
    //#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询 
    #define __USE_2013_UPDATE__			 //使用2013808版协议升级
    #define __USE_DUAL_GPRS_CLIENT__//使用2个GPRS客户端，不支持以太网的客户端

//以下是线损深化应用，增加的项目
/*
    #define __COMPUTE_XLOST__
    #define __USE_DUAL_GPRS_CLIENT__//使用2个GPRS客户端，不支持以太网的客户端
    #define __LINE_LOST_METER_OPEN__
    #define __PLC_READ_VIP_F25__           //重点表抄读电压、电流、有无功功率
	#define __PLC_READ_VIP_I__	
*/
#endif
#ifdef __QGDW_376_2009_PROTOCOL__
    #define __376_2_2013__
    #define __FTP_REMOTE_UPDATE__
    #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //载波表预抄一类数据保存在文件系统中，否则保存在spansion中
    #endif
    #define __CONCENTRATOR_INITIATIVE__//主动抄表，晓程方案、瑞斯康方案需要打开
    #define __FILL_DEFAULT_C2_DATA__    //测量点不存在时，请求2类数据是否填充EE
	#define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    
    #define __REDUCE_485_READ_ITEM__  //精简485的抄读数据项
//    #define __ROUTER_LME__ 
//    #define __METER_EVENT_GRADE__        //国网事件等级抄读
//    #define __METER_EVENT_GRADE_TEST__
//    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
//    #define __METER_CURVE_READ__         //国网曲线抄读
#endif
#ifdef __QGDW_CHECK__
	#undef __485_BAUDRATE_USE_DOC__
	//#undef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	//#undef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __PLC_READ_VIP_F25__           //重点表抄读电压、电流、有无功功率
	#define __PLC_READ_VIP_I__				
    #define __METER_EVENT_GRADE__        //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__
#ifndef __POWER_CTRL__
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
#endif
    #define __METER_CURVE_READ__         //国网曲线抄读
    
	#undef __ENABLE_SYS_TOOLS__
    #undef  __READ_HENGTONG_METER__				 //恒通规约支持
	#undef  __READ_EDMI_METER__					 //红相规约支持


#endif
#ifdef __USE_ONE_FLASH__
    #undef __ENABLE_ESAM2__
#endif

#ifdef __QGDW_09_STRUCT__
	#ifdef __QGDW_2013_STRUCT__
	#undef __QGDW_2013_STRUCT__
	#endif
	#ifdef __ENABLE_ESAM2__
	#undef __ENABLE_ESAM2__
	#endif
	#undef __PLC_REC_PLAN_FROM_F39__			//默认取消按F39方案进行抄表
    #undef __376_2_2013__						//取消2013版376.2模式
	//#undef __CHECK_POWEROFF_NEW__		 		//国网新停上电事件
	//#undef __GW_NEW_EVENT_QUERY__       		//国网新事件分类查询
#endif

#if defined(__PROVICE_NINGXIA__) && !defined(__QGDW_376_2013_PROTOCOL__)
	#define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __FTP_REMOTE_UPDATE__
	#undef __QGDW_2013_STRUCT__
	#undef __ENABLE_ESAM2__
	#define __USE_2013_UPDATE__
	#define __POWEROFF_NO_CHECK_CY__		//停上电不检测交采停电纪录，直接来自终端掉电复位
	//#define __COMPUTE_XLOST_30DAY__		//线损计算
	//#define __COMPUTE_PHASE_XLOST__
#endif

#if defined(__TOPS__)
  //定义使用TELIT通信模块
  //#define __GPRS_TELIT__
  //#define __GPRS_MC8331A__
  //#define __GPRS_M72__
  //#define __GPRS_M590__
  //#define __GPRS_G610__
  //#define __GPRS_MY__
#endif  //__TOPS__
#ifdef __JIANGSU_CHECK__
    #define __TCP_SERVER_CLIENT_MODE__  //使用混合模式
    #define __TCP_SERVER_MODE__  		//使用服务器模式
    #define __QGDW_CHECK__
#endif 
#ifdef __PROVICE_JIBEI__                        //------------------冀北
    #define __MAX_SPOT_IDX_2040__          //测量点最大支持2040个
    #define __PRECISE_TIME__                    //精确对时
    #define __CAST_DELAY_UNIT__                 //广播校时单位#endif
    #define __PRECISE_TIME_TEST__                    //精确对时
    #define __METER_EVENT_GRADE__        //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    #define __METER_CURVE_READ__         //国网曲线抄读
//以下，由于空间不够，关闭
    #undef __READ_HENGTONG_METER__				 //恒通规约支持
    #undef __READ_EDMI_METER__					 //红相规约支持
    #undef __ENABLE_SYS_TOOLS__	//系统调试工具
 //   #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
//    #undef __ENABLE_ESAM2__
    #undef  ALLOW_WRITES
    #undef __F39_DAYHOLD_F210__           //日冻结购用电信息

    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149捞表模式
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #undef __TIMER_PLC_NET_MODE_1__            //浙江模式搜表

    #undef __ERC_39__                         //ERC39：补抄失败事件记录
    #undef __ERC_40__                         //erc40：磁场异常事件记录
#endif 
#ifdef __PROVICE_HENAN_YIBANG__                        //------------------河南熠邦
    #define __PROVICE_HENAN__                   //河南
    #define __PWLEN_IS_32BYTE__                 //奕邦主站32字节PW
    #ifdef __QGDW_376_2013_PROTOCOL__
    #undef __QGDW_376_2013_PROTOCOL__
    #endif
#endif 
#ifdef __PROVICE_HENAN__                        //------------------河南
 	#define __CHECK_POWEROFF_NEW__		 		//国网新停上电事件
	#define __GW_NEW_EVENT_QUERY__       		//国网新事件分类查询   
    #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__           //载波表预抄一类数据
    #define __PLC_PRIOR_READ_HIGH__             //载波表预抄一类数据，放在每块电表日冻结后抄读，否则的话，在全部电表日冻结抄完后，才抄
    #define __PLC_REC_PLAN_FROM_F39__     //载波抄表按照F39方案进行抄表。河南要求抄读费率
    #define __F39_DAYHOLD_F210__      //F210按照F39配置方案抄读
    #define __CONCENTRATOR_INITIATIVE__//主动抄表，河南晓程方案需要打开
    //#define __COMPUTE_XLOST__
    //#define __USE_DUAL_GPRS_CLIENT__//使用2个GPRS客户端，不支持以太网的客户端
    //#define __LINE_LOST_METER_OPEN__
    #define __PLC_READ_VIP_F25__           //重点表抄读电压、电流、有无功功率
	#define __PLC_READ_VIP_I__	
    #define __REDUCE_485_READ_ITEM__  //精简485的抄读数据项
    #endif
    #define __ERC_37__                         //erc37：电能表开表盖事件记录
//    #define __ERC_41__
    #define __LINE_LOST_METER_OPEN__
 //   #define __READ_07_485_DAYHOLD__				  //07规约485电表日冻结抄读电表冻结数据项，不使用0点当前值转存
//    #define __MAX_485METER_COUNT_32__      //最大配置32块485电表（含交采）

#endif 

#ifdef __PROVICE_YUNNAN__           //------------------云南
   #define __DL698_41_0928__                   //国网DL698_41_0928版 (南网部分省市采用,包括云南等)
   #define __ALLOW_QGDW129_2004__PROTOCOL__    //是否兼容负控2004规约（2字节ＰＷ）
   #define __GB_645_YUNNAN__
   #define __GB_645_YUNNAN_CJQ_REC_2__   //按在云南电表规约抄读采集器抄两个数据项，不抄00010000,否则抄读3个数据项，
   #define __READ_485_DIRECT__                 //485一类数据实时点抄
   #undef  __NOSRC_NODE_COUNT__ 
   #define __NOSRC_NODE_COUNT__                   7    //无源干节点数量
   #define __TD_IS_TODAY__                    //时标是当天，不是前一天,云南昭通主站特色
    //#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //日冻结抄不到时改抄当前数据标识
	#undef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	#undef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __GB_645_YUNNAN_CHECK__
    #define __PROVICE_YUNNAN_TEST__  //统计类数据需要
    #define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__
//    #define __PLC_direct_curdata_to_dayhold__//特殊版本，平时不用，直接抄当前值存储为日冻结，云南标签里面有用，其他要扩展
//    #define __DAYHOLD_F210__    //普通模式不需要
    #define __UPDATE_NO_CHECK_DATE__ //升级不判断日期是否在前
//    #define __YUNANA_LZ_OLD_METER_PROTOL_FIRST__//拉合闸时，云南老规约在前
    #define __DISABLE_ESAM_F5__//关闭F5功能，宣威曲靖的新联主站总下发改参数
    #define __DIRECT_SWITCH__//支持直接合闸命令
#endif

#ifdef __PROVICE_YUNNAN_PUER__           //------------------云南普洱
    #undef __ALLOW_QGDW129_2004__PROTOCOL__    //是否兼容负控2004规约（2字节ＰＷ）
    #define __NO_QGDW_TYPE__                    //不处理协议类型，统统按照376.1处理
#endif

#ifdef __PROVICE_YUNNAN_ZHAOTONG__             //------------------云南昭通
    #undef __ALLOW_QGDW129_2004__PROTOCOL__    //是否兼容负控2004规约（2字节ＰＷ）
    #define __NO_QGDW_TYPE__                   //不处理协议类型，统统按照376.1处理
    #define __TD_IS_TODAY__                    //时标是当天，不是前一天,云南昭通主站特色
#endif

#ifdef __PROVICE_ANHUI__            //------------------安徽
   #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #define __MAX_SPOT_IDX_2040__          //测量点最大支持2040个
    #define __MAX_485METER_COUNT_32__      //最大配置32块485电表（含交采）
    #define __PLC_READ_VIP_FYG_WG__        //重点表抄读反向有功、正反向无功总电能示值
    #define __PLC_READ_VIP_F25__           //重点表抄读电压、电流、有无功功率
	#define __PLC_READ_VIP_I__				
	#define __PLC_READ_VIP_YP__
	#define __PLC_READ_VIP_WP__
    //#define __PLC_REC_CJQ_ERC_36_37__      //载波抄表采集器生成erc36电能表开表盖事件、erc37
    //#define __PLC_REC_ERC_36_OPEN__        //载波抄表采集器生成erc36电能表开表盖事件
    //#define __PLC_REC_ERC_37_OPEN__        //载波抄表采集器生成erc37电能表开端钮盖事件
    #define __ERC_51__                     //电能表停/上电事件记录  安徽是根据次数变换生成时间，第一次不生成，变化了几次就生成几个一天最多生10个
    #define __PLC_REC_CJQ_CYCLE_ERC_52__   //载波周期抄读采集器生成erc38开箱门事件
    //#define __READ_PLC_NOISE__             //读取噪声
    #define __PLC_REC_PLAN_FROM_F39__         //载波抄表按照F39方案进行抄表
    #define __PLC_READ_METER_TIME__           //载波抄读电表时钟，产生ERC12
    #define __ERC_13__                        //载波抄读状态字，生成erc13
    #define __RISECOMM_376_2_2013__
    //#define	__COMPUTE_XLOST__               //线损功能
    //#define __USE_DUAL_GPRS_CLIENT__     //双GPRS客户端
   #endif 
    #define __STAT_LOADRATE__              //统计负载率
    #define __PATCH_REC_DH__               //补抄62天日冻结
    //#define __REC_DH_485_EXT__             //抄读485扩展日冻结F300-F315
    #define __CHECK_07_DAYHOLD_TIME_1__       //抄读日冻结时间，时间不对的话，不抄日冻结，安徽是按照日冻结时间存储到对应位置
    #define __PATCH_REC_DAY_HOLD_TD_IS_CUR__  //补抄日冻结时间是抄读的时间，不是存储时标的下一日
    #define __PATCH_REC_DH_REPORT__           //上报补抄的日冻结
	//#undef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	//#undef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __CHECK_DAYHOLD_DATA__            //检查日冻结数据正确性	
#endif

#ifdef __PROVICE_FUJIAN__            //------------------福建
    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #define __PLC_READ_LOAD_RECORD__       //三相载波表抄读负荷记录数据，对应曲线F89-F94,F103,F101,按照60分钟时间间隔，保存10天的数据
   //#define __SMS_REPORT__            //允许短信上报，仅支持事件上报
   //#define __SMS_TASK_REPORT__       //允许短信任务上报
	#define __REMOTE_ETH_PRIORITY__ //以太网优先原则
    #define __PLC_READ_VIP_F25__           //载波抄读有功无功功率、电压电流(福建只抄电压)
#endif

#ifdef __PROVICE_GUIZHOU__          //------------------贵州
   #define __DL698_41_0928__
   #define __PLC_READ_FXYG__     //载波抄读反向有功
   #define __RECONNECT_MSA_AFTER_CHANGE__ 
#endif

#ifdef __PROVICE_SHANXI__           //------------------山西
    #define __SET_F25_METER_TYPE__   //根据参数设置的F25确定载波的多功能与单项
    #define __PLC_READ_METER_TIME__    //载波抄读电表时钟，产生ERC12
    #define __REDUCE_FLOW__    //缩减流量，湖南要求。
    #define __COMPUTE_XLOST__
    #undef __READ_HENGTONG_METER__				 //恒通规约支持
    #undef __READ_EDMI_METER__	
    //#define __USE_DUAL_GPRS_CLIENT__//使用2个GPRS客户端，不支持以太网的客户端
#endif

#ifdef  __PROVICE_HUNAN__           //------------------湖南
   #define __READ_PLC_NOISE__       //读取噪声
    #define __REDUCE_FLOW__          //缩减流量
    #define __OPEN_PACKAGE_MONITOR__      //开箱监视器
    #define __VOLTAGE_MONITOR__           //电压监视

    #undef __DISPLAY_IN_CHINESE_ZJWS__
    //#define __DISPLAY_HUNAN__
    #define __DISPLAY_GW_2013_HUNAN__
    #undef __DISPLAY_QINGHAI__
    #undef __DISPLAY_GW_2013__
    
    #define __READ_485_DIRECT__                //485一类数据实时点抄
    #define __NO_QGDW_TYPE__                    //不处理协议类型，统统按照376.1处理
    #define __VIP_PATCH_REC_DATA__             //重点表补抄07表小时冻结
    //#define __READ_07_485_DAYHOLD__				  //07规约485电表日冻结抄读电表冻结数据项，不使用0点当前值转存
    #define __PLC_PRIOR_READ_C1DATA__           //允许预抄一类数据项
    //#define __PLC_PRIOR_READ_HIGH__             //载波表预抄一类数据，放在每块电表日冻结后抄读，否则的话，在全部电表日冻结抄完后，才抄
    #define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //载波表预抄一类数据保存在文件系统中，否则保存在spansion中
    #define __PLC_PRIOR_READ_PER_HOUR__         //载波表预抄一类数据每小时每小时抄读一次
    #define __PLC_READ_VIP_F25__                //载波抄读有功无功功率、电压电流（湖南只抄电压电流）
    #define __PLC_READ_VIP_I__                //载波抄读有功无功功率、电压电流（湖南只抄电压电流）
    #define __PLC_REC_PLAN_FROM_F39__           //载波抄表按照F39方案进行抄表
    
    #define __F39_DAYHOLD_F32__                 //F39抄读日冻结F32
    #define __PLC_READ_VIP_LOAD_RECORD__        //重点表补抄读负荷记录
    
    #if (defined __ZhuZhou_CITY__) || (defined __HuaiHua_CITY__)    
    #undef __VIP_PATCH_REC_DATA__             //重点表补抄07表小时冻结
    #undef __PLC_READ_VIP_LOAD_RECORD__       //重点表补抄读负荷记录
        #ifdef __ZhuZhou_CITY__
        #define __F_CLASS_METER_CURVE__             //F类电表自动抄读曲线
        #endif
    #endif
    
    #define __PLC_PRIOR_F26_SWITCH__            //抄读断相记录否认时切换数据项
	
	//#define __CHECK_07_DAYHOLD_TIME__    //抄读冻结时标，冻结时标错误，当前周期不抄读日冻结
    //#define __COMPUTE_XLOST__	
   	//#define __USE_DUAL_GPRS_CLIENT__     //双GPRS客户端
    //使用湖南搜表模式，不适用浙江模式
    #ifdef __QGDW_376_2013_PROTOCOL__
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149捞表模式
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #undef __TIMER_PLC_NET_MODE_1__            //浙江模式搜表
    #undef __PLC_NET_ADD_FILE__                //新规范定义F149 01时添加档案，不加搜表文件 02添加文件，不加档案
    #undef __F39_DAYHOLD_F210__                //日冻结购用电信息
    #endif
    //#define __METER_CURVE_READ__         //国网曲线抄读

#endif
#ifdef __PROVICE_HUNAN_FK__ 
	#define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
	#define __METER_CURVE_READ__         //国网曲线抄读
#endif
#ifdef __PROVICE_SICHUAN_BOGAO__          //------------------四川博高
    #define __PROVICE_SICHUAN__
    #define __DAYHOLD_GTOUP_REPEAT_TD__   //请求二类数据组合下发时带有多个时标
#endif
#ifdef __PROVICE_SICHUAN_HLD10S__           //--------------------四川华立达，485抄表10S超时
    #define __PROVICE_SICHUAN_HLD__
#endif
#ifdef __PROVICE_SICHUAN_HLD__                //------------------四川华利达
    #define __PROVICE_SICHUAN__
    #define __INFRA_DL645_READ__          //允许通过红外抄读集中器电表数据，支持9010~901F,及相应07数据项
#endif

#ifdef __PROVICE_SICHUAN__          //------------------四川
    #define __INFRA_DL645_READ__          //允许通过红外抄读集中器电表数据，支持9010~901F,及相应07数据项
    #undef __F39_DAYHOLD_F210__                    //日冻结购用电信息
    #undef __VIP_PATCH_REC_DATA__                   //重点表补抄07表小时冻结
    #define __DAYHOLD_F167__     //日冻结电能表购用电信息 存储93天的数据
//    #undef __PATCH_REC_DH__
//    #undef __PLC_READ_FXYG__
    #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__    //允许预抄一类数据项
    #endif
    //#define __REC_PRICE_PARAM__           //抄读电价参数
    #define __MAKE_CUTPOWER_EVENT__         //带电池时生成停电事件，四川华立达检测需要
 //   #define __F210_SIMPLE_VISION__   //F210抄读简版
 //   #define __DAYHOLD_F210__
    #ifndef __REPORT_NODE_EVENT__
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    #endif
    
    #define __METER_EVENT_GRADE_TEST__   //C1F244读取事件文件的内容
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    
    #ifndef __GW_NEW_EVENT_QUERY__
    #define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #endif
    
    #ifndef __CHECK_POWEROFF_NEW__
    #define __CHECK_POWEROFF_NEW__       //国网新停上电事件
    #endif
#endif

#ifdef __PROVICE_HUBEI__            //------------------湖北
    #define __PLC_READ_VIP_F25__    //载波重点表抄读有功无功功率、电压电流
    //#define __PLC_READ_VIP_WP__    //载波重点表抄读无功功率

    #define __PLC_READ_FXYG__       //载波抄读反向有功
    #define __STAT_LOADRATE__      //统计负载率
    #define __PLC_PRIOR_READ_C1DATA__     //允许预抄一类数据项
 //   #define __HUBEI_REC_V_I__             //抄读电压电流
//	#define __HUBEI_REC_POWER__             //抄读功率
    #define __METER_DOC_F150__
  //  #define __COMPUTE_XLOST__
//    #define __USE_DUAL_GPRS_CLIENT__//使用2个GPRS客户端，不支持以太网的客户端
    #undef __ENABLE_SYS_TOOLS__	//系统调试工具
    #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #undef __READ_HENGTONG_METER__				 //恒通规约支持
    #undef __READ_EDMI_METER__					 //红相规约支持
  
    #define __METER_EVENT_GRADE__        //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__   //C1F244读取事件文件的内容
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    #define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
    #define  BASE_STATION_CHANGE
#endif

#ifdef __PROVICE_TIANJIN__          //------------------天津
    #define __PLC_READ_VIP_F25__    //载波重点表抄读有功无功功率、电压电流
    #define __PLC_READ_FXYG__       //载波抄读反向有功
    #define __STAT_LOADRATE__     //统计负载率
    #define __PLC_REC_PLAN_FROM_F39__  //载波抄表按照F39方案进行抄表
    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
#endif

#ifdef __PROVICE_CHONGQING__        //------------------重庆
    #define __485_NET__
    #define __TD_IS_TODAY1__        //时标是当天，不是前一天, 只管理日冻结的
   
   	
   	#ifndef __POWER_CTRL__    
	    #define __PLC_READ_VIP_F25__    //载波抄读有功无功功率、电压电流
	    //#define __DISPLAY_HEADER_LINE2__   //不显示顶栏横线
	    #define __PLC_READ_VIP_I__//重点表抄读三相电流
	    #define __PLC_READ_VIP_YP__
		#define __PLC_READ_VIP_WP__
	    #define __PLC_REC_PLAN_FROM_F39__ //09和13硬件，都按照F39配置进行抄读??
	    
	    #ifdef __QGDW_09_STRUCT__//针对重庆09硬件，需要升级成13协议
		    #define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
			#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
	   	#endif
	   	#define __DISPLAY_FOOTER_NO_GPRSSTATUS__    //底层状态栏不显示GPRS信息
	   	//#define __COMPUTE_XLOST__                 //增加线损功能
    	//#define __USE_DUAL_GPRS_CLIENT__          //双GPRS客户端
   	#endif
#endif

#ifdef __PROVICE_XINJIANG__            //------------------新疆
    #define __USE_DUAL_GPRS_CLIENT__
    #define __BATCH_TRANSPARENT_METER_TASK__
    //#define __BATCH_TRANSPARENT_METER_TASK_DEBUG__
    //#define __BATCH_TRANSPARENT_METER_TASK_TMP__
    //#define __BATCH_TRANSPARENT_METER_TASK_TMP1__
#endif

#ifdef __PROVICE_XINJIANGFK__            //------------------新疆负控
    #define __PROVICE_XINJIANG__
    #define __POWER_CTRL__               //负控
    #define STACK_UNUSE_ENC24J600
#endif

#ifdef __COUNTRY_BRAZIL__                       //--------------------巴西
    #define __BRAZIL_HOUR_REC__                 //每小时抄读负荷记录

    #undef __ERC_17__
    #undef __ERC_37__
	#undef __ERC_38__
	#undef __ERC_41__
    #undef __ERC_41_SHANGHAI__
	#undef __ERC_67__
    #undef __ERC_68__
    #undef __ERC_39__                         //ERC39：补抄失败事件记录
    #undef __ERC_40__                         //erc40：磁场异常事件记录
    #ifndef __POWER_CTRL__
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149捞表模式
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #undef __TIMER_PLC_NET_MODE_1__            //浙江模式搜表
    #undef __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #undef __PLC_PRIOR_READ_DATA_SAVE_FILE__   //载波表预抄一类数据保存在文件系统中，否则保存在spansion中
    #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #undef __MAX_SPOT_IDX_2040__          //测量点最大支持2040个
    #undef __PLC_REC_PLAN_FROM_F39__      //载波抄表按照F39方案进行抄表
    #endif
    #undef __METER_DOC_F150__
    #undef __PATCH_REC_DH__                    //补抄3天日冻结
    //#define __VIP_PATCH_REC_DATA__              //重点表补抄07表小时冻结
    #undef __F39_DAYHOLD_F210__                    //日冻结购用电信息
    #undef __DELETE_30_DAY_NO_REC_SPOT__       //删除30天没有抄到的测量点
    #undef __PLC_NET_ADD_FILE__                //新规范定义F149 01时添加档案，不加搜表文件 02添加文件，不加档案
    #undef __FILL_DEFAULT_C2_DATA__    //测量点不存在时，请求2类数据是否填充EE
	#undef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	#undef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
#endif

#ifdef __COUNTRY_SOUTH_AFRICA__                 //--------------------南非，同以色列，更改抄读标识
    #define  __COUNTRY_ISRAEL__ 
    #define __ISRAEL_CYCLE_REC__      //以色列周期抄表，按照F33的抄表间隔时间进行抄表最小间隔15分钟，保存96个点的数据
    #define __COUNTRY_SOUTH_AFRICA_DISPLAY__ //南非显示，XL_002
#endif

#ifdef  __COUNTRY_ISRAEL__                     //------------------以色列，增加了广播任务，扩充数据标识：AFN=05, FN=62,63 设置、取消广播任务
    #define __ISRAEL_HOUR_REC__       //不抄日冻结，抄小时冻结，0点的小时冻结转存日冻结，补抄，保存96个点的数据
    #if defined(__ISRAEL_CYCLE_REC__)
       #undef __ISRAEL_HOUR_REC__
    #endif
     #define __HAVE_PLC_BROADCAST_CTRL__  //广播控制任务
    #undef  __NOSRC_NODE_COUNT__
    #define  __NOSRC_NODE_COUNT__  7     //需要支持集中器开盖事件
    #undef __PLC_READ_FXYG__
    #define __ISRAEL_REC_CONTROL__
    //#define __ISRAEL_REC_ONE_ITEM__
    #define __USE_SMS__                  //使用短信
    #undef __PLC_READ_FXYG__
    #define __ISRAEL_PLC_REC_APM__
    #define __ISRAEL_PLC_REC_DAYHOLD_WG__
    #define __COUNTRY_ISRAEL_DISPLAY__ //厂家版本显示XL_001
    #undef __ERC_17__
    #undef __ERC_37__
	#undef __ERC_38__
	#undef __ERC_41__
    #undef __ERC_41_SHANGHAI__
	#undef __ERC_67__
    #undef __ERC_68__
    #undef __ERC_39__                         //ERC39：补抄失败事件记录
    #undef __ERC_40__                         //erc40：磁场异常事件记录
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149捞表模式
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #undef __TIMER_PLC_NET_MODE_1__            //浙江模式搜表
    #undef __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #undef __PLC_PRIOR_READ_DATA_SAVE_FILE__   //载波表预抄一类数据保存在文件系统中，否则保存在spansion中
    #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #undef __MAX_SPOT_IDX_2040__          //测量点最大支持2040个
    #undef __PLC_REC_PLAN_FROM_F39__      //载波抄表按照F39方案进行抄表

    #undef __METER_DOC_F150__
    #undef __PATCH_REC_DH__                    //补抄3天日冻结
    //#define __VIP_PATCH_REC_DATA__              //重点表补抄07表小时冻结
    #undef __F39_DAYHOLD_F210__                    //日冻结购用电信息
    #undef __DELETE_30_DAY_NO_REC_SPOT__       //删除30天没有抄到的测量点
    #undef __PLC_NET_ADD_FILE__                //新规范定义F149 01时添加档案，不加搜表文件 02添加文件，不加档案
    #undef __FILL_DEFAULT_C2_DATA__    //测量点不存在时，请求2类数据是否填充EE
	#undef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	#undef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #undef __ENABLE_ESAM__              //不使用esam
//	#undef __USE_2013_UPDATE__			 //使用2013版协议升级
#endif

#ifdef __PROVICE_PERU__
    
#endif
#ifdef __LINE_LOST_DEEP_APPLY__
    #define __COMPUTE_XLOST__
    #define __USE_DUAL_GPRS_CLIENT__//使用2个GPRS客户端，不支持以太网的客户端
    #define __LINE_LOST_METER_OPEN__
    #define __PLC_READ_VIP_F25__           //重点表抄读电压、电流、有无功功率
	#define __PLC_READ_VIP_I__	
    #define __UPDATE_NO_CHECK_DATE__ //升级不判断日期是否在前
#endif

#ifdef __PROVICE_STAR__       //思达
    #define __STAR_EXT_METER_PROTOCOL_40__//思达规约
    #define __SMS_TXT__   //支持文本短信
    #undef __DELETE_30_DAY_NO_REC_SPOT__
//	#define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
//	#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __ERC_51__                   //停上电记录事件
    #define __METER_EVENT_GRADE__        //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    #define __METER_CURVE_READ__         //国网曲线抄读
//    #define __STAR_PRIOR_F187__          //预抄0CF187窃电次数
//    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
    #define __COMPUTE_XLOST__
    #define __REC_MONTH_HOLD_07__      //抄读上一结算日数据
    #define __REC_MONTH_HOLD_07_XL__   //抄读上一计算日需量数据
#endif

#ifdef __PROVICE_ZHEJIANG__        //------------------浙江
    #define __485_BIT_SEARCH__                  //485缩位搜表
    #define __ERC_41_SHANGHAI__
    //#define __SMS_REPORT__                      //允许短信上报，仅支持事件上报
    //#define __SMS_TASK_REPORT__                 //允许短信任务上报
    //#define __PLC_READ_FXYG__                   //抄读反向有功
    //#define __READ_485_DIRECT__                 //485一类数据实时点抄
    #define __PLC_NET_F149__
    #define __PLC_NET_F149_TYPE_2__             //F149捞表模式-----在2009的编译条件下，该2项未选择
    //#define __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #define __ZHEJIANG_EMERGENCY__              //浙江 应急处理 捞表时，载波表不加入档案，不进行路由查询节点的操作
    #define __RECONNECT_MSA_AFTER_CHANGE__      //GPRS参数修改后重连
    #define __READ_07_485_DAYHOLD__             //07规约485电表日冻结抄读电表冻结数据项，不使用0点当前值转存,吉林抄读05000101（上1次）定时冻结数据，其他抄读05060101（上1次）日冻结数据
    #define __CHECK_07_DAYHOLD_TIME__           //抄读日冻结时间，时间不对的话，不抄日冻结
    #undef __USE_SMS_HEAD__	                    //使用短信头
    #define __PRECISE_TIME__                    //精确对时
    #define __CAST_DELAY_UNIT__                 //广播校时单位#endif
    #define __PRECISE_TIME_TEST__               //精确对时
    #define __METER_EVENT_GRADE__               //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__               //国网电表事件主动上报
    #define __EVENT_04001501_BEFORE__           //事件状态字放在事件记录前面
    #define __EVENT_RECORD_BLOCK__              //事件记录按照数据块抄读
    #define __METER_CURVE_READ__                //国网曲线抄读
    #define __GET_DH_AMOUNT_FROM_REAL__         //测量点的日冻结电能量从实时值去计算，不采用日冻结，暂时调试使用，仅485电表
	#define __DELETE_30_DAY_NO_REC_SPOT__       //删除30天没有抄到的测量点
//	#define __COMPUTE_XLOST__                     //线损计算
//以下4个，由于空间不够，关闭
    #undef __READ_HENGTONG_METER__              //恒通规约支持
    #undef __READ_EDMI_METER__                  //红相规约支持
    #undef __ENABLE_SYS_TOOLS__	                //系统调试工具
    #undef __CONCENTRATOR_INITIATIVE__          //集中器主动抄表模式
    #undef __F39_DAYHOLD_F210__           //日冻结购用电信息
	#undef __FTP_CUSTOM_UPDATE__
	#undef __FTP_REMOTE_UPDATE__
	#undef STACK_USE_FTP_CLIENT
	#undef __USE_2013_UPDATE__
    #undef __REDUCE_485_READ_ITEM__  //精简485的抄读数据项

#endif

#ifdef __PROVICE_GUANGXI__         //------------------广西
    #define __COMPUTE_XLOST__      //计算线损
    #define __PLC_REC_LOST_VI__    //抄读失流失压编程次数
    #define __STAT_PLC_REC_FAIL__   //统计载波抄表失败
#endif

#ifdef __PROVICE_SHANGHAI_00__     //------------------上海0
    #define __PROVICE_SHANGHAI__
    #define __DISPLAY_QINGHAI__
#endif

#ifdef __PROVICE_SHANGHAI_01__     //------------------上海1
    #define __PROVICE_SHANGHAI__
    #define __DISPLAY_GW1__
#endif

#ifdef __PROVICE_SHANGHAI_02__     //------------------上海2
    #define __PROVICE_SHANGHAI__
    #define __DISPLAY_GW2__
#endif

#ifdef __PROVICE_SHANGHAI__        //------------------上海
    #define __READ_485_DIRECT__               //485一类数据实时点抄
    #define __FTP_REMOTE_UPDATE__       //Ftp远程升级  上海扩展
    #define __PLC_READ_FXYG__              //载波抄读反向有功
    #define __ALLOW_LETTER_METER_NO__      //允许字母表号
    #define __PATCH_REC_DH__               //补抄3天日冻结  正反向

    #ifndef __QGDW_2013_STRUCT__
    #define __USE_RS485_3__            //使用485_3,仅支持695芯片
    #endif //#ifndef __QGDW_2013_STRUCT__

//    #define __PATCH_REC_C2_DATA__      //AFN=1D 补抄2类数据
    #define __VIP_PATCH_REC_DATA__            //重点表补抄07表小时冻结

    #ifndef __POWER_CTRL__
    #define __PLC_READ_VIP_V__         //载波重点表抄读电压
	#define __PLC_READ_VIP_I__         //载波重点表抄读电流
	#define __PLC_READ_VIP_YP__        //载波重点表抄读有功功率
	#define __PLC_READ_VIP_WP__        //载波重点表抄读无功功率
	#define __PLC_READ_VIP_Q__         //载波重点表抄读功率因数
    //#define __REC_MONTH_HOLD_07__             //07表月冻结是抄读上一结算日数据存储为月冻结，并且补抄上一个月的，以前是日冻结转存
    #endif //#ifndef __POWER_CTRL__

    #define __PLC_READ_METER_TIME__           //载波抄读电表时间，产生erc12
    #define __SHANGHAI09__                    //显示程序上海的特殊功能
    //#define __PLC_READ_VIP_F25__              //重点表抄读电压、电流、有无功功率
    #define __CHECK_07_DAYHOLD_TIME_1__
    #define __PLC_REC_PLAN_FROM_F39__         //载波抄表按照F39方案进行抄表
    #define __OTHER_DAYHOLDS_SAVE_SPOT_FLIE__   //载波多功能表其他日冻结数据保存在测量点文件中，存储2个月数据
    #define __VIP_METER_IS_485__                //485可以设置为重点表

    #ifndef __POWER_CTRL__
    #define __PLC_READ_VIP_LOAD_RECORD__        //重点表补抄读负荷记录
    #define __PLC_READ_VIP_FYG_WG__             //重点表抄读反向有功、正反向无功总电能示值
    #endif

    #define __METER_DOC_CRC16__                //电表档案CRC16校验
    #define __REC_MONTH_HOLD_SHANGHAI_2009__   //上海规约电表抄读月冻结

    #define __ERC_10__
    #define __ERC_13__
    
    #undef  __READ_HENGTONG_METER__				 //恒通规约支持
	#undef  __READ_EDMI_METER__					 //红相规约支持
    #undef __ENABLE_SYS_TOOLS__	//系统调试工具
    #undef __ENABLE_ESAM2__
    #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149捞表模式
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //空采集器加入电表档案
    #undef __TIMER_PLC_NET_MODE_1__            //浙江模式搜表
    #undef __ERC_37__                         //erc37：电能表开表盖事件记录
    #undef __ERC_38__                         //erc38：电能表开端钮盒事件记录
    #undef __ERC_39__                         //ERC39：补抄失败事件记录
    #undef __ERC_40__                         //erc40：磁场异常事件记录
    #undef __ERC_41_SHANGHAI__                //erc41：对时事件记录

    //#define __485_BAUDRATE_USE_DOC__            //485电表波特率使用档案中配置，不按照电表规约定义
    //#define __PROVICE_SHANGHAI_CHECK_20131110__  //上海2013.11.10送检版本使用，现场版本不要使用该标签
    #define __REMOTE_ETH_PRIORITY__	           //以太网优先原则，不开启为GPRS优先

    #ifndef __POWER_CTRL__
    //#define __METER_CURVE_READ__         //国网曲线抄读
    #define __METER_EVENT_GRADE__        //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__   //C1F244读取事件文件的内容
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    #define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
    #define __SHANGHAI_CLASS_53_54_485_CURVE__
    #endif //#ifndef __POWER_CTRL__

#endif

#ifdef __PROVICE_SHANGHAI_CHECK_20131110__     //上海2013.11.10送检版本使用
    #define __ERC_10__
    #define __ERC_13__
    #define __ERC_17__                         //ERC17：电压不平衡度
    #define __ERC_33__
    #define __ERC_37__                         //erc37：电能表开表盖事件记录
    #define __ERC_38__                         //erc38：电能表开端钮盒事件记录
    #define __ERC_39__                         //ERC39：补抄失败事件记录
    #define __ERC_40__                         //erc40：磁场异常事件记录
    #define __ERC_41_SHANGHAI__                //erc41：对时事件记录
    //#define __ERC_67__                         //ERC67：电能表掉电事件记录
    #define __ERC_68__                         //ERC68：电能表负荷开关误动作事件记录
#endif

#ifdef __PROVICE_JIANGSU__        //------------------江苏
    //#define __PLC_REC_PLAN_FROM_F39__      //载波抄表按照F39方案进行抄表
    #define __JIANGSU_EXT_METER_PROTOCOL_11_12__  //扩充电表97规约 11,12
    #define __DETECT_BLOCK_READ__       //检测块读
    #define __TCP_SERVER_CLIENT_MODE__  //使用混合模式
    #define __TCP_SERVER_MODE__  		//使用服务器模式
    #define __485_BIT_SEARCH__          //485搜表

    //#define __PLC_READ_VIP_FYG_WG__
    //#define __PLC_READ_VIP_V__
    //#define __VIP_METER_255_JIANGSU__       //重点表支持255个
    //#define __VIP_METER_255__           //重点表支持255个
    //#define __PLC_VIP_METER_REC_MODE_2__     //重点表抄读方式2
    //#define __PLC_VIP_METER_REC_MODE_2_SUPPLEMENT_REC__     //重点表抄读方式2补抄当天的小时冻结

    #undef __GPRS_MC8331A__
	#define __PLC_PRIOR_READ_C1DATA__			  //载波表预抄一类数据
    #define __DISPLAY_FOOTER_NO_GPRSSTATUS__
//按时标抄读冻结标签：
    #define __READ_07_485_DAYHOLD__               //07规约485电表日冻结抄读电表冻结数据项，不使用0点当前值转存,吉林抄读05000101（上1次）定时冻结数据，其他抄读05060101（上1次）日冻结数据
    #define __REC_MONTH_HOLD_07__             //07表月冻结是抄读上一结算日数据存储为月冻结，并且补抄上一个月的，以前是日冻结转存
    #define __REC_DAY_HOLD_READ_SETTLEMENT_DAY_DATA__      //抄表日数据抄读上一结算日数据
    #define __CHECK_07_DAYHOLD_TIME__           //抄读日冻结时间，时间不对的话，不抄日冻结

	#define	__CURVE_96POINTS_BREAK_FRAME__		//曲线数据按96个点分帧，防止主站不能处理自由分帧报文
	#define __POWEROFF_NO_CHECK_CY__		//停上电不检测交采停电纪录，直接来自终端掉电复位
    #undef __TIMER_PLC_NET_MODE_1__            //浙江模式搜表
    #undef __PLC_NET_F149_TYPE_1__
    #undef __PLC_NET_F149_TYPE_2__
	#define __OVERMAX_NO_MAXMAX__			 //越上上限时不计入越上限
	#define __COMPUTE_XLOST__                 //线损分析
#endif

#ifdef __PROVICE_GANSU__                //------------------------甘肃
    #define __USE_FIXED_HEART_CYCLE__	//使用固定心跳周期
    #undef __PLC_PRIOR_READ_C1DATA__   //载波表预抄一类数据
    #define __DAYHOLD_F167__
#endif

#ifdef __PROVICE_JILIN_FK__                    //------------------------吉林负控
    #define __PROVICE_JILIN__
    //#define __POWER_CTRL__     //负控    
#endif

#ifdef __PROVICE_JILIN__                //------------------------吉林
    #ifndef __POWER_CTRL__
    //#define __RTUADDR_IS_6BYTE__	    //终端地址是6个字节长度
    #define __PLC_REC_PLAN_FROM_F39__      //载波抄表按照F39方案进行抄表
    #define __REC_DAY_HOLD_READ_SETTLEMENT_DAY_DATA__  //抄表日数据抄读上一结算日数据
    #define __PLC_REC_VOLTMETER__                      //载波抄读电压表
    #define __READ_07_485_DAYHOLD__                    //07规约485电表日冻结抄读电表冻结数据项，不使用0点当前值转存
    #define __TD_IS_TODAY1__                           //时标是当天，不是前一天, 只管理日冻结的
    #define __TD_IS_CUR_MONTH__                        //时标是当月，不是前月, 只管理月冻结的
    //#define __READ_CYMETER_DAYHOLD_JINLIN_ITEM__       //吉林交采抄读05000101等数据项，而不是标准国网数据项
    #define __READ_07_485_DAYHOLD_JINLIN_ITEM__        //吉林485抄读05000101等数据项，而不是标准国网数据项
        #endif
	#define __CHECK_POWEROFF_NEW__		 //国网新停上电事件
	//#define __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #undef BAUDRATE_CAS_TO_CY
    #define BAUDRATE_CAS_TO_CY  2400

    #undef __ENABLE_SYS_TOOLS__	//系统调试工具
    #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #undef __ENABLE_ESAM2__
    //#undef  ALLOW_WRITES
    
#endif

#ifdef __PROVICE_HEILONGJIANG__
    #define __READ_07_485_DAYHOLD__
    #define __REDUCE_485_READ_ITEM__  //精简485的抄读数据项
//	#undef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
//	#undef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询    
	#define __METER_EVENT_GRADE__        //国网事件等级抄读
#endif

#ifdef __PROVICE_SHANDONG__       //山东
    #define __READ_07_485_DAYHOLD__
    //#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //07载波表日冻结会否认时，改抄当前数据
    #define __QGDW2013_VRATE__   //山东使用2013协议的电压合格率
    #define __CHECK_07_DAYHOLD_TIME__           //抄读日冻结时间，时间不对的话，不抄日冻结
    #define  __PLC_READ_VIP_F25__
    //#define __PLC_READ_VIP_V__                //山东只抄电压
//    #define __MAX_485METER_COUNT_64__
    #define __GET_DH_AMOUNT_FROM_REAL__     //测量点的日冻结电能量从实时值去计算，不采用日冻结，暂时调试使用，仅485电表
    #define __VOLTAGE_MONITOR__      //电压监视
    #define __PLC_PRIOR_READ_C1DATA__      //载波表预抄一类数据
#endif
#ifdef __PROVICE_SHAANXI__       //陕西
    //#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //07载波表日冻结会否认时，改抄当前数据
#endif
#ifdef __PROVICE_NEIMENG__
//是否兼容负控2004规约（2字节ＰＷ）
   #define __ALLOW_QGDW129_2004__PROTOCOL__
   #define __PWLEN_IS_2BYTE__
#endif

#ifdef __PROVICE_DELIX__
    #define __PLC_READ_VIP_F25__                //载波抄读有功无功功率、电压电流（湖南只抄电压电流）
	#define __UPDATE_F170_D7__					//更新F170的D7位，逆相序标示
    #define __COMPUTE_XLOST__
    
#endif
#ifdef __PROVICE_YUNNAN_FK__
   
	#ifdef __CHECK_POWEROFF_NEW__		 //国网新停上电事件
    #undef __CHECK_POWEROFF_NEW__
    #endif
	#ifdef __GW_NEW_EVENT_QUERY__       //国网新事件分类查询
    #undef __GW_NEW_EVENT_QUERY__
    #endif
   #define __QGDW_FK2005__
   #define __PWLEN_IS_2BYTE__
   #define __READ_EDMI_METER__
#endif
#ifdef __PROVICE_YUNNAN_AT__
  #define __UPDATE_NO_CHECK_DATE__ //升级不判断日期是否在前
  #undef __ENABLE_ESAM2__
  #define __HENAN_STATION__
  #define __GLYS_SP__     //97表功率因数特殊处理，不能按块抄
#endif

#ifdef __HENAN_STATION__
  #define __UPDATE_NO_CHECK_DATE__ //升级不判断日期是否在前
  #undef __ENABLE_ESAM2__
#endif

#ifdef __PROVICE_LIAONING__
   //#define __COMPUTE_XLOST__
   #undef __USE_DUAL_GPRS_CLIENT__
   #undef __LOST_CHECK__
  // #define __LAOBIAO_TEST_1min_report__//如果定义了老表1分钟，要打开__QGDW_CHECK__，否则有延时，具体原因未查找
 //  #define __QGDW_CHECK__
    #define __RECONNECT_MSA_AFTER_CHANGE__//修改参数后立刻执行，入F3
    #define __PLC_READ_VIP_F25__           //重点表抄读电压、电流、有无功功率
	#define __PLC_READ_VIP_I__
    #define __VOLTAGE_MONITOR__           //电压监视
    #define __PRECISE_TIME__                    //精确对时
    #define __CAST_DELAY_UNIT__                 //广播校时单位#endif
    #define __PRECISE_TIME_TEST__                    //精确对时
    //#define __METER_CURVE_READ__         //国网曲线抄读
    #define __METER_EVENT_GRADE__        //国网事件等级抄读
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__        //国网电表事件主动上报
    #define __METER_CURVE_READ__         //国网曲线抄读
    #define __ERC_41__

//空间不够，关闭
    #undef __READ_HENGTONG_METER__				 //恒通规约支持
    #undef __READ_EDMI_METER__					 //红相规约支持
    #undef __ENABLE_SYS_TOOLS__	//系统调试工具
    #undef __CONCENTRATOR_INITIATIVE__    //集中器主动抄表模式
    #undef __ENABLE_ESAM2__
    #undef  ALLOW_WRITES
    #undef __F39_DAYHOLD_F210__           //日冻结购用电信息   
#endif

#ifdef __DISPLAY_QINGHAI__ 
  #undef __DISPLAY_IN_CHINESE_ZJWS__
  #ifndef __DISPLAY_IN_CHINESE__
  #define __DISPLAY_IN_CHINESE__
  #endif 
#endif

#if defined(__GPRS_ME3000__)
  #undef __GPRS_TELIT__
  #undef __GPRS_M72__ 
  #undef __GPRS_MC8331A__ 
#endif

//上行协议选择，选则一个

//国网DL698
#define  __DL698__

#define __QGW376_1__

//北京福星晓程云南协议
//#define  __BJFXXC__

//天津规约
//#define  __TJ2008__

//河北省电力公司规约
//#define __HBPLC__



//通信端口定义
//COMMPORT_PLC  在操作上等价到COMPORT_PLC_REC
/*
#if defined (__PROVICE_YUNNAN__)
  #define COMMPORT_485_REC   0x01     //抄表
  #define COMMPORT_485_CAS   0x02     //级联
  #define COMMPORT_485_CY    0x03     //交采内表
  #define COMMPORT_PLC       0x1F     //载波设置端口，河南使用这个端口
  #define COMMPORT_PLC_REC   0x04     //载波抄表端口，集中器内部使用的端口
#elif defined (__PROVICE_CHONGQING__)
*/
#if ( (defined __PROVICE_CHONGQING__) && !(defined __QGDW_376_2013_PROTOCOL__) )//重庆09规范使用下述定义
  #define COMMPORT_485_REC   0x01     //抄表
  #define COMMPORT_485_CAS   0x02     //级联
  #define COMMPORT_485_CY    0x03     //交采内表
  #define COMMPORT_PLC       0x1F     //载波设置端口，河南使用这个端口
  #define COMMPORT_PLC_REC   0x04     //载波抄表端口，集中器内部使用的端口
#else
  #define COMMPORT_485_CY    0x01     //交采内表
  #define COMMPORT_485_REC   0x02     //抄表
  #define COMMPORT_485_CAS   0x03     //级联
  #define COMMPORT_PLC       0x1F     //载波设置端口，河南使用这个端口
  #define COMMPORT_PLC_REC   0x04     //载波抄表端口，集中器内部使用的端口
#endif

//------------------浙江界面定义
//#define __DISPLAY_FOOTER_NO_GPRSSTATUS__      //底部状态栏是否有GPRS状态
//#define __DISPLAY_HEADER_NORM_GPRSSTATUS__    //顶部GPRS指示使用标志模式
//#define __DISPLAY_FOOTER_JIANGSU_GPRSSTATUS__ //底部状态栏使用风格
//#define __DISPLAY_MENUITEM_CURSOR3__          //当前菜单项风格
//#define __DISPLAY_GPRS_SIGNAL1__              //GPRS信号强度风格(默认细线)
//#define __DISPLAY_FOOTER_LINE1__        //底部提示栏显示一条横线(默认显示细线)
//#define __DISPLAY_HEADER_LINE1__        //顶部状态栏显示一条横线(默认显示细线)
//#define __DISPLAY_LCD_TEST1__           //液晶屏自检程序,绘制风格
//#define __DISPLAY_SUBTITLE1__           //在主屏中间显示副标题风格

//第一组界面
#ifdef __DISPLAY_ZHEJIANG__
#define __DISPLAY_QINGHAI__
#endif        

#ifdef __DISPLAY_ZHEJIANG1__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW1__
#define __DISPLAY_FOOTER_JIANGSU_GPRSSTATUS__
#endif        

#ifdef __DISPLAY_ZHEJIANG2__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW2__
#endif        

#ifdef __DISPLAY_ZHEJIANG3__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW3__
#endif        

//第二组界面
//字体
#ifdef __DISPLAY_ZHEJIANG4__
#define __DISPLAY_QINGHAI__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__
#define __DISPLAY_LCD_TEST2__ 
#endif        

//字体
#ifdef __DISPLAY_ZHEJIANG5__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW1__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__ 
#define __DISPLAY_LCD_TEST2__ 
#endif        

//字体
#ifdef __DISPLAY_ZHEJIANG6__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW2__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__ 
#define __DISPLAY_LCD_TEST2__ 
#endif        

//字体
#ifdef __DISPLAY_ZHEJIANG7__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW3__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__ 
#define __DISPLAY_LCD_TEST2__ 
#endif        

//第三组界面
//字体
#ifdef __DISPLAY_ZHEJIANG8__
#define __DISPLAY_QINGHAI__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_HEADER_LINE1__
#define __DISPLAY_FOOTER_LINE1__
#define __DISPLAY_LCD_TEST4__ 
#endif        

//字体
#ifdef __DISPLAY_ZHEJIANG9__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW1__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_LCD_TEST4__ 
#endif        

//字体
#ifdef __DISPLAY_ZHEJIANG10__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW2__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_HEADER_LINE1__
#define __DISPLAY_FOOTER_LINE1__
#define __DISPLAY_LCD_TEST4__ 
#endif        

//字体
#ifdef __DISPLAY_ZHEJIANG11__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW3__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_HEADER_LINE1__
#define __DISPLAY_FOOTER_LINE1__
#define __DISPLAY_LCD_TEST4__ 
#endif        


#if defined(STACK_USE_GPRS_PPP) &&  defined(__USE_SMS__)
    #define STACK_USE_SMS
#endif

#ifdef __PLC_READ_VIP_F25__
	#define __PLC_READ_VIP_V__
	//#define __PLC_READ_VIP_I__
	//#define __PLC_READ_VIP_YP__
	//#define __PLC_READ_VIP_WP__
	//#define __PLC_READ_VIP_Q__
        //#define __PLC_READ_VIP_WG_4XX__
#endif

#if defined(__PLC_READ_VIP_LOAD_RECORD__)
    #define __PLC_READ_VIP_YP__
    #define __PLC_READ_VIP_WP__
    #define __PLC_READ_VIP_V__
    #define __PLC_READ_VIP_I__
    #define __PLC_READ_VIP_Q__
    #define __PLC_READ_VIP_WG_4XX__
#endif

//线损分析
#ifdef __COMPUTE_XLOST__
    #define __COMPUTE_XLOST_30DAY__
    #define __COMPUTE_PHASE_XLOST__
    #ifndef __PROVICE_ZHEJIANG__ 
//    #define __LOST_CHECK__           //只搜一次表
    #endif
#endif

//交采广播带回复
//#define  __ADJ_ACK__


#ifdef __SOFT_SIMULATOR__
#undef __ENABLE_ESAM2__
#undef __ENABLE_ESAM__
#endif

#ifdef BASE_STATION_CHANGE
#define CPIN_CHECK    		  1
#define UN_LOCK_CHANNEL       2
#endif

#endif  // fule


