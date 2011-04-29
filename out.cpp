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
