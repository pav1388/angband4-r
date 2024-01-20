/**
 * \file obj-desc.c
 * \brief Create object name descriptions
 *
 * Copyright (c) 1997 - 2007 Angband contributors
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
#include "obj-chest.h"
#include "obj-desc.h"
#include "obj-gear.h"
#include "obj-ignore.h"
#include "obj-knowledge.h"
#include "obj-tval.h"
#include "obj-util.h"

/**
 * Puts the object base kind's name into buf.
 */
// void object_base_name(char *buf, size_t max, int tval, bool plural)
void object_base_name(char *buf, size_t max, int tval, uint8_t plural)
{
	struct object_base *kb = &kb_info[tval];
	size_t end = 0;

	if (kb->name && kb->name[0]) 
		(void) obj_desc_name_format(buf, max, end, kb->name, NULL, plural);
}


/**
 * Puts a very stripped-down version of an object's name into buf.
 * If easy_know is true, then the IDed names are used, otherwise
 * flavours, scroll names, etc will be used.
 *
 * Just truncates if the buffer isn't big enough.
 */
void object_kind_name(char *buf, size_t max, const struct object_kind *kind,
					  bool easy_know)
{
	/* If not aware, the plain flavour (e.g. Copper) will do. */
	if (!easy_know && !kind->aware && kind->flavor)
		// my_strcpy(buf, kind->flavor->text, max);
		(void) obj_desc_name_format(buf, max, 0, kind->flavor->text, NULL, 0);

	/* Use proper name (Healing, or whatever) */
	else
		// (void) obj_desc_name_format(buf, max, 0, kind->name, NULL, false);
		(void) obj_desc_name_format(buf, max, 0, kind->name, NULL, 0);
}


/**
 * A modifier string, put where '#' goes in the basename below.  The weird
 * games played with book names are to allow the non-essential part of the
 * name to be abbreviated when there is not much room to display.
 */
static const char *obj_desc_get_modstr(const struct object_kind *kind)
{
	if (tval_can_have_flavor_k(kind))
		return kind->flavor ? kind->flavor->text : "";

	if (tval_is_book_k(kind))
		return kind->name;

	return "";
}

/**
 * An object's basic name - a generic name for flavored objects (with the
 * actual name added later depending on awareness, the name from object.txt
 * for almost everything else, and a bit extra for books. 
 */
static const char *obj_desc_get_basename(const struct object *obj, bool aware,
		bool terse, uint32_t mode, const struct player *p)
{
	bool show_flavor = !terse && obj->kind->flavor;

	if (mode & ODESC_STORE)
		show_flavor = false;
	if (aware && p && !OPT(p, show_flavors)) show_flavor = false;

	/* Artifacts are special */
	if (obj->artifact && (aware || object_is_known_artifact(obj) || terse ||
						  !obj->kind->flavor))
		return obj->kind->name;

	/* Analyze the object */
	switch (obj->tval)
	{
		case TV_FLASK:
		case TV_CHEST:
		case TV_SHOT:
		case TV_BOLT:
		case TV_ARROW:
		case TV_BOW:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		case TV_DIGGING:
		case TV_BOOTS:
		case TV_GLOVES:
		case TV_CLOAK:
		case TV_CROWN:
		case TV_HELM:
		case TV_SHIELD:
		case TV_SOFT_ARMOR:
		case TV_HARD_ARMOR:
		case TV_DRAG_ARMOR:
		case TV_LIGHT:
		case TV_FOOD:
			return obj->kind->name;

		case TV_AMULET:
			// return (show_flavor ? "& # Amulet~" : "& Amulet~");
			return (show_flavor ? "# Амулет=0110" : "Амулет=0110");

		case TV_RING:
			// return (show_flavor ? "& # Ring~" : "& Ring~");
			return (show_flavor ? "Кол=0059#" : "Кол=0059");

		case TV_STAFF:
			// return (show_flavor ? "& # Sta|ff|ves|" : "& Sta|ff|ves|");
			return (show_flavor ? "Посох=0130#" : "Посох=0130");

		case TV_WAND:
			// return (show_flavor ? "& # Wand~" : "& Wand~");
			return (show_flavor ? "# Палоч.ек=0230" : "Палоч.ек=0230");

		case TV_ROD:
			// return (show_flavor ? "& # Rod~" : "& Rod~");
			return (show_flavor ? "# Жезл=0110" : "Жезл=0110");

		case TV_POTION:
			// return (show_flavor ? "& # Potion~" : "& Potion~");
			return (show_flavor ? "# Зел=0060" : "Зел=0060");

		case TV_SCROLL:
			// return (show_flavor ? "& Scroll~ titled #" : "& Scroll~");
			return (show_flavor ? "Свит.ок=0130 с надписью #" : "Свит.ок=0130");

		case TV_MAGIC_BOOK:
			if (terse)
				// return "& Book~ #";
				return "Книг=0230 #";
			else
				// return "& Book~ of Magic Spells #";
				return "Книг=0230 Магических Заклинаний #";

		case TV_PRAYER_BOOK:
			if (terse)
				// return "& Book~ #";
				return "Книг=0230 #";
			else
				// return "& Holy Book~ of Prayers #";
				return "Свят=1210 Книг=0230 Молитв #";

		case TV_NATURE_BOOK:
			if (terse)
				// return "& Book~ #";
				return "Книг=0230 #";
			else
				// return "& Book~ of Nature Magics #";
				return "Книг=0230 Магии Природы #";

		case TV_SHADOW_BOOK:
			if (terse)
				// return "& Tome~ #";
				return "Том=0110 #";
			else
				// return "& Necromantic Tome~ #";
				return "Некромантическ=1130 Том=0110 #";

		case TV_OTHER_BOOK:
			if (terse)
				// return "& Book~ #";
				return "Книг=0230 #";
			else
				// return "& Book of Mysteries~ #";
				return "Книг=0230 Таинств #";

		case TV_MUSHROOM:
			// return (show_flavor ? "& # Mushroom~" : "& Mushroom~");
			return (show_flavor ? "# Гриб=0110" : "Гриб=0110");
	}

	// return "(nothing)";
	return "(ничего)";
}


/**
 * Start to description, indicating number/uniqueness (a, the, no more, 7, etc)
 */
static size_t obj_desc_name_prefix(char *buf, size_t max, size_t end,
		const struct object *obj, const char *basename,
		const char *modstr, bool terse, uint16_t number)
{
	if (number == 0) {
		// strnfcat(buf, max, &end, "no more ");
		strnfcat(buf, max, &end, "больше нет ");
	} else if (number > 1) {
		strnfcat(buf, max, &end, "%u ", number);
	// } else if (object_is_known_artifact(obj)) {
		// strnfcat(buf, max, &end, "the ");
	// } else if (*basename == '&') {
		// bool an = false;
		// const char *lookahead = basename + 1;

		// while (*lookahead == ' ') lookahead++;

		// if (*lookahead == '#') {
			// if (modstr && is_a_vowel(*modstr))
				// an = true;
		// } else if (is_a_vowel(*lookahead)) {
			// an = true;
		// }

		// if (!terse) {
			// if (an)
				// strnfcat(buf, max, &end, "an ");
			// else
				// strnfcat(buf, max, &end, "a ");			
		// }
	}

	return end;
}



/**
 * Formats 'fmt' into 'buf', with the following formatting characters:
 *
 * '~' at the end of a word (e.g. "fridge~") will pluralise
 *
 * '|x|y|' will be output as 'x' if singular or 'y' if plural
 *    (e.g. "kni|fe|ves|")
 *
 * '#' will be replaced with 'modstr' (which may contain the pluralising
 * formats given above).
 */
size_t obj_desc_name_format(char *buf, size_t max, size_t end,
		// const char *fmt, const char *modstr, bool pluralise)
		const char *fmt, const char *modstr, uint8_t pluralise)
{
	uint8_t index[4] = {10, 10, 10, 10};
	uint8_t cas = pluralise >> 1;
	pluralise &= 1;
	bool plural_noun = pluralise;
	
	/* Copy the string */
	while (*fmt) {
		/* Skip */
		if (*fmt == '&') {
			while (*fmt == ' ' || *fmt == '&')
				fmt++;
			continue;
		} else if (*fmt == '~') {
			/* Pluralizer (regular English plurals) */
			char prev = *(fmt - 1);

			if (!pluralise)	{
				fmt++;
				continue;
			}

			/* e.g. cutlass-e-s, torch-e-s, box-e-s */
			if (prev == 's' || prev == 'h' || prev == 'x')
				strnfcat(buf, max, &end, "es");
			else
				strnfcat(buf, max, &end, "s");
		
		// "Гра­м­ма­ти­че­с­кий сло­варь ру­с­с­ко­го язы­ка: Сло­во­и­з­ме­не­ние"
		// А. А. Зализняк
		} else if (*fmt == '.') { // Метка чередования беглой гласной с нулём
			const char *initial_fmt = fmt;
			while (*fmt) {
				if (*fmt == '=') { // Метка конца основы слова
					for (uint8_t i = 0; i != 4; i++) {
						fmt++;
						if (!(isdigit((unsigned char) *fmt)))
							break;
						
						index[i] = *fmt - 0x30;
					}
					index[W_DECL] = !index[W_DECL] ? index[W_DECL] : index[W_DECL] - 1;
					break;
				}
				fmt++;
			}
			fmt = initial_fmt;
			
			if (cas == 6 && pluralise) {
				cas = C_RODIT;
				plural_noun = false;
			}
			// прилагательное, м.р. 6 склонение (дракон.ьий=1160)
			if (index[W_PART] == W_PART_ADJ && index[W_GEN] == W_GEN_MALE && index[W_DECL] == 5) {
				if ((cas == C_IMEN || cas == C_VINIT) && !pluralise)
					fmt+=2;
			}
			// существительное, м.р. 1-8 склонение,  ж.р. 8 склонение
			if (index[W_PART] == W_PART_NOUN && (index[W_GEN] == W_GEN_MALE || (index[W_GEN] == W_GEN_FEM && index[W_DECL] == 7))) {
				if ((cas != C_IMEN && cas != C_VINIT) || plural_noun)
					fmt+=2;
			}
			// существительное, с.р. 1-5 склонение, родит.падеж , множ.число
			if (index[W_PART] == W_PART_NOUN && index[W_GEN] == W_GEN_NEUT && index[W_DECL] < 5 && cas == C_RODIT && plural_noun)
				fmt+=2;
				
			// существительное, ж.р. 1-7 склонение
			if (index[W_PART] == W_PART_NOUN && index[W_GEN] == W_GEN_FEM && index[W_DECL] < 7) {
				if (cas == C_RODIT && plural_noun)
					;
				else
					fmt+=2;
			}
			
		} else if (*fmt == '=') { // Метка конца основы слова
			for (uint8_t i = 0; i != 4; i++) {
				fmt++;
				if (!(isdigit((unsigned char) *fmt))) {
					strnfcat(buf, max, &end, " <-ОШИБКА! ");
					break;
				}
				index[i] = *fmt - 0x30;
			}
			index[W_DECL] = !index[W_DECL] ? index[W_DECL] : index[W_DECL] - 1;
			// часть речи (сущ = 0, прил = 1, не исп. числ, мест, гл, прич)
			// index[W_PART] = 
				// index[W_PART] > 5 ? 0 : index[W_PART];
			// род (сред = 0, муж = 1, жен = 2)
			// index[W_GEN] = 
				// index[W_GEN] > 2 ? 0 : index[W_GEN];
			// тип склонения по А. А. Зализняку (1-8)
			// index[W_DECL] = 
				// index[W_DECL] > 8 ? 0 : index[W_DECL] == 0 ? 0 : index[W_DECL] - 1;
			// чередование (нет = 0, систематическое = 1~3, уникальное = 4~9)
			// index[W_ALTER] = index[W_ALTER] > 9 ? 0 : index[W_ALTER];
			
			// возможные окончания существительных и прилагательных (60 шт.)
			// примерно в 3 раза меньше памяти от того, если хранить char`ы окончаний с массивах
			char *ending_chars[] = {"", "а", "ам", "ами", "ах", "ая", "е", "ев", "его", "ее", 
				"ей", "ем", "ём", "ему", "енем", "и", "ие", "ием", "ии", "ий", "им", "ими", "их", 
				"ию", "ия", "иями", "иях", "й", "о", "ов", "ого", "ое", "ой", "ом", "ому", "у", 
				"ую", "ы", "ые", "ый", "ым", "ыми", "ых", "ь", "ье", "ьё", "ьем", "ьём", "ью", 
				"ья", "ьям", "ьями", "ьях", "ю", "юю", "я", "ям", "ями", "ях", "яя"};
			
			// прилагательное[склонение][падеж][род/число]
			uint8_t ending_adjective[8][6][4] = {
				// {{с.р., м.р., ж.р., мн.ч.}}
				// склонение 1 (word=1x1x)
				{{E_OE, E_YJ, E_AYA, E_YE},	// имен.п.  C_IMEN		0
				{E_OGO, E_OGO, E_OJ, E_YH}, // родит.п. C_RODIT		1
				{E_OMU, E_OMU, E_OJ, E_YM}, // дат.п.   C_DAT		2
				{E_OE, E_YJ, E_UYU, E_YE}, 	// вин.п.   C_VINIT		3
				{E_YM, E_YM, E_OJ, E_YMI}, 	// твор.п.  C_TVORIT	4
				{E_OM, E_OM, E_OJ, E_YH}},	// пред.п.  C_PREDL		5
				// склонение 2 (word=1x2x)
				{{E_EE, E_IJ, E_YAYA, E_IE},
				{E_EGO, E_EGO, E_EJ, E_IH},
				{E_EMU, E_EMU, E_EJ, E_IM},
				{E_EE, E_IJ, E_YUYU, E_IE},
				{E_IM, E_IM, E_EJ, E_IMI},
				{E_EM, E_EM, E_EJ, E_IH}},
				// склонение 3 (word=1x3x)
				{{E_OE, E_IJ, E_AYA, E_IE},
				{E_OGO, E_OGO, E_OJ, E_IH},
				{E_OMU, E_OMU, E_OJ, E_IM},
				{E_OE, E_IJ, E_UYU, E_IE},
				{E_IM, E_IM, E_OJ, E_IMI},
				{E_OM, E_OM, E_OJ, E_IH}},
				// склонение 4 (word=1x4x)
				{{E_EE, E_IJ, E_AYA, E_IE},
				{E_EGO, E_EGO, E_EJ, E_IH},
				{E_EMU, E_EMU, E_EJ, E_IM},
				{E_EE, E_IJ, E_UYU, E_IE},
				{E_IM, E_IM, E_EJ, E_IMI},
				{E_EM, E_EM, E_EJ, E_IH}},
				// склонение 5 (word=1x5x)
				{{E_EE, E_YJ, E_AYA, E_YE},
				{E_EGO, E_EGO, E_EJ, E_YH},
				{E_EMU, E_EMU, E_EJ, E_YM},
				{E_EE, E_YJ, E_UYU, E_YE},
				{E_YM, E_YM, E_EJ, E_YMI},
				{E_EM, E_EM, E_EJ, E_YH}},
				// склонение 6 (word=1x6x)
				{{E_E, E_IJ, E_YA, E_I},
				{E_EGO, E_EGO, E_EJ, E_IH},
				{E_EMU, E_EMU, E_EJ, E_IM},
				{E_E, E_IJ, E_YU, E_I},
				{E_IM, E_IM, E_EJ, E_IMI},
				{E_EM, E_EM, E_EJ, E_IH}}, 
				// склонение 1b (word=1x7x)
				{{E_OE, E_OJ, E_AYA, E_YE},
				{E_OGO, E_OGO, E_OJ, E_YH},
				{E_OMU, E_OMU, E_OJ, E_YM},
				{E_OE, E_OJ, E_UYU, E_YE},
				{E_YM, E_YM, E_OJ, E_YMI},
				{E_OM, E_OM, E_OJ, E_YH}}, 
				// склонение 3b (word=1x8x)
				{{E_OE, E_OJ, E_AYA, E_IE},
				{E_OGO, E_OGO, E_OJ, E_IH},
				{E_OMU, E_OMU, E_OJ, E_IM},
				{E_OE, E_OJ, E_UYU, E_IE},
				{E_IM, E_IM, E_OJ, E_IMI},
				{E_OM, E_OM, E_OJ, E_IH}} };

			// существительное[склонение][падеж][род][число]
			uint8_t ending_noun[8][6][3][2] = {
				// {{с.р. ед.ч., ср.р. мн.ч.},{м.р. ед.ч., м.р. мн.ч.},{ж.р. ед.ч., ж.р. мн.ч.}}
				{ // склонение 1 (word=0x1x)
				{{E_O,E_A},{E_,E_Y},{E_A,E_Y}},				// имен.п.  C_IMEN		0
				{{E_A,E_},{E_A,E_OV},{E_Y,E_}}, 			// родит.п. C_RODIT		1
				{{E_U,E_AM},{E_U,E_AM},{E_E,E_AM}}, 		// дат.п.   C_DAT		2
				{{E_O,E_A},{E_,E_Y},{E_U,E_Y}}, 			// вин.п.   C_VINIT		3
				{{E_OM,E_AMI},{E_OM,E_AMI},{E_OJ,E_AMI}}, 	// твор.п.  C_TVORIT	4
				{{E_E,E_AH},{E_E,E_AH},{E_E,E_AH}}			// пред.п.  C_PREDL		5
				}, { // склонение 2 (word=0x2x)
				{{E_E,E_YA},{E_b,E_I},{E_YA,E_I}},
				{{E_YA,E_b},{E_YA,E_EJ},{E_I,E_b}},
				{{E_YU,E_YAM},{E_YU,E_YAM},{E_E,E_YAM}},
				{{E_E,E_YA},{E_b,E_I},{E_YU,E_I}},
				{{E_EM,E_YAMI},{E_EM,E_YAMI},{E_EJ,E_YAMI}},
				{{E_E,E_YAH},{E_E,E_YAH},{E_E,E_YAH}}
				}, { // склонение 3 (word=0x3x)
				{{E_O,E_A},{E_,E_I},{E_A,E_I}},
				{{E_A,E_},{E_A,E_OV},{E_I,E_}},
				{{E_U,E_AM},{E_U,E_AM},{E_E,E_AM}},
				{{E_O,E_A},{E_,E_I},{E_U,E_I}},
				{{E_OM,E_AMI},{E_OM,E_AMI},{E_OJ,E_AMI}},
				{{E_E,E_AH},{E_E,E_AH},{E_E,E_AH}}
				}, { // склонение 4 (word=0x4x)
				{{E_E,E_A},{E_,E_I},{E_A,E_I}},
				{{E_A,E_},{E_A,E_EJ},{E_I,E_}},
				{{E_U,E_AM},{E_U,E_AM},{E_E,E_AM}},
				{{E_E,E_A},{E_,E_I},{E_U,E_I}},
				{{E_EM,E_AMI},{E_EM,E_AMI},{E_EJ,E_AMI}},
				{{E_E,E_AH},{E_E,E_AH},{E_E,E_AH}}
				}, { // склонение 5 (word=0x5x)
				{{E_E,E_A},{E_,E_Y},{E_A,E_Y}},
				{{E_A,E_},{E_A,E_EV},{E_Y,E_}},
				{{E_U,E_AM},{E_U,E_AM},{E_E,E_AM}},
				{{E_E,E_A},{E_,E_Y},{E_U,E_Y}},
				{{E_EM,E_AMI},{E_EM,E_AMI},{E_EJ,E_AMI}},
				{{E_E,E_AH},{E_E,E_AH},{E_E,E_AH}}
				}, { // склонение 6 (word=0x6x)
				{{E_bE,E_bYA},{E_J,E_I},{E_YA,E_I}},
				{{E_bYA,E_IJ},{E_YA,E_EV},{E_I,E_J}},
				{{E_bYU,E_bYAM},{E_YU,E_YAM},{E_E,E_YAM}},
				{{E_bE,E_bYA},{E_J,E_I},{E_YU,E_I}},
				{{E_bEM,E_bYAMI},{E_EM,E_YAMI},{E_EJ,E_YAMI}},
				{{E_bE,E_bYAH},{E_E,E_YAH},{E_E,E_YAH}}
				}, { // склонение 7 (word=0x7x)
				{{E_IE,E_IYA},{E_J,E_I},{E_YA,E_I}},
				{{E_IYA,E_IJ},{E_YA,E_EV},{E_I,E_J}},
				{{E_IYU,E_IYAMI},{E_YU,E_YAM},{E_I,E_YAM}},
				{{E_IE,E_IYA},{E_J,E_I},{E_YU,E_I}},
				{{E_IEM,E_IYAMI},{E_EM,E_YAMI},{E_EJ,E_YAMI}},
				{{E_II,E_IYAH},{E_I,E_YAH},{E_I,E_YAH}}
				}, { // склонение 8 (word=0x8x)
				{{E_YA,0},{E_b,E_J},{E_b,E_I}},
				{{0,0},{E_J,E_EJ},{E_I,E_EJ}},
				{{0,0},{E_J,E_YAM},{E_I,E_YAM}},
				{{E_YA,0},{E_b,E_I},{E_b,E_I}},
				{{E_ENEM,0},{E_YOM,E_YAMI},{E_bYU,E_YAMI}},
				{{0,0},{E_J,E_YAH},{E_I,E_YAH}}
				}};

			uint8_t result = 0;
			if (cas == 6 && pluralise) {
					cas = C_RODIT;
					plural_noun = false;
			}
			// вывод окончания для прилигательных
			if (index[W_PART] == W_PART_ADJ) {
				if (index[W_ALTER]) {
					if (index[W_ALTER] == 5) // только множ.ч.
						result = ending_adjective[index[W_DECL]][cas][3];
				} else
					result = ending_adjective[index[W_DECL]][cas][pluralise ? 3 : index[W_GEN]];
			
			// вывод окончания для существительных		
			} else if (index[W_PART] == W_PART_NOUN) {
				if (index[W_ALTER]) {
					// систематические
					if (index[W_GEN] == W_GEN_NEUT && index[W_ALTER] == 1 && cas == C_VINIT && plural_noun) // яблоко=0031
						result = E_I;
					else if (index[W_GEN] == W_GEN_NEUT && index[W_ALTER] == 4 && (cas == C_IMEN || cas == C_TVORIT) && !plural_noun) // копьё=0064
						result = ending_noun[index[W_DECL]][cas][index[W_GEN]][plural_noun] + 1; // е -> ё
					else if (index[W_ALTER] == 5) // только множ.ч. (латы=0215)
						result = ending_noun[index[W_DECL]][cas][index[W_GEN]][1];
					// уникальные
					else if (index[W_GEN] == W_GEN_NEUT && index[W_ALTER] == 9) { // кольцо=0059
						strnfcat(buf, max, &end, "%s", (cas == C_RODIT && plural_noun) ? "ец" : "ьц");
						if ((cas == C_IMEN || cas == C_VINIT) && !plural_noun)
							result = E_O;
						else 
							result = ending_noun[index[W_DECL]][cas][index[W_GEN]][plural_noun];
					} else
						result = ending_noun[index[W_DECL]][cas][index[W_GEN]][plural_noun];
				
				} else
					result = ending_noun[index[W_DECL]][cas][index[W_GEN]][plural_noun];
			}
			strnfcat(buf, max, &end, "%s", ending_chars[result]);
		} else if (*fmt == '|') {
			/* Special plurals 
			* e.g. kni|fe|ves|
			*          ^  ^  ^ */
			const char *singular = fmt + 1;
			const char *plural   = strchr(singular, '|');
			const char *endmark  = NULL;

			if (plural) {
				plural++;
				endmark = strchr(plural, '|');
			}

			if (!singular || !plural || !endmark) return end;

			if (!pluralise)
				strnfcat(buf, max, &end, "%.*s",
					(int) (plural - singular) - 1,
					singular);
			else
				strnfcat(buf, max, &end, "%.*s",
					(int) (endmark - plural), plural);

			fmt = endmark;
		} else if (*fmt == '#') {
			/* Add modstr, with pluralisation if relevant */
			end = obj_desc_name_format(buf, max, end, modstr, NULL,	pluralise);
		}

		else
			buf[end++] = *fmt;

		fmt++;
	}

	buf[end] = 0;

	return end;
}


/**
 * Format object obj's name into 'buf'.
 */
static size_t obj_desc_name(char *buf, size_t max, size_t end,
		const struct object *obj, bool prefix, uint32_t mode,
		bool terse, const struct player *p)
{
	bool store = mode & ODESC_STORE ? true : false;
	bool spoil = mode & ODESC_SPOIL ? true : false;
	uint16_t number = (mode & ODESC_ALTNUM) ?
		(mode & 0xFFFF0000) >> 16 : obj->number;
	
	/* Actual name for flavoured objects if aware, or in store, or spoiled */
	bool aware = object_flavor_is_aware(obj) || store || spoil;
	/* Pluralize if (not forced singular) and
	 * (not a known/visible artifact) and
	 * (not one in stack or forced plural) */
	// bool plural = !(mode & ODESC_SINGULAR) &&
	uint8_t plural = !(mode & ODESC_SINGULAR) &&
		!obj->artifact &&
		(number != 1 || (mode & ODESC_PLURAL));
	const char *basename = obj_desc_get_basename(obj, aware, terse,
		mode, p);
	const char *modstr = obj_desc_get_modstr(obj->kind);

	/* Quantity prefix */
	if (prefix)
		end = obj_desc_name_prefix(buf, max, end, obj, basename,
			modstr, terse, number);

	/* Base name */
	plural = (PLURAL_RU(number, C_IMEN, 6, C_RODIT) << 1) + plural;
	end = obj_desc_name_format(buf, max, end, basename, modstr, plural);

	/* Append extra names of various kinds */
	if (object_is_known_artifact(obj))
		strnfcat(buf, max, &end, " %s", obj->artifact->name);
	else if ((obj->known->ego && !(mode & ODESC_NOEGO)) || (obj->ego && store))
		strnfcat(buf, max, &end, " %s", obj->ego->name);
	else if (aware && !obj->artifact &&
			 (obj->kind->flavor || obj->kind->tval == TV_SCROLL)) {
		if (terse)
			strnfcat(buf, max, &end, " '%s'", obj->kind->name);
		else
			// strnfcat(buf, max, &end, " of %s", obj->kind->name);
			strnfcat(buf, max, &end, " %s", obj->kind->name);
	}

	return end;
}

/**
 * Is obj armor?
 */
static bool obj_desc_show_armor(const struct object *obj,
		const struct player *p)
{
	return (!p || p->obj_k->ac) && (obj->ac || tval_is_armor(obj));
}

/**
 * Special descriptions for types of chest traps
 */
static size_t obj_desc_chest(const struct object *obj, char *buf, size_t max,
							 size_t end)
{
	if (!tval_is_chest(obj)) return end;

	/* The chest is unopened, but we know nothing about its trap/lock */
	if (obj->pval && !obj->known->pval) return end;

	/* Describe the traps */
	strnfcat(buf, max, &end, " (%s)", chest_trap_name(obj));

	return end;
}

/**
 * Describe combat properties of an item - damage dice, to-hit, to-dam, armor
 * class, missile multipler
 */
static size_t obj_desc_combat(const struct object *obj, char *buf, size_t max, 
		size_t end, uint32_t mode, const struct player *p)
{
	bool spoil = mode & ODESC_SPOIL ? true : false;

	/* Display damage dice if they are known */
	if (kf_has(obj->kind->kind_flags, KF_SHOW_DICE) &&
		(!p || (p->obj_k->dd && p->obj_k->ds))) {
		// strnfcat(buf, max, &end, " (%dd%d)", obj->dd, obj->ds);
		strnfcat(buf, max, &end, " (%dк%d)", obj->dd, obj->ds);
	}

	/* Display shooting power as part of the multiplier */
	if (kf_has(obj->kind->kind_flags, KF_SHOW_MULT)) {
		strnfcat(buf, max, &end, " (x%d)",
				 obj->pval + obj->modifiers[OBJ_MOD_MIGHT]);
	}

	/* No more if the object hasn't been assessed */
	if (!((obj->notice & OBJ_NOTICE_ASSESSED) || spoil)) return end;

	/* Show weapon bonuses if we know of any */
	if ((!p || (p->obj_k->to_h && p->obj_k->to_d))
			&& (tval_is_weapon(obj) || obj->to_d
			|| (obj->to_h && !tval_is_body_armor(obj))
			|| (!object_has_standard_to_h(obj)
			&& !obj->artifact && !obj->ego))) {
		/* In general show full combat bonuses */
		strnfcat(buf, max, &end, " (%+d,%+d)", obj->to_h, obj->to_d);
	} else if (obj->to_h < 0 && object_has_standard_to_h(obj)) {
		/* Special treatment for body armor with only a to-hit penalty */
		strnfcat(buf, max, &end, " (%+d)", obj->to_h);
	} else if (obj->to_d != 0 && (!p || p->obj_k->to_d)) {
		/* To-dam rune known only */
		strnfcat(buf, max, &end, " (%+d)", obj->to_d);
	} else if (obj->to_h != 0 && (!p || p->obj_k->to_h)) {
		/* To-hit rune known only */
		strnfcat(buf, max, &end, " (%+d)", obj->to_h);
	}

	/* Show armor bonuses */
	if (!p || p->obj_k->to_a) {
		if (obj_desc_show_armor(obj, p))
			strnfcat(buf, max, &end, " [%d,%+d]", obj->ac, obj->to_a);
		else if (obj->to_a)
			strnfcat(buf, max, &end, " [%+d]", obj->to_a);
	} else if (obj_desc_show_armor(obj, p)) {
		strnfcat(buf, max, &end, " [%d]", obj->ac);
	}

	return end;
}

/**
 * Describe remaining light for refuellable lights
 */
static size_t obj_desc_light(const struct object *obj, char *buf, size_t max,
							 size_t end)
{
	/* Fuelled light sources get number of remaining turns appended */
	if (tval_is_light(obj) && !of_has(obj->flags, OF_NO_FUEL))
		// strnfcat(buf, max, &end, " (%d turns)", obj->timeout);
		strnfcat(buf, max, &end, " (%d ход%s)", obj->timeout, PLURAL_RU(obj->timeout, "", "а", "ов"));

	return end;
}

/**
 * Describe numerical modifiers to stats and other player qualities which
 * allow numerical bonuses - speed, stealth, etc
 */
static size_t obj_desc_mods(const struct object *obj, char *buf, size_t max,
							size_t end)
{
	int i, j, num_mods = 0;
	int mods[OBJ_MOD_MAX] = { 0 };

	/* Run through possible modifiers and store distinct ones */
	for (i = 0; i < OBJ_MOD_MAX; i++) {
		/* Check for known non-zero mods */
		if (obj->modifiers[i] != 0) {
			/* If no mods stored yet, store and move on */
			if (!num_mods) {
				mods[num_mods++] = obj->modifiers[i];
				continue;
			}

			/* Run through the existing mods, quit on duplicates */
			for (j = 0; j < num_mods; j++)
				if (mods[j] == obj->modifiers[i]) break;

			/* Add another mod if needed */
			if (j == num_mods)
				mods[num_mods++] = obj->modifiers[i];
		}
	}

	if (!num_mods) return end;

	/* Print the modifiers */
	strnfcat(buf, max, &end, " <");
	for (j = 0; j < num_mods; j++) {
		if (j) strnfcat(buf, max, &end, ", ");
		strnfcat(buf, max, &end, "%+d", mods[j]);
	}
	strnfcat(buf, max, &end, ">");

	return end;
}

/**
 * Describe charges or charging status for re-usable items with magic effects
 */
static size_t obj_desc_charges(const struct object *obj, char *buf, size_t max,
		size_t end, uint32_t mode)
{
	bool aware = object_flavor_is_aware(obj) || (mode & ODESC_STORE);

	/* Wands and staffs have charges, others may be charging */
	if (aware && tval_can_have_charges(obj)) {
		// strnfcat(buf, max, &end, " (%d charge%s)", obj->pval,
		strnfcat(buf, max, &end, " (%d заряд%s)", obj->pval,
				 // PLURAL(obj->pval));
				 PLURAL_RU(obj->pval, "", "а", "ов"));
	} else if (obj->timeout > 0) {
		if (tval_is_rod(obj) && obj->number > 1)
			// strnfcat(buf, max, &end, " (%d charging)", number_charging(obj));
			strnfcat(buf, max, &end, " (%d заряжается)", number_charging(obj));
		else if (tval_is_rod(obj) || obj->activation || obj->effect)
			/* Artifacts, single rods */
			// strnfcat(buf, max, &end, " (charging)");
			strnfcat(buf, max, &end, " (заряжается)");
	}

	return end;
}

/**
 * Add player-defined inscriptions or game-defined descriptions
 */
static size_t obj_desc_inscrip(const struct object *obj, char *buf,
		size_t max, size_t end, const struct player *p)
{
	const char *u[6] = { 0, 0, 0, 0, 0, 0 };
	int n = 0;

	/* Get inscription */
	if (obj->note)
		u[n++] = quark_str(obj->note);

	/* Use special inscription, if any */
	if (!object_flavor_is_aware(obj)) {
		if (tval_can_have_charges(obj) && (obj->pval == 0))
			u[n++] = "empty";
		if (object_flavor_was_tried(obj))
			u[n++] = "tried";
	}

	/* Note curses */
	if (obj->known->curses)
		u[n++] = "cursed";

	/* Note ignore */
	if (p && ignore_item_ok(p, obj))
		u[n++] = "ignore";

	/* Note unknown properties */
	if (!object_runes_known(obj) && (obj->known->notice & OBJ_NOTICE_ASSESSED))
		u[n++] = "??";

	if (n) {
		int i;
		for (i = 0; i < n; i++) {
			if (i == 0)
				strnfcat(buf, max, &end, " {");
			strnfcat(buf, max, &end, "%s", u[i]);
			if (i < n - 1)
				strnfcat(buf, max, &end, ", ");
		}

		strnfcat(buf, max, &end, "}");
	}

	return end;
}


/**
 * Add "unseen" to the end of unaware items in stores,
 * and "??" to not fully known unflavored items 
 */
static size_t obj_desc_aware(const struct object *obj, char *buf, size_t max,
							 size_t end)
{
	if (!object_flavor_is_aware(obj)) {
		// strnfcat(buf, max, &end, " {unseen}");
		strnfcat(buf, max, &end, " {неизв}");
	} else if (!object_runes_known(obj)) {
		strnfcat(buf, max, &end, " {??}");
	} else if (obj->known->curses) {
		// strnfcat(buf, max, &end, " {cursed}");
		strnfcat(buf, max, &end, " {прокл}");
	}

	return end;
}


/**
 * Describes item `obj` into buffer `buf` of size `max`.
 *
 * \param buf is the buffer for the description.  Must have space for at least
 * max bytes.
 * \param max is the size of the buffer, in bytes.
 * \param obj is the object to describe.
 * \param mode must be a bitwise-or of zero or one more of the following:
 * ODESC_PREFIX prepends a 'the', 'a' or number
 * ODESC_BASE results in a base description.
 * ODESC_COMBAT will add to-hit, to-dam and AC info.
 * ODESC_EXTRA will add pval/charge/inscription/ignore info.
 * ODESC_PLURAL will pluralise regardless of the number in the stack.
 * ODESC_STORE turns off ignore markers, for in-store display.
 * ODESC_SPOIL treats the object as fully identified.
 * ODESC_CAPITAL capitalises the object name.
 * ODESC_TERSE causes a terse name to be used.
 * ODESC_NOEGO omits ego names.
 * ODESC_ALTNUM causes the high 16 bits of mode to be used as the number
 * of objects instead of using obj->number.  Note that using ODESC_ALTNUM
 * is not fully compatible with ODESC_EXTRA:  the display of number of rods
 * charging does not account for the alternate number.
 * \param p is the player whose knowledge is factored into the description.
 * If p is NULL, the description is for an omniscient observer.
 *
 * \returns The number of bytes used of the buffer.
 */
size_t object_desc(char *buf, size_t max, const struct object *obj,
		uint32_t mode, const struct player *p)
{
	bool prefix = mode & ODESC_PREFIX ? true : false;
	bool spoil = mode & ODESC_SPOIL ? true : false;
	bool terse = mode & ODESC_TERSE ? true : false;

	size_t end = 0;

	/* Simple description for null item */
	if (!obj || !obj->known)
		// return strnfmt(buf, max, "(nothing)");
		return strnfmt(buf, max, "(ничего)");

	/* Unknown itema and cash get straightforward descriptions */
	if (obj->known && obj->kind != obj->known->kind) {
		// if (prefix)
			// return strnfmt(buf, max, "an unknown item");
		// return strnfmt(buf, max, "unknown item");
		return strnfmt(buf, max, "неизвестный предмет");
	}

	if (tval_is_money(obj))
		// return strnfmt(buf, max, "%d gold pieces worth of %s%s",
		return strnfmt(buf, max, "%d золот%s монет%s %s%s", 
				// obj->pval, obj->kind->name,
				obj->pval, PLURAL_RU(obj->pval, "ую", "ые", "ых"), 
				PLURAL_RU(obj->pval, "у", "ы", ""), obj->kind->name,
				// ignore_item_ok(p, obj) ? " {ignore}" : "");
				ignore_item_ok(p, obj) ? " {игнор}" : "");

	/* Egos and kinds whose name we know are seen */
	if (obj->known->ego && !spoil)
		obj->ego->everseen = true;

	if (object_flavor_is_aware(obj) && !spoil)
		obj->kind->everseen = true;

	/** Construct the name **/

	/* Copy the base name to the buffer */
	end = obj_desc_name(buf, max, end, obj, prefix, mode, terse, p);

	/* Combat properties */
	if (mode & ODESC_COMBAT) {
		if (tval_is_chest(obj))
			end = obj_desc_chest(obj, buf, max, end);
		else if (tval_is_light(obj))
			end = obj_desc_light(obj, buf, max, end);

		end = obj_desc_combat(obj->known, buf, max, end, mode, p);
	}

	/* Modifiers, charges, flavour details, inscriptions */
	if (mode & ODESC_EXTRA) {
		end = obj_desc_mods(obj->known, buf, max, end);

		end = obj_desc_charges(obj, buf, max, end, mode);

		if (mode & ODESC_STORE)
			end = obj_desc_aware(obj, buf, max, end);
		else
			end = obj_desc_inscrip(obj, buf, max, end, p);
	}

	return end;
}
