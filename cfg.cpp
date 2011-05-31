#include <rose.h>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

enum ThreeAddressOp {
    OP_PLUS,    /* + */
    OP_MINUS,   /* - */
    OP_MUL,     /* * */
    OP_DIV,     /* / */
    OP_MOD,     /* % */
    /* TODO more ops */
};

enum InstructionType {
    COND_JMP_INST,
    UNCOND_JMP_INST,
    ASSIGN_OP_INST,
    NR_INST_TYPE
};

class Instruction {
    protected:
        SgStatement *stmt;
        int type;
        int id;
        list<string> labs;
    public:
        Instruction(SgStatement *stmt, int id) {
            this->stmt = stmt;
            this->id = id;
        }

        int get_id() { return id; }
        void set_id(int id) { this->id = id; }

        virtual ~Instruction() {
        }

        virtual const char *get_dst() = 0;

        int add_labs(list<string> &l);
};

int Instruction::add_labs(list<string> &l) {
    list<string>::iterator iter;
    for (iter = l.begin(); iter != l.end(); iter++) {
        labs.push_back(*iter);
    }
    return labs.size();
}

class ThreeAddressInstruction : public Instruction {
    public:
        ThreeAddressInstruction(SgStatement *stmt, int id = 0);
        virtual ~ThreeAddressInstruction();

        virtual const char *get_dst() { return NULL; }
    private:
        string oprand1, oprand2;
        string dst;
        int op;
};

ThreeAddressInstruction::ThreeAddressInstruction(SgStatement *stmt, int id)
    : Instruction(stmt, id) {
    type = ASSIGN_OP_INST;
}

ThreeAddressInstruction::~ThreeAddressInstruction() {
}

class UncondJmp : public Instruction {
    public:
        UncondJmp(SgStatement *stmt, int id = 0);
        virtual ~UncondJmp() {}
        virtual const char *get_dst() { return dstlab.c_str(); }
    private:
        string dstlab;
};

UncondJmp::UncondJmp(SgStatement *stmt, int id)
    : Instruction(stmt, id) {

    SgGotoStatement *goto_stmt = isSgGotoStatement(stmt);
    if (NULL == goto_stmt)
        /* TODO Exception */
        return;
    SgLabelStatement *lab_stmt = goto_stmt->get_label();
    dstlab = lab_stmt->get_label().getString();
}

class CondJmp : public Instruction {
    public:
        CondJmp(SgStatement *stmt, int id = 0);
        virtual ~CondJmp() {}
        virtual const char *get_dst() { return dstlab.c_str(); }
    private:
        SgExpression *cond;
        string dstlab;
};

CondJmp::CondJmp(SgStatement *stmt, int id)
    : Instruction(stmt, id) {
    SgIfStmt *ifstmt = isSgIfStmt(stmt);
    SgExprStatement *expr_stmt;

    cond = NULL;
    if (NULL == ifstmt)
        /* TODO exception */
        return;
    stmt = ifstmt->get_conditional();
    expr_stmt = isSgExprStatement(stmt);

    if (expr_stmt) {
        cond = expr_stmt->get_expression();
    }
    stmt = ifstmt->get_true_body();

    UncondJmp uncond(stmt);
    dstlab = uncond.get_dst();
}

class InstructionList {
    private:
        list<Instruction *> ilist;
        list<int> leaders;
        list<string> labs;

    protected:
        Instruction *append(SgExprStatement *expr);

    public:
        InstructionList() {}
        virtual ~InstructionList() {
            list<Instruction *>::iterator iter;
            for (iter = ilist.begin(); iter != ilist.end(); iter++) {
                Instruction *inst = *iter;
                delete inst;
            }
        }

        Instruction *append(SgStatement *stmt);
        Instruction *append(SgVariableDeclaration *vardecl);
};

Instruction *InstructionList::append(SgVariableDeclaration *vardecl) {
    Instruction *inst = NULL;
    /*
    cout << "[VarDecl] " << vardecl->unparseToString() << endl;
    */
    return inst;
}

Instruction *InstructionList::append(SgExprStatement *expr) {
    Instruction *inst = NULL;

    /*
    list<string>::iterator iter;

    for (iter = labs.begin(); iter != labs.end(); ++iter) {
        cout << *iter << ": ";
    }
    cout << "[ExprStmt] " << expr->unparseToString() << endl;
    */

    inst = new ThreeAddressInstruction((SgStatement *)expr);

    return inst;
}

Instruction *InstructionList::append(SgStatement *stmt) {
    Instruction *inst = NULL;
    switch (stmt->variantT()) {
        case V_SgVariableDeclaration:
        {
            SgVariableDeclaration *vardecl = isSgVariableDeclaration(stmt);
            inst = append(vardecl);
        }
        case V_SgExprStatement:
        {
            SgExprStatement *expr_stmt = isSgExprStatement(stmt);
            inst = append(expr_stmt);
        }
        case V_SgReturnStmt:
        {
            cout << "[ReturnStmt] " << stmt->unparseToString() << endl;
            labs.clear();
            break;
        }
        case V_SgIfStmt: 
        {
            inst = new CondJmp(stmt);
            cout << "[IfStmt] " << stmt->unparseToString() << endl;
            cout << "\tjmp dst: " << inst->get_dst() << endl;
            break;
        }
        case V_SgLabelStatement:
        {
            SgLabelStatement *labstmt = isSgLabelStatement(stmt);
            labs.push_back(labstmt->get_label().getString());
            inst = NULL;
            break;
        }
        default:
        {
            break;
        }
    }

    if (NULL != inst) {
        inst->add_labs(labs);
        inst->set_id(ilist.size());
        labs.clear();
        ilist.push_back(inst);
        if (0 == ilist.size())
            leaders.push_back(inst->get_id());
    }
    return inst;
}

/*
class BasicBlock {
    private:
        list<Instruction *> inst_list;
        static int nr_blk;
        int id;
    public:
        BasicBlock() {
            id = nr_blk;
            nr_blk++;
        }
        virtual ~BasicBlock() {}
};

int BasicBlock::nr_blk = 0;

class ControlFlowGraph {
    public:
        ControlFlowGraph() {}
        virtual ~ControlFlowGraph() {}
};
*/

int examine_scope(SgScopeStatement *scope, InstructionList *ilist) {
    SgBasicBlock *body;

    if (scope->variantT() != V_SgBasicBlock) {
        return -1;
    } else
        body = isSgBasicBlock(scope);

    SgStatementPtrList& stmt_list = body->get_statements();
    SgStatementPtrList::const_iterator stmt_iter;
    for (stmt_iter = stmt_list.begin();
            stmt_iter != stmt_list.end();
            stmt_iter++) {
        SgStatement *stmt = *stmt_iter;
        ilist->append(stmt);
    }
    return 0;
}

int examine_func(SgFunctionDeclaration *decl, InstructionList *ilist) {
    SgSymbol* symbol = decl->get_symbol_from_symbol_table();
    SgFunctionDefinition* def = decl->get_definition();

    /* We don't want to output the function withou definition */
    if (NULL == symbol || NULL == def)
        return -1;

    /* We don't consider the parameters to the function */
    /*
     * SgInitializedNamePtrList& name_list = decl->get_args();
     */

    SgBasicBlock* body = def->get_body();
    return examine_scope(body, ilist);
}

string prettyPrint(SgProject* project) {
    string ret = "";
    SgFilePtrList& file_list = project->get_fileList();
    SgFilePtrList::const_iterator file_iter;

    InstructionList *ilist = new InstructionList();

    /* Iterate over all files */
    for (file_iter = file_list.begin(); 
        file_iter != file_list.end(); 
        file_iter++) {
        SgSourceFile* file = isSgSourceFile(*file_iter);
        // cout << "[Print] File: " << file->getFileName() << endl;

        // print the symbol table at the global scope; SgGlobal is a
        // subclass of SgScopeStatement
        SgGlobal* global_scope = file->get_globalScope(); 

        // get the actual statements that are in this global scope
        SgDeclarationStatementPtrList& decl_list = global_scope->get_declarations();
        SgDeclarationStatementPtrList::const_iterator decl_iter;
        for(decl_iter = decl_list.begin(); 
                decl_iter != decl_list.end(); 
                decl_iter++) {
            SgDeclarationStatement* decl = *decl_iter;
            if (isSgFunctionDeclaration(decl))  {
                examine_func(isSgFunctionDeclaration(decl), ilist);
            } else if (isSgVariableDeclaration(decl)) {
            }
        }
    }

    /* Analyze the list of instructions */


    return ret;
}

