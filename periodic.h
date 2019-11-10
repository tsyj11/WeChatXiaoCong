int threadPeriodic_entry(struct pt *pt)
{
	PT_BEGIN(pt);
	while (1)
	{
		//  if((priodicCount%30) == 0 && (WiFi.status() == WL_CONNECTED)) {if(!mqttClient.connected()) mqttClient.connect();}
		if (bandingCounter > 0) {//绑定取消计时
			bandingCounter--;
			if (bandingCounter == 0) {
				ledStatus = LED_NONE;
				if (mqttClient.connected())
				{
					mqttClient.unsubscribe(mTopics.subtopic);
				}
			}
		}
		/*
		if (RunningPara.device_ok == 1) {
			int code = wulianUpload();
			Serial.print("返回http状态码:");
			Serial.println(code);
		}
		*/
		
		PT_TIMER_DELAY(pt, 1000);
	}
	PT_END(pt);
}