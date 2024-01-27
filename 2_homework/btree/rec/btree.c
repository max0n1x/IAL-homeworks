/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL; // Inicializujeme strom na NULL
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if(tree == NULL){ // Ak sme sa dostali na koniec stromu
        return false; // Vratime false
    }
    if(tree->key == key){ // Ak sme nasli hladany kluc
        *value = tree->value; // Zapamatame si hodnotu
        return true; // Vratime true
    }
    if(tree->key > key){ // Ak je hladany kluc mensi ako aktualny
        return bst_search(tree->left, key, value); // Hladame v lavej casti
    }
    else{ // Ak je hladany kluc vacsi ako aktualny
        return bst_search(tree->right, key, value); // Hladame v pravej casti
    }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if(*tree == NULL){ // Ak sme sa dostali na koniec stromu
        *tree = malloc(sizeof(bst_node_t)); // Alokujeme pamat pre novy uzol
        (*tree)->key = key; // Nastavime kluc
        (*tree)->value = value; // Nastavime hodnotu
        (*tree)->left = NULL; // Nastavime lavy podstrom na NULL
        (*tree)->right = NULL; // Nastavime pravy podstrom na NULL
        return; // Vratime sa
    }
    if((*tree)->key == key){ // Ak sme nasli hladany kluc
        (*tree)->value = value; // Zmenime hodnotu
        return; // Vratime sa
    }
    if((*tree)->key > key){ // Ak je hladany kluc mensi ako aktualny
        bst_insert(&(*tree)->left, key, value); // Vlozime do lavej casti
    }
    else{ // Ak je hladany kluc vacsi ako aktualny
        bst_insert(&(*tree)->right, key, value); // Vlozime do pravej casti
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if((*tree)->right != NULL){ // Ak sa este vieme posunut doprava
        bst_replace_by_rightmost(target, &(*tree)->right); // Posunieme sa doprava
    }
    else{ // Ak sme sa dostali na koniec
        target->key = (*tree)->key; // Nahradime kluc
        target->value = (*tree)->value; // Nahradime hodnotu
        bst_node_t *tmp = *tree; // Ulozime si aktualny uzol
        *tree = (*tree)->left; // Posunieme sa do lavej casti
        free(tmp); // Uvolnime pamat
    }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if(*tree == NULL){ // Ak sme sa dostali na koniec stromu
        return; // Vratime sa
    }
    if((*tree)->key == key){ // Ak sme nasli hladany kluc
        if((*tree)->left == NULL){ // Ak nema lavy podstrom
            bst_node_t *tmp = *tree; // Ulozime si aktualny uzol
            *tree = (*tree)->right; // Posunieme sa do pravej casti
            free(tmp); // Uvolnime pamat
        }
        else if((*tree)->right == NULL){ // Ak nema pravy podstrom
            bst_node_t *tmp = *tree; // Ulozime si aktualny uzol
            *tree = (*tree)->left; // Posunieme sa do lavej casti
            free(tmp); // Uvolnime pamat
        }
        else{ // Ak ma oba podstromy
            bst_replace_by_rightmost(*tree, &(*tree)->left); // Nahradime najpravejsim uzlom z laveho podstromu
        }
    }
    else if((*tree)->key > key){ // Ak je hladany kluc mensi ako aktualny
        bst_delete(&(*tree)->left, key); // Posunieme sa do lavej casti
    }
    else{ // Ak je hladany kluc vacsi ako aktualny
        bst_delete(&(*tree)->right, key); // Posunieme sa do pravej casti
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if(*tree == NULL){ // Ak sme sa dostali na koniec stromu
        return; // Vratime sa
    }
    bst_dispose(&(*tree)->left); // Uvolnime lavy podstrom
    bst_dispose(&(*tree)->right); // Uvolnime pravy podstrom
    free(*tree); // Uvolnime aktualny uzol
    *tree = NULL; // Nastavime aktualny uzol na NULL
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if(tree == NULL){ // Ak sme sa dostali na koniec stromu
        return; // Vratime sa
    }
    bst_add_node_to_items(tree, items); // Spracujeme aktualny uzol
    bst_preorder(tree->left, items); // Spracujeme lavy podstrom
    bst_preorder(tree->right, items); // Spracujeme pravy podstrom
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if(tree == NULL){ // Ak sme sa dostali na koniec stromu
        return; // Vratime sa
    }
    bst_inorder(tree->left, items); // Spracujeme lavy podstrom
    bst_add_node_to_items(tree, items); // Spracujeme aktualny uzol
    bst_inorder(tree->right, items); // Spracujeme pravy podstrom
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if(tree == NULL){ // Ak sme sa dostali na koniec stromu
        return; // Vratime sa
    }
    bst_postorder(tree->left, items); // Spracujeme lavy podstrom
    bst_postorder(tree->right, items); // Spracujeme pravy podstrom
    bst_add_node_to_items(tree, items); // Spracujeme aktualny uzol
}
