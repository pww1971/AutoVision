#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
1��ģ����lbp������Ҷ�ֱ��ͼ��
2�������ߵ���ֵ����ֱ��ͼΪ�������֡�����ģ��ĸߵ���ֵ�������ֵ�ֵ��rgb
3������ͼlbp����n*n�Ĵ��廬����ͳ��ֱ��ͼ�����ߵ���ֵ������������ֵ�ֵrgb����ģ���rgb�Ĳ
*/

#include "GuiContrlDlg.h"

class S_CheckParam:public mouseKeyCtr
{
public:
	S_CheckParam();
	~S_CheckParam();
	void readParam(string file);
	void writeParam(string file);
	virtual bool design(bool add = false, int id = -1);

	int m_iHisType;//0,lbpֱ��ͼ��1elbpֱ��ͼ
	int m_N;//ɨ���С
	int m_channel;//ͨ��ѡ��
	void* m_pCheck;
};
class checkPatch : CpwwRegionConnect
{
public:
	checkPatch();
	~checkPatch();
	S_CheckParam m_myParam;
	//ͼƬ�Ĵ���	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid);
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	//����pww���� �������ӺͲ�����ɨ����������ë�� �ⲽҪ�����������ڵĺ���õ���mpicF��ͼ����Ƕ�Ӧ��
	bool verticalPwwTreat(cv::Mat& src,	CTpltParam& pa,bool bDraw=false);
	bool pwwTreat(cv::Mat& src);
	//���
	bool checkDug(cv::Mat&src);
	void readMode(string file);
	void writeMode(string file);
	//pww����ģ��
	void genMode();
	//���˵� ��pww���ݲ�ֵ ���˳���2�����س��ĵ�
	bool filterPoint(cv::Point& pt1,cv::Point& pt2);
	//���� ���˵�
	bool isMaskScope(cv::Point& pt);
private:
	void cmpMask(cv::Mat& src,cv::Mat& pwwMask,CTpltParam& pa,bool bDraw);
	CPicFeater m_PicF;

	
public:
	
	static CGuiContrlDlg m_dlg;
	static bool iniDlg();//��ģʽ�Ի���
	string m_szRet;
};

#endif