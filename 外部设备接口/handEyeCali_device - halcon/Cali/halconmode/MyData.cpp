#include "stdafx.h"
#include <algorithm>
#include <sstream>

#include "MyData.h"
#include "MyDispLay.h"
#include "afxdialogex.h"
#include "checkPatch.h"
using namespace HalconCpp;

using namespace std;
using std::string;




namespace UCI {

	// ȫ�ֱ���
	OptionsMap OPTIONS;                          // Global object


	ST_MYDISP* pDis;                         // ��ʾ�����ñ���;
	CString strMoudlePath;                   // AppPath ��װĿ¼


	CString stPictureSavePath;               // ͼ�񱣴��Ŀ¼

	RIBBON_MENU RIBmenu;                     // RIBBON �˵���ť
	stElement ELE;                           // 

	
	

	void initFirst() {

		TCHAR modulePath[MAX_PATH];
		GetModuleFileName(NULL, modulePath, MAX_PATH);
		CString _strMoudlePath(modulePath);
		_strMoudlePath = _strMoudlePath.Left(_strMoudlePath.ReverseFind('\\'));
		strMoudlePath = _strMoudlePath;

		stPictureSavePath = strMoudlePath + "\\" + "Picture Save";		        // ͼ�񱣴�Ŀ¼
		_mkdir(stPictureSavePath.GetBuffer(0));
		UCI::initOptionsMap(OPTIONS);

	}


	CString Utf8ToTChar(const std::string & utf8)
	{
		// ----------------------------------------
		// Convert UTF-8 to Unicode(UTF-16).
		// UTF-8��Unicode(UTF-16)�ˉ�Q����
		// ----------------------------------------
		// First, get the size of the buffer to store Unicode chars.
		// �ޤ���Unicode�����Ф��{����Хåե���������ȡ�ä���
		INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
		WCHAR* utf16 = new WCHAR[bufsize];

		// Then, convert UTF-8 to Unicode.
		// UTF-8��Unicode�ˉ�Q����
		::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, utf16, bufsize);

		// ----------------------------------------
		// Convert Unicode(UTF-16) to TCHAR
		// Unicode(UTF-16)��TCHAR�ˉ�Q����
		// ----------------------------------------
		// Convert Unicode(WCHAR) to TCHAR(MBCS or Unicode).
		// WCHAR����CStringW�ޤ���CStringA�ˉ�Q����(�ץ������Ȥ��O���ˤ��)

		//int len = wcslen(utf16);

		CString ret(utf16, int(::wcslen(utf16)));
		delete[] utf16;

		return ret;
	}

	std::string TCharToUtf8(const CString& tchar)
	{
		// ----------------------------------------
		// Convert TCHAR to Unicode(UTF-16).
		// TCHAR��Unicode(UTF-16)�ˉ�Q����
		// ----------------------------------------
		// Convert TCHAR to Unicode.
		// TCHAR��Uniocode�ˉ�Q����
		CStringW utf16(tchar);

		// ----------------------------------------
		// Convert Unicode(UTF-16) to UTF-8.
		// Unicode(UTF-16)��UTF-8�ˉ�Q����
		// ----------------------------------------
		// First, get the size of the buffer to store UTF-8 chars.
		// �ޤ���UTF-8�����Ф��{����Хåե���������ȡ�ä���
		INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, NULL, NULL);
		CHAR* utf8 = new CHAR[bufsize];

		// Then, convert Unicode to UTF-8.
		// �Τ�UTF-8��Unicode�ˉ�Q����
		::WideCharToMultiByte(CP_UTF8, 0, utf16, -1, utf8, bufsize, NULL, NULL);

		// At the end, create the std::string that contains the UTF-8 chars.
		// std::string�ˤ���Ʒ���
		std::string ret(utf8, ::strlen(utf8));
		delete[] utf8;

		return ret;
	}

	// ����ĸ���ʼԪ��
	void InitEmptyProject(int curProject)
	{
		UCI::ELE.add_One_Element(curProject, ELEMENT_CAMERA);
		UCI::ELE.add_One_Element(curProject, ELEMENT_ROI);
		UCI::ELE.add_One_Element(curProject, ELEMENT_FUNCTION);
		UCI::ELE.add_One_Element(curProject, ELEMENT_MATCH);
	}


	//                              //Ŀ��·��            ��
	int FindSpecialFile(CString target_file,
		CString stExt, CArray<CString, CString&> *caPathFile, bool isDel)
	{
		int found_count = 0; //���Ҽ�����  

		target_file += "\\" + stExt;

		CFileFind find;
		BOOL ret = find.FindFile(target_file); //�����ļ�  
		while (ret)
		{
			ret = find.FindNextFile();
			if (find.IsDots()) //. �� .. �ļ�  
			{
			}
			else if (find.IsDirectory()) //��Ŀ¼  
			{
			}
			else //file  
			{
				found_count++; //������������  

				CString stName = find.GetFilePath();
				CFile   TempFile;

				if (caPathFile != NULL) {
					caPathFile->Add(stName);
				}

				if (isDel) {
					TempFile.Remove(stName);
				}
			}
		}

		find.Close(); //�ر�����  

		return(found_count); //������������Ŀ  
	}
	
	/// Option class constructors and conversion operators

	Option::Option(CString sname, const CString v, const CString note, bool show, OnChange f) : type("string"), imin(0), imax(0), on_change(f)
	{
		name = sname;
		defaultValue = currentValue = v;
		explain = note;
		bshow = show;
	}


	Option::Option(OnChange f) : type("button"), imin(0), imax(0), on_change(f)
	{}

	Option::Option(CString sname, int v, int minv, int maxv, const CString note, bool show, OnChange f) : type("spin"), imin(minv), imax(maxv), on_change(f)
	{
		name = sname;

		CString strv;
		strv.Format("%d", v);

		defaultValue = currentValue = strv;
		explain = note;
		bshow = show;
	}

	Option::Option(CString sname, double v, double dminv, double dmaxv, const CString note, bool show, OnChange f) : type("double"), dmin(dminv), dmax(dmaxv), on_change(f)
	{
		name = sname;

		CString strv;
		strv.Format("%f", v);

		defaultValue = currentValue = strv;
		explain = note;
		bshow = show;
	}
	


	Option::operator int() const {
		assert(type == "spin");		

		return (type == "spin" ? _ttoi(currentValue) : currentValue == "true");
	}

	Option::operator double() const
	{
		assert(type == "double");
		return _ttof(currentValue);
	}

	Option::operator CString() const {
		assert(type == "string");
		return currentValue;
	}

	Option::operator HalconCpp::HTuple() const
	{
		assert(type == "string");

		HalconCpp::HTuple hv;


		int prev_pos = 0; 
		int pos = 0;

		hv.Clear();

		int a = currentValue.Find(" ");

		while ((pos = currentValue.Find(" ", pos))!= -1) {
			CString substring = currentValue.Mid(prev_pos, pos - prev_pos);
			double d = _ttof(substring);
			hv.Append(d);
			prev_pos = ++pos;
		}	
		
		return hv;
	}

	/// operator<<() inits options and assigns idx in the correct printing order

	void Option::operator<<(const Option& o) {
		*this = o;
		idx = insert_order++;
	}

	/// operator=() updates currentValue and triggers on_change() action. It's up to
	/// the GUI to check for option's limits, but we could receive the new value from
	/// the user by console window, so let's check the bounds anyway.

	Option& Option::operator=(const CString& v) {

		assert(!type.IsEmpty());	

		if ((type == "spin" && (_ttoi(v) < imin || _ttoi(v) > imax))
			|| (type == "double" && (_ttof(v) < dmin || _ttof(v) > dmax)))
			return *this;

		currentValue = v;

		if (on_change)
			on_change(*this);

		return *this;
	}

	Option& Option::operator=(const int& v) {
		assert(!type.IsEmpty());

		CString str;
		str.Format("%d", v);

		currentValue = str;

		if (on_change)
			on_change(*this);

		return *this;
	}

	Option& Option::operator=(const double& v) {
		assert(!type.IsEmpty());

		CString str;
		str.Format("%f", v);

		currentValue = str;

		if (on_change)
			on_change(*this);

		return *this;
	}

	Option & Option::operator=(const HalconCpp::HTuple & hv)
	{
		assert(!type.IsEmpty());

		currentValue.Empty();	

		for (int i = 0; i < hv.Length(); i++) {
			double d = hv[i].D();

			CString str;
			str.Format("%f", d);

			currentValue += (str + " ");
		}

		if (on_change)
			on_change(*this);

		return *this;
	}




	void GetHTupleFromString(HalconCpp::HTuple &t, std::string &s)
	{
		std::string::size_type prev_pos = 0, pos = 0;

		t.Clear();

		while ((pos = s.find(' ', pos)) != std::string::npos)
		{
			std::string substring(s.substr(prev_pos, pos - prev_pos));

			double d = stof(substring);
			t.Append(d);
			prev_pos = ++pos;
		}
	}


	inline RIBBON_MENU::RIBBON_MENU() {
		this->bIsInROIdrawing = false;

		this->iCOMBO_MATCH_ROI_TYPE = ROI_UNITE;  // ����
	}





	OptionsMap * stElement::add_One_Element(int iProject, eELEMENT_TYPE t)
	{
		OptionsMap* o = initOptionMapElemnet(t, iProject);
		return o;
	}


	int stElement::UpdateProjectByList()
	{
		return checkPatch::S_dlg.UpdataProjectTreeByList();
	}

	






}





bool ST_MYDISP::SaveImgToFile(CString stSavePath, 
	CString stFormat, CString & stFileName)
{
	
	if (!PathIsDirectory(stSavePath)) {		
		SHCreateDirectoryEx(NULL, stSavePath, NULL);
	}
	
	stFileName = stSavePath + "\\";
	CTime t = CTime::GetCurrentTime();
	CString s = t.Format("%Y%m%d%H%M%S");
	stFileName += s + "." + stFormat;   // ��png��ʽ����ͼ��

	//HObject ho_new;

	//// ���߳�ʱҪ����һ��
	HImage GrabImage;
//	EnterCriticalSection(&m_csFGDisp);      // CriticalSect
	GrabImage = this->GrabImageDisp;
//	LeaveCriticalSection(&m_csFGDisp);      // CriticalSect
	
	try {
		GrabImage.WriteImage(CT2A(stFormat).m_psz, 0,
			CT2A(stFileName).m_psz);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}

	return true;
}

bool ST_MYDISP::SaveImgToROI()
{
	try {
		int prid = UCI::OPTIONS["��Ŀ���"];
		CString str = UCI::strMoudlePath;

		CString format = UCI::OPTIONS["���񱣴��ʽ"];

		//str = str + "\\project\\" + to_string(prid) + "\\ROI\\roimatch." + format;
		//return str;

		CString stDeviceName = UCI::OPTIONS["�������"];

		CString strTmp;
		strTmp.Format("%d", prid);

		CString stSavePath = str + "\\project\\" + strTmp
			+ "\\ROI\\" + stDeviceName;


		if (!PathIsDirectory(stSavePath)) {
			SHCreateDirectoryEx(NULL, stSavePath, NULL);
		}

		CString stFileName = stSavePath + "\\roimatch."
			+ format;

		//HImage HImage;
		//EnterCriticalSection(&m_csIPDisp);      // CriticalSect
		//GrabImage = this->GrabImageDisp;
		//HImage = this->GrabImageDisp;               // ������map����ͼ��
		//LeaveCriticalSection(&m_csIPDisp);      // CriticalSect


		// ����ԭͼ
		this->GrabImageDisp.WriteImage(CT2A(format).m_psz, 0,
			CT2A(stFileName).m_psz);

		AfxMessageBox("����ͼƬ�ɹ��ˣ�" + stFileName);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox("ST_MYDISP::SaveImgToROI()��");

		return false;
	}

	return true;
}

bool ST_MYDISP::ReadImageFromFile(CString stFileName)
{
	CT2A asc(stFileName);
	try {
		HTuple hv_ImageWidth, hv_ImageHeight;

	//	EnterCriticalSection(&m_csFGDisp);      // CriticalSect
		this->GrabImageDisp.ReadImage(asc.m_psz);
		this->GrabImageDisp.GetImageSize(&hv_ImageWidth, &hv_ImageHeight);
//		LeaveCriticalSection(&m_csFGDisp);      // CriticalSect

		
//		EnterCriticalSection(&this->m_csIPDisp);
		this->m_sIPResult.result_img = this->GrabImageDisp;
//		LeaveCriticalSection(&this->m_csIPDisp);

		int iHeight = hv_ImageHeight.I();
		int iWidth = hv_ImageWidth.I();

		int oldW = UCI::OPTIONS["ͼ����"];
		int oldH = UCI::OPTIONS["ͼ��߶�"];

		if (iHeight != oldH || iWidth != iWidth) {

			UCI::OPTIONS["ORGwhByCamera"] = 0;
			UCI::OPTIONS["ͼ����"] = iWidth;
			UCI::OPTIONS["ͼ��߶�"] = iHeight;
		}
		

		CRect cr( 0,0,iWidth - 1,iHeight - 1 );
		this->ChangeImageCRect(cr);

//		this->m_pHWinFG->SetPart(0, 0, iHeight-1, iWidth-1);
		this->m_pMHWinMain->SetPart(0, 0, iHeight - 1, iWidth - 1);
	
		return true;
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());

		return false;
	}

}

bool ST_MYDISP::LoadDefaultPicture()
{
	CString str = UCI::strMoudlePath + "\\start.png";
	this->ReadImageFromFile(str);
	SetPartHvWnd();
	
	

	return true;
}

void ST_MYDISP::ChangeImageCRect(CRect rect)
{
	rect.NormalizeRect();
	if (rect.Width() < 10) return;

	double dRatio = UCI::OPTIONS["ͼ�����"];
	rect.bottom = rect.top + int(rect.Width()*dRatio);
	this->rcImage = rect;
}

void CPPExpDefaultExceptionHandler(const HalconCpp::HException & except)
{


	CString str;

	// ��ȡ HALCON ������Ϣ
	str.Format("�������ӣ� s%, ������Ϣ�� %s, ErrorCode: %d", 
		except.ProcName(), except.ErrorMessage(), except.ErrorCode());

	AfxMessageBox(str);

	throw except;
}

