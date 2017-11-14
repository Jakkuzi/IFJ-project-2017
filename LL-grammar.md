# LL grammar

* __Start__              →   __Deklarace__
* __Start__              →   __Definice__
* __Start__              →   __Scoope__
* __Deklarace__          →   declare __Hlavička__
* __Definice__           →   __Hlavička__ eol __Tělo__ end_function
* __Scope__              →   scope __Tělo__ end_scope
* __Hlavička__           →   function id __Parametry__ as __Datový_typ__
* __Tělo__               →   __Inicializace__ eol __Tělo__
* __Tělo__               →   __Podmínka__ eol __Tělo__
* __Tělo__               →   __Cyklus__ eol __Tělo__
* __Tělo__               →   __Výraz__ eol __Tělo__
* __Tělo__               →   __Input__ eol __Tělo__
* __Tělo__               →   print __Print_param__ eol __Tělo__
* __Tělo__               →   __Přiřazení__ eol __Tělo__
* __Tělo__               →   ε
* __Input__              →   __Výraz__
* __Přiřazení__          →   id = id_funkce __Parametry__
* __Print_param__        →   __Param_p__ __Param_next__
* __Param_p__            →   __Výraz__
* __Param_next__         →   , __Param_P__
* __Parametry__          →   ( __Param__ __Next_param__ )
* __Param__              →   id as __Datový_typ__
* __Next_param__         →   , __Param__
* __Datový_typ__         →   integer
* __Datový_typ__         →   double
* __Datový_typ__         →   string
* __Inicializace__       →   dim id as __Datový_typ__
* __Podmínka__           →   if __Výraz__ then __Tělo__ end_if
* __Cyklus__             →   do while __Výraz__ eol __Tělo__ loop
* __Výraz__              →   seskupit tokeny výrazu a poslat precedenční analýze