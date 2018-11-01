/*
 keyboard.h

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#ifndef __keyboard_h__

short int
keyboard_query();

void
keyboard_read(
  unsigned char* const                o_scan,
  unsigned char* const                o_ascii);

#pragma aux keyboard_read= \
    "mov esi, eax" \
    "xor ah, ah" \
    "int 16h" \
    "mov [byte ptr esi], ah"\
    "mov [byte ptr ecx], al"\
    parm [eax][ecx]\
    modify [ esi ];

#pragma aux keyboard_query = \
    "mov ah, 01h" \
    "int 16h" \
    "jz qnokey"\
    "mov ax, 0xffff"\
    "jmp qdone"\
    "qnokey:"\
    "xor ax, ax"\
    "qdone:"\
    value [ax];

#define __keyboard_h__
#endif
