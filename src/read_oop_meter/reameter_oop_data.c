#include "readmeter_oop_data.h"
#include "../main_include.h"

//#define __DEBUG_
//BOOLEAN oad_search_in_hold_list( INT8U oad[4],tagREADITEM *read_item)
//{
	//INT8U  idx_list;
	//INT16U sub_idx;
	//INT16U sub_cnt;
	//tagREADITEM *item; 
	//for(idx_list=0;idx_list<sizeof(hold_data_list)/sizeof(READ_LIST_PROP);idx_list++)
	//{
		//sub_cnt = hold_data_list[idx_list].read_item_count;
		//item    = hold_data_list[idx_list].read_item_list;
		//for(sub_idx=0;sub_idx<sub_cnt;sub_idx++)
		//{
			////
			//if(item[sub_idx].oad == bin2_int32u(oad))
			//{
				////
				////*page_id = item[sub_idx].page_id;
				//mem_cpy((void *)read_item,(void *)&item[sub_idx],sizeof(tagREADITEM));
				//return TRUE;
			//}
		//}
	//}
//
	//return FALSE;
//}
extern BOOLEAN oad_search_in_hold_list(INT8U list_idx, INT8U oad[4], tagREADITEM *read_item);
extern BOOLEAN oad_search_in_cur_list(INT8U list_idx,INT8U oad[4],tagREADITEM *read_item);
BOOLEAN oad_search_in_list(INT8U type, INT8U list_idx,INT8U oad[4],tagREADITEM *read_item)
{
	//INT8U type;
	switch(type)
	{
		case READ_TYPE_HOLD://����
			return oad_search_in_hold_list(list_idx,oad,read_item);
		case READ_TYPE_CUR:
			/*  */
			return oad_search_in_cur_list(list_idx,oad,read_item);
			break;
		default:
			return FALSE;
	}
	return FALSE;
}

void clr_process_mask(READ_PARAMS *read_param)
{
	// 
	INT8U idx;
	INT8U tsk_idx = read_param->idx_tsk_attr;
	for(idx = 0;idx<read_param->norml_task_save_params.group_oad_params.oad_count;idx++)
	{
		clr_bit_value(gTaskPlanAttribute[tsk_idx].procs_mask, 8, 
					read_param->norml_task_save_params.group_oad_params.mask_idx[idx]);
	}
}
void save_grp_read_oad_params(GROUP_OAD_PARAMS *grp_oad_params,tagREADITEM read_item,INT8U mask_idx)
{
	INT8U pos = 0;

	/* test */
	INT32U master = 0x50040200;
	
	pos = grp_oad_params->oad_count;
	if(!grp_oad_params->oad_count)
	{
		// ��һ��OAD ??
		//mem_cpy(grp_oad_params->master_oad.object_id,(void *)&master,4);
		grp_oad_params->page_cnt  = read_item.page_count;
		grp_oad_params->page_id   = read_item.page_id;
		grp_oad_params->save_size = read_item.block_data_len;
	}
	/* OAD �洢ƫ��  Ԥ��Ӧ�𳤶� 
	 *���룬���ڴ洢��ʱ��ʹ��?? 
	 */
	mem_cpy_reverse(grp_oad_params->oad[pos],(void *)&read_item.oad, 4);
	mem_cpy(grp_oad_params->save_off[pos],(void *)&read_item.offset,2);
	set_bit_value(grp_oad_params->mask, 4, grp_oad_params->oad_count);
	/* �� plan�е� mask ���� */
	grp_oad_params->mask_idx[grp_oad_params->oad_count] = mask_idx;

	
	grp_oad_params->resp_byte_num[pos] = read_item.data_len;//read_item.item_count * read_item.data_len;
	
	grp_oad_params->oad_count++;
}


/*  ��ȡ����OAD��صĲ��� */
BOOLEAN get_plan_oad_params(READ_PARAMS *read_param,INT16U obis, INT8U index)
{
	/*  */
	INT8U process_mask[8];
	INT8U **oad;
	INT8U idx;
	INT8U max_idx;
	INT8U tsk_idx;
	INT16U page_id = 0xFFFF;
	INT16U tmp_pgid;
	GROUP_OAD_PARAMS *grp_oad_params;
	tagREADITEM read_item;
	INT8U type = READ_TYPE_CUR;/* ���� ���� ʵʱ ����?? */
	
	switch(obis)
	{
		case 0x5002: /* ���Ӷ��� */
		case 0x5003: /* Сʱ���� */
		case 0x5004: /* �ն��� */
		case 0x5005: /* �����ն��� */
		case 0x5006: /* �¶��� */
		case 0x5007: /* �궳�� */
			type = READ_TYPE_HOLD;
			break;
		default:
			
			break;
	}
	/* ���ҵ�һ����Ȼ����ͬ��ģ�ͬ��ģ���������ͬpage�滮�Ŀռ�
	 * �����ҵ��ĳ���OAD��һ������˳�򣬴����������ڴ洢��ʱ��������֯���ݣ�Ȼ��洢��
	 * 
	 */
	//idx            = read_param->norml_task_save_params.oad_idx;
	tsk_idx = read_param->idx_tsk_attr;
	if(tsk_idx > 10)
	{
		// TODO:
		
	}
	#if 0
	max_idx        = read_param->norml_task_save_params.max_oad_cnt;
	grp_oad_params = (GROUP_OAD_PARAMS *)&(read_param->norml_task_save_params.group_oad_params);
	mem_set(grp_oad_params->value,sizeof(GROUP_OAD_PARAMS),0x00);
	process_mask = read_param->norml_task_save_params.process_mask;
	#else
	max_idx        = gTaskPlanAttribute[tsk_idx].oadcnt;
	grp_oad_params = (GROUP_OAD_PARAMS *)&(read_param->norml_task_save_params.group_oad_params);
	mem_set(grp_oad_params->value,sizeof(GROUP_OAD_PARAMS),0x00);
	mem_cpy(process_mask, gTaskPlanAttribute[tsk_idx].procs_mask, 8);
	#endif
	//oad = read_param->norml_task_save_params.plan_oad;
	//oad     = gTaskPlanAttribute[tsk_idx].planOad;
	//page_id = 0;
	if(type == READ_TYPE_HOLD)
	{
		// û��Ҫ
		//mem_cpy(grp_oad_params->master_oad.value,read_param->oad,4);
	}
	for(idx=0;idx< max_idx;idx++)  //max_idx ��ʾ���������ж��ٸ�oad
	{
		if(get_bit_value(process_mask, 8, idx))
		{
			if(oad_search_in_list(type,index,gTaskPlanAttribute[tsk_idx].planOad[idx],&read_item))
			{
				if(page_id != 0xFFFF)/* ���ڵĻ�,�鿴�Ƿ�page_offset ��ͬ */
				{
					/* PAGE_ID��ͬ���Ѳ��������� */
					if(page_id == read_item.page_id)
					{
						if(grp_oad_params->oad_count >= 32)
						{
							/* ���õĴ����� ������?? */
							clr_bit_value(process_mask, 8, idx);
							continue;
						}

						/* ���ݴ洢 */
						save_grp_read_oad_params(grp_oad_params, read_item, idx);
						clr_bit_value(process_mask, 8, idx);
					}
				}
				else
				{
					
					page_id = read_item.page_id;
					/* ���ID���ϴβ�һ���������last_td */
					if(page_id != bin2_int16u(gTaskPlanAttribute[tsk_idx].page_id))
					{
						//set_cosem_datetime_s(read_param->last_td, gTaskPlanAttribute[idx].last_cycle);
						gTaskPlanAttribute[idx].read_num = 0;
						
					}
					/* ���ݴ洢 */
					save_grp_read_oad_params(grp_oad_params, read_item, idx);
					/* �������ظ���������ʱ��� TODO: */
					clr_bit_value(process_mask, 8, idx);
				}
			}
			else
			{
				clr_bit_value(process_mask, 8, idx);
			}
		}
	}

	if(grp_oad_params->oad_count)
	{
		/* PAGE_ID */
		mem_cpy(gTaskPlanAttribute[tsk_idx].page_id,(void *)&page_id,2);
		return TRUE;
	}

	//�Ҳ������Ͱ� mask������
	mem_set(gTaskPlanAttribute[tsk_idx].procs_mask,8,0x00);
	return FALSE;
}

/*
 * ���ܣ���鷽��ִ�е�ǰʱ�������ڳ���ƥ����
 * cycle_time-��ǰʱ�䣬exec_time-��������ʼʱ�䣬cycle-TI��cycle_unit-���ڵ�λ
 * �жϷ�ʽ�����ڳ���ʱ������TI���жϵ�ǰʱ���Ƿ����䷶Χ֮��
 */
BOOLEAN check_task_execute_cycle(INT8U cycle_time[7],INT8U exec_time[7],INT16U cycle, INT8U cycle_unit)
{
    INT8U  cur_exec_time[7]={0};
    
    mem_cpy(cur_exec_time, exec_time, 7);
    
    switch (cycle_unit)
    {
        case 0://�룬����ʱ���ڵ�λΪ���Ϊ֧�֣��ݲ�֧��,֧��֮����Խ�ע�����ַſ�������в���
            return FALSE;
            //datetime_add_seconds(cur_exec_time + YEAR,cur_exec_time + MONTH,cur_exec_time + DAY,cur_exec_time + HOUR,cur_exec_time + MINUTE,cur_exec_time + SECOND,cycle);
            break;
        case 1://��
            datetime_add_minutes(cur_exec_time + YEAR,cur_exec_time + MONTH,cur_exec_time + DAY,cur_exec_time + HOUR,cur_exec_time + MINUTE,cycle);
            break;
        case 2://ʱ
            datetime_add_hours(cur_exec_time + YEAR,cur_exec_time + MONTH,cur_exec_time + DAY,cur_exec_time + HOUR,cycle);
            break;
        case 3://��
            date_add_days(cur_exec_time + YEAR,cur_exec_time + MONTH,cur_exec_time + DAY,cycle);
            break;
        case 4://��
            date_add_months(cur_exec_time + YEAR,cur_exec_time + MONTH,cycle);
            break;
        case 5://��,����Ϊ���Ŀǰ�����в�֧�֣��˴�Ҳ�������������������,ֱ�Ӱ���ʱ�겻��ȷ����
            return FALSE;
            break;
    }
    // ��ǰʱ�������ʼ���ڣ�����һ���ڿ�ʼʱ����ڵ�ǰʱ�䣬��ˢ�� 
    if((compare_string_reverse(cycle_time,exec_time,7) >= 0)&&(compare_string_reverse(cur_exec_time,cycle_time,7) > 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}



/* start_time ��������ʱ�� ��һ�ε�ʱ�򣬲�����䵱ǰʱ���
 * 
 */
void check_read_cycle_one_task()//BOOLEAN first)
{
	INT8U idx;
	INT8U sub_idx;
	INT8U unit;//��λ 
	INT8U num; // ��ֵ
	INT8U cur_time[7];
	INT8U date_time_cur[8];
	INT8U check_val = 0x00; // 00 OR  ff 
	DateTime dt;
	BOOLEAN is_valid = FALSE;
	INT8U   acq_unit = 0;
	INT16U  acq_val  = 0;
	tpos_datetime(&dt);
	//if(first)
	{
		mem_cpy(cur_time,dt.value,6);
		cur_time[6] = dt.century;
		#ifdef __DEBUG_
		system_debug_data(cur_time,7);
		#endif
		mem_cpy(date_time_cur, cur_time, 7);
	}
	for(idx=0;idx<MAX_TASK_COUNT;idx++)
	{
		is_valid = FALSE;
		if(gTaskPlanAttribute[idx].task_id != 0xFF)
		{
			/*  */
			unit = gTaskPlanAttribute[idx].run_unit;
			num  = gTaskPlanAttribute[idx].run_frq;
			if(num >0)
			{
				switch(unit)
				{
					case 0://��
						break;
					case 1: // ��
						if(!check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit)  //��ǰʱ��
                           && !(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val)))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = (INT8U)(date_time_cur[MINUTE] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							datetime_minus_minutes(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							gTaskPlanAttribute[idx].last_cycle + DAY,
							gTaskPlanAttribute[idx].last_cycle + HOUR,
							gTaskPlanAttribute[idx].last_cycle + MINUTE,
							num);
							is_valid = TRUE;
						}
						//��һ��ȫFF����ȫ00����˵��
						else if(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = (INT8U)(date_time_cur[MINUTE] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							datetime_minus_minutes(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							gTaskPlanAttribute[idx].last_cycle + DAY,
							gTaskPlanAttribute[idx].last_cycle + HOUR,
							gTaskPlanAttribute[idx].last_cycle + MINUTE,
							num);
							if(check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit))
							{
								is_valid = TRUE;
							}
						}
						break;
					case 2:// Сʱ
						if(!check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit)
                           && !(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val)))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = 0;
							gTaskPlanAttribute[idx].cycle_date[HOUR] = (INT8U)(date_time_cur[HOUR] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							datetime_minus_hours(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							gTaskPlanAttribute[idx].last_cycle + DAY,
							gTaskPlanAttribute[idx].last_cycle + HOUR,
							num);
							is_valid = TRUE;
						}
						//��һ��ȫFF����ȫ00����˵��
						else if(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = 0;
							gTaskPlanAttribute[idx].cycle_date[HOUR] = (INT8U)(date_time_cur[HOUR] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							datetime_minus_hours(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							gTaskPlanAttribute[idx].last_cycle + DAY,
							gTaskPlanAttribute[idx].last_cycle + HOUR,
							num);
							if(check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit))
							{
								is_valid = TRUE;
							}
						}
						break;
					case 3:// ��
						if(!check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit)
                           && !(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val)))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = 0;
							gTaskPlanAttribute[idx].cycle_date[HOUR]   = 0;
							gTaskPlanAttribute[idx].cycle_date[DAY] = (INT8U)(date_time_cur[DAY] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							date_minus_days(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							gTaskPlanAttribute[idx].last_cycle + DAY,
							num);
							is_valid = TRUE;
						}
						//��һ��ȫFF����ȫ00����˵��
						else if(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = 0;
							gTaskPlanAttribute[idx].cycle_date[HOUR]   = 0;
							gTaskPlanAttribute[idx].cycle_date[DAY] = (INT8U)(date_time_cur[DAY] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							date_minus_days(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							gTaskPlanAttribute[idx].last_cycle + DAY,
							num);
							if(check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit))
							{
								is_valid = TRUE;
							}
						}
						break;
					case 4:// ��
						if(!check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit)
                           && !(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val)))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = 0;
							gTaskPlanAttribute[idx].cycle_date[HOUR]   = 0;
							gTaskPlanAttribute[idx].cycle_date[DAY]    = 1;
							//gTaskPlanAttribute[idx].cycle_date[MINUTE] = (INT8U)(date_time_cur[MINUTE] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							date_minus_months(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							num);
							is_valid = TRUE;
						}
						//��һ��ȫFF����ȫ00����˵��
						else if(check_is_all_ch(gTaskPlanAttribute[idx].last_cycle, 7, check_val))
						{
							mem_cpy(gTaskPlanAttribute[idx].cycle_date, date_time_cur, 7);
							gTaskPlanAttribute[idx].cycle_date[SECOND] = 0;
							gTaskPlanAttribute[idx].cycle_date[MINUTE] = 0;
							gTaskPlanAttribute[idx].cycle_date[HOUR]   = 0;
							gTaskPlanAttribute[idx].cycle_date[DAY]    = 1;
							gTaskPlanAttribute[idx].cycle_date[MONTH]  = (INT8U)(date_time_cur[MONTH] / num) * num;
							mem_cpy(gTaskPlanAttribute[idx].last_cycle, gTaskPlanAttribute[idx].cycle_date, 7);
							date_minus_months(gTaskPlanAttribute[idx].last_cycle + YEAR,
							gTaskPlanAttribute[idx].last_cycle + MONTH,
							num);
							if(check_task_execute_cycle(date_time_cur, gTaskPlanAttribute[idx].cycle_date, num, unit))
							{
								is_valid = TRUE;
							}
						}
						break;
					default:
						break;
				}
				if(is_valid)
				{
					mem_set(gTaskPlanAttribute[idx].mask,8,0x00);
					mem_set(gTaskPlanAttribute[idx].procs_mask,8,0x00);
					
					for(sub_idx=0;sub_idx<gTaskPlanAttribute[idx].oadcnt;sub_idx++)
					{
						set_bit_value(gTaskPlanAttribute[idx].mask, 8, sub_idx);
					}
					mem_cpy(gTaskPlanAttribute[idx].procs_mask,gTaskPlanAttribute[idx].mask,8);

					// ����ctrl flag
					gTaskPlanAttribute[idx].ctl_flg.in_cycle  = 1;
					gTaskPlanAttribute[idx].ctl_flg.new_cycle = 1;

					/* �ɼ����� */ 
					switch(gTaskPlanAttribute[idx].acq_type)
					{
						#if 0
						case 3:// TI
							/* ��λ  */
							
							acq_unit = gTaskPlanAttribute[idx].acq_content[0];
							acq_val  = cosem_bin2_int16u(gTaskPlanAttribute[idx].acq_content + 1);
							if(gTaskPlanAttribute[idx].run_unit < acq_unit)
							 ||(acq_val == 0)
							{
								/* ֻ�ɼ�һ���� */
								gTaskPlanAttribute[idx].point_num = 1;
							}
							else
							{
								/* �����°� */
								gTaskPlanAttribute[idx].point_num = get_point_num(acq_unit,acq_val,
								                                           gTaskPlanAttribute[idx].run_unit,
								                                           gTaskPlanAttribute[idx].run_frq);
							}
							break;
						#endif
						default:
							gTaskPlanAttribute[idx].point_num = 1;
							break;
					}
					
				}
			}
		}
	}
	
}

/* OOP���ݳ���
 * make frame ->send -> wait resp ->???
 * 
 */
extern void read_meter_fixed_time_interval_hold_data(INT16U obis,READ_PARAMS *read_param);
void read_oop_data(READ_PARAMS *read_params)
{
	/*  */
	METER_DOCUMENT meter_doc;
    INT16U  obis = 0;
	mem_cpy((void *)&meter_doc,(void *)&gmeter_doc,sizeof(METER_DOCUMENT));

	// mast oad obis 
	obis = cosem_bin2_int16u(read_params->oad.object_id); //��OAD
	switch(obis)
	{
		

		case 0x5002: /* ���Ӷ��� */
		case 0x5003: /* Сʱ���� */
		case 0x5004: /* �ն��� */
		case 0x5005: /* �����ն��� */
		case 0x5006: /* �¶��� */
		case 0x5007: /* �궳�� */
			read_meter_fixed_time_interval_hold_data(obis,read_params);
			break;
		default:
			read_meter_cur_data(obis,read_params);
			break;
			//read_meter_fixed_time_interval_hold_data();

	}
}
INT8U calculate_point_num(READ_PARAMS *read_params,INT8U ti[3])
{
	INT16U obis;
	INT8U td[7];
	INT8U num = 1;
	mem_cpy(td,read_params->last_td,7);

	obis = cosem_bin2_int16u(read_params->oad.object_id);
	if( (obis == 0x5002) || (obis == 0x5003) )
	{
		if( (ti[0] > 4) || (cosem_bin2_int16u(ti+1) == 0) )
		{
			return 1;
		}
		while(1)
		{
			cosem_datetime_add_ti(td, ti);
			if(compare_string(td,read_params->hold_td,7) < 0 )
			{
				num++;
			}
			else
			{
				break;
			}
		}

		if(num > 96)
		{
			num = 96;
		}
		return num;
	}

	return 1;
	
}
void prepare_read_oop_data_cycle(READ_PARAMS *read_params)  //׼��ѭ��
{
	/* */
	static INT8U last_minute[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	DateTime sys_datetime;
	INT8U idx;
	BOOLEAN read_flg;
	INT16U obis = 0;
	OBJECT_ATTR_DESC1   oad;
	tpos_datetime(&sys_datetime);

	if( (last_minute[MINUTE]!=sys_datetime.value[MINUTE])||(last_minute[HOUR]!=sys_datetime.value[HOUR])
      ||(last_minute[DAY]!=sys_datetime.value[DAY])||(last_minute[MONTH]!=sys_datetime.value[MONTH])
      ||(last_minute[YEAR]!=sys_datetime.value[YEAR]) )
    {
    	/* ÿ����ˢ��һ�� */
    	last_minute[MINUTE] = sys_datetime.value[MINUTE];
        last_minute[HOUR]   = sys_datetime.value[HOUR];
        last_minute[DAY]    = sys_datetime.value[DAY];
        last_minute[MONTH]  = sys_datetime.value[MONTH];
        last_minute[YEAR]   = sys_datetime.value[YEAR];
        check_read_cycle_one_task();
    }

    for(idx=0;idx<MAX_TASK_COUNT;idx++)
    {
    	read_flg = FALSE;
    	if(gTaskPlanAttribute[idx].task_id != 0x00)
    	{
			/*  */
			mem_cpy(oad.value,gTaskPlanAttribute[idx].master_oad,4);
			obis = cosem_bin2_int16u(oad.object_id);

			if( (0 == sys_datetime.value[HOUR]) && (0 == sys_datetime.value[MINUTE]) )
			{
				if(sys_datetime.value[SECOND] < 30)
				{
					/* �ն��� */
					if( (0x5004 == obis) || (0x5005 == obis) || (0x5006 == obis) )
					{
						continue;
					}
				}
			}

    		/*  */
    		if(gTaskPlanAttribute[idx].ctl_flg.new_cycle)  //�÷�����������������Ҫ���ϸ����ڵ�����
    		{
    			/* clr */
    			mem_set(read_params->value,sizeof(READ_PARAMS),0x00);
    			// id ;  ��������ȫ��������,������˼ �Ǻ� ; 
    			//master_oad:ctrl�������ߵ�  acq_type:�ɼ�����
    			read_params->task_idx[0] = gTaskPlanAttribute[idx].task_id;
    			read_params->idx_tsk_attr = idx;
    			mem_cpy(read_params->oad.value,gTaskPlanAttribute[idx].master_oad,4);
    			read_params->acq_type = gTaskPlanAttribute[idx].acq_type;
				//set_cosem_datetime_s(read_params->hold_td,);
				
				/* ֱ��ת��??? */
				set_cosem_datetime_s(read_params->hold_td, gTaskPlanAttribute[idx].cycle_date);
				set_cosem_datetime_s(read_params->last_td, gTaskPlanAttribute[idx].last_cycle);

				//�������
				gTaskPlanAttribute[idx].point_num = calculate_point_num(read_params,gTaskPlanAttribute[idx].acq_content);
				/* clr */
				mem_set(gTaskPlanAttribute[idx].page_id,2,0x00);
				
				
				// clr
    			gTaskPlanAttribute[idx].ctl_flg.new_cycle = 0;/*  */
    			read_flg = TRUE;
    		}
    		else if(!check_is_all_ch(gTaskPlanAttribute[idx].procs_mask, 8, 0x00))
    		{
    			/*
    			 * ����������û������ɣ��ſ��Կ����Ƿ���Ҫ����
    			 * ����������
    			 */
    			if(gTaskPlanAttribute[idx].task_id != read_params->task_idx[0])
    			{
    				// id ;  ��������ȫ��������, �Ǻ� ; 
    				//master_oad:ctrl�������ߵ�  acq_type:�ɼ�����
    				read_params->task_idx[0]  = gTaskPlanAttribute[idx].task_id;
    				read_params->idx_tsk_attr = idx;
    				mem_cpy(read_params->oad.value,gTaskPlanAttribute[idx].master_oad,4);
    				read_params->acq_type = gTaskPlanAttribute[idx].acq_type;
    				/* ֱ��ת��??? */
					set_cosem_datetime_s(read_params->hold_td, gTaskPlanAttribute[idx].cycle_date); //
					set_cosem_datetime_s(read_params->last_td, gTaskPlanAttribute[idx].last_cycle);  //����ʱ����ɶ��˼��������
					//�������
					gTaskPlanAttribute[idx].point_num = calculate_point_num(read_params,gTaskPlanAttribute[idx].acq_content);
    			}
    			/* reload procs_mask */
    			//mem_cpy(read_params->normal_task_oad_mask,gTaskPlanAttribute[idx].procs_mask,4);
    			read_flg = TRUE;
    		}
    		else
    		{
    			continue;
    		}
    	}
    	if(read_flg)
    	{
    		/* ������ɣ�Ȼ�󷵻� */
			read_oop_data(read_params);

			/* ����procs_mask */
    		return ;
    	}
    }
	
	
}

#define FLASH_BUFFER_SIZE   500
INT8U FLASH_BUFFER[FLASH_BUFFER_SIZE];




//#define DATA_HEADER_SIZE   sizeof(DATA_HEADER) 
#define MAX_PAGE_SIZE FLASH_SECTOR_SIZE   // 4096  

/* ������  ��ʱ������ TODO: */
typedef union{
    INT8U value[4];
    struct{
        INT8U transaction_flag;
        INT8U page_id[2];
        INT8U idx_data[2];
        INT8U block_size[2];
    }elmt;
}TRANSACTION_FLAG;

void read_flash_data(INT8U meter_no[6],INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count)
{
    PAGE_HEADER *page_header;
    INT32U addr;
    INT16U save_count,head_count;
    INT16U data_addr;
    INT16U idx_data,idx_page;
    INT16U idx_data_last,idx_page_last;
    INT16U sector;
    INT8U head_data_last;//,head_data;
    BOOLEAN is_find;

	// Ҫ��������ͷ��Ϊ�˶�ȡ��ʱ�����ʹ�� 
	save_size   += DATA_HEADER_SIZE;
	data_offset += DATA_HEADER_SIZE;
	
    mem_set(data,data_len,0xFF);
    if (save_size > FLASH_BUFFER_SIZE) return;
    if ((data_offset + data_len) >= FLASH_BUFFER_SIZE) return;  //����

    page_header = (PAGE_HEADER*)FLASH_BUFFER;

    save_count = MAX_PAGE_SIZE/save_size;
    head_count = save_count/8;
    if (save_count%8) head_count++;
    save_count = (MAX_PAGE_SIZE-head_count-6)/save_size;   //һҳ�ϴ漸�����ݿ�,��6���ֽڵĵ���ַ
    head_count = save_count/8;           //ҳͷ��־ռ���ֽ���
    if (save_count%8) head_count++;
    head_count += 6;  //��6���ֽڵĵ���ַ
    if ((head_count+save_size*save_count) > MAX_PAGE_SIZE) return;   //�������
    data_addr = head_count;  //ÿҳ���ݵ���ʼ��ַ

    tpos_mutexPend(&SIGNAL_FLASH);
    
    //Ѱ�����һ��д��λ�ã�
    is_find = FALSE;
    idx_data_last = save_count;
    idx_page_last = 0xFFFF;
    head_data_last = 0x00;
    for(idx_page=0;idx_page<page_count;idx_page++)
    {
        //addr = (page_id+idx_page)*MAX_PAGE_SIZE;
        sector = (page_id+idx_page);
        nor_flash_read_data(sector,0,page_header->meter_mo,head_count);
        if (meter_no)
        {
            if (check_is_all_ch(page_header->meter_mo,head_count,0xFF) == FALSE)
            {
                if (compare_string(page_header->meter_mo,meter_no,6) != 0)
                {
                    mem_set(page_header->meter_mo,head_count,0xFF);
                }
            }
        }
        else
        {
            if (check_is_all_ch(page_header->meter_mo,6,0xFF) == FALSE)
            {
                mem_set(page_header->meter_mo,head_count,0xFF);
            }
        }

        for(idx_data=0;idx_data<save_count;idx_data++)
        {
            if(get_bit_value(page_header->use_flag,head_count-6,idx_data))
            {
                is_find = TRUE;
                //head_data = page_header->use_flag[idx_data/8];
                break;
            }
            else
            {
                idx_data_last = idx_data;
                idx_page_last = idx_page;
                head_data_last = page_header->use_flag[idx_data_last/8];
            }
        }
        if (is_find) break;
    }
    
    if (idx_data_last >= save_count)
    {
        idx_data_last = save_count - 1;
        idx_page_last = page_count - 1;
        //addr = (page_id+idx_page_last)*MAX_PAGE_SIZE;
        sector = (page_id+idx_page_last);
        nor_flash_read_data(sector,0,page_header->meter_mo,head_count);
        head_data_last = page_header->use_flag[idx_data_last/8];
        if ((meter_no) && (compare_string(page_header->meter_mo,meter_no,6) != 0)) head_data_last = 0xFF;
        if ((!meter_no) && (check_is_all_ch(page_header->meter_mo,6,0xFF) == FALSE)) head_data_last = 0xFF;
    }

    if (is_find)
    {
        if ((head_data_last & (0x01<<(idx_data_last%8))) == 0)
        {
            #ifdef __SOFT_SIMULATOR__
            snprintf(info,200,"*** read_flash_data : page_id = %04d , idx_page = %04d , idx_data = %04d , save_count = %04d",page_id,idx_page,idx_data,save_count);
            debug_println(info);
            #endif
            //addr = (page_id+idx_page_last)*MAX_PAGE_SIZE;
            addr   = data_addr;
            addr  += idx_data_last * save_size;
            //addr  += data_offset;
            sector = (page_id+idx_page_last);
            //nor_flash_read_data(sector,addr,data,data_len);
            nor_flash_read_data(sector,addr,FLASH_BUFFER,save_size);

            mem_cpy(data,FLASH_BUFFER,DATA_HEADER_SIZE);
            mem_cpy(data+DATA_HEADER_SIZE,FLASH_BUFFER+data_offset,data_len -DATA_HEADER_SIZE);  //С�ĳ���
        }
    }

    tpos_mutexFree(&SIGNAL_FLASH);
}

//һ�����ݿ�����ߴ���256�ֽڣ����ܳ���256�ֽ�
void write_flash_data(HEADER_TD hdr_td,INT8U meter_no[6],INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count)
{
    PAGE_HEADER *page_header;
    INT32U addr;
    INT16U save_count,head_count;
    INT16U data_addr,idx_page_erase;
    INT16U idx_data,idx_page;
    INT16U idx_data_last,idx_page_last;
    INT16U sector;
    INT8U head_data_last;//,head_data;
    BOOLEAN is_find,is_write;

	// Ҫ��������ͷ��Ϊ�˶�ȡ��ʱ�����ʹ�� 
	save_size   += DATA_HEADER_SIZE;
	data_offset += DATA_HEADER_SIZE;
//	tmp += DATA_HEADER_SIZE;
    if (save_size > FLASH_BUFFER_SIZE) return;
    if ((data_offset + data_len) >= FLASH_BUFFER_SIZE) return;  //д����


    page_header = (PAGE_HEADER*)FLASH_BUFFER;

    save_count = MAX_PAGE_SIZE/save_size;
    head_count = save_count/8;
    if (save_count%8) head_count++;
    save_count = (MAX_PAGE_SIZE-head_count-6)/save_size;   //һҳ�ϴ漸�����ݿ�,��6���ֽڵĵ���ַ
    head_count = save_count/8;           //ҳͷ��־ռ���ֽ���
    if (save_count%8) head_count++;
    head_count += 6;  //��6���ֽڵĵ���ַ
    if ((head_count+save_size*save_count) > MAX_PAGE_SIZE) 
    {
    	// TODO: log_info????
    	return;   //�������
    }
    data_addr = head_count;  //ÿҳ���ݵ���ʼ��ַ

    tpos_mutexPend(&SIGNAL_FLASH);

    //Ѱ�����һ��д��λ�ã�
    is_find = FALSE;
    idx_data_last = save_count;
    idx_page_last = 0xFFFF;
    head_data_last = 0x00;
    for(idx_page=0;idx_page<page_count;idx_page++)
    {
        //addr = (page_id+idx_page)*MAX_PAGE_SIZE;
        sector = (page_id+idx_page);
        nor_flash_read_data(sector,0,page_header->meter_mo,head_count);
        if (meter_no)
        {
            if (check_is_all_ch(page_header->meter_mo,head_count,0xFF) == FALSE)
            {
                if (compare_string(page_header->meter_mo,meter_no,6) != 0)
                {
                    nor_flash_erase_page(sector);//(page_id+idx_page)*MAX_PAGE_SIZE);
                    #ifdef __SOFT_SIMULATOR__
                    snprintf(info,200,"*** erase_page : page_id = %04d , idx_page = %04d",page_id,idx_page_erase);
                    debug_println(info);
                    #endif
                    mem_set(page_header->meter_mo,head_count,0xFF);
                }
            }
        }
        else
        {
            if (check_is_all_ch(page_header->meter_mo,6,0xFF) == FALSE)
            {
                //if_nor_flash_se((page_id+idx_page)*MAX_PAGE_SIZE);
                nor_flash_erase_page(sector);
                #ifdef __SOFT_SIMULATOR__
                snprintf(info,200,"*** erase_page : page_id = %04d , idx_page = %04d",page_id,idx_page_erase);
                debug_println(info);
                #endif
                mem_set(page_header->meter_mo,head_count,0xFF);
            }
        }
        for(idx_data=0;idx_data<save_count;idx_data++)
        {
            if(get_bit_value(page_header->use_flag,head_count-6,idx_data))
            {
                is_find = TRUE;
                //head_data = page_header->use_flag[idx_data/8];
                break;
            }
            else
            {
                idx_data_last = idx_data;
                idx_page_last = idx_page;
                head_data_last = page_header->use_flag[idx_data_last/8];
            }
        }
        if (is_find) break;
    }
    
    if (idx_data_last >= save_count)
    {
        idx_data_last = save_count - 1;
        idx_page_last = page_count - 1;
        //addr = (page_id+idx_page_last)*MAX_PAGE_SIZE;
        sector = (page_id+idx_page_last);
        nor_flash_read_data(sector,0,page_header->meter_mo,head_count);
        head_data_last = page_header->use_flag[idx_data_last/8];
        if ((meter_no) && (compare_string(page_header->meter_mo,meter_no,6) != 0)) head_data_last = 0xFF;
        if ((!meter_no) && (check_is_all_ch(page_header->meter_mo,6,0xFF) == FALSE)) head_data_last = 0xFF;
    }

    is_write = TRUE; //��Ҫд����
    if (is_find)
    {
        if ((head_data_last & (0x01<<(idx_data_last%8))) == 0)
        {
            //addr = (page_id+idx_page_last)*MAX_PAGE_SIZE;
            addr  = data_addr;
            addr += idx_data_last * save_size;
			sector = (page_id+idx_page_last);
            nor_flash_read_data(sector,addr,FLASH_BUFFER,save_size);
system_debug_data(&data_offset,4);
//            if ((compare_string(FLASH_BUFFER+data_offset,data,data_len) != 0))
			if ((compare_string(FLASH_BUFFER+data_offset,data,data_len) != 0)||(compare_string(hdr_td.tsk_start_time,FLASH_BUFFER+5,7) != 0))			
            {
                if (check_is_all_ch(FLASH_BUFFER+data_offset,data_len,0xFF) == FALSE)   //��Ҫ��һ���µ�λ��
                {
                    if (idx_data >= (save_count - 1))  //д��ҳ�����һλ��ʱ�������¸�ҳ
                    {
                        idx_page_erase = idx_page;
                        idx_page_erase++;
                        if (idx_page_erase >= page_count) idx_page_erase = 0;
                        nor_flash_erase_page((page_id+idx_page_erase));//*MAX_PAGE_SIZE);
                        #ifdef __SOFT_SIMULATOR__
                        snprintf(info,200,"*** erase_page : page_id = %04d , idx_page = %04d",page_id,idx_page_erase);
                        debug_println(info);
                        #endif
                    }
                }
                else  //ʹ�þɵ�λ��
                {
                    //head_data = head_data_last;
                    idx_page = idx_page_last;
                    idx_data = idx_data_last;
                    idx_data_last = 0xFFFF;
                }
            }
            else is_write = FALSE;  //һģһ��������д��
        }
        else
        {
            idx_data_last = 0xFFFF;  
        }
    }
    else
    {
        idx_data_last = 0xFFFF;
        idx_page = 0;
        idx_data = 0;
        //head_data = 0xFF;
        nor_flash_erase_page((page_id+idx_page));//*MAX_PAGE_SIZE);
        #ifdef __SOFT_SIMULATOR__
        snprintf(info,200,"*** Warning : erase_page  page_id = %04d , idx_page = %04d",page_id,idx_page_erase);
        debug_println(info);
        #endif
    }

    if (is_write)
    {
        #ifdef __SOFT_SIMULATOR__
        snprintf(info,200,"*** write_flash_data : page_id = %04d , idx_page = %04d , idx_data = %04d , save_count = %04d",page_id,idx_page,idx_data,save_count);
        debug_println(info);
        #endif
        //addr = (page_id+idx_page)*MAX_PAGE_SIZE;
        addr   = data_addr;
        addr  += idx_data * save_size;
		sector = (page_id+idx_page);

		mem_set(FLASH_BUFFER,4,0xFF);
		mem_cpy(FLASH_BUFFER+4,hdr_td.tsk_start_time,24);
        if (idx_data_last < save_count)  //��Ҫ�������ݿ飬����֮ǰ���ݣ�����������copy������Ȼ��д��
        {
            mem_cpy(FLASH_BUFFER+data_offset,data,data_len);
            nor_flash_write_data(sector,addr,FLASH_BUFFER,save_size);
        }
        else //ֱ��д��
        {
			mem_cpy(FLASH_BUFFER+data_offset,data,data_len);
            nor_flash_write_data(sector,addr,FLASH_BUFFER,save_size);
//          nor_flash_write_data(sector,addr+data_offset,data,data_len);
        }
        //дuse_flag
        //addr = (page_id+idx_page)*MAX_PAGE_SIZE;
        nor_flash_read_data(sector,0,page_header->meter_mo,head_count);
        if (meter_no) mem_cpy(page_header->meter_mo,meter_no,6);
        clr_bit_value(page_header->use_flag,head_count-6,idx_data);
        nor_flash_write_data(sector,0,page_header->meter_mo,head_count);
    }
	

		system_debug_data(&data_offset,4);
//		system_debug_data(&p,4);
    tpos_mutexFree(&SIGNAL_FLASH);
}

//save_size�����ߴ���(FLASH_BUFFER_SIZE - DATA_HEADER_SIZE)�ֽڣ����ܳ���FLASH_BUFFER_SIZE�ֽ�
//���ݵĿ�ʼ�洢��DATA_HEADER�ṹ,��ռ��DATA_HEADER_SIZE���ֽ�
void read_flash_data_from_save_idx(INT32U save_idx,INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count)
{
    INT32U addr,idx_data;
    INT16U save_count,max_count;
    INT16U idx_page;
    INT16U sector = 0;

    mem_set(data,data_len,0xFF);

    if ((save_size + DATA_HEADER_SIZE) > FLASH_BUFFER_SIZE) return;
    if ((data_offset + DATA_HEADER_SIZE + data_len) >= FLASH_BUFFER_SIZE) return;  //д����

    save_count = (MAX_PAGE_SIZE - sizeof(TRANSACTION_FLAG)) / (save_size + DATA_HEADER_SIZE); //ÿҳ�ϵĿ���
    max_count = save_count * page_count;           //һ����Ŀ���
    idx_data = save_idx % max_count;
    idx_page = idx_data / save_count;              //����һҳ��
    idx_data = idx_data % save_count;              //��ҳ�ڵĵڼ���λ��
	
    tpos_mutexPend(&SIGNAL_FLASH);

    sector = (page_id+idx_page);
    addr   = idx_data * (save_size + DATA_HEADER_SIZE);
    #ifdef __DEBUG_
	system_debug_data((void *)&sector,2);
	system_debug_data((void *)&addr,2);
	#endif
    nor_flash_read_data(sector,addr,FLASH_BUFFER,save_size+DATA_HEADER_SIZE);
    if (compare_string((INT8U*)&save_idx,FLASH_BUFFER,4) == 0) 
    {

        mem_cpy(data,FLASH_BUFFER,DATA_HEADER_SIZE);  //ʱ���ڴ�,�����ʱ��
		mem_cpy(data+DATA_HEADER_SIZE,FLASH_BUFFER+data_offset+DATA_HEADER_SIZE,data_len - DATA_HEADER_SIZE);//������������ݺ���һ��ƫ��
        #ifdef __SOFT_SIMULATOR__
        snprintf(info,200,"*** read_flash_data_from_save_idx : page_id = %04d , idx_page = %04d , idx_data = %04d , save_count = %04d , save_idx = %d",page_id,idx_page,idx_data,save_count,save_idx);
        debug_println(info);
        #endif
    }

    tpos_mutexFree(&SIGNAL_FLASH);
}

//save_size�����ߴ���(FLASH_BUFFER_SIZE - DATA_HEADER_SIZE)�ֽڣ����ܳ���FLASH_BUFFER_SIZE�ֽ�
//���ݵĿ�ʼ�洢��DATA_HEADER�ṹ��,��ռ��DATA_HEADER_SIZE���ֽ�
void write_flash_data_from_save_idx(HEADER_TD hdr_td,INT32U save_idx,INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count)
{
    INT16U addr,idx_data,idx;
    INT16U save_count,max_count;
    INT16U idx_page;
    INT16U sector = 0;
    INT16U tmp_len;
    INT8U erase_type;
    TRANSACTION_FLAG transaction_flag;

    erase_type = 0;
    if ((save_size + DATA_HEADER_SIZE) > FLASH_BUFFER_SIZE) return;
    if ((data_offset + DATA_HEADER_SIZE + data_len) >= FLASH_BUFFER_SIZE) return;  //д����

    save_count = (MAX_PAGE_SIZE - sizeof(TRANSACTION_FLAG)) / (save_size + DATA_HEADER_SIZE); //ÿҳ�ϵĿ���
    max_count = save_count * page_count;           //һ����Ŀ�����Ҳ���Ǵ���ٴ����ݵĸ��� count 
    idx_data = save_idx % max_count; // saveidx �ϴ�ȡ�������0 ~ max_cont-1��Χ��
    idx_page = idx_data / save_count;              //����һҳ��
    idx_data = idx_data % save_count;              //��ҳ�ڵĵڼ���λ��
	
    tpos_mutexPend(&SIGNAL_FLASH);

	/*  */
	sector = page_id+idx_page;
    addr   = idx_data * (save_size + DATA_HEADER_SIZE);
    nor_flash_read_data(sector,addr,FLASH_BUFFER,save_size+DATA_HEADER_SIZE);
    if ((check_is_all_ch(FLASH_BUFFER,save_size+DATA_HEADER_SIZE,0xFF) == FALSE)  //д����
      && (compare_string((INT8U*)&save_idx,FLASH_BUFFER,4) != 0) )
    {
        erase_type = 0xAA; //�����������������¼�鴦��
    }
    else if ((check_is_all_ch(FLASH_BUFFER+DATA_HEADER_SIZE+data_offset,data_len,0xFF) == FALSE) 
          && (compare_string(FLASH_BUFFER+DATA_HEADER_SIZE+data_offset,data,data_len) != 0))
    {
        erase_type = 0x55; //���ڲ���
    }
    
    /* ������Ϣ */
    tmp_len = (save_size + DATA_HEADER_SIZE);
    if (erase_type) 
    {
        if (idx_data > 0) //�����ݰ���������ҳ��˵����Ҫ�Ȱ������ݣ�Ȼ����� Ȼ���ٰ������
        {
            #ifdef __SOFT_SIMULATOR__
            snprintf(info,200,"*** erase_page : data_buffer_page_id = %04d",PAGE_ID_DATA_BUFFER);
            debug_println(info);
            #endif
            nor_flash_erase_page(PAGE_ID_DATA_BUFFER);  //��������ҳ
            //addr_tmp = (page_id+idx_page) * MAX_PAGE_SIZE;
            
            for(idx=0;idx<idx_data;idx++)
            {
                nor_flash_read_data(sector,idx*tmp_len,FLASH_BUFFER,tmp_len);
                if (check_is_all_ch(FLASH_BUFFER,tmp_len,0xFF) == FALSE)
                {
                    nor_flash_write_data(PAGE_ID_DATA_BUFFER,idx*tmp_len,FLASH_BUFFER,tmp_len);
                }
            }
            
            if (erase_type == 0x55) //���ڲ�����������Ҳ���ȥ
            {
                nor_flash_read_data(sector,idx_data*tmp_len,FLASH_BUFFER,tmp_len);
                if (check_is_all_ch(FLASH_BUFFER,tmp_len,0xFF) == FALSE)
                {
                    nor_flash_write_data(PAGE_ID_DATA_BUFFER,idx_data*tmp_len,FLASH_BUFFER,tmp_len);
                }
            }

            //��������
            transaction_flag.elmt.transaction_flag = 0xE7;
            transaction_flag.elmt.page_id[0] = (page_id+idx_page);
            transaction_flag.elmt.page_id[1] = (page_id+idx_page)>>8;
            transaction_flag.elmt.idx_data[0] = idx_data;
            transaction_flag.elmt.idx_data[1] = idx_data>>8;
            transaction_flag.elmt.block_size[0] = save_size+10;
            transaction_flag.elmt.block_size[1] = (save_size+10)>>8;
            nor_flash_write_data(PAGE_ID_DATA_BUFFER,MAX_PAGE_SIZE-sizeof(TRANSACTION_FLAG),transaction_flag.value,sizeof(TRANSACTION_FLAG));
        }
        
        #ifdef __SOFT_SIMULATOR__
        snprintf(info,200,"*** erase_page : page_id = %04d , idx_page = %04d",page_id,idx_page);
        debug_println(info);
        #endif
        nor_flash_erase_page((page_id+idx_page));  //����ҳ
        
        if (idx_data > 0) //�����ݴӻ���ҳ���
        {
            //��������
            transaction_flag.elmt.transaction_flag = 0xC3;
            nor_flash_write_data(PAGE_ID_DATA_BUFFER,MAX_PAGE_SIZE-sizeof(TRANSACTION_FLAG),transaction_flag.value,sizeof(TRANSACTION_FLAG));

            //addr_tmp = (page_id+idx_page) * MAX_PAGE_SIZE;
            for(idx=0;idx<idx_data;idx++)
            {
                nor_flash_read_data(PAGE_ID_DATA_BUFFER,idx*tmp_len,FLASH_BUFFER,tmp_len);
                if (check_is_all_ch(FLASH_BUFFER,tmp_len,0xFF) == FALSE)
                {
                    nor_flash_write_data(sector,idx*tmp_len,FLASH_BUFFER,tmp_len);
                }
            }
            
            //��������  �������
            transaction_flag.elmt.transaction_flag = 0x00;
            nor_flash_write_data(PAGE_ID_DATA_BUFFER,MAX_PAGE_SIZE-sizeof(TRANSACTION_FLAG),transaction_flag.value,sizeof(TRANSACTION_FLAG));
            
            if (erase_type == 0x55) //���ڲ����������������
            {
                nor_flash_read_data(PAGE_ID_DATA_BUFFER,idx_data*tmp_len,FLASH_BUFFER,tmp_len);
            }
        }
        
        if (erase_type == 0x55)
        {
            mem_set(FLASH_BUFFER+DATA_HEADER_SIZE+data_offset,FLASH_BUFFER_SIZE-(DATA_HEADER_SIZE+data_offset),0xFF);
        }
        else
        {
            mem_set(FLASH_BUFFER,FLASH_BUFFER_SIZE,0xFF);
        }
    }
    #ifdef __DEBUG_
	system_debug_data((void *)&sector,2);
	system_debug_data((void *)&addr,2);
	#endif
    if (compare_string(FLASH_BUFFER+DATA_HEADER_SIZE+data_offset,data,data_len) != 0)
    {
        mem_cpy(FLASH_BUFFER,(INT8U*)&save_idx,4);
        /*  */
        mem_cpy(FLASH_BUFFER+4,hdr_td.tsk_start_time,24);
        
        mem_cpy(FLASH_BUFFER+DATA_HEADER_SIZE+data_offset,data,data_len);
		#ifdef __DEBUG_
		system_debug_data((void *)&sector,2);
		system_debug_data((void *)&addr,2);
		#endif

        nor_flash_write_data(sector,addr,FLASH_BUFFER,save_size+DATA_HEADER_SIZE);
	    }

    tpos_mutexFree(&SIGNAL_FLASH);
}
void save_oop_data(INT8U *buf,INT16U buf_len)
{
	
}



void save_normal_list_data(READ_PARAMS *read_param,INT8U *frame,INT16U frame_len)
{
	/*  */
	GROUP_OAD_PARAMS *params;
	INT8U *buf;
	INT32U save_idx;
	INT16U pos = 0;
	INT16U expect_len = 0;
	INT16U data_len = 0;
	INT16U data_offset = 0;
	INT16U offset = 0;
	INT16U save_len = 0;
	INT16U cp_len;
	INT8U  col_cnt = 0;
	INT8U  idx_col;
	INT8U  hold_time[7] = {0};
	HEADER_TD hdr_td;
	DateTime cur_dt;
	METER_DOCUMENT meter_doc;
	
    mem_cpy((void *)&meter_doc,(void *)&gmeter_doc,sizeof(METER_DOCUMENT));
	
	tpos_datetime(&cur_dt);
	

	
	params = &(read_param->norml_task_save_params.group_oad_params);
	buf = read_param->send_frame;/* ���� �����ٸĵ� */

	
	pos = get_frame_header_length_gb_oop(frame);
	pos += 1; /* get response */
	pos += 2;  //apdu��piid
	col_cnt = frame[pos++];
	#ifdef __DEBUG_
	system_debug_data(frame+pos,frame_len-pos);
	#endif
	mem_set(buf,300,0xFF);
	/* �ȶ�oad��������Ӧ���� */
    for(idx_col=0;idx_col<col_cnt;idx_col++)
    {
		if(compare_string(frame+pos,params->oad[idx_col],4)!=0)
	    {
	        return;
	    }
	    pos += 4;
		if(frame[pos++] != 1)   //20 05 02 00 " 00 03 "   20 06 02 00 " 00 03 "   �������ֵ�����ڣ���ô��ʵ����һ��������Ϣ�����������ֽڡ� 
		{
			pos+=1; 
			continue;
		}
		/* ��ȡ������Ϣ */
		data_len = get_object_attribute_item_len(frame+pos,frame[pos]);
		expect_len = params->resp_byte_num[idx_col];
		offset     = bin2_int16u(params->save_off[idx_col]);
        if(data_len == 1)
        {
        	/* û������ */
        	buf[offset] = 0xFF;
        }
        else// ��������
        {
        	//
        	if(data_len >(expect_len-1))
        	{
        		cp_len = expect_len - 1 ;
        	}
        	else
        	{
        		cp_len = data_len;
        	}
        	buf[offset] = 1;//������
        	mem_cpy(buf+offset+1,frame+pos,cp_len);
        }
        if( save_len <= (offset+expect_len) )
        {
        	save_len = offset + expect_len;
        }
		pos += data_len;
//	    offset+=data_len;
    }
    //
    #ifdef __DEBUG_
	system_debug_data(buf, save_len);
	#endif
	get_datetime_from_cosem(read_param->hold_td, hold_time);
	save_idx = getPassedDays(2000+hold_time[YEAR],hold_time[MONTH],hold_time[DAY]);

	hdr_td.tsk_start_time[0] = DT_DATETIME_S;
	hdr_td.save_time[0]      = DT_DATETIME_S;
	hdr_td.success_time[0]   = DT_DATETIME_S;
	mem_cpy(hdr_td.tsk_start_time+1,read_param->hold_td,7);
	set_cosem_datetime_s(hdr_td.success_time+1, cur_dt.value);
	mem_cpy(hdr_td.save_time+1,read_param->hold_td,7);


	write_flash_data(hdr_td,meter_doc.meter_no, data_offset, buf, save_len, 
	                 params->save_size, read_param->start_page+params->page_id, params->page_cnt);
}
/* 
 * ����ʱ��  hold_flag
 */
void parse_save_oop_data(READ_PARAMS *read_param,BOOLEAN hold_flag, INT8U *frame,INT16U frame_len)
{
	GROUP_OAD_PARAMS *params;
	INT8U *master_oad;
	INT8U *buf;
	INT32U save_idx;
	INT16U pos = 0;
	INT16U obis;
	INT16U data_len = 0;
	INT16U idx_col;
	INT16U offset = 0;
	INT16U data_offset;
	INT16U expect_len = 0;
	INT8U  type;
	INT8U  col_cnt;
	//INT8U  hold_flag; //�����־???
	INT8U  find_idx = 0;
	INT8U  cp_len = 0;
	INT8U  save_len = 0;
	INT8U  minus_val = 0;
	INT8U  hold_time[7] = {0};
	INT8U  hold_oad[4]  = {0x20,0x21,0x02,0x00}; // ����ʱ��OAD
	HEADER_TD hdr_td;
	DateTime cur_dt;

 	tpos_datetime(&cur_dt);
	
	///* ��������仯���ȴ�����ĳ��ʱ��30S�� */
	//if( (gSystemInfo.plan_task_chg) && (diff_sec_between_dt(&cur_dt, &gSystemInfo.dt) > 30) )
	//{
		//gSystemInfo.plan_task_chg = 0;
		//rebuilt_task_plan_index();
		//check_read_cycle_one_task();
	//}
	
	params = &(read_param->norml_task_save_params.group_oad_params);
	buf = read_param->send_frame;/* ���� �����ٸĵ� */
	pos = get_frame_header_length_gb_oop(frame);
	
	if(frame[pos++] == SERVER_APDU_GET)
	{
		type = frame[pos];// APDU
		switch(type)
		{
			case GET_RESPONSE_NORMAL:
				break;
		    case GET_RESPONSE_NORMAL_LIST:
		    	save_normal_list_data(read_param,frame,frame_len);
				break;
			case GET_RESPONSE_RECORD:
				pos += 2;  //apdu��piid
				master_oad = frame+pos;
				pos += 4;
				col_cnt = frame[pos++];// �� ���� 

				/* �ȽϷ��͵�OAD�ͽ���OAD�Ƿ�һ�� 
				 * ����OAD 00+4�ֽڵ�OAD=5B
				 */
				find_idx=0xFF; 
				for(idx_col=0;idx_col<col_cnt;idx_col++)
	            {
	            	if(hold_flag)
	            	{
		            	if(idx_col == 0)  //
		            	{
		            		/* �ҵ�����ʱ��  ����ʹ�� */
		                    if(compare_string(frame+pos+idx_col*5+1,hold_oad,4)==0)
		                    {
		                        find_idx = idx_col;
		                    }
		            	}
		            	else
		            	{
		                    if(compare_string(frame+pos+(idx_col)*5+1,params->oad[idx_col-1],4)!=0)
		                    {
		                        goto err;
		                    }
	                    }
                    }
                    else
                    {
                    	if(compare_string(frame+pos+(idx_col)*5+1,params->oad[idx_col],4)!=0)
	                    {
	                        goto err;
	                    }
                    }
                    
	            }
				pos+=5*col_cnt;//��������
				if (frame[pos++] != 1)//��¼����
				{
	                goto err;
	            }
	            if (frame[pos++] == 0)/* ��ʱ����һ���� */
	            {
	                return 0;
	            }

	            if(hold_flag)
	            {
	            	minus_val = 1;
	            }
				// �ȴ�hold_td(��������ʱ��) ��ȡ��ת��hold_time
				// ����û�ж���ʱ������ TODO:::::
	            get_datetime_from_cosem(read_param->hold_td, hold_time);
	            // ���������¼  ����??	            
	            //for()
	            {
		            mem_set(buf,300,0xFF);
		            save_len = 0;
		            for(idx_col=0;idx_col<col_cnt;idx_col++)
		            {
			            /* ��ȡ������Ϣ */
			            data_len = get_object_attribute_item_len(frame+pos,frame[pos]);
						if(find_idx == idx_col)
						{
							/* TODO: */
							if(frame[pos]==DT_DATETIME_S)
							{
								/* ��ȡʱ�� */
								//mem_cpy(hold_time,frame+pos+1,7);
								get_datetime_from_cosem(frame+pos+1, hold_time);
							}
							pos += data_len;
							continue;
						}
						// -1 mark
						expect_len = params->resp_byte_num[idx_col-minus_val];
						offset     = bin2_int16u(params->save_off[idx_col-minus_val]);
			            if(data_len == 1)
			            {
			            	/* û������ */
			            	buf[offset] = 0xFF;
			            }
			            else// ��������
			            {
			            	//
			            	if(data_len >(expect_len-1))
			            	{
			            		cp_len = expect_len - 1 ;
			            	}
			            	else
			            	{
			            		cp_len = data_len;
			            	}
			            	buf[offset] = 1;//������
			            	mem_cpy(buf+offset+1,frame+pos,cp_len);
			            }
			            if( save_len <= (offset+expect_len) )
			            {
			            	save_len = offset + expect_len;
			            }
			            pos += data_len;
		            }
					#ifdef __DEBUG_
					system_debug_data(buf, save_len);
					#endif
					//save_oop_data();
					save_idx = getPassedDays(2000+hold_time[YEAR],hold_time[MONTH],hold_time[DAY]);
					obis = cosem_bin2_int16u(read_param->oad.object_id);
					switch(obis)
					{
					case 0x5002: /* ���� */
						save_idx *= 96;
						save_idx += hold_time[HOUR] *4;
						save_idx += hold_time[MINUTE] /15;
						break;
					case 0x5003: /* HOUR */
						save_idx *= 24;
						save_idx += hold_time[HOUR];
						break;
					case 0x5005: /* �� */
						save_idx += hold_time[MONTH];
						break;
					default:
						break;
					}
					//�ݶ�Ϊ0 �� �������һ֡���ݰ����滮page����������,����û��ƫ��
					// TODO:
					data_offset = 0;

					hdr_td.tsk_start_time[0] = DT_DATETIME_S;
					hdr_td.save_time[0]      = DT_DATETIME_S;
					hdr_td.success_time[0]   = DT_DATETIME_S;
					mem_cpy(hdr_td.tsk_start_time+1,read_param->hold_td,7);
					set_cosem_datetime_s(hdr_td.success_time+1, cur_dt.value);
					switch(gTaskPlanAttribute[read_param->idx_tsk_attr].store_time_flag)
					{
					case 1: // ����ʼʱ��
						mem_cpy(hdr_td.save_time+1,read_param->hold_td,7);
						break;
					case 2: // ����ڵ��� 0 ��
						mem_set(cur_dt.value,DAY,0x00);
						set_cosem_datetime_s(hdr_td.save_time+1, cur_dt.value);
						break;
					case 3: // ��������� 23:59:00
						mem_set(cur_dt.value,DAY,0x00);
						date_minus_days(cur_dt.value+YEAR, cur_dt.value+MONTH, cur_dt.value+DAY, 1);
						cur_dt.value[HOUR]   = 23;
						cur_dt.value[MINUTE] = 59;
						set_cosem_datetime_s(hdr_td.save_time+1, cur_dt.value);
						break;
					case 4: // ��������� 00:00:00
						mem_set(cur_dt.value,DAY,0x00);
						date_minus_days(cur_dt.value+YEAR, cur_dt.value+MONTH, cur_dt.value+DAY, 1);
						set_cosem_datetime_s(hdr_td.save_time+1, cur_dt.value);
						break;	
					case 5: // ����ڵ���1�� 00:00:00
						mem_set(cur_dt.value,MONTH,0x00);
						cur_dt.value[DAY]  = 1;
						set_cosem_datetime_s(hdr_td.save_time+1, cur_dt.value);
						break;
					case 6: // ���ݶ���ʱ��
						mem_cpy(hdr_td.save_time+1,read_param->hold_td,7);
						break;
					default:
						mem_cpy(hdr_td.save_time+1,read_param->hold_td,7);
						break;
						
					}
					write_flash_data_from_save_idx(hdr_td, save_idx, data_offset, buf, save_len, 
					                               params->save_size, read_param->start_page+params->page_id, params->page_cnt);
					#ifdef __DEBUG_
					read_flash_data_from_save_idx(save_idx, data_offset, read_param->recv_frame, save_len, 
					                               params->save_size, read_param->start_page+params->page_id, params->page_cnt);
					system_debug_data(read_param->recv_frame, save_len);
					#endif
	            }
				break;
		}
		return;
	}
	//else /* err ?? */
	{
		//clear or todo:
	}
err:
	return;
}
