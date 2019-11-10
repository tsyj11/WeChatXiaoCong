void MasterReceProc(u8 devnum)
{
  #pragma pack()
    if (RS485A.UN2.slaveBuf.funcode == 0x03)      //如果从机返回数据中功能码是03
    {
      MachinePara.machineType          = HTONS(RS4852.UN2.slaveBuf.type);         
      MachinePara.paraTemp             = HTONS(RS4852.UN2.slaveBuf.temp);         
      MachinePara.setTemp              = HTONS(RS4852.UN2.slaveBuf.setTemp);      
      MachinePara.paraSpeed            = HTONS(RS4852.UN2.slaveBuf.speed);        
      MachinePara.setSpeed             = HTONS(RS4852.UN2.slaveBuf.setSpeed);      
      MachinePara.setTimeHour          = HTONS(RS4852.UN2.slaveBuf.setTimeHour);   
      MachinePara.setTimeMinute        = HTONS(RS4852.UN2.slaveBuf.setTimeMinute); 
      MachinePara.paraTimeHour         = HTONS(RS4852.UN2.slaveBuf.timeHour);      
      MachinePara.paraTimeMinute       = HTONS(RS4852.UN2.slaveBuf.timeMinute);   
      MachinePara.paraTempRangeMin     = HTONS(RS4852.UN2.slaveBuf.tempRangeMin);  //温度量程最小值
      MachinePara.paraTempRangeMax     = HTONS(RS4852.UN2.slaveBuf.tempRangeMax);  //温度量程最大值
      MachinePara.paraSpeedMin         = HTONS(RS4852.UN2.slaveBuf.speedMin);      //速度最小值
      MachinePara.paraSpeedMax         = HTONS(RS4852.UN2.slaveBuf.speedMax);      //速度最大值
      MachinePara.alarm                = HTONS(RS4852.UN2.slaveBuf.alarm);         //内部传感器
      MachinePara.paraRunStatus        = HTONS(RS4852.UN2.slaveBuf.status);        //运行状态
      MachinePara.paraCurrentMode      = HTONS(RS4852.UN2.slaveBuf.currentMode);   //当前模式
      MachinePara.paraPwrOffMem        = HTONS(RS4852.UN2.slaveBuf.powerOffMem);   //掉电记忆
      // MachinePara.setTempRangeMin      = RS4852.UN2.slaveBuf.;
      // MachinePara.setTempRangeMax      = HTONS(RS4852.UN2.slaveBuf.);
      // MachinePara.setSpeedMin          = HTONS(RS4852.UN2.slaveBuf.setHumidity);
      // MachinePara.setSpeedMax          = HTONS(RS4852.UN2.slaveBuf.setTemp);
      // MachinePara.speedSetMax          = HTONS(RS4852.UN2.slaveBuf.speedMax);
      // MachinePara.illuminationMax      = RS4852.UN2.slaveBuf.illuminationRange;
      // MachinePara.concentrationMax     = HTONS(RS4852.UN2.slaveBuf.concentrationRange);
      // MachinePara.setBean              = HTONS(RS4852.UN2.slaveBuf.been);

    } 
    else if (RS4852.UN2.slaveBuf.funcode == 0x10)       //如果从机返回来的数据中功能码是10
    {
    
    } 
    
