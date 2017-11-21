# Syntaktická analýza

## Vstup
*	Inicializovaná struktura pro uložení kódu

## Výstup
*	0	-	Vše OK
*	2	-	Chybná syntax
*	99	-	Chyba při alokaci

## Funkčnost
*	Kontrola správnosti výrazu
	*	Posloupnost tokenů (i = 56 + 40 * 30)
	*	Nekontroluje typy (i = 56 + !"ahoj")
		*	Datové tokeny (valueOfX) jsou přejmenovány na '_i_' a předány přecedenční analýze
		*	Všechny ostatní tokeny jsou předány normálně 
*	Kontrola posloupnosti příkazů
	*	např. Do While _výraz_ EOL _sekvence příkazů_ Loop
*	Kontrola posloupnosti v kódu
	*	Po _Scope_ již nemůže být deklarována nebo definována žádná funkce
