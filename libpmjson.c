#include <stdio.h>
#include <field_annotation.h>
#include <libpmjson.h>
#include <string.h>

static void json_marshal_string(char ** const post_data_temp_ptr,
								const char * const value_string,
								int value_string_len,
								const int bitmap_field_annotation_attribute)
{
	if(!IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_ADD_DOUBLE_QUOTES)) {
		**post_data_temp_ptr = '\"';
		(*post_data_temp_ptr)++;
	}

	if(IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_JSON_ESCAPE)) {
		if(-1 == value_string_len) {
			value_string_len = strlen(value_string);
		}
		memcpy(*post_data_temp_ptr,
			   value_string,
			   value_string_len);
		(*post_data_temp_ptr) += value_string_len;
	}
	else {
		//todo json escape
		value_string_len = strlen(value_string);
		memcpy(*post_data_temp_ptr,
			   value_string,
			   value_string_len);
		(*post_data_temp_ptr) += value_string_len;
	}

	if(!IS_BIT_SET(bitmap_field_annotation_attribute,
				   BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_ADD_DOUBLE_QUOTES)) {
		**post_data_temp_ptr = '\"';
		(*post_data_temp_ptr)++;
	}
}

static void json_marshal_number(char ** const post_data_temp_ptr,
								const char key[],
								const int key_len,
								const void * const value_number,
								void (*number_to_string_function)(char **,
																  const void *,
																  int),
								int * const precede_by_comma,
								const int bitmap_field_annotation_attribute)
{
	if(NULL == value_number &&
	   IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY)) {
		return;
	}
	if(1 == *precede_by_comma) {
		**post_data_temp_ptr = ',';
		(*post_data_temp_ptr)++;
	}

	*precede_by_comma = 1;

	json_marshal_string(post_data_temp_ptr,
						key,
						key_len,
						(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_JSON_ESCAPE));

	**post_data_temp_ptr = ':';
	(*post_data_temp_ptr)++;

	if(NULL == value_number) {
		memcpy(*post_data_temp_ptr,
			   JSON_NULL,
			   (sizeof(JSON_NULL) - 1));
		(*post_data_temp_ptr) += (sizeof(JSON_NULL) - 1);
		return;
	}

	if(IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_ADD_DOUBLE_QUOTES)) {
		**post_data_temp_ptr = '\"';
		(*post_data_temp_ptr)++;
	}

	number_to_string_function(post_data_temp_ptr,
							  value_number,
							  bitmap_field_annotation_attribute);

	if(IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_ADD_DOUBLE_QUOTES)) {
		**post_data_temp_ptr = '\"';
		(*post_data_temp_ptr)++;
	}
}

static void number_to_string_function_long_long(char ** const post_data_temp_ptr,
												const void *value_number,
												const int bitmap_field_annotation_attribute)
{
	const long long * const value = (const long long *) value_number;
	if(IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_BOOLEAN)) {
		if(*value) {
			memcpy(*post_data_temp_ptr,
				   JSON_TRUE,
				   (sizeof(JSON_TRUE) - 1));
			(*post_data_temp_ptr) += (sizeof(JSON_TRUE) - 1);
		}
		else {
			memcpy(*post_data_temp_ptr,
				   JSON_FALSE,
				   (sizeof(JSON_FALSE) - 1));
			(*post_data_temp_ptr) += (sizeof(JSON_FALSE) - 1);
		}
	}
	else {
		if((*value ) < 0) {
			**post_data_temp_ptr = '-';
			(*post_data_temp_ptr)++;
		}

		char *value_start = *post_data_temp_ptr;
		long long value_copy = *value;
		do {
			**post_data_temp_ptr = ('0' + (value_copy % 10));
			(*post_data_temp_ptr)++;
			value_copy /= 10;
		} while(0 != value_copy);
		char *value_end = ((*post_data_temp_ptr) - 1);
		while(value_start < value_end) {
			char tmp = *value_start;
			*value_start = *value_end;
			*value_end = tmp;
			value_start++;
			value_end--;
		}
	}
}

void json_marshal_long_long(char ** const post_data_temp_ptr,
							const char key[],
							const int key_len,
							const long long * const value,
							int * const precede_by_comma,
							const int bitmap_field_annotation_attribute)
{
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						value,
						number_to_string_function_long_long,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}


void json_marshal_long(char ** const post_data_temp_ptr,
					   const char key[],
					   const int key_len,
					   const long * const value,
					   int * const precede_by_comma,
					   const int bitmap_field_annotation_attribute)
{
	const long long value_long_long = *value;
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						&value_long_long,
						number_to_string_function_long_long,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}

void json_marshal_int(char ** const post_data_temp_ptr,
					  const char key[],
					  const int key_len,
					  const int * const value,
					  int * const precede_by_comma,
					  const int bitmap_field_annotation_attribute)
{
	const long long value_long_long = *value;
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						&value_long_long,
						number_to_string_function_long_long,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}

void json_marshal_short(char ** const post_data_temp_ptr,
						const char key[],
						const int key_len,
						const short * const value,
						int * const precede_by_comma,
						const int bitmap_field_annotation_attribute)
{
	const long long value_long_long = *value;
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						&value_long_long,
						number_to_string_function_long_long,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}

void json_marshal_char(char ** const post_data_temp_ptr,
					   const char key[],
					   const int key_len,
					   const char * const value,
					   int * const precede_by_comma,
					   const int bitmap_field_annotation_attribute)
{
	const long long value_long_long = *value;
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						&value_long_long,
						number_to_string_function_long_long,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}

static void number_to_string_function_long_double(char ** const post_data_temp_ptr,
												  const void *value_number,
												  const int bitmap_field_annotation_attribute /* ignored */)
{
	(void) bitmap_field_annotation_attribute;
	const long double * const value = (const long double *) value_number;
	(*post_data_temp_ptr) += sprintf(*post_data_temp_ptr,
									 "%Le",
									 *value);
}

void json_marshal_long_double(char ** const post_data_temp_ptr,
							  const char key[],
							  const int key_len,
							  const long double * const value,
							  int * const precede_by_comma,
							  const int bitmap_field_annotation_attribute)
{
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						value,
						number_to_string_function_long_double,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}


void json_marshal_double(char ** const post_data_temp_ptr,
						 const char key[],
						 const int key_len,
						 const double * const value,
						 int * const precede_by_comma,
						 const int bitmap_field_annotation_attribute)
{
	const long double value_long_double = *value;
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						&value_long_double,
						number_to_string_function_long_double,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}


void json_marshal_float(char ** const post_data_temp_ptr,
						const char key[],
						const int key_len,
						const float * const value,
						int * const precede_by_comma,
						const int bitmap_field_annotation_attribute)
{
	const long double value_long_double = *value;
	json_marshal_number(post_data_temp_ptr,
						key,
						key_len,
						&value_long_double,
						number_to_string_function_long_double,
						precede_by_comma,
						bitmap_field_annotation_attribute);
}
void json_marshal_char_array(char ** const post_data_temp_ptr,
							 const char key[],
							 const int key_len,
							 const char value[], /* string */
							 const int len, /* ignored */
							 const int size, /* ignored */
							 int * const precede_by_comma,
							 int bitmap_field_annotation_attribute)
{
	(void) len;
	(void) size;
	if(IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_ADD_DOUBLE_QUOTES)) {
		//fatal json break fix: e.g assume key=key1 value="\0", in that case there will a json break as follows: "key1":,"key2":"validvalue"
		SET_BIT(bitmap_field_annotation_attribute,
				BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY);
		//todo should throw a compiler time error instead via libclang parsing if this condition is configured for a fielddecl
	}
	
	if((NULL == value ||
		'\0' == *value) &&
	   IS_BIT_SET(bitmap_field_annotation_attribute,
				  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY)) {
		return;
	}

	if(1 == *precede_by_comma) {
		**post_data_temp_ptr = ',';
		(*post_data_temp_ptr)++;
	}
	*precede_by_comma = 1;

	json_marshal_string(post_data_temp_ptr,
						key,
						key_len,
						(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_JSON_ESCAPE));

	**post_data_temp_ptr = ':';
	(*post_data_temp_ptr)++;

	if(NULL == value) {
		memcpy(*post_data_temp_ptr,
			   JSON_NULL,
			   (sizeof(JSON_NULL) - 1));
		(*post_data_temp_ptr) += (sizeof(JSON_NULL) - 1);
		return;
	}

	json_marshal_string(post_data_temp_ptr,
						value,
						-1,
						bitmap_field_annotation_attribute);
}
