
uint8_t read_testKey() {
	return digitalRead(KEY);
}
void set_device(String content)
{
	int i = 0;
	int zhengshu_num = 0;
	int xiaoshu_num = 0;
	char buffer[60];
	int point_flag = 0;
	if (content.indexOf("温度") > 0)
	{
		//MasterSetData(0x01, 0x00, 0x02, temperatureH, temperatureL);
		//Serial.println(content.length());
		for (i = 0; i < content.length(); ++i)
		{
			if (content[i] >= '0' && content[i] <= '9')
			{
				break;
			}
		}
		for (; i < content.length(); ++i)
		{
			if (content[i] == '.')
			{
				point_flag = 1;

			}
			if (content[i] >= '0' && content[i] <= '9')
			{
				if (point_flag == 1) {
					xiaoshu_num = xiaoshu_num *10  + content[i] - '0';
				}
				else
				{
					zhengshu_num = zhengshu_num * 10 + content[i] - '0';
				}
			}
		}
		if (xiaoshu_num == 0)
		{
			zhengshu_num = zhengshu_num * 10;
		}
		else
		{
			xiaoshu_num = xiaoshu_num * 10;
			zhengshu_num = zhengshu_num * 10 + xiaoshu_num/10;
		}
		if (zhengshu_num != 0)
		{
			MasterSetData(0x01, 0x00, 0x02, zhengshu_num >> 8, zhengshu_num & 0x00ff);
			APPSendMessage("设置温度成功");
		}
		
	}
	else if (content.indexOf("速度") > 0)
	{
		for (; i < content.length(); ++i)
		{
			if (content[i] == '.')
			{
				break;
			}
			if (content[i] >= '0' && content[i] <= '9')
			{
				zhengshu_num = zhengshu_num * 10 + content[i] - '0';
			}
		}
		if (zhengshu_num != 0) {
			MasterSetData(0x01, 0x00, 0x04, zhengshu_num >> 8, zhengshu_num & 0x00ff);
			APPSendMessage("设置速度成功");
		}
		
	}
    
}
void testKeyLong()
{
	Serial.println("hhhhhhhhhhh");
	if (ledStatus != LED_BANDING)
	{
		bind_led();
		Serial.println("enter the band function");
		ledStatus = LED_BANDING;
		bandingCounter = 300;
		if (mqttClient.connected())
		{
			Serial.println(mTopics.subtopic);
			mqttClient.subscribe(mTopics.subtopic, 0);
		}
		
	}
	else if (ledStatus == LED_BANDING)
	{
		ledStatus = LED_NONE;
		if (mqttClient.connected())
		{
			mqttClient.unsubscribe(mTopics.subtopic);
		}
		
	}
	return;
}
//
void send_device_data()
{
	char buffer[80];
	sprintf(buffer, "当前数据:温度%.1f\n速度%d", (float)Parameter[1]/10, Parameter[3]);
	APPSendMessage(buffer);
}
//����MQTT������Ϣ
void JSONparse(char* topic, char *str) {
	Serial.println("topic:");
	Serial.println(topic);
	Serial.println(mTopics.adminAPPSubTopic);
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(str); //decoded
	if (!root.success()) {
		return;
	}

	String type = root["t"];
	String userid = root["i"];
	int msgID = root["mid"];
	String content = root["c"];
	String src = root["s"];//
	root.prettyPrintTo(Serial);

	if (strcmp(topic, mTopics.dataSubTopic) == 0) { //����΢��
		
		
	}
	else if (strcmp(topic, mTopics.adminAPPSubTopic) == 0) { //����APP
		Serial.println("22222");
		//Serial.println(content);
		//Serial.println(content.indexOf("查询数据"));
		if (content.indexOf("查询数据") > -1)
		{
			//Serial.println("Query the data of device");
			send_device_data();
		}
		else if (content.indexOf("设置") > -1)
		{
			Serial.println("set the data of device");
			set_device(content);
		}
	}
	else if (strcmp(topic, mTopics.subtopic) == 0) { //���Գ�ʼ��
		if (type.equals("apbd"))
		{
			RunningPara.messageReceiveID = msgID;
			String devID = root["d"];
			if (!devID.equals(clientID)) return;
			strcpy(mSavePara.Para.appid, userid.c_str());
			GetNetID();
		    mqttClient.subscribe(mTopics.adminAPPSubTopic, 0);
			APPSendMessage("设备已绑定成功!");

			Serial.println("设备已绑定成功!");
			Serial.println(mSavePara.Para.appid);

			mSavePara.Para.wifiMode = WIFI_STA;
			Serial.println("a");
			successLED = 2;
			Serial.println("b");
			mqttClient.unsubscribe(mTopics.subtopic);
			Serial.println("c");
			sprintf(mTopics.adminAPPubTopic, "%s/%s/appub", mSavePara.Para.appid, mSavePara.Para.usercode);
			sprintf(mTopics.adminAPPSubTopic, "%s/%s/%s/apsub", mSavePara.Para.appid, clientID, mSavePara.Para.usercode);
			Serial.println("d");
			//bind_led();
			Serial.println("f");
			ledStatus = LED_BANDING;
			Serial.println("g");
			bandingCounter = 0;
			Serial.println("h");
			CheckSaveConfig();
			Serial.println("i");
		}
	}
	return;
}