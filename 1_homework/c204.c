/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

    if(!Stack_IsEmpty(stack)){

        char top;
        Stack_Top(stack, &top);

        while (top != '('){

            Stack_Pop(stack);

            postfixExpression[*postfixExpressionLength] = top;
            (*postfixExpressionLength)+=1;

            Stack_Top(stack, &top);
        }

        Stack_Pop(stack);

    }

}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    char top = '\0';

    // If the stack isn't empty, get its top element.
    if (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &top);
    }

    // Process based on the current character.
    switch (c) {
        case '+':
        case '-':
            // For '+' and '-' operators.
            if (top == '+' || top == '-' || top == '*' || top == '/') {
                // If the top of stack has another operator, pop it to the postfix string.
                postfixExpression[*postfixExpressionLength] = top;
                (*postfixExpressionLength)++;

                Stack_Pop(stack);

                // Recursive call to handle potential further operations with same precedence or higher.
                doOperation(stack, c, postfixExpression, postfixExpressionLength);
            } else {
                // If no conflicting operator on top, push the current operator to the stack.
                Stack_Push(stack, c);
            }
            break;

        case '*':
        case '/':
            // For '*' and '/' operators.
            if (top == '+' || top == '-') {
                // If the top of stack has lower precedence operators, push the current operator.
                Stack_Push(stack, c);
            } else if (top == '*' || top == '/') {
                // If the top of stack has another operator of same precedence, pop it to the postfix string.
                postfixExpression[*postfixExpressionLength] = top;
                (*postfixExpressionLength)++;

                Stack_Pop(stack);

                // Recursive call to handle further potential operations of same precedence.
                doOperation(stack, c, postfixExpression, postfixExpressionLength);
            } else {
                // If no conflicting operator on top, push the current operator to the stack.
                Stack_Push(stack, c);
            }
            break;

        case '(':
            // Directly push opening parenthesis to the stack.
            Stack_Push(stack, c);
            break;

        case ')':
            // Pop operators from the stack to the postfix string until encountering an opening parenthesis.
            untilLeftPar(stack, postfixExpression, postfixExpressionLength);
            break;

        case '=':
            // Pop all remaining operators from the stack to the postfix string.
            if (!Stack_IsEmpty(stack)) {
                while (stack->topIndex >= 0) {
                    Stack_Top(stack, &top);
                    postfixExpression[*postfixExpressionLength] = top;
                    (*postfixExpressionLength) += 1;

                    Stack_Pop(stack);
                }
            }

            // Add the '=' operator to the postfix string (assuming this is an end marker for the expression).
            postfixExpression[*postfixExpressionLength] = c;
            (*postfixExpressionLength) += 1;
            break;
    }

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix(const char *infixExpression) {

    // Estimate the maximum length of the postfix expression.
    unsigned postfixExpressionLength = 0;

    // Allocate memory for the postfix expression.
    char* postfixExpression = (char*)malloc(sizeof(char) * MAX_LEN);
    if (!postfixExpression) return NULL;  // Return NULL if memory allocation fails.

    // Allocate memory for the stack and initialize it.
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    Stack_Init(stack);

    // Traverse through the input infix expression.
    for (int i = 0; i < MAX_LEN - 1; i++) {

        char element = infixExpression[i];

        // Check for the end of the input string.
        if (element == '\0') {
            postfixExpression[postfixExpressionLength] = '\0';  // Null-terminate the postfix string.
            break;
        }

        // Check if the current character is NOT a digit or an uppercase/lowercase alphabet character.
        if ((element < 48 || (element > 57 && element < 65) || (element > 90 && element < 97) || element > 122)) {

            // Handle the operation (e.g., push/pop operators to/from stack).
            doOperation(stack, element, postfixExpression, &postfixExpressionLength);

        } else { // The current character is a digit or an alphabetic character.

            // Append the character directly to the postfix expression.
            postfixExpression[postfixExpressionLength++] = element;

        }
    }

    // Cleanup: Dispose the stack and free the allocated memory for it.
    Stack_Dispose(stack);
    free(stack);

    // Return the postfix notation string.
    return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

//    Push int value to stack byte by byte
    for (unsigned long i = 0; i < sizeof(int); i++) {

        Stack_Push(stack, (char)((value >> (i * 8)) & 0xFF));

    }
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

    *value = 0;
//    Pop int value from stack byte by byte
    for (int i = sizeof(int) - 1; i >= 0 && !Stack_IsEmpty(stack); i--) {

        char byte;
        Stack_Top(stack, &byte);
        *value |= (byte & 0xFF) << (i * 8);
        Stack_Pop(stack);

    }

}

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {

    // Allocate memory for the stack and initialize it.
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    Stack_Init(stack);

    // Convert the infix expression to postfix notation.
    char *postfixExp = infix2postfix(infixExpression);

    int operation_result, first_operand, second_operand, i = 0;

    // Initialize the result to -1 by default.
    *value = -1;

    // Traverse through the postfix expression.
    while (postfixExp[i]) {
        char c = postfixExp[i];

        // Check if the current character is a variable (alphabetic character).
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            // Lookup the variable's value.
            for (int j = 0; j < variableValueCount; j++) {
                if (variableValues[j].name == c) {
                    *value = variableValues[j].value;
                    break;
                }
            }

            // Push the variable's value onto the stack.
            expr_value_push(stack, *value);

        } else if (c >= '0' && c <= '9') { // Check if the current character is a digit.

            // Convert the character to its integer value.
            *value = c - '0';

            // Push the digit's value onto the stack.
            expr_value_push(stack, *value);

        } else if (c == '+' || c == '-' || c == '*' || c == '/') { // Check if the current character is an operator.

            // Pop the top two values from the stack as operands.
            expr_value_pop(stack, &second_operand);
            expr_value_pop(stack, &first_operand);

            // Perform the arithmetic operation.
            switch (c) {
                case '*': operation_result = first_operand * second_operand; break;
                case '/': operation_result = first_operand / second_operand; break;
                case '+': operation_result = first_operand + second_operand; break;
                case '-': operation_result = first_operand - second_operand; break;
            }

            // Push the result back onto the stack.
            expr_value_push(stack, operation_result);
        }

        // Move to the next character in the postfix expression.
        i++;
    }

    // If the stack is not empty, pop the result and store it in 'value'.
    if (!Stack_IsEmpty(stack)) {
        expr_value_pop(stack, value);
    } else {
        *value = 0; // Default result value if the stack is empty.
    }

    // Cleanup: Dispose the stack and free the allocated memory.
    Stack_Dispose(stack);
    free(stack);
    free(postfixExp);

    return true;
}


/* Konec c204.c */
