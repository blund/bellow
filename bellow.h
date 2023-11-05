#include <stdint.h>
#include <stdbool.h>

#define f32 float
#define u32 uint32_t
#define u8  uint8_t


// Nyttemakroer for kall av Forth kode.
// Siden vi raw-dogger en del pekere, er disse greie å ha så vi ikke også må rote med syntax

// _call will cast a pointer to a function pointer and call it
#define _call(fn) ((FN)(fn))() 
#define _deref(ptr) *(void**)(ptr)


// Used for calling Forth functions
typedef void (*FN) (void); 

typedef struct Word {
    struct Word* link;
    u8   flags_and_len;
    char name[31];
    u32* data; // @MERK - instruksjonene for ordet starter her og fortsetter utenfor structen.
} Word;



// The Forth execution context.
// All Forth functions should refer to this common context
typedef struct ForthCtx {
    u32* eax;
    u32* instr_ptr; 	// 'instruction' pointer

    u32 STATE;
    u32* HERE;
    Word* LATEST;
    u32 S0;
    u32 BASE;

    u32 R0;

    bool debug_prints;
} ForthCtx;

enum Flags {
    FLAG_IMMED   = 0x80,
    FLAG_HIDDEN  = 0x20,
    FLAG_LENMASK = 0x1f,
};

enum STATE {
    STATE_IMMEDIATE = 0x0,
    STATE_COMPILE   = 0x1,
};


Word* declare_word(char* name, int flags);
void add_code_to_word(FN ptr);

#define make_header_const(label) void label()
make_header_const(RZ);

#define make_header_var(label) void label()
make_header_var(STATE);
make_header_var(HERE);
make_header_var(LATEST);
make_header_var(S0);
make_header_var(BASE);
make_header_var(DPRINT);

// A simple, very type unsafe stack.
typedef struct Stack {
    // @Note - stack grows down. Boundary is the highest legal index
    int       data[4096];
    const int capacity;
    int       top;
} Stack;


void stack_push(Stack* stack, u32 guy);
u32  stack_pop(Stack* stack);

void DOCOL();
void EXIT();

void LIT();

void DROP();
void SWAP();
void DUP();
void DUP2();

void ADD();
void SUB();
void MUL();
void DIV();
void MOD();

void FADD();
void FSUB();
void FMUL();
void FDIV();
void FMOD();

void ZEROEQ();
void EQ();

void STORE();
void FETCH();
void TOR();
void RSPSTORE();
void RSPFETCH();
void RSPDROP();
void DSPSTORE();
void DSPFETCH();

void KEY();
void WORD();
void NUMBER();
void FIND();
void TCFA();
void TDFA();
void CREATE();
void COMMA();
void LBRAC();
void RBRAC();
void IMMEDIATE();
void HIDDEN();
void TICK();

void BRANCH();
void BRANCH0();

void LITSTRING();
void TELL();
void EMIT();

void INTERPRET();
void DIE();

void CHAR();
void EXECUTE();
void SYSCALL3();
void SYSCALL2();
void SYSCALL1();
void SYSCALL0();

void declare_var_STATE();
void declare_var_HERE();
void declare_var_LATEST();
void declare_var_SZ();
void declare_var_BASE();

void declare_const_VERSION();

