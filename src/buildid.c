/**
 * \file buildid.c
 * \brief Compile in build details
 *
 * Copyright (c) 2011 Andi Sidwell
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

#include "buildid.h"

/*
 * Allow the build system to generate version.h (and define
 * the HAVE_VERSION_H preprocessor macro) or get the version via the BUILD_ID
 * preprocessor macro.  If neither is available, use a sensible default.
 */
#ifdef HAVE_VERSION_H
#include "version.h"
#elif defined(BUILD_ID)
#define STR(x) #x
#define XSTR(x) STR(x)
#define VERSION_STRING XSTR(BUILD_ID)
#endif
#ifndef VERSION_STRING
#define VERSION_STRING "4.2.5"
#endif

const char *buildid = VERSION_NAME " " VERSION_STRING;
const char *buildver = VERSION_STRING;

/**
 * Hack -- Link a copyright message into the executable
 */
const char *copyright =
	"Авторское право (c) 1987-2022 Участники проекта Ангбанд.\n"
	"\n"
	"Эта работа является свободным программным обеспечением; вы можете\n"
	"распространять ее и/или изменять в соответствии с условиями либо:\n"
	"\n"
	"a) GNU General Public License опубликованная организацией Free Software\n"
	"   Foundation, версия 2, или\n"
	"\n"
	"b) лицензия Angband:\n"
	"   Данное ПО можно копировать и распространять для образовательных, \n"
	"   исследовательских целях и не в целях получения прибыли при условии, \n"
	"   что данное авторское право и заявление будут включены во все такие копии. \n"
	"   Могут применяться другие авторские права.\n";
	