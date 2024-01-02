/**
 * \file ui-death.c
 * \brief Handle the UI bits that happen after the character dies.
 *
 * Copyright (c) 1987 - 2007 Angband contributors
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

#include "angband.h"
#include "cmds.h"
#include "game-input.h"
#include "init.h"
#include "obj-desc.h"
#include "obj-info.h"
#include "savefile.h"
#include "store.h"
#include "ui-death.h"
#include "ui-history.h"
#include "ui-input.h"
#include "ui-knowledge.h"
#include "ui-menu.h"
#include "ui-object.h"
#include "ui-player.h"
#include "ui-score.h"
#include "ui-spoil.h"

/**
 * Write formatted string `fmt` on line `y`, centred between points x1 and x2.
 */
static void put_str_centred(int y, int x1, int x2, const char *fmt, ...)
{
	va_list vp;
	char *tmp;
	size_t len;
	int x;

	/* Format into the (growable) tmp */
	va_start(vp, fmt);
	tmp = vformat(fmt, vp);
	va_end(vp);

	/* Centre now; account for possible multibyte characters */
	len = utf8_strlen(tmp);
	x = x1 + ((x2-x1)/2 - len/2);

	put_str(tmp, y, x);
}


/**
 * Display the tombstone/retirement screen
 */
static void display_exit_screen(void)
{
	ang_file *fp;
	char buf[1024];
	int line = 0;
	time_t death_time = (time_t)0;
	bool retired = streq(player->died_from, "Retiring");

	Term_clear();
	(void)time(&death_time);
	struct tm *now = localtime(&death_time);

	/* Open the background picture */
	path_build(buf, sizeof(buf), ANGBAND_DIR_SCREENS,
		(retired) ? "retire.txt" : "dead.txt");
	fp = file_open(buf, MODE_READ, FTYPE_TEXT);
	
	if (fp) {
		text_out_indent = (Term->wid - 80) / 2;
		Term_gotoxy(0, (Term->hgt - 23) / 5);
		
		while (file_getl(fp, buf, sizeof(buf))) {
			// put_str(buf, line++, 0);
			text_out_e("%s", buf);
			text_out("\n");
		}
		text_out_indent = 0;
		file_close(fp);
	}

	line = 7;

	put_str_centred(line++, 8, 8+31, "%s", player->full_name);
	// put_str_centred(line++, 8, 8+31, "the");
	put_str_centred(line++, 8, 8+31, "%d - %d", (now->tm_year + 1900) - player->age, now->tm_year + 1900);
	
	if (player->total_winner)
		// put_str_centred(line++, 8, 8+31, "Magnificent");
		put_str_centred(line++, 8, 8+31, "Великолепный");
	else
		put_str_centred(line++, 8, 8+31, "%s", player->class->title[(player->lev - 1) / 5]);

	line++;

	put_str_centred(line++, 8, 8+31, "%s", player->class->name);
	// put_str_centred(line++, 8, 8+31, "Level: %d", (int)player->lev);
	put_str_centred(line++, 8, 8+31, "Уровень: %d", (int)player->lev);
	// put_str_centred(line++, 8, 8+31, "Exp: %d", (int)player->exp);
	put_str_centred(line++, 8, 8+31, "Опыт: %d", (int)player->exp);
	// put_str_centred(line++, 8, 8+31, "AU: %d", (int)player->au);
	put_str_centred(line++, 8, 8+31, "Золото: %d", (int)player->au);
	if (retired) {
		// put_str_centred(line++, 8, 8+31, "Retired on Level %d",
		put_str_centred(line++, 8, 8+31, "Отставка на этаже %d",
			player->depth);
	} else {
		// put_str_centred(line++, 8, 8+31, "Killed on Level %d",
		if (player->depth == 0){put_str_centred(line++, 8, 8+31, "Погиб в городе");}
		else {put_str_centred(line++, 8, 8+31, "Погиб на %d-м этаже", player->depth);}
		// put_str_centred(line++, 8, 8+31, "by %s.", player->died_from);
		put_str_centred(line++, 8, 8+31, "от %s", player->died_from);
	}

	line++;
	
	// put_str_centred(line, 8, 8+31, "on %-.24s", ctime(&death_time));
	
	if (now->tm_hour > 21)
		strnfmt(buf, sizeof(buf), "поздним вечером %d ", now->tm_mday);
	else if (now->tm_hour > 17)
		strnfmt(buf, sizeof(buf), "вечером %d ", now->tm_mday);
	else if (now->tm_hour > 10)
		strnfmt(buf, sizeof(buf), "днём %d ", now->tm_mday);
	else if (now->tm_hour > 6)
		strnfmt(buf, sizeof(buf), "утром %d ", now->tm_mday);
	else if (now->tm_hour > 4)
		strnfmt(buf, sizeof(buf), "ранним утром %d ", now->tm_mday);
	else
		strnfmt(buf, sizeof(buf), "ночью %d ", now->tm_mday);
	
	switch (now->tm_mon + 1) {
		case 1:
			my_strcat(buf, "января", sizeof(buf));
			break;
		case 2:
			my_strcat(buf, "февраля", sizeof(buf));
			break;
		case 3:
			my_strcat(buf, "марта", sizeof(buf));
			break;
		case 4:
			my_strcat(buf, "апреля", sizeof(buf));
			break;
		case 5:
			my_strcat(buf, "мая", sizeof(buf));
			break;
		case 6:
			my_strcat(buf, "июня", sizeof(buf));
			break;
		case 7:
			my_strcat(buf, "июля", sizeof(buf));
			break;
		case 8:
			my_strcat(buf, "августа", sizeof(buf));
			break;
		case 9:
			my_strcat(buf, "сентября", sizeof(buf));
			break;
		case 10:
			my_strcat(buf, "октября", sizeof(buf));
			break;
		case 11:
			my_strcat(buf, "ноября", sizeof(buf));
			break;
		case 12:
			my_strcat(buf, "декабря", sizeof(buf));
			break;
		default:
			my_strcat(buf, "дня месяца", sizeof(buf));
	}
		
	put_str_centred(line, 8, 8+31, "%s", buf);
}


/**
 * Display the winner crown
 */
static void display_winner(void)
{
	char buf[1024];
	ang_file *fp;

	int wid, hgt;
	int i = 2;

	path_build(buf, sizeof(buf), ANGBAND_DIR_SCREENS, "crown.txt");
	fp = file_open(buf, MODE_READ, FTYPE_TEXT);

	Term_clear();
	Term_get_size(&wid, &hgt);

	if (fp) {
		char *pe;
		long lw;
		int width;

		/* Get us the first line of file, which tells us how long the */
		/* longest line is */
		file_getl(fp, buf, sizeof(buf));
		lw = strtol(buf, &pe, 10);
		width = (pe != buf && lw > 0 && lw < INT_MAX) ? (int)lw : 25;

		/* Dump the file to the screen */
		while (file_getl(fp, buf, sizeof(buf))) {
			put_str(buf, i++, (wid / 2) - (width / 2));
		}

		file_close(fp);
	}

	// put_str_centred(i, 0, wid, "All Hail the Mighty Champion!");
	put_str_centred(i, 0, wid, "Слава Могучему Чемпиону!");

	event_signal(EVENT_INPUT_FLUSH);
	pause_line(Term);
}


/**
 * Menu command: dump character dump to file.
 */
static void death_file(const char *title, int row)
{
	// char buf[1024];
	char buf[2048];
	// char ftmp[80];
	char ftmp[160];

	/* Get the filesystem-safe name and append .txt */
	player_safe_name(ftmp, sizeof(ftmp), player->full_name, false);
	my_strcat(ftmp, ".txt", sizeof(ftmp));

	if (get_file(ftmp, buf, sizeof buf)) {
		bool success;

		/* Dump a character file */
		screen_save();
		success = dump_save(buf);
		screen_load();

		/* Check result */
		if (success)
			// msg("Character dump successful.");
			msg("Сохранение персонажа выполнено успешно.");
		else
			// msg("Character dump failed!");
			msg("Сохранение персонажа завершилося неудачей!");

		/* Flush messages */
		event_signal(EVENT_MESSAGE_FLUSH);
	}
}

/**
 * Menu command: view character dump and inventory.
 */
static void death_info(const char *title, int row)
{
	struct store *home = &stores[f_info[FEAT_HOME].shopnum - 1];

	screen_save();

	/* Display player */
	display_player(0);

	/* Prompt for inventory */
	// prt("Hit any key to see more information: ", 0, 0);
	prt("Нажмите любую клавишу для дополнительной информации: ", 0, 0);
	c_prt(COLOUR_L_BLUE, "-ещё-", 0, 53);
	
	/* Allow abort at this point */
	(void)anykey();


	/* Show equipment and inventory */

	/* Equipment -- if any */
	if (player->upkeep->equip_cnt) {
		Term_clear();
		show_equip(OLIST_WEIGHT | OLIST_SEMPTY | OLIST_DEATH, NULL);
		// prt("You are using: -more-", 0, 0);
		prt("Вы использовали: ", 0, 0);
		c_prt(COLOUR_L_BLUE, "-ещё-", 0, 17);
		(void)anykey();
	}

	/* Inventory -- if any */
	if (player->upkeep->inven_cnt) {
		Term_clear();
		show_inven(OLIST_WEIGHT | OLIST_DEATH, NULL);
		// prt("You are carrying: -more-", 0, 0);
		prt("У вас с собой было: ", 0, 0);
		c_prt(COLOUR_L_BLUE, "-ещё-", 0, 20);
		(void)anykey();
	}

	/* Quiver -- if any */
	if (player->upkeep->quiver_cnt) {
		Term_clear();
		show_quiver(OLIST_WEIGHT | OLIST_DEATH, NULL);
		// prt("Your quiver holds: -more-", 0, 0);
		prt("В вашем колчане было: ", 0, 0);
		c_prt(COLOUR_L_BLUE, "-ещё-", 0, 22);
		(void)anykey();
	}

	/* Home -- if anything there */
	if (home->stock) {
		int page;
		struct object *obj = home->stock;

		/* Display contents of the home */
		for (page = 1; obj; page++) {
			int line;

			/* Clear screen */
			Term_clear();

			/* Show 12 items */
			for (line = 0; obj && line < 12; obj = obj->next, line++) {
				uint8_t attr;

				// char o_name[80];
				char o_name[160];
				// char tmp_val[80];
				char tmp_val[160];

				/* Print header, clear line */
				strnfmt(tmp_val, sizeof(tmp_val), "%c) ", I2A(line));
				prt(tmp_val, line + 2, 4);

				/* Get the object description */
				object_desc(o_name, sizeof(o_name), obj,
					ODESC_PREFIX | ODESC_FULL, player);

				/* Get the inventory color */
				attr = obj->kind->base->attr;

				/* Display the object */
				c_put_str(attr, o_name, line + 2, 7);
			}

			/* Caption */
			// prt(format("Your home contains (page %d): -more-", page), 0, 0);
			prt(format("В вашем доме было (стр. %d): ", page), 0, 0);
			c_prt(COLOUR_L_BLUE, "-ещё-", 0, 29);

			/* Wait for it */
			(void)anykey();
		}
	}

	screen_load();
}

/**
 * Menu command: peruse pre-death messages.
 */
static void death_messages(const char *title, int row)
{
	screen_save();
	do_cmd_messages();
	screen_load();
}

/**
 * Menu command: see top twenty scores.
 */
static void death_scores(const char *title, int row)
{
	screen_save();
	show_scores();
	screen_load();
}

/**
 * Menu command: examine items in the inventory.
 */
static void death_examine(const char *title, int row)
{
	struct object *obj;
	const char *q, *s;

	/* Get an item */
	// q = "Examine which item? ";
	q = "Какой предмет изучить? ";
	// s = "You have nothing to examine.";
	s = "Вам нечего изучать.";

	while (get_item(&obj, q, s, 0, NULL, (USE_INVEN | USE_QUIVER | USE_EQUIP | IS_HARMLESS))) {
		// char header[120];
		char header[240];

		textblock *tb;
		region area = { 1, 0, 0, 0 };

		tb = object_info(obj, OINFO_NONE);
		object_desc(header, sizeof(header), obj,
			ODESC_PREFIX | ODESC_FULL | ODESC_CAPITAL, player);

		textui_textblock_show(tb, area, header);
		textblock_free(tb);
	}
}


/**
 * Menu command: view character history.
 */
static void death_history(const char *title, int row)
{
	history_display();
}

/**
 * Menu command: allow spoiler generation (mainly for randarts).
 */
static void death_spoilers(const char *title, int row)
{
	do_cmd_spoilers();
}

/***
 * Menu command: start a new game
 */
static void death_new_game(const char *title, int row)
{
    // play_again = get_check("Start a new game? ");
    play_again = get_check("Начать новую игру? ");
}

/**
 * Menu structures for the death menu. Note that Quit must always be the
 * last option, due to a hard-coded check in death_screen
 */
static menu_action death_actions[] =
{
	// { 0, 'i', "Information",   death_info      },
	{ 0, 'i', "Информация",   death_info      },
	// { 0, 'm', "Messages",      death_messages  },
	{ 0, 'm', "Сообщения",      death_messages  },
	// { 0, 'f', "File dump",     death_file      },
	{ 0, 'f', "Сохранить в файл",     death_file      },
	// { 0, 'v', "View scores",   death_scores    },
	{ 0, 'v', "Смотреть счёт",   death_scores    },
	// { 0, 'x', "Examine items", death_examine   },
	{ 0, 'x', "Проверить предметы", death_examine   },
	// { 0, 'h', "History",       death_history   },
	{ 0, 'h', "История",       death_history   },
	// { 0, 's', "Spoilers",      death_spoilers  },
	{ 0, 's', "Спойлеры",      death_spoilers  },
	// { 0, 'n', "New Game",      death_new_game  },
	{ 0, 'n', "Новая игра",      death_new_game  },
	// { 0, 'q', "Quit",          NULL            },
	{ 0, 'q', "Выход",          NULL            },
};



/**
 * Handle character death
 */
void death_screen(void)
{
	struct menu *death_menu;
	bool done = false;
	const region area = { 51, 2, 0, N_ELEMENTS(death_actions) };

	/* Winner */
	if (player->total_winner)
	{
		display_winner();
	}

	/* Tombstone/retiring */
	display_exit_screen();

	/* Flush all input and output */
	event_signal(EVENT_INPUT_FLUSH);
	event_signal(EVENT_MESSAGE_FLUSH);

	/* Display and use the death menu */
	death_menu = menu_new_action(death_actions,
			N_ELEMENTS(death_actions));

	death_menu->flags = MN_CASELESS_TAGS;

	menu_layout(death_menu, &area);

	while (!done && !play_again)
	{
		ui_event e = menu_select(death_menu, EVT_KBRD, false);
		if (e.type == EVT_KBRD)
		{
			if (e.key.code == KTRL('X')) break;
			if (e.key.code == KTRL('N')) play_again = true;
		}
		else if (e.type == EVT_SELECT)
		{
			// done = get_check("Do you want to quit? ");
			done = get_check("Вы хотите выйти из игры? ");
		}
	}

	menu_free(death_menu);
}
