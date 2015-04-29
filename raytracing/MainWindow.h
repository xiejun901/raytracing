#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_
#pragma once

#include<windows.h>

#include<stdlib.h>
#include<math.h>
#include<vector>

#include<d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>

#include"SceneObject.h"

template<class Interface>
inline void SafeRelease(Interface ** pp)
{
	if (*pp != nullptr)
	{
		(*pp)->Release();
		*pp = nullptr;
	}
}
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

class Sphere;
class PerspectiveCamera;
class MainWindow
{
public:
	MainWindow();
	~MainWindow();
	HRESULT Initialize();//��ʼ��������ֵΪ��ʼ��״̬���Ƿ�ɹ�֮��ģ�
	void RunMessageLoop();
private:
	HRESULT CreateDeviceIndependentResources();//
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	virtual HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	/***************************************************************************************************************************************
	*CALLBACK: �����˺�������Ϊ�ص�������
	*1  �ص����������йؼ��� CALLBACK���ص��������������ȫ�ֺ������߾�̬��������Ҫʹ����ĳ�Ա����(Ҳ����˵ Ҫʹ��ȫ�ֺ���) ��Ϊcallback�������ڳ�Ա����ǰʹ��static,Ҳ�����ں���ǰ����static���δʣ����������ص�������
	*2  �ص����������ɿ�����ֱ�ӵ���ִ��(ֻ��ʹ��ϵͳ�ӿ�API������Ϊ���)
	*3  �ص�����ͨ����Ϊ�������ݸ�ϵͳAPI���ɸ�API������
	*4  �ص��������ܱ�ϵͳAPI����һ�Σ�Ҳ���ܱ�ѭ�����ö��
	*/
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void OnTimer();
	void DrawBitmap();
	void generateBitmap();
	inline void setColor(int x, int y, int red, int green, int blue,int alpha)
	{
		m_pix[y][x] = (red<<16) | (green<<8) | (blue)|(alpha<<24);
	}
	void renderDepth(Sphere scene, PerspectiveCamera camera, int maxDepth);
	void renderNormal(Sphere scene, PerspectiveCamera camera);
	void renderRayTrace(BaseObject &scene, PerspectiveCamera camera);
	void renderUnion(UnionObject &scene, PerspectiveCamera camera);
private:
	HWND m_hwnd;
	ID2D1Factory *m_pDirect2dFactory;//
	ID2D1HwndRenderTarget *m_pRenderTarget;
	ID2D1SolidColorBrush *m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush *m_pCornflowerBlueBrush;
	DWM_TIMING_INFO m_DwmTimingInfo;
	ID2D1Bitmap *m_pBitmap;
	int m_pix[256][256];
	int clocktime;
};

#endif