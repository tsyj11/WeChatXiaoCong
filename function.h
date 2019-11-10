//post上传数据
//int wulianUpload()
//{
//  HTTPClient http;
//  String addr ="http://php.yckz003.top";
//  addr.trim();
//  String url ="/data_up/history_upload";
//  url.trim();
//  String content = ReplaceFunction(mSavePara.Para.customuptemp);
//  http.begin(addr + url);
//  http.addHeader(true, "", mSavePara.Para.customupkey);
//  int httpCode = http.POST(content);
//  http.end();
//  return httpCode;
//}

//modbus协议16位CRC校验算法,返回unsigned short(两字节)类型的CRC校验值
//int * puchMsg, 要进行CRC校验的消息
//unsigned short usDataLen,消息中的字节数
unsigned short CRC16(int * puchMsg, unsigned short usDataLen)
{
  int i, j;                             
  unsigned short usRegCRC = 0xFFFF;     //用于保存crc值

  for(i=0; i<usDataLen; i++)            //循环处理传输的数据
  {
    usRegCRC ^= * puchMsg++;            //异或算法得到crc值
    for (j=0; j<8; j++)                 //循环处理每个bit位
    {
      if(usRegCRC & 0x0001)
      usRegCRC = usRegCRC >> 1 ^ 0xA001;
      else
        usRegCRC >>= 1;
    }
  }
  return usRegCRC;
}    

//调用MasterGetData(u8 devnum)时，实参填写设备码，可获取对应设备的各项参数
void MasterGetData(u8 devnum)
{
  int message[8] = {devnum,0x03,0x00,0x00,0x00,0x11}; 
  int crc = CRC16(message, 6);   //调用函数计算校验值
  int crclow = crc & 0x00ff;     //取crc校验值低八位
  int crchigh = crc >>8;         //取crc校验值高八位
  message[6] = crclow;           //先存crc校验值低八位
  message[7] = crchigh;          //再存crc校验值高八位
  for(int i=0; i<8; i++)     
  RS485.write(message[i]);      //通过串口按字节向从机发送获取数据指令
  delay(5000);
}

//MasterSetData(0x01, 0x00, 0x04, speedH, speedL);
//MasterSetData(0x01, 0x00, 0x02, temperatureH, temperatureL);


/*
  调用MasterSetData()时，实参依次填写设备码、寄存器地址高八位、寄存器地址低八位、设置参数高八位、设置参数低八位
  可修改对应寄存器中参数的值
*/
void MasterSetData(u8 devnum, u8 idhigh, u8 idlow, u8 setdatahigh, u8 setdatalow)
{
  int message[8] = {devnum, 0x06, idhigh, idlow, setdatahigh, setdatalow};
  int crc = CRC16(message, 6);   //调用函数计算校验值
  int crclow = crc & 0x00ff;     //取crc校验值低八位
  int crchigh = crc >>8;         //取crc校验值高八位
  message[6] = crclow;           //先存crc校验值低八位
  message[7] = crchigh;          //再存crc校验值高八位
  for(int i=0; i<8; i++)     
  RS485.write(message[i]);      //通过串口按字节向从机发送获取数据指令
  //delay(5000);
}

/*处理设备采集的数据，将两字节十六进制数拼接成一字节*/
void Joint()
{
  int i;
  int j = 0;
  for (i=3;i<37;i+=2)
  {
    Parameter[j]=(RS485A.UN2.slavePtr[i]<<8)+RS485A.UN2.slavePtr[i+1];
    j++;
  }
}

//连接到服务器
//void reconnect()
//{
//  //不断循环直到连接上服务器
//  while(!client.connected())
//  {
//    Serial.println("attempting mqtt connection....");
//    String clientid = "esp8266_D102";
//    //尝试连接服务器
//    if (client.connect(clientid.c_str()))
//    {
//      Serial.println("esp8266_D102 connected to the mqtt successfully !");
//      client.publish("outTopic", "hello world");
//      client.subscribe("inTopic");
//     }
//    else
//    {
//      Serial.print("failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(5000);
//      }
//   }
//}



//void StoragePara()
//{
//  RS485A.UN2.slaveBuf.devnum = RS485A.UN2.slavePtr[0];
//  RS485A.UN2.slaveBuf.funcode = RS485A.UN2.slavePtr[1];
//  RS485A.UN2.slaveBuf.datalen = RS485A.UN2.slavePtr[2];
//  RS485A.UN2.slaveBuf.type = Parameter[0];
//  RS485A.UN2.slaveBuf.temp = Parameter[1];
//  RS485A.UN2.slaveBuf.setTemp = Parameter[2];
//  RS485A.UN2.slaveBuf.speed = Parameter[3];
//  RS485A.UN2.slaveBuf.setSpeed = Parameter[4];
//  RS485A.UN2.slaveBuf.setTimeHour = Parameter[5];
//  RS485A.UN2.slaveBuf.setTimeMinute = Parameter[6];
//  RS485A.UN2.slaveBuf.timeHour = Parameter[7];
//  RS485A.UN2.slaveBuf.timeMinute = Parameter[8];
//  RS485A.UN2.slaveBuf.tempRangeMin = Parameter[9];
//  RS485A.UN2.slaveBuf.tempRangeMax = Parameter[10];
//  RS485A.UN2.slaveBuf.speedMin = Parameter[11];
//  RS485A.UN2.slaveBuf.speedMax = Parameter[12];
//  RS485A.UN2.slaveBuf.alarm = Parameter[13];
//  RS485A.UN2.slaveBuf.status = Parameter[14];
//  RS485A.UN2.slaveBuf.currentMode = Parameter[15];
//  RS485A.UN2.slaveBuf.powerOffMem = Parameter[16];
//  RS485A.UN2.slaveBuf.checkcode = Parameter[17];
//}
