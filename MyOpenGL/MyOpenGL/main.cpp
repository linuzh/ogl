// Vlo�en� knihoven
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"glaux.lib")

#include <windows.h>// Hlavi�kov� soubor pro Windows
#include <gl\gl.h>// Hlavi�kov� soubor pro OpenGL32 knihovnu
#include <gl\glu.h>// Hlavi�kov� soubor pro Glu32 knihovnu
//#include <gl\glaux.h>// Hlavi�kov� soubor pro Glaux knihovnu

HDC hDC = NULL;// Priv�tn� GDI Device Context
HGLRC hRC = NULL;// Trval� Rendering Context
HWND hWnd = NULL;// Obsahuje Handle na�eho okna
HINSTANCE hInstance;// Obsahuje instanci aplikace

bool keys[256];// Pole pro ukl�d�n� vstupu z kl�vesnice
bool active = TRUE;// Ponese informaci o tom, zda je okno aktivn�
bool fullscreen = TRUE;// Ponese informaci o tom, zda je program ve fullscreenu
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);// Deklarace procedury okna (funk�n� prototyp)

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)// Zm�na velikosti a inicializace OpenGL okna
{
	if (height == 0)// Zabezpe�en� proti d�len� nulou
	{
		height = 1;// Nastav� v��ku na jedna
	}
	glViewport(0, 0, width, height);// Resetuje aktu�ln� nastaven�
	glMatrixMode(GL_PROJECTION);// Zvol� projek�n� matici
	glLoadIdentity();// Reset matice
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);// V�po�et perspektivy
	glMatrixMode(GL_MODELVIEW);// Zvol� matici Modelview
	glLoadIdentity();// Reset matice
}

int InitGL(GLvoid)// V�echno nastaven� OpenGL
{
	glShadeModel(GL_SMOOTH);// Povol� jemn� st�nov�n�
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);// �ern� pozad�
	glClearDepth(1.0f);// Nastaven� hloubkov�ho bufferu
	glEnable(GL_DEPTH_TEST);// Povol� hloubkov� testov�n�
	glDepthFunc(GL_LEQUAL);// Typ hloubkov�ho testov�n�
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Nejlep�� perspektivn� korekce
	return TRUE;// Inicializace prob�hla v po��dku
}

int DrawGLScene(GLvoid)// Vykreslov�n�

{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Sma�e obrazovku a hloubkov� buffer

	glLoadIdentity();// Reset matice

	// Sem m��ete kreslit

	return TRUE;// Vykreslen� prob�hlo v po��dku

}

GLvoid KillGLWindow(GLvoid)// Zav�r�n� okna

{
	if (fullscreen)// Jsme ve fullscreenu?

	{
		ChangeDisplaySettings(NULL, 0);// P�epnut� do syst�mu

		ShowCursor(TRUE);// Zobraz� kurzor my�i

	}
	if (hRC)// M�me rendering kontext?

	{
		if (!wglMakeCurrent(NULL, NULL))// Jsme schopni odd�lit kontexty?

		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		}
		if (!wglDeleteContext(hRC))// Jsme schopni smazat RC?

		{

			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		}

		hRC = NULL;// Nastav� hRC na NULL

	}
	if (hDC && !ReleaseDC(hWnd, hDC))// Jsme schopni uvolnit DC

	{

		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hDC = NULL;// Nastav� hDC na NULL

	}
	if (hWnd && !DestroyWindow(hWnd))// Jsme schopni odstranit okno?

	{

		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hWnd = NULL;// Nastav� hWnd na NULL

	}
	if (!UnregisterClass("OpenGL", hInstance))// Jsme schopni odregistrovat t��du okna?

	{

		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hInstance = NULL;// Nastav� hInstance na NULL

	}

}
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)

{
	GLuint PixelFormat;// Ukl�d� form�t pixel�
	WNDCLASS wc;// Struktura Windows Class
	DWORD dwExStyle;// Roz���en� styl okna

	DWORD dwStyle;// Styl okna
	RECT WindowRect;// Obd�ln�k okna

	WindowRect.left = (long)0;// Nastav� lev� okraj na nulu

	WindowRect.right = (long)width;// Nastav� prav� okraj na zadanou hodnotu

	WindowRect.top = (long)0;// Nastav� horn� okraj na nulu

	WindowRect.bottom = (long)height;// Nastav� spodn� okraj na zadanou hodnotu
	fullscreen = fullscreenflag;// Nastav� prom�nnou fullscreen na spr�vnou hodnotu
	hInstance = GetModuleHandle(NULL);// Z�sk� instanci okna

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// P�ekreslen� p�i zm�n� velikosti a vlastn� DC

	wc.lpfnWndProc = (WNDPROC)WndProc;// Definuje proceduru okna

	wc.cbClsExtra = 0;// ��dn� extra data

	wc.cbWndExtra = 0;// ��dn� extra data

	wc.hInstance = hInstance;// Instance

	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);// Standardn� ikona

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);// Standardn� kurzor my�i

	wc.hbrBackground = NULL;// Pozad� nen� nutn�

	wc.lpszMenuName = NULL;// Nechceme menu

	wc.lpszClassName = "OpenGL";// Jm�no t��dy okna
	if (!RegisterClass(&wc))// Registruje t��du okna

	{

		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// P�i chyb� vr�t� false

	}
	if (fullscreen)// Budeme ve fullscreenu?

	{
		DEVMODE dmScreenSettings;// M�d za��zen�

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));// Vynulov�n� pam�ti

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);// Velikost struktury Devmode

		dmScreenSettings.dmPelsWidth = width;// ���ka okna

		dmScreenSettings.dmPelsHeight = height;// V��ka okna

		dmScreenSettings.dmBitsPerPel = bits;// Barevn� hloubka

		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Pokus� se pou��t pr�v� definovan� nastaven�

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)

		{
			// Nejde-li fullscreen, m��e u�ivatel spustit program v okn� nebo ho opustit

			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)

			{
				fullscreen = FALSE;// B�h v okn�

			}

			else

			{
				// Zobraz� u�ivateli zpr�vu, �e program bude ukon�en

				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);

				return FALSE;// Vr�t� FALSE

			}

		}

	}
	if (fullscreen)// Jsme st�le ve fullscreenu?

	{
		dwExStyle = WS_EX_APPWINDOW;// Roz���en� styl okna

		dwStyle = WS_POPUP;// Styl okna

		ShowCursor(FALSE);// Skryje kurzor

	}

	else

	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;// Roz���en� styl okna

		dwStyle = WS_OVERLAPPEDWINDOW;// Styl okna

	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);// P�izp�soben� velikosti okna
	// Vytvo�en� okna

	if (!(hWnd = CreateWindowEx(dwExStyle,// Roz���en� styl

		"OpenGL",// Jm�no t��dy

		title,// Titulek

		dwStyle |// Definovan� styl

		WS_CLIPSIBLINGS |// Po�adovan� styl

		WS_CLIPCHILDREN,// Po�adovan� styl

		0, 0,// Pozice

		WindowRect.right - WindowRect.left,// V�po�et ���ky

		WindowRect.bottom - WindowRect.top,// V�po�et v��ky

		NULL,// ��dn� rodi�ovsk� okno

		NULL,// Bez menu

		hInstance,// Instance

		NULL)))// Nep�edat nic do WM_CREATE
	{

		KillGLWindow();// Zru�� okno

		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Vr�t� chybu

	}
	static PIXELFORMATDESCRIPTOR pfd =// Ozn�m�me Windows jak chceme v�e nastavit

	{

	sizeof(PIXELFORMATDESCRIPTOR),// Velikost struktury

	1,// ��slo verze

	PFD_DRAW_TO_WINDOW |// Podpora okna

	PFD_SUPPORT_OPENGL |// Podpora OpenGL

	PFD_DOUBLEBUFFER,// Podpora Double Bufferingu

	PFD_TYPE_RGBA,// RGBA Format

	bits,// Zvol� barevnou hloubku

	0, 0, 0, 0, 0, 0,// Bity barev ignorov�ny

	0,// ��dn� alpha buffer

	0,// Ignorov�n Shift bit

	0,// ��dn� akumula�n� buffer

	0, 0, 0, 0,// Akumula�n� bity ignorov�ny

	16,// 16-bitov� hloubkov� buffer (Z-Buffer)

	0,// ��dn� Stencil Buffer

	0,// ��dn� Auxiliary Buffer

	PFD_MAIN_PLANE,// Hlavn� vykreslovac� vrstva

	0,// Rezervov�no

	0, 0, 0// Maska vrstvy ignorov�na

	};
	if (!(hDC = GetDC(hWnd)))// Poda�ilo se p�ipojit kontext za��zen�?

	{

		KillGLWindow();// Zav�e okno

		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukon�� program

	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))// Poda�ilo se naj�t Pixel Format?

	{

		KillGLWindow();// Zav�e okno

		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukon�� program

	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))// Poda�ilo se nastavit Pixel Format?

	{

		KillGLWindow();// Zav�e okno

		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukon�� program

	}
	if (!(hRC = wglCreateContext(hDC)))// Poda�ilo se vytvo�it Rendering Context?

	{

		KillGLWindow();// Zav�e okno

		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukon�� program

	}
	if (!wglMakeCurrent(hDC, hRC))// Poda�ilo se aktivovat Rendering Context?

	{

		KillGLWindow();// Zav�e okno

		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukon�� program

	}
	ShowWindow(hWnd, SW_SHOW);// Zobrazen� okna

	SetForegroundWindow(hWnd);// Do pop�ed�

	SetFocus(hWnd);// Zam��� fokus

	ReSizeGLScene(width, height);// Nastaven� perspektivy OpenGL sc�ny
	if (!InitGL())// Inicializace okna

	{

		KillGLWindow();// Zav�e okno

		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukon�� program

	}
	return TRUE;// V�e prob�hlo v po��dku

}
LRESULT CALLBACK WndProc(HWND hWnd,// Handle okna

	UINT uMsg,// Zpr�va pro okno

	WPARAM wParam,// Dopl�kov� informace

	LPARAM lParam)// Dopl�kov� informace

{
	switch (uMsg)// V�tven� podle p��choz� zpr�vy

	{
	case WM_ACTIVATE:// Zm�na aktivity okna

	{

		if (!HIWORD(wParam))// Zkontroluje zda nen� minimalizovan�

		{

			active = TRUE;// Program je aktivn�

		}

		else

		{

			active = FALSE;// Program nen� aktivn�

		}

		return 0;// N�vrat do hlavn�ho cyklu programu

	}
	case WM_SYSCOMMAND:// Syst�mov� p��kaz

	{

		switch (wParam)// Typ syst�mov�ho p��kazu

		{

		case SC_SCREENSAVE:// Pokus o zapnut� �et�i�e obrazovky

		case SC_MONITORPOWER:// Pokus o p�echod do �sporn�ho re�imu?

			return 0;// Zabr�n� oboj�mu

		}

		break;// N�vrat do hlavn�ho cyklu programu

	}
	case WM_CLOSE:// Povel k ukon�en� programu

	{

		PostQuitMessage(0);// Po�le zpr�vu o ukon�en�

		return 0;// N�vrat do hlavn�ho cyklu programu

	}
	case WM_KEYDOWN:// Stisk kl�vesy

	{

		keys[wParam] = TRUE;// Ozn�m� to programu

		return 0;// N�vrat do hlavn�ho cyklu programu

	}
	case WM_KEYUP:// Uvoln�n� kl�vesy

	{

		keys[wParam] = FALSE;// Ozn�m� to programu

		return 0;// N�vrat do hlavn�ho cyklu programu

	}
	case WM_SIZE:// Zm�na velikosti okna

	{

		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord=���ka, HiWord=V��ka

		return 0;// N�vrat do hlavn�ho cyklu programu

	}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);// P�ed�n� ostatn�ch zpr�v syst�mu

}
int WINAPI WinMain(HINSTANCE hInstance,// Instance

	HINSTANCE hPrevInstance,// P�edchoz� instance

	LPSTR lpCmdLine,// Parametry p��kazov� ��dky

	int nCmdShow)// Stav zobrazen� okna

{
	MSG msg;// Struktura zpr�v syst�mu

	BOOL done = FALSE;// Prom�nn� pro ukon�en� programu
	// Dotaz na u�ivatele pro fullscreen/okno

	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)

	{

		fullscreen = FALSE;// B�h v okn�

	}
	if (!CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen))// Vytvo�en� OpenGL okna

	{

		return 0;// Konec programu p�i chyb�

	}
	while (!done)// Hlavn� cyklus programu

	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))// P�i�la zpr�va?

		{
			if (msg.message == WM_QUIT)// Obdr�eli jsme zpr�vu pro ukon�en�?

			{

				done = TRUE;// Konec programu

			}

			else// P�ed�me zpr�vu procedu�e okna

			{
				TranslateMessage(&msg);// P�elo�� zpr�vu

				DispatchMessage(&msg);// Ode�le zpr�vu

			}

		}

		else// Pokud nedo�la ��dn� zpr�va

		{
			if (active)// Je program aktivn�?

			{

				if (keys[VK_ESCAPE])// Byl stisknut ESC?

				{

					done = TRUE;// Ukon��me program

				}

				else// P�ekreslen� sc�ny

				{
					DrawGLScene();// Vykreslen� sc�ny

					SwapBuffers(hDC);// Prohozen� buffer� (Double Buffering)

				}

			}
			if (keys[VK_F1])// Byla stisknuta kl�vesa F1?

			{

				keys[VK_F1] = FALSE;// Ozna� ji jako nestisknutou

				KillGLWindow();// Zru�� okno

				fullscreen = !fullscreen;// Negace fullscreen

				// Znovuvytvo�en� okna

				if (!CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen))

				{

					return 0;// Konec programu pokud nebylo vytvo�eno

				}

			}

		}

	}
	KillGLWindow();// Zav�e okno

	return (msg.wParam);// Ukon�en� programu

}
