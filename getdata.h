int temp_count = 0;
int threadRs485_entry(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1)
  {
	if (analogRead(A0) > 100) {
		testKeyLong();
	}
	int buttonState = digitalRead(KEY);//读取当前按键状态
	if (buttonState == LOW) {
		smartConfig();
	}
	
    Serial.println("\n向设备请求参数信息.....");
    MasterGetData(0x01);              //向设备请求参数信息
    PT_TIMER_DELAY(pt, 500);
    if (RS485.available()>0)          //判断串口缓冲区是否接受到数据
    {
      PT_TIMER_DELAY(pt, 200);
      int len = RS485.available();    //读取串口缓冲区数据的字节数
      if (len < 40)                   //如果从机返回的数据长度等于39字节，则存储该数据
      {
		 // RunningPara.device_ok = 1;
        for (int i=0; i<len; i++)
        {
          RS485A.UN2.slavePtr[i] = RS485.read();     //将串口缓冲区的数据按字节存储到slavePtr[]数组中
         }
		Serial.println("设备返回信息：");
		for (int i = 0; i < len; i++)
		{
			Serial.printf("%02x ", RS485A.UN2.slavePtr[i]);
		}

       }
	  else { while (RS485.available()) RS485.read(); }
      /*Serial.println("设备返回信息：");
      for (int i=0; i<len; i++)
       {
         Serial.printf("%02x ",RS485A.UN2.slavePtr[i]);
        } */ 
      }
      /*{ Serial.println("串口通讯失败!"); }*/
      Joint();
      Serial.println("\n处理后的有用数据：");
      for(int i=0;i<17;i++)
      {
        Serial.printf("%.4x ",Parameter[i]);
      }
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["机器类型"] = Parameter[0];
      root["当前温度"] = Parameter[1];
      root["温度设定值"] = Parameter[2];
      root["当前速度"] = Parameter[3];
      root["速度设定值"] = Parameter[4];
      root["小时设定值"] = Parameter[5];
      root["分钟设定值"] = Parameter[6];
      root["当前小时值"] = Parameter[7];
      root["当前分钟值"] = Parameter[8];
      root["温度量程最小值"] = Parameter[9];
      root["温度量程最大值"] = Parameter[10];
      root["速度最小值"] = Parameter[11];
      root["速度最大值"] = Parameter[12];
      root["故障报警"] = Parameter[13];
      root["运行状态"] = Parameter[14];
      root["当前模式"] = Parameter[15];
      root["掉电记忆"] = Parameter[16];
	  check_alarm();
	  if (temp_count == 0)
	  {
		  int code = wulianUpload();
		  Serial.print("返回http状态码:");
		  Serial.println(code);
	  }
	  ++temp_count;
	  if (temp_count >= 3)
		  temp_count = 0;
      root.prettyPrintTo(Serial);
    PT_TIMER_DELAY(pt, 300);
  }
  PT_END(pt);
}

int wulianUpload()
{
	HTTPClient http;
	String str = "userkey:";
	//str = str + String(mSavePara.Para.deviceID)；
	str = str + String(mSavePara.Para.appid) + "," + String(clientID);
	String addr = String(mSavePara.Para.customupaddr);
	addr.trim();
	String url = String(mSavePara.Para.customupurl);
	url.trim();
	char buffer[80];
	sprintf(buffer, "{\"w\":%.1f,\"sd\":%d}", (float)Parameter[1] / 10, Parameter[3]);
	//String content = "";//ReplaceFunction(mSavePara.Para.customuptemp);
	http.begin(addr + url);
	http.addHeader(true, "", str);//mSavePara.Para.customupkey);
	//APPSendMessage(content);
	Serial.println("上传历史数据:1");
	Serial.println(buffer);
	Serial.println(str);
	Serial.println(mSavePara.Para.customupaddr);
	Serial.println(mSavePara.Para.customupurl);
	int httpCode = http.POST(buffer);
	http.end();
	return httpCode;
}

//检查报警
void check_alarm()
{
	//Parameter[13];
	int num = Parameter[13];
	if (num != 0) {
		Serial.print("报警标志位:");
		Serial.println(num);
		if (num & 0x01 > 0) {
			//内部传感器故障
			APPSendMessageAlarm("内部传感器故障");
		}
		if (num & 0x02 > 0) {
			//内部传感器温度报警
			APPSendMessageAlarm("内部传感器温度报警");
		}
		if (num & 0x04 > 0) {
			//外部传感器故障
			APPSendMessageAlarm("外部传感器故障");
		}
		if (num & 0x08 > 0) {
			//外部传感器超温
			APPSendMessageAlarm("外部传感器超温");
		}
		if (num & 0x10 > 0) {
			//机械限温超温
			APPSendMessageAlarm("机械限温超温");
		}
		if (num & 0x20 > 0) {
			//机械限温传感器故障
			APPSendMessageAlarm("机械限温传感器故障");
		}
		if (num & 0x40 > 0) {
			//电机堵转
			APPSendMessageAlarm("电机堵转");
		}
		if (num & 0x80 > 0) {
			//外部传感器跌落
			APPSendMessageAlarm("外部传感器跌落");
		}
		if (num & 0x0100 > 0) {
			//外部传感器未插入
			APPSendMessageAlarm("外部传感器未插入");
		}
	}
}
