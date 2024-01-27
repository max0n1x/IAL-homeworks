/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    for(int i = 0; i < HT_SIZE; i++){
        (*table)[i] = NULL; // Pro kazdy prvek tabulky ukladame hodnotu NULL
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    int id = get_hash(key); // Index prvku v tabulke
    ht_item_t *searched_item = (*table)[id]; // Prvok na danom indexe

    while(searched_item != NULL){ // Prechadzame vsetky synonyma
        if(strcmp(searched_item->key, key) == 0){ // Ak sa kluc zhoduje
            return searched_item; // Vratime najdeny prvok
        }
        searched_item = searched_item->next; // Posunieme sa na dalsi prvok
    }
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    int id = get_hash(key); // Index prvku v tabulke
    ht_item_t *searched_item = ht_search(table, key); // Prvok s danym klucom

    if(searched_item != NULL){ // Ak sa prvok s danym klucom nachadza v tabulke
        searched_item->value = value; // Zmenime jeho hodnotu
    } else { // Ak sa prvok s danym klucom nenachadza v tabulke
        ht_item_t *new_item = malloc(sizeof(ht_item_t)); // Vytvorime novy prvok
        new_item->key = key; // Nastavime kluc
        new_item->value = value; // Nastavime hodnotu
        new_item->next = (*table)[id]; // Nastavime nasledujuci prvok
        (*table)[id] = new_item; // Vlozime prvok do tabulky
    }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *searched_item = ht_search(table, key); // Prvok s danym klucom
    if(searched_item != NULL){ // Ak sa prvok s danym klucom nachadza v tabulke
        return &(searched_item->value); // Vratime jeho hodnotu
    } else { // Ak sa prvok s danym klucom nenachadza v tabulke
        return NULL; // Vratime NULL
    }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int id = get_hash(key); // Index prvku v tabulke
    ht_item_t *target = (*table)[id]; // Prvok na danom indexe
    ht_item_t *previous_item = NULL; // Predchadzajuci prvok

    while(target != NULL){ // Prechadzame vsetky synonyma
        if(strcmp(target->key, key) == 0){ // Ak sa kluc zhoduje
            if(previous_item == NULL){ // Ak je prvok prvy v zozname synonym
                (*table)[id] = target->next; // Posunieme sa na dalsi prvok
            } else { // Ak nie je prvok prvy v zozname synonym
                previous_item->next = target->next; // Posunieme sa na dalsi prvok
            }
            free(target); // Uvolnime pamat
            return; // Ukoncime funkciu
        }
        previous_item = target;
        target = target->next; // Posunieme sa na dalsi prvok
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    for(int i = 0; i < HT_SIZE; i++){ // Prechadzame vsetky prvky tabulky
        ht_item_t *target = (*table)[i]; // Prvok na danom indexe
        while(target != NULL){ // Prechadzame vsetky synonyma
            ht_item_t *next_item = target->next; // Nasledujuci prvok
            free(target); // Uvolnime pamat
            target = next_item; // Posunieme sa na dalsi prvok
        }
        (*table)[i] = NULL; // Nastavime hodnotu na NULL
    }
}
