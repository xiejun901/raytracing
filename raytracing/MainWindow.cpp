#include"MainWindow.h"
#include"vector3D.h"
#include"Ray3.h"
#include"SceneObject.h"
MainWindow::MainWindow() :
m_hwnd(0),
m_pDirect2dFactory(nullptr),
m_pRenderTarget(nullptr),
m_pLightSlateGrayBrush(nullptr),
m_pCornflowerBlueBrush(nullptr),
m_pBitmap(nullptr)
{
}
MainWindow::~MainWindow()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
	SafeRelease(&m_pBitmap);
}
void MainWindow::RunMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT MainWindow::Initialize()
{
	HRESULT hr;
	hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MainWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";
		RegisterClassEx(&wcex);

		FLOAT dpiX, dpiY;
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		m_hwnd = CreateWindow(
			L"D2DDemoApp",
			L"Direct2D Demo App",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
			);

		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ZeroMemory(&m_DwmTimingInfo, sizeof(m_DwmTimingInfo));
			m_DwmTimingInfo.cbSize = sizeof(m_DwmTimingInfo);

			// Get the composition refresh rate. If the DWM isn't running,
			// get the refresh rate from GDI -- probably going to be 60Hz
			if (FAILED(DwmGetCompositionTimingInfo(NULL, &m_DwmTimingInfo)))
			{
				HDC hdc = GetDC(m_hwnd);
				m_DwmTimingInfo.rateCompose.uiDenominator = 1;
				m_DwmTimingInfo.rateCompose.uiNumerator = GetDeviceCaps(hdc, VREFRESH);
				ReleaseDC(m_hwnd, hdc);
			}
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}
	return hr;
}

HRESULT MainWindow::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	return hr;
}

HRESULT MainWindow::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
			);
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush);
		}
	}
	return hr;
}
void MainWindow::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}
LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		MainWindow *pMainWindow = (MainWindow *)pcs->lpCreateParams;
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pMainWindow));
		result = 1;
	}
	else
	{
		MainWindow *pMainWindow = reinterpret_cast<MainWindow *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandle = false;
		if (pMainWindow)
		{
			switch (message)
			{
			case WM_SIZE:
			{
							UINT width = LOWORD(lParam);
							UINT height = HIWORD(lParam);
							pMainWindow->OnResize(width, height);
			}
				result = 0;
				wasHandle = true;
				break;
			case WM_DISPLAYCHANGE:
				////{
				////	InvalidateRect(hwnd, nullptr, false);
				////}
				////result = 0;
				////wasHandle = true;
				////break;
			case WM_PAINT:
			{
							 PAINTSTRUCT ps;
							 BeginPaint(hwnd, &ps);
							 pMainWindow->OnRender();
							 EndPaint(hwnd, &ps);
							 //ValidateRect(hwnd, nullptr);
			}
				result = 0;
				wasHandle = true;
				break;
			case WM_DESTROY:
			{
							   PostQuitMessage(0);
			}
				result = 0;
				wasHandle = true;
				break;
			}
		}
		if (!wasHandle)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return result;
}

HRESULT MainWindow::OnRender()
{
	HRESULT hr = S_OK;
	//创建设备依赖资源，包含一个呈现器，两个SolidBrush;
	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		//创建了一个单位矩阵传SetTranform()方法里去，转换呈现器？没懂。。。应该是创建个用于旋转的矩阵就能旋转，单位矩阵应该就是不变吧
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		//检测呈现器的绘制区域大小
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		DrawBitmap();
		hr = m_pRenderTarget->EndDraw();

	}
	InvalidateRect(m_hwnd, nullptr, false);
	return hr;
}
void MainWindow::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
}

void MainWindow::DrawBitmap()
{
	HRESULT hr = S_OK;
	generateBitmap();
	//for (int i = 0; i < 256; i++)
	//for (int j = 0; j < 256; j++)
	//	m_pix[i][j] =j<<8;
	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
		);
	D2D1_BITMAP_PROPERTIES props;
	props.pixelFormat = pixelFormat;
	props.dpiX = 96;
	props.dpiY = 96;
	hr = m_pRenderTarget->CreateBitmap(D2D1::SizeU(256, 256), static_cast<void *>(m_pix), static_cast<UINT32>(1024), props, &m_pBitmap);
	D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	D2D1_SIZE_F size = m_pBitmap->GetSize();

	// Draw a bitmap in the upper-left corner of the window.
	m_pRenderTarget->DrawBitmap(
		m_pBitmap,
		D2D1::RectF(0.0f, 0.0f, size.width, size.height)
		);
	//rtSize.height / 2;
	//rtSize.width / 2;
	//m_pRenderTarget->DrawBitmap(
	//	m_pBitmap,
	//	D2D1::RectF(rtSize.width / 2 - size.width / 2, rtSize.height / 2 - size.height / 2, rtSize.width / 2 + size.width / 2, rtSize.height / 2 -+ size.height / 2)
	//	);

}


void MainWindow::generateBitmap()
{
	//Sphere scene(vector3D<float>(0, 10, -10),10);
	//Sphere scene2(vector3D<float>(0, 10, 10), 10);
	//SphereObject scene3(vector3D<float>(-10, 10, -10), 10);
	//PerspectiveCamera camera(vector3D<float>(0, 5, 15), vector3D<float>(0, 0, -1), vector3D<float>(0, 1, 0), 90);
	//PerspectiveCamera camera1(vector3D<float>(0, 10, 10), vector3D<float>(0, 0, -1), vector3D<float>(0, 1, 0), 75);
	//PlaneObject scene4(vector3D<float>(0, 1, 0), 0);
	//BaseObject pbo = &scene4;
	//int maxDepth = 20;
	//renderDepth(scene, camera1, maxDepth);
	//renderDepth(scene2, camera, maxDepth);
	//renderNormal(scene, camera);
	//renderRayTrace(scene4, camera);
	std::shared_ptr<SphereObject> sphere1(new SphereObject(vector3D<float>(-10, 10, -10), 10));
	std::shared_ptr<SphereObject> sphere2(new SphereObject(vector3D<float>(10, 10, -10), 10));
	std::shared_ptr<PlaneObject> plane(new PlaneObject(vector3D<float>(0, 1, 0), 0));

	sphere1->m_material = std::shared_ptr<Material>(new PhongMaterial(Color<float>(1.0f, 0.0f, 0.0f), Color<float>(1, 1, 1), 16,0.15));
	sphere2->m_material = std::shared_ptr<Material>(new PhongMaterial(Color<float>(0.0f, 0.0f,1.0f), Color<float>(1, 1, 1), 16,0.15));
	plane->m_material = std::shared_ptr<Material>(new CheckerMaterial(0.1f,0.5f));
	UnionObject uo;
	uo.insert(sphere1);
	uo.insert(sphere2);
	uo.insert(plane);
	PerspectiveCamera camera(vector3D<float>(0, 5, 15), vector3D<float>(0, 0, -1), vector3D<float>(0, 1, 0), 90);
	//renderUnion(uo, camera);
	renderRayTraceReflection(uo, camera,5);
}
//渲染深度
void MainWindow::renderDepth(Sphere scene, PerspectiveCamera camera, int maxDepth)
{
	camera.initialize();
	for (int y = 0; y < 256; y++)
	{
		auto sy = 1.0f - y / 256.0f;
		for (int x = 0; x < 256; x++)
		{
			auto sx = x / 256.0f;
			auto ray = camera.generateRay(sx, sy);
			auto result = scene.intersect(ray);
			if (result.geometry != 0)
			{
				auto depth = 255 - min(result.distance / maxDepth * 255, 255);
				//setColor(x, y, static_cast<int>((result.normal.x + 1) * 128), static_cast<int>((result.normal.y + 1) * 128), static_cast<int>((result.normal.z + 1) * 128), 0);
				setColor(x, y, static_cast<int>(depth),0 /*static_cast<int>(depth)*/,0 /*static_cast<int>(depth)*/, 0);
			}
			else
			{
			//	setColor(x, y, 255, 255, 255, 0);
			}
		}
	}
}

//渲染法向量
void MainWindow::renderNormal(Sphere scene, PerspectiveCamera camera)
{
	camera.initialize();
	for (int y = 0; y < 256; y++)
	{
		auto sy = 1.0f - y / 256.0f;
		for (int x = 0; x < 256; x++)
		{
			auto sx = x / 256.0f;
			auto ray = camera.generateRay(sx, sy);
			auto result = scene.intersect(ray);
			if (result.geometry != 0)
			{
				setColor(x, y, static_cast<int>((result.normal.x+ 1) * 128), static_cast<int>((result.normal.y + 1) * 128), static_cast<int>((result.normal.z + 1) * 128), 0);
			}
			else
			{
				setColor(x, y, 255, 255, 255, 0);
			}
		}
	}
}

void MainWindow::renderRayTrace(BaseObject &scene, PerspectiveCamera camera)
{
	camera.initialize();
	for (int y = 0; y < 256; y++)
	{
		auto sy = 1.0f - y / 256.0f;
		for (int x = 0; x < 256; x++)
		{
			auto sx = x / 256.0f;
			auto ray = camera.generateRay(sx, sy);
			auto result = scene.intersect(ray);
			if (result.geometry != 0)
			{
				auto color = result.m_material->sample(ray,result.position,result.normal);
				if (color.red>1)
					color.red = 1;
				if (color.green>1)
					color.green = 1;
				if (color.blue>1)
					color.blue = 1;
				setColor(x, y, static_cast<int>(color.red * 255), static_cast<int>(color.green * 255), static_cast<int>(color.blue * 255), 0);
			}
			
		}
	}
}

void MainWindow::renderUnion(UnionObject &scene, PerspectiveCamera camera)
{
	camera.initialize();
	for (int y = 0; y < 256; y++)
	{
		auto sy = 1.0f - y / 256.0f;
		for (int x = 0; x < 256; x++)
		{
			auto sx = x / 256.0f;
			auto ray = camera.generateRay(sx, sy);
			auto result = scene.intersect(ray);
			if (result.geometry != 0)
			{
				auto color = result.m_material->sample(ray, result.position, result.normal);
				if (color.red>1)
					color.red = 1;
				if (color.green>1)
					color.green = 1;
				if (color.blue>1)
					color.blue = 1;
				setColor(x, y, static_cast<int>(color.red * 255), static_cast<int>(color.green * 255), static_cast<int>(color.blue * 255), 0);
			}

		}
	}
}


void MainWindow::renderRayTraceReflection(UnionObject &scene, PerspectiveCamera camera,int maxReflect)
{
	camera.initialize();
	for (int y = 0; y < 256; y++)
	{
		auto sy = 1.0f - y / 256.0f;
		for (int x = 0; x < 256; x++)
		{
			auto sx = x / 256.0f;
			auto ray = camera.generateRay(sx, sy);
		//	auto result = rayTraceRecursive(scene,ray,3);
			auto color = rayTraceRecursive(scene, ray, maxReflect);;
			if (color.red>1)
				color.red = 1;
			if (color.green>1)
				color.green = 1;
			if (color.blue>1)
				color.blue = 1;
			setColor(x, y, static_cast<int>(color.red * 255), static_cast<int>(color.green * 255), static_cast<int>(color.blue * 255), 0);

		}
	}
}