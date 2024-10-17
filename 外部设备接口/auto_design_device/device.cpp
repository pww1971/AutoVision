#ifdef WIN32
#include <Winsock2.h>
#endif
#include "device.h"


CDevice::CDevice()
{
	wechatIni();
}
CDevice::~CDevice()
{
	release();
}
void CDevice::release()
{
	
}

bool CDevice::autoDesign(std::vector<cv::Mat>& vm)
{
	m_mkc.updateRawPic(vm);
	if (1 == m_mkc.m_curProc.parallelMatchTplt(vm))
	{
		GDKT::CDateTime dt;
		string str = m_mainDir + dt.ToString("%Y-%m-%d-%H-%M-%S");
		pww_mkdir(str.c_str());
		if (1 == m_mkc.autoUpdate(str))
		{
			m_mkc.m_curProc.writeParam();
			m_mkc.m_curProc.clone(m_bkProc);
			return true;
		}
		m_mkc.m_curProc.clone(m_bkProc);
	}
	
	return false;
}
bool CDevice::searchObj(std::vector<cv::Mat>& vm)
{
	std::vector<string> vct;
	getFileNameDir(m_mainDir.c_str(), vct);
	CTemplateProc check;
	for (int i=0;i<vct.size();i++)
	{
		if (check.loadTplt(m_mainDir + vct[i] + PROCFILE))
		{
			if (1 == check.parallelMatchTplt(vm))
			{
				return true;
			}
		}
	}
	return false;
}
bool CDevice::ini(string modelcfg, string newModelDir)
{
	m_mkc.m_szDir = modelcfg;
	m_mkc.m_curProc.loadTplt(modelcfg);
	m_bkProc.clone(m_mkc.m_curProc);

	m_mainDir = getExePath() + newModelDir + "/";
	pww_mkdir(m_mainDir.c_str());

	return true;
}

bool CDevice::wechatIni()
{
	string szExePath = getExePath();
	string detect_prototxt = szExePath + "qrcode/detect.prototxt";//这个路径需要改
	string detect_caffe_model = szExePath + "qrcode/detect.caffemodel";
	string sr_prototxt = szExePath + "qrcode/sr.prototxt";
	string sr_caffe_model = szExePath + "qrcode/sr.caffemodel";
	try
	{
		m_wecharDecode = makePtr<wechat_qrcode::WeChatQRCode>(detect_prototxt, detect_caffe_model,
			sr_prototxt, sr_caffe_model);
	}
	catch (const std::exception& e)
	{
		//cout << e.what() << endl;" Could not initialize tesseract.";
		return false;
	}
	return true;
}

bool CDevice::checkObj(std::vector<cv::Mat>& vm)
{
	std::vector<Mat> points;
	auto decoded_info = m_wecharDecode->detectAndDecode(vm[0], points);
	CTemplateProc check;
	for (int i = 0; i < decoded_info.size(); i++)
	{
		if (check.loadTplt(m_mainDir + decoded_info[i] + PROCFILE))
		{
			if (1 == check.parallelMatchTplt(vm))
			{
				return true;
			}
		}
	}
	return false;
}

bool CDevice::wechatDesign(std::vector<cv::Mat>& vm)
{
	m_mkc.updateRawPic(vm);
	std::vector<Mat> points;
	auto decoded_info = m_wecharDecode->detectAndDecode(vm[0], points);
	
	for (int i = 0; i < decoded_info.size(); i++)
	{
		if (1 == m_mkc.m_curProc.parallelMatchTplt(vm))
		{
			string str = m_mainDir + decoded_info[i];
			pww_mkdir(str.c_str());
			if (1 == m_mkc.autoUpdate(str))
			{
				m_mkc.m_curProc.writeParam();
				m_mkc.m_curProc.clone(m_bkProc);
				return true;
			}
			m_mkc.m_curProc.clone(m_bkProc);
		}
	}
	
	return false;
}
