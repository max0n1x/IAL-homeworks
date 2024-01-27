/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */

///** Dvousměrně vázaný seznam. */
//typedef struct {
//    /** Ukazatel na první prvek seznamu. */
//    DLLElementPtr firstElement;
//    /** Ukazatel na aktuální prvek seznamu. */
//    DLLElementPtr activeElement;
//    /** Ukazatel na posledni prvek seznamu. */
//    DLLElementPtr lastElement;
//} DLList;
void DLL_Init( DLList *list ) {

    list->firstElement = list->activeElement = list->lastElement = NULL;

}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    DLLElementPtr element = list->firstElement;

    while (element) {

        DLLElementPtr nextElem = element->nextElement;
        free(element);
        element = nextElem;

    }

    list->firstElement = list->activeElement = list->lastElement = NULL;

}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    DLLElementPtr element = (DLLElementPtr) malloc(sizeof(struct DLLElement));

    if (!element) {

        DLL_Error();
        return;

    }

    element->data = data;
    element->previousElement = NULL;
    element->nextElement = list->firstElement;

    if (list->firstElement) {

        list->firstElement->previousElement = element;

    }

    list->firstElement = element;

    if (!list->lastElement) {

        list->lastElement = element;

    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    DLLElementPtr element = (DLLElementPtr) malloc(sizeof(struct DLLElement));

    if (!element) {

        DLL_Error();
        return;

    }

    element->data = data;
    element->previousElement = list->lastElement;
    element->nextElement = NULL;

    if (list->lastElement) {

        list->lastElement->nextElement = element;

    }

    list->lastElement = element;

    if (!list->firstElement) {

        list->firstElement = element;

    }

}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list->activeElement = list->firstElement;

}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    list->activeElement = list->lastElement;

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if (!list->firstElement) {

        DLL_Error();
        return;

    }

    *dataPtr = list->firstElement->data;

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    if (!list->lastElement) {

        DLL_Error();
        return;

    }

    *dataPtr = list->lastElement->data;

}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    if (!list->firstElement) {

        return;

    }

    DLLElementPtr element = list->firstElement;
    list->firstElement = element->nextElement;

    if (list->firstElement) {

        list->firstElement->previousElement = NULL;

    }

    if (list->activeElement == element) {

        list->activeElement = NULL;

    }

    if (list->lastElement == element) {

        list->lastElement = NULL;

    }

    free(element);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    if (list->lastElement == NULL) {
        return;
    }
    DLLElementPtr lastElement = list->lastElement;
    list->lastElement = lastElement->previousElement;
    if (list->lastElement != NULL) {
        list->lastElement->nextElement = NULL;
    }
    if (list->activeElement == lastElement) {
        list->activeElement = NULL;
    }
    if (list->firstElement == lastElement) {
        list->firstElement = NULL;
    }
    free(lastElement);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    if (list->activeElement == NULL || list->activeElement->nextElement == NULL) {
        return;
    }
    DLLElementPtr element = list->activeElement->nextElement;
    list->activeElement->nextElement = element->nextElement;
    if (element->nextElement != NULL) {
        element->nextElement->previousElement = list->activeElement;
    }
    if (list->lastElement == element) {
        list->lastElement = list->activeElement;
    }
    free(element);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    if (list->activeElement == NULL || list->activeElement->previousElement == NULL) {
        return;
    }
    DLLElementPtr element = list->activeElement->previousElement;
    list->activeElement->previousElement = element->previousElement;
    if (element->previousElement != NULL) {
        element->previousElement->nextElement = list->activeElement;
    }
    if (list->firstElement == element) {
        list->firstElement = list->activeElement;
    }
    free(element);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    if (list->activeElement == NULL) {
        return;
    }
    DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
        DLL_Error();
        return;
    }
    newElement->data = data;
    newElement->previousElement = list->activeElement;
    newElement->nextElement = list->activeElement->nextElement;
    if (list->activeElement->nextElement != NULL) {
        list->activeElement->nextElement->previousElement = newElement;
    }
    list->activeElement->nextElement = newElement;
    if (list->lastElement == list->activeElement) {
        list->lastElement = newElement;
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    if (list->activeElement == NULL) {
        return;
    }
    DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
        DLL_Error();
        return;
    }
    newElement->data = data;
    newElement->previousElement = list->activeElement->previousElement;
    newElement->nextElement = list->activeElement;
    if (list->activeElement->previousElement != NULL) {
        list->activeElement->previousElement->nextElement = newElement;
    }
    list->activeElement->previousElement = newElement;
    if (list->firstElement == list->activeElement) {
        list->firstElement = newElement;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    if (!list->activeElement) {

        DLL_Error();
        return;

    }

    *dataPtr = list->activeElement->data;

}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    if (!list->activeElement) {

        return;

    }

    list->activeElement->data = data;

}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    if (!list->activeElement) {

        return;

    }

    list->activeElement = list->activeElement->nextElement;

}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    if (!list->activeElement) {

        return;

    }

    list->activeElement = list->activeElement->previousElement;

}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    return list->activeElement ? 1 : 0;

}

/* Konec c206.c */
