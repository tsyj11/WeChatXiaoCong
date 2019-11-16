/*
   adminAPPubTopic: 设备发布给手机的mqtt主题
           设备报警或者反馈给管理员数据时发布到这个主题
           (用户id/123456/appub)
   subtopic:设备和管理员手机绑定通过这个主题
           (设备序列号/sub)
   adminAPPSubTopic:管理员手机远程发送数据给设备通过这个主题
           (用户id/设备序列号/123456/apsub)
*/

void WebSendMessage(String content)
{
	if (!mqttClient.connected()) return;
	delay(0); mqttClient.publish(mTopics.dataPubTopic, 0, false, content.c_str());
}
void APPSendMessagePublish(String content)
{
	if (!mqttClient.connected() && (strlen(mSavePara.Para.appid) < 10)) return;
	String data = base64Encode(content);
	delay(0); mqttClient.publish(mTopics.adminAPPubTopic, 0, false, data.c_str());
}
void connectToMqtt() {
	if (WiFi.isConnected()) {
		mqttClient.disconnect();
		mqttClient.connect();
	}
}

void APPSendMessageAlarm(String content)
{
	
	if (!mqttClient.connected() && (strlen(mSavePara.Para.appid) < 10)) return;
	String info = "{\"mt\":23,\"f\":\"" + String(clientID) + "\",\"c\":\"" + content + "\"}";
	String data = base64Encode(info);
	delay(0); mqttClient.publish(mTopics.adminAPPubTopic, 0, false, data.c_str());
}

void APPSendMessage(String content)
{
	if (!mqttClient.connected() && (strlen(mSavePara.Para.appid) < 10)) return;
	String info = "{\"mt\":1,\"t\":\"ack\",\"f\":\"" + String(clientID) + "\",\"c\":\"" + content + "\"}";
	String data = base64Encode(info);
	delay(0); mqttClient.publish(mTopics.adminAPPubTopic, 0, false, data.c_str());
}

void onMqttConnect() {
	Serial.println("连接上MQTT服务器");
	mqttReconnectTimer = 0;
	APPSendMessage(emoji_1f60a);
	mqttClient.subscribe(mTopics.dataSubTopic, 0);
	mqttClient.subscribe(mTopics.adminAPPSubTopic, 0);
	RunningPara.mqttState = 1;
	MQTTLED_ON();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
	mqttReconnectTimer = 1;
	connectToMqtt();
	MQTTLED_OFF();
	RunningPara.mqttState = 0;
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
	//  ledStatus = LED_MQTTON;
}
void onMqttUnsubscribe(uint16_t packetId) {
	//  ledStatus = LED_MQTTON;
}

void onMqttPublish(uint16_t packetId) {
	//  ledStatus = LED_MQTTON;
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
	payload[len] = 0;
	String payloadStr = "";
	if (strcmp(topic, mTopics.dataSubTopic) == 0) payloadStr = payload;
	else payloadStr = base64Decode(payload);

	JSONparse(topic, (char *)payloadStr.c_str());
}

void MqttInit() {
	mqttClient.disconnect();
	mqttClient.onConnect(onMqttConnect);
	mqttClient.onDisconnect(onMqttDisconnect);
	mqttClient.onSubscribe(onMqttSubscribe);
	mqttClient.onUnsubscribe(onMqttUnsubscribe);
	mqttClient.onMessage(onMqttMessage);
	mqttClient.onPublish(onMqttPublish);
	mqttClient.setServer(MQTTSERVER, 2568);
	mqttClient.setKeepAlive(30).setCredentials("test_jim", "123456").setClientId(clientID);
	mqttClient.connect();
}