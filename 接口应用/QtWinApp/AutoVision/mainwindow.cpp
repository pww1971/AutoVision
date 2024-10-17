#include "automove.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QtWinExtras/QtWinExtras>   //winAPI，需要使用它来获取opencv窗口句柄

//运动界面的响应函数  可以弹出制程对话框
#define MVSHARPWIN(funname) bool funname(string* str) { \
    if(g_MB->getRuningState())					\
        return 0;				\
    return 1;}

//运动界面
MVSHARPWIN(onMvSharpWin1)
MVSHARPWIN(onMvSharpWin2)
MVSHARPWIN(onMvSharpWin3)
MVSHARPWIN(onMvSharpWin4)
MVSHARPWIN(onMvSharpWin5)
MVSHARPWIN(onMvSharpWin6)
MVSHARPWIN(onMvSharpWin7)
MVSHARPWIN(onMvSharpWin8)

void onMvMouse(int event, int x, int y, int flag = 0, void* p = NULL)
{
    g_MB->m_demoCtrl.onMouse(event, x, y, flag, p);
}

void ioCallBack4(int procId, int id, string str, void* p)
{
    if (g_MB->m_bStop)
        return;
    g_MB->m_demoCtrl.setMvState(procId, id);
}

void loadPicProc()
{
    //加载所有图像制程
    CConfig cfg(getExePath() + PROCCFG);
    string szVal = cfg.getItem("cfgNum");
    if ("" != szVal)
    {
        int len = atoi(szVal.c_str());
        for (int k = 0; k < len; k++)
        {
            szVal = cfg.getItem(CStrProcess::itoa_radixN(k));
            if (g_MB->m_pCheck && g_MB->m_pCheck->loadprocCheck(k, szVal))
            {
                ;
            }
        }
    }
    else
    {
        g_MB->m_pCheck->loadprocCheck(0, getExePath()+ "picProc");
    }
}

void reloadMVCfg(string dir)
{
    g_MB->Mvstop();
    string file = dir + "/MoveCfg";
    g_MB->reload(file);
    g_MB->analyseArrayMV();
    string command = g_MB->m_mtc.m_szArrayStr;
    g_MB->m_mtc.analyseVec(command);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (NULL == g_MB)
    {
        g_MB = new CVisionSystem();
    }
    vector<sharpFun> m_vctFun;
    sharpFun fun[] = { onMvSharpWin1,onMvSharpWin2,onMvSharpWin3,onMvSharpWin4,onMvSharpWin5,onMvSharpWin6,onMvSharpWin7,onMvSharpWin8 };
    int len = sizeof(fun) / sizeof(sharpFun);
    for (int i = 0; i < len; i++)
    {
        m_vctFun.push_back(fun[i]);
    }

    g_MB->ini();
    {
        //回调初始化
        g_MB->m_errFun = 0;
        g_MB->m_mtc.m_arrErrFun = 0;
        g_MB->m_io.m_ioStateFun = 0;
        g_MB->m_ioMvStateFun = ioCallBack4;
        g_MB->m_CheckRet = 0;
        g_MB->m_writePic.m_fun = 0;
    }

    string szviewname = g_MB->m_demoCtrl.m_mainWinName;
    cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
    cv::setMouseCallback(szviewname, onMvMouse);
    cv::namedWindow("motorview", cv::WINDOW_NORMAL);
    cv::namedWindow("win_opencv1", cv::WINDOW_NORMAL);



    //运动界面设置
     string szPath = getExePath();
    //图像制程加载
     g_MB->m_pCheck->loadprocCheck(0, szPath + "picProc/2");
    //运动控制
    szPath += "move/2";
    reloadMVCfg(szPath);

    //运动界面设置
    if (!g_MB->m_demoCtrl.ini(szPath, m_vctFun))
         ;

    szviewname = g_MB->m_demoCtrl.m_mainWinName;
    HWND hwnd=(HWND)cvGetWindowHandle(szviewname.c_str());
    HWND hparent=::GetParent(hwnd);//获取父窗口
    ::SetParent(hwnd,(HWND)this->ui->widget->winId());//设置父窗口
    ShowWindow(hparent, SW_HIDE);
    resizeWindow(szviewname.c_str(), cv::Size(this->ui->widget->width(), this->ui->widget->height()));//将尺寸设置与QLabel一致

    hwnd=(HWND)cvGetWindowHandle("motorview");
    hparent=::GetParent(hwnd);//获取父窗口
    ::SetParent(hwnd,(HWND)this->ui->widget_2->winId());//设置父窗口
    ShowWindow(hparent, SW_HIDE);
    resizeWindow("motorview", cv::Size(this->ui->widget_2->width(), this->ui->widget_2->height()));//将尺寸设置与QLabel一致

    hwnd=(HWND)cvGetWindowHandle("win_opencv1");
    hparent=::GetParent(hwnd);//获取父窗口
    ::SetParent(hwnd,(HWND)this->ui->widget_3->winId());//设置父窗口
    ShowWindow(hparent, SW_HIDE);
    resizeWindow("win_opencv1", cv::Size(this->ui->widget_3->width(), this->ui->widget_3->height()));//将尺寸设置与QLabel一致

    m_timerId = this->startTimer(1000);
}

MainWindow::~MainWindow()
{
    if (NULL != g_MB)
    {
        delete g_MB;
        g_MB = NULL;
    }
    this->killTimer(m_timerId);
    delete ui;
}
void MainWindow::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_timerId)
    {


        {
            //m_timerId2 = this->startTimer(1000);
            this->killTimer(m_timerId);
        }
    }
    else
        cv::waitKey(1);
}
std::string stringToUTF8(const std::string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    ZeroMemory(pwBuf, nwLen * 2 + 2);
    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);
    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string strRet(pBuf);

    delete []pwBuf;
    delete []pBuf;
    pwBuf = NULL;
    pBuf  = NULL;

    return strRet;
}

std::string UTF8ToString(const std::string& str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    memset(pwBuf, 0, nwLen * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);
    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string strRet = pBuf;

    delete []pBuf;
    delete []pwBuf;
    pBuf = NULL;
    pwBuf = NULL;

    return strRet;
}


void MainWindow::on_pushButton_clicked()
{
    ui->label->setText(stringToUTF8(g_threadMgr.checkThread()).c_str());
    cv::waitKey(1);

}


void MainWindow::on_pushButton_2_clicked()
{
    //开始运行
   g_MB->m_pCheck->m_curCamId = 0;//仿真相机编号 simulation文件夹的0文件夹
   g_MB->m_io.ioTrigPause(false);
   string command = g_MB->m_szChainMv;
   if (command != "")
   {
       g_MB->updateProb();
       g_MB->pushMsg(command);
   }
}




