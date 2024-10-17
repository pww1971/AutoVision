宏
_LINHUA 凌华运动卡
IOEXT 凌华+io扩展卡
_SIMULATION 仿真运动卡
_ZMC 正运动运动卡


凌华卡的软触发127 126是留给了卡急停状态 在驱动代码426行 state[g_ioNum-n-1] = (( motion_io >> MIO_EMG ) & 1);//急停状态


不需要主程序的源码调试运动卡dll的方法：
工程属性-》调试-》命令 填上 ../x64\Debug\AutoVision.exe
运动卡设置为启动项目 


针对AD转换和插补运动的相关接口 可以根据需求自行添加
1、在MOVE_CARD.cpp 文件里添加并实现
extern "C" PROCLIB_API void myDefineMoveType();
void myDefineMoveType()
{
查询运动卡关于AD转换和插补运动的api接口实现
}
2、外部导入设备中写下代码
void (*myDefineMove)()=NULL;//先定义函数指针
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		CMoveDllType* handle = mb->m_mtc.m_handle;
		if (NULL == myDefineMove)
			if (!handle->GetFunction("myDefineMoveType", myDefineMove)) //从dll中加载AD或插补运动函数
				return false;
		if (data == "check")
		{
			myDefineMove();//直接调用该函数
		}
	}

}


机器人或机械手臂一般采用通讯方式，也可以封装成扩展接口的形式 让外部导入设备调用