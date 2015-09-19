#include "StdAfx.h"
#include "HalconAction.h"
#include "DetectCircularhole.h"
#include "DataUtility.h"


#define MINGRAY			_T("MinGray")
#define	MAXGRAY			_T("MaxGray")
#define	MINAREA			_T("MinArea")
#define	MAXAREA			_T("MaxArea")
#define	MINROUNDNESS	_T("MinRoundness")
#define	MAXROUNDNESS	_T("MaxRoundness")

#define STANDARDCIRLEGRAY			_T("Standard Circle Gray")
#define	STANDARDCIRLEAREA			_T("Standard Circle Area")
#define	STANDARDCIRLEROUNDNESS		_T("Standard Circle Roundness")

#define OBLONGCIRLEGRAY				_T("Oblong Circle Gray")
#define	OBLONGCIRLEAREA				_T("Oblong Circle Area")
#define	OBLONGCIRLEROUNDNESS		_T("Oblong Circle Roundness")

#define FIXTURECIRLEGRAY			_T("Fixture Circle Gray")
#define	FIXTURECIRLEAREA			_T("Fixture Circle Area")
#define	FIXTURECIRLEROUNDNESS		_T("Fixture Circle Roundness")

#define SPECIALCIRLEGRAY			_T("Special Circle Gray")
#define	SPECIALCIRLEAREA			_T("Special Circle Area")
#define	SPECIALCIRLEROUNDNESS		_T("Special Circle Roundness")


CDetectCircularhole::CDetectCircularhole()
	: m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(999999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_detectType(CIRCLE_DETECT_TYPE_NORMAL)
{
	
	m_ShowErrorMeg = true;
	m_ShowProcessingImage = true;

	m_ConfigPath = DataUtility::GetExePath() + _T("\\ProcessConfig\\ImageProcess.ini");
	LoadConfig();
}



CDetectCircularhole::~CDetectCircularhole(void)
{
}

void CDetectCircularhole::SetImageObject(Hobject image)
{
	ho_Capture1 = image;
	Halcon::get_image_size(ho_Capture1, &hv_imageWidth, &hv_imageHeight);
}

bool CDetectCircularhole::DetectCirleCenter(float &row, float &column)
{
	if(RunSelectTarget())
	{
		row = (float)(hv_targetRow[0].D());
		column = (float)(hv_targetColumn[0].D());
		return true;
	}
	return false;
}

void CDetectCircularhole::ShowErrorMessage(bool show)
{
	m_ShowErrorMeg = show;
}

void CDetectCircularhole::ShowMessage(CString msg)
{
	if(m_ShowErrorMeg)
	{
		AfxMessageBox(msg);
	}
}

bool CDetectCircularhole::RunThreshold()
{
	try
	{
		Halcon::get_image_size(ho_Capture1, &hv_imageWidth, &hv_imageHeight);
		threshold(ho_Capture1, &ho_Region, m_MinGray, m_MaxGray);
		if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
		{
			clear_window(HDevWindowStack::GetActive());
			set_color(HDevWindowStack::GetActive(),"red");
			disp_obj(ho_Region, HDevWindowStack::GetActive());
			connection(ho_Region, &ho_ConnectedRegions);

			//为了设定参数方便，显示面积在m_MinCirleArea/10~m_MaxCirleArea*10（Pixel）的区域面积值
			Hobject selectRegions;
			HTuple RegionCount, Area, Row, Column;
			select_shape(ho_ConnectedRegions, &selectRegions, "area", "and", m_MinCirleArea/10, m_MaxCirleArea*10);
			count_obj(selectRegions, &RegionCount);
			area_center(selectRegions, &Area, &Row, &Column);
			set_color(HDevWindowStack::GetActive(),"blue");
			for (int i=1; i<=RegionCount; i+=1)
			{
				set_tposition(HDevWindowStack::GetActive(), HTuple(Row[i-1]), HTuple(Column[i-1]));
				write_string(HDevWindowStack::GetActive(), Area[i-1].D());
			}
		}
	}
	catch(...)
	{
		ShowMessage("二值化失败！");
		return false;
	}
	return true;
}

bool CDetectCircularhole::RunSelectTarget()
{
	if(RunThreshold())
	{
		try
		{
			select_shape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("area").Append("roundness")), "and", (HTuple(m_MinCirleArea).Append(m_MinRoundness)), (HTuple(m_MaxCirleArea).Append(m_MaxRoundness)));
			
			fill_up(ho_SelectedRegions, &ho_RegionFillUp);
			gen_contour_region_xld(ho_RegionFillUp, &ho_Contour, "border");

			HTuple row1, column1, row2, column2, length;
			diameter_region(ho_RegionFillUp, &row1, &column1, &row2, &column2, &length);
			hv_targetRow = (row1 + row2) / 2;
			hv_targetColumn = (column1 + column2) / 2;

			if (m_ShowProcessingImage && HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"red");
				disp_obj(ho_Contour, HDevWindowStack::GetActive());

				set_color(HDevWindowStack::GetActive(),"red");
				disp_cross(HDevWindowStack::GetActive(), hv_targetRow, hv_targetColumn, 20, 0);
				set_color(HDevWindowStack::GetActive(),"green");
				disp_cross(HDevWindowStack::GetActive(), hv_imageHeight / 2, hv_imageWidth / 2, 20, 0);
			}
			return true;
		}
		catch(...)
		{
			ShowMessage("检测圆形区域失败！");
		}
	}
	return false;
}

void CDetectCircularhole::LoadConfig()
{
	switch(m_detectType)
	{
	case CIRCLE_DETECT_TYPE_NORMAL:
		m_MinGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_OBLONG:
		m_MinGray = DataUtility::GetProfileInt(OBLONGCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(OBLONGCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(OBLONGCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(OBLONGCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(OBLONGCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(OBLONGCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_FIXTURE:
		m_MinGray = DataUtility::GetProfileInt(FIXTURECIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(FIXTURECIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(FIXTURECIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(FIXTURECIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(FIXTURECIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(FIXTURECIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	case CIRCLE_DETECT_TYPE_SPECIAL:
		m_MinGray = DataUtility::GetProfileInt(SPECIALCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(SPECIALCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(SPECIALCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(SPECIALCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(SPECIALCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(SPECIALCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;

	default:
		m_MinGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, 0);
		m_MaxGray = DataUtility::GetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, 100);
		m_MinCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, 150000.0);
		m_MaxCirleArea = DataUtility::GetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, 300000.0);
		m_MinRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, 0.5);
		m_MaxRoundness = DataUtility::GetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, 1.0);
		break;
	}
}

void CDetectCircularhole::SaveConfig()
{
	switch(m_detectType)
	{
	case CIRCLE_DETECT_TYPE_NORMAL:
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_OBLONG:
		DataUtility::SetProfileInt(OBLONGCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(OBLONGCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(OBLONGCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(OBLONGCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(OBLONGCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(OBLONGCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_FIXTURE:
		DataUtility::SetProfileInt(FIXTURECIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(FIXTURECIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(FIXTURECIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(FIXTURECIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(FIXTURECIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(FIXTURECIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	case CIRCLE_DETECT_TYPE_SPECIAL:
		DataUtility::SetProfileInt(SPECIALCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(SPECIALCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(SPECIALCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(SPECIALCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(SPECIALCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(SPECIALCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;

	default:
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MINGRAY, m_ConfigPath, m_MinGray);
		DataUtility::SetProfileInt(STANDARDCIRLEGRAY, MAXGRAY, m_ConfigPath, m_MaxGray);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MINAREA, m_ConfigPath, m_MinCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEAREA, MAXAREA, m_ConfigPath, m_MaxCirleArea);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MINROUNDNESS, m_ConfigPath, m_MinRoundness);
		DataUtility::SetProfileFloat(STANDARDCIRLEROUNDNESS, MAXROUNDNESS, m_ConfigPath, m_MaxRoundness);
		break;
	}
}
