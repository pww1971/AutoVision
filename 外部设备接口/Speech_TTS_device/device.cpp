
#include "device.h"

static vector<BYTE> s_vctSoundData;
static BOOL     s_bRecurr = TRUE;

void CALLBACK callback(HWAVEIN   hwi,                              // 设备句柄
	UINT      uMsg,							   // 消息
	DWORD_PTR dwInstance,					   // 对象
	DWORD_PTR dwParam1,						   // 参数1
	DWORD_PTR dwParam2)						   // 参数2
{
	// 获取对象
//	USER* user2 = (USER*)dwInstance;
	// 获取音频头
	PWAVEHDR  pwhdr = (PWAVEHDR)dwParam1;

	// 处理消息
	switch (uMsg)
	{
	case WIM_OPEN:                                 // 打开录音设备

		//printf("成功打开设备..\n");
		break;

	case WIM_DATA:                                 // 缓冲区已满
	{
		//	printf("缓冲池已满..\n");
		//	printf("a:%d , b:%c \n", user2->a, user2->b);
			// 缓冲池信息
		DWORD buflen = pwhdr->dwBufferLength;
		DWORD bytrecd = pwhdr->dwBytesRecorded;

		if (bytrecd > 0)
		{
			// 缓冲扩增
			s_vctSoundData.resize(s_vctSoundData.size() + bytrecd * sizeof(BYTE));
			// 存储新内容
			memcpy(&s_vctSoundData[s_vctSoundData.size() - bytrecd], pwhdr->lpData, bytrecd);
			//	printf("已存储:%d byte\n", hasRecorded);

		}

		// 循环 	
		if (s_bRecurr)
		{
			// 加入缓存
			waveInAddBuffer(hwi, pwhdr, sizeof(WAVEHDR));
		}
	}
	break;

	case WIM_CLOSE:                               // 关闭录音设备
	{
		//printf("停止录音..\n");

	}
	break;

	default:
		break;
	}
}

CVADTreat::CVADTreat()
{
	m_bStop = false;
	m_runing = false;
	m_szDescription = "语音识别线程";
	//open();
}

CVADTreat::~CVADTreat(void)
{
	m_bStop = true;

	for (int t = 0; t < 500; t++)
	{
		if (m_runing)
			mysleep(10);
		else
			break;
	}
	stop();
}

//下层调用，传消息过来
int CVADTreat::svc()
{
	m_runing = true;

	HWAVEIN         hWaveIn;		        //输入设备
	HWAVEOUT        hWaveOut;		        //输出设备
	WAVEFORMATEX    waveform;	            //定义音频流格式
	BYTE* pBuffer1, * pBuffer2;				//输入音频缓冲区（左右声道）
	WAVEHDR         whdr_i1, whdr_i2;       //输入音频头
	WAVEHDR         whdr_o;                //输出音频头
//	USER* user = new USER();		//定义用户

	// 设备数量
	int count = waveInGetNumDevs();
//	printf("\n音频输入数量：%d\n", count);

	// 设备名称
	WAVEINCAPS waveIncaps;
	MMRESULT mmResult = waveInGetDevCaps(0, &waveIncaps, sizeof(WAVEINCAPS));//2
//	std::cout << "设备名称：" << waveIncaps.szPname << std::endl;

	// 设置音频流格式
	waveform.nSamplesPerSec = 16000;												// 采样率
	waveform.wBitsPerSample = 16;												// 采样精度
	waveform.nChannels = 1;                                                     // 声道个数
	waveform.cbSize = 0;														// 额外空间	
	waveform.wFormatTag = WAVE_FORMAT_PCM;										// 音频格式
	waveform.nBlockAlign = (waveform.wBitsPerSample * waveform.nChannels) / 8;  // 块对齐
	waveform.nAvgBytesPerSec = waveform.nBlockAlign * waveform.nSamplesPerSec;  // 传输速率

	//分配内存
	pBuffer1 = new BYTE[1024 * 10000];
	pBuffer2 = new BYTE[1024 * 10000];
	memset(pBuffer1, 0, 1024 * 10000);   // 内存置0
	memset(pBuffer2, 0, 1024 * 10000);   // 内存置0

	// 设置音频头
	whdr_i1.lpData = (LPSTR)pBuffer1; // 指向buffer
	whdr_i1.dwBufferLength = 1024 * 10000;     // buffer大小
	whdr_i1.dwBytesRecorded = 0;      // buffer存放大小
	whdr_i1.dwUser = 0;
	whdr_i1.dwFlags = 0;
	whdr_i1.dwLoops = 1;
	whdr_i2.lpData = (LPSTR)pBuffer1; // 指向buffer
	whdr_i2.dwBufferLength = 1024 * 10000;     // buffer大小
	whdr_i2.dwBytesRecorded = 0;      // buffer存放大小
	whdr_i2.dwUser = 0;
	whdr_i2.dwFlags = 0;
	whdr_i2.dwLoops = 1;


	string path = getExePath();
	Model* mm = create_model((path + "voicemodel/paddlespeech_cli").c_str(), 0);
	Audio audio(0);
	while (!m_bStop)
	{
		//cout << "begin: \r\n";
		s_bRecurr = TRUE;

		// 开启录音
		MMRESULT mRet = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)callback, NULL/*(DWORD_PTR)user*/, CALLBACK_FUNCTION);
		waveInPrepareHeader(hWaveIn, &whdr_i1, sizeof(WAVEHDR));//准备buffer
		waveInPrepareHeader(hWaveIn, &whdr_i2, sizeof(WAVEHDR));//准备buffer
		waveInAddBuffer(hWaveIn, &whdr_i1, sizeof(WAVEHDR));    //添加buffer
		waveInAddBuffer(hWaveIn, &whdr_i2, sizeof(WAVEHDR));    //添加buffer

		waveInStart(hWaveIn);
		Sleep(1000);
		s_bRecurr = FALSE;

		audio.load((char*)s_vctSoundData.data(), s_vctSoundData.size());
		//audio.disp();
		s_vctSoundData.clear();

		audio.split();

		float* buff;
		int len;
		int flag;
		string text_heard = "";
		while (audio.fetch(buff, len, flag) > 0) {
			mm->reset();
			text_heard += mm->forward(buff, len, flag);
		}
		if ("" == text_heard)
		{
			mysleep(100);
		}
		else if(treat(text_heard))
			;
		//waveInStop(hWaveIn);
		waveInReset(hWaveIn);
	}
	waveInClose(hWaveIn);
	delete mm;

	m_runing = false;
	return 0;
}

bool CVADTreat::treat(string& text_heard)
{
	text_heard = CHASCII(text_heard);
	// remove text between brackets using regex
	{
		std::regex re("\\[.*?\\]");
		text_heard = std::regex_replace(text_heard, re, "");
	}

	// remove all characters, except for letters, numbers, punctuation and ':', '\'', '-', ' '
//	text_heard = std::regex_replace(text_heard, std::regex("[^a-zA-Z0-9\\.,\\?!\\s\\:\\'\\-]"), "");

	// take first line
	text_heard = text_heard.substr(0, text_heard.find_first_of('\n'));

	// remove leading and trailing whitespace
	text_heard = std::regex_replace(text_heard, std::regex("^\\s+"), "");
	text_heard = std::regex_replace(text_heard, std::regex("\\s+$"), "");

	if (text_heard.empty() ) 
		return false;
	
	return true;
}


CDevice::CDevice()
{
	m_procAddress = "";
	m_cfgAdd = "";
	pSpVoice = NULL;
	m_iState = -1;
	m_mb = NULL;
	m_timeout = 60000;//60秒超时
	m_time_start2 = m_time_start = clock();
	m_szMyName = "小小";
	m_bMsg = false;
}
CDevice::~CDevice()
{
	if (pSpVoice != NULL)
	{
		((ISpVoice*)pSpVoice)->Speak(L"退出语音播报", SPF_PURGEBEFORESPEAK | SPF_DEFAULT, NULL);
		((ISpVoice*)pSpVoice)->Release();
		pSpVoice = NULL;
	}
}
void CDevice::loadcfg()
{
	if ("" != m_cfgAdd)
	{
		CConfig cfg(m_cfgAdd);
		string szVal = cfg.getItem("procAddress");
		if ("" != szVal)
		{
			m_procAddress = szVal;
		}
		if ("" != m_procAddress)//目录中自动找到极坐标定位的配置文件和模板图片 并能加载成功 
		{
		}
	}
	if (pSpVoice == NULL)
	{
		if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pSpVoice)))
		{
			AfxMessageBox("不能初始化语音");
		}
		else {
			Speech("语音播报模块加载成功");
		}
	}
}
void CDevice::writecfg()
{
	if ("" != m_cfgAdd)
	{
		remove(m_cfgAdd.c_str());
		CConfig cfg(m_cfgAdd);
		cfg.updateItem("\n#制程地址", "");
		cfg.updateItem("reportAddress", m_procAddress);
	}
}

void CDevice::Speech(CString str, bool clear) 
{
	if (pSpVoice != NULL) {
		if (clear) {
			((ISpVoice*)pSpVoice)->Speak(str.AllocSysString(), SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
		}
		else {
			((ISpVoice*)pSpVoice)->Speak(str.AllocSysString(), SPF_ASYNC | SPF_DEFAULT, NULL);     // 朗读中文和英文的混合字符串   
		}
	}
}
void CDevice::speechPause()
{
	if (2000 < clock() - m_time_start2)
	{
		Speech("静止状态");
		m_time_start2 = clock();
	}
}
bool CDevice::treat(string& text_heard)
{
	if (!CVADTreat::treat(text_heard))
		return false;
	if (-1 == m_iState)
	{
		if (-1 != text_heard.find(m_szMyName))
		{
			m_iState = 0;
			m_time_start = clock();
			Speech("收到 请指示");
		}
		if (m_mb->m_iPause)
			speechPause();
		return true;
	}
	else if (m_timeout < clock() - m_time_start)
	{
		m_iState = -1;
		Speech("呼叫指示超时");
		return true;
	}

	if (m_bMsg)
	{
		string str = m_mb->findMsg(text_heard);//查询短消息
		if ("" != str)
		{
			Speech("收到");
			m_mb->pushMsg(str);
			m_time_start = clock();
			return true;
		}
	}
	if (-1 != text_heard.find("暂停"))
	{
		Speech("收到");
		m_time_start = clock();
		if (!m_mb->m_iPause)
			m_mb->MvPause();
	}
	else if (-1 != text_heard.find("继续"))
	{
		Speech("收到");
		m_time_start = clock();
		if (m_mb->m_iPause)
			m_mb->MvPause();
	}
	else if (m_mb->m_iPause)
	{
		speechPause();
		return true;
	}
	else if (-1 != text_heard.find("启动") || -1 != text_heard.find("运行"))
	{
		Speech("收到");
		if (m_mb->m_iState != -1)//急停状态
		{
			Speech("紧急状态！");
			return true;
		}
		m_mb->m_io.ioTrigPause(false);
		string command = m_mb->m_szChainMv;
		if (command != "")
		{
			m_mb->updateProb();
			m_mb->pushMsg(command);
			m_time_start = clock();
		}
	}
	else if (-1 != text_heard.find("停"))
	{
		Speech("收到");
		m_mb->Mvstop();
		m_time_start = clock();
	}
	else
	{
	}

	return true;
}
