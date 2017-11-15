 1. __S__ 					→	declare  function __Hlavička__ eol __S__
 2. __S__					→	function __Hlavička__ eol __Tělo_fce__ function eol __S__
 3. __S__					→	scope __Tělo_scope__ scope
 4. __Hlavička__			→	id ( __Parametry__ as __Datový_typ__
 5. __Parametry__			→	id as __Datový_typ__ __Parametr_next__
 6. __Parametr_next__		→	, __Parametry__
 7. __Parametr_next__		→	)
 8. __Tělo_fce__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_fce__
 9. __Tělo_fce__			→	if __Výraz__ then eol __Tělo_if1__ if eol __Tělo_fce__	
10. __Tělo_fce__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_fce__
11. __Tělo_fce__			→	id = __Přiřazení__ eol __Tělo_fce__
12. __Tělo_fce__			→	input id eol __Tělo_fce__
13. __Tělo_fce__			→	print __Print_param__ eol __Tělo_fce__
14. __Tělo_fce__			→	return __Výraz__ eol __Tělo_fce__
15. __Tělo_fce__			→	return __Přiřazení__ eol __Tělo_fce__
16. __Tělo_fce__			→	end
17. __Tělo_if1__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_if1__
18. __Tělo_if1__			→	if __Výraz__ then eol __Tělo_if1__ if eol __Tělo_if1__
19. __Tělo_if1__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_if1__
20. __Tělo_if1__			→	id = __Přiřazení__ eol __Tělo_if1__
21. __Tělo_if1__			→	input id eol __Tělo_if1__
22. __Tělo_if1__			→	print __Print_param__ eol __Tělo_if1__
23. __Tělo_if1__			→	return __Výraz__ eol __Tělo_if1__
24. __Tělo_if1__			→	return __Přiřazení__ eol __Tělo_if1__
25. __Tělo_if1__			→	else eol __Tělo_if2__
26. __Tělo_if2__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_if2__
27. __Tělo_if2__			→	if __Výraz__ then eol __Tělo_if1__ if eol __Tělo_if2__
28. __Tělo_if2__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_if2__
29. __Tělo_if2__			→	id = __Přiřazení__ eol __Tělo_if2__
30. __Tělo_if2__			→	input id eol __Tělo_if2__
31. __Tělo_if2__			→	print __Print_param__ eol __Tělo_if2__
32. __Tělo_if2__			→	return __Výraz__ eol __Tělo_if2__
33. __Tělo_if2__			→	return __Přiřazení__ eol __Tělo_if2__
34. __Tělo_if2__			→	end
35. __Tělo_while__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_while__
36. __Tělo_while__			→	if __Výraz__ then eol __Tělo_if1__ if eol __Tělo_while__
37. __Tělo_while__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_while__
38. __Tělo_while__			→	id = __Přiřazení__ eol __Tělo_while__
39. __Tělo_while__			→	input id eol __Tělo_while__
40. __Tělo_while__			→	print __Print_param__ eol __Tělo_while__
41. __Tělo_while__			→	return __Výraz__ eol __Tělo_while__
42. __Tělo_while__			→	return __Přiřazení__ eol __Tělo_while__
43. __Tělo_while__			→	loop
44. __Tělo_scope__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_scope__
45. __Tělo_scope__			→	if __Výraz__ then eol __Scope_if__ if eol __Tělo_scope_	
46. __Tělo_scope__			→	do while __Výraz__ eol __Scope_while__ eol __Tělo_scope__
47. __Tělo_scope__			→	id = __Přiřazení__ eol __Tělo_scope__
48. __Tělo_scope__			→	input id eol __Tělo_scope__
49. __Tělo_scope__			→	print __Print_param__ eol __Tělo_scope__
50. __Tělo_scope__			→	end
51. __Scope_while__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_while__
52. __Scope_while__			→	if __Výraz__ then eol __Tělo_if1__ if eol __Tělo_while__
53. __Scope_while__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_while__
54. __Scope_while__			→	id = __Přiřazení__ eol __Tělo_while__
55. __Scope_while__			→	input id eol __Tělo_while__
56. __Scope_while__			→	print __Print_param__ eol __Tělo_while__
57. __Scope_while__			→	loop
58. __Scope_if1__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_if1__
59. __Scope_if1__			→	if __Výraz__ then eol __Scope_if1__ if eol __Scope_if1__
60. __Scope_if1__			→	do while __Výraz__ eol __Tělo_while__ eol __Scope_if1__
61. __Scope_if1__			→	id = __Přiřazení__ eol __Scope_if1__
62. __Scope_if1__			→	input id eol __Scope_if1__
63. __Scope_if1__			→	print __Print_param__ eol __Scope_if1__
64. __Scope_if1__			→	else eol __Scope_if2__
65. __Scope_if2__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_if2__
66. __Scope_if2__			→	if __Výraz__ then eol __Scope_if1__ if eol __Scope_if2__
67. __Scope_if2__			→	do while __Výraz__ eol __Tělo_while__ eol __Scope_if2__
68. __Scope_if2__			→	id = __Přiřazení__ eol __Scope_if2__
69. __Scope_if2__			→	input id eol __Scope_if2__
70. __Scope_if2__			→	print __Print_param__ eol __Scope_if2__
71. __Scope_if2__			→	end
72. __Přiřazení__			→	valueOfxxx - EOL, then, ;
73. __Přiřazení__			→	id __Id_as__
74. __Přiřazení__			→ 	eol
75. __Id_as__				→	( __Parametr_fce__ eol
76. __Id_as__				→	{operátor} -- výraz
77. __Parametr_fce__ 		→	id __Parametr_fce_next__
78. __Parametr_fce_next__	→	, __Parametr_funkce__
79. __Parametr_fce_next__	→	)
80. __Výraz__				→	id - EOL, then, ;
81. __Výraz__				→	valueOfInteger - EOL, then, ;
82. __Výraz__				→	valueOfDouble - EOL, then, ;
83. __Výraz__				→	valueOfDoubleWithExp - EOL, then, ;
84. __Datový_typ__			→	integer
85. __Datový_typ__			→	double
86. __Datový_typ__			→	string
87. __Print_param__			→	__Výraz__ ; __Print_param__
88. __Print_param__			→	eol