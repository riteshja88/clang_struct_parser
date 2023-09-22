#include <iostream>
#include <clang-c/Index.h>

void visit_struct_member(CXCursor cursor)
{
    if (clang_getCursorKind(cursor) != CXCursor_FieldDecl) {
        return; // Skip non-field declarations
    }

    CXString field_name = clang_getCursorSpelling(cursor);
	const char * const field_name_cstr = clang_getCString(field_name);

	CXType field_type = clang_getCursorType(cursor);
	CXString field_type_str = clang_getTypeSpelling(field_type);
	const char * const field_type_cstr = clang_getCString(field_type_str);
	
	std::cout << field_type.kind << " " << field_name_cstr << "();" << std::endl;
    
    clang_disposeString(field_name);
    clang_disposeString(field_type_str);
}

void visit_struct_declaration(CXCursor cursor)
{
    if (CXCursor_StructDecl == clang_getCursorKind(cursor)) {
        CXString struct_name = clang_getCursorSpelling(cursor);
		const char * const struct_name_cstr = clang_getCString(struct_name);
		std::cout << struct_name_cstr << "{" << std::endl;
        // Visit the struct's children to find its members
        clang_visitChildren(cursor,
							[](CXCursor child, CXCursor parent, CXClientData data)
							{
								visit_struct_member(child);
								return CXChildVisit_Continue;
							},
							nullptr);
		std::cout << "} //" << struct_name_cstr << std::endl;
		clang_disposeString(struct_name);
    }
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

    clang_visitChildren(clang_getTranslationUnitCursor(translation_unit),
						[](CXCursor cursor, CXCursor parent, CXClientData data)
						{
							visit_struct_declaration(cursor);
							return CXChildVisit_Continue;
						},
						nullptr);

    clang_disposeTranslationUnit(translation_unit);
    clang_disposeIndex(clang_index);

    return 0;
}
