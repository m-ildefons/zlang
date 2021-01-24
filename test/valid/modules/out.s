    .file      "module1.x"
    .text
    .global    zlang_mod1
    .type      zlang_mod1, @function
zlang_mod1:
    pushq  %rbp
    movq   %rsp, %rbp
    movq   $7, %rax
    movq   %rbp, %rsp
    popq   %rbp
    ret
    movq   $0, %rax
    movq   %rbp, %rsp
    popq   %rbp
    ret
    .size   zlang_mod1, .-zlang_mod1
    .ident  "ZLang Version 0.1"
