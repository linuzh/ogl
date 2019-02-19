// Vložení knihoven
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"glaux.lib")

#include <windows.h>// Hlavièkový soubor pro Windows
#include <gl\gl.h>// Hlavièkový soubor pro OpenGL32 knihovnu
#include <gl\glu.h>// Hlavièkový soubor pro Glu32 knihovnu
//#include <gl\glaux.h>// Hlavièkový soubor pro Glaux knihovnu

HDC hDC = NULL;// Privátní GDI Device Context
HGLRC hRC = NULL;// Trvalý Rendering Context
HWND hWnd = NULL;// Obsahuje Handle našeho okna
HINSTANCE hInstance;// Obsahuje instanci aplikace

bool keys[256];// Pole pro ukládání vstupu z klávesnice
bool active = TRUE;// Ponese informaci o tom, zda je okno aktivní
bool fullscreen = TRUE;// Ponese informaci o tom, zda je program ve fullscreenu
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);// Deklarace procedury okna (funkèní prototyp)

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)// Zmìna velikosti a inicializace OpenGL okna
{
	if (height == 0)// Zabezpeèení proti dìlení nulou
	{
		height = 1;// Nastaví výšku na jedna
	}
	glViewport(0, 0, width, height);// Resetuje aktuální nastavení
	glMatrixMode(GL_PROJECTION);// Zvolí projekèní matici
	glLoadIdentity();// Reset matice
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);// Výpoèet perspektivy
	glMatrixMode(GL_MODELVIEW);// Zvolí matici Modelview
	glLoadIdentity();// Reset matice
}

int InitGL(GLvoid)// Všechno nastavení OpenGL
{
	glShadeModel(GL_SMOOTH);// Povolí jemné stínování
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);// Èerné pozadí
	glClearDepth(1.0f);// Nastavení hloubkového bufferu
	glEnable(GL_DEPTH_TEST);// Povolí hloubkové testování
	glDepthFunc(GL_LEQUAL);// Typ hloubkového testování
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Nejlepší perspektivní korekce
	return TRUE;// Inicializace probìhla v poøádku
}

int DrawGLScene(GLvoid)// Vykreslování

{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Smaže obrazovku a hloubkový buffer

	glLoadIdentity();// Reset matice

	// Sem mùžete kreslit

	return TRUE;// Vykreslení probìhlo v poøádku

}

GLvoid KillGLWindow(GLvoid)// Zavírání okna

{
	if (fullscreen)// Jsme ve fullscreenu?

	{
		ChangeDisplaySettings(NULL, 0);// Pøepnutí do systému

		ShowCursor(TRUE);// Zobrazí kurzor myši

	}
	if (hRC)// Máme rendering kontext?

	{
		if (!wglMakeCurrent(NULL, NULL))// Jsme schopni oddìlit kontexty?

		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		}
		if (!wglDeleteContext(hRC))// Jsme schopni smazat RC?

		{

			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		}

		hRC = NULL;// Nastaví hRC na NULL

	}
	if (hDC && !ReleaseDC(hWnd, hDC))// Jsme schopni uvolnit DC

	{

		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hDC = NULL;// Nastaví hDC na NULL

	}
	if (hWnd && !DestroyWindow(hWnd))// Jsme schopni odstranit okno?

	{

		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hWnd = NULL;// Nastaví hWnd na NULL

	}
	if (!UnregisterClass("OpenGL", hInstance))// Jsme schopni odregistrovat tøídu okna?

	{

		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		hInstance = NULL;// Nastaví hInstance na NULL

	}

}
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)

{
	GLuint PixelFormat;// Ukládá formát pixelù
	WNDCLASS wc;// Struktura Windows Class
	DWORD dwExStyle;// Rozšíøený styl okna

	DWORD dwStyle;// Styl okna
	RECT WindowRect;// Obdélník okna

	WindowRect.left = (long)0;// Nastaví levý okraj na nulu

	WindowRect.right = (long)width;// Nastaví pravý okraj na zadanou hodnotu

	WindowRect.top = (long)0;// Nastaví horní okraj na nulu

	WindowRect.bottom = (long)height;// Nastaví spodní okraj na zadanou hodnotu
	fullscreen = fullscreenflag;// Nastaví promìnnou fullscreen na správnou hodnotu
	hInstance = GetModuleHandle(NULL);// Získá instanci okna

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// Pøekreslení pøi zmìnì velikosti a vlastní DC

	wc.lpfnWndProc = (WNDPROC)WndProc;// Definuje proceduru okna

	wc.cbClsExtra = 0;// Žádná extra data

	wc.cbWndExtra = 0;// Žádná extra data

	wc.hInstance = hInstance;// Instance

	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);// Standardní ikona

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);// Standardní kurzor myši

	wc.hbrBackground = NULL;// Pozadí není nutné

	wc.lpszMenuName = NULL;// Nechceme menu

	wc.lpszClassName = "OpenGL";// Jméno tøídy okna
	if (!RegisterClass(&wc))// Registruje tøídu okna

	{

		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Pøi chybì vrátí false

	}
	if (fullscreen)// Budeme ve fullscreenu?

	{
		DEVMODE dmScreenSettings;// Mód zaøízení

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));// Vynulování pamìti

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);// Velikost struktury Devmode

		dmScreenSettings.dmPelsWidth = width;// Šíøka okna

		dmScreenSettings.dmPelsHeight = height;// Výška okna

		dmScreenSettings.dmBitsPerPel = bits;// Barevná hloubka

		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Pokusí se použít právì definované nastavení

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)

		{
			// Nejde-li fullscreen, mùže uživatel spustit program v oknì nebo ho opustit

			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)

			{
				fullscreen = FALSE;// Bìh v oknì

			}

			else

			{
				// Zobrazí uživateli zprávu, že program bude ukonèen

				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);

				return FALSE;// Vrátí FALSE

			}

		}

	}
	if (fullscreen)// Jsme stále ve fullscreenu?

	{
		dwExStyle = WS_EX_APPWINDOW;// Rozšíøený styl okna

		dwStyle = WS_POPUP;// Styl okna

		ShowCursor(FALSE);// Skryje kurzor

	}

	else

	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;// Rozšíøený styl okna

		dwStyle = WS_OVERLAPPEDWINDOW;// Styl okna

	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);// Pøizpùsobení velikosti okna
	// Vytvoøení okna

	if (!(hWnd = CreateWindowEx(dwExStyle,// Rozšíøený styl

		"OpenGL",// Jméno tøídy

		title,// Titulek

		dwStyle |// Definovaný styl

		WS_CLIPSIBLINGS |// Požadovaný styl

		WS_CLIPCHILDREN,// Požadovaný styl

		0, 0,// Pozice

		WindowRect.right - WindowRect.left,// Výpoèet šíøky

		WindowRect.bottom - WindowRect.top,// Výpoèet výšky

		NULL,// Žádné rodièovské okno

		NULL,// Bez menu

		hInstance,// Instance

		NULL)))// Nepøedat nic do WM_CREATE
	{

		KillGLWindow();// Zruší okno

		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Vrátí chybu

	}
	static PIXELFORMATDESCRIPTOR pfd =// Oznámíme Windows jak chceme vše nastavit

	{

	sizeof(PIXELFORMATDESCRIPTOR),// Velikost struktury

	1,// Èíslo verze

	PFD_DRAW_TO_WINDOW |// Podpora okna

	PFD_SUPPORT_OPENGL |// Podpora OpenGL

	PFD_DOUBLEBUFFER,// Podpora Double Bufferingu

	PFD_TYPE_RGBA,// RGBA Format

	bits,// Zvolí barevnou hloubku

	0, 0, 0, 0, 0, 0,// Bity barev ignorovány

	0,// Žádný alpha buffer

	0,// Ignorován Shift bit

	0,// Žádný akumulaèní buffer

	0, 0, 0, 0,// Akumulaèní bity ignorovány

	16,// 16-bitový hloubkový buffer (Z-Buffer)

	0,// Žádný Stencil Buffer

	0,// Žádný Auxiliary Buffer

	PFD_MAIN_PLANE,// Hlavní vykreslovací vrstva

	0,// Rezervováno

	0, 0, 0// Maska vrstvy ignorována

	};
	if (!(hDC = GetDC(hWnd)))// Podaøilo se pøipojit kontext zaøízení?

	{

		KillGLWindow();// Zavøe okno

		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukonèí program

	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))// Podaøilo se najít Pixel Format?

	{

		KillGLWindow();// Zavøe okno

		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukonèí program

	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))// Podaøilo se nastavit Pixel Format?

	{

		KillGLWindow();// Zavøe okno

		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukonèí program

	}
	if (!(hRC = wglCreateContext(hDC)))// Podaøilo se vytvoøit Rendering Context?

	{

		KillGLWindow();// Zavøe okno

		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukonèí program

	}
	if (!wglMakeCurrent(hDC, hRC))// Podaøilo se aktivovat Rendering Context?

	{

		KillGLWindow();// Zavøe okno

		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukonèí program

	}
	ShowWindow(hWnd, SW_SHOW);// Zobrazení okna

	SetForegroundWindow(hWnd);// Do popøedí

	SetFocus(hWnd);// Zamìøí fokus

	ReSizeGLScene(width, height);// Nastavení perspektivy OpenGL scény
	if (!InitGL())// Inicializace okna

	{

		KillGLWindow();// Zavøe okno

		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

		return FALSE;// Ukonèí program

	}
	return TRUE;// Vše probìhlo v poøádku

}
LRESULT CALLBACK WndProc(HWND hWnd,// Handle okna

	UINT uMsg,// Zpráva pro okno

	WPARAM wParam,// Doplòkové informace

	LPARAM lParam)// Doplòkové informace

{
	switch (uMsg)// Vìtvení podle pøíchozí zprávy

	{
	case WM_ACTIVATE:// Zmìna aktivity okna

	{

		if (!HIWORD(wParam))// Zkontroluje zda není minimalizované

		{

			active = TRUE;// Program je aktivní

		}

		else

		{

			active = FALSE;// Program není aktivní

		}

		return 0;// Návrat do hlavního cyklu programu

	}
	case WM_SYSCOMMAND:// Systémový pøíkaz

	{

		switch (wParam)// Typ systémového pøíkazu

		{

		case SC_SCREENSAVE:// Pokus o zapnutí šetøièe obrazovky

		case SC_MONITORPOWER:// Pokus o pøechod do úsporného režimu?

			return 0;// Zabrání obojímu

		}

		break;// Návrat do hlavního cyklu programu

	}
	case WM_CLOSE:// Povel k ukonèení programu

	{

		PostQuitMessage(0);// Pošle zprávu o ukonèení

		return 0;// Návrat do hlavního cyklu programu

	}
	case WM_KEYDOWN:// Stisk klávesy

	{

		keys[wParam] = TRUE;// Oznámí to programu

		return 0;// Návrat do hlavního cyklu programu

	}
	case WM_KEYUP:// Uvolnìní klávesy

	{

		keys[wParam] = FALSE;// Oznámí to programu

		return 0;// Návrat do hlavního cyklu programu

	}
	case WM_SIZE:// Zmìna velikosti okna

	{

		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord=Šíøka, HiWord=Výška

		return 0;// Návrat do hlavního cyklu programu

	}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);// Pøedání ostatních zpráv systému

}
int WINAPI WinMain(HINSTANCE hInstance,// Instance

	HINSTANCE hPrevInstance,// Pøedchozí instance

	LPSTR lpCmdLine,// Parametry pøíkazové øádky

	int nCmdShow)// Stav zobrazení okna

{
	MSG msg;// Struktura zpráv systému

	BOOL done = FALSE;// Promìnná pro ukonèení programu
	// Dotaz na uživatele pro fullscreen/okno

	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO)

	{

		fullscreen = FALSE;// Bìh v oknì

	}
	if (!CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen))// Vytvoøení OpenGL okna

	{

		return 0;// Konec programu pøi chybì

	}
	while (!done)// Hlavní cyklus programu

	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))// Pøišla zpráva?

		{
			if (msg.message == WM_QUIT)// Obdrželi jsme zprávu pro ukonèení?

			{

				done = TRUE;// Konec programu

			}

			else// Pøedáme zprávu proceduøe okna

			{
				TranslateMessage(&msg);// Pøeloží zprávu

				DispatchMessage(&msg);// Odešle zprávu

			}

		}

		else// Pokud nedošla žádná zpráva

		{
			if (active)// Je program aktivní?

			{

				if (keys[VK_ESCAPE])// Byl stisknut ESC?

				{

					done = TRUE;// Ukonèíme program

				}

				else// Pøekreslení scény

				{
					DrawGLScene();// Vykreslení scény

					SwapBuffers(hDC);// Prohození bufferù (Double Buffering)

				}

			}
			if (keys[VK_F1])// Byla stisknuta klávesa F1?

			{

				keys[VK_F1] = FALSE;// Oznaè ji jako nestisknutou

				KillGLWindow();// Zruší okno

				fullscreen = !fullscreen;// Negace fullscreen

				// Znovuvytvoøení okna

				if (!CreateGLWindow("NeHe's OpenGL Framework", 640, 480, 16, fullscreen))

				{

					return 0;// Konec programu pokud nebylo vytvoøeno

				}

			}

		}

	}
	KillGLWindow();// Zavøe okno

	return (msg.wParam);// Ukonèení programu

}
