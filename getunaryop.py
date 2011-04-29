binopstr = "SgExpressionRoot, SgMinusOp, SgUnaryAddOp, SgNotOp, SgPointerDerefExp, SgAddressOfOp, SgMinusMinusOp, SgPlusPlusOp, SgBitComplementOp, SgCastExp, SgThrowOp, SgRealPartOp, SgImagPartOp, SgConjugateOp"
binop = binopstr.split(", ")

out = open("unary.cpp", "w+")

for op in binop:
    print op + ": "
    sym = raw_input()
    sym = sym.strip()
    if not sym:
        continue
    out.write("        case ")
    out.write("V_" + op + ":\n")
    out.write("             out << \"(" + sym + "\";\n")
    out.write(" "*12)
    out.write("unaryop = isSgUnaryOp(expr);\n")
    out.write(" "*12)
    out.write("e1 = unaryop->get_operand();\n")
    out.write(" "*12)
    out.write("examineExpr(e1, out);\n")
    out.write(" "*12)
    out.write("out << \")\";\n")
    out.write("             break;\n")

