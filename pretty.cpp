#include <rose.h>

using namespace std;

void examineScopeStatement(SgScopeStatement* scope, string name) {
  SgSymbolTable* symbol_table = scope->get_symbol_table();
  set<SgNode*> symbol_nodes = symbol_table->get_symbols();
  set<SgNode*>::const_iterator symbol_iter;
  int num_vars = 0;
  for (symbol_iter = symbol_nodes.begin(); 
       symbol_iter != symbol_nodes.end(); 
       ++symbol_iter) {
    SgSymbol* symbol = isSgSymbol(*symbol_iter);
    cout << "[Scope " << name << "] Symbol: "<<symbol->get_name().getString()<<endl;
    if (isSgVariableSymbol(symbol)) num_vars++;
  }
  cout << "[Scope " << name << "] Num symbols: " << symbol_nodes.size() << endl;      
  cout << "[Scope " << name << "] Num variable symbols: " << num_vars << endl;      
}

void examineVariableDeclaration(SgVariableDeclaration* decl) {
  SgInitializedNamePtrList& name_list = decl->get_variables();
  SgInitializedNamePtrList::const_iterator name_iter;
  for (name_iter = name_list.begin(); 
       name_iter != name_list.end(); 
       name_iter++) {
    SgInitializedName* name = *name_iter;
    SgSymbol* symbol = name->get_symbol_from_symbol_table();
    cout << "[Decl] Variable (name:"<<symbol->get_name().getString();
    cout << ",type:"<<symbol->get_type()->class_name();
    cout << ",init:";
    SgInitializer* init_expr = name->get_initializer();
    if (init_expr) 
      cout << init_expr->class_name();
    else
      cout << "none";
    cout << ")" << endl;
  }
}

void examineFunctionDeclaration(SgFunctionDeclaration* decl) {
  SgSymbol* symbol = decl->get_symbol_from_symbol_table();
  if (symbol) { // for some reason, some functions do not have symbols
    cout << "[Func] Function (name:"<<symbol->get_name().getString();
    cout << ",type:"<<symbol->get_type()->class_name() << ")" << endl;
  } else {
    cout << "[Func] Function (no name)" << endl;
  }
  SgFunctionDefinition* def = decl->get_definition();
  if (def) {
    SgBasicBlock* body = def->get_body();
    SgStatementPtrList& stmt_list = body->get_statements();
    cout << "[Func] - " << stmt_list.size() << " statements" << endl;
    // An SgBasicBlock is a subclass of SgScopeStatement
    examineScopeStatement(body,symbol->get_name().getString());
  } else {
    cout << "[Func] - no body" << endl;
  }
}

string prettyPrint(SgProject* project) {
  SgFilePtrList& file_list = project->get_fileList();
  SgFilePtrList::const_iterator file_iter;
  string ret;
  for (file_iter = file_list.begin(); 
       file_iter != file_list.end(); 
       file_iter++) {
    SgSourceFile* file = isSgSourceFile(*file_iter);
    // cout << "[Print] File: " << file->getFileName() << endl;

    // print the symbol table at the global scope; SgGlobal is a
    // subclass of SgScopeStatement
    SgGlobal* global_scope = file->get_globalScope(); 
    examineScopeStatement(global_scope,"global");

    // get the actual statements that are in this global scope
    SgDeclarationStatementPtrList& decl_list = global_scope->get_declarations();
    SgDeclarationStatementPtrList::const_iterator decl_iter;
    for(decl_iter = decl_list.begin(); 
	decl_iter != decl_list.end(); 
	decl_iter++) {
      SgDeclarationStatement* decl = *decl_iter;
      if (isSgFunctionDeclaration(decl)) 
	examineFunctionDeclaration(isSgFunctionDeclaration(decl));
      if (isSgVariableDeclaration(decl)) 
	examineVariableDeclaration(isSgVariableDeclaration(decl));
    }
  }
  return "\nnot implemented yet!\n";
}

