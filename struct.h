struct struct1_t {
	int x;
};
typedef struct struct1_t struct1_t;
struct ritesh_t {
	char char001 __attribute__((annotate("dont-add-double-quotes"))) __attribute__((annotate("dont-json-escape")));
	char *char002 __attribute__((annotate("omit-empty")));
	char char003[12] __attribute__((annotate("omit-empty")));
	char char003_count __attribute__((annotate("no-marshal")));
	char *char004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	char char004_count __attribute__((annotate("no-marshal")));
	char char005[9][13] __attribute__((annotate("omit-empty")));
	char char005_count __attribute__((annotate("no-marshal")));
	char (*char006)[19] __attribute__((annotate("omit-empty")));
	char char006_count __attribute__((annotate("no-marshal")));

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

	short short001 __attribute__((annotate("add-double-quotes")));
	short *short002 __attribute__((annotate("omit-empty")));
	short short003[12] __attribute__((annotate("omit-empty")));
	short short003_count __attribute__((annotate("no-marshal")));
	short *short004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	short short004_count __attribute__((annotate("no-marshal")));

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
