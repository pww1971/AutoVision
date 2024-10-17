#include "stdafx.h"

#include <algorithm>
#include <sstream>

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;

using namespace std;
using std::string;

namespace UCI {





	// 从上到下查，如果发现有元素引用了不存在的元素，9将这个元素删除，返回FALSE;
	BOOL stElement::isELeListOKandDelBad(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,OptionsMap *o)
	{
		POSITION pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			POSITION oldPos = pos;
			OptionsMap *o = &eleList.GetNext(pos);   // 这儿的POS取值后会自动指向下一个！！
		int etype = (*o)[CST_PosEleType];
			switch (etype) {
			case ELEMENT_ANGLE:     // 由二条线组成，
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
				name = (*o)["引用元素名称1"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["引用元素名称2"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
				eleList.RemoveAt(oldPos);
				return FALSE;
				}
			}
			break;
			case ELEMENT_RESULT:               // 引用了其它元素的结果
			{
				CString name;
				name = (*o)["输出元素名称"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
			}
			break;
			case ELEMENT_BALLOONLENGTH:           // 引用了其它元素的结果
			{
				CString name;
				name = (*o)["引用顶点1"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["引用顶点2"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["引用顶点3"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
				name = (*o)["引用顶点4"];
				if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
					eleList.RemoveAt(oldPos);
					return FALSE;
				}
			}
			break;
			case ELEMENT_MM:                    // 引用了其它元素的结果
			{
				//CString name;
				//name = (*o)["校正元素的参数 X"];
				//if (name != "" && GetOptionsMapByEleName(eleList,name) == nullptr) {
				//	eleList.RemoveAt(oldPos);
				//	return FALSE;
				//}
				//name = (*o)["校正元素的参数 Y"];
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

	// 删除一个元素

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

		// 还要删除相关的引用这个元素的其它元素
		pos = eleList.GetHeadPosition();
		while (pos != NULL) {
			OptionsMap *o = &eleList.GetNext(pos);
			if (isELeListOKandDelBad(eleList,o) == FALSE) {    
				pos = eleList.GetHeadPosition();   // 已经删除了，要重新更新一下
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
			// 对每一个元素进行测量	
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