#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "bellow.h"

#define panic(arg, ...) printf(arg, ##__VA_ARGS__)



char    word_buffer[32];
ForthCtx ctx = {
	.esi = 0,
	.eax = 0,
	.LATEST = 0,
};


Stack return_stack = {
	.capacity = 4096, // @MERK - HELT VILKÅRLIG!
	.top     = -1,
};

Stack data_stack = {
	.capacity = 4096, // @MERK - HELT VILKÅRLIG!
	.top     = -1,
};



inline void stack_push(Stack* stack, u32 guy) {
  if (stack->top > stack->capacity-1) panic("tried to push onto stack but surpassed its end!\n");
  // Kanskje stack->top - 1...
  stack->top++;
  stack->data[stack->top] = guy;
}

inline u32 stack_pop(Stack* stack) {
  if ((stack->top) < -1) panic("Tried to pop off the stack but surpassed its bottom!\n"
			      "Stack: [%d, %d, %d, %d, ...]);\n",
			      stack->data[0],
			      stack->data[1],
			      stack->data[2],
			      stack->data[3]);
  // Kanskje stack->top + 1
  u32 guy = stack->data[stack->top];
  stack->top--;
  return guy;
}


inline Word* declare_word(char* name, int flags) {
	Word* new_word = (Word*)ctx.HERE;
	*new_word = (Word){
		.link = ctx.LATEST,
		.flags_and_len = flags + strlen(name),
	};
	strcpy(new_word->name, name);

	ctx.HERE   = (u32*)&new_word->data;
	ctx.LATEST = new_word;

	return new_word;
}

inline void add_code_to_word(FN ptr) {
	*ctx.HERE = (u32)ptr;
	ctx.HERE++; /* Adds 4 to the index, i.e. 1 word */
}

// Macro to define variables.
// Works by creating a Word that pushes the address of a variable to the stack.
#define make_declare_var(name, label, addr, value) 	\
  void label() {					\
    stack_push(&data_stack, (u32)addr); 		\
							\
  }							\
  void declare_var_##label() {				\
    declare_word(#label, 0);				\
    add_code_to_word(label);				\
    /**addr = value;*/					\
  }							\

make_declare_var("STATE",  STATE,  &ctx.STATE,  0);
make_declare_var("HERE",   HERE,   &ctx.HERE,   0);
make_declare_var("LATEST", LATEST, &ctx.LATEST, 0);
make_declare_var("S0",     SZ,     &ctx.S0,     0);
make_declare_var("BASE",   BASE,   &ctx.BASE,  10);


#define make_declare_const(name, label, value)	\
  void label() {				\
    puts("RZ!");				\
    stack_push(&data_stack, value);		\
						\
  }						\
  void declare_const_##label() {		\
    declare_word(#label, 0);			\
    add_code_to_word(label);			\
  } 						\

#include <asm/unistd.h>
make_declare_const("VERSION", VERSION, 1);
make_declare_const("R0", RZ, (u32)return_stack.data);
//make_declare_const("DOCOL", __DOCOL, DOCOL);

make_declare_const("F_IMMED", __F_IMMED, FLAG_IMMED);
make_declare_const("F_HIDDEN", __F_HIDDEN, FLAG_HIDDEN);
make_declare_const("F_LENMASK", __F_LENMASK, FLAG_LENMASK);

make_declare_const("SYS_EXIT", SYS_EXIT, __NR_exit);
make_declare_const("SYS_OPEN", SYS_OPEN, __NR_open);
make_declare_const("SYS_CLOSE", SYS_CLOSE, __NR_close);
make_declare_const("SYS_READ", SYS_READ, __NR_read);
make_declare_const("SYS_WRITE", SYS_WRITE, __NR_write);
make_declare_const("SYS_CREAT", SYS_CREAT, __NR_creat);
make_declare_const("SYS_BRK", SYS_BRK, __NR_brk);

make_declare_const("O_RDONLY", __O_RDONLY, 0);
make_declare_const("O_WRONLY", __O_WRONLY, 1);
make_declare_const("O_RDWR", __O_RDWR, 0x2);
make_declare_const("O_CREAT", __O_CREAT, 0x0100);
make_declare_const("O_EXCL", __O_EXCL, 0x0200);
make_declare_const("O_TRUNC", __O_TRUNC, 0x01000);
make_declare_const("O_APPEND", __O_APPEND, 0x02000);
make_declare_const("O_NONBLOCK", __O_NONBLOCK, 0x04000);

void push_return_stack(u32 val /* val to be written to the stack */) {
	stack_push(&return_stack, val);
}

void pop_return_stack(u32* ptr /* where to put the popped value */) {
	*(u32*)ptr = stack_pop(&return_stack);
}

/* DEFINISJONER AV FORTH ORD! */



void DOCOL() {
	push_return_stack((int)ctx.esi);
	ctx.esi = ctx.eax + 1; // @MERk - +1 begrunn av peker-aritmetikk
	
}

void EXIT() {
	pop_return_stack(ctx.esi);
	// @TODO - denne skal vanlig vis kalle next...
}


void LIT() {
	ctx.eax = _deref(ctx.esi); 	// Samme som lodsl
	ctx.esi++;			//

	stack_push(&data_stack, (u32)ctx.eax);
}


void DROP() {
	stack_pop(&data_stack);
	
}

void SWAP() {
	u32 a = stack_pop(&data_stack);
	u32 b = stack_pop(&data_stack);
	stack_push(&data_stack, a);
	stack_push(&data_stack, b);
	
	
}

void DUP() {
	u32 a = stack_pop(&data_stack);
	stack_push(&data_stack, a);
	stack_push(&data_stack, a);
	
}


void STORE() {
	u32* addr  = (u32*)stack_pop(&data_stack);
	u32  value = stack_pop(&data_stack);
	*addr = value;	
	
}

void FETCH() {
	u32* addr  = (u32*)stack_pop(&data_stack);
	u32 value = *addr;
	stack_push(&data_stack, value);
	
}


void TOR() {
	u32* addr  = (u32*)stack_pop(&return_stack);
	u32  value = stack_pop(&return_stack);
	*addr = value;	
	
}

void FROMR() {
	u32* addr  = (u32*)stack_pop(&return_stack);
	u32 value = *addr;
	stack_push(&return_stack, value);
	
}

void RSPFETCH() {
	stack_push(&data_stack, return_stack.top);
	
}

void RSPSTORE() {
	return_stack.top = stack_pop(&data_stack);
	
}

void RSPDROP() {
	stack_pop(&return_stack);
	
}

void DSPFETCH() {
	stack_push(&data_stack, data_stack.top);
	
}

void DSPSTORE() {
	data_stack.top = stack_pop(&data_stack);
	
}

char _KEY() {
	return getchar();
}

void KEY() {
  char c = _KEY();
  stack_push(&data_stack, c);
  
}


void EMIT() {
	char to_write = stack_pop(&data_stack);
	printf("%c", to_write); // Ikke gjør det her...
	
}


void WORD() {
	char*   pointer = word_buffer;
	memset(word_buffer, 0, 32);
	
	char c;
 look_for_start:
	c  = _KEY();
	// Jonesforth kan garantere at char-en er i eax. Vi kan ikke det, så vi må hente den fra stacken.

	switch(c) {
	case('\\'): goto handle_comment;
	case(' '):  goto look_for_start;
	case('\n'):  goto look_for_start;
	}

	*pointer++ = c;

 read_word:
	// Hvis vi ikke har en kommentar eller en blank, har vi et ord!

	c = _KEY();
	if (c != ' ' && c != '\n') {
	  *pointer++ = c;
	  goto read_word;
	}

	int len = pointer - word_buffer;
	u32 addr = (u32)word_buffer;

	printf(" -- [WORD: read string '%s' of len %d]\n", word_buffer, len);

	stack_push(&data_stack, len);
	stack_push(&data_stack, addr);
	return;
	
	// Will eat letters of a comment until we have a newline
 handle_comment: {
	  c = _KEY();
	  if (c == '\n') goto look_for_start;
	  goto handle_comment;
	}
}

void NUMBER() {
  char* addr = (char*)stack_pop(&data_stack);
  int   len  = stack_pop(&data_stack);

  char* endptr = (char*)1;

  int number = strtol(addr, &endptr, 10);
  int chars_remaining = len - (endptr - addr); // endptr - addr is offset of first invalid character, and
  // we want difference between the length of the string and this offset to know the amount of invalid chars

  printf(" -- [NUMBER: result: %d, %d chars rem]\n", number, chars_remaining);

  stack_push(&data_stack, number);
  stack_push(&data_stack, chars_remaining);
}


void FIND() {
  char* name_addr = (char*)stack_pop(&data_stack);
  int   name_len  = stack_pop(&data_stack);

  Word* latest = ctx.LATEST;

 try_compare:
  // Hvis vi har NULL har vi kommet til starten av lenken, og ordet er dermed ikke definert.
  // Putt 0 på stacken og retruner
  if (NULL == latest) {
    printf(" -- [FIND: '%s' not a word, assume number]\n", name_addr);
    stack_push(&data_stack, name_len);
    stack_push(&data_stack, (u32)name_addr);
    stack_push(&data_stack, 0);
    return;
  }

  u8 len = latest->flags_and_len & (FLAG_HIDDEN | FLAG_LENMASK);


  // If they are not the same, jump to the next link down the chain and try again
  if (name_len != len) {
  try_next:
    latest = latest->link;
    goto try_compare;
  }

  int is_equal = strncmp(name_addr, latest->name, len);

  if (is_equal != 0) goto try_next;
  
  // Strengene er like, så vi fant vår peker!
  // Push addresse på stack og vær lykkelig
  stack_push(&data_stack, (u32)latest->data);
  printf(" -- [FIND: '%s' a word, data addr %p]\n", name_addr, latest->data);
  return;

}

void TCFA() {
  Word* ptr = (Word*)stack_pop(&data_stack);
  stack_push(&data_stack, (u32)&ptr->data);
  
}

void TDFA() {
  Word* ptr = (Word*)stack_pop(&data_stack);
  stack_push(&data_stack, (u32) (&ptr->data+1));
  
}


void CREATE() {
  char* name_addr = (char*)stack_pop(&data_stack);
  int   name_len  = stack_pop(&data_stack);
    
  Word* ptr = (Word*)stack_pop(&data_stack);
  declare_word(name_addr, 0);
  
}

void COMMA() {
  // Add a code word to the HERE pointer (used to add code to a definition)
  u32 word = stack_pop(&data_stack);
  *ctx.HERE++ = word;
  
}

void LBRAC() {
  ctx.STATE = STATE_IMMEDIATE;
  
}

void RBRAC() {
  ctx.STATE = STATE_COMPILE;
  
}

void IMMEDIATE() {
  Word* latest = ctx.LATEST;
  latest->flags_and_len ^= FLAG_IMMED;
  
}

void HIDDEN() {
  Word* entry = (Word*)stack_pop(&data_stack);
  entry->flags_and_len ^= FLAG_HIDDEN;
  
}

void TICK() {
  // @FIKS - lodsl, skriv om med WORD, FIND, >CFA i Forth
  u32 next = *ctx.esi;
  ctx.eax = (u32*)next;
  ctx.esi += 1;

  stack_push(&data_stack, next);
  
}

void BRANCH() {
  puts(" -- [BRANCH]\n");
  u32 esi = (u32)ctx.esi;
  esi += *ctx.esi;
  ctx.esi = (u32*)esi;
  
}

void BRANCH0() {
  u32 top = stack_pop(&data_stack);
  if (top == 0) {
    BRANCH();
  } else {
    // lodsl
    ctx.eax = (u32*)*ctx.esi;
    ctx.esi++;
  }
  
}

void LITSTRING() {

}


void TELL() {
}

// @RYDD - Kanskje gjør som Jonesforth gjør - trekk ut _NUMBER, _WORD, etc som funsjoner som tar et parameter.
//         Det gjør denne koden litt tydeligere
void INTERPRET() {
  puts(" -- [INTERPRET]");
  int interpret_is_lit = 0;

  WORD();
  FIND();
  u32 addr = stack_pop(&data_stack);

  // Sjekk om addr er 0. I så fall, gå ut i fra at det er et tall :)
  if (addr == 0) goto number;

  // Sjekk om det er immediate
  Word* word = (Word*)addr;
  u8 flags_and_len  = word->flags_and_len;
  u32* codeword_ptr = word->data; // Ekvivalent til TCFA

  ctx.eax = codeword_ptr;
  
  if (flags_and_len & FLAG_IMMED) goto compile_or_exec;
  

 number: // 1
  interpret_is_lit = 1;

  // Sett tallet tilbake på stacken
  NUMBER();
  u32 remaining = stack_pop(&data_stack);
  u32 number    = stack_pop(&data_stack);
  if (remaining != 0) {
    printf(" -- [INTERPRET: parse failed, disregarding input]\n");
    return;
  }
  stack_push(&data_stack, (u32)LIT);
  stack_push(&data_stack, remaining);
  return;

 compile_or_exec:
  // Sjekk om vi er i immediate eller compile mode
  if (ctx.STATE == STATE_IMMEDIATE) goto exec;

  // Case: compile mode!
  stack_push(&data_stack, addr);
  COMMA();
  if (interpret_is_lit == 0) 
  
 exec: // 4
  if (interpret_is_lit == 0) goto exec_literal;
  ((FN)ctx.eax)();
  
 exec_literal: // 5
  // HER SKAL TALLET FRA NUM SETTES PÅ STACKEN HVIS DET VAR ET TALL...

 error: // 6

 print_error: // 7

  return;
}



void DIE() {
  puts("DIE!\n");
  return;
}
