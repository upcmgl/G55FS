#include "../main_include.h"
#include<stdarg.h>

char HEX[]="0123456789ABCDEF";
/*+++
   ���ܣ��Ƚ������ֽڴ��Ĵ�С
   ������INT8U *str1  ��1
         INT8U *str2  ��2
         int  len      �Ƚϳ���
   ���أ� -1: str1 < str2    
           0: str1 = str2
           1: str1 > str2
---*/
INT16S  compare_string(INT8U *str1,INT8U *str2,INT16U len)
{
    if((str1==NULL) || (str2==NULL))//������ݵĲ���ΪNULL������Ϊ�ȽϽ����ͬ
    {
        return -2;
    }
    while(len > 0)
    {
        if(*str1 < *str2) return -1;
        if(*str1 > *str2) return 1;
        str1++;
        str2++;
        len--;
    }
    return 0;
}
/*+++
   ���ܣ��Ƚ������ֽڴ��Ĵ�С,����Ƚ�
   ������INT8U *str1  ��1
         INT8U *str2  ��2
         int  len      �Ƚϳ���
   ���أ� -1: str1 < str2    
           0: str1 = str2
           1: str1 > str2
---*/
INT16S  compare_string_reverse(INT8U *str1,INT8U *str2,INT16U len)
{
    while(len > 0)
    {
        len--;
        if(str1[len] < str2[len]) return -1;
        if(str1[len] > str2[len]) return 1;
    }
    return 0;
}
//���򿽱��ַ���
void buffer_reverse(INT8U *dst,INT16U len)
{
    INT16U idx,time;
	INT8U tmp;
	time = (INT16U)(len/2);
    for(idx=0;idx<time;idx++)
    {
       	tmp = dst[idx];
		dst[idx] = dst[len-idx-1];
		dst[len-idx-1] = tmp;
    }
}
//ȥ�ո���
void trimspace(INT8U *str,INT16U size)
{
    INT16U idx,pos;
    //ȥǰ�ո��ҵ���һ���ǿո�λ�ã��Ӹ�λ�ý�������
    idx=0;
    pos=size;
    while(idx<size)
    {
        if(str[idx]!=' ')
    {
            pos=idx;
            break;
        }
        idx++;
    }
    if(pos==size)//ȫ��Ϊ�ո�
        {
        str[0]='\0';
        }
    else if(pos>0)
    {
        mem_cpy(str,str+pos,size-pos);
        mem_set(str+size-pos,pos,0x00);
    }
   //ȥ��ո�˼·Ϊ�Ӻ���ң����ֿո�������Ϊ'\0',ֱ�������ǿո�Ϊֹ
   idx=size;
   while(--idx)
   {
   	   if((str[idx]!='\0') && (str[idx]!=' ')) break;
       str[idx]='\0';
   }
}
/*+++
  ���ܣ� ����ָ���ַ���,֧�ֵڼ��γ���
----*/
INT16S  str_find_again(INT8U *buffer,INT16U buflen,INT8U *target,INT16U targetlen,INT8U againcount)
{
    INT16U pos;
    INT16U idx;

    for(pos=0;pos<buflen;pos++)
    {
        for(idx=0;idx<targetlen;idx++)
        {
           if(target[idx]!=buffer[idx]) break;
        }
        if(idx>=targetlen)
        {
         	againcount--;
            if(againcount==0) return pos;
        }
        buffer++;
    }
    return -1;
}
INT16S  str_find_reverse_non(INT8U *buffer,INT16U buflen,INT8S c)
{
    INT16S idx;

    for(idx=buflen-1;idx>=0;idx--)
    {
        if(buffer[idx]!=c)
        {
            return buflen-idx-1;
        }
    }
    return -1;
}
INT16U str_fill(INT8U *buffer,INT16U buflen,INT8S fill_c,INT16U width_len)
{
    INT16U str_len;
    INT16S idx;

    str_len=str_cmdlen(buffer);
    if(width_len>=buflen)
    {
        width_len=buflen-1;
    }
    if(str_len>=width_len)
    {
        return str_len;
    }
    buffer[width_len]='\0';
    for(idx=width_len-1;idx>=0;idx--)
    {
        if(width_len-idx<=str_len)
        {
            buffer[idx]=buffer[str_len+idx-width_len];
        }
        else
        {
            buffer[idx]=fill_c;
        }

    }
    return width_len;

}
/*+++
  ���ܣ� ת�����ε��ַ���
  ������
          INT32S value           ��ת��������ֵ
          char *str             �������������
          INT8U radix           ����,��Χ��2~16
  ���أ�
          �������������
---*/
char* int2str(INT32S value,char *str,INT8U radix)
{
    INT32U val;
    INT8U idx=0;
    INT8U offset=0;

    str[idx]='\0';
    if(radix>16)
    {
        return str;
    }
    if(value<0)
    {
        val=-value;
        str[idx++]='-';
        offset=1;
    }
    else
    {
        val=value;
    }
    
    do
    {
        str[idx++]=HEX[val % radix];
        val/=radix;
    }while(val);
    str[idx]='\0';
    
    buffer_reverse((INT8U*)(str+offset),idx-offset);

    return str;
}
/*+++
  ���ܣ� �ַ�����ʽ����������֧��%c%d%x%s,��֧��%f
  ������
          char *buffer          ���������
          INT16U size           �����������С,��СΪ2
          const char * format   �����ʽ���ַ���
          ...                   �䳤�������
  ���أ�
          ��
---*/
//void str_nprintf(char *buffer,INT16U size,const char * format,...)
//{   
//    va_list ap;
//    INT16U leftlen;
//    INT8U tmp_size,idx;
//    char ch,ch1;
//    char *p;
//    INT32S num;
//    INT8U len_flag;
//    INT8U len_size;
//    INT8U format_flag;
//    INT8U tmp[32]={0};
//    BOOLEAN is_valid;
//
//
//    if(size<2)return ;
//
//    va_start(ap,format);
//
//    format_flag =0;
//    len_flag    =0;
//    len_size    =0;
//    leftlen     =size-1;
//    is_valid=TRUE;
//    do
//    {
//        ch=*format;
//        if(ch==0)break;
//        format++;
//        if(leftlen==0) break; //Խ��
//        if(ch=='%')
//        {
//            format_flag=1;
//            len_size=0;
//            len_flag=0;
//            continue;
//        }
//        if(format_flag)
//        {
//            if((ch=='0') && (len_flag==0))
//            {
//                len_flag=1;
//                continue;
//            }
//            if((ch>='0') &&  (ch<='9'))
//            {
//                len_size*=10;
//                len_size+=ch-'0';
//                continue;
//            }
//            switch(ch)
//            {
//                case 'c':
//                case 'C':
//                {
//                    ch1= va_arg(ap,int);
//                    if(len_size>1)
//                    {
//                        tmp[0]=ch1;
//                        tmp[1]='\0';
//                        str_fill(tmp,sizeof(tmp),' ',len_size);
//                        if(len_size>leftlen)
//                        {
//                            is_valid=FALSE;
//                            break; //Խ��
//                        }
//                        mem_cpy(buffer,tmp,len_size);
//                        buffer[len_size]='\0';
//                        buffer+=len_size;
//                        leftlen-=len_size;
//                    }
//                    else
//                    {
//                        *buffer=ch1;
//                        buffer++;
//                        leftlen--;
//                        *buffer='\0';
//                    }
//                    break;
//                }
//                case 'x':
//                case 'X':
//                {
//                    num= va_arg(ap,int);
//                    int2str(num,(char*)tmp,16);
//                    tmp_size=str_cmdlen(tmp);
//                    len_size=(tmp_size>len_size)?tmp_size:len_size;
//                    if(len_flag==1)//��Ҫ����
//                    {
//                        str_fill(tmp,sizeof(tmp),'0',len_size);
//                    }
//                    for(idx=0;idx<len_size;idx++)
//                    {
//                        if((tmp[idx]>='a') && (tmp[idx]<='f') && (ch=='X'))
//                        {
//                            tmp[idx]-=32;
//                        }
//                        else if((tmp[idx]>='A') && (tmp[idx]<='F') && (ch=='x'))
//                        {
//                            tmp[idx]+=32;
//                        }
//                    }
//                    if(len_size>leftlen)
//                    {
//                        is_valid=FALSE;
//                        break; //Խ��
//                    }
//
//                    mem_cpy(buffer,tmp,len_size);
//                    buffer[len_size]='\0';
//                    buffer+=len_size;
//                    leftlen-=len_size;
//                    break;
//                }
//                case 'd':
//                case 'D':
//                {
//                    num= va_arg(ap,int);
//                    int2str(num,(char*)tmp,10);
//                    tmp_size=str_cmdlen(tmp);
//                    len_size=(tmp_size>len_size)?tmp_size:len_size;
//                    if(len_flag==1)//��Ҫ����
//                    {
//                        str_fill(tmp,sizeof(tmp),'0',len_size);
//                    }
//
//                    if(len_size>leftlen)
//                    {
//                        is_valid=FALSE;
//                        break; //Խ��
//                    }
//
//                    mem_cpy(buffer,tmp,len_size);
//                    buffer[len_size]='\0';
//                    buffer+=len_size;
//                    leftlen-=len_size;
//                    break;
//                }
//                case 's':
//                case 'S':
//                {
//                    p =va_arg(ap,char *);
//                    tmp_size=str_cmdlen((INT8U*)p);
//                    if(len_size>0)
//                    {
//                        if(len_size>leftlen)
//                        {
//                            is_valid=FALSE;
//                            break; //Խ��
//                        }
//                        
//                        if(len_size>tmp_size)
//                        {
//                            mem_set(buffer,len_size-tmp_size,' ');
//                            buffer+=len_size-tmp_size;
//                            leftlen-=len_size-tmp_size;
//                        }
//                        //tmp_size=(len_size<tmp_size)?len_size:tmp_size;
//                        mem_cpy(buffer,p,tmp_size);
//                        buffer[tmp_size]='\0';
//                        buffer+=tmp_size;
//                        leftlen-=tmp_size;
//                    }
//                    else
//                    {
//                        if(tmp_size>leftlen)
//                        {
//                            is_valid=FALSE;
//                            break; //Խ��
//                        }
//                        
//                        mem_cpy(buffer,p,tmp_size+1);
//                        buffer+=tmp_size;
//                        leftlen-=tmp_size;
//                    }
//                    break;
//                }
//                default:
//                   break;
//            }
//            if(!is_valid)
//            {
//                break;
//            }
//            format_flag=0;
//            continue;
//        }
//        *buffer=ch;
//        buffer++;
//        *buffer='\0';
//        leftlen--;
//    }while(ch!=0);
//    va_end(ap);
//}   
/*+++
  ���ܣ� �ַ�����ʽ����������֧��%c%d%x%s,��֧��%f
  ������
          char *buffer          ���������
          const char * format   �����ʽ���ַ���
          ...                   �䳤�������
  ���أ�
          ��
---*/
//void str_printf(char *buffer,const char * format,...)
//{   
//    va_list ap;
//    INT8U tmp_size,idx;
//    char ch,ch1;
//    char *p;
//    INT32S num;
//    INT8U len_flag;
//    INT8U len_size;
//    INT8U format_flag;
//    INT8U tmp[32]={0};
//
//
//    va_start(ap,format);
//
//    format_flag =0;
//    len_flag    =0;
//    len_size    =0;
//    do
//    {
//        ch=*format;
//        if(ch==0)break;
//        format++;
//        if(ch=='%')
//        {
//            format_flag=1;
//            len_size=0;
//            len_flag=0;
//            continue;
//        }
//        if(format_flag)
//        {
//            if((ch=='0') && (len_flag==0))
//            {
//                len_flag=1;
//                continue;
//            }
//            if((ch>='0') &&  (ch<='9'))
//            {
//                len_size*=10;
//                len_size+=ch-'0';
//                continue;
//            }
//            switch(ch)
//            {
//                case 'c':
//                case 'C':
//                {
//                    ch1= va_arg(ap,int);
//                    if(len_size>1)
//                    {
//                        tmp[0]=ch1;
//                        tmp[1]='\0';
//                        str_fill(tmp,sizeof(tmp),' ',len_size);
//                        mem_cpy(buffer,tmp,len_size);
//                        buffer[len_size]='\0';
//                        buffer+=len_size;
//                    }
//                    else
//                    {
//                        *buffer=ch1;
//                        buffer++;
//                        *buffer='\0';
//                    }
//                    break;
//                }
//                case 'x':
//                case 'X':
//                {
//                    num= va_arg(ap,int);
//                    int2str(num,(char *)tmp,16);
//                    tmp_size=str_cmdlen(tmp);
//                    len_size=(tmp_size>len_size)?tmp_size:len_size;
//                    if(len_flag==1)//��Ҫ����
//                    {
//                        str_fill(tmp,sizeof(tmp),'0',len_size);
//                    }
//
//                    for(idx=0;idx<len_size;idx++)
//                    {
//                        if((tmp[idx]>='a') && (tmp[idx]<='f') && (ch=='X'))
//                        {
//                            tmp[idx]-=32;
//                        }
//                        else if((tmp[idx]>='A') && (tmp[idx]<='F') && (ch=='x'))
//                        {
//                            tmp[idx]+=32;
//                        }
//                    }
//                    mem_cpy(buffer,tmp,len_size);
//                    buffer[len_size]='\0';
//                    buffer+=len_size;
//                    break;
//                }
//                case 'd':
//                case 'D':
//                {
//                    num= va_arg(ap,int);
//                    int2str(num,(char*)tmp,10);
//                    tmp_size=str_cmdlen(tmp);
//                    len_size=(tmp_size>len_size)?tmp_size:len_size;
//
//                    if(len_flag==1)//��Ҫ����
//                    {
//                        str_fill(tmp,sizeof(tmp),'0',len_size);
//                    }
//                    mem_cpy(buffer,tmp,len_size);
//                    buffer[len_size]='\0';
//                    buffer+=len_size;
//                    break;
//                }
//                case 's':
//                case 'S':
//                {
//                    p =va_arg(ap,char *);
//                    tmp_size=str_cmdlen((INT8U*)p);
//                    if(len_size>0)
//                    {
//                        if(len_size>tmp_size)
//                        {
//                            mem_set(buffer,len_size-tmp_size,' ');
//                            buffer+=len_size-tmp_size;
//                        }
//                        //tmp_size=(len_size<tmp_size)?len_size:tmp_size;
//                        mem_cpy(buffer,p,tmp_size);
//                        buffer[tmp_size]='\0';
//                        buffer+=tmp_size;
//                    }
//                    else
//                    {
//                        mem_cpy(buffer,p,tmp_size+1);
//                        buffer+=tmp_size;
//                    }
//                    break;
//                }
//                default:
//                   break;
//            }
//            format_flag=0;
//            continue;
//        }
//        *buffer=ch;
//        buffer++;
//        *buffer='\0';
//    }while(ch!=0);
//    va_end(ap);    
//}

////������ԭ��char *strcpy(char *s1, const char *s2);
////������ char �͵����ֵ��Ĭ�������Ϊ 255�����ָ�� -fsigned-char ѡ����Ϊ 127��
////������ char �͵���Сֵ��Ĭ�������Ϊ 0�����ָ�� -fsigned-char ѡ����Ϊ -128
//INT8S *str_cpy(INT8S *s1, INT8S *s2)
//{
//    return (INT8S *)strcpy((char *)s1, (const char *)s2);
//}




 