#pragma once

// TODO: ajoutez ici le code des messages personnalisés 
// Commencez toujours par WM_USER + 1

// Exemple : 
// #define WM_CHANGED_DOCUMENT (WM_USER + 1)
// #define WM_ROI_UPDATED (WM_CHANGED_DOCUMENT + 1)
// etc.

enum EWindowMessage : UINT
{
	WM_UPDATE_PANE = WM_USER + 1
};

enum EPane : WPARAM
{
	EPane_ResourceList = 1 << 0
};