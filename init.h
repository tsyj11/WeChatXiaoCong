//设置STA模式
void STA_wifi()
{
  WiFi.begin(mSavePara.Para.stassid, mSavePara.Para.stapasswd);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void StartSta() {
	if (mSavePara.Para.isDhcp == 0) {
		IPAddress ip(mSavePara.Para.ip[0], mSavePara.Para.ip[1], mSavePara.Para.ip[2], mSavePara.Para.ip[3]);
		IPAddress dns(mSavePara.Para.dns[0], mSavePara.Para.dns[1], mSavePara.Para.dns[2], mSavePara.Para.dns[3]);
		IPAddress gateway(mSavePara.Para.gateway[0], mSavePara.Para.gateway[1], mSavePara.Para.gateway[2], mSavePara.Para.gateway[3]);
		IPAddress subnet(mSavePara.Para.subnet[0], mSavePara.Para.subnet[1], mSavePara.Para.subnet[2], mSavePara.Para.subnet[3]);
		WiFi.config(ip, gateway, subnet, dns, dns);
	}
	WiFi.hostname(mSavePara.Para.nickname);
	WiFi.begin(mSavePara.Para.stassid, mSavePara.Para.stapasswd);
}
//设置AP模式
void StartAP() {
	//  IPAddress apIP(192, 168, 4, 1);
	//  IPAddress netMsk(255, 255, 255, 0);
	//  WiFi.softAPConfig(apIP, apIP, netMsk);  
	sprintf(mSavePara.Para.apssid, "ht_%02X%02X%02X", mac[0], mac[1], mac[2]);
	WiFi.softAP(mSavePara.Para.apssid, mSavePara.Para.appasswd);
}
void StartWiFi() {
	WiFi.disconnect();
	if (mSavePara.Para.wifiMode == WIFI_OFF) {
		WiFi.mode(WIFI_OFF);
	}
	else if (mSavePara.Para.wifiMode == WIFI_AP) {
		WiFi.mode(WIFI_AP);
		StartAP();
	}
	else if (mSavePara.Para.wifiMode == WIFI_AP_STA) {
		WiFi.mode(WIFI_AP_STA);
		StartAP();
		StartSta();
	}
	else {
		WiFi.mode(WIFI_STA);
		StartSta();
	}
}

//初始化函数
void device_init()
{
	delay(200);
	RS485.setPin(13, 15);
	RS485.begin(9600);
	Serial.begin(115200);
	//初始化引脚
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(RELAY, OUTPUT);
	pinMode(INPUT1, INPUT_PULLUP);

	pinMode(LED1, OUTPUT);
	//pinMode(KEY, INPUT_PULLUP);
	pinMode(KEY, INPUT);
	pinMode(KEY2, INPUT_PULLUP);
	NETLED_OFF();
	MQTTLED_OFF();
	STATUSLED_OFF();

	ReadConfig();  //读取EEPROM数据到设备配置里
	

	{
		//打印mqtt相关主题
		Serial.println("打印设备参数信息:");
		Serial.println("adminAPPubTopic:");
		Serial.println(mTopics.adminAPPSubTopic);
		Serial.println("adminAPPSubTopic:");
		Serial.println(mTopics.adminAPPSubTopic);
		Serial.println("dataSubTopic:");
		Serial.println(mTopics.dataSubTopic);
		Serial.println("dataPubTopic:");
		Serial.println(mTopics.dataPubTopic);
		Serial.println("mTopics.subtopic");
		Serial.println(mTopics.subtopic);

		//打印部分设备配置参数
		Serial.println("openid:");
		Serial.println(mSavePara.Para.openID);
		Serial.println("sta_ssid:");
		Serial.println(mSavePara.Para.stassid);
		Serial.println("sta_passwd:");
		Serial.println(mSavePara.Para.stapasswd);
		Serial.println("ap_ssid:");
		Serial.println(mSavePara.Para.apssid);
		Serial.println("ap_passwd:");
		Serial.println(mSavePara.Para.appasswd);
	}
	WiFi.macAddress(mac);
	sprintf(clientID, PRODUCT"%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	Serial.println(clientID);
	sprintf(mTopics.adminAPPubTopic, "%s/%s/appub", mSavePara.Para.appid, mSavePara.Para.usercode);
	sprintf(mTopics.adminAPPSubTopic, "%s/%s/%s/apsub", mSavePara.Para.appid, clientID, mSavePara.Para.usercode);
	sprintf(mTopics.dataSubTopic, "%s/%s/dsub", clientID, mSavePara.Para.opencode);
	sprintf(mTopics.dataPubTopic, "%s/%s/dpub", clientID, mSavePara.Para.opencode);
	sprintf(mTopics.subtopic, "%s/sub", clientID);

	{//按键1开机长按进入WIFI AP,进行配网
		if (analogRead(A0) > 100)
		{
			mSavePara.Para.wifiMode = WIFI_AP_STA;
			StartWiFi();
			HttpServerInit();
			while (1)
			{
				NETLED_ON();
				MQTTLED_ON();
				STATUSLED_ON();
				httpserver.handleClient();
				delay(0);
			}
		}
	}
	STA_wifi(); 

	MqttInit();
}
