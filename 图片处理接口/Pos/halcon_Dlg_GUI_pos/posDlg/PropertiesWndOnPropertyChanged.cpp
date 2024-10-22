
// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

#include "stdafx.h"
#include "checkPatch.h"
#include "PropertiesWnd.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
using namespace HalconCpp;

// ȫ�ֱ������øı�
LRESULT CPropertiesWnd::OnPropertyChangedGlobal(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;

	CString name = pProp->GetName();            // ���ı�Ĳ�������

	int id = int(pProp->GetData());             // ���Ե�����ֵ
	COleVariant t1 = pProp->GetValue();         // �ı�֮���ֵ 
	t1.ChangeType(t1.vt);

	//COleVariant t2 = pProp->GetOriginalValue();   // �ı�֮ǰ��ֵ 

	UCI::OptionsMap* om = &UCI::OPTIONS;
	
	if (om->count(name)) {
		switch (t1.vt) {
		case VT_I4:       // ����
			(*om)[name] = t1.intVal;			
			break;
		case VT_R8:       // ������
			(*om)[name] = t1.dblVal;
			if (name == "ӳ����"
				|| name == "�궨 Z �� �Ƕ�"
				|| name == "�궨 Y �� �Ƕ�"
				|| name == "�궨 X �� �Ƕ�"
				|| name == "�궨ԭ�� X ����"
				|| name == "�궨ԭ�� Y ����"
				|| name == "�궨ԭ�� Z ����"
				) {
				UCI::OPTIONS["�궨��Ϣ OK"] = 0;
			}
			break;
		case VT_BSTR:     // �ַ���
			(*om)[name] = (CString)t1.bstrVal;
			if (name == "�������") 
			{
				// ����ȫ�ֱ���
				CString CameraName = (*om)[name];
//				UCI::sDB.MyReadSettingFromSqlite(CameraName);

				// ��������
				this->m_wndPropList.RemoveAll();     // �Ƴ�ԭ������������
				InitPropList_Common();
				this->m_wndPropList.Invalidate();    // ����ˢ��һ��

				// ���µ�����Ӧ���������
				//string lastCammer = UCI::OPTIONS["�������"];
				//UCI::sDB.MyReadCAMparaFromSqlite(lastCammer);        // �������	
				
				// ���½���Ŀ¼
				int curProject = UCI::OPTIONS[CST_PosProcID];
//				UCI::sDB.PrepareCamDeviceCatalog(curProject);
				
			}			

			break;
		default:
			break;
		}
	}
	return 0;
}



// �������Ըı�
LRESULT CPropertiesWnd::OnPropertyChangedProperty(WPARAM wParam, LPARAM lParam)
{
	try {

		CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;

		CString name = pProp->GetName();            // ���ı�Ĳ�������

		int id = int(pProp->GetData());                              // ���Ե�����ֵ
		COleVariant t1 = pProp->GetValue();                          // �ı�֮���ֵ 
		t1.ChangeType(t1.vt);

		COleVariant t2 = pProp->GetOriginalValue();                  // �ı�֮ǰ��ֵ 	

		// �õ���ǰ������
		UCI::OptionsMap* om = CurOption;       // 

		// UCI::CammerParaChanged = TRUE;
		int etype = (*om)[CST_PosEleType];


		if (om->count(name)) {
			switch (t1.vt) {
			case VT_I4:       // ����
				(*om)[name] = t1.intVal;
				break;
			case VT_R8:       // ������
				(*om)[name] = t1.dblVal;
				break;
			case VT_BSTR:     // �ַ���
				(*om)[name] = (CString)(t1.bstrVal);

				if (name == "���ö���1") {

				}
				else if (name == "���ö���2") {

				}
				else if (name == "���ö���3") {

				}
				else if (name == "���ö���4") {


				}

				else if (name == "����Ԫ������1") {

					CString elename = (CString)t1.bstrVal;
					UCI::OptionsMap* oc = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,elename);
					CMFCPropertyGridProperty* NpProp;


					// ������ѡ��Ԫ�ص�����
					NpProp = m_wndPropList.FindItemByData(USE_Distance1_Parament_Name_ID);
					NpProp->RemoveAllOptions();
					NpProp->SetValue((_variant_t)"");

					// ���ҵ��п������Ԫ�ر���
					for (UCI::OptionsMap::iterator it=oc->begin();it!=oc->end(); it++) 
					{
						UCI::Option& o = it->second;
						CString sname = it->first;

						if (sname.Left(4) == "RES_") {
							NpProp->AddOption(sname);
						}
					}
				}
				else if (name == "����Ԫ������2") {

					CString elename = (CString)t1.bstrVal;
					UCI::OptionsMap* oc = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,elename);

					CMFCPropertyGridProperty* NpProp;


					// ������ѡ��Ԫ�ص�����
					NpProp = m_wndPropList.FindItemByData(USE_Distance2_Parament_Name_ID);
					NpProp->RemoveAllOptions();
					NpProp->SetValue((_variant_t)"");

					// ���ҵ��п������Ԫ�ر���
					for (UCI::OptionsMap::iterator it=oc->begin();it!=oc->end(); it++) 
					{
						UCI::Option& o = it->second;
						CString sname = it->first;

						if (sname.Left(4) == "RES_") {
							NpProp->AddOption(sname);
						}
					}
				}
				else if (name == "���Ԫ������") {

					CString elename = (CString)t1.bstrVal;
					UCI::OptionsMap* oc = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,elename);

					CMFCPropertyGridProperty* NpProp;


					// ������ѡ��Ԫ�ص�����
					NpProp = m_wndPropList.FindItemByData(OUT_Element_Name_ID);
					NpProp->RemoveAllOptions();
					NpProp->SetValue((_variant_t)"");

					// ���ҵ��п������Ԫ�ر���
					for (UCI::OptionsMap::iterator it=oc->begin();it!=oc->end(); it++) 
					{
						UCI::Option& o = it->second;
						CString sname = it->first;

						if (sname.Left(4) == "RES_") {
							NpProp->AddOption(sname);
						}
					}
				}
				else if (name == "У��Ԫ�� X") {

					//CString elename = (t1.bstrVal);
					//UCI::OptionsMap* oc = UCI::ELE.GetOptionsMapByEleName(elename);

					//CMFCPropertyGridProperty* NpProp;


					// ������ѡ��Ԫ�ص�����
					//NpProp = m_wndPropList.FindItemByData(OUT_Element_Name_IDX);
					//NpProp->RemoveAllOptions();
					//NpProp->SetValue((_variant_t)L"");

					//// ���ҵ��п������Ԫ�ر���
					//for (auto& it : *oc)
					//{
					//	UCI::Option& o = it.second;
					//	CString sname = it.first;

					//	if (sname.Left(4) == L"RES_") {
					//		NpProp->AddOption(sname);
					//	}
					//}
				}
				else if (name == "У��Ԫ�� Y") {

					//CString elename = (t1.bstrVal);
					//UCI::OptionsMap* oc = UCI::ELE.GetOptionsMapByEleName(elename);

					//CMFCPropertyGridProperty* NpProp;

					//// ������ѡ��Ԫ�ص�����
					//NpProp = m_wndPropList.FindItemByData(OUT_Element_Name_IDY);
					//NpProp->RemoveAllOptions();
					//NpProp->SetValue((_variant_t)L"");

					//// ���ҵ��п������Ԫ�ر���
					//for (auto& it : *oc)
					//{
					//	UCI::Option& o = it.second;
					//	CString sname = it.first;

					//	if (sname.Left(4) == L"RES_") {
					//		NpProp->AddOption(sname);
					//	}
					//}
				}

				break;
			default:
				break;
			}
		}

		if (om->count(name)) {
			// ����Ҫ����һ���������
			if (etype == ELEMENT_CAMERA) {
			//	((checkPatch*)checkPatch::S_dlg.m_pCheck)->writeMode();
				//if (UCI::pDis->pCAM->GetIsOpen() == FALSE) { // ��ǰ����ǹصģ���Ҫ��һ�£�Ȼ���ٸ���Ҫ���õ�
				//	UCI::pDis->DisOpenCamera();
				//}
				//if (UCI::pDis->pCAM->GetIsOpen()) {
				//	UCI::sDB.SetOneCamePara(name, (*om)[name]);
				//}
			}
		}

		HalconCpp::HImage hImg;
		hImg = UCI::pDis->m_sIPResult.result_img;
		UCI::pDis->DisProcessROImodel(hImg, UCI::pDis->hv_Model->ho_ModelRegion);   // ����ģ��
}
	catch (HException& except) {
		//UCI::sDB.Log(except, "OnPropertyChangedProperty(��");
		return 0;
	}


	return 0;
}


LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	
	// �ȵõ���ǰ�����Դ��ڵ�ID
	int num = this->m_wndObjectCombo.GetCurSel();
	switch (num) {

	case PROPLIST_OTHER:
		return OnPropertyChangedProperty(wParam, lParam);
		break;
	case PROPLIST_GLOBAL:
		return OnPropertyChangedGlobal(wParam, lParam);
		break;
	default:
		break;
	}
	
	return 0;
}