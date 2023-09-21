#include <iostream>
#include <clang-c/Index.h>

void PrintStructMembers(CXCursor cursor) {
    if (clang_getCursorKind(cursor) != CXCursor_FieldDecl) {
        return; // Skip non-field declarations
    }

    CXString fieldName = clang_getCursorSpelling(cursor);
    CXType fieldType = clang_getCursorType(cursor);
    CXString fieldTypeStr = clang_getTypeSpelling(fieldType);

    std::cout << "Field Name: " << clang_getCString(fieldName) << "\n";
    std::cout << "Field Type: " << clang_getCString(fieldTypeStr) << "\n\n";

    clang_disposeString(fieldName);
    clang_disposeString(fieldTypeStr);
}

void VisitStruct(CXCursor cursor) {
    if (clang_getCursorKind(cursor) == CXCursor_StructDecl) {
        CXString structName = clang_getCursorSpelling(cursor);
        std::cout << "Struct Name: " << clang_getCString(structName) << "\n\n";
        clang_disposeString(structName);

        // Visit the struct's children to find its members
        clang_visitChildren(cursor, [](CXCursor child, CXCursor parent, CXClientData data) {
            PrintStructMembers(child);
            return CXChildVisit_Continue;
        }, nullptr);
    }
}

int main(int argc, char **argv) {
    // Initialize libclang
    CXIndex index = clang_createIndex(0, 0);

    // Parse a C source file
    const char* sourceFile = argv[1];
    const char* compileArgs[] =
		{
			"-I/usr/noincludeyet"
		}; // Include directories
    int numArgs = sizeof(compileArgs) / sizeof(compileArgs[0]);

    CXTranslationUnit translationUnit = clang_parseTranslationUnit(index,
																   sourceFile,
																   compileArgs,
																   numArgs,
																   nullptr,
																   0,
																   CXTranslationUnit_None);

    if (!translationUnit) {
        std::cerr << "Error parsing the source file." << std::endl;
        return 1;
    }

    // Visit the translation unit's cursor to find structs
    clang_visitChildren(clang_getTranslationUnitCursor(translationUnit),
						[](CXCursor cursor, CXCursor parent, CXClientData data) { VisitStruct(cursor); return CXChildVisit_Continue; },
						nullptr);

    // Clean up
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);

    return 0;
}
