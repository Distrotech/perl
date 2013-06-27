/* -*- buffer-read-only: t -*-
   !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!!
   This file is built by regen_perly.pl from perly.y.
   Any changes made here will be lost!
 */

#ifdef PERL_CORE
/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     GRAMPROG = 258,
     GRAMEXPR = 259,
     GRAMBLOCK = 260,
     GRAMBARESTMT = 261,
     GRAMFULLSTMT = 262,
     GRAMSTMTSEQ = 263,
     WORD = 264,
     METHOD = 265,
     FUNCMETH = 266,
     THING = 267,
     PMFUNC = 268,
     PRIVATEREF = 269,
     QWLIST = 270,
     FUNC0OP = 271,
     FUNC0SUB = 272,
     UNIOPSUB = 273,
     LSTOPSUB = 274,
     PLUGEXPR = 275,
     PLUGSTMT = 276,
     LABEL = 277,
     FORMAT = 278,
     SUB = 279,
     ANONSUB = 280,
     PACKAGE = 281,
     USE = 282,
     WHILE = 283,
     UNTIL = 284,
     IF = 285,
     UNLESS = 286,
     ELSE = 287,
     ELSIF = 288,
     CONTINUE = 289,
     FOR = 290,
     GIVEN = 291,
     WHEN = 292,
     DEFAULT = 293,
     LOOPEX = 294,
     DOTDOT = 295,
     YADAYADA = 296,
     FUNC0 = 297,
     FUNC1 = 298,
     FUNC = 299,
     UNIOP = 300,
     LSTOP = 301,
     RELOP = 302,
     EQOP = 303,
     MULOP = 304,
     ADDOP = 305,
     DOLSHARP = 306,
     DO = 307,
     HASHBRACK = 308,
     NOAMP = 309,
     LOCAL = 310,
     MY = 311,
     REQUIRE = 312,
     COLONATTR = 313,
     FORMLBRACK = 314,
     FORMRBRACK = 315,
     PREC_LOW = 316,
     DOROP = 317,
     OROP = 318,
     ANDOP = 319,
     NOTOP = 320,
     ASSIGNOP = 321,
     DORDOR = 322,
     OROR = 323,
     ANDAND = 324,
     BITOROP = 325,
     BITANDOP = 326,
     SHIFTOP = 327,
     MATCHOP = 328,
     REFGEN = 329,
     UMINUS = 330,
     POWOP = 331,
     POSTJOIN = 332,
     POSTDEC = 333,
     POSTINC = 334,
     PREDEC = 335,
     PREINC = 336,
     ARROW = 337,
     PEG = 338
   };
#endif

/* Tokens.  */
#define GRAMPROG 258
#define GRAMEXPR 259
#define GRAMBLOCK 260
#define GRAMBARESTMT 261
#define GRAMFULLSTMT 262
#define GRAMSTMTSEQ 263
#define WORD 264
#define METHOD 265
#define FUNCMETH 266
#define THING 267
#define PMFUNC 268
#define PRIVATEREF 269
#define QWLIST 270
#define FUNC0OP 271
#define FUNC0SUB 272
#define UNIOPSUB 273
#define LSTOPSUB 274
#define PLUGEXPR 275
#define PLUGSTMT 276
#define LABEL 277
#define FORMAT 278
#define SUB 279
#define ANONSUB 280
#define PACKAGE 281
#define USE 282
#define WHILE 283
#define UNTIL 284
#define IF 285
#define UNLESS 286
#define ELSE 287
#define ELSIF 288
#define CONTINUE 289
#define FOR 290
#define GIVEN 291
#define WHEN 292
#define DEFAULT 293
#define LOOPEX 294
#define DOTDOT 295
#define YADAYADA 296
#define FUNC0 297
#define FUNC1 298
#define FUNC 299
#define UNIOP 300
#define LSTOP 301
#define RELOP 302
#define EQOP 303
#define MULOP 304
#define ADDOP 305
#define DOLSHARP 306
#define DO 307
#define HASHBRACK 308
#define NOAMP 309
#define LOCAL 310
#define MY 311
#define REQUIRE 312
#define COLONATTR 313
#define FORMLBRACK 314
#define FORMRBRACK 315
#define PREC_LOW 316
#define DOROP 317
#define OROP 318
#define ANDOP 319
#define NOTOP 320
#define ASSIGNOP 321
#define DORDOR 322
#define OROR 323
#define ANDAND 324
#define BITOROP 325
#define BITANDOP 326
#define SHIFTOP 327
#define MATCHOP 328
#define REFGEN 329
#define UMINUS 330
#define POWOP 331
#define POSTJOIN 332
#define POSTDEC 333
#define POSTINC 334
#define PREDEC 335
#define PREINC 336
#define ARROW 337
#define PEG 338



#ifdef PERL_IN_TOKE_C
static bool
S_is_opval_token(int type) {
    switch (type) {
    case FUNC0OP:
    case FUNC0SUB:
    case FUNCMETH:
    case LSTOPSUB:
    case METHOD:
    case PLUGEXPR:
    case PLUGSTMT:
    case PMFUNC:
    case PRIVATEREF:
    case QWLIST:
    case THING:
    case UNIOPSUB:
    case WORD:
	return 1;
    }
    return 0;
}
#endif /* PERL_IN_TOKE_C */
#endif /* PERL_CORE */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */

    I32	ival; /* __DEFAULT__ (marker for regen_perly.pl;
				must always be 1st union member) */
    char *pval;
    OP *opval;
    GV *gvval;
#ifdef PERL_IN_MADLY_C
    TOKEN* p_tkval;
    TOKEN* i_tkval;
#else
    char *p_tkval;
    I32	i_tkval;
#endif
#ifdef PERL_MAD
    TOKEN* tkval;
#endif



/* Line 2068 of yacc.c  */
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif





/* Generated from:
 * 4470a58f5636cb74189a9361e6c9e14550f644963dc646ae95d04dae62315c14 perly.y
 * 5c9d2a0262457fe9b70073fc8ad6c188f812f38ad57712b7e2f53daa01b297cc regen_perly.pl
 * ex: set ro: */
