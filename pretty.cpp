#include <rose.h>
#include <iostream>
#include <sstream>

using namespace std;

void examineExpr(SgExpression *expr, ostream &out);
void examinePrimTypeName(SgType *type, ostream &out);
void examineStatement(SgStatement *stmt, ostream &out);
void examineVariableDeclaration(SgVariableDeclaration* decl, ostream &out);

void examineType(SgType *type, ostream &out) {
    int nr_stars = 0;
    stringstream ss1;
    if (NULL == type) {
        out << "void";
        return;
    }
    while (isSgArrayType(type) ||
            isSgPointerType(type)) {
        if (isSgArrayType(type)) {
            SgArrayType *atype = isSgArrayType(type);
            SgExpression *expr = atype->get_index();

            type = atype->get_base_type();
            ss1 << "[";
            if (expr)
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
    out << ss1.str();
}

void examineScopeStatement(SgScopeStatement* scope, string name, ostream &out) {
    SgSymbolTable* symbol_table = scope->get_symbol_table();
    set<SgNode*> symbol_nodes = symbol_table->get_symbols();
    set<SgNode*>::const_iterator symbol_iter;

    out << "{" << endl;
    /*
    for (symbol_iter = symbol_nodes.begin(); 
        symbol_iter != symbol_nodes.end(); 
        ++symbol_iter) {
        SgSymbol* symbol = isSgSymbol(*symbol_iter);
        if (isSgVariableSymbol(symbol)) {
            SgVariableSymbol *varsym = isSgVariableSymbol(symbol);
            SgInitializedName *initname = varsym->get_declaration();
            SgDeclarationStatement *decl = initname->get_declaration();
            if (isSgVariableDeclaration(decl)) {
                SgVariableDeclaration *vardecl = isSgVariableDeclaration(decl);
                examineVariableDeclaration(vardecl, out);
            }
        }
    }
    */

    SgBasicBlock *body;
    if (scope->variantT() != V_SgBasicBlock) {
        out << "}" << endl;
        return;
    } else
        body = isSgBasicBlock(scope);

    SgStatementPtrList& stmt_list = body->get_statements();
    SgStatementPtrList::const_iterator stmt_iter;

    for (stmt_iter = stmt_list.begin();
            stmt_iter != stmt_list.end();
            stmt_iter++) {
        SgStatement *stmt = *stmt_iter;
        out << "/* " << stmt->unparseToString() << " */" << endl;
        examineStatement(stmt, out);
        out << endl;
    }

    out << "}" << endl;
  /*
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
  */
}

void examineExpr(SgExpression *expr, ostream &out) {
    stringstream ss1;
    stringstream ss2;
    stringstream ss3;
    SgExpression *e1;
    SgExpression *e2;
    SgBinaryOp *binop;
    SgUnaryOp *unaryop;
    SgType *type;
    switch(expr->variantT()) {
        /* Begin UnaryOp */
        case V_SgMinusOp:
             out << "(-";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgUnaryAddOp:
             out << "(+";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgNotOp:
             out << "(!";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgPointerDerefExp:
             out << "(*";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgAddressOfOp:
             out << "(&";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgMinusMinusOp:
            out << "(--";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgPlusPlusOp:
             /* TODO we need to deal with ++/--i and i++/--
              */
            unaryop = isSgUnaryOp(expr);
            SgPlusPlusOp *ppop = isSgPlusPlusOp(expr);
            
            out << "(++";
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgBitComplementOp:
            out << "(~";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
        case V_SgCastExp:
            out << "(";
            SgCastExp *castexp = isSgCastExp(expr);
            e1 = castexp->get_operand();
            type = castexp->get_type();
            examineType(type, out);
            out << ")";
            examineExpr(e1, out);
             break;
        /* End UnaryOp */
        /* Begin BinaryOp */
        case V_SgEqualityOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "==";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgLessThanOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "<";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgGreaterThanOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << ">";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgNotEqualOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "!=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgLessOrEqualOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "<=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgGreaterOrEqualOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << ">=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgAddOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "+";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgSubtractOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "-";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgMultiplyOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "*";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgDivideOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "/";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgIntegerDivideOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "/";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgModOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "%";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgAndOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "&&";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgOrOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "||";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgBitXorOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "^";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgBitAndOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "&";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgBitOrOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "|";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgCommaOpExp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << ",";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgLshiftOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "<<";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgRshiftOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << ">>";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "=";
            examineExpr(e2, out);
            break;
        case V_SgPlusAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "+=";
            examineExpr(e2, out);
            break;
        case V_SgMinusAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "-=";
            examineExpr(e2, out);
            break;
        case V_SgAndAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "&=";
            examineExpr(e2, out);
            break;
        case V_SgIorAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "|=";
            examineExpr(e2, out);
            break;
        case V_SgMultAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "*=";
            examineExpr(e2, out);
            break;
        case V_SgDivAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "/=";
            examineExpr(e2, out);
            break;
        case V_SgModAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "%=";
            examineExpr(e2, out);
            break;
        case V_SgXorAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "^=";
            examineExpr(e2, out);
            break;
        case V_SgLshiftAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "<<=";
            examineExpr(e2, out);
            break;
        case V_SgRshiftAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << ">>=";
            examineExpr(e2, out);
            break;
        case V_SgExponentiationOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "ExpUnknown";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgConcatenationOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "CatUnknown";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgPntrArrRefExp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            examineExpr(e1, out);
            out << "[";
            examineExpr(e2, out);
            out << "]";

        /* End BinaryOp */
        case V_SgVarRefExp:
            SgVarRefExp *varref = isSgVarRefExp(expr);
            if (NULL == varref)
                return;
            SgVariableSymbol *svsym = varref->get_symbol();
            if (NULL == svsym)
                return;
            out << svsym->get_name().getString();
            break;
        case V_SgLabelRefExp:
            SgLabelRefExp *labref = isSgLabelRefExp(expr);
            out << labref->get_name().getString();
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
            cerr << "UNKNOWN EXPR[" << expr->class_name() << "] " << expr->unparseToString() << endl;
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
        case V_SgTypeVoid:
            out << "void";
            break;
        default:
            out << "UNKNOWN TYPE!!! " << type->unparseToString() << endl;
            break;
    }
    return;
}

void examineInitializedName(SgInitializedName *name, ostream &out) {
    SgSymbol* symbol = name->get_symbol_from_symbol_table();
    if (NULL == symbol)
        return;
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
            if (expr)
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

    SgInitializer *initer = name->get_initializer();
    if (initer) {
        switch (initer->variantT()) {
            case V_SgAssignInitializer:
                SgAssignInitializer *ai = isSgAssignInitializer(initer);
                SgExpression *expr = ai->get_operand();
                if (expr) {
                    out << "=";
                    examineExpr(expr, out);
                }
                break;
            default:
                break;
        }
    }
}


void examineStatement(SgStatement *stmt, ostream &out) {
    SgExpression *expr;
    SgExprStatement *expr_stmt;
    int i;
    if (NULL == stmt)
        return;
    switch(stmt->variantT()) {
        case V_SgExprStatement:
        {
            expr_stmt = isSgExprStatement(stmt);
            examineExpr(expr_stmt->get_expression(), out);
            out << ";";
            break;
        }
        case V_SgVariableDeclaration:
        {
            SgVariableDeclaration *vardecl = isSgVariableDeclaration(stmt);
            examineVariableDeclaration(vardecl, out);
            break;
        }
        case V_SgReturnStmt:
        {
            SgReturnStmt *retstmt = isSgReturnStmt(stmt);
            out << "return ";
            expr = retstmt->get_expression();
            if (expr) {
                examineExpr(expr, out);
            }
            out << ";";
            break;
        }
        case V_SgForStatement:
        {
            out << "for (";
            SgForStatement *forstmt = isSgForStatement(stmt);
            SgStatementPtrList &init_stmt_list = forstmt->get_init_stmt();
            SgStatementPtrList::const_iterator init_stmt_iter;
            for (init_stmt_iter = init_stmt_list.begin();
                    init_stmt_iter != init_stmt_list.end();
                    init_stmt_iter++) {
                stmt = *init_stmt_iter;
                if (init_stmt_iter != init_stmt_list.begin())
                    out << ", ";
                expr_stmt = isSgExprStatement(stmt);
                if (expr_stmt)
                    examineExpr(expr_stmt->get_expression(), out);
            }
            out << "; ";
            expr_stmt = isSgExprStatement(forstmt->get_test());
            if (expr_stmt)
                examineExpr(expr_stmt->get_expression(), out);
            out << "; ";
            expr = forstmt->get_increment();
            examineExpr(expr, out);
            out << ")" << endl;
            examineStatement(forstmt->get_loop_body(), out);
            break;
        }
        case V_SgScopeStatement:
        {
            SgScopeStatement *scope = isSgScopeStatement(stmt);
            examineScopeStatement(scope, "for loop", out);
            break;
        }
        case V_SgIfStmt: 
        {
            SgIfStmt *ifstmt = isSgIfStmt(stmt);
            out << "if (";
            stmt = ifstmt->get_conditional();
            expr_stmt = isSgExprStatement(stmt);
            if (expr_stmt)
                examineExpr(expr_stmt->get_expression(), out);
            out << ")" << endl;
            stmt = ifstmt->get_true_body();
            examineStatement(stmt, out);
            stmt = ifstmt->get_false_body();
            if (stmt) {
                out << "else";
                examineStatement(stmt, out);
            }
            break;
        }
        case V_SgWhileStmt:
        {
            SgWhileStmt *whilestmt = isSgWhileStmt(stmt);
            expr_stmt = isSgExprStatement(whilestmt->get_condition());
            out << "while (";
            if (expr_stmt)
                examineExpr(expr_stmt->get_expression(), out);
            out << ")" << endl;
            stmt = whilestmt->get_body();
            examineStatement(stmt, out);
            break;
        }
    }
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
            if (expr)
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

    SgInitializer *initer = name->get_initializer();
    if (initer) {
        switch (initer->variantT()) {
            case V_SgAssignInitializer:
                SgAssignInitializer *ai = isSgAssignInitializer(initer);
                SgExpression *expr = ai->get_operand();
                if (expr) {
                    out << "=";
                    examineExpr(expr, out);
                }
                break;
            default:
                break;
        }
    }

    /* end of this decl */
    out << ";";

    /*
    cout << "[Decl] Variable (name:"<<symbol->get_name().getString();
    cout << ",type:"<<symbol->get_type()->class_name();
    cout << ",init:";

    SgInitializer* init_expr = name->get_initializer();
    if (init_expr) 
      cout << init_expr->class_name();
    else
      cout << "none";
    cout << ")" << endl;
    */
  }
}

void examineFunctionDeclaration(SgFunctionDeclaration* decl, ostream &out) {
    SgSymbol* symbol = decl->get_symbol_from_symbol_table();
    SgFunctionDefinition* def = decl->get_definition();

    /* We don't want to output the function withou definition */
    if (NULL == symbol || NULL == def)
        return;
    SgType *ret_type = decl->get_orig_return_type();
    examineType(ret_type, out);

    out << " " << symbol->get_name().getString();
    out << "(";

    SgInitializedNamePtrList& name_list = decl->get_args();
    SgInitializedNamePtrList::const_iterator name_iter;
    for (name_iter = name_list.begin(); 
            name_iter != name_list.end(); 
            name_iter++) {
        if (name_iter != name_list.begin())
            out << ", ";
        SgInitializedName* name = *name_iter;
        examineInitializedName(name, out);
    }
    out << ")";
    out << endl;

    SgBasicBlock* body = def->get_body();
    examineScopeStatement(body,symbol->get_name().getString(), out);
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
    /*
    examineScopeStatement(global_scope,"global");
    */

    // get the actual statements that are in this global scope
    SgDeclarationStatementPtrList& decl_list = global_scope->get_declarations();
    SgDeclarationStatementPtrList::const_iterator decl_iter;
    for(decl_iter = decl_list.begin(); 
	decl_iter != decl_list.end(); 
	decl_iter++) {
      SgDeclarationStatement* decl = *decl_iter;
      if (isSgFunctionDeclaration(decl)) 
	examineFunctionDeclaration(isSgFunctionDeclaration(decl), rets);
      if (isSgVariableDeclaration(decl)) {
	examineVariableDeclaration(isSgVariableDeclaration(decl), rets);
            rets << endl;
        }
    }
  }
  return rets.str();
}

