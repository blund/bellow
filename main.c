#include <stdio.h>
#include <stdlib.h>

#include "bellow.h"

/*
//  --- !!TODO!! ---
// - Sørg for at LATEST-variablen settes opp riktig
*/

extern ForthCtx ctx;
extern Stack    return_stack;
extern Stack    data_stack;

extern u32*    word_curr_ptr;
extern Word*   last_link;
int main() {
	// Sett opp terminal
	void* ptr0 = malloc(4096*8 * sizeof(int));

	// Initialize return stack;

	// @MERK - peker som brukes til å holde styr på hvor vi skriver Words, siden
	// de har variabel lengde (utenfor en struct), selv om at det er 'in-place'.
	ctx.HERE = ptr0;

	// @MERK - 
	declare_word("docol", 0);
	add_code_to_word(DOCOL);
	// Declare some base variables

	declare_var_STATE();
	declare_var_HERE();
	declare_var_LATEST();
	declare_var_SZ();
	declare_var_BASE();
	declare_var_DPRINT();

	declare_const_VERSION();

	declare_word("exit", 0);
	add_code_to_word(EXIT);

	declare_word("lit", 0);
	add_code_to_word(LIT);

	declare_word("drop", 0);
	add_code_to_word(DROP);

	declare_word("swap", 0);
	add_code_to_word(SWAP);

	declare_word("dup", 0);
	add_code_to_word(DUP);

	declare_word("dup2", 0);
	add_code_to_word(DUP2);

	
	declare_word("+", 0);
	add_code_to_word(ADD);
  
	declare_word("-", 0);
	add_code_to_word(SUB);

	declare_word("*", 0);
	add_code_to_word(MUL);
	
	declare_word("/", 0);
	add_code_to_word(DIV);

	declare_word("%", 0);
	add_code_to_word(MOD);


	declare_word("f+", 0);
	add_code_to_word(FADD);

	declare_word("f-", 0);
	add_code_to_word(FSUB);
  
	declare_word("f*", 0);
	add_code_to_word(FMUL);

	declare_word("f/", 0);
	add_code_to_word(FDIV);



	declare_word("!", 0);
	add_code_to_word(STORE);

	declare_word("@", 0);
	add_code_to_word(FETCH);

	
	declare_word("key", 0);
	add_code_to_word(KEY);

	declare_word("word", 0);
	add_code_to_word(WORD);

	declare_word("number", 0);
	add_code_to_word(NUMBER);

	declare_word("find", 0);
	add_code_to_word(FIND);

	declare_word(">cfa", 0);
	add_code_to_word(TCFA);

	declare_word(">dfa", 0);
	add_code_to_word(TDFA);

	declare_word("create", 0);
	add_code_to_word(CREATE);

	declare_word("[", FLAG_HIDDEN);
	add_code_to_word(LBRAC);

	declare_word("]", 0);
	add_code_to_word(RBRAC);

	declare_word("immediate", 0);
	add_code_to_word(IMMEDIATE);

	declare_word("hidden", 0);
	add_code_to_word(HIDDEN);


	declare_word(":", 0);
	add_code_to_word(DOCOL);
	add_code_to_word(WORD);
	add_code_to_word(CREATE);
	add_code_to_word(LIT);
	add_code_to_word(DOCOL);
	add_code_to_word(COMMA);
	add_code_to_word(LATEST);
	add_code_to_word(FETCH);
	add_code_to_word(HIDDEN);
	add_code_to_word(RBRAC);
	add_code_to_word(EXIT);

	declare_word(";", FLAG_IMMED);
	add_code_to_word(DOCOL);
	add_code_to_word(LIT);
	add_code_to_word(EXIT);
	add_code_to_word(COMMA);
	add_code_to_word(LATEST);
	add_code_to_word(FETCH);
	add_code_to_word(HIDDEN);
	add_code_to_word(LBRAC);
	add_code_to_word(EXIT);

	declare_word("hide", 0);
	add_code_to_word(DOCOL);
	add_code_to_word(WORD);
	add_code_to_word(FIND);
	add_code_to_word(HIDDEN);
	add_code_to_word(EXIT);
    
	declare_word("interpret", 0);
	add_code_to_word(INTERPRET);
	
	Word* quit = declare_word("quit", 0);
	add_code_to_word(DOCOL);
	add_code_to_word(RZ);
	add_code_to_word(RSPSTORE);
	add_code_to_word(INTERPRET);
	add_code_to_word(BRANCH);
	add_code_to_word((FN)-8);

	declare_word("interpret", 0);
	add_code_to_word(INTERPRET);

	declare_word("char", 0);
	add_code_to_word(CHAR);

	declare_word("execute", 0);
	add_code_to_word(EXECUTE);

	declare_word("syscall3", 0);
	add_code_to_word(SYSCALL3);

	declare_word("syscall2", 0);
	add_code_to_word(SYSCALL2);

	declare_word("syscall1", 0);
	add_code_to_word(SYSCALL1);

	declare_word("syscall0", 0);
	add_code_to_word(SYSCALL0);


	// ~~~ SETUP! ~~~
	ctx.S0 = (u32)ptr0;
	ctx.instr_ptr = &quit->data;

 call:
	ctx.eax = ctx.instr_ptr++;
	_call(*ctx.eax);
	goto call;

	// Vi kan ikke kalle NEXT() på slutten av hver
	// funksjon, siden dette ville øke størrelsen på
	// variabel-stacken vår for hvert funksjonskall (C-kallestil)
	// I stedet drar vi "NEXT" ut av funksjonene
	// og gjør en ekvivalent ting eksplisitt i en løkke.

	return 0;
}

