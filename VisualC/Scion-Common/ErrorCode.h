 // ErrorCode.mc File
 // The following are the message definitions.
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +-+-+-+-+-+---------------------+-------------------------------+
//  |S|R|C|N|r|    Facility         |               Code            |
//  +-+-+-+-+-+---------------------+-------------------------------+
//
//  where
//
//      S - Severity - indicates success/fail
//
//          0 - Success
//          1 - Fail (COERROR)
//
//      R - reserved portion of the facility code, corresponds to NT's
//              second severity bit.
//
//      C - reserved portion of the facility code, corresponds to NT's
//              C field.
//
//      N - reserved portion of the facility code. Used to indicate a
//              mapped NT status value.
//
//      r - reserved portion of the facility code. Reserved for internal
//              use. Used to indicate HRESULT values that are not status
//              values, but are instead message ids for display strings.
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: E_MMIOERR_ACCESSDENIED
//
// MessageText:
//
// Le fichier est protégé et ne peut pas être ouvert
//
#define E_MMIOERR_ACCESSDENIED           0xE0001000L

//
// MessageId: E_MMIOERR_INVALIDFILE
//
// MessageText:
//
// Le fichier est invalide
//
#define E_MMIOERR_INVALIDFILE            0xE0001001L

//
// MessageId: E_MMIOERR_NETWORKERROR
//
// MessageText:
//
// Le réseau ne répond pas à la demande d’ouverture d’un fichier distant
//
#define E_MMIOERR_NETWORKERROR           0xE0001002L

//
// MessageId: E_MMIOERR_PATHNOTFOUND
//
// MessageText:
//
// La spécification du répertoire est incorrecte
//
#define E_MMIOERR_PATHNOTFOUND           0xE0001003L

//
// MessageId: E_MMIOERR_SHARINGVIOLATION
//
// MessageText:
//
// Le fichier est utilisé par une autre application et n’est pas disponible
//
#define E_MMIOERR_SHARINGVIOLATION       0xE0001004L

//
// MessageId: E_MMIOERR_TOOMANYOPENFILES
//
// MessageText:
//
// Le nombre de fichiers ouverts simultanément est à un niveau maximal
//
#define E_MMIOERR_TOOMANYOPENFILES       0xE0001005L

//
// MessageId: E_MMIOERR_CHUNKNOTFOUND
//
// MessageText:
//
// La fin du fichier (ou la fin du segment parent, le cas échéant) a été atteinte avant que le segment souhaité ait été trouvé
//
#define E_MMIOERR_CHUNKNOTFOUND          0xE0001006L

//
// MessageId: E_MMIOERR_CANNOTSEEK
//
// MessageText:
//
// Une erreur s’est produite lors de la recherche jusqu’à la fin du bloc
//
#define E_MMIOERR_CANNOTSEEK             0xE0001007L

//
// MessageId: E_MMIOERR_CANNOTWRITE
//
// MessageText:
//
// Le contenu de la mémoire tampon n’a pas pu être écrit sur le disque
//
#define E_MMIOERR_CANNOTWRITE            0xE0001008L

//
// MessageId: E_MMIOERR_CANNOTEXPAND
//
// MessageText:
//
// Le fichier mémoire spécifié ne peut pas être développé
//
#define E_MMIOERR_CANNOTEXPAND           0xE0001009L

//
// MessageId: E_MMIOERR_CANNOTREAD
//
// MessageText:
//
// Une erreur s’est produite lors du rechargement de la mémoire tampon
//
#define E_MMIOERR_CANNOTREAD             0xE000100AL

//
// MessageId: E_MMIOERR_OUTOFMEMORY
//
// MessageText:
//
// Il n’y avait pas assez de mémoire pour développer un fichier mémoire pour une écriture ultérieure
//
#define E_MMIOERR_OUTOFMEMORY            0xE000100BL

//
// MessageId: E_MMIOERR_UNBUFFERED
//
// MessageText:
//
// Le fichier spécifié n’est pas ouvert pour les E/S mises en mémoire tampon
//
#define E_MMIOERR_UNBUFFERED             0xE000100CL

//
// MessageId: E_MMIOERR_NOTWAVEFILE
//
// MessageText:
//
// Le fichier n'est pas un fichier .wave
//
#define E_MMIOERR_NOTWAVEFILE            0xE000100DL

//
// MessageId: E_MMIOERR_CORRUPTWAVEFILE
//
// MessageText:
//
// Le fichier .wave est corrompu
//
#define E_MMIOERR_CORRUPTWAVEFILE        0xE000100EL

