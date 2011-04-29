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
            out << "(";
            examineExpr(e1, out);
            out << "=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgPlusAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "+=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgMinusAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "-=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgAndAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "&=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgIorAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "|=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgMultAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "*=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgDivAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "/=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgModAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "%=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgXorAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "^=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgLshiftAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << "<<=";
            examineExpr(e2, out);
            out << ")";
            break;
        case V_SgRshiftAssignOp:
            binop = isSgBinaryOp(expr);
            e1 = binop->get_lhs_operand();
            e2 = binop->get_rhs_operand();
            out << "(";
            examineExpr(e1, out);
            out << ">>=";
            examineExpr(e2, out);
            out << ")";
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
