 1. __S__ 				→	declare  function __Hlavička__ eol __S__
 2. __S__				→	function __Hlavička__ eol __Tělo__ end function eol __S__
 3. __S__				→	scope __Tělo__ end scope eol __S__
 4. __S__				→ 	ε
 5. __Hlavička__		→	id ( __Parametry__ as __Datový_typ__
 6. __Parametry__		→	id as __Datový_typ__ __Parametr_next__
 7. __Parametr_next__	→	, __Parametry__
 8. __Parametr_next__	→	)
 9. __Tělo_fce__		→	dim id as __Datový_typ__ __Přiřazení__ __Tělo__
10. __Tělo_fce__		→	if __Výraz__ then __Tělo__ end if eol __Tělo__
11. __Tělo_fce__		→	do while __Výraz__ eol __Tělo__ loop eol __Tělo__
12. __Tělo_fce__		→	id = __Výraz__ eol __Tělo__
13. __Tělo_fce__		→	input id eol __Tělo__
14. __Tělo_fce__		→	print __Print_param__ __Tělo__
15. __Tělo_fce__		→	return __Výraz__ 
16. __Tělo_fce__		→	ε
17. __Tělo_scope__		→	dim id as __Datový_typ__ __Přiřazení__ __Tělo__
18. __Tělo_scope__		→	if __Výraz__ then __Tělo__ end if eol __Tělo__
19. __Tělo_scope__		→	do while __Výraz__ eol __Tělo__ loop eol __Tělo__
20. __Tělo_scope__		→	id = __Výraz__ eol __Tělo__
21. __Tělo_scope__		→	input id eol __Tělo__
22. __Tělo_scope__		→	print __Print_param__ __Tělo__
23. __Tělo_scope__		→	ε
24. __Print_param__		→	__Výraz__ ; __Print_param__
25. __Print_param__		→	eol
26. __Přiřazení__		→	= __Výraz__
27. __Přiřazení__		→	eol
28. __Datový_typ__		→	integer
29. __Datový_typ__		→	double
30. __Datový_typ__		→	string
31. __Výraz__			→	seskupit tokeny výrazu a poslat precedenční analýze,
							cokoliv co začíná _id_ a končí _eol_