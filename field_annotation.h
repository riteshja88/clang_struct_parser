enum {
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_NO_MARSHAL = 0, /* used-by-parser */
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY = 1, /* int float */ //string
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_POINTER_TO_ARRAY = 2, /* used-by-parser */
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_ADD_DOUBLE_QUOTES = 3, /* int float */
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_BOOLEAN = 4, /* int !float */
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_ADD_DOUBLE_QUOTES = 5, /* string */
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_JSON_ESCAPE = 6, /* string */
};

#define IS_BIT_SET(flag, bit) ((flag & (1u << bit)) ? 1 : 0)
#define SET_BIT(flag, bit) (flag |= (1u << bit))

#define JSON_NULL "null"
#define JSON_TRUE "true"
#define JSON_FALSE "false"
