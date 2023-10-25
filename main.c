#include <stdio.h>
#include <stdlib.h>

#include "bellow.h"
/*
  void dummy0() {
  puts("A");
  NEXT();
  }

  void dummy1() {
  puts("B");
  NEXT();
  }

  void dummy2() {
  puts("C");
  }
*/


/*
//  --- !!TODO!! ---
// - Sørg for at LATEST-variablen settes opp riktig
*/

extern ForthCtx ctx;
extern Stack    return_stack;
extern Stack    data_stack;

extern u32*    word_curr_ptr;
extern Word*   last_link;

void say_the_thing() {
    static int a = 0;
    a ++;
    printf("%d\n", a);
}

int main() {
    // Sett opp terminal
    void* ptr0 = malloc(4096*8 * sizeof(int));

    // Initialize return stack;

    // @MERK - peker som brukes til å holde styr på hvor vi skriver Words, siden
    // de har variabel lengde (utenfor en struct), selv om at det er 'in-place'.
    ctx.HERE = ptr0;

    // @MERK - 
    declare_word("DOCOL", 0);
    add_code_to_word(DOCOL);
    // Declare some base variables

    declare_var_STATE();
    declare_var_LATEST();
    declare_var_SZ();
    declare_var_BASE();
    declare_const_VERSION();

    declare_word("EXIT", 0);
    add_code_to_word(EXIT);

    declare_word("DIE", 0);
    add_code_to_word(DIE);

    declare_word("LIT", 0);
    add_code_to_word(LIT);

    declare_word("DROP", 0);
    add_code_to_word(DROP);

    declare_word("SWAP", 0);
    add_code_to_word(SWAP);

    declare_word("DUP", 0);
    add_code_to_word(DUP);

  
    declare_word("!", 0);
    add_code_to_word(STORE);

    declare_word("@", 0);
    add_code_to_word(FETCH);

	
    declare_word("KEY", 0);
    add_code_to_word(KEY);

    declare_word("WORD", 0);
    add_code_to_word(WORD);

    declare_word("NUMBER", 0);
    add_code_to_word(NUMBER);

    declare_word("FIND", 0);
    add_code_to_word(FIND);

    declare_word(">CFA", 0);
    add_code_to_word(TCFA);

    declare_word(">DFA", 0);
    add_code_to_word(TDFA);

    declare_word("CREATE", 0);
    add_code_to_word(CREATE);

    declare_word("[", FLAG_HIDDEN);
    add_code_to_word(LBRAC);

    declare_word("]", 0);
    add_code_to_word(RBRAC);

    declare_word("IMMEDIATE", 0);
    add_code_to_word(IMMEDIATE);

    declare_word("HIDDEN", 0);
    add_code_to_word(HIDDEN);


    declare_word(":", 0);
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

    declare_word(";", 0);
    add_code_to_word(LIT);
    add_code_to_word(EXIT);
    add_code_to_word(COMMA);
    add_code_to_word(LATEST);
    add_code_to_word(FETCH);
    add_code_to_word(HIDDEN);
    add_code_to_word(LBRAC);
    add_code_to_word(EXIT);

    declare_word("HIDE", 0);
    add_code_to_word(WORD);
    add_code_to_word(FIND);
    add_code_to_word(HIDDEN);
    add_code_to_word(EXIT);
    
    declare_word("INTERPRET", 0);
    add_code_to_word(INTERPRET);
	
    Word* quit = declare_word("QUIT", 0);
    add_code_to_word(RZ);
    add_code_to_word(RSPSTORE);
    add_code_to_word(INTERPRET);
    add_code_to_word(BRANCH);
    add_code_to_word((FN)-8);

    // ~~~ SETUP! ~~~
    ctx.S0 = (u32)ptr0;
    ctx.esi = &quit->data;

 call:
    ctx.eax = _deref(ctx.esi++);
    _call(ctx.eax);
    goto call;

    // Vi kan ikke kalle NEXT() på slutten av hver
    // funksjon, siden dette ville øke størrelsen på
    // variabel-stacken vår for hvert funksjonskall (C-kallestil)
    // I stedet drar vi "NEXT" ut av funksjonene
    // og gjør en ekvivalent ting eksplisitt i en løkke.

	
	

	
    return 0;
}

