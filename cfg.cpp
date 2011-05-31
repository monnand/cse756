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
    RETURN_INST,
    NR_INST_TYPE
};

class Instruction {
    protected:
        SgStatement *stmt;
        int type;
        int id;
        list<string> labs;
        string str_labs;
    public:
        Instruction(SgStatement *stmt, int id) {
            this->stmt = stmt;
            this->id = id;
        }

        bool is_jmp() { return type == COND_JMP_INST || type == UNCOND_JMP_INST; }
        bool is_cond_jmp() { return type == COND_JMP_INST; }
        bool is_uncond_jmp() { return type == UNCOND_JMP_INST; }
        bool is_valid() { return type >= COND_JMP_INST && type < NR_INST_TYPE; }

        int get_id() { return id; }
        void set_id(int id) { this->id = id; }

        virtual ~Instruction() {
        }

        virtual const char *get_dst() { return NULL; }

        const list<string> &get_labs() { return labs; }
        int add_labs(list<string> &l);
        bool in_labs(string lab);
        bool in_labs(list<string> &l);

        const char *get_original_code() { return (str_labs + stmt->unparseToString()).c_str(); }
};

bool Instruction::in_labs(string lab) {
    list<string>::iterator iter;
    for (iter = labs.begin(); iter != labs.end(); iter++)
        if (lab == *iter)
            return true;
    return false;
}

bool Instruction::in_labs(list<string> &l) {
    list<string>::iterator iter;
    for (iter = l.begin(); iter != l.end(); iter++)
        if (in_labs(*iter))
            return true;
    return false;
}

int Instruction::add_labs(list<string> &l) {
    list<string>::iterator iter;
    for (iter = l.begin(); iter != l.end(); iter++) {
        labs.push_back(*iter);
        str_labs += (*iter) + ": ";
    }
    return labs.size();
}

class ThreeAddressInstruction : public Instruction {
    public:
        ThreeAddressInstruction(SgStatement *stmt, int id = 0);
        virtual ~ThreeAddressInstruction();
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

class ReturnInstruction : public Instruction {
    public:
        ReturnInstruction(SgStatement *stmt, int id = 0) : Instruction(stmt, id) { type = RETURN_INST; }
        virtual ~ReturnInstruction() {}
};

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
    type = UNCOND_JMP_INST;
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

    type = COND_JMP_INST;

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

class BasicBlock {
    private:
        list<Instruction *> inst_list;
        static int nr_blk;
        int id;

        list<string> dst_labs;
        list<string> labs;
    public:
        BasicBlock() {
            id = nr_blk;
            nr_blk++;
        }
        virtual ~BasicBlock() {}
        int add_instruction(Instruction *inst);
        void basic_info(ostream &out) {
            out << "block id: " << id << endl;
            out << "\tPoint to ";
            list<string>::iterator iter;
            for (iter = dst_labs.begin(); iter != dst_labs.end(); iter++) {
                out << *iter << " ";
            }
            out << endl << "\tLabs: ";
            for (iter = labs.begin(); iter != labs.end(); iter++) {
                out << *iter << " ";
            }

            out << endl << "\tInstruction:" << endl;

            list<Instruction *>::iterator iiter;
            for (iiter = inst_list.begin(); iiter != inst_list.end(); iiter++) {
                Instruction *i = *iiter;
                out << "\t  " << i->get_original_code() << endl;
            }
            out << endl;
        }
};

int BasicBlock::nr_blk = 0;

int BasicBlock::add_instruction(Instruction *inst) {
    const list<string> &ilabs = inst->get_labs();
    list<string>::const_iterator iter;

    for(iter = ilabs.begin(); iter != ilabs.end(); iter++) {
        string l = *iter;
        labs.push_back(l);
    }

    if (inst->is_jmp()) {
        string dst = inst->get_dst();
        dst_labs.push_back(dst);
    }

    inst_list.push_back(inst);

    return 0;
}

class InstructionList {
    private:
        list<Instruction *> ilist;
        list<int> leaders;
        list<string> labs;
        list<string> jmp_dsts;
        list<int> jmp_next;

        bool is_jmp_next;

    protected:
        Instruction *append(SgExprStatement *expr);
        Instruction *append(SgVariableDeclaration *vardecl);

    public:
        InstructionList() { is_jmp_next = true; }
        virtual ~InstructionList() {
            list<Instruction *>::iterator iter;
            for (iter = ilist.begin(); iter != ilist.end(); iter++) {
                Instruction *inst = *iter;
                delete inst;
            }
        }

        list<Instruction *> &get_inst_list() { return ilist; }

        Instruction *append(SgStatement *stmt);
        bool is_leader(Instruction *inst);
        int build_leaders();
        int build_basic_blocks(list<BasicBlock *> &blist);
};

int InstructionList::build_basic_blocks(list<BasicBlock *> &blist) {
    BasicBlock *block = NULL;
    list<Instruction *>::iterator iter;
    for (iter = ilist.begin(); iter != ilist.end(); iter++) {
        Instruction *inst = *iter;
        if (is_leader(inst)) {
            if (NULL != block)
                blist.push_back(block);
            block = new BasicBlock();
        }
        if (NULL != block)
            block->add_instruction(inst);
    }
    if (NULL != block)
        blist.push_back(block);

    return 0;
}

bool InstructionList::is_leader(Instruction *inst) {
    if (NULL == inst)
        return false;

    list<string>::iterator iter;
    for(iter = jmp_dsts.begin(); iter != jmp_dsts.end(); iter++) {
        string dst = *iter;

        if (inst->in_labs(dst)) {
            return true;
        }
    }

    list<int>::iterator int_iter;
    for(int_iter = jmp_next.begin(); int_iter != jmp_next.end(); int_iter++) {
        if (*int_iter == inst->get_id()) {
            return true;
        }
    }
    return false;
}

int InstructionList::build_leaders() {
    leaders.clear();
    list<Instruction *>::iterator iter;
    for (iter = ilist.begin(); iter != ilist.end(); iter++) {
        Instruction *inst = *iter;
        if (is_leader(*iter)) {
            leaders.push_back((*iter)->get_id());
        }
    }
    return 0;
}

Instruction *InstructionList::append(SgVariableDeclaration *vardecl) {
    Instruction *inst = NULL;
    return inst;
}

Instruction *InstructionList::append(SgExprStatement *expr) {
    Instruction *inst = NULL;
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
            break;
        }
        case V_SgExprStatement:
        {
            SgExprStatement *expr_stmt = isSgExprStatement(stmt);
            inst = append(expr_stmt);
            break;
        }
        case V_SgReturnStmt:
        {
            labs.clear();
            inst = new ReturnInstruction(stmt);
            break;
        }
        case V_SgIfStmt: 
        {
            inst = new CondJmp(stmt);
            break;
        }
        case V_SgGotoStatement:
        {
            inst = new UncondJmp(stmt);
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
        if (is_jmp_next) {
            jmp_next.push_back(inst->get_id());
            is_jmp_next = false;
        }
        if (inst->is_jmp()) {
            is_jmp_next = true;
            string dst = inst->get_dst();
            jmp_dsts.push_back(dst);
        } else {
            is_jmp_next = false;
        }
    }
    return inst;
}

/*
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

    list<BasicBlock *> blist;
    ilist->build_basic_blocks(blist);

    list<BasicBlock *>::iterator iter;

    for(iter = blist.begin(); iter != blist.end(); iter++) {
        (*iter) -> basic_info(cout);
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

