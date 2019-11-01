#include "../main_include.h"

/*+++
  ���ܣ����û���������
---*/

void   mem_set(void *buffer,INT16U len,INT8U value)
{
    INT8U *ch_buffer;

    ch_buffer = (INT8U *)buffer;
    while(len!=0)
    {
        *ch_buffer++ = value;
        len--;
    }
}

//�ڴ渴��
//ע��һ��Ҫ�������ҽ��и��ƣ���Ϊ�п�����ͬһ�����ڵĸ��ơ�
void    mem_cpy(void *dst,void *src,INT16U len)
{
INT16U i;

   for(i=0;i<len;i++)
   {
       ((INT8U *)dst)[i] = ((INT8U *)src)[i];
   }

}
void mem_cpy_reverse(INT8U *dst,INT8U *src,INT16U len)
{
    INT16U idx;
    for(idx=0;idx<len;idx++)
    {
        dst[idx] = src[len-idx-1];
    }
}

//�ڴ渴��
//ע��һ��Ҫ����������и��ƣ���Ϊ�п�����ͬһ�����ڵĸ��ơ�
void mem_cpy_right(void *dst,void *src,INT16U len)
{
   INT16U i;

   for(i=0;i<len;i++)
   {
       ((INT8U *)dst)[len-1-i] = ((INT8U *)src)[len-1-i];
   }

}

//�ڴ涼��ͬһ���ַ�
INT8U  mem_all_is(void *dst,INT16U len,  INT8U c)
{
   INT8U  *p;

   p = (INT8U *)dst;
   while(len--)
   {
        if((*p++) != c)    return FALSE;
   }
   return TRUE;
}

//�ڴ涼��ͬһ���ַ�
INT8U  mem_is_same(void *dst,void * src,INT16U len)
{
   INT8U  *p;
   INT8U  *q;
   p = (INT8U *)dst;
   q = (INT8U *)src;
   while(len--)
   {
        if((*p++) != (*q++))    return FALSE;
   }
   return TRUE;
}
//�ֽ���λ˳���D0��D7, DL698

#define BITSTRING_MASK   0x01


//�ֽ���λ˳���D7~D0,�Ϸ�������Լ,62056λ��
//#define BITSTRING_MASK   0x80


/*++++
  ���ܣ����λ����ĳ��λ����1��0
  ������INT8U *BS           λ��
       INT8U bitStringLen  λ���ֽڳ���
       int  bitpos                �ڣ�λ     0:����bit0 
  ������bit0-bit7  ˳��0-7
 
----*/
INT8U get_bit_value(INT8U *BS,INT16U bitStringlen,INT16U bitpos)
{
    INT8U  byte_no;
    if(bitpos > 7)
    {
        byte_no = bitpos >> 3;
        BS += byte_no;
        if(bitStringlen<=byte_no) return 0;
        //bitStringlen-=byte_no;//δʹ�õ�ֵ
        bitpos &= 0x07;
    }
    return (*BS) & (BITSTRING_MASK<<bitpos);

}

//  ��ȡλ����1�ĸ���
INT32U get_bit_count(INT8U *BS,INT16U bitStringlen)
{
	INT32U idx_byte,count_bit;
	INT8U idx_bit;
	count_bit=0;
	for(idx_byte=0; idx_byte<bitStringlen; idx_byte++)
	{
		for(idx_bit=0; idx_bit<8; idx_bit++)
		{
			if(BS[idx_byte] & (1<<idx_bit))
				count_bit++;
		}
	}
return count_bit;

}

/*++++
  ���ܣ�����λ����ĳ��λ����1
  ������INT8U *BS           λ��
       INT8U bitStringLen  λ������
       int  bitpos                �ڣ�λ     0:����bit0  
----*/

void set_bit_value(INT8U *BS,INT16U bitStringlen,INT16U bitpos)
{
   INT8U  byte_no;
   if(bitpos > 7)
   {
         byte_no = bitpos >> 3;
         BS += byte_no;
         if(bitStringlen<=byte_no) return;
         //bitStringlen-=byte_no;//δʹ�õ�ֵ
         bitpos &= 0x07;
   }
   (*BS) |=   (BITSTRING_MASK<<bitpos);
}

/*++++
  ���ܣ�����λ����ĳ��λ����0
  ������INT8U *BS           λ��
       INT8U bitStringLen  λ������
       int  bitpos                �ڣ�λ     0:����bit0  
----*/

void  clr_bit_value(INT8U *BS,INT16U bitStringlen,INT16U bitpos)
{
   INT8U  byte_no;
   if(bitpos > 7)
   {
         byte_no = bitpos >> 3;
         BS += byte_no;
         if(bitStringlen<=byte_no) return;
         //bitStringlen-=byte_no;//δʹ�õ�ֵ
         bitpos &= 0x07;
   }
   (*BS) &=   ~(BITSTRING_MASK<<bitpos);
}

 //����λ��ȡ��
void bit_value_opt_inversion(INT8U *BS,INT16U bitStringlen)
{
    INT16U idx;

    for(idx=0;idx<bitStringlen;idx++)
    {
        BS[idx] = ~ BS[idx];
    }
}

//����λ�����������㣬������ڵ�һ��λ����
void bit_value_opt_and(INT8U *BS1,INT8U *BS2,INT16U bitStringlen)
{
    INT16U idx;

    for(idx=0;idx<bitStringlen;idx++)
    {
        BS1[idx] &= BS2[idx];
    }
}

//����λ�����������㣬������ڵ�һ��λ����
void bit_value_opt_or(INT8U *BS1,INT8U *BS2,INT16U bitStringlen)
{
    INT16U idx;

    for(idx=0;idx<bitStringlen;idx++)
    {
        BS1[idx] |= BS2[idx];
    }
}
//��һ���ֽ���λ��˳��ߵ���max_len<=8
void bit_seq_reverse(INT8U *BS1,INT8U max_len)
{
    INT8U idx;
    INT8U BS2,mask;


    BS2=*BS1;
    *BS1=0;
    for(idx=0;idx<max_len;idx++)
    {
        mask=(1<<idx);
        if(BS2 & mask)
        {
            *BS1 |= 1<<(max_len-idx-1);
        }
    }
}
