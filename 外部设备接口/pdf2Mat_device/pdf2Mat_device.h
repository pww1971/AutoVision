// pdf2Mat_device.h: pdf2Mat_device DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include "automove.h"


#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv, void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();


#include <mupdf/fitz.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#pragma comment( lib, "libmupdf.lib")
#pragma comment( lib, "libthirdparty.lib")
#pragma comment( lib, "libresources.lib")
//#pragma comment( lib, "libcurl.lib")
//#pragma comment( lib, "libextract.lib")
//#pragma comment( lib, "libpkcs7.lib")
//#pragma comment( lib, "libfreeglut.lib")
//#pragma comment( lib, "libmuthreads.lib")
//#pragma comment( lib, "mu-office-lib.lib")
//#pragma comment( lib, "libmutool.lib")
cv::Mat fz_pixmap_to_mat(fz_context* ctx, fz_pixmap* pix)
{
	// Get image properties
	int width = fz_pixmap_width(ctx, pix);
	int height = fz_pixmap_height(ctx, pix);
	int channels = fz_pixmap_components(ctx, pix);

	// Create OpenCV Mat object
	cv::Mat mat(height, width, CV_8UC(channels));
	int rowsize = width * channels;
	// Copy pixmap data to Mat
	unsigned char* src = fz_pixmap_samples(ctx, pix);
	for (int y = 0; y < height; ++y)
	{
		unsigned char* dst = mat.ptr<unsigned char>(y);
		memcpy(dst, src, rowsize);
		src += rowsize;
	}

	return mat;
}
bool PDF2PNG(std::string pdfFilePath, std::vector<cv::Mat>& vm/*,std::string outputPNGFilePath=""*/)
{
	if (pdfFilePath.empty() )
	{
		return false;
	}

	//使用默认值
	int pageCount = 0;
	float zoom = 100;
	float rotate = 0;

	fz_context* ctx;
	fz_document* doc;
	fz_pixmap** pix;
	fz_matrix ctm;
	int x, y;

	/* Create a context to hold the exception stack and various caches. */
	ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	if (!ctx)
	{
		fprintf(stderr, "cannot create mupdf context\n");
		return false;
	}

	/* Register the default file types to handle. */
	fz_try(ctx)
	{
		fz_register_document_handlers(ctx);
	}
	fz_catch(ctx)
	{
		fprintf(stderr, "cannot register document handlers: %s\n", fz_caught_message(ctx));
		fz_drop_context(ctx);
		return false;
	}

	/* Open the document. */
	fz_try(ctx)
	{
		doc = fz_open_document(ctx, pdfFilePath.c_str());
	}
	fz_catch(ctx)
	{
		fprintf(stderr, "cannot open document: %s\n", fz_caught_message(ctx));
		fz_drop_context(ctx);
		return false;
	}

	/* Count the number of pages. */
	fz_try(ctx)
	{
		pageCount = fz_count_pages(ctx, doc);
	}
	fz_catch(ctx)
	{
		fprintf(stderr, "cannot count number of pages: %s\n", fz_caught_message(ctx));
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return false;
	}

	/*if (pdfPageIdx < 0 || pdfPageIdx >= pageCount)
	{
		fprintf(stderr, "page number out of range: %d (page count %d)\n", pdfPageIdx + 1, pageCount);
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return false;
	}*/

	/* Compute a transformation matrix for the zoom and rotation desired. */
	/* The default resolution without scaling is 72 dpi. */
	ctm = fz_scale(zoom / 100, zoom / 100);
	ctm = fz_pre_rotate(ctm, rotate);

	/* Render page to an RGB pixmap. */
	fz_try(ctx)
	{
		pix = new fz_pixmap*[pageCount];
		for (int i=0;i< pageCount;i++)
			pix[i] = fz_new_pixmap_from_page_number(ctx, doc, i, ctm, fz_device_rgb(ctx), 0);
	}
	fz_catch(ctx)
	{
		fprintf(stderr, "cannot render page: %s\n", fz_caught_message(ctx));
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return false;
	}
	fz_try(ctx)
	{
		vm.clear();
		for (int i = 0; i < pageCount; i++)
			vm.push_back(fz_pixmap_to_mat(ctx, pix[i]));
		
		/*if ("" != outputPNGFilePath)
			fz_save_pixmap_as_png(ctx, pix[0], outputPNGFilePath.c_str());*/
	}
	fz_catch(ctx)
	{
		for (int i = 0; i < pageCount; i++)
			fz_drop_pixmap(ctx, pix[i]);
		delete[] pix;
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return false;
	}

	/* Clean up. */
	for (int i = 0; i < pageCount; i++)
		fz_drop_pixmap(ctx, pix[i]);
	delete[] pix;
	fz_drop_document(ctx, doc);
	fz_drop_context(ctx);

	return true;
}

class S_DeviceParam
{
public:
	S_DeviceParam()
	{
		m_procAddress = "";
		m_cfgAdd = "";
	}
	~S_DeviceParam()
	{

	}

	void loadcfg()
	{
		if ("" != m_cfgAdd)
		{
			CConfig cfg(m_cfgAdd);
			string szVal = cfg.getItem("procAddress");
			if ("" != szVal)
			{
				m_procAddress = szVal;
			}
			if ("" != m_procAddress)//目录中自动找到极坐标定位的配置文件和模板图片 并能加载成功 
			{
			}
		}

	};
	void writecfg()
	{
		if ("" != m_cfgAdd)
		{
			remove(m_cfgAdd.c_str());
			CConfig cfg(m_cfgAdd);
			cfg.updateItem("\n#制程地址", "");
			cfg.updateItem("reportAddress", m_procAddress);
		}
	}
	string m_procAddress;
	string m_cfgAdd;

};


// Cpdf2MatdeviceApp
// 有关此类实现的信息，请参阅 pdf2Mat_device.cpp
//

class Cpdf2MatdeviceApp : public CWinApp
{
public:
	Cpdf2MatdeviceApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
