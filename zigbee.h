/******************************************************************************************
                                  Zigbee通讯协议函数库（DL-LN3X）
 /*****************************************************************************************/

#include "Arduino.h"
#include "math_program.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long



bool rev_flag=NULL;
int data_cout=0;
int data[100];
unsigned long time_millis;

/******************************************************************************************
函数名称：zigbee_port(int port)
函数作用：按DL-LN3X格式处理端口号数据，并发送出去
输入参数：port  ——————————待处理端口号
输出参数：无
测试例程：zigbee_port(241);while(1);
返回参数：F1//端口号为241（16进制为F1）
 /*****************************************************************************************/
int zigbee_port(int port)
{
  if(port==255)
    Serial.print("FEFD");
  else if(port==254)Serial.print("FEFC");
  else if(port<=15){Serial.print("0");Serial.print(port,HEX);}
  else  Serial.print(port,HEX);  
}

/******************************************************************************************
函数名称：zigbee_ID(uint ID)
函数作用：按DL-LN3X格式处理远程地址数据，并发送出去
输入参数：ID  ——————————待远程地址号
输出参数：无
测试例程：zigbee_ID(4097);while(1);
返回参数：0101//端口号为4097（16进制为1001）
 /*****************************************************************************************/
uint zigbee_ID(uint ID)
{
  //端口号(ID)在 0000(0) 和 000F(15) 之间
  if(ID<=15)
    {Serial.print("0");Serial.print(ID,HEX);Serial.print("00");}
    
  //端口号(ID)在 0010(16) 和 00FF(255) 之间
  else if(ID>15&&ID<=255)
    {
      //输出低8位数据
      if(ID==255)Serial.print("FEFD");
      else if (ID==254)Serial.print("FEFC");
      else Serial.print(ID,HEX);
      //输出高8位数据
      Serial.print("00");
    }
  //端口号(ID)在 0100(256) 和 0FFF(4095) 之间
  else if(ID>255&&ID<=4095)
    {
      //输出低8位数据
      if(ID%256==255)Serial.print("FEFD");
      else if (ID==254)Serial.print("FEFC");
      else {Serial.print(((ID%256)/16),HEX);Serial.print(((ID%256)%16),HEX);}
      //输出高8位数据
      Serial.print("0");Serial.print((ID/256),HEX);
    }
  //端口号(ID)在 1000(4095) 和 FFFF(65536) 之间
  else if(ID>4095&&ID<=65536)
    {    
      //输出低8位数据  
      if((ID%4096)%256==255)Serial.print("FEFD");
      else if((ID%4096)%256==254)Serial.print("FEFC");
      else {Serial.print(((ID%4096)%256)/16,HEX);Serial.print(((ID%4096)%256)%16,HEX);}
      //输出高8位数据
      if(ID/256==255)Serial.print("FEFD");
      else if(ID/256==254)Serial.print("FEFC");
      else {Serial.print((ID/4096),HEX);Serial.print((ID%4096)/256,HEX);}
    }  
}

/******************************************************************************************
函数名称：zigbee_data(int data_length,int data[])
函数作用：按DL-LN3X格式处理数据，并发送出去
输入参数：data_length——————————数据长度
          data[]     ——————————数据内容存储数组
输出参数：无
测试例程：int a[]={10,31,45,1};zigbee_data(sizeof(a)/sizeof(int),a);while(1);
返回参数：0A1F2D01//以16进制输出数组内容
 /*****************************************************************************************/
int zigbee_data(int data_length,int data[])
{
  for(int i=0;i<data_length;i++)  
    {     
      //Serial.print("message[");Serial.print(i);Serial.print("]=");
      //传输数据格式处理
      if(data[i]==0)Serial.print("00");else if(data[i]<17&&data[i]>0)Serial.print("0");
      if(data[i]==255)Serial.print("FEFD");else if(data[i]==254)Serial.print("FEFC");
      else Serial.print(data[i],HEX);
      //释放data数组内存
      data[i]=NULL;   
    }
}

/******************************************************************************************
函数名称：zigbee_route(int route)
函数作用：按DL-LN3X格式处理信道，并发送出去
输入参数：route      ——————————信道
输出参数：无
测试例程：zigbee_route(19);while(1);
返回参数：13//以16进制输出19(十六进制为13)
 /*****************************************************************************************/
int zigbee_route(int route)
{
  if(route<=15)Serial.print("0");
  Serial.print(route,HEX);
}

/******************************************************************************************
函数名称：zigbee_baudrate(ulong baudrate)
函数作用：按DL-LN3X格式处理波特率，并发送出去
输入参数：baudrate  ——————————波特率
输出参数：无
测试例程：zigbee_baudrate(9600);while(1);
返回参数：13//以16进制输出19(十六进制为13)
 /*****************************************************************************************/
int zigbee_baudrate(ulong baudrate)
{
  if(baudrate==2400)Serial.print("00");
  else if(baudrate==4800)Serial.print("01");
  else if(baudrate==9600)Serial.print("02");
  else if(baudrate==14400)Serial.print("03");
  else if(baudrate==19200)Serial.print("04");
  else if(baudrate==28800)Serial.print("05");
  else if(baudrate==38400)Serial.print("06");
  else if(baudrate==57600)Serial.print("07");
  else if(baudrate==115200)Serial.print("08");
  else if(baudrate==230400)Serial.print("09");
  else if(baudrate==125000)Serial.print("0A");
  else if(baudrate==250000)Serial.print("0B");
  else if(baudrate==500000)Serial.print("0C");
  else Serial.print("02");
}

/******************************************************************************************
函数名称：zigbee_rebaudrate(ulong baudrate)
函数作用：接收到DL-LN的比特率信息，用于调试
输入参数：baudrate  ——————————波特率
输出参数：无
测试例程：zigbee_rebaudrate(2);while(1);
返回参数：9600
 /*****************************************************************************************/
int zigbee_rebaudrate(ulong baudrate)
{
  if(baudrate==0)Serial.print("2400");
  else if(baudrate==1)Serial.print("4800");
  else if(baudrate==2)Serial.print("9600");
  else if(baudrate==3)Serial.print("14400");
  else if(baudrate==4)Serial.print("19200");
  else if(baudrate==5)Serial.print("28800");
  else if(baudrate==6)Serial.print("38400");
  else if(baudrate==7)Serial.print("57600");
  else if(baudrate==8)Serial.print("115200");
  else if(baudrate==9)Serial.print("230400");
  else if(baudrate==10)Serial.print("125000");
  else if(baudrate==11)Serial.print("250000");
  else if(baudrate==12)Serial.print("500000");
  else Serial.print("9600");
}

/******************************************************************************************
函数名称：zigbee_send(int sent_port,int rev_port,uint ID,int data_length,int data[])
函数作用：按DL-LN3X格式处理数据，并发送出去
输入参数：sent_port  ——————————源端口号
          rev_port   ——————————目的端口号
          add        ——————————远程地址
          data_length——————————数据长度
          data[]     ——————————数据内容
输出参数：无
测试例程：int a[]={10,31,45,1};zigbee_send(145,144,768,sizeof(a)/sizeof(int),a);while(1);
返回参数：FE08919003000A1F2D01FF//数组内容（16进制格式）：0A1F2D01
 /*****************************************************************************************/
char zigbee_send(int sent_port,int rev_port,uint ID,int Data_length,int Data[])
{ 
  Serial.print("FE");
  //数据长度处理
  if(Data_length<11)Serial.print("0");Serial.print(Data_length+4);
  //源端口号格式处理
  zigbee_port(sent_port);
  //目的端口号格式处理
  zigbee_port(rev_port);
  //远程地址格式处理
  zigbee_ID(ID);
  //数据处理
  zigbee_data(sizeof(Data)/sizeof(int),Data);

  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_ID(int sent_port,uint ID,uint new_ID)
函数作用：为某模块设置新的远程地址(ID)
输入参数：sent_port  ——————————源端口号
          ID         ——————————远程地址(ID)
          new_ID     ——————————新远程地址(ID)
输出参数：无
测试例程：zigbee_set_ID(145,0,39065);while(1);
返回参数：FE07919003000A1F2D01FF//数组内容（16进制格式）：0A1F2D01
 /*****************************************************************************************/
char zigbee_set_ID(int sent_port,uint ID,uint new_ID)
{
  Serial.print("FE07"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号
  Serial.print("21");
  //原远程地址
  zigbee_ID(ID);
  //命令
  Serial.print("12");
  //新远程地址
  zigbee_ID(new_ID);
  
  Serial.print("FF");  
}

/******************************************************************************************
函数名称：zigbee_led(int sent_port,uint ID,float Led_time)
函数作用：点亮LED
输入参数：sent_port  ——————————源端口号
          ID         ——————————远程地址(ID)
          Led_time   ——————————LED点亮时间(单位为 S)
输出参数：无
测试例程：zigbee_led(145,0,0.5);while(1);
返回参数：FE07912100002005FF//点亮LED0.5s
 /*****************************************************************************************/
int zigbee_led(int sent_port,uint ID,float Led_time)
{
  Serial.print("FE06"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X20)
  Serial.print("20");
  //原远程地址
  zigbee_ID(ID);
  //LED点亮时间 Led_time s
  int led_time=Led_time*10;
  if(led_time<=15)Serial.print("0");
  Serial.print(led_time,HEX);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_read_ID(int sent_port)
函数作用：读取模块地址
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_ID(145);while(1);
返回参数：FE059121000001FF//数据是(0X0000)命令是(0X01)
 /*****************************************************************************************/
int zigbee_read_ID(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X01)
  Serial.print("01");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_read_netID(int sent_port)
函数作用：读取模块网络ID地址(netID)
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_netID(145);while(1);
返回参数：FE059121000002FF//数据是0X02
 /*****************************************************************************************/
int zigbee_read_netID(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X02)
  Serial.print("02");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_read_route(int sent_port)
函数作用：读取模块信道
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_route(145);while(1);
返回参数：FE059121000003FF//数据是0X03
 /*****************************************************************************************/
int zigbee_read_route(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X03)
  Serial.print("03");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_read_baudrate(int sent_port)
函数作用：读取模块波特率
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_read_baudrate(145);while(1);
返回参数：FE059121000004FF//数据是0X04
 /*****************************************************************************************/
int zigbee_read_baudrate(int sent_port)
{
    Serial.print("FE05"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("04");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_set_ID(int sent_port,uint ID)
函数作用：设置模块地址(ID)
输入参数：sent_port  ——————————源端口号
          ID      ——————————新的地址(ID)
输出参数：无
测试例程：zigbee_set_ID(145,65029);while(1);
返回参数：FE07912100001105FEFF//数据是0X11,新地址(ID)是FE05
 /*****************************************************************************************/
int zigbee_set_ID(int sent_port,uint ID)
{
    Serial.print("FE07"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("11");
  //新地址
  zigbee_ID(ID);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_set_netID(int sent_port,uint ID)
函数作用：设置模块的网络地址(ID)
输入参数：sent_port  ——————————源端口号
          ID      ——————————新的地址
输出参数：无
测试例程：zigbee_set_netID(145,65029);while(1);
返回参数：FE07912100001205FEFF//数据是0X12,新网络ID(netID)是FE05
 /*****************************************************************************************/
int zigbee_set_netID(int sent_port,uint netID)
{
    Serial.print("FE07"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("12");
  //新地址
  zigbee_ID(netID);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_set_route(int sent_port,int route)
函数作用：设置模块信道
输入参数：sent_port  ——————————源端口号
          route      ——————————新信道
输出参数：无
测试例程：zigbee_set_route(145,21);while(1);
返回参数：FE07912100001313FF//数据是0X13,信道是19(十六进制为13)
 /*****************************************************************************************/
int zigbee_set_route(int sent_port,int route)
{
    //Serial.print("FE07"); 
  Serial.print("FE07");
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("13");
  //新地址
  zigbee_route(route);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_set_baudrate(int sent_port,int baudrate)
函数作用：读取模块波特率
输入参数：sent_port  ——————————源端口号
          baudrate   ——————————波特率
输出参数：无
测试例程：zigbee_set_baudrate(145,3600);while(1);
返回参数：FE06912100001402FF//设置波特率是9600(0X02)
 /*****************************************************************************************/
int zigbee_set_baudrate(int sent_port,ulong baudrate)
{

  Serial.print("FE06"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X04)
  Serial.print("14");
  //设置波特率
  zigbee_baudrate(baudrate);
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_restart(int sent_port,int baudrate)
函数作用：重启模块
输入参数：sent_port  ——————————源端口号
输出参数：无
测试例程：zigbee_restart(145);while(1);
返回参数：FE06912100001402FF//设置波特率是9600(0X02)
 /*****************************************************************************************/
int zigbee_restart(int sent_port)
{
  
  Serial.print("FE05"); 
  
  //源端口号 
  zigbee_port(sent_port);
  //目的端口号(0X21)
  Serial.print("21");
  //远程地址(ID),只接受0X0000的包
  Serial.print("0000");
  //命令(0X10)
  Serial.print("10");
    
  Serial.print("FF");
}

/******************************************************************************************
函数名称：zigbee_rev()
函数作用：接收串口数据，并按照zigbee格式内容进行处理
输入参数：无
输出参数：返回接收数据长度
测试例程：zigbee_rev();
返回参数：串口输入(0102030405060708090A0B0C0D0E0F)
          未取消注释(   data[0]=1 \n data[1]=2 \n data[2]=3 \n data[3]=4 \n
                        data[4]=5 \n data[5]=6 \n data[6]=7 \n data[7]=8 \n
                        data[8]=9 \n data[9]=10\n data[10]=11 \n data[11]=12 \n 
                        data[12]=13 \n data[13]=14 \n data[14]=15)
 /*****************************************************************************************/
int zigbee_rev()
{
  //串口输入开始接收信息
  while(Serial.available()>0)
  {
   //数组开始接收串口输入信息
   data[data_cout]=Serial.read();
   //串口数据标志符置高
   rev_flag=true;
   //装载时间校准数据，防止误操作
   time_millis=millis();
    //偶数位数据为一直字符数据，开始处理
    if(data_cout%2!=0)
      {
        //装载时间校准数据，开始记录处理时间
        time_millis=millis();
        //处理数据，并转成int数型储存在数组中
        data[(data_cout/2)]=data_rate(data[(data_cout-1)],data[data_cout]); 
      }
    //数据记录操作
    data_cout++;
  }
  //如果距离上次串口处理时间超过20ms，则认为串口接收完毕
  if((time_millis+20)<=millis()&&rev_flag==true)
  {
    //串口接收标识符置低
    rev_flag=NULL;
    translation(data,(data_cout/2));  
    //串口输出数据
    for(int i=0;i<data_cout;i=i+2)
    {Serial.print("   data[");Serial.print(i/2);Serial.print("]=");Serial.println(data[i/2]);}    
    //返回数据长度
    Serial.print("data length=");Serial.println(data_cout/2);
    int data_cout_stored=data_cout/2;data_cout=0;
    return data_cout_stored;
  }
}

/******************************************************************************************
函数名称：zigbee_answer(int *data)
函数作用：接收串口数据，并按照zigbee格式内容进行处理
输入参数：无
输出参数：返回接收数据长度
测试例程：zigbee_rev()
返回参数：串口输入(0102030405060708090A0B0C0D0E0F)
          未取消注释(   data[0]=1 \n data[1]=2 \n data[2]=3 \n data[3]=4 \n
                        data[4]=5 \n data[5]=6 \n data[6]=7 \n data[7]=8 \n
                        data[8]=9 \n data[9]=10\n data[10]=11 \n data[11]=12 \n 
                        data[12]=13 \n data[13]=14 \n data[14]=15)
 /*****************************************************************************************/
uint zigbee_answer(int *data)
{
  //确定是否为21端口数据
  for(int i=0;i<2;i++)*data++;
  if(*data==33)
    for(int i=0;i<4;i++)*data++;
    switch(*data)
      {
        //读取地址ID，返回地址(收到的信息形如 FE0721900000210F00FF )
        case 33:{
                  *data++;uint ID_stored=*data;
                  *data++;ID_stored=((*data*256)+ID_stored);
                  for(int i=0;i<8;i++)*data--;
                  //串口显示地址
                  Serial.print("read ID=");Serial.println(ID_stored);
                  //返回地址
                  return ID_stored;
                }
        //读取网络地址(netID)，返回网络地址(收到的信息形如 FE0721900000228819F )
        case 34:{
                  *data++;uint netID_stored=*data;
                  *data++;netID_stored=((*data*256)+netID_stored);
                  for(int i=0;i<8;i++)*data--;
                  //串口显示网络地址
                  Serial.print("read netID=");Serial.println(netID_stored);
                  //返回网络地址
                  return netID_stored;
                }
        //读取信道，返回信道(收到的信息形如 FE0721900000228819F )
        case 35:{
                  *data++;int rout_stored=*data;
                  for(int i=0;i<7;i++)*data--;
                  //串口显示信道
                  Serial.print("read rout=");Serial.println(rout_stored);
                  //返回信道
                  return rout_stored;
                }
        //读取波特率，返回波特率(收到的信息形如 FE0721900000228819F )
        case 36:{
                  *data++;int baudrate_stored=*data;
                  for(int i=0;i<8;i++)*data--;
                  //串口显示波特率
                  zigbee_rebaudrate(baudrate_stored);
                  //返回波特率对应代码
                  return baudrate_stored;
                }
      }
}







