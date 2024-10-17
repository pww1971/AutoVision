
#include "MyData.h"
#include "MyDispLay.h"
#pragma once


// CProjectView 视图

//封装了树控件和项目map表的联动响应 添加删除显示等 
class CProjectTree : public CTreeCtrl
{

	// 构造
public:
	CProjectTree();

	UCI::OptionsMap *CurProp;     // 当前元素
	int curElementPropertyIndex;             // 当前的属性ID

	// 当前选择的Item
	HTREEITEM m_CurrentSelectItem;	

	HTREEITEM findProjectItemByPrID(int prID);

	void DelAllByPrID(int prID);

	BOOL AddOneElementTail(UCI::OptionsMap *o);

	UCI::OptionsMap* AddOneElement(UCI::OptionsMap *o, HTREEITEM hCurrentSel);
	


	// 重写
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// 实现
public:
	virtual ~CProjectTree();

private:
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	int UpdataProjectTreeByList();
};


