// ProjectView.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectTree.h"
#include "checkPatch.h"
using namespace std;

// CProjectView

//IMPLEMENT_DYNCREATE(CProjectTreeView, CTreeView)

CProjectTree::CProjectTree()
{
	m_CurrentSelectItem = NULL;
	CurProp = NULL;     // 当前元素
	curElementPropertyIndex = 0;             // 当前的属性ID

}

CProjectTree::~CProjectTree()
{
}


//https://social.msdn.microsoft.com/Forums/vstudio/en-US/7a012ba0-dad8-4644-810f-68f2ff94572f/vs-2015-c-compile-error-extra-text-after-expected-end-of-number?forum=vcgeneral

BEGIN_MESSAGE_MAP(CProjectTree, CTreeCtrl)
	ON_NOTIFY_REFLECT_EX1(TVN_BEGINLABELEDIT, &CProjectTree::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT_EX1(TVN_ENDLABELEDIT, &CProjectTree::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT_EX1(NM_DBLCLK, &CProjectTree::OnNMDblclk)
	ON_NOTIFY_REFLECT_EX1(NM_CLICK, &CProjectTree::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 消息处理程序


HTREEITEM CProjectTree::findProjectItemByPrID(int prID)         // 通过项目ID查找项目节点
{
	
	HTREEITEM hroot = this->GetRootItem();

	HTREEITEM hChildItem = GetChildItem(hroot);

	while (hChildItem != NULL) {

		if (this->GetItemData(hChildItem) == prID) {
			return hChildItem;
		}

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
	}
	
	return HTREEITEM();
}

// 删除项目下的所有测量元素
void CProjectTree::DelAllByPrID(int prID)
{
	HTREEITEM hroot = this->GetRootItem();
	HTREEITEM hChildItem = GetChildItem(hroot);
//	int prID = UCI::OPTIONS[CST_PosProcID];

	// 查找每一个项目
	while (hChildItem != NULL) {

		//if (this->GetItemData(hChildItem) == prID) {

			// 开始删除这个节点下面的孙节点
			HTREEITEM it = GetChildItem(hChildItem);

			while (it != NULL) {
				HTREEITEM hNextChild = GetNextSiblingItem(it);
				DeleteItem(it);
				it = hNextChild;
			}
		//}
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
	}
}

// 添加一个测量元素 到 项目树 上的尾部
BOOL CProjectTree::AddOneElementTail(UCI::OptionsMap * o)
{
	
	HTREEITEM hroot = this->GetRootItem();
	HTREEITEM hChildItem = GetChildItem(hroot);

	int prID = UCI::OPTIONS[CST_PosProcID];

	// 查找每一个项目
	while (hChildItem != NULL) {

		if (this->GetItemData(hChildItem) == prID) {

			CString name = (*o)[CST_PosEleName];
			int type = (*o)[CST_PosEleType];  

			POSITION pos = UCI::ELE.GetOptionsPosByEleName(*UCI::ELE.EleList,name);

			UINT mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

			this->InsertItem(mask, name,
				ELEMTEN_PIC_INDEX_NOSEL[type], ELEMTEN_PIC_INDEX_SEL[type],
				0, 0, (LPARAM)pos, hChildItem, NULL);

			this->Expand(hChildItem, TVE_EXPAND);
			return 0;
		}
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
	}
	
	return 1;
}

UCI::OptionsMap* CProjectTree::AddOneElement(UCI::OptionsMap * o, HTREEITEM hCurrentSel)
{
	HTREEITEM hroot = this->GetRootItem();
	HTREEITEM hProjectItem = GetChildItem(hroot);

	int prID = UCI::OPTIONS[CST_PosProcID];
	
	// 查找每一个项目
	while (hProjectItem != NULL) {
		if (this->GetItemData(hProjectItem) == prID) {
			CString name = (*o)[CST_PosEleName];
			int type = (*o)[CST_PosEleType];
			UINT mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					
			POSITION npos = UCI::ELE.GetOptionsPosByEleName(*UCI::ELE.EleList,name);
			if (hCurrentSel == NULL) {   // 显示			
			}
			else {
				UCI::ELE.EleList->InsertAfter(npos, *o);
				UCI::ELE.EleList->RemoveTail();
			}

			this->InsertItem(mask, name,
				ELEMTEN_PIC_INDEX_NOSEL[type], ELEMTEN_PIC_INDEX_SEL[type],
				0, 0, (LPARAM)npos, hProjectItem, hCurrentSel);   // 尾部插入

			this->Expand(hProjectItem, TVE_EXPAND);
			return o;
		}

		hProjectItem = GetNextItem(hProjectItem, TVGN_NEXT);
	}
	return NULL;
}

// 
BOOL CProjectTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}



void CProjectTree::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	this->GetEditControl()->LimitText(50);

	*pResult = 0;
}


void CProjectTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	HTREEITEM it = GetSelectedItem();
	HTREEITEM parent = this->GetParentItem(it);

	if (parent == nullptr) {   // 这个是根目录
		return;
	}

	TCHAR s[50];
	this->GetEditControl()->GetWindowText(s, 50);

	if (this->GetParentItem(parent) == nullptr) {    // 这个是项目

		// 更新项目名称
		int proID = (int)this->GetItemData(it);
	//	UCI::sDB.MyProjectRename(s, proID);
		this->SetItemText(it, s);
	}
	else {
		// 这是一个元素节点， 更名是要查一下是不是有重复的名称
		UCI::OptionsMap* oc = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,s);
		if (oc == nullptr) {
			this->SetItemText(it, s);
			POSITION pos = (POSITION)this->GetItemData(it);
			UCI::OptionsMap *ot = &UCI::ELE.EleList->GetNext(pos);
			//CString old = (*ot)[CST_PosEleName];
			(*ot)[CST_PosEleName] = (CString)s;
		}
		else {
			AfxMessageBox(CST_PosWord9);
		}
	}

	*pResult = 0;
}

// 用户双击了元素
void CProjectTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 如果当前状态不是 READY, 就返回
	HTREEITEM hSel = this->GetSelectedItem();
	HTREEITEM hParent;

	CString name = this->GetItemText(hSel);
	if (name == CST_PosWord17) { // root 节点
		return;
	}

	hParent = this->GetParentItem(hSel);
	if (hParent == nullptr)return;
	int curPrID = UCI::OPTIONS[CST_PosProcID];

	//===========================================================================================
	// 项目节点
	if (this->GetItemText(hParent) == CST_PosWord17) {
		int prID = (int)this->GetItemData(hSel);      

		if (prID == curPrID) {  // 双击了当前的项目，则显示全局的变量
			checkPatch::S_dlg.SetPettyCurSel(PROPLIST_GLOBAL);
			checkPatch::S_dlg.SetPropertyCurOptionsMap(&UCI::OPTIONS);
			checkPatch::S_dlg.ProPertyUpdate();
			return;
		}
		return;
	}
	//===========================================================================================
	// 元素节点
	int prID = (int)this->GetItemData(hParent);                        // 得到父节点的项目编号	
	if (prID == curPrID) {	

		// 在属性表中显示当前元素
		checkPatch::S_dlg.SetPettyCurSel(PROPLIST_OTHER);
		UCI::OptionsMap* ocur = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,name);
		checkPatch::S_dlg.SetPropertyCurOptionsMap(ocur);
		checkPatch::S_dlg.ProPertyUpdate();

		UCI::pDis->EditOneElement(ocur);
	}
	
	*pResult = 0;
}

// 单击鼠标
void CProjectTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint   point(GetMessagePos());
	unsigned   int   nFlags   =   0;
	ScreenToClient(&point);
	HTREEITEM   hItem   =   HitTest(point,&nFlags);
	if  ((nFlags& TVHT_NOWHERE))
	{
		UpdataProjectTreeByList();
	}
	UCI::pDis->OnRButtonDown(NULL);	
	checkPatch::S_dlg.SetTimer(1,300,NULL);
	*pResult = 0;
}

int CProjectTree::UpdataProjectTreeByList()
{
	int prID = UCI::OPTIONS[CST_PosProcID];

	// 查找当前项目树的节点
	HTREEITEM hProject = findProjectItemByPrID(prID);

	if (hProject != NULL) {

		DelAllByPrID(prID);

		// 插入所有该项目的元素
		POSITION pos = UCI::ELE.EleList->GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *o = &UCI::ELE.EleList->GetNext(pos);
			AddOneElement(o, NULL);
		}
	}

	return 0;
}
