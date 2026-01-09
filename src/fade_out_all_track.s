.align 4
ActivateFadeOutAll:
    ldr  r1,=fadeOutAllActive
    mov  r2,#1
    strb r2,[r1]
    b    FadeOutAll // tail-call optimization

ActivateFadeInAll:
    ldr  r1,=fadeOutAllActive
    mov  r2,#0
    strb r2,[r1]
    b    FadeInAll // tail-call optimization
.pool