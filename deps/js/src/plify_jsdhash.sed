/ * Double hashing implementation./a\
\ * GENERATED BY js/src/plify_jsdhash.sed -- DO NOT EDIT!!!
/ * Double hashing, a la Knuth 6./a\
\ * GENERATED BY js/src/plify_jsdhash.sed -- DO NOT EDIT!!!
s/jsdhash_h___/pldhash_h___/
s/jsdhash\.bigdump/pldhash.bigdump/
s/jstypes\.h/nscore.h/
s/jsbit\.h/prbit.h/
s/jsdhash\.h/pldhash.h/
s/jsdhash\.c/pldhash.c/
s/jsdhash:/pldhash:/
s/jsutil\.h/nsDebug.h/
s/JS_DHASH/PL_DHASH/g
s/JS_DHash/PL_DHash/g
s/JSDHash/PLDHash/g
s/JSHash/PLHash/g
s/uint8  /PRUint8/g
s/uint16  /PRUint16/g
s/uint32  /PRUint32/g
s/\([^U]\)int8  /\1PRInt8/g
s/\([^U]\)int16  /\1PRInt16/g
s/\([^U]\)int32  /\1PRInt32/g
s/uint8/PRUint8/g
s/uint16/PRUint16/g
s/uint32/PRUint32/g
s/\([^U]\)int8/\1PRInt8/g
s/\([^U]\)int16/\1PRInt16/g
s/\([^U]\)int32/\1PRInt32/g
s/JSBool/PRBool/g
s/extern JS_PUBLIC_API(\([^()]*\))/NS_COM_GLUE \1/
s/JS_PUBLIC_API(\([^()]*\))/\1/
s/JS_NewDHashTable/PL_NewDHashTable/
s/JS_ASSERT(0)/NS_NOTREACHED("0")/
s/\( *\)JS_ASSERT(\(.*\));/\1NS_ASSERTION(\2,\n\1             "\2");/
s/JSDHASH_ONELINE_ASSERT(\(.*\));/NS_ASSERTION(\1, "\1");/
s/JS_UNLIKELY/NS_UNLIKELY/g
s/JS_LIKELY/NS_LIKELY/g
s/JS_/PR_/g
s/fprintf(stderr,/printf_stderr(/