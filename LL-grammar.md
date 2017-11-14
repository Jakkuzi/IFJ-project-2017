# LL grammar

1. __Start__              →   __Deklarace__
2. __Start__              →   __Definice__
3. __Start__              →   __Scoope__
4. __Deklarace__          →   declare __Hlavička__
5. __Definice__           →   __Hlavička__ eol __Tělo__ end End_function
6. __Scope__              →   scope __Tělo__ end_scope
7. __Hlavička__           →   function id __Parametry__ as __Datový_typ__
8. __Tělo__               →   __Inicializace__ eol __Tělo__
9. __Tělo__               →   __Podmínka__ eol __Tělo__
10. __Tělo__               →   __Cyklus__ eol __Tělo__
11. __Tělo__               →   __Výraz__ eol __Tělo__
12. __Tělo__               →   __Input__ eol __Tělo__
13. __Tělo__               →   print __Print_param__ eol __Tělo__
14. __Tělo__               →   __Přiřazení__ eol __Tělo__
15. __Tělo__               →   ε
16. __Input__              →   __Výraz__
17. __Přiřazení__          →   id = id_funkce __Parametry__
18. __Print_param__        →   __Param_p__ __Param_next__
19. __Param_p__            →   __Výraz__
20. __Param_next__         →   , __Param_P__
21. __Parametry__          →   ( __Param__ __Next_param__ )
22. __Param__              →   id as __Datový_typ__
23. __Next_param__         →   , __Param__
24. __Datový_typ__         →   integer
25. __Datový_typ__         →   double
26. __Datový_typ__         →   string
27. __Inicializace__       →   dim id as __Datový_typ__
28. __Podmínka__           →   if __Výraz__ then __Tělo__ end_if
29. __Cyklus__             →   do while __Výraz__ eol __Tělo__ loop
30. __Výraz__              →   seskupit tokeny výrazu a poslat precedenční analýze

|              | declare | eol | scope | function | dim | if | then | do | while | loop | id | as | print | integer | double | string | ε | + | - | * | / | = | ( | ) | , |
|--------------|---------|-----|-------|----------|-----|----|------|----|-------|------|----|----|-------|---------|--------|--------|---|---|---|---|---|---|---|---|---|
| Start        |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Deklarace    |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Definice     |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Scope        |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Hlavička     |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Tělo         |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Input        |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Přiřazení    |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Print_param  |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Param_p      |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Param_next   |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Parametry    |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Param        |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Next_param   |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Datový_typ   |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Inicializace |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Podmínka     |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |
| Cyklus       |         |     |       |          |     |    |      |    |       |      |    |    |       |         |        |        |   |   |   |   |   |   |   |   |   |