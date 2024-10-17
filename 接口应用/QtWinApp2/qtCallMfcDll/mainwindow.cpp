#include <afxwin.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include "tinyxml2.h"
using namespace tinyxml2;

CProcDll m_pCheck;//检测dll
void onMouse(int event, int x, int y, int flag = 0, void* p = NULL)
{
    if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_RBUTTONUP || event == cv::EVENT_MBUTTONDOWN)
    {
        m_pCheck.dll_onMouseN(0, event, x, y, m_pCheck.m_pCheck);
        /*if (event == cv::EVENT_MBUTTONDOWN)
        {
            MessageBox(0, (*(string*)m_pCheck.m_pCheck).c_str(), "检测线程状态", MB_YESNO | MB_ICONQUESTION);
        }*/
    }
}
//递归获取二次制程的所有ng和ok名称
void getNgType(string xmlStr, vector<string>& vct, string szNgOrOk)
{
    if ("" == xmlStr)
    {
        return;
    }
    tinyxml2::XMLDocument doc;
    doc.Parse(xmlStr.c_str());
    if (!doc.Error())
    {
        XMLElement* rootEle = doc.FirstChildElement("POS");
        if (!rootEle)
        {
            vct.push_back("空制程");
            return;
        }
        else
        {
            const char* elementName = "mask";
            XMLElement* maskEle = rootEle->FirstChildElement(elementName);
            if (!maskEle)
            {
                vct.push_back(rootEle->Attribute("AnotherName"));
                return;
            }
            string str;
            while (maskEle) {//遍历mask
                XMLElement* checkEle = maskEle->FirstChildElement();
                while (checkEle) {//遍历检测子项
                    const char* szch = checkEle->Attribute(szNgOrOk.c_str());
                    if (NULL != szch)//提取ok或ng的制程名
                    {
                        vct.push_back(maskEle->Attribute("AnotherName"));//提取mask的制程名
                        getNgType(szch, vct, szNgOrOk);
                        return;
                    }
                    checkEle = checkEle->NextSiblingElement();
                }
                maskEle = maskEle->NextSiblingElement(elementName);		//下一个掩膜检测项目
            }
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openDlg();

    string szviewname = "win_opencv";
    m_pCheck.setViewName("");
    cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
    HWND hWnd = (HWND)cvGetWindowHandle(szviewname.c_str());;
    HWND hParent = ::GetParent(hWnd);
    ::SetParent(hWnd, (HWND)this->ui->widget_2->winId());
    ::ShowWindow(hParent, SW_HIDE);
    cv::resizeWindow(szviewname, cv::Size(this->ui->widget_2->width(), this->ui->widget_2->height()));
    cv::setMouseCallback(szviewname, onMouse);

    this->ui->widget_2->hide();
    this->ui->pushButton_2->hide();
    this->ui->pushButton_3->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDlg()
{
    CWnd * pWnd = CWnd::FromHandle((HWND)(this->ui->widget->winId()));
    m_pCheck.RunOpencvGuiDlg(pWnd);
    m_pCheck.VisibleOpencvGuiDlg(1);
    m_pCheck.SetOpencvGuiDir("./picProc");

}

void MainWindow::on_pushButton_clicked()
{
    if (this->ui->widget->isHidden())
    {
        this->ui->widget->show();
        this->ui->pushButton_4->show();

        this->ui->widget_2->hide();
        this->ui->pushButton_2->hide();
        this->ui->pushButton_3->hide();
    }
    else
    {
        this->ui->widget->hide();
        this->ui->pushButton_4->hide();

        this->ui->widget_2->show();
        this->ui->pushButton_2->show();
        this->ui->pushButton_3->show();

    }
}


void MainWindow::on_pushButton_2_clicked()
{
    // 选择文件夹
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择文件路径"), "./", QFileDialog::ShowDirsOnly);
    m_pCheck.loadprocCheck(0, dir.toStdString());
}


void MainWindow::on_pushButton_3_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);

    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选择文件"));

    //设置打开的文件路径
    fileDialog->setDirectory("./");

    //设置文件过滤器,只显示.ui .cpp 文件,多个过滤文件使用空格隔开
    fileDialog->setNameFilter(tr("File(*.bmp* *.png*)"));

    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);

    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);

    //获取选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        CoordinatePosXYZ pd;
        pd.vm.clear();
        for(int i=0;i<fileNames.size();i++)
        {
            pd.vm.push_back(cv::imread(fileNames[i].toStdString()));
        }
        if (pd.vm[0].empty())
        {
            return;
        }
        //内含并行运算 不需要多线程访问 单线程就可以把cpu吃光
        pd.m_procid = 0;
        m_pCheck.procCheck(&pd);

        //提取具体的ng或ok结果
        string szRet = pd.str;
        if (szRet.size() > 6)
        {
            vector<string>  o;
            getNgType(szRet, o, "NG");
            for (int i = 0; i < o.size(); i++)
            {
                string szNgType = o[i];//ng具体的类型
            }
        }
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    // 选择文件
    QString file;
    QStringList files = QFileDialog::getOpenFileNames(this, tr("selected"), "./", tr("File(*.bmp* *.png*)"), &file, QFileDialog::ShowDirsOnly);

    //获取选择的文件的路径
    if (files.size()>0)
    {
        vector<cv::Mat> vm;
        for (int i = 0; i < files.size(); i++)
        {
            vm.push_back(cv::imread(files[i].toStdString()));
        }
        m_pCheck.updateMain(vm);
    }

}


void MainWindow::on_pushButton_5_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择文件路径"), "./", QFileDialog::ShowDirsOnly);
    m_pCheck.SetOpencvGuiDir(dir.toStdString());
}

