#pragma once

#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED
#include "Common.h"
#include <fstream>
#include <sstream>
#include <xstring>
#include <afxtempl.h>
#include <HalconCpp.h>
#include <map>
#include "pwwVision.h"

class ST_MYDISP;

namespace UCI {

	class Option;
	class RIBBON_MENU;
	class stElement;

	/// Custom comparator because UCI options should be case insensitive
	//struct CaseInsensitiveLess {
	//	bool operator() (const std::string&, const std::string&) const;
	//};

	/// Our options container is actually a std::map
	typedef std::map<CString, Option> OptionsMap;

	class RIBBON_MENU {

	public:

		RIBBON_MENU();

		bool bIsInROIdrawing;                                 // 当前是不是在 ROI 绘图中

		eCOMBO_MATCH_ROI_TYPE iCOMBO_MATCH_ROI_TYPE;          // 当前的类型
	};


	class ST_PROJECT
	{
	public:

		ST_PROJECT(int id) { this->ID = id; };

		int ID;
		std::string ProjectName;
		std::string ProjectNote;
		std::string datatime;
	};

	/// Option class implements an option as defined by UCI protocol
	class Option {

		typedef void(*OnChange)(Option&);     // 值改变了，要调用相应的函数去处理一下

	public:
		
		Option(OnChange = nullptr);
		// Option(bool v, OnChange = nullptr);	
		Option(CString sname, const CString v, const CString note = "Paramter Note",bool show = true, OnChange = nullptr);
		Option(CString sname, int v, int min, int max, const CString note = "Paramter Note", bool show = true, OnChange = nullptr);
		Option(CString sname, double v, double dmin, double dmax, const CString note = "Paramter Note", bool show = true, OnChange = nullptr);

		//Option(CString sname, int v, int min, int max, bool iscout, const CString note = "Paramter Note", bool show = true, OnChange = nullptr);
		//Option(CString sname, const CString v, bool iscout, const CString note = "Paramter Note", bool show = true, OnChange = nullptr);
		//Option(CString sname, double v, double dmin, double dmax, bool iscout, const CString note = "Paramter Note", bool show = true, OnChange = nullptr);

		Option& operator=(const CString&);
		Option& operator=(const int&);
		Option& operator=(const double&);
		Option& operator=(const HalconCpp::HTuple&);

		//Option& operator=(const std::string&, const std::string&);
		//Option& operator=(const int&, const std::string&);
		//Option& operator=(const double&, const std::string&);

		void operator<<(const Option&);
		operator int() const;
		operator double() const;
		operator CString() const;	
		operator HalconCpp::HTuple() const;

		
		size_t getIdx() { return this->idx; };
		CString getName() { return this->name; };
		CString getType() { return this->type; };
		CString getcurrentValue() { return this->currentValue; };
		CString getNote() { return this->explain; };
		int getimin() { return this->imin; };
		int getimax() { return this->imax; };
		bool getshow() { return this->bshow; };
		//bool getCamIsOut() { return this->camIsOut; };

		void setNote(const CString str) { this->explain = str; };

		static size_t insert_order;

		string getOptionStr()
		{
			string info=name + "@" + defaultValue + "@" + currentValue + "@" + type + "@" + explain + "@";
			char sz[1024];
			sprintf(sz,"%d@%d@%lf@%lf@%d@%d",imin,imax,dmin,dmax,idx,bshow);
			info += sz;
			return info;
		};

		void strToOption(string str)
		{
			vector<string> vct;
			GDKT::CStrProcess::Split(str,"@",vct);
			if(vct.size()<8)
				return;
			name = vct[0].c_str();
			defaultValue = vct[1].c_str();
			currentValue = vct[2].c_str();
			type = vct[3].c_str();
			explain = vct[4].c_str();
			imin = atoi(vct[5].c_str());
			imax = atoi(vct[6].c_str());
			if(vct.size()>7)
				dmin = atof(vct[7].c_str());
			if(vct.size()>8)
				dmax = atof(vct[8].c_str());
			if(vct.size()>9)
				idx = atoi(vct[9].c_str());
			if(vct.size()>10)
				bshow = atoi(vct[10].c_str());
		};
	private:
		//friend std::ostream& operator<<(std::ostream&, const OptionsMap&);

		
		CString name, defaultValue, currentValue, type, explain;
		int imin, imax;
		double dmin, dmax;
		size_t idx;
		bool bshow;                // 是否显示
		//bool camIsOut = false;     // 是否要输出到相机参数中去
		OnChange on_change;        // 触发函数
	};

	class stElement {

	public:		
		//friend int SQLDATA::MyLoadElementFromDatabase();
		//friend int CProjectView::UpdataByList();
		stElement()
		{
			EleList = NULL;//
		}
		~stElement()
		{
			EleList = NULL;//
		}
	public:

		OptionsMap* add_One_Element(int iProject, eELEMENT_TYPE);                          // 添加一个元素

		BOOL isELeListOKandDelBad(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,OptionsMap *o);

		int UpdateProjectByList();                                                         // 更新项目树的当前项目
		OptionsMap* initOptionMapElemnet(eELEMENT_TYPE t, int ProjectID);                  // 元素初始化
		int saveOneOptionsMap(OptionsMap& o, int index);                                   // 保存一个元素要数据库中去

		BOOL isHaveSaveElementName(CString str);                                            // 是否有相同的名称
		
		BOOL RemoveOneElement(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,CString stEleName);    
		OptionsMap *GetOptionsMapByEletype(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,eELEMENT_TYPE e);

		OptionsMap* GetOptionsMapByEleName(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,CString name);                                   // 通过元素名称查找元素
		POSITION GetOptionsPosByEleName(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,CString name);


		// 变量
	public:
		CList <OptionsMap, OptionsMap>* EleList;                 // 测量元素   
	private:
		void initOptionMapElemnetLine(OptionsMap& o);
		void initOptionMapElemnetCircle(OptionsMap& o);
		void initOptionMapElemnetResult(OptionsMap& o);
		void initOptionMapElemnetAngle(OptionsMap& o);             // 检测二个直线元素的角度
		void initOptionMapElemnetDistance(OptionsMap& o);
		void initOptionMapElemnetCamera(OptionsMap& o);            // 初始化一个相机参数
		void initOptionMapFUNCTION(OptionsMap& o);
		void initOptionMapMATCH(OptionsMap& o);

		void initOptionMapElemnetMM(OptionsMap& o);
		void initOptionMapElemnetPeek(OptionsMap& o);
		void initOptionMapELEMENT_MEASURE_RECTANGLE2(OptionsMap& o);
		void initOptionMapBalloonFlaw(OptionsMap& o);
		void initOptionMapROI(OptionsMap& o);
		void initOptionMapBalloonLength(OptionsMap& o);
	};	

	void initFirst();              // 先初始一些基本的变量
	void initOptionsMap(OptionsMap&);

//	int MySaveSettingToSqlite();
//	int MyReadSettingFromSqlite();

	void GetHTupleFromString(HalconCpp::HTuple&, std::string&);
	/// =====================================================================
	/// 全局函数	


	int FindSpecialFile(CString stPath,      // 查找当前目录指定后缀的文件数量
		CString stExt, CArray<CString, CString&>* caPathFile, bool isDel = false);

    CString Utf8ToTChar(const std::string& utf8);
	std::string TCharToUtf8(const CString& tchar);

	void InitEmptyProject(int prID);

	/// =====================================================================

	/// =====================================================================
	/// Halcon 例程
	void action_wafer_dicing();					    // 4.1 半导体晶片切割
	void action_read_serila_number();				// 4.2 读取相机上的系列号
	void action_inspection_of_saw_blades();         // 4.3 读取相机上的系列号
	void action_pring_inspection();                 // 4.4 印刷检查
	void action_BGA_inspection();                   // 4.5 BGA 检测
	void action_sureface_inspection();              // 4.6 表面检测
	void action_park_plug_measuring();				// 4.7 电火花测量	
	void action_mold_flash_detection();             // 4.8 模制品披峰检测
	void action_puched_sheet_detection();           // 4.9 冲孔板检查
	void action_3D_reconstrucion();                 // 4.10 双目立体视觉三维平面重构
	void action_pose_verfication_of_res();          // 4.11 电阻姿态检验
	void action_classification_fabrics();           // 4.12 非织布分类
	

	void reopen_window_fit(HalconCpp::HObject ho_Image, HalconCpp::HTuple hv_MaxExtent, HalconCpp::HTuple hv_Row, HalconCpp::HTuple hv_Col,
		HalconCpp::HTuple *hv_WindowHandle);

	void init_font(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Font, HalconCpp::HTuple hv_Size);

	void dev_update_all(HalconCpp::HTuple hv_DisplayMode);

	void write_message(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Row, HalconCpp::HTuple hv_Column, HalconCpp::HTuple hv_String,
		HalconCpp::HTuple hv_WindowCoordinates);

	void get_shape_model_contour_ref(HalconCpp::HObject ho_ModelRegion, HalconCpp::HObject *ho_ModelContoursTrans,
		HalconCpp::HTuple hv_ModelID);

	void gen_arrow_contour_xld(HalconCpp::HObject *ho_Arrow, HalconCpp::HTuple hv_Row1, HalconCpp::HTuple hv_Col1, HalconCpp::HTuple hv_Row2,
		HalconCpp::HTuple hv_Col2, HalconCpp::HTuple hv_Size);

	void plot_tuple(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Tuple, HalconCpp::HTuple hv_TextXAxis, HalconCpp::HTuple hv_TextYAxis);

	extern stElement ELE;	
	extern OptionsMap OPTIONS;                                // 全局变量
	extern CString strMoudlePath;                             // AppPath 安装目录
	extern RIBBON_MENU RIBmenu;                              // 菜单显示用结构变量          
	extern ST_MYDISP* pDis;                                  // 显示窗口用结构变量;
	extern CString stPictureSavePath;                        // 图像保存的目录

} // namespace UCI

 






#endif