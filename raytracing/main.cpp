#include"MainWindow.h"
#include"vector3D.h"
#include"Ray3.h"
#include"color.h"
int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
	)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	//vector3D<float> v1(2.0, 3.0,4.0);
	//vector3D<float> v2(3.0, 5.0,7.0);
	//vector3D<float> v3;
	//v3 = v1 + v2;
	//v3 = v1*v2;
	//v3 = 2.0f*v2;
	//v3 = v2*2.0f;
	//v3 = v2 / 1.5f;
	//v3 =v1- v2;
	//auto temp = v3.length();
	//temp = v3.sqrLength();
	//v3=v2.normalize();
	//v3=v2.negte();
	//v3 = cross(v1, v2);
	//Color<float> c1(1, 1, 1);
	//Color<float> c2(3, 4, 5);
	//Color<float> c3;
	//c3 = c1 + c2;

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			MainWindow app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}