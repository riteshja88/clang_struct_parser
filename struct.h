struct ritesh_t {
	int int001;
	int *int002 __attribute__((annotate("omit-empty")));
	int int003[11] __attribute__((annotate("omit-empty")));
	int int003_count __attribute__((annotate("no-marshal")));
	int *int004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	int int004_count __attribute__((annotate("no-marshal")));

	long long001;
	long *long002 __attribute__((annotate("omit-empty")));
	long long003[11] __attribute__((annotate("omit-empty")));
	long long003_count __attribute__((annotate("no-marshal")));
	long *long004 __attribute__((annotate("pointer-to-array"))) __attribute__((annotate("omit-empty")));
	long long004_count __attribute__((annotate("no-marshal")));
};
typedef struct ritesh_t ritesh3_t;
struct s0_t {
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
	struct ritesh_t struct_ritesh;
	ritesh3_t struct_ritesh3;

	double __double[10];
	struct ritesh_t _struct_ritesh[10];
	ritesh3_t _struct_ritesh3[10];

	double *___double;
	struct ritesh_t *__struct_ritesh;
	ritesh3_t *__struct_ritesh3;
};
