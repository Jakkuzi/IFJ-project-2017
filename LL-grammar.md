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
12. __Tělo_fce__			→	id = __Přiřazení2__ eol __Tělo_fce__
13. __Tělo_fce__			→	input id eol __Tělo_fce__
14. __Tělo_fce__			→	print __Print_param__ __Tělo_fce__
15. __Tělo_fce__			→	return __Přiřazení2__ eol __Tělo_fce__
16. __Tělo_fce__			→	end
<!-- Tělo if1 -->
17. __Tělo_if1__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_if1__
18. __Tělo_if1__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_if1__
19. __Tělo_if1__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_if1__
20. __Tělo_if1__			→	id = __Přiřazení2__ eol __Tělo_if1__
21. __Tělo_if1__			→	input id eol __Tělo_if1__
22. __Tělo_if1__			→	print __Print_param__ __Tělo_if1__
23. __Tělo_if1__			→	return __Přiřazení2__ eol __Tělo_if1__
24. __Tělo_if1__			→	else eol __Tělo_if2__
<!-- Tělo if2 -->
25. __Tělo_if2__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_if2__
26. __Tělo_if2__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_if2__
27. __Tělo_if2__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_if2__
28. __Tělo_if2__			→	id = __Přiřazení2__ eol __Tělo_if2__
29. __Tělo_if2__			→	input id eol __Tělo_if2__
30. __Tělo_if2__			→	print __Print_param__ __Tělo_if2__
31. __Tělo_if2__			→	return __Přiřazení2__ eol __Tělo_if2__
32. __Tělo_if2__			→	end
<!-- Tělo while -->
33. __Tělo_while__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_while__
34. __Tělo_while__			→	if __Výraz_if__ then eol __Tělo_if1__ if eol __Tělo_while__
35. __Tělo_while__			→	do while __Výraz__ eol __Tělo_while__ eol __Tělo_while__
36. __Tělo_while__			→	id = __Přiřazení2__ eol __Tělo_while__
37. __Tělo_while__			→	input id eol __Tělo_while__
38. __Tělo_while__			→	print __Print_param__ eol __Tělo_while__
39. __Tělo_while__			→	return __Přiřazení2__ eol __Tělo_while__
40. __Tělo_while__			→	loop
<!-- Tělo scope -->
41. __Tělo_scope__			→	dim id as __Datový_typ__ __Přiřazení__ __Tělo_scope__
42. __Tělo_scope__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Tělo_scope_
43. __Tělo_scope__			→	do while __Výraz__ eol __Scope_while__ eol __Tělo_scope__
44. __Tělo_scope__			→	id = __Přiřazení2__ eol __Tělo_scope__
45. __Tělo_scope__			→	input id eol __Tělo_scope__
46. __Tělo_scope__			→	print __Print_param__ __Tělo_scope__
47. __Tělo_scope__			→	end
<!-- Scope while -->
48. __Scope_while__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_while__
49. __Scope_while__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Scope_while__
50. __Scope_while__			→	do while __Výraz__ eol __Scope_while__ eol __Scope_while__
51. __Scope_while__			→	id = __Přiřazení2__ eol __Scope_while__
52. __Scope_while__			→	input id eol __Scope_while__
53. __Scope_while__			→	print __Print_param__ __Scope_while__
54. __Scope_while__			→	loop
<!-- Scope if1 -->
55. __Scope_if1__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_if1__
56. __Scope_if1__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Scope_if1__
57. __Scope_if1__			→	do while __Výraz__ eol __Scope_while__ eol __Scope_if1__
58. __Scope_if1__			→	id = __Přiřazení2__ eol __Scope_if1__
59. __Scope_if1__			→	input id eol __Scope_if1__
60. __Scope_if1__			→	print __Print_param__ __Scope_if1__
61. __Scope_if1__			→	else eol __Scope_if2__
<!-- Scope if2 -->
62. __Scope_if2__			→	dim id as __Datový_typ__ __Přiřazení__ __Scope_if2__
63. __Scope_if2__			→	if __Výraz_if__ then eol __Scope_if1__ if eol __Scope_if2__
64. __Scope_if2__			→	do while __Výraz__ eol __Scope_while__ eol __Scope_if2__
65. __Scope_if2__			→	id = __Přiřazení2__ eol __Scope_if2__
66. __Scope_if2__			→	input id eol __Scope_if2__
67. __Scope_if2__			→	print __Print_param__ __Scope_if2__
68. __Scope_if2__			→	end
<!-- Zbytek -->
70. __Přiřazení__			→	= __Přiřazení2__ eol
71. __Přiřazení__			→ 	eol
72. __Přiřazení2__			→	id __Id_as__
73. __Přiřazení2__			→	valueOfxxx -- (eol)
74. __Id_as__				→	( __Parametr_fce__ (eol)
75. __Id_as__				→	{operátor} -- výraz (eol)
76. __Parametr_fce__ 		→	id __Parametr_fce_next__
77. __Parametr_fce_next__	→	, __Parametr_funkce__
78. __Parametr_fce_next__	→	)
79. __Výraz__				→	id - EOL, ;
80. __Výraz__				→	valueOfxxx - EOL, ;
81. __Výraz_if__			→	id - (then)
82. __Výraz_if__			→	valueOfxxx - (then)
83. __Datový_typ__			→	integer
84. __Datový_typ__			→	double
85. __Datový_typ__			→	string
86. __Print_param__			→	__Přiřazení2__ ; __Print_param__
87. __Print_param__			→	eol


*	výrazy se pohlcují až po _then_, _eol_ a _;_
*	(eol) - nepushuje se na stack

<!--


201	→	23 30 202 122 201
201	→	30 202 122 205 30 122 201
201	→	38 122 209 38
202	→	11 100 203 21 220 
203	→	11 21 220 204
203	→	101
204	→	105 203
204	→	101
Tělo funkce
205	→	24 11 21 220 213 205
205	→	31 219 41 122 206 31 122 205
205	→	25 42 218 122 208 122 205
205	→	11 86 214 122 205
205	→	32 11 122 205
205	→	36 221 205
205	→	37 214 122 205
205	→	28
Tělo if1
206	→	24 11 21 220 213 206
206	→	31 219 41 122 206 31 122 206
206	→	25 42 218 122 208 122 206
206	→	11 86 214 122 206
206	→	32 11 122 206
206	→	36 221 206
206	→	37 214 122 206
206	→	27 122 206
Tělo if2
207	→	24 11 21 220 213 207
207	→	31 219 41 122 206 31 122 207
207	→	25 42 218 122 208 122 207
207	→	11 86 214 122 207
207	→	32 11 122 207
207	→	36 221 207
207	→	37 214 122 207
207	→	28
Tělo while
208	→	24 11 21 220 213 208
208	→	31 219 41 122 206 31 122 208
208	→	25 42 218 122 208 122 208
208	→	11 86 214 122 208
208	→	32 11 122 208
208	→	36 221 208
208	→	37 214 122 208
208	→	35
Tělo scope
209	→	24 11 21 220 213 209
209	→	31 219 41 122 211 31 122 209
209	→	25 42 218 122 210 122 209
209	→	11 86 214 122 209
209	→	32 11 122 209
209	→	36 221 209
209	→	28
Scope while
210	→	24 11 21 220 213 210
210	→	31 219 41 122 211 31 122 210
210	→	25 42 218 122 210 122 210
210	→	11 86 214 122 210
210	→	32 11 122 210
210	→	36 221 210
210	→	35
Scope if1
211	→	24 11 21 220 213 211
211	→	31 219 41 122 211 31 122 211
211	→	25 42 218 122 210 122 211
211	→	11 86 214 122 211
211	→	32 11 122 211
211	→	36 221 211
211	→	27 122 212
Scope if2
212	→	24 11 21 220 213 212
212	→	31 219 41 122 211 31 122 212
212	→	25 42 218 122 210 122 212
212	→	11 86 214 122 212
212	→	32 11 122 212
212	→	36 221 212
212	→	28
Zbytek
213	→	86 214 122
213	→	122
214	→	11 215
214	→	{12, 13, 14}
215	→	100 216
215	→	{81, 82, 83, 84}
216	→	11 217
217	→	105 216
217	→	101
218	→	{11, 12, 13, 14}
219	→	{11, 12, 13, 14}
220	→	33
220	→	39
220	→	26
221	→	214 73 221
221	→	122




-->