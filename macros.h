#ifndef _MACROS_H_
#define _MACROS_H_
#define ASSERT_TRUE(mp_expression, mp_label) if(!(mp_expression)) goto mp_label
#define ASSERT_FALSE(mp_expression, mp_label) if((mp_expression)) goto mp_label
#define ASSERT_POS(mp_expression, mp_label) if((mp_expression) < 0) goto mp_label
#define ASSERT_EQU(mp_expression, mp_value, mp_label) if((mp_expression) != mp_value) goto mp_label
#endif //_MACROS_H_