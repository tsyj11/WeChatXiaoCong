char temphtml[1600];
void sendIndex()
{
	sprintf(temphtml, "<html><head><meta charset='utf-8'><title>配置</title></head><body><form action='/' method='POST'><fieldset><legend align='center'>网页配置</legend><table align='center'><tr><td>设备SN号</td><td><p>%s</p></td></tr><tr><td>设备名字</td><td><input type='text' name='name' value='%s'></td></tr><tr><td>路由SSID:</td><td><input type='text' name='ssid' value='%s'></td></tr><tr><td>路由密码:</td><td><input type='text' name='pwd' value='%s'></td></tr><tr><td>登陆账号:</td><td><input type='text' name='wacc' value='%s'></td></tr><tr><td>登陆密码:</td><td><input type='text' name='wpwd' value='%s'></td></tr><tr><td>微信ID:</td><td><input type='text' name='wxid' value='%s'></td></tr><tr><td>手机appid:</td><td><input type='text' name='appid' value='%s'></td></tr><tr><td>手机管理员:</td><td><input type='text' name='gsmTo' value='%s'></td></tr><tr><td>配对码:</td><td><input type='text' name='code' value='%s'></td></tr><tr><td colspan='2' align='center'><button type='submit'>更新</button></td></tr></table></fieldset></form><fieldset><legend align='center'>固件更新</legend><table align='center'><tr><td colspan='2' align='center'><button onclick='window.location.href=&quot;/update&quot;'>升级</button></td></tr></table></fieldset></body></html>", clientID, mSavePara.Para.nickname, mSavePara.Para.stassid, mSavePara.Para.stapasswd, mSavePara.Para.authname, mSavePara.Para.authpasswd, mSavePara.Para.openID, mSavePara.Para.appid, mSavePara.Para.gsmTo[0], mSavePara.Para.usercode);
	httpserver.send_P(200, "text/html", temphtml);
}

void HttpServerInit() {
	httpserver.on("/", HTTP_GET, []() {
		sendIndex();
	});
	httpserver.on("/", HTTP_POST, []() {
		String name = httpserver.arg("name");
		String stassid = httpserver.arg("ssid");
		String stapasswd = httpserver.arg("pwd");
		String authname = httpserver.arg("wacc");
		String authpasswd = httpserver.arg("wpwd");
		String  appid = httpserver.arg("appid");
		String  openid = httpserver.arg("wxid");
		String usercode = httpserver.arg("code");
		String gsmTo = httpserver.arg("gsmTo");

		gsmTo.trim();
		name.trim();
		stassid.trim();
		stapasswd.trim();
		authname.trim();
		authpasswd.trim();
		openid.trim();
		appid.trim();
		usercode.trim();

		if (name.length() < sizeof(mSavePara.Para.nickname) - 1) strcpy(mSavePara.Para.nickname, name.c_str());
		if (authname.length() < sizeof(mSavePara.Para.authname) - 1) strcpy(mSavePara.Para.authname, authname.c_str());
		if (authpasswd.length() < sizeof(mSavePara.Para.authpasswd) - 1) strcpy(mSavePara.Para.authpasswd, authpasswd.c_str());
		if (stassid.length() < sizeof(mSavePara.Para.stassid) - 1) strcpy(mSavePara.Para.stassid, stassid.c_str());
		if (stapasswd.length() < sizeof(mSavePara.Para.stapasswd) - 1) strcpy(mSavePara.Para.stapasswd, stapasswd.c_str());
		if (openid.length() < sizeof(mSavePara.Para.openID) - 1) strcpy(mSavePara.Para.openID, openid.c_str());
		if (appid.length() < sizeof(mSavePara.Para.appid) - 1) strcpy(mSavePara.Para.appid, appid.c_str());
		if (gsmTo.length() < sizeof(mSavePara.Para.gsmTo[0]) - 1) strcpy(mSavePara.Para.gsmTo[0], gsmTo.c_str());
		if (usercode.length() > 0 && usercode.length() < sizeof(mSavePara.Para.usercode) - 1) strcpy(mSavePara.Para.usercode, usercode.c_str());

		CheckSaveConfig();
		sendIndex();
	});
	httpserver.onNotFound([]() {
		sendIndex();
	});
	httpUpdater.setup(&httpserver);
	httpserver.begin();
}