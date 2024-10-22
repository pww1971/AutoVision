#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
������ͷ���߼�⣬���Ƶ�dll���  ģ����������ķ�ʽ
������ת�Ƕ�
����ÿ���������������ĽǶ� ��ѯ �ҵ��Ƕ�һ�¾��������  �γ����� �����������ߵĽǶ���Ϊ��ת�Ƕ�
*/

#include "PwwRegionConnect.h"
#include "templateDesign.h"
#include "GuiContrlDlg.h"
struct S_CheckParam:CTpltParam
{
	S_CheckParam()
	{
	}
};
class checkPatch : CpwwRegionConnect
{
public:
	checkPatch();
	~checkPatch();
	CTpltParam m_myParam[2];
	//ͼƬ�Ĵ���	
	bool treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	//����pww���� �������ӺͲ�����ɨ����������ë�� �ⲽҪ�����������ڵĺ���õ���mpicF��ͼ����Ƕ�Ӧ��
	bool verticalPwwTreat(cv::Mat& src,	CTpltParam& pa,bool bDraw=false);

	bool soldered_dot(cv::Mat&src,CTpltParam pa[]);
	void readMode(string file);
	void writeMode(string file);

private:
	void cmpMask(cv::Mat& src,cv::Mat& pwwMask,CTpltParam& pa,bool bDraw);
	CPicFeater m_PicF;
public:
	static CGuiContrlDlg m_dlg;
	static bool iniDlg();//��ģʽ�Ի���

};

#endif