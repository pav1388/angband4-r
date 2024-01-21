/**
 * \file z-util.h
 * \brief Low-level string handling and other utilities.
 *
 * Copyright (c) 1997-2005 Ben Harrison, Robert Ruehlmann.
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#ifndef INCLUDED_Z_UTIL_H
#define INCLUDED_Z_UTIL_H

#include "h-basic.h"


struct my_rational { unsigned int n, d; }; /* numerator and denominator */


/**
 * ------------------------------------------------------------------------
 * Available variables
 * ------------------------------------------------------------------------ */


/**
 * The name of the program.
 */
extern char *argv0;


/**
 * Aux functions
 */
extern size_t (*text_mbcs_hook)(wchar_t *dest, const char *src, int n);
extern int (*text_wctomb_hook)(char *s, wchar_t wchar);
extern int (*text_wcsz_hook)(void);
extern int (*text_iswprint_hook)(wint_t wc);
extern wchar_t *(*text_wcschr_hook)(const wchar_t *wcs, wchar_t wc);
extern size_t (*text_wcslen_hook)(const wchar_t *s);
extern void (*plog_aux)(const char *);
extern void (*quit_aux)(const char *);


/**
 * ------------------------------------------------------------------------
 * Available Functions
 * ------------------------------------------------------------------------ */


/**
 * Return "s" (or not) depending on whether n is singular.
 */
#define PLURAL(n)				((n) == 1 ? "" : "s")


/**
 * Для русского языка
 */
#define PLURAL_RU(n, a, b, c)	((((n) % 10) == 1) && (((n) % 100) != 11) ? (a) : (((n) % 10) > 1) && (((n) % 10) < 5) && (((n) % 100) != 12) && (((n) % 100) != 13) && (((n) % 100) != 14) ? (b) : (c))

#define CAUSE_OF_DEATH(a)	(streq((a), "starvation") ? "голодания" : streq((a), "poison") ? "отравления" : streq((a), "a fatal wound") ? "смертельного ранения" : streq((a), "Ripe Old Age") ? "Почтенного Возраста" : (a))

#define C_IMEN		0
#define C_RODIT		1
#define C_DAT		2
#define C_VINIT		3
#define C_TVORIT	4
#define C_PREDL		5
#define W_PART		0
#define W_GEN		1
#define W_DECL		2
#define W_ALTER		3
#define W_PART_NOUN	0
#define W_PART_ADJ	1
#define W_GEN_NEUT	0
#define W_GEN_MALE	1
#define W_GEN_FEM	2

#define	E_		0
#define	E_A		1
#define	E_AM	2
#define	E_AMI	3
#define	E_AH	4
#define	E_AYA	5
#define	E_E		6
#define	E_EV	7
#define	E_EGO	8
#define	E_EE	9
#define	E_EJ	10
#define	E_EM	11
#define	E_YO	12
#define	E_YOM	13
#define	E_EMU	14
#define	E_ENEM	15
#define	E_I		16
#define	E_IE	17
#define	E_IEM	18
#define	E_II	19
#define	E_IJ	20
#define	E_IM	21
#define	E_IMI	22
#define	E_IH	23
#define	E_IYU	24
#define	E_IYA	25
#define	E_IYAMI	26
#define	E_IYAH	27
#define	E_J		28
#define	E_O		29
#define	E_OV	30
#define	E_OGO	31
#define	E_OE	32
#define	E_OJ	33
#define	E_OM	34
#define	E_OMU	35
#define	E_U		36
#define	E_UYU	37
#define	E_Y		38
#define	E_YE	39
#define	E_YJ	40
#define	E_YM	41
#define	E_YMI	42
#define	E_YH	43
#define	E_b		44
#define	E_bE	45
#define	E_bYO	46
#define	E_bEM	47
#define	E_bYOM	48
#define	E_bYU	49
#define	E_bYA	50
#define	E_bYAM	51
#define	E_bYAMI	52
#define	E_bYAH	53
#define	E_YU	54
#define	E_YUYU	55
#define	E_YA	56
#define	E_YAM	57
#define	E_YAMI	58
#define	E_YAH	59
#define	E_YAYA	60


/**
 * Return the verb form matching the given count
 */
#define VERB_AGREEMENT(count, singular, plural)    (((count) == 1) ? (singular) : (plural))


/**
 * Count the number of characters in a UTF-8 encoded string
 */
size_t utf8_strlen(const char *s);

/**
 * Clip a null-terminated UTF-8 string 's' to 'n' unicode characters.
 * e.g. utf8_clipto("example", 4) will clip after 'm', resulting in 'exam'.
 */
void utf8_clipto(char *s, size_t n);

/**
 * Advance a pointer to a UTF-8 buffer by a given number of Unicode code points.
 */
char *utf8_fskip(char *s, size_t n, char *lim);

/**
 * Decrement a pointer to a UTF-8 buffer by a given number of Unicode code
 * points.
 */
char *utf8_rskip(char *s, size_t n, char *lim);

/**
 * Convert a sequence of UTF-32 values, in the native byte order, to UTF-8.
 */
size_t utf32_to_utf8(char *out, size_t n_out, const uint32_t *in, size_t n_in,
	size_t *pn_cnvt);

/**
 * Return whether a given UTF-32 value corresponds to a printable character.
 */
bool utf32_isprint(uint32_t v);

/**
 * Case insensitive comparison between two strings
 */
extern int my_stricmp(const char *s1, const char *s2);

/**
 * Case insensitive comparison between two strings, up to n characters long.
 */
extern int my_strnicmp(const char *a, const char *b, int n);

/**
 * Case-insensitive strstr
 */
extern char *my_stristr(const char *string, const char *pattern);

/**
 * Copy up to 'bufsize'-1 characters from 'src' to 'buf' and NULL-terminate
 * the result.  The 'buf' and 'src' strings may not overlap.
 *
 * Returns: strlen(src).  This makes checking for truncation
 * easy.  Example:
 *   if (my_strcpy(buf, src, sizeof(buf)) >= sizeof(buf)) ...;
 *
 * This function should be equivalent to the strlcpy() function in BSD.
 */
extern size_t my_strcpy(char *buf, const char *src, size_t bufsize);

/**
 * Try to append a string to an existing NULL-terminated string, never writing
 * more characters into the buffer than indicated by 'bufsize', and
 * NULL-terminating the buffer.  The 'buf' and 'src' strings may not overlap.
 *
 * my_strcat() returns strlen(buf) + strlen(src).  This makes checking for
 * truncation easy.  Example:
 *   if (my_strcat(buf, src, sizeof(buf)) >= sizeof(buf)) ...;
 *
 * This function should be equivalent to the strlcat() function in BSD.
 */
extern size_t my_strcat(char *buf, const char *src, size_t bufsize);

/**
 * Capitalise string 'buf'
 */
void my_strcap(char *buf);

/**
 * Test equality, prefix, suffix
 */
extern bool streq(const char *s, const char *t);
extern bool prefix(const char *s, const char *t);
extern bool prefix_i(const char *s, const char *t);
extern bool suffix(const char *s, const char *t);
extern bool suffix_i(const char *s, const char *t);

#define streq(s, t)		(!strcmp(s, t))

/**
 * Skip occurrences of a characters
 */
extern void strskip(char *s, const char c, const char e);
extern void strescape(char *s, const char c);

/**
 * Get the integer value of a hex string
 */
extern int hex_str_to_int(const char *s);

/**
 * Change escaped characters into their literal representation
 */
extern void strunescape(char *s);

/**
 * Determines if a string is "empty"
 */
bool contains_only_spaces(const char* s);

/**
 * Check if a char is a vowel
 */
bool is_a_vowel(int ch);


/**
 * Allow override of the multi-byte to wide char conversion
 */
size_t text_mbstowcs(wchar_t *dest, const char *src, int n);

/**
 * Convert a wide character to multibyte representation.
 */
int text_wctomb(char *s, wchar_t wchar);

/**
 * Get the maximum size to store a wide character converted to multibyte.
 */
int text_wcsz(void);

/**
 * Return whether the given wide character is printable.
 */
int text_iswprint(wint_t wc);

/**
 * Return pointer to the first occurrence of wc in the wide-character
 * string pointed to by wcs, or NULL if wc does not occur in the
 * string.
 */
wchar_t *text_wcschr(const wchar_t *wcs, wchar_t wc);

/**
 * Return the number of wide characters in s.
 */
size_t text_wcslen(const wchar_t *s);

/**
 * Print an error message
 */
extern void plog(const char *str);

/**
 * Exit, with optional message
 */
extern void quit(const char *str);


/**
 * Sorting functions
 */
extern void sort(void *array, size_t nmemb, size_t smemb,
		 int (*comp)(const void *a, const void *b));

/**
 * Create a hash for a string
 */
uint32_t djb2_hash(const char *str);

/**
 * Mathematical functions
 */
int mean(const int *nums, int size, struct my_rational *frac);
int variance(const int *nums, int size, bool unbiased, bool of_mean,
		struct my_rational *frac);
unsigned int gcd(unsigned int a, unsigned int b);
struct my_rational my_rational_construct(unsigned int numerator,
		unsigned int denominator);
unsigned int my_rational_to_uint(const struct my_rational *a,
		unsigned int scale, unsigned int *remainder);
struct my_rational my_rational_product(const struct my_rational *a,
		const struct my_rational *b);
struct my_rational my_rational_sum(const struct my_rational *a,
		const struct my_rational *b);

#endif /* INCLUDED_Z_UTIL_H */
