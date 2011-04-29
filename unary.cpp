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
             out << "(++";
            unaryop = isSgUnaryOp(expr);
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
             out << "(cast";
            unaryop = isSgUnaryOp(expr);
            e1 = unaryop->get_operand();
            examineExpr(e1, out);
            out << ")";
             break;
