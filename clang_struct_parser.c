#include <iostream>
#include <string.h>
#include <clang-c/Index.h>

static const char *g_CXType_to_FunctionCallMapping[500]; // one big value chosen without digging in much deep

static void init_g_CXType_to_FunctionCallMapping() {
	for(int i=0;i<sizeof(g_CXType_to_FunctionCallMapping)/sizeof(g_CXType_to_FunctionCallMapping[0]);i++) {
		g_CXType_to_FunctionCallMapping[i] = nullptr;
	}
	g_CXType_to_FunctionCallMapping[CXType_Int] = "json_marshal_int";
	g_CXType_to_FunctionCallMapping[CXType_Long] = "json_marshal_long";
}

enum {
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_NO_MARSHAL = 0,
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY = 1,
	BITPOS_FIELD_ANNOTATION_ATTRIBUTE_POINTER_TO_ARRAY = 2,
};


// Callback function to visit attributes within a field
static CXChildVisitResult visitAnnotateAttr(CXCursor cursor,
											CXCursor parent,
											CXClientData client_data)
{
	int *bitmap_field_annotation_attribute = (int*)client_data;
	if (CXCursor_AnnotateAttr != clang_getCursorKind(cursor)) {
		std::cerr << "Error: Only annotation attributes are allowed for field declarations." << std::endl;
		return CXChildVisit_Break;
    }

	CXString annotation = clang_getCursorSpelling(cursor);
	const char * const annotatin_cstr = clang_getCString(annotation);
	do {
		if(0 == strcmp("no-marshal", annotatin_cstr)) {
			*bitmap_field_annotation_attribute |= (1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_NO_MARSHAL);
			break;
		}

		if(0 == strcmp("omit-empty", annotatin_cstr)) {
			*bitmap_field_annotation_attribute |= (1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_OMIT_EMPTY);
			break;
		}

		if(0 == strcmp("pointer-to-array", annotatin_cstr)) {
			*bitmap_field_annotation_attribute |= (1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_POINTER_TO_ARRAY);
			break;
		}
		std::cerr << "Error: Unknown attribute " << annotatin_cstr << std::endl;
		clang_disposeString(annotation);
		return CXChildVisit_Break;
	} while(0);
	clang_disposeString(annotation);
    return CXChildVisit_Continue;
}


static CXChildVisitResult visitFieldDecl(CXCursor cursor,
										 CXCursor parent,
										 CXClientData client_data)
{
    if (CXCursor_FieldDecl != clang_getCursorKind(cursor)) {
		std::cerr << "Error: Only field declarations are allowed in struct declaration." << std::endl;
		return CXChildVisit_Break;
    }
    
	CXType field_type = clang_getCursorType(cursor);
	CXType field_type_final = field_type;
	const char *ampersand = "&";

	
	CXString field_name = clang_getCursorSpelling(cursor);
	const char * const field_name_cstr = clang_getCString(field_name);
	//std::cout << "\t";
	//std::cout << field_type.kind << " " << field_name_cstr << "();" << std::endl;

	do {
		int is_pointer_to_array = 0;
		int bitmap_field_annotation_attribute_CXType_Pointer = 0;
		if(CXType_Pointer == field_type.kind) {
			const int rc = clang_visitChildren(cursor,
											   visitAnnotateAttr,
											   &bitmap_field_annotation_attribute_CXType_Pointer);
			if(0 != rc ) {
				clang_disposeString(field_name);
				return CXChildVisit_Break;
			}
			if(0 == (bitmap_field_annotation_attribute_CXType_Pointer & (1 <<  BITPOS_FIELD_ANNOTATION_ATTRIBUTE_POINTER_TO_ARRAY))) {
				field_type_final = clang_getPointeeType(field_type);
				ampersand = "";
				is_pointer_to_array = 0;
			}
			else {
				field_type_final = clang_getPointeeType(field_type);
				is_pointer_to_array = 1;
			}
		}

		if(CXType_ConstantArray == field_type.kind ||
		   1 == is_pointer_to_array) {
			int bitmap_field_annotation_attribute = 0;
			if(CXType_ConstantArray == field_type.kind) {
				field_type_final = clang_getArrayElementType(field_type);
			}
			if(1 == is_pointer_to_array) {
				bitmap_field_annotation_attribute = bitmap_field_annotation_attribute_CXType_Pointer;
			}
			if(nullptr != g_CXType_to_FunctionCallMapping[field_type_final.kind]) {
				CXString field_name = clang_getCursorSpelling(cursor);
				const char * const field_name_cstr = clang_getCString(field_name);
				std::cout << "\t";
				std::cout << g_CXType_to_FunctionCallMapping[field_type_final.kind] << "_array"
						  << "(post_data_temp_ptr, \""
						  << field_name_cstr << "\", "
						  << "value->"<< field_name_cstr << ", "
						  << "value->"<< field_name_cstr << "_count"
						  << ", precede_by_comma"
						  << ", 0x" << std::hex << bitmap_field_annotation_attribute
						  << ");" << std::endl;

				clang_disposeString(field_name);
				break;
			}
			else {
				CXString field_type_final_str = clang_getTypeSpelling(field_type_final);
				const char * const field_type_final_cstr = clang_getCString(field_type_final_str);
				std::cerr << "Error: " << field_type_final_cstr << " type is not supported." << std::endl;
				clang_disposeString(field_type_final_str);
				clang_disposeString(field_name);
				return CXChildVisit_Break;
			}
		}

		if(nullptr != g_CXType_to_FunctionCallMapping[field_type_final.kind]) {
			int bitmap_field_annotation_attribute = 0;
			const int rc = clang_visitChildren(cursor,
											   visitAnnotateAttr,
											   &bitmap_field_annotation_attribute);
			if(0 != rc ) {
				clang_disposeString(field_name);
				return CXChildVisit_Break;
			}
			if(0 == (bitmap_field_annotation_attribute & (1 << BITPOS_FIELD_ANNOTATION_ATTRIBUTE_NO_MARSHAL))) {
				std::cout << "\t";
				std::cout << g_CXType_to_FunctionCallMapping[field_type_final.kind]
						  << "(post_data_temp_ptr, \""
						  << field_name_cstr << "\", "
						  << ampersand << "value->" << field_name_cstr
						  << ", precede_by_comma"
						  << ", 0x" << std::hex << bitmap_field_annotation_attribute
						  << ");" << std::endl;
			}
		}
		else {
			CXString field_type_final_str = clang_getTypeSpelling(field_type_final);
			const char * const field_type_final_cstr = clang_getCString(field_type_final_str);
			std::cerr << "Error: " << field_type_final_cstr << " type is not supported." << std::endl;
			clang_disposeString(field_type_final_str);
			clang_disposeString(field_name);
			return CXChildVisit_Break;
		}

		
	} while (0);


	clang_disposeString(field_name);
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
	std::cout << "void json_marshal_" << struct_name_cstr << "(char **post_data_temp_ptr, const char key[], const struct " << struct_name_cstr << " *value, int *precede_by_comma);" << std::endl; // function declaration
	std::cout << "void json_marshal_" << struct_name_cstr << "(char ** const post_data_temp_ptr, const char key[], const struct " << struct_name_cstr << " * const value, int * const precede_by_comma)" << std::endl; // function definition
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
