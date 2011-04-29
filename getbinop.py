binopstr = "SgArrowExp, SgDotExp, SgDotStarOp, SgArrowStarOp, SgEqualityOp, SgLessThanOp, SgGreaterThanOp, SgNotEqualOp, SgLessOrEqualOp, SgGreaterOrEqualOp, SgAddOp, SgSubtractOp, SgMultiplyOp, SgDivideOp, SgIntegerDivideOp, SgModOp, SgAndOp, SgOrOp, SgBitXorOp, SgBitAndOp, SgBitOrOp, SgCommaOpExp, SgLshiftOp, SgRshiftOp, SgPntrArrRefExp, SgScopeOp, SgAssignOp, SgPlusAssignOp, SgMinusAssignOp, SgAndAssignOp, SgIorAssignOp, SgMultAssignOp, SgDivAssignOp, SgModAssignOp, SgXorAssignOp, SgLshiftAssignOp, SgRshiftAssignOp, SgExponentiationOp, SgConcatenationOp, SgPointerAssignOp"

binop = binopstr.split(", ")

out = open("out.cpp", "w+")

for op in binop:
    print op + ": "
    sym = raw_input()
    sym = sym.strip()
    if not sym:
        continue
    out.write("        case ")
    out.write("V_" + op + ":\n")
    out.write("            out << \"" + sym + "\";\n")
    out.write("            break;\n")

