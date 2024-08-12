// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore les performances de génération des futures builds.
// Cela affecte également les performances d'IntelliSense, notamment la complétion du code et de nombreuses fonctionnalités de navigation du code.
// Toutefois, les fichiers listés ici sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les gains de performance.

#ifndef PCH_H
#define PCH_H

// ajouter les en-têtes à précompiler ici
#include "framework.h"

#include <propkey.h>
#include <dwmapi.h>

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Dwmapi.lib")

#include "Messages.h"
#include "Helpers.h"

#define APP_MODE_SDI

#ifdef APP_MODE_MDI
#define FrameWnd CMDIFrameWndEx
#else
#define FrameWnd CFrameWndEx
#endif

#include <Scion/Engine.h>

#endif //PCH_H
