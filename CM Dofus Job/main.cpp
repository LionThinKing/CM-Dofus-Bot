#include <windows.h>
#include "Job.cpp"
#include <stdio.h>





POINT cursor;
HHOOK KeyBoardHook;

LRESULT CALLBACK ScanHookKey(int nCode, WPARAM wParam, LPARAM lParam) 
{
	
    PKBDLLHOOKSTRUCT Key = (PKBDLLHOOKSTRUCT) (lParam);
    if (wParam == WM_KEYDOWN)
    {
		VKCode=Key->vkCode;
	}	
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void Leer()
{
	int c=0,size=0,p=0;
	char *Temp=(char*)malloc(100);
	memset(Temp,0,100);
	FILE *data;
	
	if(data=fopen("Data/Data.ini", "r"))
	{
		fseek(data, 104, SEEK_SET);
		while( (c=getc(data)) !='\n' )
		{
			Temp[p]=c;
			p++;
		}
		SetWindowText(GetDlgItem(hBot,ID_NAME),Temp);
		while( (c=getc(data)) !='='){}
		
		memset(Temp,0,100);
		p=0;
        while( (c=getc(data)) !=EOF )
		{
			Temp[p]=c;
			p++;
		}
		SetWindowText(GetDlgItem(hBot,ID_TIME),Temp);
	}
	fclose(data);
}


void Save()
{
	char *Temp=(char*)malloc(100);
	memset(Temp,0,100);
	FILE *data;
	
	DeleteFile("Data/Data.ini");
	if(data=fopen("Data/Data.ini", "w"))
    {
        fprintf(data,"[*]CM Dofus Job 0.1\n[*]Authors: TR31N0RD & Krosaver\n[*]Team: <Coded Masters Labs> (CM Labs)\n\n");
        
		GetWindowText(GetDlgItem(hBot,ID_NAME),Temp,100);
		fprintf(data,"[Name]=%s\n",Temp);
	
		GetWindowText(GetDlgItem(hBot,ID_TIME),Temp,100);
		fprintf(data,"[Time]=%s",Temp);
	} 
	fclose(data);
}

DWORD WINAPI Job1(LPVOID Data)
{
	Job *jb = new Job();
	char *Temp=(char*)malloc(100);
	int size=0;
	
	
	
	memset(Temp,100,0);
	
	//Get Size Name Of Player
	size=GetWindowTextLength(GetDlgItem(hBot,ID_NAME));
	if(size==0)
	{
		SetWindowText(GetDlgItem(hBot,ID_STATUS),"[X] Debe Digitar Un Nombre de Ventana");
		return 0;
	}
	
	//Get Player Name
	GetWindowText(GetDlgItem(hBot,ID_NAME),Temp,100);
	jb->SetPlayer(Temp);
	if(!jb->GetPlayer())
	{
		SetWindowText(GetDlgItem(hBot,ID_STATUS),"[X] Ventana No Encontrada");
		return 0;
	}
	else
	{
		SetWindowText(GetDlgItem(hBot,ID_STATUS),"[OK] Seleccione Las Posiciones Con F8");
		ShowWindow(jb->GetPlayer(),SW_RESTORE);
	}
	
	//GetTime
	size=GetWindowTextLength(GetDlgItem(hBot,ID_TIME));
	if(size==0)
	{
		SetWindowText(GetDlgItem(hBot,ID_STATUS),"[X] Debe Digitar Un Tiempo De Espera");
		return 0;
	}
	GetWindowText(GetDlgItem(hBot,ID_TIME),Temp,100);
	size=atoi(Temp);
	jb->SetTime(size);
	
	//Set Windows Size
	SetWindowPos(jb->GetPlayer(),HWND_NOTOPMOST,0,0,1080,889,SWP_SHOWWINDOW);
	
	//Get Positions of Mouse
	while(true)
	{
		if(VKCode==VK_F8)
		{
			SetWindowText(GetDlgItem(hBot,ID_STATUS),"[OK] Posicion Insertada");
			GetCursorPos(&cursor);
			ScreenToClient(jb->GetPlayer(),&cursor);
			jb->InsertarPos(cursor.x,cursor.y);
			VKCode=0;
			SetWindowText(GetDlgItem(hBot,ID_POS),jb->GetNumPos());
		}
		
		if(VKCode==VK_F9)
		{
			VKCode=0;
			break;
		}
		Sleep(20);
	}
	SetWindowText(GetDlgItem(hBot,ID_STATUS),"[OK] Comienza el Recorrido");
	jb->RecorrerPos();
	UnhookWindowsHookEx(KeyBoardHook);
	SendMessage(hBot, WM_CLOSE, 0, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) 
{
	HICON hIcon;
	switch(Message) 
	{
		case WM_CREATE:
			{
				//Set Icon
            	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadImage(NULL, "Data/CM Dofus Job.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
				
				CreateWindowEx(0,"STATIC", "Player", WS_VISIBLE | WS_CHILD, 10, 50, 80, 20,hwnd, (HMENU)(ID_SNAME), NULL, NULL);
				CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE ,100,50,300,20,hwnd,(HMENU)(ID_NAME),NULL,NULL);
				
				CreateWindowEx(0,"STATIC", "Time", WS_VISIBLE | WS_CHILD, 10, 80, 80, 20,hwnd, (HMENU)(ID_STIME), NULL, NULL);
				CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE ,100,80,50,20,hwnd,(HMENU)(ID_TIME),NULL,NULL);
				
				CreateWindowEx(0,"STATIC", "# Pos", WS_VISIBLE | WS_CHILD, 10, 110, 80, 20,hwnd, (HMENU)(ID_SPOS), NULL, NULL);
				CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE ,100,110,50,20,hwnd,(HMENU)(ID_POS),NULL,NULL);
				EnableWindow(GetDlgItem(hwnd,ID_POS),false);
				
				CreateWindowEx(0,"STATIC", "#Enemigos", WS_VISIBLE | WS_CHILD, 10, 140, 80, 20,hwnd, (HMENU)(ID_SNE), NULL, NULL);
				CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE ,100,140,50,20,hwnd,(HMENU)(ID_NE),NULL,NULL);
				EnableWindow(GetDlgItem(hwnd,ID_NE),false);
				
				CreateWindowEx(0,"STATIC", "# Aliados", WS_VISIBLE | WS_CHILD, 10, 170, 80, 20,hwnd, (HMENU)(ID_SNA), NULL, NULL);
				CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE ,100,170,50,20,hwnd,(HMENU)(ID_NA),NULL,NULL);
				EnableWindow(GetDlgItem(hwnd,ID_NA),false);
				
				CreateWindowEx(0,"STATIC", "Status", WS_VISIBLE | WS_CHILD, 10, 200, 80, 20,hwnd, (HMENU)(ID_SSTATUS), NULL, NULL);
				CreateWindow("EDIT","",WS_OVERLAPPED | WS_CHILD | WS_VISIBLE ,100,200,300,20,hwnd,(HMENU)(ID_STATUS),NULL,NULL);
				EnableWindow(GetDlgItem(hwnd,ID_STATUS),false);
				
				CreateWindowEx(0,"BUTTON", "Start",WS_CHILD | WS_VISIBLE , 300,10,100,30,hwnd, (HMENU)ID_START, NULL, NULL);
				hBot=hwnd;
				Leer();
				break;
				
			}
		
		case WM_COMMAND:
			{
				switch(LOWORD(wParam)) 
				{
					case ID_START:
					{
						EnableWindow(GetDlgItem(hwnd,ID_START),false);
						KeyBoardHook=SetWindowsHookEx(WH_KEYBOARD_LL,ScanHookKey,(HINSTANCE)GetWindowLong(GetForegroundWindow(),-6),0);
						DWORD Data;
						HANDLE hJob=CreateThread(NULL, 0, Job1, &Data, 0, NULL);
	                    if(hJob==NULL)
	                    {
	                        ExitProcess(Data);
	                    }
					}
				}
				break;
			}
			
			
		case WM_CLOSE: 
			{
				Save();
				DestroyWindow(hwnd);
				break;
			}
		
		case WM_DESTROY: 
			{
				PostQuitMessage(0);
				break;
			}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; 
	HWND hwnd;
	MSG Msg; 

	
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; 
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);
	wc.lpszClassName = "CM_Dofus_Job";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); 
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"CM_Dofus_Job","CM Dofus Job 0.1 ",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		440, 
		270, 
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	while(GetMessage(&Msg, NULL, 0, 0) > 0) { 
		TranslateMessage(&Msg); 
		DispatchMessage(&Msg); 
	}
	return Msg.wParam;
}
