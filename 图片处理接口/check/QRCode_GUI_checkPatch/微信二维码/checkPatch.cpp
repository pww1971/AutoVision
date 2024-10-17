#include "checkPatch.h"
#include <regex>


checkPatch::checkPatch()
{
	detector = NULL;
}

checkPatch::~checkPatch()
{
	detector = NULL;
}
void checkPatch::readMode(string file)
{
	CConfig cfg(file + "/qrcode.cfg");
	m_szRule = cfg.getItem("m_szRule");
}
void checkPatch::writeMode(string file)
{
	pww_mkdir(file.c_str());
	CConfig cfg(file + "/qrcode.cfg");
	cfg.updateItem("m_szRule", m_szRule);
}
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid)
{
	cv::Mat& src = vctData[curid]->m_pic;
	if (!src.data)
		return false;
	if (pa.m_dllPa.ini < 1)
	{
		string szExePath = getExePath();
		string detect_prototxt = szExePath + "qrcode/detect.prototxt";//这个路径需要改
		string detect_caffe_model = szExePath + "qrcode/detect.caffemodel";
		string sr_prototxt = szExePath + "qrcode/sr.prototxt";
		string sr_caffe_model = szExePath + "qrcode/sr.caffemodel";
		try
		{
			detector = makePtr<wechat_qrcode::WeChatQRCode>(detect_prototxt, detect_caffe_model,
				sr_prototxt, sr_caffe_model);
		}
		catch (const std::exception& e)
		{
			//cout << e.what() << endl;
			vctData[curid]->m_szRet = " Could not initialize tesseract.";
			return false;
		}
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.ini == 0)
		{
			;
		}
		else if (pa.m_dllPa.ini == -1)
		{
		/*	CPaddleOcrParamDlg S_dlg;
			S_dlg.m_pCheck = this;
			S_dlg.m_pic = src;
			INT_PTR nResponse = S_dlg.DoModal();
			if (nResponse == IDOK)
			{
				writeMode(pa.m_dllPa.szfile);
			}
			else// if (nResponse == IDCANCEL)
			{
				return false;
			}*/
		}
		pa.m_dllPa.ini = 1;
		return true;
	}
	else
	{
		vector<Mat> points;
		auto decoded_info = detector->detectAndDecode(src, points);
		if (decoded_info.size() > 0)
		{
			if (pa.m_inputRoi == 0)//定位
			{
				int len = decoded_info.size();
				for (int pi = vctData.size(); pi < len; pi++)
					vctData.push_back(new S_TpltData());
				char sz[128];
				for (int a = 0; a < len; a++)
				{
					Point pt1 = points[a].at<Point2f>(0);
					Point pt2 = points[a].at<Point2f>(2);
					Rect rect{ pt1, pt2 };
					vctData[a]->m_roiRect.angle = cv::fastAtan2(pt2.y - pt1.y, pt2.x - pt1.x);
					sprintf(sz, " 相似度=%f,旋转角=%.4f ", 1, vctData[a]->m_roiRect.angle);
					vctData[a]->m_szRet = sz;//decoded_info[a];
					vctData[a]->m_roiRect.center = (pt1 + pt2) / 2;
					vctData[a]->m_roiRect.size = rect.size();
					cv::Rect rt = vctData[a]->m_roiRect.boundingRect();
					rt &= cv::Rect(0, 0, src.cols, src.rows);
					vctData[a]->m_pic = cv::Mat(src, rt).clone();
					vctData[a]->m_checkRet = true;
				}
			}
			//for (int i = 0; i < len; i++)
			//{
			//	//cout << "decode-" << i + 1 << ": " << decoded_info[i] << endl;
			//	Point pt1 = Point((int)points[i].at<float>(0, 0), (int)points[i].at<float>(0, 1));
			//	Point pt2 = Point((int)points[i].at<float>(1, 0), (int)points[i].at<float>(1, 1));
			//	Point pt3 = Point((int)points[i].at<float>(2, 0), (int)points[i].at<float>(2, 1));
			//	Point pt4 = Point((int)points[i].at<float>(3, 0), (int)points[i].at<float>(3, 1));
			//	line(src, pt1, pt2, Scalar(0, 255, 0), 2);
			//	line(src, pt2, pt3, Scalar(0, 255, 0), 2);
			//	line(src, pt3, pt4, Scalar(0, 255, 0), 2);
			//	line(src, pt4, pt1, Scalar(0, 255, 0), 2);
			//	putText(src, decoded_info[i], pt1, 0, 0.5, Scalar(255, 0, 0), 2);
			//}
			else
			{
				vctData[curid]->m_szRet = decoded_info[0];
				vctData[curid]->m_szRet = CStrProcess::Trim(vctData[curid]->m_szRet, " \t\f\v\n\r");
				//配置文件的匹配规则做匹配
				if (vctData[curid]->m_szRet != "")
				{
					if ("" != m_szRule)
					{
						regex r(m_szRule);
						if (std::regex_match(vctData[curid]->m_szRet, r)) //regex_search "(C562415)(\\dddd)"  头文字C562415和末尾四个数字
						{
							vctData[curid]->m_checkRet = true;
							return true;
						}
						else
							return false;
					}
					vctData[curid]->m_checkRet = true;
					return true;
				}
			}
		}
		return false;
	}

	return false;
}

//画出ng的标识
bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	return true;
}

void checkPatch::writeFile( CTpltParam& pa)
{

}
