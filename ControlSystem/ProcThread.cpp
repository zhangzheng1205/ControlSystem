// ProcThread.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ProcThread.h"
#include "IMotorCtrl.h"
#include "ImageProcess.h"
#include "ImageProcSettingDlg.h"

// CProcThread

IMPLEMENT_DYNCREATE(CProcThread, CWinThread)

CProcThread::CProcThread()
: m_IMotoCtrl(NULL)
, m_IsMotroCtrlConnected(FALSE)
, m_IImageProcess(NULL)
, m_ImageProcSetDlg(NULL)
, m_HalconWndOpened(false)
, m_pListData(NULL)
{
}

CProcThread::~CProcThread()
{
}

BOOL CProcThread::InitInstance()
{
	//��ʼ���忨
	m_IMotoCtrl = new IMotorCtrl();
	if(NULL != m_IMotoCtrl)
	{
		INT32 intResult = 0;
		intResult = m_IMotoCtrl->Init();
		if(0 != intResult)
		{
			AfxMessageBox(_T("���ƿ���ʼ��ʧ�ܣ�"));
		}
	}

	m_IImageProcess = new CImageProcess();

	return TRUE;
}

int CProcThread::ExitInstance()
{
	if(m_ImageProcSetDlg != NULL)
	{
		delete m_ImageProcSetDlg;
		m_ImageProcSetDlg = NULL;
	}

	if(m_IImageProcess != NULL)
	{
		delete m_IImageProcess;
	}
	
	if(m_IMotoCtrl != NULL)
	{
		m_IMotoCtrl->CloseUSB();
		m_IMotoCtrl->DeInit();

		delete m_IMotoCtrl;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcThread, CWinThread)
	ON_THREAD_MESSAGE(WM_MOTOR_CONNECT,&CProcThread::OnMotorConnect)
	ON_THREAD_MESSAGE(WM_MOTOR_CLEAR_ZERO_X,&CProcThread::OnMotorClearZeroX)
	ON_THREAD_MESSAGE(WM_MOTOR_CLEAR_ZERO_Y,&CProcThread::OnMotorClearZeroY)
	ON_THREAD_MESSAGE(WM_MOTOR_CLEAR_ZERO_Z,&CProcThread::OnMotorClearZeroZ)
	ON_THREAD_MESSAGE(WM_MOTOR_MANUAL_START_MOVE,&CProcThread::OnMotorManualStartMove)
	ON_THREAD_MESSAGE(WM_MOTOR_MANUAL_STOP_MOVE,&CProcThread::OnMotorManualStopMove)
	ON_THREAD_MESSAGE(WM_MOTOR_GET_STATUS,&CProcThread::OnMotorGetStatus)
	ON_THREAD_MESSAGE(WM_MOTOR_STOP,&CProcThread::OnMotorStop)
	ON_THREAD_MESSAGE(WM_MOTOR_MOVE_TO,&CProcThread::OnMotorMoveTo)
	ON_THREAD_MESSAGE(WM_DO_MANUAL_MEAR,&CProcThread::OnDoManualMear)
	ON_THREAD_MESSAGE(WM_DO_AUTO_MEAR,&CProcThread::OnDoAutoMear)
	ON_THREAD_MESSAGE(WM_IMAGE_PROC,&CProcThread::OnDoImageProc)
	ON_THREAD_MESSAGE(WM_IMAGE_LOAD,&CProcThread::OnDoImageLoad)
	ON_THREAD_MESSAGE(WM_IMAGE_PROC_SETTING,&CProcThread::OnImageProcSetting)
END_MESSAGE_MAP()

void CProcThread::OnMotorConnect(WPARAM wParam,LPARAM lParam)
{
	m_IsMotroCtrlConnected = false;
	if(NULL != m_IMotoCtrl)
	{
		m_IMotoCtrl->CloseUSB();

		INT32 iResult = m_IMotoCtrl->OpenUSB();
		if(0 != iResult)
		{
			AfxMessageBox("�򿪿��ƿ�USBʧ��!");
			return;
		}

		iResult = m_IMotoCtrl->Check();
		if(0 != iResult)
		{
			AfxMessageBox("�����ƿ�ʧ�ܣ�");
			return;
		}
		else
		{
			m_IsMotroCtrlConnected = true;
			AfxMessageBox("���ƿ����ӳɹ���");
		}
	}
}

void CProcThread::OnMotorClearZeroX(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisCurrPos(AXIS_X, 0.0);
	}
	else
	{
		AfxMessageBox("���ƿ�δ���ӣ�");
	}
}

void CProcThread::OnMotorClearZeroY(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Y, 0.0);
	}
	else
	{
		AfxMessageBox("���ƿ�δ���ӣ�");
	}
}

void CProcThread::OnMotorClearZeroZ(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Z, 0.0);
	}
	else
	{
		AfxMessageBox("���ƿ�δ���ӣ�");
	}
}

void CProcThread::OnMotorManualStartMove(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);
	int dir = int(lParam);

	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisVelocityStart(axis, dir);
	}
	else
	{
		AfxMessageBox("���ƿ�δ���ӣ�");
	}
}

void CProcThread::OnMotorManualStopMove(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);

	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisVelocityStop(axis);
	}
	else
	{
		AfxMessageBox("���ƿ�δ���ӣ�");
	}
}

void CProcThread::OnMotorGetStatus(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);
	int status = int(lParam);

	if(status == CURR_POS)
	{
		float iTempPos = 0.0;
		m_IMotoCtrl->GetAxisCurrPos(axis, &iTempPos);
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_MOTOR_UPDATE_STATUS, wParam, (LPARAM)iTempPos);
	}
}

void CProcThread::OnMotorStop(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);

	if(NULL != m_IMotoCtrl)
	{
		m_IMotoCtrl->SetAxisPositionStop(axis);
	}
}

void CProcThread::OnMotorMoveTo(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);
	float pos = float(lParam);

	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->MoveTo(axis, pos);
	}
	else
	{
		AfxMessageBox("���ƿ�δ���ӣ�");
	}
}

bool CProcThread::ConvertStringToFloat(CString buffer, float &value)
{
	if(buffer != "")
	{
		char *endptr;
		endptr = NULL;
		double d;
		d = strtod(buffer, &endptr);
		if (errno != 0 || (endptr != NULL && *endptr != '\0'))
		{
			return false;
		}
		else
		{
			value = (float)d;
			return true;
		}
	}
}

bool CProcThread::GetFloatItem(int row, int column, float &value)
{
	CString buffer=""; 
	if(NULL != m_pListData) buffer += m_pListData->GetItemText(row,column);
	return ConvertStringToFloat(buffer, value);
}

bool CProcThread::SetFloatItem(int row, int column, float value)
{
	CString buffer=""; 
	buffer.Format("%f", value);
	if(NULL != m_pListData)
	{
		m_pListData->SetItemText(row,column, buffer);
		return true;
	}
	else
	{
		return false;
	}
}

bool CProcThread::GetMeasureTargetValue(int row, float &x, float &y, float &z)
{
	const int XColumn = 2;
	const int YColumn = 3;
	const int ZColumn = 4;

	if(GetFloatItem(row, XColumn, x))
	{
		if(GetFloatItem(row, YColumn, y))
		{
			if(GetFloatItem(row, ZColumn, z))
			{
				return true;
			}
		}
	}

	return false;
}

bool CProcThread :: SetMeasureResultValue(int row, float resultX, float resultY, float resultZ)
{
	const int XResultColumn = 5;
	const int YResultColumn = 6;
	const int ZResultColumn = 7;

	SetFloatItem(row, XResultColumn, resultX);
	SetFloatItem(row, YResultColumn, resultY);
	SetFloatItem(row, ZResultColumn, resultZ);

	return true;
}

void CProcThread::OnDoAutoMear(WPARAM wParam,LPARAM lParam)
{
	const int StartRow = 4;
	m_pListData = (CListCtrl*)wParam;
	int usedRowNum = (int)lParam;

	CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
	if(detecter != NULL)
	{
		detecter->LoadConfig();
	}

	float x = 0.0, y = 0.0, z = 0.0;
	float retX = 0.0, retY = 0.0, retZ = 0.0;
	CString testNum;
	for(int i = StartRow; i < usedRowNum; i++)
	{
		if(GetMeasureTargetValue(i, x, y, z))
		{
		//	testNum = m_ListData.GetItemText(i, 0);
			if(0 == CalculatePoint(x, y, z, retX, retY, retZ))
			{
		//		CString log;
		//		log.Format(_T("Num %s, X=%f, Y=%f, Z=%f"),testNum, retX,  retY,  retZ);
		//		CLog::Instance()->Log(log);
				SetMeasureResultValue(i, retX, retY, retZ);
			}
		}
	}
}

int CProcThread::CalculatePoint(float x, float y, float z, float &retx, float &rety, float &retz)
{
	int ret = -1;

	ret = m_IMotoCtrl->MoveTo(AXIS_X, x);
	ret = m_IMotoCtrl->MoveTo(AXIS_Y, y);
	while(ret == 0)
	{
		if((m_IMotoCtrl->IsOnMoving(AXIS_X) == false) && (m_IMotoCtrl->IsOnMoving(AXIS_Y) == false))
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	//ͬ����Ϣ���ȴ����߳����ս��
	ret = ::SendMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_MAIN_THREAD_DO_CAPTURE, 0, 0);

	if(ret == 0)
	{
		m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
		ret = m_IImageProcess->Process(x, y, retx, rety);
	}
	//retx = x + 10;
	//rety = y + 10;

	ret = m_IMotoCtrl->MoveTo(AXIS_X, retx);
	ret = m_IMotoCtrl->MoveTo(AXIS_Y, rety);
	while(ret == 0)
	{
		if((m_IMotoCtrl->IsOnMoving(AXIS_X) ==  false) && (m_IMotoCtrl->IsOnMoving(AXIS_Y) == false))
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	//�ƶ�Z��
	ret = m_IMotoCtrl->MoveTo(AXIS_Z, z);
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_Z) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	//Z�������ƶ���ֱ���Ӵ���λ����ֹͣ
	//retz = z + 10;

	return ret;
}
void CProcThread::OnDoManualMear(WPARAM wParam,LPARAM lParam)
{
	float* pPos = (float*)wParam;
	float PosX = pPos[0];
	float PosY = pPos[1];
	float PosZ = pPos[2];
	float resPosX = 0.0;
	float resPosY = 0.0;
	float resPosZ = 0.0;

	if(0 == CalculatePoint(PosX, PosY, PosZ, resPosX, resPosY, resPosZ))
	{
		//success
	}
	else
	{
		//failed
	}
}

void CProcThread::OnDoImageProc(WPARAM wParam,LPARAM lParam)
{
	OpenHalconWindow();

	if((NULL != m_IImageProcess) && m_IImageProcess->LoadProcessImage())
	{
		CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
		if(detecter != NULL)
		{
			detecter->LoadConfig();
		}
		
		float x = 0.0;
		float y = 0.0;
		bool ret = m_IImageProcess->FindTargetPoint(x, y);
		if(!ret)
		{
			AfxMessageBox("Can not find target!");
		}
		else
		{
			CString msg;
			msg.Format("�������Ϊx=%f, y=%f.", x, y);
			AfxMessageBox(msg);
		}
	}
}

void CProcThread::OnDoImageLoad(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IImageProcess)
	{
		m_IImageProcess->LoadProcessImage();
	}
}

void CProcThread::OnImageProcSetting(WPARAM wParam,LPARAM lParam)
{
	OpenHalconWindow();
	
	if(NULL != m_IImageProcess && m_IImageProcess->LoadProcessImage())
	{
		if(m_ImageProcSetDlg == NULL)
		{
			m_ImageProcSetDlg = new CImageProcSettingDlg();
			m_ImageProcSetDlg->Create(CImageProcSettingDlg::IDD, GetMainWnd());
		}
		CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
		if(detecter != NULL)
		{
			m_ImageProcSetDlg->SetCircleDetecter(detecter);
			m_ImageProcSetDlg->ShowWindow(SW_SHOW);
		}
	}
}

void CProcThread::OpenHalconWindow()
{
	if(m_HalconWndOpened)
	{
		return;
	}

	Halcon::set_window_attr("background_color","black");

	Halcon::open_window(0, 0, 640, 480, 0, "","",&m_WindowHandle);

	HDevWindowStack::Push(m_WindowHandle);

	m_HalconWndOpened = true;
}

// CProcThread message handlers