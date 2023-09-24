#include <iostream>
#include <string.h>
#include <clang-c/Index.h>
#include <field_annotation.h>

static const char *g_CXType_to_FunctionCallMapping[500]; // one big value chosen without digging in much deep

static void init_g_CXType_to_FunctionCallMapping() {
	for(int i=0;i<sizeof(g_CXType_to_FunctionCallMapping)/sizeof(g_CXType_to_FunctionCallMapping[0]);i++) {
		g_CXType_to_FunctionCallMapping[i] = nullptr;
	}
	g_CXType_to_FunctionCallMapping[CXType_Int] = "json_marshal_int";
	g_CXType_to_FunctionCallMapping[CXType_Short] = "json_marshal_short";
	g_CXType_to_FunctionCallMapping[CXType_Long] = "json_marshal_long";
	g_CXType_to_FunctionCallMapping[CXType_LongLong] = "json_marshal_long_long";
	g_CXType_to_FunctionCallMapping[CXType_Float] = "json_marshal_float";
	g_CXType_to_FunctionCallMapping[CXType_Double] = "json_marshal_double";
	g_CXType_to_FunctionCallMapping[CXType_LongDouble] = "json_marshal_long_double";
	g_CXType_to_FunctionCallMapping[CXType_Typedef] = "json_marshal_"; /* name of struct type will get added to this prefix */
	g_CXType_to_FunctionCallMapping[CXType_Char_S] = "json_marshal_char";
}

typedef struct {
	int bitmap_field_annotation_attribute;
	char *json_field_alias;
} field_decl_annotation_info_t;


// Callback function to visit attributes within a field
static CXChildVisitResult visitAnnotateAttr(CXCursor cursor,
											CXCursor parent,
											CXClientData client_data)
{
	if (CXCursor_AnnotateAttr != clang_getCursorKind(cursor)) {
		return CXChildVisit_Continue;
	}

	field_decl_annotation_info_t *field_decl_annotation_info = (field_decl_annotation_info_t *)client_data;

	CXString annotation = clang_getCursorSpelling(cursor);
	const char * const annotatin_cstr = clang_getCString(annotation);
	do {
		if(0 == strcmp("no-marshal", annotatin_cstr)) {
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_NO_MARSHAL);
			break;
		}

		if(0 == strcmp("omit-empty", annotatin_cstr)) {
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY);
			break;
		}

		if(0 == strcmp("pointer-to-array", annotatin_cstr)) {
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_POINTER_TO_ARRAY);
			break;
		}
		if(0 == strcmp("add-double-quotes", annotatin_cstr)) {
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_ADD_DOUBLE_QUOTES);
			break;
		}
		if(0 == strcmp("boolean", annotatin_cstr)) {
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_BOOLEAN);
			break;
		}
		if(0 == strcmp("dont-add-double-quotes", annotatin_cstr)) { /* Note: use with caution to avoid json breaks */
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_ADD_DOUBLE_QUOTES);
			break;
		}
		if(0 == strcmp("dont-json-escape", annotatin_cstr)) { /* Note: use with caution to avoid json breaks */
			field_decl_annotation_info->bitmap_field_annotation_attribute |=
				(1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_DONT_JSON_ESCAPE);
			break;
		}

#define ANNOTATION_PREFIX "json_field_alias:"
		if(0 == strncmp(ANNOTATION_PREFIX,
						annotatin_cstr,
						strlen(ANNOTATION_PREFIX))) {
			field_decl_annotation_info->json_field_alias =
				strdup(annotatin_cstr + strlen(ANNOTATION_PREFIX));
			break;
		}

		std::cerr << "Error: Unknown attribute " << annotatin_cstr << std::endl;
		clang_disposeString(annotation);
		return CXChildVisit_Break;
	} while(0);
	clang_disposeString(annotation);
    return CXChildVisit_Continue;
}

typedef enum {
	FIELD_DECL_TYPE_UNKNOWN = -1,
	FIELD_DECL_TYPE_VARIABLE = 0,
	FIELD_DECL_TYPE_POINTER_TO_VARIABLE = 1,
	FIELD_DECL_TYPE_VARIABLE_ARRAY = 2,
	FIELD_DECL_TYPE_POINTER_TO_VARIABLE_ARRAY = 3,
	FIELD_DECL_TYPE_VARIABLE_2D_ARRAY = 4,
	FIELD_DECL_TYPE_POINTER_TO_VARIABLE_2D_ARRAY = 5,
} field_decl_type_t;



static CXChildVisitResult visitFieldDecl(CXCursor cursor,
										 CXCursor parent,
										 CXClientData client_data)
{
    if (CXCursor_FieldDecl != clang_getCursorKind(cursor)) {
		std::cerr << "Error: Only field declarations are allowed in struct declaration." << std::endl;
		return CXChildVisit_Break;
    }

	do {
		field_decl_annotation_info_t field_decl_annotation_info = {
			.bitmap_field_annotation_attribute = 0,
			.json_field_alias = NULL,
		};
		const int rc = clang_visitChildren(cursor,
										   visitAnnotateAttr,
										   &field_decl_annotation_info.bitmap_field_annotation_attribute);
		if(0 != rc ) {
			return CXChildVisit_Break;
		}

		if(0 != (field_decl_annotation_info.bitmap_field_annotation_attribute & (1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_NO_MARSHAL))) {
			break;
		}

		CXType field_type = clang_getCursorType(cursor);
		CXType field_type_variable = field_type;
		field_decl_type_t field_decl_type = FIELD_DECL_TYPE_UNKNOWN;
		do {
			if(CXType_ConstantArray == field_type.kind) {
				field_type_variable = clang_getArrayElementType(field_type);
				if(CXType_ConstantArray == field_type_variable.kind) {
					field_type_variable = clang_getArrayElementType(field_type_variable);
					field_decl_type = FIELD_DECL_TYPE_VARIABLE_2D_ARRAY;
				}
				else {
					field_decl_type = FIELD_DECL_TYPE_VARIABLE_ARRAY;
				}
				break;
			}

			if(CXType_Pointer == field_type.kind) {
				field_type_variable = clang_getPointeeType(field_type);
				if(CXType_ConstantArray == field_type_variable.kind) {
					field_type_variable = clang_getArrayElementType(field_type_variable);
					field_decl_type = FIELD_DECL_TYPE_POINTER_TO_VARIABLE_2D_ARRAY;
				}
				else if(0 != (field_decl_annotation_info.bitmap_field_annotation_attribute & (1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_POINTER_TO_ARRAY))) {
					field_decl_type = FIELD_DECL_TYPE_POINTER_TO_VARIABLE_ARRAY;
				}
				else {
					field_decl_type = FIELD_DECL_TYPE_POINTER_TO_VARIABLE;
				}
				break;
			}

			field_type_variable = field_type;
			field_decl_type = FIELD_DECL_TYPE_VARIABLE;
		} while(0);

		if(nullptr != g_CXType_to_FunctionCallMapping[field_type_variable.kind]) {
			CXString field_name = clang_getCursorSpelling(cursor);
			const char * const field_name_cstr = clang_getCString(field_name);
		
			std::cout << "\t";
			std::cout << g_CXType_to_FunctionCallMapping[field_type_variable.kind];
			if(CXType_Typedef == field_type_variable.kind) {
				CXString field_type_name = clang_getTypeSpelling(field_type_variable);
				const char * const field_type_name_cstr = clang_getCString(field_type_name);
				std::cout << field_type_name_cstr;
				clang_disposeString(field_type_name);
			}
			do {
				if(FIELD_DECL_TYPE_VARIABLE_ARRAY == field_decl_type ||
				   FIELD_DECL_TYPE_POINTER_TO_VARIABLE_ARRAY == field_decl_type) {
					std::cout << "_array";
					break;
				}
				if(FIELD_DECL_TYPE_VARIABLE_2D_ARRAY == field_decl_type ||
				   FIELD_DECL_TYPE_POINTER_TO_VARIABLE_2D_ARRAY == field_decl_type) {
					std::cout << "_2darray";
					break;
				}
			} while(0);
			std::cout << "("
					  << "post_data_temp_ptr, ";

			if(NULL == field_decl_annotation_info.json_field_alias) {
				std::cout << "\"" << field_name_cstr << "\", "
						  << std::dec << strlen(field_name_cstr) << " /* key_len */, ";
			}
			else {
				std::cout << "\"" << field_decl_annotation_info.json_field_alias << "\", "
						  << std::dec << strlen(field_decl_annotation_info.json_field_alias) << " /* key_len */, ";
				free(field_decl_annotation_info.json_field_alias);
			}
			if(FIELD_DECL_TYPE_VARIABLE_2D_ARRAY == field_decl_type) {
				CXType field_type_tmp = clang_getArrayElementType(field_type);
				std::cout << std::dec << clang_getArraySize(field_type_tmp) << ", ";
			}
			else if(FIELD_DECL_TYPE_POINTER_TO_VARIABLE_2D_ARRAY == field_decl_type) {
				CXType field_type_tmp = clang_getPointeeType(field_type);
				std::cout << std::dec << clang_getArraySize(field_type_tmp) << ", ";
			}
			else if(FIELD_DECL_TYPE_VARIABLE == field_decl_type) {
				std::cout << "&";
			}
			std::cout << "value->"<< field_name_cstr << ", ";
			if(FIELD_DECL_TYPE_VARIABLE_ARRAY == field_decl_type ||
			   FIELD_DECL_TYPE_POINTER_TO_VARIABLE_ARRAY == field_decl_type ||
			   FIELD_DECL_TYPE_VARIABLE_2D_ARRAY == field_decl_type ||
			   FIELD_DECL_TYPE_POINTER_TO_VARIABLE_2D_ARRAY == field_decl_type) {
				std::cout << "value->"<< field_name_cstr << "_count, ";
				if(FIELD_DECL_TYPE_VARIABLE_ARRAY == field_decl_type ||
				   FIELD_DECL_TYPE_VARIABLE_2D_ARRAY == field_decl_type) {
					std::cout << std::dec << clang_getArraySize(field_type) << ", ";
				}
				else if(FIELD_DECL_TYPE_POINTER_TO_VARIABLE_ARRAY == field_decl_type ||
						FIELD_DECL_TYPE_POINTER_TO_VARIABLE_2D_ARRAY == field_decl_type) {
					std::cout << std::dec << -1 << ", ";;
				}
			}
			std::cout<< "precede_by_comma, "
					 << "0x" << std::hex << field_decl_annotation_info.bitmap_field_annotation_attribute
					 << ");" << std::endl;

			clang_disposeString(field_name);
		}
		else {
			std::cerr << "Error: Field declaration of type not supported." << std::endl;
			return CXChildVisit_Break;
		}
	} while(0);

	return CXChildVisit_Continue;
}

static CXChildVisitResult visitStructDecl(CXCursor cursor,
										  CXCursor parent,
										  CXClientData client_data)
{
	if(CXCursor_TypedefDecl == clang_getCursorKind(cursor)) {
		return CXChildVisit_Continue;
	}

    if(CXCursor_StructDecl != clang_getCursorKind(cursor)) {
		std::cerr << "Error: Only struct declarations are allowed." << std::endl;
		return CXChildVisit_Break;
	}

	CXString struct_name = clang_getCursorSpelling(cursor);
	const char * const struct_name_cstr = clang_getCString(struct_name);
	std::cout << "void json_marshal_" << struct_name_cstr << "(char **post_data_temp_ptr, const char key[], int key_len, const struct " << struct_name_cstr << " *value, int *precede_by_comma, int bitmap_field_annotation_attribute);" << std::endl; // function declaration
	std::cout << "void json_marshal_" << struct_name_cstr << "(char ** const post_data_temp_ptr, const char key[], const int key_len, const struct " << struct_name_cstr << " * const value, int * const precede_by_comma, const int bitmap_field_annotation_attribute)" << std::endl; // function definition
	std::cout << "{" << std::endl;
	// Visit the struct's children to find its members
	const int rc = clang_visitChildren(cursor,
									   visitFieldDecl,
									   nullptr);
	if(0 != rc ) {
		clang_disposeString(struct_name);
		return CXChildVisit_Break;
	}
	std::cout << "} // json_marshal_" << struct_name_cstr << "()" << std::endl;
	clang_disposeString(struct_name);
	return CXChildVisit_Continue;
}

int main(int argc, char **argv)
{
	if(argc != 2) {
		std::cerr << "Usage:" << argv[0] << " filename" <<std::endl;
		return 2;
	}
    CXIndex clang_index = clang_createIndex(0, 0);

    const char* source_file = argv[1];
    const char* compile_args[] =
		{
			"-I/usr/noincludeyet"
		}; // Include directories
    int num_args = sizeof(compile_args) / sizeof(compile_args[0]);

    CXTranslationUnit translation_unit = clang_parseTranslationUnit(clang_index,
																	source_file,
																	compile_args,
																	num_args,
																	nullptr,
																	0,
																	CXTranslationUnit_None);

    if (!translation_unit) {
        std::cerr << "Error parsing the source file." << std::endl;
		clang_disposeIndex(clang_index);
        return 1;
    }

	init_g_CXType_to_FunctionCallMapping();
	const int rc = clang_visitChildren(clang_getTranslationUnitCursor(translation_unit),
									   visitStructDecl,
									   nullptr);

    clang_disposeTranslationUnit(translation_unit);
    clang_disposeIndex(clang_index);

    return rc;
}
