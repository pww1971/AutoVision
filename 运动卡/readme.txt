��
_LINHUA �軪�˶���
IOEXT �軪+io��չ��
_SIMULATION �����˶���
_ZMC ���˶��˶���


�軪��������127 126�������˿���ͣ״̬ ����������426�� state[g_ioNum-n-1] = (( motion_io >> MIO_EMG ) & 1);//��ͣ״̬


����Ҫ�������Դ������˶���dll�ķ�����
��������-������-������ ���� ../x64\Debug\AutoVision.exe
�˶�������Ϊ������Ŀ 


���ADת���Ͳ岹�˶�����ؽӿ� ���Ը��������������
1����MOVE_CARD.cpp �ļ�����Ӳ�ʵ��
extern "C" PROCLIB_API void myDefineMoveType();
void myDefineMoveType()
{
��ѯ�˶�������ADת���Ͳ岹�˶���api�ӿ�ʵ��
}
2���ⲿ�����豸��д�´���
void (*myDefineMove)()=NULL;//�ȶ��庯��ָ��
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		CMoveDllType* handle = mb->m_mtc.m_handle;
		if (NULL == myDefineMove)
			if (!handle->GetFunction("myDefineMoveType", myDefineMove)) //��dll�м���AD��岹�˶�����
				return false;
		if (data == "check")
		{
			myDefineMove();//ֱ�ӵ��øú���
		}
	}

}


�����˻��е�ֱ�һ�����ͨѶ��ʽ��Ҳ���Է�װ����չ�ӿڵ���ʽ ���ⲿ�����豸����