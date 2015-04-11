#ifndef _HQDLAPIUSER_H
#define _HQDLAPIUSER_H

#include "DataType.h"

#ifdef HQDLAPI_EXPORTS
#define HQDL_API extern "C" __declspec(dllexport) int WINAPI
#else
#define HQDL_API extern "C" __declspec(dllimport) int WINAPI
#endif


/*==============================================================
	�ص���������
  *-------------------------------------------------------------*/
typedef VOID (WINAPI *DL_AUTOCALLBACK )( DWORD   dw1, LPVOID lpContext );
typedef VOID (WINAPI *DL_FRAMECALLBACK)( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext );

/*==============================================================
    2005.12.13
	��˵��:	V1.0.0.1�������л�������ʵ���ҵ�����ͷ�ṩͳһ�ӿ�
			���ڹ�����ǿ���Golden Hawk�ṩ���пɲ����ӿ�
			���������򻯰�����ͷ��Щ�ӿڲ�֧��,
			�������White Hawk��֧��HqDLBitOperation
			���ںڰ�����ͷ��֧��HqDLSetDoAWB,����
			���ںڰ�����ͷ,��ԭʼ����Ԥ�����ɼ��Ǳ�׼�ķ�ʽ,���ڰ�����ͷҲ������
			��ɫ����ͷ�ķ�ʽ����.
    2005.12.19
	��˵��:	V1.0.0.3��������⺯��HqDLNoiseCalibration
			����λ�õ��ں���HqDLSizeCalibration
    2006.1.4
	��˵��:	V1.0.0.4������Ƶ��HqDLAutoFocus����Դ����HqDLLedControl
    2006.1.18
	��˵��:	V1.0.0.6����avi¼ӰHqDLCaptureToAvi���Զ���������HqDLSetDoAF

    2006.1.18
	��˵��:	V1.0.0.7�޸���avi¼ӰHqDLCaptureToAvi��������Ҫֹͣ¼Ӱʱ��
			¼Ӱ�ر�ʱ���ļ�������Ϊ��
			�޸���Ԥ�����ڲ��ܴ����Ĵ���
    2006.4.18
	��˵��:	V1.0.0.8������ֱ�ӱ���JPEGͼ����,HqDLGetRgbFrameToJpeg
    2006.7.13
	��˵��:	V1.0.0.9����������avi�ɼ�֡��������HqDLCaptureToAviEx
    2006.7.25
	��˵��:	V1.0.0.10�����˶Զ������ͷ�Ŀ��ƹ���
			HqDLGetTotalDeviceNum����ȡ��PC���ӵ�����ͷ��
			HqDLGetDeviceID���ڵõ�ָ������ͷ��ID�ţ���������������ͷ����

  ==============================================================*/


 /*==============================================================
	����:	��ʼ���豸����������ͷ������������������ĵ���
	����:

	����:	pIndex			��������ͷ����
			pFilterName		����
			pCapInfo		��ʼ������ͷ����   
	˵��:   ����Ҫ��pCapInfo->Buffer�����㹻���ڴ�ռ䣬һ��Ҫ��������Ĳɼ��ռ�Ҫ��һЩ
			�Ƽ���MAXWIDTH*MAXHEIGHT+512�����ڴ��ݲɼ�����
  --------------------------------------------------------------*/
HQDL_API 
HqDLInitialize(	IN  LPCTSTR			pFilterName,
				OUT int				*pIndex,	
				IN  CapInfoStruct	*pCapInfo,
				OUT HANDLE			*hCamera
				);	
/*==============================================================
	����:	HqDLStartView
	����:	��Ԥ������,��������Ƶ��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLStartView( IN HANDLE	hCamera, 
			  IN LPCTSTR	lpszWindowName, 
			  IN DWORD		dwStyle, 
			  IN int		x, 
			  IN int		y, 
			  IN int		nWidth, 
			  IN int		nHeight, 
			  IN HWND		hwndParent, 
			  IN HMENU		nIDorHMenu
			  );
/*==============================================================
	����:	HqDLPauseView
	����:	ֹͣ��Ƶ��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLStopView( IN HANDLE  hCamera );

/*==============================================================
	����:	HqDLPauseView
	����:	��ͣ��Ƶ��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLPauseView( IN HANDLE hCamera, IN BOOL bPause );


/*==============================================================
	����:	HqDLUninitialize
	����:	����ʼ���豸
	����:   
	˵��:   �����ڳ����˳�ʱ����,�����ͷ��ڴ����ռ�
  --------------------------------------------------------------*/
HQDL_API
HqDLUninitialize( IN HANDLE  *hCamera );


/*==============================================================
	����:	HqDLSetCapInfo
	����:	ʵʱ������Ƶ����
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetCapInfo( IN HANDLE hCamera , IN CapInfoStruct  *pCapInfo );

/*==============================================================
	����:	HqDLGetFrameRate
	����:	�õ�����ͷ�ĵ�ǰ֡��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLGetFrameRate( IN HANDLE	hCamera, OUT float *pfFrameRate );

/*==============================================================
	����:	HqDLGetOneFrame
	����:	�ɼ�һ֡ԭʼ���ݵ�pCapInfo->Buffer��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRawFrame(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo);

/*==============================================================
	����:	HqDLGetRgbFrame
	����:	�ɼ�һ֡24bitRGB���ݵ�pDest�У�ͬʱԭʼ���ݻᱣ����pCapInfo->Buffer��
			pDest�еĽ�������CAMPARAM�е��趨���ı䣬�������bClearView,pDest���Ǿ���
			�Ż���ת���㷨���õ�24bit�Ľ�����������bBW,��ô��ת��Ϊ8bit�ĻҶ�ͼ������
			�������UPDOWN��ͼ�����ݻ�����Ӧ�Ĵ�ֱ��ת��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRgbFrame(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest);

/*==============================================================
	����:	HqDLGetPausedFrame
	����:	���ݵ�ǰ��ͣ����Ƶ���õ���ͣ����Ƶ����
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetPausedFrame(IN HANDLE hCamera, OUT BYTE* pDest);

/*==============================================================
	����:	HqDLGetPausedFrameToBmp
	����:	����ǰ��ͣ��Ƶ����ΪBMP�ļ�
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetPausedFrameToBmp(IN HANDLE hCamera, OUT BYTE* pDest, LPCTSTR strFileName );



/*==============================================================
	����:	HqDLGetRawFrameToBmp
	����:	�ɼ�һ֡ԭʼ���ݵ�pCapInfo->Buffer��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRawFrameToBmp(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, LPCTSTR  strFileName );

/*==============================================================
	����:	HqDLGetRgbFrameToBmp
	����:	�ɼ�һ֡ͼ���ļ����ļ���ʽ�����CAMPARAM�е��趨�ı䡣
			�������bClearView,pDest���Ǿ����Ż���ת���㷨���õ�24bit�Ľ����
			�������bBW,��ô��ת��Ϊ8bit�ĻҶ�ͼ������
			�������UPDOWN��ͼ�����ݻ�����Ӧ�Ĵ�ֱ��ת��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRgbFrameToBmp( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName ); 


/*==============================================================
	����:	HqDLGetRgbFrameToJpeg
	����:	�ɼ�һ֡ͼ��Jpeg�ļ����ļ���ʽ�����CAMPARAM�е��趨�ı䡣
			�������bClearView,pDest���Ǿ����Ż���ת���㷨���õ�24bit�Ľ����
			�������bBW,��ô��ת��Ϊ8bit�ĻҶ�ͼ������
			�������UPDOWN��ͼ�����ݻ�����Ӧ�Ĵ�ֱ��ת��

	����:   nQuality Ϊѹ������ 1 - 100
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetRgbFrameToJpeg( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName, int nQuality ); 

/*==============================================================
	����:	HqDLSetParam
	����:	����ͼ�����
	����:   DLParam�������£�
					BWSHOW = 0,		// �Ƿ�ڰ���ʾ
					RAWDATASHOW,	// �Ƿ�ֱ����ʾԭʼ����
					CLEARVIEW,		// �Ƿ������Ż��㷨����ת��
					PAUSE,			// �Ƿ���ͣ��Ƶ
					COLOROFFSET,	// �Ƿ������ɫƫ�Ƶ���
					UPDOWN,			// ���·�ת
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetParam(IN HANDLE hCamera, DLPARAM  DLParam, ULONG  Value );


/*==============================================================
	����:	HqDLSetGammaValue
	����:	����GammaУ������
	����:   fGammaValue �Ƽ���Χ 1 - 200
	˵��:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetGammaValue( IN HANDLE hCamera, IN float fGammaValue, IN BOOL bGammaOn);

/*==============================================================
	����:	HqDLSetFrameCallback
	����:	���������Ԥ�������ϵ���Ļص���lpParamΪ�û�������
	����:   pfFrameCallback( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext )
			lpParam1 - ָ����ʾ����ָ��
			lpPoint  - �����ͼ��λ��
			lpContext- ������
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetFrameCallback( IN HANDLE hCamera, DL_FRAMECALLBACK pfFrameCallback, LPVOID lpContext );

/*==============================================================
	����:	HqDLEnableColorOffset
	����:	������ɫƫ�����ĵ���
	����:	nRedOffset    -255 ~ 255   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLEnableColorOffset( IN HANDLE hCamera, IN int nRedOffset, IN int nGreenOffset, IN int nBlueOffset, IN BOOL bColorOffset );


/*==============================================================
	����:	HqDLBitOperation
	����:	����Golden Hawk���������
	����:	����,���ô˺���ʱ������������Ƶ
	˵��:  
  --------------------------------------------------------------*/
HQDL_API 
HqDLBitOperation( IN HANDLE hCamera, IN BYTE Out, OUT BYTE *pIn, IN UCHAR Command );


/*==============================================================
	����:	HqDLBitOperationX
	����:	IO�˿ڲ���
	����:	
			Out[15:8] Ϊ����IO�˿ڵķ��� 0-INPUT 1-OUTPUT
			Out[7:0]  Ϊ�����ֵ
			pInΪIO�ڵ�״̬

	˵��:   ����Ƶ��������в�Ҫ����
  --------------------------------------------------------------*/
HQDL_API 
HqDLBitOperationX( IN HANDLE hCamera, IN USHORT Out, OUT BYTE *pIn );


/*==============================================================
	����:	HqDLRegOperation
	����:	����
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLRegOperation( IN HANDLE hCamera, IN BYTE RegAddress, IN OUT USHORT* Value, IN BOOL bWrite);


/*==============================================================
	����:	
	����:	
	����:   pfAWBCallback ָ��AWB������Ļص�����ָ��
			VOID (WINAPI *DL_AUTOCALLBACK )( DWORD dw1, LPVOID lpContext );
			lpParam1	��ָ��AWB�����ָ�룬ͨ��lpParam1�����AWB������£�
			ULONG   Result = dw1;				// 0xFF202020
			BYTE btRed		= ( BYTE )(Result);				// 0x20
			BYTE btGreen	= ( BYTE )( Result>>8 );		// 0x20
			BYTE btBlue		= ( BYTE )( Result>>16 );		// 0x20
			ULONG Exposure  = ( BYTE )( Result>>24 ) * 2;	// 255 * 2 = 510 ms
			
	˵��:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetDoAWB( IN HANDLE hCamera, IN BOOL bAWB, IN BYTE btTarget,
			  IN DL_AUTOCALLBACK pfAWBCallback,	 IN LPVOID lpContext );

/*==============================================================
	����:	HqDLSetDoAE
	����:	��ʼ�Զ��ع����
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetDoAE( IN HANDLE hCamera, IN BOOL bAE, IN BYTE btTarget, 
			 IN DL_AUTOCALLBACK pfAECallback, IN LPVOID lpContext );

/*==============================================================
	����:	HqDLSetDoAF
	����:	��ʼ�Զ��۽�
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API 
HqDLSetDoAF( IN HANDLE hCamera, IN BOOL bAF );


/*==============================================================
	����:	HqDLSetViewWin
	����:	����Ԥ�����ڵ�λ��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLSetViewWin(IN HANDLE hCamera, IN DLVIDEORECT *pRect);


/*==============================================================
	����:	HqDLGetViewWin
	����:	�õ�Ԥ�����ڵ�λ��
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetViewWin(IN HANDLE hCamera, IN DLVIDEORECT *pRect);


/*==============================================================
	����:	HqDLGetTotalDeviceNum
	����:	�õ����м�������ͷ
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetTotalDeviceNum(IN HANDLE hCamera, IN int *pDeviceNum );

/*--------------------------------------------------------------
	����:	HqDLGetDeviceID
	����:	�õ�������ͷ��ID�ţ����ڶ�ָ������ͷ���в���
	����:   
	˵��:   ��Ҫ����Multi����
	
  =============================================================*/
HQDL_API
HqDLGetDeviceID(IN HANDLE hCamera, IN USHORT *pDeviceID );


/*==============================================================
	����:	HqDLGetSerialsNum
	����:	�õ�����ͷ���к�
	����:   
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLGetSerialsNum(IN HANDLE hCamera, IN char *SerialsNum );


/*==============================================================
	����:	HqDLSizeCalibration
	����:	ר�Ŷ��ڲɼ�ͼ��λ�õ�У����������洢��Ӳ��֮��
	����:   bWrite    = TRUE Ϊд����   = FALSE Ϊ������
			��Ϊ*pOffH, *pOffV, *pWidth, *pHeight��8�ֽڸ�ֵ��������д������
			���������ɹ����غ���8���ֽڻ���д��Ӳ���洢���У���ʱ����ͨ�����������ж�ȡ
			����ȡʱ������bWrite = FALSE, ���������ɹ����غ�*pOffH, *pOffV, *pWidth, *pHeight
			�����ܶ�ȡ����8���ֽ�
	˵��:   ���ô˺���ʱ����Ҫ����Ԥ����Ƶ��
  --------------------------------------------------------------*/
HQDL_API
HqDLSizeCalibration(IN HANDLE hCamera, IN OUT USHORT *pOffH, IN OUT USHORT *pOffV,
				IN OUT USHORT  *pWidth, IN OUT USHORT *pHeight, BOOL bWrite );

/*==============================================================
	����:   HqDLNoiseCalibration
	����:	����У��ͼ����Ҫ����ȥ����㡣
	����:	bPreview   ָ���Ƿ���Ԥ��ʱ����ȥ�빦��
			bSave      ָ���Ƿ��ڱ���ʱ����ȥ�빦��
			FileName   ����У��ͼ����ļ���,�ǲɼ�ԭʼ���ݵõ���256ɫbmp�Ҷ�ͼ,
			��FileName = ""ʱ,����ֻ���ü���ȥ���ʱ��
	˵��:   ����������Ƶ��ʱ����
	ע��:   ����ͼ��Ĵ�С��������Ƶ�ɼ���С���,���򲻻����
  --------------------------------------------------------------*/
HQDL_API 
HqDLNoiseCalibration( IN HANDLE hCamera, const char*  FileName, BOOL bPreview, BOOL bSave );


/*------------------------------------------------------------------------------------------
	������  HqDLBrightnessCalibration
	���ܣ�	���Ⱦ���

  =========================================================================================*/
HQDL_API 
HqDLBrightnessCalibration( IN HANDLE hCamera, const char*  FileName, BOOL bPreview, BOOL bSave );


/*==============================================================
	����:	HqDLAutoFocus 
	����:	���о۽�����
	����:   Step ���һ���ƶ��Ĳ�����(1~255) Խ�󲽽�Խ��
			bFocusNear TRUE ���� FALSE ��Զ
	˵��:   GoldenEagle ����
  --------------------------------------------------------------*/
HQDL_API 
HqDLAutoFocus( IN HANDLE hCamera, USHORT  Step, BOOL bFocusNear );

/*==============================================================
	����:	HqDLLedControl
	����:	���й�Դ����
	����:   Value (0~10)
	˵��:   GoldenEagle ����
  --------------------------------------------------------------*/
HQDL_API 
HqDLLedControl( IN HANDLE hCamera, USHORT  Value );

/*==============================================================
	����:	HqDLCaptureToAvi
	����:	AVI¼Ӱ
	����:   bStart = TRUE ����¼Ӱ FALSE ֹͣ¼Ӱ
	˵��:   
  --------------------------------------------------------------*/
HQDL_API
HqDLCaptureToAvi( IN HANDLE hCamera, BOOL bStart, LPCTSTR  strFileName );

/*==============================================================
	����:	HqDLCaptureX
	����:	ѹ��¼Ӱ
	����:   bStart = TRUE ����¼Ӱ FALSE ֹͣ¼Ӱ
	˵��:   
			strFileName Ϊ¼Ӱ�����ļ���·���������
			dwRate ��������¼Ӱ֡��
	
			ֹͣ¼Ӱ����HqDLCaptureX(m_hDevice, FALSE, "", 0 );
  -------------------------------------------------------------*/
HQDL_API
HqDLCaptureX( IN HANDLE hCamera, BOOL bStart, LPCTSTR  strFileName, DWORD dwRate );


HQDL_API
HqDLCaptureToAviEx( IN HANDLE hCamera, IN BOOL bStart, IN LPCTSTR  strFileName, IN DWORD dwRate );


#endif