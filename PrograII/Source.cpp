#include <windows.h>
#include <string>
#include <CommDlg.h>
#include "resource.h"
#include <mmsystem.h>
#include <fstream>
#include <string.h>
#include <ctime>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")

using namespace std;
//imagen 2 es la de perfil
struct contactos {
	char nombre[50];
	char direccion[256];
	char correo[80];
	char telefono[30];
	char catego[30];
	char cancion[256];
	char nomcanc[256];
	char imagen1[256];
	char imagen2[256];
	contactos *sig;
	contactos *prev;
};

struct alarmas {
	char nomcita[50];
	char fechacit[20];
	char hora[10];
	char min[10];
	char nomson[256];
	char rutason[256];
	char descripcion[256];
	char contactos[256];
	alarmas *sig;
	alarmas *prev;
};

//Para Timer
#define ID_5SECONDS 100
UINT TimerID = 0;

bool reprodu = false;
contactos *listac = NULL;
alarmas *listaa = NULL;

// Dialog handle.
HWND ghDlg = 0;
HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;

//funciones para el tiempo
time_t rawtime;
struct tm *timeinfo;
//Calcular lo del calendario
int year, month, day;
const char * weekday[] = { "Sunday", "Monday",
"Tuesday", "Wednesday",
"Thursday", "Friday", "Saturday" };

char *AbrirArchivos(HWND Dialogo, int filtro);
void Llenarmeses(HWND handle);
void Llenarcombos(HWND handle);
void Llenaminuto(HWND handle, int desi);
char *funcionfecha(int dia, int mes, int anio, int suma);
string obtenerDireccion(string filename);
BOOL CALLBACK AboutDlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCONTACTOS_AGREGAR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCONTACTOS_MODIFICACI40002(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCITAS_AGREGAR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCITAS_MODIFICACI40004(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCONSULTA_CONTACTOSEXISTENTES(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgCITASEXISTENTES_SEMANALES(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

//Canciones predeterminadas
static char nompred1[50] = "Predeterminada1", nompred2[50] = "Predeterminada2";
static string predeterminada1 = obtenerDireccion("Predeterminada1.mp3");
static string predeterminada2 = obtenerDireccion("Predeterminada2.mp3");
//static char predeterminada1[256] = "C:\\Users\\Usuario\\Desktop\\Agenda\\PrograII\\PrograII\\Predeterminada1.mp3";
//static char predeterminada2[256] = "C:\\Users\\Usuario\\Desktop\\Agenda\\PrograII\\PrograII\\Predeterminada2.mp3";


//------HACE QUE FUNCIONE TODO (VOID MAIN)----
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	// Create the modeless dialog window. This program is a pure
	// dialog window application, and the dialog window is the
	// "main" window.
	TimerID = SetTimer(ghDlg, ID_5SECONDS, 5000, NULL);
	ghDlg = CreateDialog(
		hInstance, // Application instance.
		MAKEINTRESOURCE(IDD_CITASEXISTENTES_MENSUALES), // Dialog resource ID.
		0, // Parent window--null for no parent.
		AboutDlg); // Dialog window procedure.
				   // Show the dialog.
	ShowWindow(ghDlg, showCmd);
	UpdateWindow(ghDlg);
	// Enter the message loop.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		// Is the message a dialog message? If so the function
		// IsDialogMessage will return true and then dispatch
		// the message to the dialog window procedure.
		// Otherwise, we process as the message as normal.
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
//-----------------------------------------------

//IDD_LOGIN
BOOL CALLBACK AboutDlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hfechap, hmesact, hconmesanio, hconmes, hevent1, hevent2, hevent3, hevent4, hevent5, hevent6, hevent7, hevent8,
	hevent9, hevent10, hevent11, hevent12, hevent13, hevent14, hevent15, hevent16, hevent17, hevent18, hevent19, hevent20,
	hevent21, hevent22, hevent23, hevent24, hevent25, hevent26, hevent27, hevent28, hevent29, hevent30, hevent31;
	static char fecha[80], mesactual[80], horaact[80], auxhora[20], auxmin[20], auxcurrentday[10], auxcurrentmonth[20],
	auxcurrentyear[20],auxnumeritos[20], auxcurrentanioactpch[20], anioprest[20], diaprest[20], mesprest[20], auxhoraact[80], auxmesprest[20],
		auxcurrentcit[80], yearforsearch[20], monthforsearch[20], Comando[MAX_PATH] = "";
	static contactos * auxarchcon, *auxpuncon = NULL;
	static alarmas * auxarchala, *auxpunala = NULL, *tiemposalar, *reccfecha;
	static int z=1;
	UINT Indice;
	char *pch;

	//static char direccion[] = "C:/Users/Usuario/Desktop/Agenda/PrograII/cont.bin";
	static string direccion = obtenerDireccion("cont.bin");
	//static char direccion1[] = "C:/Users/Usuario/Desktop/Agenda/PrograII/alarm.bin";
	static string direccion1 = obtenerDireccion("alarm.bin");
	ifstream Archmementra;
	
	switch (msg)
	{
	case WM_TIMER:

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(horaact, 80, "%H/%M/%S", timeinfo);
		strcpy(auxhoraact, horaact);
		pch = strtok(auxhoraact, "/");
		while (pch != NULL) {
			if (z == 1) {
				strcpy(auxhora, pch);
			}
			if (z == 2) {
				strcpy(auxmin, pch);

			}
			z++;
			pch = strtok(NULL, "/");
		}
		z = 1;

		if (listaa != NULL) {
			tiemposalar = listaa;
			while (tiemposalar != NULL){
				if (strcmp(tiemposalar->fechacit, fecha)==0) {
					if (strcmp(tiemposalar->hora, auxhora)==0) {
						if (strcmp(tiemposalar->min, auxmin)==0) {
							strcat(Comando, "open \"");
							strcat(Comando, tiemposalar->rutason);
							strcat(Comando, "\" alias mp3 ");
							mciSendString(Comando, NULL, 0, 0);
							mciSendString("play mp3", NULL, 0, 0);
							strcpy(Comando, "");
							int nResult = MessageBox(hDlg, tiemposalar->nomcita , "Tiene una cita pendiente", MB_OK + MB_ICONINFORMATION);
							switch (nResult) {
								case IDOK:
								mciSendString("stop mp3", NULL, 0, 0);
								mciSendString("close mp3", NULL, 0, 0);
								break;
							}
						}
					}
				}
				tiemposalar = tiemposalar->sig;
			}
		}

		
	return true;

	case WM_INITDIALOG:
		TimerID = SetTimer(hDlg, ID_5SECONDS, 60000, NULL);
	
		//Recuperar lista de contactos
		Archmementra.open(direccion, ios::binary);
		if (Archmementra.is_open()) {
			contactos * auxnuevocon = new contactos;
			Archmementra.read((char *)auxnuevocon, sizeof(contactos));
			while (!Archmementra.eof()) {
				auxnuevocon->sig = NULL;
				auxnuevocon->prev = NULL;
				if (listac == NULL) {
					listac = auxnuevocon;
				}
				else {
					auxpuncon = listac;
					while (auxpuncon->sig != NULL) {
						auxpuncon = auxpuncon->sig;
					}
					auxpuncon->sig = auxnuevocon;
					auxnuevocon->prev = auxpuncon;
				}
				auxnuevocon = new contactos;
				Archmementra.read((char *)auxnuevocon, sizeof(contactos));
			}
			Archmementra.close();
		}
		else {
			MessageBox(hDlg, "No se han registrado contactos previamente", "AVISO", MB_OK + MB_ICONINFORMATION);
		}

		//Recuperar lista de alarmas
		Archmementra.open(direccion1, ios::binary);
		if (Archmementra.is_open()) {
			alarmas * auxnuevoala = new alarmas;
			Archmementra.read((char *)auxnuevoala, sizeof(alarmas));
			while (!Archmementra.eof()) {
				auxnuevoala->sig = NULL;
				auxnuevoala->prev = NULL;
				if (listaa == NULL) {
					listaa = auxnuevoala;
				}
				else {
					auxpunala = listaa;
					while (auxpunala->sig != NULL) {
						auxpunala = auxpunala->sig;
					}
					auxpunala->sig = auxnuevoala;
					auxnuevoala->prev = auxpunala;
				}
				auxnuevoala = new alarmas;
				Archmementra.read((char *)auxnuevoala, sizeof(alarmas));
			}
			Archmementra.close();
		}
		else {
			MessageBox(hDlg, "No se han registrado alarmas previamente", "AVISO", MB_OK + MB_ICONINFORMATION);
		}

		hfechap = GetDlgItem(hDlg, IDC_MES_FECHAHOY);
		hmesact = GetDlgItem(hDlg, IDC_MES);
		hconmes = GetDlgItem(hDlg, IDC_COMMESES);
		hconmesanio = GetDlgItem(hDlg, IDC_MES_FECHAANIO);
		hevent1 = GetDlgItem(hDlg, IDC_LISTEVENT1);
		hevent2 = GetDlgItem(hDlg, IDC_LISTEVENT2);
		hevent3 = GetDlgItem(hDlg, IDC_LISTEVENT3);
		hevent4 = GetDlgItem(hDlg, IDC_LISTEVENT4);
		hevent5 = GetDlgItem(hDlg, IDC_LISTEVENT5);
		hevent6 = GetDlgItem(hDlg, IDC_LISTEVENT6);
		hevent7 = GetDlgItem(hDlg, IDC_LISTEVENT7);
		hevent8 = GetDlgItem(hDlg, IDC_LISTEVENT8);
		hevent9 = GetDlgItem(hDlg, IDC_LISTEVENT9);
		hevent10 = GetDlgItem(hDlg, IDC_LISTEVENT10);
		hevent11 = GetDlgItem(hDlg, IDC_LISTEVENT11);
		hevent12 = GetDlgItem(hDlg, IDC_LISTEVENT12);
		hevent13 = GetDlgItem(hDlg, IDC_LISTEVENT13);
		hevent14 = GetDlgItem(hDlg, IDC_LISTEVENT14);
		hevent15 = GetDlgItem(hDlg, IDC_LISTEVENT15);
		hevent16 = GetDlgItem(hDlg, IDC_LISTEVENT16);
		hevent17 = GetDlgItem(hDlg, IDC_LISTEVENT17);
		hevent18 = GetDlgItem(hDlg, IDC_LISTEVENT18);
		hevent19 = GetDlgItem(hDlg, IDC_LISTEVENT19);
		hevent20 = GetDlgItem(hDlg, IDC_LISTEVENT20);
		hevent21 = GetDlgItem(hDlg, IDC_LISTEVENT21);
		hevent22 = GetDlgItem(hDlg, IDC_LISTEVENT22);
		hevent23 = GetDlgItem(hDlg, IDC_LISTEVENT23);
		hevent24 = GetDlgItem(hDlg, IDC_LISTEVENT24);
		hevent25 = GetDlgItem(hDlg, IDC_LISTEVENT25);
		hevent26 = GetDlgItem(hDlg, IDC_LISTEVENT26);
		hevent27 = GetDlgItem(hDlg, IDC_LISTEVENT27);
		hevent28 = GetDlgItem(hDlg, IDC_LISTEVENT28);
		hevent29 = GetDlgItem(hDlg, IDC_LISTEVENT29);
		hevent30 = GetDlgItem(hDlg, IDC_LISTEVENT30);
		hevent31 = GetDlgItem(hDlg, IDC_LISTEVENT31);
		Llenarmeses(hconmes);

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(mesactual, 80, "%B", timeinfo);
		SendMessage(hmesact, WM_SETTEXT, 80, (LPARAM)mesactual);

		strftime(fecha, 80, "%d/%m/%Y", timeinfo);
		SendMessage(hfechap, WM_SETTEXT, 80, (LPARAM)fecha);
		//auxcurrentanioact
		strcpy(auxcurrentanioactpch, fecha);
		pch = strtok(auxcurrentanioactpch, "/");
		while (pch != NULL) {
			if (z == 1) {
				strcpy(diaprest, pch);
			}
			if (z == 2) {
				strcpy(mesprest, pch);
			}
			if (z == 3) {
				strcpy(anioprest, pch);
			}
			z++;
			pch = strtok(NULL, "/");
		}
		z = 1;

		strftime(horaact, 80, "%H/%M/%S", timeinfo);
		strcpy(auxhoraact, horaact);
		pch = strtok(auxhoraact, "/");
		while (pch != NULL)	{
			if (z == 1) {
				strcpy(auxhora, pch);
			}
			if (z == 2) {
				strcpy(auxmin, pch);
			}
			z++;
			pch = strtok(NULL, "/");
		}
		z = 1;

		// Para ubicar las fechas
		reccfecha = listaa;
		while (reccfecha != NULL) {
			strcpy(auxcurrentcit,reccfecha->fechacit);
			pch = strtok(auxcurrentcit, "/");
			while (pch != NULL) {
					if (z == 1) {
						strcpy(auxcurrentday, pch);
					}
					if (z == 2) {
						strcpy(auxcurrentmonth, pch);
					}
					if (z == 3) {
						strcpy(auxcurrentyear, pch);
					}
					z++;
					pch = strtok(NULL, "/");
			}
			z = 1;

			if (strcmp(anioprest, auxcurrentyear) == 0) {
				if (strcmp(mesprest, auxcurrentmonth) == 0) {
					if (strcmp(auxcurrentday, "01") == 0) {
						SendMessage(hevent1, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "02") == 0) {
						SendMessage(hevent2, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "03") == 0) {
						SendMessage(hevent3, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "04") == 0) {
						SendMessage(hevent4, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "05") == 0) {
						SendMessage(hevent5, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "06") == 0) {
						SendMessage(hevent6, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "07") == 0) {
						SendMessage(hevent7, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "08") == 0) {
						SendMessage(hevent8, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "09") == 0) {
						SendMessage(hevent9, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "10") == 0) {
						SendMessage(hevent10, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "11") == 0) {
						SendMessage(hevent11, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "12") == 0) {
						SendMessage(hevent12, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "13") == 0) {
						SendMessage(hevent13, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "14") == 0) {
						SendMessage(hevent14, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "15") == 0) {
						SendMessage(hevent15, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "16") == 0) {
						SendMessage(hevent16, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "17") == 0) {
						SendMessage(hevent17, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "18") == 0) {
						SendMessage(hevent18, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "19") == 0) {
						SendMessage(hevent19, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "20") == 0) {
						SendMessage(hevent20, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "21") == 0) {
						SendMessage(hevent21, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "22") == 0) {
						SendMessage(hevent22, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "23") == 0) {
						SendMessage(hevent23, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "24") == 0) {
						SendMessage(hevent24, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "25") == 0) {
						SendMessage(hevent25, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "26") == 0) {
						SendMessage(hevent26, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "27") == 0) {
						SendMessage(hevent27, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "28") == 0) {
						SendMessage(hevent28, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "29") == 0) {
						SendMessage(hevent29, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "30") == 0) {
						SendMessage(hevent30, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
					if (strcmp(auxcurrentday, "31") == 0) {
						SendMessage(hevent31, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
					}
				}
			}
			reccfecha= reccfecha->sig;
		}

		return true;
		
	case WM_COMMAND:
		
		switch (LOWORD(wParam))
		{


		
		case IDC_BUTBUSQF:
			SendMessage(hevent1, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent2, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent3, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent4, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent5, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent6, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent7, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent8, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent9, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent10, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent11, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent12, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent13, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent14, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent15, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent16, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent17, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent18, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent19, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent20, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent21, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent22, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent23, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent24, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent25, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent26, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent27, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent28, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent29, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent30, LB_RESETCONTENT, 0, 0);
			SendMessage(hevent31, LB_RESETCONTENT, 0, 0);

			Indice = SendMessage(hconmes, CB_GETCURSEL, 0, 0);
			//Enero es 0 y Dicienbre es 11
			SendMessage(hconmes, CB_GETLBTEXT, (WPARAM)Indice, (LPARAM)monthforsearch);
			SendMessage(hmesact, WM_SETTEXT, 20, (LPARAM)monthforsearch);

			SendMessage(hconmesanio, WM_GETTEXT, 20, (LPARAM)yearforsearch);

			reccfecha = listaa;
			while (reccfecha != NULL){	
				strcpy(auxcurrentcit, reccfecha->fechacit);
				pch = strtok(auxcurrentcit, "/");
				while (pch != NULL) {
					if (z == 1) {
						strcpy(auxcurrentday, pch);
					}
					if (z == 2) {
						strcpy(auxcurrentmonth, pch);
					}
					if (z == 3) {
						strcpy(auxcurrentyear, pch);
					}
					z++;
					pch = strtok(NULL, "/");
				}
				z = 1;

				if (strcmp(yearforsearch, auxcurrentyear) == 0) {
					Indice++;
					if (Indice < 10) {
						strcpy(auxmesprest, "0");
						_itoa(Indice, auxnumeritos, 10);
						strcat(auxmesprest, auxnumeritos);
					}
					else {
						_itoa(Indice, auxnumeritos, 10);
						strcpy(auxmesprest, auxnumeritos);
					}

					if (strcmp(auxmesprest, auxcurrentmonth) == 0) {
						if (strcmp(auxcurrentday, "01") == 0) {
							SendMessage(hevent1, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "02") == 0) {
							SendMessage(hevent2, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "03") == 0) {
							SendMessage(hevent3, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "04") == 0) {
							SendMessage(hevent4, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "05") == 0) {
							SendMessage(hevent5, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "06") == 0) {
							SendMessage(hevent6, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "07") == 0) {
							SendMessage(hevent7, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "08") == 0) {
							SendMessage(hevent8, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "09") == 0) {
							SendMessage(hevent9, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "10") == 0) {
							SendMessage(hevent10, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "11") == 0) {
							SendMessage(hevent11, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "12") == 0) {
							SendMessage(hevent12, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "13") == 0) {
							SendMessage(hevent13, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "14") == 0) {
							SendMessage(hevent14, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "15") == 0) {
							SendMessage(hevent15, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "16") == 0) {
							SendMessage(hevent16, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "17") == 0) {
							SendMessage(hevent17, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "18") == 0) {
							SendMessage(hevent18, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "19") == 0) {
							SendMessage(hevent19, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "20") == 0) {
							SendMessage(hevent20, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "21") == 0) {
							SendMessage(hevent21, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "22") == 0) {
							SendMessage(hevent22, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "23") == 0) {
							SendMessage(hevent23, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "24") == 0) {
							SendMessage(hevent24, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "25") == 0) {
							SendMessage(hevent25, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "26") == 0) {
							SendMessage(hevent26, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "27") == 0) {
							SendMessage(hevent27, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "28") == 0) {
							SendMessage(hevent28, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "29") == 0) {
							SendMessage(hevent29, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "30") == 0) {
							SendMessage(hevent30, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
						if (strcmp(auxcurrentday, "31") == 0) {
							SendMessage(hevent31, LB_ADDSTRING, 0, (LPARAM)reccfecha->nomcita);
						}
					}
				}
				reccfecha = reccfecha->sig;
				Indice--;
			}


			return true;
			//-----------------
		case ID_CONTACTOS_AGREGAR:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CONTACTOS_AGREGAR), hDlg, AboutDlgCONTACTOS_AGREGAR);
			return true;

		case ID_CONTACTOS_MODIFICACI40002:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CONTACTOS_MODIFICACI40002), hDlg, AboutDlgCONTACTOS_MODIFICACI40002);
			return true;

		case ID_CITAS_AGREGAR:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CITAS_AGREGAR), hDlg, AboutDlgCITAS_AGREGAR);
			return true;
		
		case ID_CITAS_MODIFICACI40004:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CITAS_MODIFICACI40004), hDlg, AboutDlgCITAS_MODIFICACI40004);
			return true;

		case ID_CONSULTA_CONTACTOSEXISTENTES:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CONSULTA_CONTACTOSEXISTENTES), hDlg, AboutDlgCONSULTA_CONTACTOSEXISTENTES);
			return true;

		case ID_CITASEXISTENTES_SEMANALES:
			DialogBox(ghAppInst, MAKEINTRESOURCE(IDD_CITASEXISTENTES_SEMANALES), hDlg, AboutDlgCITASEXISTENTES_SEMANALES);
			return true;

		}
		return true;

	case WM_CLOSE:
		DestroyWindow(hDlg);
		return true;

	case WM_DESTROY:
		//Guardar contactos
		fstream FileContac;
		FileContac.open(direccion, ios::out | ios::binary | ios::trunc);
		if (FileContac.is_open()) {
			auxarchcon = listac;
			while (auxarchcon != NULL) {
				FileContac.write((char*)auxarchcon, sizeof(contactos));
				auxarchcon = auxarchcon->sig;
			}
			FileContac.close();
		} // Finalizar guardar contactos

		  //Guardar Alarmas
		fstream FileAlarma;
		FileAlarma.open(direccion1, ios::out | ios::binary | ios::trunc);
		if (FileAlarma.is_open()) {
			auxarchala = listaa;
			while (auxarchala != NULL) {
				FileAlarma.write((char*)auxarchala, sizeof(alarmas));
				auxarchala = auxarchala->sig;
			}
			FileAlarma.close();
		} // Finalizar guardar alarmas

		PostQuitMessage(0);
		return true;

	}
	return false;
}

BOOL CALLBACK AboutDlgCONTACTOS_AGREGAR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnom, hdirr, hcorreo, htel, hcatego, hcanf;
	static contactos * auxconagr = NULL;
	static HBITMAP bmp02, bmp01;
	UINT Indice;
	static char filename[MAX_PATH], filename2[MAX_PATH], filemusic[MAX_PATH], Comando[MAX_PATH]= "";
	char *file1;

	switch (msg)
	{
	case WM_INITDIALOG:
		hnom = GetDlgItem(hDlg, IDC_CON_ANOM);
		hdirr = GetDlgItem(hDlg, IDC_CON_ADIR);
		hcorreo = GetDlgItem(hDlg, IDC_CON_ACOREL);
		htel = GetDlgItem(hDlg, IDC_CON_ATEL);
		hcatego = GetDlgItem(hDlg, IDC_CON_ACATE);
		hcanf = GetDlgItem(hDlg, IDC_CON_ACANF);
		Llenarcombos(hcatego);

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_AGR_BTNAIMG1:

				file1 = AbrirArchivos(hDlg,1);
				strcpy(filename, file1);
				//La dirección se encuentra en filename
				bmp01 = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_AGR_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01);
			return true;

			case IDC_AGR_BTNAIMG2:

				file1 = AbrirArchivos(hDlg,1);
				strcpy(filename2, file1);
				//La dirección se encuentra en filename
				bmp02 = (HBITMAP)LoadImage(NULL, filename2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_AGR_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp02);
			return true;

			case IDC_AGR_BTNAPLAY:
				strcat(Comando, "open \"");
				strcat(Comando, filemusic);
				strcat(Comando, "\" alias mp3 ");
				mciSendString(Comando, NULL, 0, 0);
				mciSendString("play mp3", NULL, 0, 0);
				strcpy(Comando, "");
				reprodu = true;

			return true;

			case IDC_AGR_BTNAPAUSE:
				mciSendString("pause mp3", NULL, 0, 0);
			return true;

			case IDC_AGR_BTNASTOP:
				mciSendString("stop mp3", NULL, 0, 0);
				mciSendString("close mp3", NULL, 0, 0);
				reprodu = false;
			return true;

			case IDC_AGR_BTNACANCEL:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}
				EndDialog(hDlg, 0);
				return true;

			case IDC_AGR_BTNAEXMMP:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}
				file1 = AbrirArchivos(hDlg, 2);
				strcpy(filemusic, file1);
				SendMessage(hcanf, WM_SETTEXT, 260, (LPARAM)filemusic);
			return true;

			case IDC_AGR_BTNAAGRCON:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}
				contactos * nuevoconta = new contactos;
				SendMessage(hnom, WM_GETTEXT, 50, (LPARAM)nuevoconta->nombre);
				SendMessage(hdirr, WM_GETTEXT, 256, (LPARAM)nuevoconta->direccion);
				SendMessage(hcorreo, WM_GETTEXT, 80, (LPARAM)nuevoconta->correo);
				SendMessage(htel, WM_GETTEXT, 30, (LPARAM)nuevoconta->telefono);
				strcpy(nuevoconta->cancion, filemusic);
				strcpy(nuevoconta->imagen1, filename);
				strcpy(nuevoconta->imagen2, filename2);	
				Indice = SendMessage(hcatego, CB_GETCURSEL, 0, 0);
				SendMessage(hcatego, CB_GETLBTEXT, (WPARAM)Indice, (LPARAM)nuevoconta->catego);
				SendMessage(hcanf, WM_GETTEXT, 256, (LPARAM)nuevoconta->nomcanc);

				if ( (*(nuevoconta->nombre) == NULL) || (*(nuevoconta->direccion) == NULL) || (*(nuevoconta->correo) == NULL) || (*(nuevoconta->telefono) == NULL)
					|| (*(nuevoconta->cancion) == NULL) || (*(nuevoconta->imagen1) == NULL) || (*(nuevoconta->imagen2) == NULL)) {
					MessageBox(hDlg, "Faltan datos por capturar", "AVISO", MB_OK + MB_ICONERROR);
					delete nuevoconta;
				}
				else {
					nuevoconta->sig = NULL;
					nuevoconta->prev = NULL;
					if (listac == NULL) {
						listac = nuevoconta;
					}
					else {
						auxconagr = listac;
						while (auxconagr->sig != NULL) {
							auxconagr = auxconagr->sig;
						}
						auxconagr->sig = nuevoconta;
						nuevoconta->prev = auxconagr;
					}
					MessageBox(hDlg, "Contacto agregado correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
					SendMessage(hnom, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hdirr, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hcorreo, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(htel, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hcanf, WM_SETTEXT, 10, (LPARAM)"");
					SendDlgItemMessage(hDlg, IDC_AGR_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, NULL);
					SendDlgItemMessage(hDlg, IDC_AGR_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, NULL);

				}
			return true;
		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		if (reprodu == true) {
			mciSendString("stop mp3", NULL, 0, 0);
			mciSendString("close mp3", NULL, 0, 0);
			reprodu = false;
		}
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

BOOL CALLBACK AboutDlgCONTACTOS_MODIFICACI40002(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnomm, hdirrm, hcorreom, htelm, hcategom, hcanfm, hcriteriom, hnombusm;
	static contactos * auxconmod = NULL;
	static HBITMAP bmp01m;
	UINT Indicemod;
	static contactos *auxpunplasig = NULL, *auxpunplaant = NULL;
	static char filenamem[MAX_PATH], filenamem2[MAX_PATH], filemusicm[MAX_PATH], Comandom[MAX_PATH] = "";
	char *file1;

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listac == NULL)
		{
			EndDialog(hDlg, 0);
			return true;
		}
		hnomm = GetDlgItem(hDlg, IDC_MOD_MNOM);
		hdirrm = GetDlgItem(hDlg, IDC_MOD_MDIR);
		hcorreom = GetDlgItem(hDlg, IDC_MOD_CDIR);
		htelm = GetDlgItem(hDlg, IDC_MOD_MTEL);
		hcategom = GetDlgItem(hDlg, IDC_MOD_MCATE);
		hcanfm = GetDlgItem(hDlg, IDC_MOD_MCANF);
		hcriteriom = GetDlgItem(hDlg, IDC_MOD_CATBUSC);
		hnombusm = GetDlgItem(hDlg, IDC_MOD_NOMBBUS);
		auxconmod = listac;
		SendMessage(hcriteriom, CB_ADDSTRING, 0, (LPARAM)"Todos");
		Llenarcombos(hcriteriom);
		Llenarcombos(hcategom);
		SendMessage(hnomm, WM_SETTEXT, 50, (LPARAM)auxconmod->nombre);
		SendMessage(hdirrm, WM_SETTEXT, 256, (LPARAM)auxconmod->direccion);
		SendMessage(hcorreom, WM_SETTEXT, 80, (LPARAM)auxconmod->correo);
		SendMessage(htelm, WM_SETTEXT, 30, (LPARAM)auxconmod->telefono);
		Indicemod = SendMessage(hcategom, CB_FINDSTRING, 0, (LPARAM)auxconmod->catego);
		SendMessage(hcategom, CB_SETCURSEL, (WPARAM)Indicemod, 0);
		SendMessage(hcanfm, WM_SETTEXT, 256, (LPARAM)auxconmod->nomcanc);
		strcpy(filemusicm, auxconmod->cancion);
		strcpy(filenamem, auxconmod->imagen1);
		bmp01m = (HBITMAP)LoadImage(NULL, filenamem, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
		SendDlgItemMessage(hDlg, IDC_MOD_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
		strcpy(filenamem2, auxconmod->imagen2);
		bmp01m = (HBITMAP)LoadImage(NULL, filenamem2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		SendDlgItemMessage(hDlg, IDC_MOD_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_AGR_BTNMPAUSE:
				mciSendString("pause mp3", NULL, 0, 0);
			return true;

			case IDC_AGR_BTNMPLAY:
				strcat(Comandom, "open \"");
				strcat(Comandom, auxconmod->cancion);
				strcat(Comandom, "\" alias mp3 ");
				mciSendString(Comandom, NULL, 0, 0);
				mciSendString("play mp3", NULL, 0, 0);
				strcpy(Comandom, "");
				reprodu = true;
				return true;

			case IDC_CON_MELIMINAR:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);	
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}
				// contactos *auxpunplasig = NULL, *auxpunplaant = NULL;
				auxpunplaant = auxconmod->prev;
				auxpunplasig = auxconmod->sig;
				if ((auxpunplaant == NULL) && (auxpunplasig == NULL)) {
					listac = NULL;
				}
				else if (auxpunplaant == NULL) {
					listac = listac->sig;
					listac->prev = NULL;
				}
				else if (auxpunplasig == NULL) {
					auxpunplaant->sig = NULL;
				}
				else {
					auxpunplaant->sig = auxpunplasig;
					auxpunplasig->prev = auxpunplaant;
				}
				delete auxconmod;
				auxconmod = listac;
				if (listac != NULL) {
					SendMessage(hnomm, WM_SETTEXT, 50, (LPARAM)auxconmod->nombre);
					SendMessage(hdirrm, WM_SETTEXT, 256, (LPARAM)auxconmod->direccion);
					SendMessage(hcorreom, WM_SETTEXT, 80, (LPARAM)auxconmod->correo);
					SendMessage(htelm, WM_SETTEXT, 30, (LPARAM)auxconmod->telefono);
					Indicemod = SendMessage(hcategom, CB_FINDSTRING, 0, (LPARAM)auxconmod->catego);
					SendMessage(hcategom, CB_SETCURSEL, (WPARAM)Indicemod, 0);
					SendMessage(hcanfm, WM_SETTEXT, 256, (LPARAM)auxconmod->nomcanc);
					strcpy(filenamem, auxconmod->imagen1);
					strcpy(filemusicm, auxconmod->cancion);
					bmp01m = (HBITMAP)LoadImage(NULL, filenamem, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_MOD_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
					strcpy(filenamem2, auxconmod->imagen2);
					bmp01m = (HBITMAP)LoadImage(NULL, filenamem2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_MOD_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
				}
				else {
					MessageBox(hDlg, "Se encuentra eliminada toda la lista", "Aviso", MB_OK + MB_ICONINFORMATION);
				}

			return true;

			case IDC_AGR_BTNMSTOP:
				mciSendString("stop mp3", NULL, 0, 0);
				mciSendString("close mp3", NULL, 0, 0);
				reprodu = false;
				return true;

			case IDC_MOD_BTNAIMG2:
				file1 = AbrirArchivos(hDlg, 1);
				strcpy(filenamem2, file1);
				//La dirección se encuentra en filename
				bmp01m = (HBITMAP)LoadImage(NULL, filenamem2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_MOD_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
				return true;

			case IDC_AGR_BTNAIMG1:
				file1 = AbrirArchivos(hDlg, 1);
				strcpy(filenamem, file1);
				//La dirección se encuentra en filename
				bmp01m = (HBITMAP)LoadImage(NULL, filenamem, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
				//Filename tiene la dirreción de la imagen
				SendDlgItemMessage(hDlg, IDC_MOD_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
				return true;

			case IDC_MOD_CONEXAM:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}
				file1 = AbrirArchivos(hDlg, 2);
				strcpy(filemusicm, file1);
				SendMessage(hcanfm, WM_SETTEXT, 260, (LPARAM)filemusicm);
				return true;

			case IDC_MOD_BTNAAGRCON:
				SendMessage(hnomm, WM_GETTEXT, 50, (LPARAM)auxconmod->nombre);
				SendMessage(hdirrm, WM_GETTEXT, 256, (LPARAM)auxconmod->direccion);
				SendMessage(hcorreom, WM_GETTEXT, 80, (LPARAM)auxconmod->correo);
				SendMessage(htelm, WM_GETTEXT, 30, (LPARAM)auxconmod->telefono);
				SendMessage(hcanfm, WM_GETTEXT, 256, (LPARAM)auxconmod->nomcanc);
				strcpy(auxconmod->cancion, filemusicm);
				strcpy(auxconmod->imagen1, filenamem);
				strcpy(auxconmod->imagen2, filenamem2);
				Indicemod = SendMessage(hcategom, CB_GETCURSEL, 0, 0);
				SendMessage(hcategom, CB_GETLBTEXT, (WPARAM)Indicemod, (LPARAM)auxconmod->catego);

				return true;

			case IDC_MOD_BOTBUSCAR:

				return true;

			case IDC_MOD_BTNMNEXT:
				if (listac == NULL)
				{
					EndDialog(hDlg, 0);
					return true;
				}

				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}

				if (auxconmod->sig != NULL) {
					auxconmod = auxconmod->sig;
					SendMessage(hnomm, WM_SETTEXT, 50, (LPARAM)auxconmod->nombre);
					SendMessage(hdirrm, WM_SETTEXT, 256, (LPARAM)auxconmod->direccion);
					SendMessage(hcorreom, WM_SETTEXT, 80, (LPARAM)auxconmod->correo);
					SendMessage(htelm, WM_SETTEXT, 30, (LPARAM)auxconmod->telefono);
					Indicemod = SendMessage(hcategom, CB_FINDSTRING, 0, (LPARAM)auxconmod->catego);
					SendMessage(hcategom, CB_SETCURSEL, (WPARAM)Indicemod, 0);
					SendMessage(hcanfm, WM_SETTEXT, 256, (LPARAM)auxconmod->nomcanc);
					strcpy(filenamem, auxconmod->imagen1);
					bmp01m = (HBITMAP)LoadImage(NULL, filenamem, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
					strcpy(filemusicm, auxconmod->cancion);
					SendDlgItemMessage(hDlg, IDC_MOD_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
					strcpy(filenamem2, auxconmod->imagen2);
					bmp01m = (HBITMAP)LoadImage(NULL, filenamem2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_MOD_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);

				}
				else {
					MessageBox(hDlg, "Final de la Lista de Contactos", "Aviso", MB_OK + MB_ICONINFORMATION);
				}

				return true;

			case IDC_MOD_BTNMPREV:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}

				if (listac == NULL)
				{
					EndDialog(hDlg, 0);
					return true;
				}

				if (auxconmod->prev != NULL) {
					auxconmod = auxconmod->prev;
					SendMessage(hnomm, WM_SETTEXT, 50, (LPARAM)auxconmod->nombre);
					SendMessage(hdirrm, WM_SETTEXT, 256, (LPARAM)auxconmod->direccion);
					SendMessage(hcorreom, WM_SETTEXT, 80, (LPARAM)auxconmod->correo);
					SendMessage(htelm, WM_SETTEXT, 30, (LPARAM)auxconmod->telefono);
					Indicemod = SendMessage(hcategom, CB_FINDSTRING, 0, (LPARAM)auxconmod->catego);
					SendMessage(hcategom, CB_SETCURSEL, (WPARAM)Indicemod, 0);
					SendMessage(hcanfm, WM_SETTEXT, 256, (LPARAM)auxconmod->nomcanc);
					strcpy(filemusicm, auxconmod->cancion);
					strcpy(filenamem, auxconmod->imagen1);
					bmp01m = (HBITMAP)LoadImage(NULL, filenamem, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_MOD_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);
					strcpy(filenamem2, auxconmod->imagen2);
					bmp01m = (HBITMAP)LoadImage(NULL, filenamem2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_MOD_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01m);

				}
				else {
					MessageBox(hDlg, "Final de la Lista de Contactos", "Aviso", MB_OK + MB_ICONINFORMATION);
				}

				return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		if (reprodu == true) {
			mciSendString("stop mp3", NULL, 0, 0);
			mciSendString("close mp3", NULL, 0, 0);
			reprodu = false;
		}
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

BOOL CALLBACK AboutDlgCITAS_AGREGAR(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnomcit, hfechacit, hsonala, hdesc, hcontex, hconagr, hhora, hmin;
	UINT index;
	static alarmas * auxalarmagr;
	contactos * auxcontcit;
	static char texto[256], auxselp[256];
	static int i;
	switch (msg)
	{
	case WM_INITDIALOG:
		if (listac == NULL)
		{
			MessageBox(hDlg, "Por favor agregue Contactos", "AVISO", MB_OK + MB_ICONINFORMATION);
			EndDialog(hDlg, 0);
			return true;
		}
		hnomcit = GetDlgItem(hDlg, IDC_CIT_ANOM);
		hfechacit = GetDlgItem(hDlg, IDC_CIT_AFECITA);
		hsonala = GetDlgItem(hDlg, IDC_CIT_ASONIDO);
		hdesc = GetDlgItem(hDlg, IDC_CIT_ADESC);
		hcontex = GetDlgItem(hDlg, IDC_CIT_ALISTEXI);
		hconagr = GetDlgItem(hDlg, IDC_CIT_ALISTAGR);
		hhora = GetDlgItem(hDlg, IDC_CIT_AHORA);
		hmin = GetDlgItem(hDlg, IDC_CIT_AMIN);
		Llenaminuto(hhora, 1);
		Llenaminuto(hmin, 2);
		auxcontcit = listac;
		while(auxcontcit != NULL) {
			SendMessage(hcontex, LB_ADDSTRING, 0, (LPARAM)auxcontcit->nombre);
			auxcontcit = auxcontcit->sig;
		}
		SendMessage(hsonala, CB_ADDSTRING, 0, (LPARAM)nompred1);
		SendMessage(hsonala, CB_ADDSTRING, 0, (LPARAM)nompred2);

		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

			case IDC_CIT_ABOTEX:
				index = SendMessage(hcontex, LB_GETCURSEL, 0, 0);
				SendMessage(hcontex, LB_GETTEXT, index, (LPARAM)texto);
				SendMessage(hconagr, LB_ADDSTRING, 0, (LPARAM)texto);

				auxcontcit = listac;
				while (auxcontcit != NULL) {
					if (strcmp(auxcontcit->nombre, texto) == 0) {
						SendMessage(hsonala, CB_INSERTSTRING, 0, (LPARAM)auxcontcit->nomcanc);
					}
					auxcontcit = auxcontcit->sig;
				}

			return true;

			case IDC_CIT_ABOTDEL:
				index = SendMessage(hconagr, LB_GETCURSEL, 0, 0);
				SendMessage(hconagr, LB_GETTEXT, index, (LPARAM)texto);
				SendMessage(hconagr, LB_DELETESTRING, index, 0);
				auxcontcit = listac;
				while (auxcontcit != NULL) {
					if (strcmp(auxcontcit->nombre, texto) == 0) {
						index = SendMessage(hsonala, CB_FINDSTRING, 0, (LPARAM)auxcontcit->nomcanc);
						SendMessage(hsonala, CB_DELETESTRING, (WPARAM)index, 0);
					}
					auxcontcit = auxcontcit->sig;
				}
				
			return true;

			case IDC_CIT_AAGRALA:
				alarmas * nuevoalarma = new alarmas;
				SendMessage(hnomcit, WM_GETTEXT, 50, (LPARAM)nuevoalarma->nomcita);

				GetWindowText(hfechacit, (LPTSTR)nuevoalarma->fechacit, 20);

				index = SendMessage(hhora, CB_GETCURSEL, 0, 0);
				SendMessage(hhora, CB_GETLBTEXT, (WPARAM)index, (LPARAM)nuevoalarma->hora);

				index = SendMessage(hmin, CB_GETCURSEL, 0, 0);
				SendMessage(hmin, CB_GETLBTEXT, (WPARAM)index, (LPARAM)nuevoalarma->min);

				index = SendMessage(hsonala, CB_GETCURSEL, 0, 0);
				SendMessage(hsonala, CB_GETLBTEXT, (WPARAM)index, (LPARAM)nuevoalarma->nomson);

				auxcontcit = listac;
				while (auxcontcit != NULL) {
					if (strcmp(auxcontcit->nomcanc, nuevoalarma->nomson) == 0) {
						strcpy(nuevoalarma->rutason, auxcontcit->cancion);
					}
					auxcontcit = auxcontcit->sig;
				}

				if (strcmp(nompred1, nuevoalarma->nomson) == 0) {
					strcpy(nuevoalarma->rutason, predeterminada1.c_str());
				}
				if (strcmp(nompred2, nuevoalarma->nomson) == 0)
				{
					strcpy(nuevoalarma->rutason, predeterminada2.c_str());
				}


				SendMessage(hdesc, WM_GETTEXT, 256, (LPARAM)nuevoalarma->descripcion);

				index = SendMessage(hconagr, LB_GETCOUNT, 0, 0);
				while (i < index)
				{
					if (i<1) {
						SendMessage(hconagr, LB_GETTEXT, i, (LPARAM)nuevoalarma->contactos);
					}
					else {
						SendMessage(hconagr, LB_GETTEXT, i, (LPARAM)auxselp);
						strcat(nuevoalarma->contactos, "/");
						strcat(nuevoalarma->contactos, auxselp);
					}
					i++;
				}
				i = 0;

				if ((*(nuevoalarma->nomcita) == NULL) || (*(nuevoalarma->fechacit) == NULL) || (*(nuevoalarma->hora) == NULL) || (*(nuevoalarma->min) == NULL)
					|| (*(nuevoalarma->nomson) == NULL) || (*(nuevoalarma->rutason) == NULL) || (*(nuevoalarma->descripcion) == NULL) || (*(nuevoalarma->contactos) == NULL)) {
					MessageBox(hDlg, "Faltan datos por capturar", "AVISO", MB_OK + MB_ICONERROR);
					delete nuevoalarma;
				}
				else {
					nuevoalarma->sig = NULL;
					nuevoalarma->prev = NULL;
					if (listaa == NULL) {
						listaa = nuevoalarma;
					}
					else {
						auxalarmagr = listaa;
						while (auxalarmagr->sig != NULL) {
							auxalarmagr = auxalarmagr->sig;
						}
						auxalarmagr->sig = nuevoalarma;
						nuevoalarma->prev = auxalarmagr;
					}
					MessageBox(hDlg, "Cita agendada correctamente", "AVISO", MB_OK + MB_ICONINFORMATION);
					SendMessage(hnomcit, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hdesc, WM_SETTEXT, 10, (LPARAM)"");
					SendMessage(hconagr, LB_RESETCONTENT, 0, 0);
					SendMessage(hfechacit, CB_RESETCONTENT, 0, 0);
				}

			return true;
		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

BOOL CALLBACK AboutDlgCITAS_MODIFICACI40004(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnomcitm, hfechacitm, hsonalam, hdescm, hcontexm, hconagrm, hhoram, hminm, hbusq;

	UINT indexmod;
	static alarmas * auxalarmmod;
	contactos * auxcontcitmod;
	char *pch;
	static alarmas *auxpunalasig = NULL, *auxpunalaant = NULL;
	static char textomod[256], auxselpmod[256], auxpchmodala[256];
	static int im;

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listaa == NULL)
		{
			EndDialog(hDlg, 0);
			return true;
		}
		hnomcitm = GetDlgItem(hDlg, IDC_CIT_MNOM);
		hfechacitm = GetDlgItem(hDlg, IDC_CIT_MFECITA);
		hsonalam = GetDlgItem(hDlg, IDC_CIT_MSONIDO);
		hcontexm = GetDlgItem(hDlg, IDC_CIT_MLISTEXI);
		hconagrm = GetDlgItem(hDlg, IDC_CIT_MLISTAGR);
		hhoram = GetDlgItem(hDlg, IDC_CIT_MHORA);
		hminm = GetDlgItem(hDlg, IDC_CIT_MMIN);
		hbusq = GetDlgItem(hDlg, IDC_CIT_MNOMBUS);
		hdescm = GetDlgItem(hDlg, IDC_CIT_MDESC);
		SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred1);
		SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred2);
		Llenaminuto(hhoram, 1);
		Llenaminuto(hminm, 2);
		auxcontcitmod = listac;
		while (auxcontcitmod != NULL) {
			SendMessage(hcontexm, LB_ADDSTRING, 0, (LPARAM)auxcontcitmod->nombre);
			auxcontcitmod = auxcontcitmod->sig;
		}
		auxalarmmod = listaa;
		SendMessage(hnomcitm, WM_SETTEXT, 50, (LPARAM)auxalarmmod->nomcita);
		SendMessage(hfechacitm, WM_SETTEXT, 20, (LPARAM)auxalarmmod->fechacit);
		indexmod = SendMessage(hhoram, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->hora);
		SendMessage(hhoram, CB_SETCURSEL, (WPARAM)indexmod, 0);
		indexmod = SendMessage(hminm, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->min);
		SendMessage(hminm, CB_SETCURSEL, (WPARAM)indexmod, 0);
		SendMessage(hdescm, WM_SETTEXT, 256, (LPARAM)auxalarmmod->descripcion);
		strcpy(auxpchmodala, auxalarmmod->contactos);
		pch = strtok(auxpchmodala, "/");
		while (pch != NULL)
		{
			SendMessage(hconagrm, LB_ADDSTRING, 0, (LPARAM)pch);
			auxcontcitmod = listac;
			while (auxcontcitmod != NULL) {
				if (strcmp(pch, auxcontcitmod->nombre) == 0) {
					SendMessage(hsonalam, CB_INSERTSTRING, 0, (LPARAM)auxcontcitmod->nomcanc);
				}
				auxcontcitmod = auxcontcitmod->sig;
			}
			pch = strtok(NULL, "/");
		}

		indexmod = SendMessage(hsonalam, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->nomson);
		SendMessage(hsonalam, CB_SETCURSEL, (WPARAM)indexmod, 0);
		
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_CIT_MELIMINAR:
			//alarmas *auxpunalasig = NULL, *auxpunalaant = NULL;
			auxpunalaant = auxalarmmod->prev;
			auxpunalasig = auxalarmmod->sig;
			if ((auxpunalaant == NULL) && (auxpunalasig == NULL)) {
				listaa = NULL;
			}
			else if (auxpunalaant == NULL) {
				listaa = listaa->sig;
				listaa->prev = NULL;
			}
			else if (auxpunalasig == NULL) {
				auxpunalaant->sig = NULL;
			}
			else {
				auxpunalaant->sig = auxpunalasig;
				auxpunalasig->prev = auxpunalaant;
			}
			delete auxalarmmod;
			auxalarmmod = listaa;
			if (listaa != NULL) {
				SendMessage(hconagrm, LB_RESETCONTENT, 0, 0);
				SendMessage(hsonalam, CB_RESETCONTENT, 0, 0);
				SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred1);
				SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred2);
				SendMessage(hnomcitm, WM_SETTEXT, 50, (LPARAM)auxalarmmod->nomcita);
				SendMessage(hfechacitm, WM_SETTEXT, 20, (LPARAM)auxalarmmod->fechacit);
				indexmod = SendMessage(hhoram, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->hora);
				SendMessage(hhoram, CB_SETCURSEL, (WPARAM)indexmod, 0);
				indexmod = SendMessage(hminm, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->min);
				SendMessage(hminm, CB_SETCURSEL, (WPARAM)indexmod, 0);
				SendMessage(hdescm, WM_SETTEXT, 256, (LPARAM)auxalarmmod->descripcion);
				strcpy(auxpchmodala, auxalarmmod->contactos);
				pch = strtok(auxpchmodala, "/");
				while (pch != NULL)
				{
					SendMessage(hconagrm, LB_ADDSTRING, 0, (LPARAM)pch);
					auxcontcitmod = listac;
					while (auxcontcitmod != NULL) {
						if (strcmp(pch, auxcontcitmod->nombre) == 0) {
							SendMessage(hsonalam, CB_INSERTSTRING, 0, (LPARAM)auxcontcitmod->nomcanc);
						}
						auxcontcitmod = auxcontcitmod->sig;
					}
					pch = strtok(NULL, "/");
				}
				indexmod = SendMessage(hsonalam, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->nomson);
				SendMessage(hsonalam, CB_SETCURSEL, (WPARAM)indexmod, 0);
			}
			else {
				MessageBox(hDlg, "Se encuentra eliminada toda la lista", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
		return true;

		case IDC_CIT_MGUARDARC:
			SendMessage(hnomcitm, WM_GETTEXT, 50, (LPARAM)auxalarmmod->nomcita);
			SendMessage(hfechacitm, WM_GETTEXT, 50, (LPARAM)auxalarmmod->fechacit);

			indexmod = SendMessage(hhoram, CB_GETCURSEL, 0, 0);
			SendMessage(hhoram, CB_GETLBTEXT, (WPARAM)indexmod, (LPARAM)auxalarmmod->hora);

			indexmod = SendMessage(hminm, CB_GETCURSEL, 0, 0);
			SendMessage(hminm, CB_GETLBTEXT, (WPARAM)indexmod, (LPARAM)auxalarmmod->min);

			indexmod = SendMessage(hsonalam, CB_GETCURSEL, 0, 0);
			SendMessage(hsonalam, CB_GETLBTEXT, (WPARAM)indexmod, (LPARAM)auxalarmmod->nomson);

			auxcontcitmod = listac;
			while (auxcontcitmod != NULL) {
				if (strcmp(auxcontcitmod->nomcanc, auxalarmmod->nomson) == 0) {
					strcpy(auxalarmmod->rutason, auxcontcitmod->cancion);
				}
				auxcontcitmod = auxcontcitmod->sig;
			}

			if (strcmp(nompred1, auxalarmmod->nomson) == 0) {
				strcpy(auxalarmmod->rutason, predeterminada1.c_str());
			}
			if (strcmp(nompred2, auxalarmmod->nomson) == 0)
			{
				strcpy(auxalarmmod->rutason, predeterminada2.c_str());
			}

			SendMessage(hdescm, WM_GETTEXT, 256, (LPARAM)auxalarmmod->descripcion);

			indexmod = SendMessage(hconagrm, LB_GETCOUNT, 0, 0);
			while (im < indexmod)
			{
				if (im<1) {
					SendMessage(hconagrm, LB_GETTEXT, im, (LPARAM)auxalarmmod->contactos);
				}
				else {
					SendMessage(hconagrm, LB_GETTEXT, im, (LPARAM)auxselpmod);
					strcat(auxalarmmod->contactos, "/");
					strcat(auxalarmmod->contactos, auxselpmod);
				}
				im++;
			}
			im = 0;

		return true;

		case IDC_CIT_MBOTEX:
			indexmod = SendMessage(hcontexm, LB_GETCURSEL, 0, 0);
			SendMessage(hcontexm, LB_GETTEXT, indexmod, (LPARAM)textomod);
			SendMessage(hconagrm, LB_ADDSTRING, 0, (LPARAM)textomod);

			auxcontcitmod = listac;
			while (auxcontcitmod != NULL) {
				if (strcmp(auxcontcitmod->nombre, textomod) == 0) {
					SendMessage(hsonalam, CB_INSERTSTRING, 0, (LPARAM)auxcontcitmod->nomcanc);
				}
				auxcontcitmod = auxcontcitmod->sig;
			}
			return true;

		case IDC_CIT_MBOTDEL:
			indexmod = SendMessage(hconagrm, LB_GETCURSEL, 0, 0);
			SendMessage(hconagrm, LB_GETTEXT, indexmod, (LPARAM)textomod);
			SendMessage(hconagrm, LB_DELETESTRING, indexmod, 0);
			auxcontcitmod = listac;
			while (auxcontcitmod != NULL) {
				if (strcmp(auxcontcitmod->nombre, textomod) == 0) {
					indexmod = SendMessage(hsonalam, CB_FINDSTRING, 0, (LPARAM)auxcontcitmod->nomcanc);
					SendMessage(hsonalam, CB_DELETESTRING, (WPARAM)indexmod, 0);
				}
				auxcontcitmod = auxcontcitmod->sig;
			}
			return true;

		case IDC_CIT_MSIG:

			if (listaa == NULL)
			{
				EndDialog(hDlg, 0);
				return true;
			}

			if (auxalarmmod->sig != NULL) {
				SendMessage(hconagrm, LB_RESETCONTENT, 0, 0);
				SendMessage(hsonalam, CB_RESETCONTENT, 0, 0);
				SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred1);
				SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred2);

				auxalarmmod = auxalarmmod->sig;
				SendMessage(hnomcitm, WM_SETTEXT, 50, (LPARAM)auxalarmmod->nomcita);
				SendMessage(hfechacitm, WM_SETTEXT, 20, (LPARAM)auxalarmmod->fechacit);
				indexmod = SendMessage(hhoram, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->hora);
				SendMessage(hhoram, CB_SETCURSEL, (WPARAM)indexmod, 0);
				indexmod = SendMessage(hminm, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->min);
				SendMessage(hminm, CB_SETCURSEL, (WPARAM)indexmod, 0);
				SendMessage(hdescm, WM_SETTEXT, 256, (LPARAM)auxalarmmod->descripcion);
				strcpy(auxpchmodala, auxalarmmod->contactos);
				pch = strtok(auxpchmodala, "/");
				while (pch != NULL)
				{
					SendMessage(hconagrm, LB_ADDSTRING, 0, (LPARAM)pch);
					auxcontcitmod = listac;
					while (auxcontcitmod != NULL) {
						if (strcmp(pch, auxcontcitmod->nombre) == 0) {
							SendMessage(hsonalam, CB_INSERTSTRING, 0, (LPARAM)auxcontcitmod->nomcanc);
						}
						auxcontcitmod = auxcontcitmod->sig;
					}
					pch = strtok(NULL, "/");
				}

				indexmod = SendMessage(hsonalam, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->nomson);
				SendMessage(hsonalam, CB_SETCURSEL, (WPARAM)indexmod, 0);
			}
			else {
				MessageBox(hDlg, "Final de la Lista de alarmas", "Aviso", MB_OK + MB_ICONINFORMATION);
			}

			return true;

		case IDC_CIT_MPREV:
			if (listaa == NULL)
			{
				EndDialog(hDlg, 0);
				return true;
			}

			if (auxalarmmod->prev != NULL) {
				SendMessage(hconagrm, LB_RESETCONTENT, 0, 0);
				SendMessage(hsonalam, CB_RESETCONTENT, 0, 0);
				SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred1);
				SendMessage(hsonalam, CB_ADDSTRING, 0, (LPARAM)nompred2);
				auxalarmmod = auxalarmmod->prev;
				SendMessage(hnomcitm, WM_SETTEXT, 50, (LPARAM)auxalarmmod->nomcita);
				SendMessage(hfechacitm, WM_SETTEXT, 20, (LPARAM)auxalarmmod->fechacit);
				indexmod = SendMessage(hhoram, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->hora);
				SendMessage(hhoram, CB_SETCURSEL, (WPARAM)indexmod, 0);
				indexmod = SendMessage(hminm, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->min);
				SendMessage(hminm, CB_SETCURSEL, (WPARAM)indexmod, 0);
				SendMessage(hdescm, WM_SETTEXT, 256, (LPARAM)auxalarmmod->descripcion);
				strcpy(auxpchmodala, auxalarmmod->contactos);
				pch = strtok(auxpchmodala, "/");
				while (pch != NULL)
				{
					SendMessage(hconagrm, LB_ADDSTRING, 0, (LPARAM)pch);
					auxcontcitmod = listac;
					while (auxcontcitmod != NULL) {
						if (strcmp(pch, auxcontcitmod->nombre) == 0) {
							SendMessage(hsonalam, CB_INSERTSTRING, 0, (LPARAM)auxcontcitmod->nomcanc);
						}
						auxcontcitmod = auxcontcitmod->sig;
					}
					pch = strtok(NULL, "/");
				}

				indexmod = SendMessage(hsonalam, CB_FINDSTRING, 0, (LPARAM)auxalarmmod->nomson);
				SendMessage(hsonalam, CB_SETCURSEL, (WPARAM)indexmod, 0);
			}
			else {
				MessageBox(hDlg, "Final de la Lista de alarmas", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
			return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

BOOL CALLBACK AboutDlgCONSULTA_CONTACTOSEXISTENTES(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hnomc, hdirrc, hcorreoc, htelc, hcategoc, hcanfc,hcriterio;
	static contactos * auxconcon = NULL;
	static HBITMAP bmp01c;
	UINT Indicecon;
	static char filenamec[MAX_PATH], filemusicc[MAX_PATH], Comandoc[MAX_PATH] = "";
	char *file1;

	switch (msg)
	{
	case WM_INITDIALOG:
		if (listac == NULL)
		{
			EndDialog(hDlg, 0);
			return true;
		}
		hnomc = GetDlgItem(hDlg, IDC_CON_CNOM);
		hdirrc = GetDlgItem(hDlg, IDC_CON_CDIR);
		hcorreoc = GetDlgItem(hDlg, IDC_CON_CCOREL);
		htelc = GetDlgItem(hDlg, IDC_CON_CTEL);
		hcategoc = GetDlgItem(hDlg, IDC_CON_CCATE);
		hcanfc = GetDlgItem(hDlg, IDC_CON_CCANF);
		hcriterio = GetDlgItem(hDlg, IDC_CON_CRITERIOCON);
		SendMessage(hcriterio, CB_ADDSTRING, 0, (LPARAM)"Nombre");
		SendMessage(hcriterio, CB_ADDSTRING, 0, (LPARAM)"Todos");
		Llenarcombos(hcriterio);
		auxconcon = listac;
		SendMessage(hnomc, WM_SETTEXT, 50, (LPARAM)auxconcon->nombre);
		SendMessage(hdirrc, WM_SETTEXT, 256, (LPARAM)auxconcon->direccion);
		SendMessage(hcorreoc, WM_SETTEXT, 80, (LPARAM)auxconcon->correo);
		SendMessage(htelc, WM_SETTEXT, 30, (LPARAM)auxconcon->telefono);
		SendMessage(hcategoc, WM_SETTEXT, 30, (LPARAM)auxconcon->catego);
		SendMessage(hcanfc, WM_SETTEXT, 256, (LPARAM)auxconcon->nomcanc);
		strcpy(filenamec, auxconcon->imagen1);
		bmp01c = (HBITMAP)LoadImage(NULL, filenamec, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
		SendDlgItemMessage(hDlg, IDC_CON_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01c);
		strcpy(filenamec, auxconcon->imagen2);
		bmp01c = (HBITMAP)LoadImage(NULL, filenamec, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		SendDlgItemMessage(hDlg, IDC_CON_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01c);


		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
			case IDC_AGR_BTNCPLAY:
				strcat(Comandoc, "open \"");
				strcat(Comandoc, auxconcon->cancion);
				strcat(Comandoc, "\" alias mp3 ");
				mciSendString(Comandoc, NULL, 0, 0);
				mciSendString("play mp3", NULL, 0, 0);
				strcpy(Comandoc, "");
				reprodu = true;
			return true;

			case IDC_AGR_BTNCPAUSE:
				mciSendString("pause mp3", NULL, 0, 0);
				return true;

			case IDC_AGR_BTNCSTOP:
				mciSendString("stop mp3", NULL, 0, 0);
				mciSendString("close mp3", NULL, 0, 0);
				reprodu = false;
				return true;

			case IDC_CON_BTNCNEXT:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}

				if (auxconcon->sig != NULL) {
					auxconcon = auxconcon->sig;
					SendMessage(hnomc, WM_SETTEXT, 50, (LPARAM)auxconcon->nombre);
					SendMessage(hdirrc, WM_SETTEXT, 256, (LPARAM)auxconcon->direccion);
					SendMessage(hcorreoc, WM_SETTEXT, 80, (LPARAM)auxconcon->correo);
					SendMessage(htelc, WM_SETTEXT, 30, (LPARAM)auxconcon->telefono);
					SendMessage(hcategoc, WM_SETTEXT, 30, (LPARAM)auxconcon->catego);
					SendMessage(hcanfc, WM_SETTEXT, 256, (LPARAM)auxconcon->nomcanc);
					strcpy(filenamec, auxconcon->imagen1);
					bmp01c = (HBITMAP)LoadImage(NULL, filenamec, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_CON_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01c);
					strcpy(filenamec, auxconcon->imagen2);
					bmp01c = (HBITMAP)LoadImage(NULL, filenamec, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_CON_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01c);
				}
				else {
					MessageBox(hDlg, "Final de la Lista de Contactos", "Aviso", MB_OK + MB_ICONINFORMATION);
				}
			return true;

			case IDC_CON_BTNCPREV:
				if (reprodu == true) {
					mciSendString("stop mp3", NULL, 0, 0);
					mciSendString("close mp3", NULL, 0, 0);
					reprodu = false;
				}

				if (auxconcon->prev != NULL) {
					auxconcon = auxconcon->prev;
					SendMessage(hnomc, WM_SETTEXT, 50, (LPARAM)auxconcon->nombre);
					SendMessage(hdirrc, WM_SETTEXT, 256, (LPARAM)auxconcon->direccion);
					SendMessage(hcorreoc, WM_SETTEXT, 80, (LPARAM)auxconcon->correo);
					SendMessage(htelc, WM_SETTEXT, 30, (LPARAM)auxconcon->telefono);
					SendMessage(hcategoc, WM_SETTEXT, 30, (LPARAM)auxconcon->catego);
					SendMessage(hcanfc, WM_SETTEXT, 256, (LPARAM)auxconcon->nomcanc);
					strcpy(filenamec, auxconcon->imagen1);
					bmp01c = (HBITMAP)LoadImage(NULL, filenamec, IMAGE_BITMAP, 310, 270, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_CON_BMPAGRN, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01c);
					strcpy(filenamec, auxconcon->imagen2);
					bmp01c = (HBITMAP)LoadImage(NULL, filenamec, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_CON_BMPAPQA, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp01c);
				}
				else {
					MessageBox(hDlg, "Final de la Lista de Contactos", "Aviso", MB_OK + MB_ICONINFORMATION);
				}
				return true;

		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		if (reprodu == true) {
			mciSendString("stop mp3", NULL, 0, 0);
			mciSendString("close mp3", NULL, 0, 0);
			reprodu = false;
		}
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

BOOL CALLBACK AboutDlgCITASEXISTENTES_SEMANALES(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hdia1, hdia2, hdia3, hdia4, hdia5, hdia6, hdia7, hfechahoy,hdiacompleto, hmes, hhoraact, hevent1, hevent2, hevent3, hevent4, hevent5, hevent6, hevent7;
	static char fecha[80], hora[80], dia[80], auxpchfecha[80], diaact[10], mesact[10], anioact[10];
	static char mesesito[20],fechamodificada[80], fechamodificada2[80], fechamodificada3[80], fechamodificada4[80], fechamodificada5[80], fechamodificada6[80];
	static int l=1, audiaact, aumesact, auanioact;
	static contactos * auxcontactos;
	static alarmas * auxalarmas;
	char *fechamod;

	char *pch;
	switch (msg)
	{
	case WM_INITDIALOG:

		hdia1 = GetDlgItem(hDlg, IDC_SEM_DIA1);
		hdia2 = GetDlgItem(hDlg, IDC_SEM_DIA2);
		hdia3 = GetDlgItem(hDlg, IDC_SEM_DIA3);
		hdia4 = GetDlgItem(hDlg, IDC_SEM_DIA4);
		hdia5 = GetDlgItem(hDlg, IDC_SEM_DIA5);
		hdia6 = GetDlgItem(hDlg, IDC_SEM_DIA6);
		hdia7 = GetDlgItem(hDlg, IDC_SEM_DIA7);
		hmes = GetDlgItem(hDlg, IDC_SEM_MESACT);
		hfechahoy = GetDlgItem(hDlg, IDC_SEM_FECHAACT);
		hevent1 = GetDlgItem(hDlg, IDC_SEM_EVENT1);
		hevent2 = GetDlgItem(hDlg, IDC_SEM_EVENT2);
		hevent3 = GetDlgItem(hDlg, IDC_SEM_EVENT3);
		hevent4 = GetDlgItem(hDlg, IDC_SEM_EVENT4);
		hevent5 = GetDlgItem(hDlg, IDC_SEM_EVENT5);
		hevent6 = GetDlgItem(hDlg, IDC_SEM_EVENT6);
		hevent7 = GetDlgItem(hDlg, IDC_SEM_EVENT7);
		hhoraact = GetDlgItem(hDlg, IDC_SEM_HORAACT);
		hdiacompleto = GetDlgItem(hDlg, IDC_SEM_DIAACT);
		
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(fecha, 80, "%d/%m/%Y", timeinfo);
		SendMessage(hfechahoy, WM_SETTEXT, 80, (LPARAM)fecha);

		strftime(hora, 80, "%H:%M:%S", timeinfo);
		SendMessage(hhoraact, WM_SETTEXT, 80, (LPARAM)hora);

		strftime(mesesito, 80, "%B", timeinfo);
		SendMessage(hmes, WM_SETTEXT, 20, (LPARAM)mesesito);

		strftime(dia, 80, "%A", timeinfo);
		SendMessage(hdiacompleto, WM_SETTEXT, 80, (LPARAM)dia);

		SendMessage(hdia1, WM_SETTEXT, 80, (LPARAM)dia);

		strcpy(auxpchfecha, fecha);
		pch = strtok(auxpchfecha, "/");
		while (pch != NULL)
		{
			
			if (l == 1) {
				strcpy(diaact, pch);
				l++;
			}
			else if (l == 2) {
				strcpy(mesact, pch);
				l++;
			}
			else {
				strcpy(anioact, pch);
			}

			pch = strtok(NULL, "/");
		}
		l = 1;

		audiaact = atoi(diaact);
		aumesact = atoi(mesact);
		auanioact = atoi(anioact);

		//Dia 2
		timeinfo->tm_year = auanioact - 1900;
		timeinfo->tm_mon = aumesact - 1;
		timeinfo->tm_mday = audiaact + 1;
		/* call mktime: timeinfo->tm_wday will be set */
		mktime(timeinfo);
		SendMessage(hdia2, WM_SETTEXT, 80, (LPARAM)weekday[timeinfo->tm_wday]);

		//Dia 3
		timeinfo->tm_year = auanioact - 1900;
		timeinfo->tm_mon = aumesact - 1;
		timeinfo->tm_mday = audiaact + 2;
		/* call mktime: timeinfo->tm_wday will be set */
		mktime(timeinfo);
		SendMessage(hdia3, WM_SETTEXT, 80, (LPARAM)weekday[timeinfo->tm_wday]);

		//Dia 4
		timeinfo->tm_year = auanioact - 1900;
		timeinfo->tm_mon = aumesact - 1;
		timeinfo->tm_mday = audiaact + 3;
		/* call mktime: timeinfo->tm_wday will be set */
		mktime(timeinfo);
		SendMessage(hdia4, WM_SETTEXT, 80, (LPARAM)weekday[timeinfo->tm_wday]);
		
		//Dia 5
		timeinfo->tm_year = auanioact - 1900;
		timeinfo->tm_mon = aumesact - 1;
		timeinfo->tm_mday = audiaact + 4;
		/* call mktime: timeinfo->tm_wday will be set */
		mktime(timeinfo);
		SendMessage(hdia5, WM_SETTEXT, 80, (LPARAM)weekday[timeinfo->tm_wday]);

		//Dia 6
		timeinfo->tm_year = auanioact - 1900;
		timeinfo->tm_mon = aumesact - 1;
		timeinfo->tm_mday = audiaact + 5;
		/* call mktime: timeinfo->tm_wday will be set */
		mktime(timeinfo);
		SendMessage(hdia6, WM_SETTEXT, 80, (LPARAM)weekday[timeinfo->tm_wday]);

		//Dia 7
		timeinfo->tm_year = auanioact - 1900;
		timeinfo->tm_mon = aumesact - 1;
		timeinfo->tm_mday = audiaact + 6;
		/* call mktime: timeinfo->tm_wday will be set */
		mktime(timeinfo);
		SendMessage(hdia7, WM_SETTEXT, 80, (LPARAM)weekday[timeinfo->tm_wday]);
		//Dia2
		fechamod = funcionfecha(audiaact, aumesact, auanioact,1);
		strcpy(fechamodificada, fechamod);
		//Dia3
		fechamod = funcionfecha(audiaact, aumesact, auanioact, 2);
		strcpy(fechamodificada2, fechamod);
		//Dia4
		fechamod = funcionfecha(audiaact, aumesact, auanioact, 3);
		strcpy(fechamodificada3, fechamod);
		//Dia5
		fechamod = funcionfecha(audiaact, aumesact, auanioact, 4);
		strcpy(fechamodificada4, fechamod);
		//Dia6
		fechamod = funcionfecha(audiaact, aumesact, auanioact, 5);
		strcpy(fechamodificada5, fechamod);
		//Dia7
		fechamod = funcionfecha(audiaact, aumesact, auanioact, 6);
		strcpy(fechamodificada6, fechamod);


		//Mostrarlos en el List de la fecha correspondiente
		auxalarmas = listaa;
		while (auxalarmas != NULL) {
			if (strcmp(auxalarmas->fechacit, fecha) == 0) {
				SendMessage(hevent1, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			if (strcmp(auxalarmas->fechacit, fechamodificada) == 0) {
			SendMessage(hevent2, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			if (strcmp(auxalarmas->fechacit, fechamodificada2) == 0) {
				SendMessage(hevent3, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			if (strcmp(auxalarmas->fechacit, fechamodificada3) == 0) {
				SendMessage(hevent4, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			if (strcmp(auxalarmas->fechacit, fechamodificada4) == 0) {
				SendMessage(hevent5, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			if (strcmp(auxalarmas->fechacit, fechamodificada5) == 0) {
				SendMessage(hevent6, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			if (strcmp(auxalarmas->fechacit, fechamodificada6) == 0) {
				SendMessage(hevent7, LB_ADDSTRING, 0, (LPARAM)auxalarmas->nomcita);
			}
			auxalarmas = auxalarmas->sig;
		}


		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{


		}
		return true;
		// If the dialog was closed (user pressed 'X' button)
		// then terminate the dialog.
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return true;
	case WM_DESTROY:

		return true;
	}
	return false;
}

char *AbrirArchivos(HWND Dialogo, int filtro)
{
	char *nombre = "";
	char szFile[260];       // buffer for file name

	OPENFILENAME ofn;       // common dialog box structure
	HANDLE hf;              // file handle
							// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Dialogo;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	//--------------
	if (filtro == 1)
		ofn.lpstrFilter = "Todos\0*.*\0Mapas de Bits\0*.BMP\0";
	else if (filtro == 2)
		ofn.lpstrFilter = "Todos\0*.*\0Sonidos MP3\0*.MP3\0";
	ofn.nFilterIndex = 2;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//--------------
	if (GetOpenFileName(&ofn) == TRUE)
	{
		nombre = ofn.lpstrFile;
	}
	else
	{
		nombre = "";
		MessageBox(Dialogo, "No eligió archivo", "Aviso", MB_OK | MB_ICONINFORMATION);
	}
	return nombre;
}

void Llenarcombos(HWND handle) {
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Familia");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Amigos");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Colegio");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Conocidos");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Grupo de trabajo");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Favoritos");
}

void Llenaminuto(HWND handle, int desi) {
	char aux[10] = "";

	if (desi == 2) {
		
		for (int i = 59; i > 9; i--) {
			_itoa(i, aux, 10);
			SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)aux);
		}
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"09");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"08");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"07");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"06");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"05");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"04");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"03");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"02");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"01");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"00");

	}
	else {
		for (int i = 23; i > 9; i--) {
			_itoa(i, aux, 10);
			SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)aux);
		}
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"09");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"08");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"07");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"06");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"05");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"04");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"03");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"02");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"01");
		SendMessage(handle, CB_INSERTSTRING, 0, (LPARAM)"00");
	}
	
}

char *funcionfecha(int dia, int mes, int anio, int suma) {
	char *nombre = NULL;
	char auxchar[20];
	char fecha[20];

	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			dia = dia + suma;
			_itoa(dia, auxchar, 10);
			if (dia < 10) {
				strcpy(fecha, "0");
				strcat(fecha, auxchar);
			} else {
				strcpy(fecha, auxchar);
			}
			
		}
		else if (i==1) {
			_itoa(mes, auxchar, 10);
			strcat(fecha, "/");
			if (mes < 10) {
				strcat(fecha, "0");
			}
			strcat(fecha, auxchar);
			
		}
		else {
			_itoa(anio, auxchar, 10);
			strcat(fecha, "/");
			strcat(fecha, auxchar);
			break;
		}

	}
	nombre = fecha;
	return nombre;

}

void Llenarmeses(HWND handle) {
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Enero");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Febrero");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Marzo");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Abril");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Mayo");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Junio");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Julio");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Agosto");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Septiembre");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Octubre");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Noviembre");
	SendMessage(handle, CB_ADDSTRING, 0, (LPARAM)"Diciembre");
}

string obtenerDireccion(string filename) {
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, path, -1, ch, 260, &DefChar, NULL);
	string str(ch);
	size_t found = str.find_last_of("/\\");
	string pathoriginal = str.substr(0, found);
	string fileexecutable = str.substr(found + 1);
	string pathfile = pathoriginal + "\\" + filename;
	return pathfile;
}