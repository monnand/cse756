#include <rose.h>
#include <iostream>
#include <sstream>

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

void getOperatorSym(SgBinaryOp *binop, ostream &out) {
    switch(binop->variantT()) {
        case V_SgEqualityOp:
            out << "==";
            break;
        case V_SgLessThanOp:
            out << "<";
            break;
        case V_SgGreaterThanOp:
            out << ">";
            break;
        case V_SgNotEqualOp:
            out << "!=";
            break;
        case V_SgLessOrEqualOp:
            out << "<=";
            break;
        case V_SgGreaterOrEqualOp:
            out << ">=";
            break;
        case V_SgAddOp:
            out << "+";
            break;
        case V_SgSubtractOp:
            out << "-";
            break;
        case V_SgMultiplyOp:
            out << "*";
            break;
        case V_SgDivideOp:
            out << "/";
            break;
        case V_SgIntegerDivideOp:
            out << "/";
            break;
        case V_SgModOp:
            out << "%";
            break;
        case V_SgAndOp:
            out << "&&";
            break;
        case V_SgOrOp:
            out << "||";
            break;
        case V_SgBitXorOp:
            out << "^";
            break;
        case V_SgBitAndOp:
            out << "&";
            break;
        case V_SgBitOrOp:
            out << "|";
            break;
        case V_SgCommaOpExp:
            out << ",";
            break;
        case V_SgLshiftOp:
            out << "<<";
            break;
        case V_SgRshiftOp:
            out << ">>";
            break;
        case V_SgAssignOp:
            out << "=";
            break;
        case V_SgPlusAssignOp:
            out << "+=";
            break;
        case V_SgMinusAssignOp:
            out << "-=";
            break;
        case V_SgAndAssignOp:
            out << "&=";
            break;
        case V_SgIorAssignOp:
            out << "|=";
            break;
        case V_SgMultAssignOp:
            out << "*=";
            break;
        case V_SgDivAssignOp:
            out << "/=";
            break;
        case V_SgModAssignOp:
            out << "%=";
            break;
        case V_SgXorAssignOp:
            out << "^=";
            break;
        case V_SgLshiftAssignOp:
            out << "<<=";
            break;
        case V_SgRshiftAssignOp:
            out << ">>=";
            break;
        case V_SgExponentiationOp:
            out << "ExpUnknown";
            break;
        case V_SgConcatenationOp:
            out << "CatUnknown";
            break;
        default:
            break;
    }
    out << "??";
    return;
}

void examineExpr(SgExpression *expr, ostream &out) {
    stringstream ss1;
    stringstream ss2;
    stringstream ss3;
    SgExpression *e1;
    SgExpression *e2;
    switch(expr->variantT()) {
        case V_SgBinaryOp:
            SgBinaryOp *binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            examineExpr(e1, ss1);
            e2 = binop->get_rhs_operand();
            examineExpr(e2, ss2);
            getOperatorSym(binop, ss3);
            out << ss1.str() << ss3.str() << ss2.str();
            break;
        case V_SgIntVal:
            SgIntVal *intval = isSgIntVal(expr);
            out << intval->get_value();
            break;
        case V_SgLongIntVal:
            SgLongIntVal *longval = isSgLongIntVal(expr);
            out << longval->get_value() << "L";
            break;
        case V_SgUnsignedIntVal:
            SgUnsignedIntVal *uintval = isSgUnsignedIntVal(expr);
            out << uintval->get_value() << "U";
            break;
        case V_SgUnsignedLongVal:
            SgUnsignedLongVal *ulongval = isSgUnsignedLongVal(expr);
            out << ulongval->get_value() << "UL";
            break;
        default:
            cerr << "UNKNOWN EXPR: " << expr->unparseToString() << endl;
            break;
    }
    return;
}

void examinePrimTypeName(SgType *type, ostream &out) {
    switch(type->variantT()) {
        case V_SgTypeInt:
            out << "int";
            break;
        case V_SgTypeLong:
            out << "long";
            break;
        case V_SgTypeDouble:
            out << "double";
            break;
        case V_SgTypeFloat:
            out << "float";
            break;
        default:
            out << "UNKNOWN TYPE!!!";
            break;
    }
    return;
}

void examineVariableDeclaration(SgVariableDeclaration* decl, ostream &out) {
  SgInitializedNamePtrList& name_list = decl->get_variables();
  SgInitializedNamePtrList::const_iterator name_iter;
  for (name_iter = name_list.begin(); 
       name_iter != name_list.end(); 
       name_iter++) {
    SgInitializedName* name = *name_iter;
    SgSymbol* symbol = name->get_symbol_from_symbol_table();
    SgType *type = symbol->get_type();
    int nr_stars = 0;
    stringstream ss1;

    while (isSgArrayType(type) ||
            isSgPointerType(type)) {
        if (isSgArrayType(type)) {
            SgArrayType *atype = isSgArrayType(type);
            SgExpression *expr = atype->get_index();

            type = atype->get_base_type();
            ss1 << "[";
            examineExpr(expr, ss1);
            ss1 << "]";
        } else {
            SgPointerType *ttype = isSgPointerType(type);
            type = ttype->get_base_type();
            nr_stars++;
        }
    }

    examinePrimTypeName(type, out);
    out << " ";
    for (int i = 0; i < nr_stars; ++i)
        out << "*";
    out << symbol->get_name().getString();
    out << ss1.str();


    /* end of this decl */
    out << ";" << endl;

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

void examineFunctionDeclaration(SgFunctionDeclaration* decl, ostream &out) {
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
  stringstream rets;
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
	examineFunctionDeclaration(isSgFunctionDeclaration(decl), rets);
      if (isSgVariableDeclaration(decl)) 
	examineVariableDeclaration(isSgVariableDeclaration(decl), rets);
    }
  }
  return rets.str();
}

