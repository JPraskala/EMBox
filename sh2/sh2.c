#include "sh2.h"

void ADD(SH2* sh2, LongWord op) {
	Byte rn = (op & 0x0F00) >> 8;
	Byte rm = (op & 0x00F0) >> 4;
	sh2->R[rn] += sh2->R[rm];
	sh2->PC += 2;
}

void ADDI(SH2* sh2, LongWord op) {
	Byte rn = (op & 0x0F00) >> 8;
	Word i = (op & 0x00FF);
	sh2->R[rn] = i;
	sh2->PC += 2;
}

void ADDC(SH2* sh2, LongWord op) {
	Byte rn = (op & 0x0F00) >> 8;
	Byte rm = (op & 0x00F0) >> 4;
	Word tmp1 = sh2->R[rn] + sh2->R[rm];
	Word tmp0 = sh2->R[rn];
	Byte T = (sh2->SR & 0x00000001);
	
	sh2->R[rn] += tmp1 + T;

	if (tmp0 > tmp1 || tmp1 > sh2->R[rn]) {
		sh2->SR |= 1u; // set T bit to 1
	} else {
		sh2->SR &= ~(1u); // set T bit to 0
	}
	sh2->PC += 2;
}

void ADDV(SH2* sh2, LongWord op) {
	Byte rn = (op & 0x0F00) >> 8;
	Byte rm = (op & 0x00F0) >> 4;
	unsigned long res = sh2->R[rn] + sh2->R[rm];
	if ((res & 0xFFFFFFFF00000000) != 0) {
		sh2->SR |= 1u;
	} else {
		sh2->SR &= ~(1u);
	}
	sh2->PC += 2;
}

void AND(SH2* sh2, LongWord op) {
	Byte rn = (op & 0x0F00) >> 8;
	Byte rm = (op & 0x00F0) >> 4;
	sh2->R[rn] &= sh2->R[rm];
	sh2->PC += 2;
}

void ANDI(SH2* sh2, LongWord op) {
	LongWord i = (op & 0x000000FF);
	sh2->R[0] &= i;
	sh2->PC += 2;
}

void ANDM(SH2* sh2, LongWord op) {
	LongWord i = 0x000000FF;
	LongWord tmp = Read_Byte(sh2->GBR+sh2->R[0]);
	tmp &= (0x000000FF & i);
	Write_Byte(sh2->GBR+sh2->R[0], tmp);
	sh2->PC += 2;
}

void BF(SH2* sh2, LongWord op) {
	LongWord disp;
	Byte T = (sh2->SR & 0x00000001);
	Byte d = (op & 0x00FF);
	if ((d&0x00) == 0) {
		disp =  (0x000000FF & d);
	} else {
		disp = (0xFFFFFF00 & d);
	}
	if (T == 0) {
		sh2->PC+=(disp<<1)+4;
	} else {
		sh2->PC += 2;
	}
}

// TODO
// Gotta revisit the documentation on delayed branching
void BFS(SH2* sh2, LongWord op) {}
void BRA(SH2* sh2, LongWord op) {}
void BRAF(SH2* sh2, LongWord op) {}
void BSR(SH2* sh2, LongWord op) {}
void BSRF(SH2* sh2, LongWord op) {}


void BT(SH2* sh2, LongWord op) {
	LongWord disp;
	Byte T = (sh2->SR & 0x00000001);
	Byte d = (op & 0x00FF);
	if ((d&0x00) == 0) {
		disp =  (0x000000FF & d);
	} else {
		disp = (0xFFFFFF00 & d);
	}
	if (T == 1) {
		sh2->PC+=(disp<<1)+4;
	} else {
		sh2->PC += 2;
	}
}

// TODO
// Gotta revisit the documentation on delayed branching
void BTS(SH2* sh2, LongWord op) {}

void CLRMAC(SH2* sh2) {
	sh2->MACH=0;
	sh2->MACL=0;
	sh2->PC+=2;
}


void CLRT(SH2* sh2) {
	sh2->SR &= ~(1u); // set T bit to 0
	sh2->PC+=2;
}
