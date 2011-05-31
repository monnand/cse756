#include <rose.h>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>

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

const int ENTRY_BLOCK = -1;
const int EXIT_BLOCK = -2;

class BasicBlock {
    private:
        list<Instruction *> inst_list;
        static int nr_blk;
        int id;

        set<string> dst_labs;
        set<string> labs;

        set<BasicBlock *> successors;
        set<BasicBlock *> predecessors;

    public:
        BasicBlock() {
            id = nr_blk;
            nr_blk++;
        }
        BasicBlock(int id) {
            if (id == ENTRY_BLOCK || id == EXIT_BLOCK)
                this->id = id;
        }
        virtual ~BasicBlock() {}
        int add_instruction(Instruction *inst);

        set<BasicBlock *> &get_successors() { return successors; }
        set<BasicBlock *> &get_predecessors() { return predecessors; }

        int get_id() { return id; }

        string name() { 
            stringstream ss;
            if (id >= 0) {
                ss << "blk" << id;
            } else if (id == ENTRY_BLOCK) {
                ss << "ENTRY";
            } else if (id == EXIT_BLOCK) {
                ss << "EXIT";
            } else {
                ss << "UNKNOWN";
            }
            return ss.str();
        }

        bool point_to(BasicBlock *blk);
        BasicBlock *append_exit() {
            BasicBlock *exitblk = new BasicBlock(EXIT_BLOCK);
            successors.insert(exitblk);
            exitblk->predecessors.insert(this);
            return exitblk;
        }

        void basic_info(ostream &out) {
            out << "block id: " << id << endl;
            out << "\tPoint to ";
            set<string>::iterator iter;
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

bool BasicBlock::point_to(BasicBlock *blk) {
    set<string>::iterator iter;
    set<string>::iterator test;
    bool ret = false;
    if (id < 0) {
        if (blk->id == 0 && id == ENTRY_BLOCK)
            ret = true;
        else
            ret = false;
    }
    
    if (id + 1 == blk->id)
        ret = true;
    for (iter = dst_labs.begin(); iter != dst_labs.end(); iter++) {
        test = blk->labs.find(*iter);
        if (test != blk->labs.end()) {
            ret = true;
            break;
        }
    }

    if (ret) {
        set<BasicBlock *>::iterator b;
        successors.insert(blk);
        blk->predecessors.insert(this);
    }
    return ret;
}

int BasicBlock::add_instruction(Instruction *inst) {
    const list<string> &ilabs = inst->get_labs();
    list<string>::const_iterator iter;

    for(iter = ilabs.begin(); iter != ilabs.end(); iter++) {
        string l = *iter;
        labs.insert(l);
    }

    if (inst->is_jmp()) {
        string dst = inst->get_dst();
        dst_labs.insert(dst);
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

class GraphTraverser {
    public:
        virtual int visit_node(BasicBlock *blk) { return 0; }
        virtual int visit_edge(BasicBlock *from, BasicBlock *to) { return 0; }
        GraphTraverser() {}
        virtual ~GraphTraverser() {}
};

class ControlFlowGraph {
    private:
        set<BasicBlock *> blklist;
        BasicBlock *entry;
        BasicBlock *exit;

        int dfs_traverse_r(GraphTraverser &tr, BasicBlock *start, bool backward, set<int> *visited);
    public:

        BasicBlock *get_entry() { return entry; }
        BasicBlock *get_exit() { return exit; }
        set<BasicBlock *> &get_blocks() { return blklist; }

        ControlFlowGraph() { entry = NULL; exit = NULL; }
        virtual ~ControlFlowGraph();
        int build_graph(list<BasicBlock *> &blist);

        int dfs_traverse(GraphTraverser &tr, BasicBlock *start = NULL, bool backward = false, set<int> *visited = NULL);
};

int ControlFlowGraph::dfs_traverse(GraphTraverser &tr, BasicBlock *start, bool backward, set<int> *visited) {
    if (NULL == start) {
        if (backward)
            start = exit;
        else
            start = entry;
    }
    set<int> empty;
    if (NULL == visited) {
        visited = &empty;
    }
    return dfs_traverse_r(tr, start, backward, visited);
}

int ControlFlowGraph::dfs_traverse_r(GraphTraverser &tr, BasicBlock *start, bool backward, set<int> *visited) {
    set<int>::iterator test;
    test = visited->find(start->get_id());
    int i;
    if (test == visited->end()) {
        i = tr.visit_node(start);
        visited->insert(start->get_id());
        if (i != 0)
            return i;
    } else {
        return 0;
    }

    set<BasicBlock *>::iterator iter;
    set<BasicBlock *> &adjs = start->get_successors();
    if (backward) {
        adjs = start->get_predecessors();
    }

    for(iter = adjs.begin(); iter != adjs.end(); iter++) {
        BasicBlock *blk = *iter;
        test = visited->find(blk->get_id());

        /* We haven't visit this node */
        if (test == visited->end()) {
            i = tr.visit_edge(start, blk);
            if (i != 0)
                return i;

            i = dfs_traverse_r(tr, blk, backward, visited);
            if (i != 0)
                return i;
        }
    }
    return 0;
}

ControlFlowGraph::~ControlFlowGraph() {
    set<BasicBlock *>::iterator iter;
    for(iter = blklist.begin(); iter != blklist.end(); iter++) {
        BasicBlock *blk = *iter;
        if (NULL != blk)
            delete blk;
    }
}

int ControlFlowGraph::build_graph(list<BasicBlock *> &blist) {
    list<BasicBlock *>::iterator iter;
    list<BasicBlock *>::iterator j;
    BasicBlock *entry = new BasicBlock(ENTRY_BLOCK);
    BasicBlock *exit = entry;
    this->entry = entry;

    blist.push_front(entry);
    for(iter = blist.begin(); iter != blist.end(); iter++) {
        BasicBlock *blk = *iter;
        for (j = blist.begin(); j != blist.end(); j++) {
            blk->point_to(*j);
        }
        blklist.insert(blk);

        if (exit->get_id() < blk->get_id()) {
            exit = blk;
        }
    }

    exit = exit->append_exit();
    this->exit = exit;
    blklist.insert(exit);
    return 0;
}

typedef pair<BasicBlock *, BasicBlock *> CFGEdge;

int find_back_edges(ControlFlowGraph *cfg, list<CFGEdge> &bedges) {
    set<BasicBlock *> &blks = cfg->get_blocks();
    set<BasicBlock *>::iterator blk_iter;

    for (blk_iter = blks.begin(); blk_iter != blks.end(); blk_iter++) {
        set<BasicBlock *> &sucs = (*blk_iter)->get_successors();
        set<BasicBlock *>::iterator suc;

        BasicBlock *from, *to;
        from = *blk_iter;
        for (suc = sucs.begin(); suc != sucs.end(); suc++) {
            to = *suc;
            if (to->get_id() < 0)
                continue;
            if (from->get_id() >= to->get_id()) {
                CFGEdge e = CFGEdge(from, to);
                bedges.push_back(e);
            }
        }
    }

    return 0;
}

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

ControlFlowGraph *build_cfg(InstructionList *ilist) {
    ControlFlowGraph *cfg = new ControlFlowGraph();
    list<BasicBlock *> blist;
    ilist->build_basic_blocks(blist);

    list<BasicBlock *>::iterator iter;
    list<BasicBlock *>::iterator j;

    /*
    for(iter = blist.begin(); iter != blist.end(); iter++) {
        (*iter) -> basic_info(cout);
    }
    */

    cfg->build_graph(blist);

    set<BasicBlock *> &bset = cfg->get_blocks();

    /*
    for(iter = blist.begin(); iter != blist.end(); iter++) {
        BasicBlock *blk = *iter;
        for (j = blist.begin(); j != blist.end(); j++) {
            if (blk->point_to(*j))  {
                cout << "blk" << blk->get_id() << " -> " << "blk" << (*j) ->get_id() << endl;
            }
        }
    }
    */
    return cfg;
}

class NodeCollector : public GraphTraverser {
    private:
        set<BasicBlock *> *visited;
    public:
        NodeCollector(set<BasicBlock *> &v) {
            visited = &v;
        }
        virtual ~NodeCollector() {}
        virtual int visit_node(BasicBlock *blk) {
            visited->insert(blk);
            return 0;
        }
};

class NatrualLoop {
    private:
        set<BasicBlock *> loop_set;
    public:
        NatrualLoop(ControlFlowGraph &cfg, CFGEdge &backedge);
        void info(ostream &out);

        bool is_nested_in(NatrualLoop &outter);
};

NatrualLoop::NatrualLoop(ControlFlowGraph &cfg, CFGEdge &backedge) {
    BasicBlock *from = backedge.first;
    BasicBlock *to = backedge.second;
    set<int> visited;
    visited.insert(to->get_id());

    loop_set.insert(to);

    NodeCollector c(loop_set);
    cfg.dfs_traverse(c, from, true, &visited);
}

bool NatrualLoop::is_nested_in(NatrualLoop &outter) {
    bool test;
    test = includes(outter.loop_set.begin(), outter.loop_set.end(),
            loop_set.begin(), loop_set.end());
    return test;
}

void NatrualLoop::info(ostream &out) {
    set<BasicBlock *>::iterator loop_elem;
    out << "  loop = {" ;
    for(loop_elem = loop_set.begin(); loop_elem != loop_set.end(); loop_elem++) {
        out << (*loop_elem)->name() << ", ";
    }
    out << "}" << endl;
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
    ControlFlowGraph *cfg = build_cfg(ilist);
    list<CFGEdge> bedges;

    find_back_edges(cfg, bedges);
    list<CFGEdge>::iterator iter;

    cout << "Backedges: " << endl;
    NatrualLoop *loop = NULL;
    list<NatrualLoop *> loops;
    for (iter = bedges.begin(); iter != bedges.end(); iter++) {
        BasicBlock *from, *to;
        from = iter->first;
        cout << from->name() << "->";

        to = iter->second;
        cout << to->name() << ";" << endl;

        loop = new NatrualLoop(*cfg, *iter);
        loops.push_back(loop);
    }

    list<NatrualLoop *>::iterator loop_iter;
    list<NatrualLoop *>::iterator loop_iter2;

    cout << "Loops: " << endl;
    for(loop_iter = loops.begin(); loop_iter != loops.end(); loop_iter++) {
        loop = *loop_iter;
        loop->info(cout);
        for(loop_iter2 = loops.begin(); loop_iter2 != loops.end(); loop_iter2++) {
            NatrualLoop *loop2 = *loop_iter2;
            if (loop->is_nested_in(*loop2)) {
                cout << "    Nested in ";
                loop2->info(cout);
            }
        }
    }

    /*
    set<BasicBlock *> &blks = cfg->get_blocks();
    set<BasicBlock *>::iterator blk_iter;

    for (blk_iter = blks.begin(); blk_iter != blks.end(); blk_iter++) {
        set<BasicBlock *> &sucs = (*blk_iter)->get_successors();
        set<BasicBlock *>::iterator suc;
        for (suc = sucs.begin(); suc != sucs.end(); suc++) {
            cout << (*blk_iter)->name() << " -> " << (*suc)->name() << ";" << endl;
        }
    }
    */

    return ret;
}

