 1. __S__ 					→	declare  function __Hlavička__ eol __S__
 2. __S__					→	function __Hlavička__ eol __Tělo_fce__ function eol __S__
 3. __S__					→	scope eol __Tělo_scope__ scope

 4. __Hlavička__			→	id ( __Parametry__ as __Datový_typ__

 5. __Parametry__			→	id as __Datový_typ__ __Parametr_next__
 6. __Parametry__			→	)

 7. __Parametr_next__		→	, __Parametry__
 8. __Parametr_next__		→	)
<!-- Tělo funkce -->
 9. __Tělo_fce__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_fce__
10. __Tělo_fce__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_fce__	
11. __Tělo_fce__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_fce__
12. __Tělo_fce__			→	id = __Přiřazení2__ __Tělo_fce__
13. __Tělo_fce__			→	input id eol __Tělo_fce__
14. __Tělo_fce__			→	print __Print_param__ __Tělo_fce__
15. __Tělo_fce__			→	return __Přiřazení2__ __Tělo_fce__
16. __Tělo_fce__			→	end
<!-- Tělo if1 -->
17. __Tělo_if1__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_if1__
18. __Tělo_if1__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_if1__
19. __Tělo_if1__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_if1__
20. __Tělo_if1__			→	id = __Přiřazení2__ __Tělo_if1__
21. __Tělo_if1__			→	input id eol __Tělo_if1__
22. __Tělo_if1__			→	print __Print_param__ __Tělo_if1__
23. __Tělo_if1__			→	return __Přiřazení2__ __Tělo_if1__
24. __Tělo_if1__			→	else eol __Tělo_if2__
<!-- Tělo if2 -->
25. __Tělo_if2__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_if2__
26. __Tělo_if2__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_if2__
27. __Tělo_if2__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_if2__
28. __Tělo_if2__			→	id = __Přiřazení2__ __Tělo_if2__
29. __Tělo_if2__			→	input id eol __Tělo_if2__
30. __Tělo_if2__			→	print __Print_param__ __Tělo_if2__
31. __Tělo_if2__			→	return __Přiřazení2__ __Tělo_if2__
32. __Tělo_if2__			→	end
<!-- Tělo while -->
33. __Tělo_while__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_while__
34. __Tělo_while__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_while__
35. __Tělo_while__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_while__
36. __Tělo_while__			→	id = __Přiřazení2__ __Tělo_while__
37. __Tělo_while__			→	input id eol __Tělo_while__
38. __Tělo_while__			→	print __Print_param__ __Tělo_while__
39. __Tělo_while__			→	return __Přiřazení2__ __Tělo_while__
40. __Tělo_while__			→	loop
<!-- Tělo scope -->
41. __Tělo_scope__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_scope__
42. __Tělo_scope__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Tělo_scope_
43. __Tělo_scope__			→	do while __Výraz__ eol __Scope_while__ eol __Tělo_scope__
44. __Tělo_scope__			→	id = __Přiřazení2__ __Tělo_scope__
45. __Tělo_scope__			→	input id eol __Tělo_scope__
46. __Tělo_scope__			→	print __Print_param__ __Tělo_scope__
47. __Tělo_scope__			→	end
<!-- Scope while -->
48. __Scope_while__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_while__
49. __Scope_while__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Scope_while__
50. __Scope_while__			→	do while __Výraz__ eol __Scope_while__ eol __Scope_while__
51. __Scope_while__			→	id = __Přiřazení2__ __Scope_while__
52. __Scope_while__			→	input id eol __Scope_while__
53. __Scope_while__			→	print __Print_param__ __Scope_while__
54. __Scope_while__			→	loop
<!-- Scope if1 -->
55. __Scope_if1__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_if1__
56. __Scope_if1__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Scope_if1__
57. __Scope_if1__			→	do while __Výraz__ eol __Scope_while__ eol __Scope_if1__
58. __Scope_if1__			→	id = __Přiřazení2__ __Scope_if1__
59. __Scope_if1__			→	input id eol __Scope_if1__
60. __Scope_if1__			→	print __Print_param__ __Scope_if1__
61. __Scope_if1__			→	else eol __Scope_if2__
<!-- Scope if2 -->
62. __Scope_if2__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_if2__
63. __Scope_if2__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Scope_if2__
64. __Scope_if2__			→	do while __Výraz__ eol __Scope_while__ eol __Scope_if2__
65. __Scope_if2__			→	id = __Přiřazení2__ __Scope_if2__
66. __Scope_if2__			→	input id eol __Scope_if2__
67. __Scope_if2__			→	print __Print_param__ __Scope_if2__
68. __Scope_if2__			→	end
<!-- Zbytek -->
69. __Přiřazení__			→	= __Přiřazení2__ 
70. __Přiřazení__			→ 	eol

71. __Přiřazení2__			→	id __Id_as__
72. __Přiřazení2__			→	valueOfX __Id_as__
73. __Přiřazení2__			→	299

74. __Id_as__				→	( __Parametr_fce__
75. __Id_as__               →   eol
76. __Id_as__				→	298

77. __Parametr_fce__ 		→	id __Parametr_fce_next__
78. __Parametr_fce__ 		→	valueOfX __Parametr_fce_next__
79. __Parametr_fce__ 		→	) eol

80. __Parametr_fce_next__	→	, __Parametr_funkce__
81. __Parametr_fce_next__	→	) eol

82. __Výraz__				→	id __Id_as__
83. __Výraz__				→	298

84. __Výraz_if__			→	id __Id_as__
85. __Výraz_if__			→	296

86. __Datový_typ__			→	integer
87. __Datový_typ__			→	double
88. __Datový_typ__			→	string

89. __Print_param__			→	eol
90. __Print_param__			→	297 ; __Print_param__

91. __Speciální funkce__
*	297, 296, 298, 299
	*   298		→	id je ve struktuře kódu
*	výrazy se pohlcují až po oddělovače _then_, _eol_ a _;_
*   Celý výraz se sjednotí, upraví do speciální formy (i+i...) a pošle se precedenční analýze

