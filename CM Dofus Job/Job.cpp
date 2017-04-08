#include <windows.h>


int VKCode;
HWND hBot;

enum
{
	ID_SPOS,
	ID_POS,
	ID_SNAME,
	ID_NAME,
	ID_STIME,
	ID_TIME,
	ID_SSTATUS,
	ID_STATUS,
	ID_START,
	ID_SNE,
	ID_NE,
	ID_SNA,
	ID_NA
};

class Job
{
	private:
		HWND Player;
		int Time;
		int WinLong;
		int nEnemigos;
		int nAliados;
		int *Enemigos;
		int *Aliados;
		struct Position
		{
			int x;
			int y;
			struct Position *Next;
		};
		typedef struct Position Pos;
		Pos *PTR,*R;
		
	
	public:
		
		Job()
		{
			PTR=0;
		}
		
		void SetPlayer(char *NameWindow)
		{
			Player=FindWindow("ApolloRuntimeContentWindow", TEXT(NameWindow));
		}
				
		HWND GetPlayer()
		{
			return Player; 
		}
		
		void SetTime(int t)
		{
			Time=t;
		}
		
		char *GetNumPos()
		{
			
			char *Buffer=(char*)malloc(10);
			memset(Buffer,0,10);
			int c=0;
			Pos *P;
			for(P=PTR;P!=NULL;P=P->Next)
		    {
				c++;
			}
			itoa (c,Buffer,10);
			return Buffer;
		}
		
		void InsertarPos(int x, int y)
		{
			
			Pos *Aux = PTR;
			Aux=(Pos*)malloc(sizeof(Pos));
			if(!PTR)
			{
				PTR=Aux;
				PTR->x=x;
				PTR->y=y;
				PTR->Next=NULL;
				R=PTR;
			}
			else
			{
				R->Next=Aux;
				R=Aux;
				R->x=x;
				R->y=y;
				R->Next=NULL;
			}
		}
		
		void ClickPos(int x, int y)
		{
		    SendMessage(GetPlayer(), WM_LBUTTONDOWN, MK_LBUTTON, LPARAM MAKELPARAM(x,y));
			SendMessage(GetPlayer(), WM_LBUTTONUP, MK_LBUTTON, LPARAM MAKELPARAM(x,y));   
		}
		
		void OficioLvlUp()
		{
			COLORREF color;
			color=GetPixel(GetWindowDC(GetPlayer()), 505, 510);
			
			if(color==(RGB(255, 97, 0)))
			{
				ClickPos(500,480);
			}			
			ReleaseDC(GetPlayer(),0);
		}
		
		void DetectarEnemigos()
		{
			int X=1016,Y=720;
			Aliados=(int*)malloc(10);
			Enemigos=(int*)malloc(10);
			memset(Aliados,0,10);
			memset(Enemigos,0,10);
			nAliados=0;
			nEnemigos=0;
			bool Continue;
			
			do
			{
				Continue=false;
				COLORREF color=GetPixel(GetWindowDC(GetPlayer()), X, Y);
				int Value;
				
				//Aliado
				Value=GetRValue(color);
				if(Value<=255 && Value>=245)
				{
					Value=GetGValue(color);
					if(Value<=28 && Value>=18)
					{
						Value=GetBValue(color);
						if(Value<=28 && Value>=18)
						{
						
							Aliados[nAliados]=X;
							nAliados++;
							SetWindowText(GetDlgItem(hBot,ID_STATUS),"[!] Aliado Detectado...");
							Continue=true;
						}
					}
				}
				
				//Enemigo
				Value=GetRValue(color);
				if(Value<=40 && Value>15)
				{
					Value=GetGValue(color);
					if(Value<=225 && Value>=200)
					{
						Value=GetBValue(color);
						if(Value<=255 && Value>=200)
						{
							SetWindowText(GetDlgItem(hBot,ID_STATUS),"[!] Enemigo Detectado...");
							Enemigos[nEnemigos]=X;
							nEnemigos++;
							Continue=true;
						}
					}
				}
				X-=54;
			}while(Continue);
			
			char *Set=(char*)malloc(100);
			itoa(nEnemigos,Set,10);
			SetWindowText(GetDlgItem(hBot,ID_NE),Set);
			itoa(nAliados,Set,10);
			SetWindowText(GetDlgItem(hBot,ID_NA),Set);
		}
		
		bool EsperarTurno()
		{
			bool bValidate=false;
			int c=0;
			
			SetWindowText(GetDlgItem(hBot,ID_STATUS),"[!] Esperando Turno...");
			
			while(c<nAliados)
			{
				COLORREF color=GetPixel(GetWindowDC(GetPlayer()),(Aliados[c]-18), 748);        			
				if(color==RGB(255,97,0))
				{
					SetWindowText(GetDlgItem(hBot,ID_STATUS),"Turno Para  Atacar...");
					bValidate=true;            
				}
				c++;
			}
			Sleep(100);			
			return bValidate;
		}
		
		void LanzarAtaque()
		{
			for(int c=0;c<nEnemigos;c++)
			{
				//Selecciona Flecha Magica
				ClickPos(696,759);
				Sleep(50);
				//Selecciona Enemigo
				ClickPos(Enemigos[c]-28,690);
			}
			Sleep(50);
			//Pasa Turno
			ClickPos(507,827);
		}
		
		bool FinBatalla()
		{
			bool Validate=false;
			COLORREF color=GetPixel(GetWindowDC(GetPlayer()),486,647);
			if(color==(RGB(255, 97, 0)))
			{
				SetWindowText(GetDlgItem(hBot,ID_STATUS),"Batalla Finalizada...");
				Validate=true;
			}
			return Validate;
		}
		
		
		void Pelear()
		{
			SetWindowText(GetDlgItem(hBot,ID_STATUS),"Empiezo A Detectar Enemigos");
			DetectarEnemigos();
			//Click en el boton Listo
			ClickPos(507,827);
			
			while(true)
			{
				if(FinBatalla()){break;}
				
				if(EsperarTurno())
				{
				
					//Lanza Ataque
					Sleep(200);
					LanzarAtaque();
				}
				Sleep(10);
			}
			
			//Click Fin de Batalla
			ClickPos(469,617);
			Sleep(100);
		}
		
		void DetectarBatalla()
		{
			COLORREF color;
			color=GetPixel(GetWindowDC(GetPlayer()), 510, 852);
			
			if(color==(RGB(255, 97, 0)))
			{
				Pelear();
			}			
			ReleaseDC(GetPlayer(),0);
			
		}
		
		void RecorrerPos()
		{
		    bool val=true;
		   	Pos *P;
			
			char *Mensaje=(char*)malloc(100),*Buffer=(char*)malloc(10);
			
		    while(val)
		    {
				int c=1;
		        for(P=PTR;P!=NULL;P=P->Next)
		        {
					if(VKCode==VK_F10){val=false;break;}
					DetectarBatalla();
					OficioLvlUp();
					memset(Mensaje,0,100);
					strcat(Mensaje,"Realizando Oficio En La Posicion #");
					itoa (c,Buffer,10);
					strcat(Mensaje,Buffer);
					SetWindowText(GetDlgItem(hBot,ID_STATUS),Mensaje);
		            ClickPos(P->x,P->y);
		            Sleep(Time);
					c++;
		        }   
		    }
		}
		
		
};
