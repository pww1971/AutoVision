
#include "camera.h"


CCamera::CCamera()
{
}

CCamera::~CCamera()
{

}

//相机抓图
int CCamera::grapImage(cv::Mat& m)
{
	return 0;
}

/*相机驱动初始化，
参数：nIndex设备标记 
返回值：-1 错误，0 正常
*/
int CCamera::ini(unsigned int nIndex )
{
	return 0;
}
