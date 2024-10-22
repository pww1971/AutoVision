#include "stdafx.h"
#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;


bool ST_MYDISP::Write_Image(HalconCpp::HImage Image, CString PathName)
{
	
	// ����ͼ��
	try {
		int nPos;
		CString ImageType;
		CString stPath;


		nPos = PathName.ReverseFind('.');
		if (nPos > -1) {
			ImageType = PathName.Right(PathName.GetLength() - nPos - 1);
		}
		else {
			ImageType = "bmp";
		}

		nPos = PathName.ReverseFind('\\');
		if (nPos > -1) {
			stPath = PathName.Left(nPos);
		}
		else {
			stPath = UCI::strMoudlePath;
		}

		// ����·��
		if (!PathIsDirectory(stPath)) {
			SHCreateDirectoryEx(NULL, stPath, NULL);
		}

		Image.WriteImage(CT2A(ImageType).m_psz, 0, CT2A(PathName).m_psz);
	
		return true;
	}
	catch (HalconCpp::HException& except) {
//		UCI::sDB.Log(except, "ST_MYDISP::Write_Image ����ͼ��");
		return false;
	}

	return true;
}








bool ST_MYDISP::GetCameraPoseFromPictre()
{
	return false;
}



