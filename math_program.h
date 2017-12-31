  /******************************************************************************************
                                  数据数学关系处理函数
 /*****************************************************************************************/
 
#include "Arduino.h"

#define erro 0
#define end 1



/******************************************************************************************
函数名称：array_memset(int *array_name,int array_set,int array_length)
函数作用：将int数形数组全部清零
输入参数：*array_name ——————————指针（指向数组的第一个元素地址，每次使用需要将
                                               指针地址更新）
          array_set   ——————————设置值（清零时为NULL或者0）
          array_length——————————数组长度
输出参数：无
测试例程：int a[]={2,3,4,5,6};
          for(int i=0;i<5;i++) {Serial.print(a[i]);}
          Serial.println();
          array_memset(a,NULL,sizeof(a)/sizeof(int));
          for(int i=0;i<5;i++) {Serial.print(a[i]);}
          while(1);
返回参数：23456\n00000
 /*****************************************************************************************/
 int array_memset(int *array_name,int array_set,int array_length)
{
  for(int i=0;i<array_length;i++)
  {
    *array_name=array_set;
    *array_name++; 
  }
} 

/******************************************************************************************
函数名称：ascall_change(int  data)
函数作用：按ASCALL码表将对应char数型转换成int数型
输入参数： data       ——————————输入数据
输出参数：对应的int数型格式数据
测试例程：while(Serial.available() > 0)
         {   
            c=char(Serial.read());
            Serial.print("  HEX=");Serial.print(c); 
            int b;
            b=ascall_change(c);
            Serial.print("    DEC=");Serial.println(b);     
          }
返回参数：输入1返回(  HEX=1    DEC=1)
          输入a返回(  HEX=a    DEC=10)
/*****************************************************************************************/
int ascall_change(int  data)
{
  int data_change;
  if(data>47&&data<58)data_change=(data-48);
  else if(data>64&&data<71)data_change=(data-55);
  else if(data>96&&data<103)data_change=(data-87);
  else data_change=15;
  return data_change;
}

/******************************************************************************************
函数名称：data_rate(char data_char01,char data_char02)
函数作用：将两个16进制数组处理成10进制内容(例如：16进制为FE，处理成二进制为254)
输入参数： data_char02——————————输入16进制第一个内容
           data_char01——————————输入16进制第二个内容
输出参数：对应的int数型格式数据
测试例程：//全局变量
          int data_cout=0;char data_char01,data_char02;

          //**********主循环内容**********
          //串口输入触发函数
           while(Serial.available()>0)
            {
              //奇数处理方式
              if(data_cout%2==0)
                {data[data_char01]=Serial.read();}
              //偶数处理方式
              else 
                {data[data_char02]=Serial.read();data_rate(data_char01,data_char02);}
              data_cout++;
            }                          
返回参数：输入10返回(  HEX=1 0    DEC=17)
          输入fe返回(  HEX=f e    DEC=254)
          输入FF返回(  HEX=F F    DEC=255)
/*****************************************************************************************/
int data_rate(char data_char01,char data_char02)
{
  int data=ascall_change(data_char01)*16+ascall_change(data_char02);
  //Serial.print("   HEX=");Serial.print(data_char01);Serial.print(" ");Serial.print(data_char02);
  //Serial.print("   DEC=");Serial.println(data);
  return data;  
}

/******************************************************************************************
函数名称：int translation(int *data, int data_length)
函数作用：返转义，同时返回有效数据数组长度(包括数据头与数据尾)
输入参数： 
输出参数：有效数据长度(int数型)
测试例程：int a[14]={253,1,254,253,4,254,252,254,253,9,10,11,12,255};
          Serial.println(translation(a, sizeof(a)/sizeof(int)));
          while(1);
返回参数：11(原转义数据：253, 1,254,253, 4,254,252,254,253,9 ,10 ,11,12,255)
            (反转义数据：253, 1,255，  , 4,254,   ,255,   ,9 ,10 ,11,12,255)
            (HEX内容   ：FD ,01,FF ,   ,04,FE ,   ,FF ,   , 09,0A,0B,0C,FF)
            FD01FF04FEFF090A0B0CFF
/*****************************************************************************************/
int translation(int *data, int data_length)
{
  //声明局部变量flag(记录转义次数)
  int flag=0;
  //声明局部变量data_stored(数据暂存)
  int data_stored;  
  //将转义数据反转义，并将多余数据标记为0
  for(int i=0;i<data_length;i++,*data++)
  {
    //检测数据头标记符是否存在，判断接收数据是否正确
    if(i==0&&*data!=253)
      return erro;
    //检测数据尾标记符是否存在，判断接收数据是否正确
    else if(i==(data_length-1)&&*data!=255) 
      return erro;
    //将转义内容反转义(FEFD>>FF ; FEFC>>FE)
    else if(*data==254)
      {
        //检测下一个数据内容，判断是否为转义内容
        *data++;
        if(*data==252||*data==253)
          //反转义(FEFD>>FF00; FEFC>>FE00)
          {data_stored=*data;*data=0;*data--;*data=(data_stored+2);}
        else *data--;
      }
  }
  //将指针地址返回数组的首元素地址
  for(int i=0;i<data_length;i++)*data--;
  //Serial.println("---------------------------------------------");
  //删除转义多余数据(即0)
  for(int i=0;i<data_length;i++,*data++)
  {
    //检测是否为需删除数据
    if(*data==0)
      {*data--;if(*data==255||*data==254){flag++;}*data++;}
    //若数据为删除的数据，进行标记(flag),嗅探位移数据操作次数
    line_a:for(int j=0;j<flag;j++)*data++;
    //判断位移数据是否也为删除数据,并进行处理
    if(*data==0)
      {
        *data--;
        if(*data==255||*data==254)
          {flag++;goto line_a;}
      }
    //将位移数据储存在局部变量(data_stored)中
    data_stored=*data;
    //位移至需要位移的数据位置
    for(int j=0;j<flag;j++)*data--;
    //替换数据，实现数组位移
    *data=data_stored;
  }
  //指针位置复位至数组首数据位置
  for(int i=0;i<data_length;i++)*data--;  
  //返回真实数组数据长度
  return (data_length-flag);
}






