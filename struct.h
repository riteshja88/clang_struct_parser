struct struct1_t {
	int x;
};
typedef struct struct1_t struct1_t;
struct ritesh_t {
	struct1_t struct001 __attribute__((annotate("add-double-quotes")));
	struct1_t *struct002 __attribute__((annotate("omit-empty")));
	struct1_t struct003[12] __attribute__((annotate("omit-empty")));
	struct1_t struct003_count __attribute__((annotate("no-marshal")));
	struct1_t *struct004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	struct1_t struct004_count __attribute__((annotate("no-marshal")));

	int int001 __attribute__((annotate("add-double-quotes"))) __attribute__((annotate("json_field_alias:int001_alias")));
	int *int002 __attribute__((annotate("omit-empty"))) __attribute__((annotate("json_field_alias:int002_alias")));;
	int int003[11] __attribute__((annotate("omit-empty"))) __attribute__((annotate("json_field_alias:int003_alias")));;
	int int003_count __attribute__((annotate("no-marshal")));
	int *int004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty"))) __attribute__((annotate("json_field_alias:int004_alias")));;
	int int004_count __attribute__((annotate("no-marshal")));

	long long001 __attribute__((annotate("add-double-quotes")));
	long *long002 __attribute__((annotate("omit-empty")));
	long long003[12] __attribute__((annotate("omit-empty")));
	long long003_count __attribute__((annotate("no-marshal")));
	long *long004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	long long004_count __attribute__((annotate("no-marshal")));

	long long longlong001 __attribute__((annotate("add-double-quotes")));
	long long *longlong002 __attribute__((annotate("omit-empty")));
	long long longlong003[12] __attribute__((annotate("omit-empty")));
	long long longlong003_count __attribute__((annotate("no-marshal")));
	long long *longlong004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	long long longlong004_count __attribute__((annotate("no-marshal")));

	float float001 __attribute__((annotate("add-double-quotes")));
	float *float002 __attribute__((annotate("omit-empty")));
	float float003[11] __attribute__((annotate("omit-empty")));
	float float003_count __attribute__((annotate("no-marshal")));
	float *float004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	float float004_count __attribute__((annotate("no-marshal")));

	double double001 __attribute__((annotate("add-double-quotes")));
	double *double002 __attribute__((annotate("omit-empty")));
	double double003[11] __attribute__((annotate("omit-empty")));
	double double003_count __attribute__((annotate("no-marshal")));
	double *double004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	double double004_count __attribute__((annotate("no-marshal")));

	long double longdouble001 __attribute__((annotate("add-double-quotes")));
	long double *longdouble002 __attribute__((annotate("omit-empty")));
	long double longdouble003[11] __attribute__((annotate("omit-empty")));
	long double longdouble003_count __attribute__((annotate("no-marshal")));
	long double *longdouble004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	long double longdouble004_count __attribute__((annotate("no-marshal")));

	int bool001 __attribute__((annotate("boolean"))) __attribute__((annotate("add-double-quotes")));
	int *bool002 __attribute__((annotate("boolean"))) __attribute__((annotate("omit-empty")));
	int bool003[11] __attribute__((annotate("boolean"))) __attribute__((annotate("omit-empty")));
	int bool003_count __attribute__((annotate("boolean"))) __attribute__((annotate("no-marshal")));
	int *bool004 __attribute__((annotate("boolean"))) __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	int bool004_count __attribute__((annotate("boolean"))) __attribute__((annotate("no-marshal")));
};
typedef struct ritesh_t ritesh3_t;
struct s0_t {
	/*
	unsigned char uchar;
	unsigned short ushort;
	unsigned int uint;
	unsigned long ulong;
	unsigned long long ulonglong;	
	char _char;
	signed char schar;
	short _short;
	int _int;
	long _long;
	long long _longlong;
	float _float;
	double _double;
	*/
	struct ritesh_t struct_ritesh;
	ritesh3_t struct_ritesh3;

	double __double[10];
	struct ritesh_t _struct_ritesh[10];
	ritesh3_t _struct_ritesh3[10];

	double *___double;
	struct ritesh_t *__struct_ritesh;
	ritesh3_t *__struct_ritesh3;
};
