struct s0 {
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
};

struct s0_1 {
	unsigned char uchar[10];
	unsigned short ushort[10];
	unsigned int uint[10];
	unsigned long ulong[10];
	unsigned long long ulonglong[10];	
	char _char[10];
	signed char schar[10];
	short _short[10];
	int _int[10];
	long _long[10];
	long long _longlong[10];
	float _float[10];
	double _double[10];
	struct ritesh2_t struct_ritesh[10];
};

struct s0_2 {
	unsigned char uchar[10][20];
	unsigned short ushort[10][20];
	unsigned int uint[10][20];
	unsigned long ulong[10][20];
	unsigned long long ulonglong[10][20];	
	char _char[10][20];
	signed char schar[10][20];
	short _short[10][20];
	int _int[10][20];
	long _long[10][20];
	long long _longlong[10][20];
	float _float[10][20];
	double _double[10][20];
	struct ritesh_t struct_ritesh[10][20];
};

struct s00 {
	unsigned char *uchar;
	unsigned short *ushort;
	unsigned int *uint;
	unsigned long *ulong;
	unsigned long long *ulonglong;	
	char *_char;
	signed char *schar;
	short *_short;
	int *_int;
	long *_long;
	long long *_longlong;
	float *_float;
	double *_double;
	struct ritesh_t *struct_ritesh;
};

struct s000 {
	unsigned char **uchar;
	unsigned short **ushort;
	unsigned int **uint;
	unsigned long **ulong;
	unsigned long long **ulonglong;	
	char **_char;
	signed char **schar;
	short **_short;
	int **_int;
	long **_long;
	long long **_longlong;
	float **_float;
	double **_double;
	struct ritesh_t **struct_ritesh;
};

struct s007 {
	unsigned char *uchar[10];
	unsigned short *ushort[10];
	unsigned int *uint[10];
	unsigned long *ulong[10];
	unsigned long long *ulonglong[10];	
	char *_char[10];
	signed char *schar[10];
	short *_short[10];
	int *_int[10];
	long *_long[10];
	long long *_longlong[10];
	float *_float[10];
	double *_double[10];
	struct ritesh_t *struct_ritesh[10];
};

struct s1 {
	char c1;
	char *c2_1;
	char **c2_2;
	char ***c2_3;
	char c3[10];
	char c4[10][10];
	char c5[10][10][10];
	char *c6[10];
};

struct s2 {
	int c1;
	int *c2_1;
	int **c2_2;
	int ***c2_3;
	int c3[10];
	int c4[10][10];
	int c5[10][10][10];
	int *c6[10];
};
