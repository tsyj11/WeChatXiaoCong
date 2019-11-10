#define PRODUCT "CLF02"
#define MQTTSERVER "www.yckz003.top" 

SoftwareSerial RS485;

ESP8266WebServer  httpserver(80);
ESP8266HTTPUpdateServer httpUpdater;
AsyncMqttClient mqttClient;
//输入输出引脚配置
#define RESET 1
#define KEY  0  //绑定按键引脚
#define KEY2 A0 //配网按键引脚

#define LED1 2 //模块状态
#define LED3 3 //连接公网
#define LED2 10 //连接到路由器

int Parameter[20];

#define INPUT1 12
#define OW   4
#define RELAY  16

#define SDA 4
#define SCL 5

#define STATUSLED_ON()  digitalWrite(LED1,LOW)
#define STATUSLED_OFF() digitalWrite(LED1,HIGH)
#define STATUSLED_TOGGLE() digitalWrite(LED1,!digitalRead(LED1))
#define NETLED_ON()  digitalWrite(LED2,LOW)
#define NETLED_OFF() digitalWrite(LED2,HIGH)
#define NETLED_TOGGLE() digitalWrite(LED2,!digitalRead(LED2))
#define MQTTLED_ON()  digitalWrite(LED3,LOW)
#define MQTTLED_OFF() digitalWrite(LED3,HIGH)
#define MQTTLED_TOGGLE() digitalWrite(LED3,!digitalRead(LED3))
#define OUTPUT1_ON()  digitalWrite(RELAY,HIGH)
#define OUTPUT1_OFF() digitalWrite(RELAY,LOW)
#define OUTPUT1_TOGGLE() digitalWrite(RELAY,!digitalRead(RELAY))

#define RESTVAL 9
#define SENSORNUM 10

#define VERSION "H_V2.0" 
#define CRONLEN  4
#define TEMPLLEN  6
#define CONLEN   3
#define YUYINLEN 5
#define NAME_NUM 15
#define NTPUPDATE_TIME 10
#define DISP_LEN 5
#define  DS_MAX 5
#define CONTROLDELAY_TIME 30

char clientID[30];
byte mac[6];

//网络平台类型
enum {
	ONENET = 1,
	LEWEI,
	YEELINK,
	CUSTOM
};

enum {
	PIN_ON,
	PIN_OFF,
	PIN_TOGGLE
};

enum MESSFROM {
	MSG_FROM_WIFI_APP,
	MSG_FROM_WIFI_WX,
	MSG_FROM_GSM_APP,
	MSG_FROM_GSM_WX,
	MSG_FROM_GSM_SMS,
	MSG_FROM_GSM_WEB
};

enum MessageStatus {
	SENDING,
	SUCCESS,
	FAILED,
	RECEIVE,
	READ
};

enum MqttMsgType {
	NONE,
	TXT,
	VOICE,
	VIDEO,
	IMAGE,
	LOCATION,
	MSG_FILE,
	CALL,
	STATE,
	RICH_TEXT,
	CMD,
	FRIEND,
	SYS,
	RESERVED1,
	RESERVED2,
	RESERVED3,
	APBDACK,
	RC315,
	I2C,
	IR,
	MODBUS,
	LOWP,
	PERI,
	ALARM
};

enum {
	INFO_TYPE_EMAILSET,
	INFO_TYPE_OTHERSET,
	//  INFO_TYPE_PERI,
	INFO_TYPE_NETACCOUNT,
	INFO_TYPE_WIFISET,
	//  INFO_TYPE_CRONSET,
	//  INFO_TYPE_LDSET,
	INFO_TYPE_ACKBD,
	INFO_TYPE_ACKJC,
	INFO_TYPE_ONLINE,
	INFO_TYPE_BOARDINFO,
	INFO_TYPE_WXINFO,
	INFO_TYPE_WXIO,
	INFO_TYPE_INDEX,
	INFO_TYPE_WULIANSET,
	INFO_TYPE_ACKWEB,
	INFO_TYPE_ACKMSG,
	//  INFO_TYPE_YUYIN,
	INFO_TYPE_SEARCHINFO,
	INFO_TYPE_SENSORLIST,
	INFO_TYPE_IRLIST,
	INFO_TYPE_WIRELESSLIST,
	INFO_TYPE_GETTIME,
	INFO_TYPE_MODBUS
};

enum LED {
	LED_NONE,
	LED_MAIL,
	LED_SMARTCONFIG,
	LED_SMARTCONFIGDONE,
	LED_BANDING,
	LED_SUCCESS
};

enum MSG_MESSAGE {
	MSG_TYPE_NONE,
	MSG_TYPE_WXBDACK,
	MSG_TYPE_WXJCACK
};

enum GSMSTATUS {
	INIT,
	REGISTERED,
	UNREGISTER
};

enum SIMSTATUS {
	UNINSERT,
	INSERT
};

volatile byte successLED = 0;

//mqtt相关
typedef struct {
	char adminAPPSubTopic[70];
	char adminAPPubTopic[70];
	char dataSubTopic[36];
	char dataPubTopic[36];
	char subtopic[36];
}Topics;
Topics mTopics, oldTopics;
String alarmContent = "";

String appMessageContent = "";
String wxMessageContent = "";
String emailMessageContent = "";
String msgAckTemp = "";


time_t mqttReconnectTimer;
time_t wifiReconnectTimer;

byte bandingCounter = 0; //绑定设备计数
byte ledStatus = 0;
struct Button smartConfigKey, input1, testKey;

static struct pt thread_key,
         thread_modbus, 
	     thread_periodic;

//modbus数据结构
#pragma pack(1)  //让编译器将结构体数据强制连续排列，数据输出更紧凑
typedef struct
{
  union     //UN1  主机向从机写数据
  {
    struct
    {
      u8 devnum;            //设备号
      u8 funcode;           //功能码(0x10写)
      u8 idhigh;            //起始地址高八位
      u8 idlow;             //起始地址低八位
      u8 numhigh;           //寄存器数高八位
      u8 numlow;            //寄存器数低八位
      u8 datalen;           //数据字节长度
      s16 setTemp;          //设定温度
      s16 setSpeed;         //设定速度
      s16 setTimeHour;      //设定时间（小时）
      s16 setTimeMinute;    //设定时间（分钟）
      u16 tempMin;          //设定温度量程最小值
      u16 tempMax;          //设定温度量程最大值
      u16 speedMin;         //设定温度最小值
      u16 speedMax;         //设定温度最大值
      u16 setStatus;        //设置运行状态
      s16 setCurrentMode;   //设置当前模式（1/2/3）
      u16  setPwrOffMem;    //设置掉电记忆(0/1)
    } __attribute__ ((aligned (1))) masterBuf; 
    u8 masterPtr[40];
  } UN1;
  
  union       //UN2  当主机发送读取指令后从机向主机返回的参数
  {
    struct {
      u8 devnum;           //地址码
      u8 funcode;          //功能码
      u8 datalen;          //返回的数据长度
      u16 type;            //机器类型（全部两字节）
      u16 temp;            //当前温度
      u16 setTemp;         //温度设定值
      u16 speed;           //当前速度
      u16 setSpeed;        //速度设定值
      u16 setTimeHour;     //小时设定值
      u16 setTimeMinute;   //分钟设定值
      u16 timeHour;        //当前小时值
      u16 timeMinute;      //当前分钟值
      u16 tempRangeMin;    //温度量程最小值
      u16 tempRangeMax;    //温度量程最大值
      u16 speedMin;        //速度最小值
      u16 speedMax;        //速度最大值
      u16 alarm;           //故障报警（使用0-9位）
      u16 status;          //运行状态（到此处一共15个数据，共30字节）
      s16 currentMode;     //当前模式（协议2.2添加的）
      u16 powerOffMem;     //掉电记忆（协议2.2添加的）
      u16 checkcode;             //校验码
    }slaveBuf;                   //从机缓冲 modbus
    u8 slavePtr[40];             //从机返回的数据
  } UN2;
} RS485_TYPE;
#pragma pack()
RS485_TYPE RS485A;


struct _MachinePara       //机器参数
{                   
  s16 machineType = 0;
  s16 paraTemp = 0;
  s16 setTemp = 50.0;     //缺省值为50
  s16 paraSpeed = 0;
  s16 setSpeed = 100;     //速度设定缺省值为100
  s16 setTimeHour = 0;
  s16 setTimeMinute = 0;
  s16 paraTimeHour = 0;
  s16 paraTimeMinute = 0;
  // s16 paraTempRangeMin = 0;
  // s16 paraTempRangeMax = 0;
  s16 setTempRangeMin = 0.0;
  s16 setTempRangeMax = 320.0;    //最大设定温度为320
  s16 paraSpeedMin = 0; 
  s16 paraSpeedMax = 0;
  s16 setSpeedMin = 100;     //最小设定速度缺省值为100
  s16 setSpeedMax = 1500;    //最大设定速度缺省值为1500
  u16 alarm;                 //报警值，位操作
  u16 paraRunStatus = 0;
  u16 paraCurrentMode = 0;     //（协议2.2添加）
  u16 paraPwrOffMem = 0;
  u16 setCurrentMode = 0;
  u16 setPwrOffMem = 0;
}MachinePara;



struct {
	time_t cpuTime;//CPU UNIX时间
	time_t startupTime;

	byte appPwrOnAlm;
	byte gsmPwrOnAlm;
	byte wifiPwrOnAlm;
	int mqttState;

	byte isMathExpress;
	byte dataTriged;
	byte rs485Triged;
	byte startSmartconfig;
	byte alarm;
	byte sosAlarm;
	byte paraRestore;
	byte alarmFile;
	byte yunUpload;
	byte emailTest;
	int messageReceiveID;
	byte isSetting;
	byte portMode;
	byte modbusErrCounter;
	byte cronCommandAct1;
	byte cronCommandAct2;
	byte cronCommandAct3;
	byte tempRemote;
	byte remoteSetReply;
	byte firstAlarmCount;
	byte offline;
	int controlDelayGSM;
	int controlDelayWIFI;

	byte wifisConnected;

	byte messageSend;
	byte gsmFirstMsg;
	byte syncTime;

	byte mqttIsConnectted;
	byte wifiNetIsOn;
	byte gsmStatus;
	byte simStatus;
	byte rssi;
	byte iccid;
	byte gsmrssi;

	bool gprsSocketStatus;
	String gsmImei;
	String gsmIccid;
	byte messageProc;
} RunningPara;


//wifi设备基础信息
typedef union _SavePara {
	struct {
		char jiami[30];
		byte reset;
		byte timeControlEn;//定时
		byte ntpEn;//网络授时
		byte uploadEn;
		byte emailAlarmEn;
		byte appAlarmEn;
		byte wxAlarmEn;
		byte ldctrlEn;//联动控制使能
		byte alarmEn; //报警总开关
		byte fileEn;  //数据文件记录
		byte gsmAlarmEn;
		byte callAlarmEn;
		byte controllerAlarmEn; //控制器内部报警
		byte relayStartUp;
		byte portMode;

		byte deviceID;
		byte wifiMode;//0STA 1AP 2STA+AP
		byte isDhcp;
		byte ip[4];
		byte dns[4];
		byte gateway[4];
		byte subnet[4];
		byte apip[4];

		int highTemp; //超温报警
		int lowTemp;  //低温报警
		int highHumi; //超湿
		int lowHumi;  //低湿
		int highspeed;//超速

		char dispName[DISP_LEN][20];
		char dispValue[DISP_LEN][30];

		char yuyincmds[YUYINLEN][10];
		char yuyinact[YUYINLEN][10];
		byte yuyindo[YUYINLEN];

		char stassid[20];
		char stapasswd[20];
		char apssid[20];
		char appasswd[20];
		char authname[20];
		char authpasswd[20];

		char emailAccount[30];
		char emailPasswd[20];
		char emailTo[5][30];
		byte emailHost;
		char emailsubject[20];

		char gsmTo[6][12];//短信接收
		char feeTemplate[30];//短信欠费模板

		int wlinterval;//上传间隔
		char customuptemp[120];
		char customupurl[70];
		char customupaddr[30];//CUSTOM上传
		char customupkey[45];

		int fileRecordInterval;
		char fileTemplate[10];
		int screenInterval;
		char screenTemplate[10];

		char openID[60];//微信OPENID
		char openID2[60];//微信OPENID
		char appid[60];//APP的UUID
		char usercode[10];
		char opencode[10];
		char nickname[20];
		char punycode[20];

		byte cronhour[CRONLEN];
		byte cronmin[CRONLEN];
		byte cronEn[CRONLEN];
		byte workday[CRONLEN];
		byte cronact[CRONLEN];

		char templAct[TEMPLLEN];
		char dataValue[TEMPLLEN][30];

		char concmds[CONLEN][50];
		byte conact[CONLEN];
		char iccid[30];
		char imei[30];
	} Para;
	byte bytes[100];
} SavePara;
SavePara mSavePara;


char emoji_1f60a[] = { 0xF0,0x9F,0x98,0x8A,0 };
char emoji_1f633[] = { 0xF0,0x9F,0x98,0xB3,0 };
char emoji_1f604[] = { 0xF0,0x9F,0x98,0x84,0 };
char emoji_1f600[] = { 0xF0,0x9F,0x98,0x80,0 };

int lastMsgID = -1;
int ledflash = 0;
String ackMessageStr = "";

void APPSendMessage(String content);
void set_device(String content);
int wulianUpload();
void check_alarm();
void smartConfig();