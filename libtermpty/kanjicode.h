#pragma once
#define	SIC_A	"\033$@"
#define	SIC_B	"\033$B"
#define	SOC_H	"\033(H"
#define	SOC_J	"\033(J"
#define	SOC_B	"\033(B"

#define	euc 	0
#define	sj	1
#define	jisAH	2
#define	jisAJ	3
#define	jisAB	4
#define	jisBH	5
#define	jisBJ	6
#define	jisBB	7

typedef short kanjicode;

struct code_mode {
	char	*sicode;
	char	*socode;
	char	*name;
	char	si_len;
	char	so_len;	
};

extern struct code_mode codeMode[8];

#define SIcode(c)	codeMode[c].sicode
#define SOcode(c)	codeMode[c].socode
#define SIlen(c)	codeMode[c].si_len
#define SOlen(c)	codeMode[c].so_len

#define isJIS(c)	((c) > sj)

#define KS_NOKANJI	0
#define KS_BYTE1	1
#define KS_BYTE2	2
#define KS_ESC		3
#define KS_KESC		4
#define KS_ESCDL	5
#define KS_ESCPAR	6
#define KS_KESCDL	7
#define KS_KESCPAR	8
