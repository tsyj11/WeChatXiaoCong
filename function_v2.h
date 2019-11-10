void SaveConfig() {
	int len = sizeof(SavePara);
	EEPROM.begin(len);
	for (int i = 0; i < len; i++) EEPROM.write(i, mSavePara.bytes[i]);
	EEPROM.commit();
}

void ResetConfig()
{
	mSavePara.Para.reset = RESTVAL;
	mSavePara.Para.wifiMode = WIFI_AP_STA;
	mSavePara.Para.timeControlEn = 0;
	mSavePara.Para.ntpEn = 1;
	mSavePara.Para.uploadEn = 0;
	mSavePara.Para.emailAlarmEn = 0;
	mSavePara.Para.appAlarmEn = 1;
	mSavePara.Para.wxAlarmEn = 1;
	mSavePara.Para.ldctrlEn = 0;
	mSavePara.Para.alarmEn = 1;
	mSavePara.Para.gsmAlarmEn = 0;
	mSavePara.Para.callAlarmEn = 0;
	mSavePara.Para.isDhcp = 1;
	mSavePara.Para.relayStartUp = 0;
	mSavePara.Para.controllerAlarmEn = 1;

	mSavePara.Para.deviceID = 1;

	mSavePara.Para.ip[0] = 192;
	mSavePara.Para.ip[1] = 168;
	mSavePara.Para.ip[2] = 1;
	mSavePara.Para.ip[3] = 30;

	mSavePara.Para.dns[0] = 114;
	mSavePara.Para.dns[1] = 114;
	mSavePara.Para.dns[2] = 114;
	mSavePara.Para.dns[3] = 114;
	mSavePara.Para.gateway[0] = 192;
	mSavePara.Para.gateway[1] = 168;
	mSavePara.Para.gateway[2] = 1;
	mSavePara.Para.gateway[3] = 1;
	mSavePara.Para.subnet[0] = 255;
	mSavePara.Para.subnet[1] = 255;
	mSavePara.Para.subnet[2] = 255;
	mSavePara.Para.subnet[3] = 0;

	mSavePara.Para.highTemp = 0; //超温报警
	mSavePara.Para.lowTemp = 0;  //低温报警
	mSavePara.Para.highHumi = 0; //超湿
	mSavePara.Para.lowHumi = 0; //低湿
	mSavePara.Para.highspeed = 0;//超速

	mSavePara.Para.emailHost = 0;
	//  strcpy(mSavePara.Para.emailAccount, "15268026301@139.com");
	//  strcpy(mSavePara.Para.emailPasswd, "a4752386");
	strcpy(mSavePara.Para.emailsubject, "邮件报警");
	strcpy(mSavePara.Para.emailAccount, "");
	strcpy(mSavePara.Para.emailPasswd, "");
	for (int i = 0; i < 5; i++) strcpy(mSavePara.Para.emailTo[i], " ");
	//  strcpy(mSavePara.Para.emailTo[0],"15268026301@139.com");
	//  strcpy(mSavePara.Para.emailTo[1],"844725532@qq.com");
	//  strcpy(mSavePara.Para.emailTo[2],"3050311118@163.com");
	//  strcpy(mSavePara.Para.emailTo[3],"a3050311118@163.com");
	//  strcpy(mSavePara.Para.emailTo[4],"15268026301@139.com");

	for (int i = 0; i < 6; i++)
	{
		//    strcpy(mSavePara.Para.gsmTo[i], "15268026301");
		strcpy(mSavePara.Para.gsmTo[i], " ");
	}
	strcpy(mSavePara.Para.feeTemplate, "您的手机账户实际可用");

	for (int i = 0; i < CRONLEN; i++)
	{
		mSavePara.Para.cronhour[i] = 7;
		mSavePara.Para.cronmin[i] = 0;
		mSavePara.Para.cronact[i] = 0;
		mSavePara.Para.cronEn[i] = 0;
		mSavePara.Para.workday[i] = 0;
	}
	for (int i = 0; i < TEMPLLEN; i++)
	{
		mSavePara.Para.templAct[i] = 0;
		strcpy(mSavePara.Para.dataValue[i], "0");
	}
	for (int i = 0; i < CONLEN; i++)
	{
		strcpy(mSavePara.Para.concmds[i], "");
		mSavePara.Para.conact[i] = 0;
	}
	for (int i = 0; i < YUYINLEN; i++)
	{
		strcpy(mSavePara.Para.yuyincmds[i], "  ");
		strcpy(mSavePara.Para.yuyinact[i], "  ");
		mSavePara.Para.yuyindo[i] = 0;
	}

	//ONENET
	//api-key: RwCrCfM=TMzpdLoKaH6ayXtY=7I=
	//设备ID 17718953
	//{"datastreams":[{"id":"aa","datapoints":[{"value":123}]}]}

	mSavePara.Para.wlinterval = 15;
	strcpy(mSavePara.Para.customupaddr, "http://php.yckz003.top");
	strcpy(mSavePara.Para.customupurl, "/data_up/history_upload");
	strcpy(mSavePara.Para.customupkey, "userkey:2512d8b672524137af5e85155276ec4b");
	//  strcpy(mSavePara.Para.customuptemp, "[{ \"Name\":\"bbb\", \"Value\":\"TH40W\" }]");
	//  strcpy(mSavePara.Para.customuptemp, "{\"s\":1,\"sd\":1,\"w\":20,\"g\":1,\"n\":1,\"y\":1,\"z\":1,\"lx\":1,\"mz\":1}");
	strcpy(mSavePara.Para.customuptemp, "{\"lx\":MT,\"mz\":\"NAME\",\"s\":SHD,\"sd\":SD,\"w\":WD,\"g\":GZD,\"n\":ND,\"y\":YL,\"z\":ZW}");
	mSavePara.Para.fileRecordInterval = 30;
	strcpy(mSavePara.Para.fileTemplate, "(tm),(wd),(shd),(sd),(nd),(yl),(gzd),(sj)");

	strcpy(mSavePara.Para.stassid, "vivo");
	strcpy(mSavePara.Para.stapasswd, "15121181361");
	//  strcpy(mSavePara.Para.openID, "omB5-1q_KNa7_jtUmAgPJqqriKA8");
	//  strcpy(mSavePara.Para.appid, "52961cf3-0ad4-39bf-b8d9-5bb653475bae");

	//  strcpy(mSavePara.Para.stassid, "zzz");
	//  strcpy(mSavePara.Para.stapasswd, "12345678");
	strcpy(mSavePara.Para.openID, "omB5-1q_KNa7_jtUmAgPJqqriKA8");
	strcpy(mSavePara.Para.openID2, "omB5-1q_KNa7_jtUmAgPJqqriKA8");
	//  strcpy(mSavePara.Para.appid, "52961cf3-0ad4-39bf-b8d9-5bb653475bae");
	strcpy(mSavePara.Para.appid, "6c85e8f4-2094-3956-870c-f218b0f2e63e");

	sprintf(mSavePara.Para.apssid, "ht_%02X%02X%02X", mac[0], mac[1], mac[2]);
	strcpy(mSavePara.Para.appasswd, "12345678");
	strcpy(mSavePara.Para.authname, "admin");
	strcpy(mSavePara.Para.authpasswd, "123456");
	strcpy(mSavePara.Para.usercode, "123456");
	strcpy(mSavePara.Para.opencode, "123456");
	strcpy(mSavePara.Para.nickname, mSavePara.Para.apssid);
	sprintf(mSavePara.Para.punycode, "ht_%02X%02X%02X", mac[0], mac[1], mac[2]);
	strcpy(mSavePara.Para.iccid, "");
	SaveConfig();
	//memcpy(oldSavePara.bytes, mSavePara.bytes, sizeof(mSavePara));
}

void ReadConfig() {
	int len = sizeof(SavePara);
	EEPROM.begin(len);
	for (int i = 0; i < len; i++)
	{
		mSavePara.bytes[i] = EEPROM.read(i);
	}
	if (mSavePara.Para.reset != RESTVAL) {
		ResetConfig();
	}
}

void CheckSaveConfig() {
	SaveConfig();
}

void GetNetID()
{
	sprintf(mTopics.adminAPPubTopic, "%s/%s/appub", mSavePara.Para.appid, mSavePara.Para.usercode);
	sprintf(mTopics.adminAPPSubTopic, "%s/%s/%s/apsub", mSavePara.Para.appid, clientID, mSavePara.Para.usercode);
	sprintf(mTopics.dataSubTopic, "%s/%s/dsub", clientID, mSavePara.Para.opencode);
	sprintf(mTopics.dataPubTopic, "%s/%s/dpub", clientID, mSavePara.Para.opencode);
	sprintf(mTopics.subtopic, "%s/sub", clientID);
}

void CheckTopics()
{
	if (memcmp(&mTopics, &oldTopics, sizeof(Topics)) != 0) {
		if (memcmp(mTopics.dataSubTopic, oldTopics.dataSubTopic, sizeof(mTopics.dataSubTopic)) != 0) mqttClient.subscribe(mTopics.dataSubTopic, 0);
		if (memcmp(mTopics.adminAPPSubTopic, oldTopics.adminAPPSubTopic, sizeof(mTopics.adminAPPSubTopic)) != 0) if (strlen(mSavePara.Para.appid) > 10) mqttClient.subscribe(mTopics.adminAPPSubTopic, 0);
	}
	memcpy(&oldTopics, &mTopics, sizeof(Topics));
}

String base64Encode(String content)
{
	char encoded[1024]; encoded[0] = b64_alphabet[random(51)];
	encoded[1] = encoded[0] + 1;
	strcpy(encoded + 1, b64_encode(content.c_str()));
	//转换结束再进行前面1,2字节的位置交换
	char temp = encoded[0];
	encoded[0] = encoded[1];
	encoded[1] = temp;
	return String(encoded);
}

String base64Decode(String content)
{
	//转换结束再进行前面1,2字节的位置交换
	char temp = content[0];
	content[0] = content[1];
	content[1] = temp;
	char* decoded = b64_decode((char *)(content.c_str() + 1));
	return String(decoded);
}

void bind_led()
{
	for (int i = 0; i < 5; i++) {
		STATUSLED_ON(); delay(100);
		STATUSLED_OFF(); delay(100);
	}
}

void smartConfig()
{
	int smartTimer = 0;
	WiFi.mode(WIFI_STA);
	WiFi.beginSmartConfig();
	while (1)
	{
		STATUSLED_ON(); delay(1000);
		STATUSLED_OFF(); delay(1000);
		smartTimer++;
		if (smartTimer > 30) {
			//StartSta();
			break;
		}
		if (WiFi.smartConfigDone())
		{
			delay(500);
			Serial.println("智能配网:");
			Serial.println(WiFi.SSID().c_str());
			Serial.println(WiFi.psk().c_str());
			strcpy(mSavePara.Para.stassid, WiFi.SSID().c_str());
			strcpy(mSavePara.Para.stapasswd, WiFi.psk().c_str());
			mSavePara.Para.wifiMode = WIFI_STA;
			CheckSaveConfig();
			successLED = 1;
			//StartSta();
			break;
		}
	}
}