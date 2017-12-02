#include "test_prec.h"



int count_ok=0;
int count_errors=0;
char input1[10] = "i+i";
char input2[10] = "ii+";
char input3[10] = "i=i+i";
char input4[10] = "i+=i";
char input5[10] = "i=i+i>i";
char input6[15] = "i>=i<=i<>i";
char input7[10] = "(i)";
char input8[10] = "i<=>=i";
char input9[10] = "i/(i+i)";
char input10[10] = "i(i+i)";
char input11[40] = "i=i+i*(i-(i+i/i)*((i\\i-i>i)/i))=i";
char input12[5] = "a";

int main() {

	/*char cmp_string(char* string)*/
	TEST_EQUAL("test_cmp_string1_E", 'E', cmp_string("[i]"));
	TEST_EQUAL("test_cmp_string2_E", 'E', cmp_string("[E+E]"));
	TEST_EQUAL("test_cmp_string3_E", 'E', cmp_string("[E-E]"));
	TEST_EQUAL("test_cmp_string4_E", 'E', cmp_string("[E*E]"));
	TEST_EQUAL("test_cmp_string5_E", 'E', cmp_string("[E/E]"));
	TEST_EQUAL("test_cmp_string6_E", 'E', cmp_string("[E\\E]"));
	TEST_EQUAL("test_cmp_string7_E", 'E', cmp_string("[E<E]"));
	TEST_EQUAL("test_cmp_string8_E", 'E', cmp_string("[E>E]"));
	TEST_EQUAL("test_cmp_string9_E", 'E', cmp_string("[ElE]"));
	TEST_EQUAL("test_cmp_string10_E", 'E', cmp_string("[EmE]"));
	TEST_EQUAL("test_cmp_string11_E", 'E', cmp_string("[E!E]"));
	TEST_EQUAL("test_cmp_string12_E", 'E', cmp_string("[E=E]"));
	TEST_EQUAL("test_cmp_string13_E", 'E', cmp_string("[(E)]"));
	TEST_EQUAL("test_cmp_string14_X", 'X', cmp_string("sjxb"));
	TEST_EQUAL("test_cmp_string15_X", 'X', cmp_string("E"));

	/*int precedencni(char* input)*/
//	TEST_EQUAL("test_precedencni1_0", 0, precedencni("i+i"));
//	TEST_EQUAL("test_precedencni2_2", 2, precedencni("ii+"));
//	TEST_EQUAL("test_precedencni3_0", 0, precedencni("i=i+i"));
//	TEST_EQUAL("test_precedencni4_2", 2, precedencni("i+=i"));
//	TEST_EQUAL("test_precedencni5_0", 0, precedencni("i=i+i>i"));
//	TEST_EQUAL("test_precedencni6_0", 0, precedencni("i>=i<=i<>i"));
//	TEST_EQUAL("test_precedencni7_0", 0, precedencni("(i)"));
//	TEST_EQUAL("test_precedencni8_2", 2, precedencni("i<=>=i"));
//	TEST_EQUAL("test_precedencni9_0", 0, precedencni("i/(i+i)"));
//	TEST_EQUAL("test_precedencni10_2", 2, precedencni("i(i+i)"));
//	TEST_EQUAL("test_precedencni11_0", 0, precedencni("i=i+i*(i-(i+i/i)*((i\\i-i>i)/i))=i"));


	TEST_EQUAL("test_precedencni1_0", 0, precedencni(input1));
	TEST_EQUAL("test_precedencni2_2", 2, precedencni(input2));
	TEST_EQUAL("test_precedencni3_0", 0, precedencni(input3));
	TEST_EQUAL("test_precedencni4_2", 2, precedencni(input4));
	TEST_EQUAL("test_precedencni5_0", 0, precedencni(input5));
	TEST_EQUAL("test_precedencni6_0", 0, precedencni(input6));
	TEST_EQUAL("test_precedencni7_0", 0, precedencni(input7));
	TEST_EQUAL("test_precedencni8_2", 2, precedencni(input8));
	TEST_EQUAL("test_precedencni9_0", 0, precedencni(input9));	
	TEST_EQUAL("test_precedencni10_2", 2, precedencni(input10));
	TEST_EQUAL("test_precedencni11_0", 0, precedencni(input11));
	TEST_EQUAL("test_precedencni12_2", 2, precedencni(input12));


	printf("_________________________________\n");

	printf("%d tests passed, %d tests failed.\n", count_ok, count_errors);

	printf("_________________________________\n");
	return 0;
}
