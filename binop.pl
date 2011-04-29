
while (<>) {
    chomp;
    if (/case /) {
        print "$_\n";
        print " "x12;
        print "binop = isSgBinaryOp(expr);\n";
        print " "x12;
        print "e1 = binop->get_lhs_operand();\n";
        print " "x12;
        print "e2 = binop->get_rhs_operand();\n";
        print " "x12;
        print "out << \"(\";\n";
        print " "x12;
        print "examineExpr(e1, out);\n";
    } elsif (/break;/) {
        print " "x12;
        print "examineExpr(e2, out);\n";
        print " "x12;
        print "out << \")\";\n";
        print " "x12;
        print "break;\n";
    } else {
        print "$_\n";
    }
}
