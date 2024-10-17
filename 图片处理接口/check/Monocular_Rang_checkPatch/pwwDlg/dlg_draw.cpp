
#ifndef DLG_DRAW_H
#define DLG_DRAW_H
#include "dlg_draw.h"

//制程界面操作
void onMouse(int event, int x, int y, int flag, void*p)
{
	mouseKeyCtr* pmkc = (mouseKeyCtr*)p;

//	if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_RBUTTONDOWN )
	{
//		pmkc->m_bScaleState = true;
		pmkc->onMouse( event,  x,  y,  flag, p);
	}
}


#endif //HISTOGRAM_INCLUDE_H