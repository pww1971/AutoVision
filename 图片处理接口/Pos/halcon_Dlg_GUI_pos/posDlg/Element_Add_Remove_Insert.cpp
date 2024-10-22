#include "stdafx.h"

#include <algorithm>
#include <sstream>

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;

using namespace std;
using std::string;

namespace UCI {





	// ���ϵ��²飬���������Ԫ�������˲����ڵ�Ԫ�أ�9�����Ԫ��ɾ��������FALSE;
	BOOL stElement::isELeListOKandDelBad(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,OptionsMap *o)
	{
		POSITION pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION oldPos = pos;
			OptionsMap *o = &eleList.GetNext(pos);   // �����POSȡֵ����Զ�ָ����һ������
		int etype = (*o)[CST_PosEleType];
			switch (etype) {
			case ELEMENT_ANGLE:     // �ɶ�������ɣ�
			{
				CString name;
				name = (*o)["USE_LINE1_Parament_Name"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["USE_LINE2_Parament_Name"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
				eleList.RemoveAt(oldPos);
				return FALSE;
				}
			}
			break;
			case ELEMENT_DISTANCE:
			{
				CString name;
				name = (*o)["����Ԫ������1"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["����Ԫ������2"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
				eleList.RemoveAt(oldPos);
				return FALSE;
				}
			}
			break;
			case ELEMENT_RESULT:               // ����������Ԫ�صĽ��
			{
				CString name;
				name = (*o)["���Ԫ������"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
			}
			break;
			case ELEMENT_BALLOONLENGTH:           // ����������Ԫ�صĽ��
			{
				CString name;
				name = (*o)["���ö���1"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["���ö���2"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["���ö���3"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["���ö���4"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
			}
			break;
			case ELEMENT_MM:                    // ����������Ԫ�صĽ��
			{
				//CString name;
				//name = (*o)["У��Ԫ�صĲ��� X"];
				//if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
				//	eleList.RemoveAt(oldPos);
				//	return FALSE;
				//}
				//name = (*o)["У��Ԫ�صĲ��� Y"];
				//if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
				//	eleList.RemoveAt(oldPos);
				//	return FALSE;
				//}
				//return FALSE;
			}
			break;
			default:
				break;
			}		
		}
		return TRUE;
	}

	// ɾ��һ��Ԫ��

	BOOL stElement::RemoveOneElement(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,CString stEleName)
	{
		POSITION pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION prepos = pos;
			UCI::OptionsMap *ot = &eleList.GetNext(pos);
			CString otName = (*ot)[CST_PosEleName];
			if (otName == stEleName) {
				eleList.RemoveAt(prepos);
				break;
			}
		}

		// ��Ҫɾ����ص��������Ԫ�ص�����Ԫ��
		pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &eleList.GetNext(pos);
			if (isELeListOKandDelBad(eleList,o) == FALSE) {    
				pos = eleList.GetHeadPosition();   // �Ѿ�ɾ���ˣ�Ҫ���¸���һ��
			}
		}		

		return TRUE;
	}

	OptionsMap * stElement::GetOptionsMapByEletype(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,eELEMENT_TYPE e)
	{
		POSITION pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &eleList.GetNext(pos);
			//////////////////////////////////////////////////
			// ��ÿһ��Ԫ�ؽ��в���	
			int etype = (*o)[CST_PosEleType];

			if (etype == e) return o;
		}
		return nullptr;
	}

	OptionsMap * stElement::GetOptionsMapByEleName(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,CString fname)
	{
		POSITION pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &eleList.GetNext(pos);
			CString name = (*ot)[CST_PosEleName];
			if (fname == name)
			{
				return ot;
			}
		}		
		return nullptr;
	}

	POSITION stElement::GetOptionsPosByEleName(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,CString fname)
	{
		POSITION pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION oldpos = pos;
			UCI::OptionsMap *ot = &eleList.GetNext(pos);
			CString name = (*ot)[CST_PosEleName];
			if (fname == name)
			{
				return oldpos;
			}
		}
		return NULL;
	}

}