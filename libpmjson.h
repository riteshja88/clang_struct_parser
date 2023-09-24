void json_marshal_long_long(char **post_data_temp_ptr,
							const char key[],
							int key_len,
							const long long *value,
							int *precede_by_comma,
							int bitmap_field_annotation_attribute);

void json_marshal_long(char **post_data_temp_ptr,
					   const char key[],
					   int key_len,
					   const long *value,
					   int *precede_by_comma,
					   int bitmap_field_annotation_attribute);

void json_marshal_int(char **post_data_temp_ptr,
					  const char key[],
					  int key_len,
					  const int *value,
					  int *precede_by_comma,
					  int bitmap_field_annotation_attribute);

void json_marshal_short(char **post_data_temp_ptr,
						const char key[],
						int key_len,
						const short *value,
						int *precede_by_comma,
						int bitmap_field_annotation_attribute);

void json_marshal_long_double(char **post_data_temp_ptr,
							  const char key[],
							  int key_len,
							  const long double *value,
							  int *precede_by_comma,
							  int bitmap_field_annotation_attribute);

void json_marshal_double(char **post_data_temp_ptr,
						 const char key[],
						 int key_len,
						 const double *value,
						 int *precede_by_comma,
						 int bitmap_field_annotation_attribute);

void json_marshal_float(char **post_data_temp_ptr,
						const char key[],
						int key_len,
						const float *value,
						int *precede_by_comma,
						int bitmap_field_annotation_attribute);

void json_marshal_char_array(char **post_data_temp_ptr,
							 const char key[],
							 int key_len,
							 const char value[], /* string */
							 int len, /* ignored */
							 int size, /* ignored */
							 int *precede_by_comma,
							 int bitmap_field_annotation_attribute);
