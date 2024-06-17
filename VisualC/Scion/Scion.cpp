// Scion.cpp : définit les routines d'initialisation pour la DLL.
//

#include "pch.h"
#include "framework.h"
#include "Scion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: si cette DLL est liée dynamiquement aux DLL MFC,
//		toute fonction exportée de cette DLL qui appelle
//		MFC doit avoir la macro AFX_MANAGE_STATE ajoutée au
//		tout début de la fonction.
//
//		Par exemple :
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// corps de fonction normal ici
//		}
//
//		Il est très important que cette macro se trouve dans chaque
//		fonction, avant tout appel à MFC.  Cela signifie qu'elle
//		doit être la première instruction dans la
//		fonction, avant toute déclaration de variable objet
//		dans la mesure où leurs constructeurs peuvent générer des appels à la DLL
//		MFC.
//
//		Consultez les notes techniques MFC 33 et 58 pour plus de
//		détails.
//

CScionApp theApp;

#pragma region Constructors

CScionApp::CScionApp()
{
	// TODO: ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}

CScionApp::~CScionApp()
{

}

#pragma endregion
#pragma region Overridables

BOOL CScionApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CScionApp::ExitInstance()
{
	// TODO: ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

	return CWinApp::ExitInstance();
}

#pragma endregion
#pragma region Messages

BEGIN_MESSAGE_MAP(CScionApp, CWinApp)
END_MESSAGE_MAP()

#pragma endregion