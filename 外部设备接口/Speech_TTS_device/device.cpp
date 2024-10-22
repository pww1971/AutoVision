
#include "device.h"

static vector<BYTE> s_vctSoundData;
static BOOL     s_bRecurr = TRUE;

void CALLBACK callback(HWAVEIN   hwi,                              // �豸���
	UINT      uMsg,							   // ��Ϣ
	DWORD_PTR dwInstance,					   // ����
	DWORD_PTR dwParam1,						   // ����1
	DWORD_PTR dwParam2)						   // ����2
{
	// ��ȡ����
//	USER* user2 = (USER*)dwInstance;
	// ��ȡ��Ƶͷ
	PWAVEHDR  pwhdr = (PWAVEHDR)dwParam1;

	// ������Ϣ
	switch (uMsg)
	{
	case WIM_OPEN:                                 // ��¼���豸

		//printf("�ɹ����豸..\n");
		break;

	case WIM_DATA:                                 // ����������
	{
		//	printf("���������..\n");
		//	printf("a:%d , b:%c \n", user2->a, user2->b);
			// �������Ϣ
		DWORD buflen = pwhdr->dwBufferLength;
		DWORD bytrecd = pwhdr->dwBytesRecorded;

		if (bytrecd > 0)
		{
			// ��������
			s_vctSoundData.resize(s_vctSoundData.size() + bytrecd * sizeof(BYTE));
			// �洢������
			memcpy(&s_vctSoundData[s_vctSoundData.size() - bytrecd], pwhdr->lpData, bytrecd);
			//	printf("�Ѵ洢:%d byte\n", hasRecorded);

		}

		// ѭ�� 	
		if (s_bRecurr)
		{
			// ���뻺��
			waveInAddBuffer(hwi, pwhdr, sizeof(WAVEHDR));
		}
	}
	break;

	case WIM_CLOSE:                               // �ر�¼���豸
	{
		//printf("ֹͣ¼��..\n");

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
	m_szDescription = "����ʶ���߳�";
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

//�²���ã�����Ϣ����
int CVADTreat::svc()
{
	m_runing = true;

	HWAVEIN         hWaveIn;		        //�����豸
	HWAVEOUT        hWaveOut;		        //����豸
	WAVEFORMATEX    waveform;	            //������Ƶ����ʽ
	BYTE* pBuffer1, * pBuffer2;				//������Ƶ������������������
	WAVEHDR         whdr_i1, whdr_i2;       //������Ƶͷ
	WAVEHDR         whdr_o;                //�����Ƶͷ
//	USER* user = new USER();		//�����û�

	// �豸����
	int count = waveInGetNumDevs();
//	printf("\n��Ƶ����������%d\n", count);

	// �豸����
	WAVEINCAPS waveIncaps;
	MMRESULT mmResult = waveInGetDevCaps(0, &waveIncaps, sizeof(WAVEINCAPS));//2
//	std::cout << "�豸���ƣ�" << waveIncaps.szPname << std::endl;

	// ������Ƶ����ʽ
	waveform.nSamplesPerSec = 16000;												// ������
	waveform.wBitsPerSample = 16;												// ��������
	waveform.nChannels = 1;                                                     // ��������
	waveform.cbSize = 0;														// ����ռ�	
	waveform.wFormatTag = WAVE_FORMAT_PCM;										// ��Ƶ��ʽ
	waveform.nBlockAlign = (waveform.wBitsPerSample * waveform.nChannels) / 8;  // �����
	waveform.nAvgBytesPerSec = waveform.nBlockAlign * waveform.nSamplesPerSec;  // ��������

	//�����ڴ�
	pBuffer1 = new BYTE[1024 * 10000];
	pBuffer2 = new BYTE[1024 * 10000];
	memset(pBuffer1, 0, 1024 * 10000);   // �ڴ���0
	memset(pBuffer2, 0, 1024 * 10000);   // �ڴ���0

	// ������Ƶͷ
	whdr_i1.lpData = (LPSTR)pBuffer1; // ָ��buffer
	whdr_i1.dwBufferLength = 1024 * 10000;     // buffer��С
	whdr_i1.dwBytesRecorded = 0;      // buffer��Ŵ�С
	whdr_i1.dwUser = 0;
	whdr_i1.dwFlags = 0;
	whdr_i1.dwLoops = 1;
	whdr_i2.lpData = (LPSTR)pBuffer1; // ָ��buffer
	whdr_i2.dwBufferLength = 1024 * 10000;     // buffer��С
	whdr_i2.dwBytesRecorded = 0;      // buffer��Ŵ�С
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

		// ����¼��
		MMRESULT mRet = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)callback, NULL/*(DWORD_PTR)user*/, CALLBACK_FUNCTION);
		waveInPrepareHeader(hWaveIn, &whdr_i1, sizeof(WAVEHDR));//׼��buffer
		waveInPrepareHeader(hWaveIn, &whdr_i2, sizeof(WAVEHDR));//׼��buffer
		waveInAddBuffer(hWaveIn, &whdr_i1, sizeof(WAVEHDR));    //���buffer
		waveInAddBuffer(hWaveIn, &whdr_i2, sizeof(WAVEHDR));    //���buffer

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
	m_timeout = 60000;//60�볬ʱ
	m_time_start2 = m_time_start = clock();
	m_szMyName = "СС";
	m_bMsg = false;
}
CDevice::~CDevice()
{
	if (pSpVoice != NULL)
	{
		((ISpVoice*)pSpVoice)->Speak(L"�˳���������", SPF_PURGEBEFORESPEAK | SPF_DEFAULT, NULL);
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
		if ("" != m_procAddress)//Ŀ¼���Զ��ҵ������궨λ�������ļ���ģ��ͼƬ ���ܼ��سɹ� 
		{
		}
	}
	if (pSpVoice == NULL)
	{
		if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pSpVoice)))
		{
			AfxMessageBox("���ܳ�ʼ������");
		}
		else {
			Speech("��������ģ����سɹ�");
		}
	}
}
void CDevice::writecfg()
{
	if ("" != m_cfgAdd)
	{
		remove(m_cfgAdd.c_str());
		CConfig cfg(m_cfgAdd);
		cfg.updateItem("\n#�Ƴ̵�ַ", "");
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
			((ISpVoice*)pSpVoice)->Speak(str.AllocSysString(), SPF_ASYNC | SPF_DEFAULT, NULL);     // �ʶ����ĺ�Ӣ�ĵĻ���ַ���   
		}
	}
}
void CDevice::speechPause()
{
	if (2000 < clock() - m_time_start2)
	{
		Speech("��ֹ״̬");
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
			Speech("�յ� ��ָʾ");
		}
		if (m_mb->m_iPause)
			speechPause();
		return true;
	}
	else if (m_timeout < clock() - m_time_start)
	{
		m_iState = -1;
		Speech("����ָʾ��ʱ");
		return true;
	}

	if (m_bMsg)
	{
		string str = m_mb->findMsg(text_heard);//��ѯ����Ϣ
		if ("" != str)
		{
			Speech("�յ�");
			m_mb->pushMsg(str);
			m_time_start = clock();
			return true;
		}
	}
	if (-1 != text_heard.find("��ͣ"))
	{
		Speech("�յ�");
		m_time_start = clock();
		if (!m_mb->m_iPause)
			m_mb->MvPause();
	}
	else if (-1 != text_heard.find("����"))
	{
		Speech("�յ�");
		m_time_start = clock();
		if (m_mb->m_iPause)
			m_mb->MvPause();
	}
	else if (m_mb->m_iPause)
	{
		speechPause();
		return true;
	}
	else if (-1 != text_heard.find("����") || -1 != text_heard.find("����"))
	{
		Speech("�յ�");
		if (m_mb->m_iState != -1)//��ͣ״̬
		{
			Speech("����״̬��");
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
	else if (-1 != text_heard.find("ͣ"))
	{
		Speech("�յ�");
		m_mb->Mvstop();
		m_time_start = clock();
	}
	else
	{
	}

	return true;
}
