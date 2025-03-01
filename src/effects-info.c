/**
 * \file effects-info.c
 * \brief Implement interfaces for displaying information about effects
 *
 * Copyright (c) 2020 Eric Branlund
 * Copyright (c) 2010 Andi Sidwell
 * Copyright (c) 2004 Robert Ruehlmann
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

#include "effects-info.h"
#include "effects.h"
#include "init.h"
#include "message.h"
#include "mon-summon.h"
#include "obj-info.h"
#include "player-timed.h"
#include "project.h"
#include "z-color.h"
#include "z-form.h"
#include "z-util.h"


static struct {
        int index;
        int args;
        int efinfo_flag;
        const char *desc;
	const char *menu_name;
} base_descs[] = {
        { EF_NONE, 0, EFINFO_NONE, "", "" },
        #define EFFECT(x, a, b, c, d, e, f) { EF_##x, c, d, e, f },
        #include "list-effects.h"
        #undef EFFECT
};


/**
 * Get the possible dice strings.
 */
static void format_dice_string(const random_value *v, int multiplier,
	size_t len, char* dice_string)
{
	if (v->dice && v->base) {
		if (multiplier == 1) {
			// strnfmt(dice_string, len, "%d+%dd%d", v->base, v->dice,
			strnfmt(dice_string, len, "%d+%dк%d", v->base, v->dice,
				v->sides);
		} else {
			// strnfmt(dice_string, len, "%d+%d*(%dd%d)",
			strnfmt(dice_string, len, "%d+%d*(%dк%d)",
				multiplier * v->base, multiplier, v->dice,
				v->sides);
		}
	} else if (v->dice) {
		if (multiplier == 1) {
			// strnfmt(dice_string, len, "%dd%d", v->dice, v->sides);
			strnfmt(dice_string, len, "%dк%d", v->dice, v->sides);
		} else {
			// strnfmt(dice_string, len, "%d*(%dd%d)", multiplier,
			strnfmt(dice_string, len, "%d*(%dк%d)", multiplier,
				v->dice, v->sides);
		}
	} else {
		strnfmt(dice_string, len, "%d", multiplier * v->base);
	}
}


/**
 * Appends a message describing the magical device skill bonus and the average
 * damage. Average damage is only displayed if there is variance or a magical
 * device bonus.
 */
static void append_damage(char *buffer, size_t buffer_size, random_value value,
	int dev_skill_boost)
{
	if (dev_skill_boost != 0) {
		// my_strcat(buffer, format(", which your device skill increases by %d%%",
		my_strcat(buffer, format(", в результате ваш навык механизмов увеличивается на %d%%",
			dev_skill_boost), buffer_size);
	}

	if (randcalc_varies(value) || dev_skill_boost > 0) {
		// Ten times the average damage, for 1 digit of precision
		int dam = (100 + dev_skill_boost) * randcalc(value, 0, AVERAGE) / 10;
		// my_strcat(buffer, format(" for an average of %d.%d damage", dam / 10,
		my_strcat(buffer, format(" в среднем %d.%d урона", dam / 10,
			dam % 10), buffer_size);
	}
}


static void copy_to_textblock_with_coloring(textblock *tb, const char *s)
{
	// для русского языка
	char desc[512];
	int i = 0;
	
	while (*s) {
		if (isdigit((unsigned char) *s)) {
			textblock_append(tb, "%s", desc);
			textblock_append_c(tb, COLOUR_L_GREEN, "%c", *s);
			memset(desc, 0, i);
			i = 0;
		} else {
			// textblock_append(tb, "%c", *s);
			desc[i++] = *s;
			desc[i] = 0;
		}
		++s;
	}
	textblock_append(tb, "%s", desc);
}


/**
 * Creates a description of the random or select effect which chooses from the
 * next count effects in the linked list starting with e.  The description is
 * prefaced with the contents of *prefix if prefix is not NULL.  It is also
 * prefaced with the contents of *type_prefix which would normally be
 * "randomly " for a random effect and NULL for a select effect.
 * dev_skill_boost is the percent increase in damage to report for the device
 * skill.  Sets *nexte to point to the element in the linked list or NULL that
 * is immediately after the count effects.  Returns a non-NULL value if there
 * was at least one effect that could be described.  Otherwise, returns NULL.
 */
static textblock *create_nested_effect_description(const struct effect *e,
	int count, const char *prefix, const char *type_prefix,
	int dev_skill_boost, const struct effect **nexte)
{
	/*
	 * Do one pass through the effects to determine if they are of all the
	 * the same basic type.  That is used to condense the description in
	 * the case where all are breaths.  Ignore random nested effects since
	 * they will do nothing when the outer random effect is processed with
	 * effect_do().
	 */
	textblock *res = NULL;
	const struct effect *efirst;
	const dice_t *first_dice;
	int first_ind, first_other;
	random_value first_rv = { 0, 0, 0, 0 };
	bool same_ind, same_other, same_dice;
	int irand, jrand;
	int nvalid;

	/* Find the first effect that's valid and not random nor select. */
	irand = 0;
	while (1) {
		if (!e || irand >= count) {
			/*
			 * There's no valid or non-random effects; do nothing.
			 */
			*nexte = e;
			return false;
		}
		if (effect_desc(e) && e->index != EF_RANDOM &&
				e->index != EF_SELECT) {
			break;
		}
		e = e->next;
		++irand;
	}

	efirst = e;
	first_ind = e->index;
	first_other = e->other;
	first_dice = e->dice;
	if (e->dice) {
		dice_random_value(e->dice, &first_rv);
	}

	nvalid = 1;
	same_ind = true;
	same_other = true;
	same_dice = true;
	for (e = efirst->next, jrand = irand + 1;
		e && jrand < count;
		e = e->next, ++jrand) {
		if (!effect_desc(e) || e->index == EF_RANDOM ||
				e->index == EF_SELECT) {
			continue;
		}
		++nvalid;
		if (e->index != first_ind) {
			same_ind = false;
		}
		if (e->other != first_other) {
			same_other = false;
		}
		if (e->dice) {
			if (first_dice) {
				random_value this_rv;

				dice_random_value(e->dice, &this_rv);
				if (this_rv.base != first_rv.base ||
					this_rv.dice != first_rv.dice ||
					this_rv.sides != first_rv.sides ||
					this_rv.m_bonus != first_rv.m_bonus) {
					same_dice = false;
				}
			} else {
				same_dice = false;
			}
		} else if (first_dice) {
			same_dice = false;
		}
	}
	*nexte = e;

	if (same_ind && base_descs[first_ind].efinfo_flag == EFINFO_BREATH &&
		same_dice && same_other) {
		/* Concatenate the list of possible elements. */
		// char breaths[120], dice_string[20], desc[200];
		char breaths[256], dice_string[20], desc[512];
		int ivalid;

		strnfmt(breaths, sizeof(breaths), "%s",
			projections[efirst->subtype].player_desc);
		ivalid = 1;
		for (e = efirst->next, jrand = irand + 1;
			e && jrand < count;
			e = e->next, ++jrand) {
			if (!effect_desc(e) || e->index == EF_RANDOM ||
					e->index == EF_SELECT) {
				continue;
			}
			if (ivalid == nvalid - 1) {
				my_strcat(breaths,
					// (nvalid > 2) ? ", or " : " or ",
					(nvalid > 2) ? ", или " : " или ",
					sizeof(breaths));
			} else {
				my_strcat(breaths, ", ", sizeof(breaths));
			}
			my_strcat(breaths, projections[e->subtype].player_desc,
				sizeof(breaths));
			++ivalid;
		}

		/* Then use that in the effect description. */
		format_dice_string(&first_rv, 1, sizeof(dice_string),
			dice_string);
		strnfmt(desc, sizeof(desc), effect_desc(efirst), breaths,
			efirst->other, dice_string);
		append_damage(desc, sizeof(desc), first_rv,
			efirst->index == EF_BREATH ? 0 : dev_skill_boost);

		res = textblock_new();
		if (prefix) {
			textblock_append(res, "%s", prefix);
		}
		if (type_prefix) {
			textblock_append(res, "%s", type_prefix);
		}
		copy_to_textblock_with_coloring(res, desc);
	} else {
		/* Concatenate the effect descriptions. */
		textblock *tb;
		int ivalid;
		
		tb = effect_describe(efirst, type_prefix, dev_skill_boost,
			true);
		if (tb) {
			ivalid = 1;
			if (prefix) {
				res = textblock_new();
				textblock_append(res, "%s", prefix);
				textblock_append_textblock(res, tb);
				textblock_free(tb);
			} else {
				res = tb;
			}
		} else {
			ivalid = 0;
			--nvalid;
		}
		for (e = efirst->next, jrand = irand + 1;
			e && jrand < count;
			e = e->next, ++jrand) {
			if (!effect_desc(e) || e->index == EF_RANDOM ||
					e->index == EF_SELECT) {
				continue;
			}
			tb = effect_describe(e,
				(ivalid == 0) ? type_prefix : NULL,
				dev_skill_boost, true);
			if (!tb) {
				--nvalid;
				continue;
			}
			if (prefix && ! res) {
				assert(ivalid == 0);
				res = textblock_new();
				textblock_append(res, "%s", prefix);
			}
			if (res) {
				if (ivalid > 0) {
					textblock_append(res,
						(ivalid == nvalid - 1) ?
						// " or " : ", ");
						" или " : ", ");
				}
				textblock_append_textblock(res, tb);
				textblock_free(tb);
			} else {
				res = tb;
			}
			++ivalid;
		}
	}

	return res;
}


/**
 * Creates a new textblock which has a description of the effect in *e (and
 * any linked to it because e->index == EF_RANDOM or e->index == EF_SELECT) if
 * only_first is true or has a description of *e and all the subsequent effects
 * if only_first is false.  If none of the effects has a description, will
 * return NULL.  If there is at least one effect with a description and prefix
 * is not NULL, the string pointed to by prefix will be added to the textblock
 * before the descriptions.  dev_skill_boost is the percent increase from the
 * device skill to show in the descriptions.
 */
textblock *effect_describe(const struct effect *e, const char *prefix,
	int dev_skill_boost, bool only_first)
{
	textblock *tb = NULL;
	int nadded = 0;
	// char desc[250];
	char desc[512];
	random_value value = { 0, 0, 0, 0 };
	bool value_set = false;

	while (e) {
		const char* edesc = effect_desc(e);
		int roll = 0;
		char dice_string[20];

		/* Deal with special clear value effect. */
		if (e->index == EF_CLEAR_VALUE) {
			assert(value_set);
			value_set = false;
			e = e->next;
			continue;
		}

		/* Deal with special set value effect. */
		if (e->index == EF_SET_VALUE) {
			assert(e->dice != NULL);
			roll = dice_roll(e->dice, &value);
			value_set = true;
			e = e->next;
			continue;
		}

		if ((e->dice != NULL) && !value_set) {
			roll = dice_roll(e->dice, &value);
		}

		/* Deal with special random or select effects. */
		if (e->index == EF_RANDOM || e->index == EF_SELECT) {
			const struct effect *nexte;
			textblock *tbe = create_nested_effect_description(
				e->next, roll, (nadded == 0) ? prefix : NULL,
				// (e->index == EF_RANDOM) ? "randomly " : NULL,
				(e->index == EF_RANDOM) ? "случайно " : NULL,
				dev_skill_boost, &nexte);

			e = (only_first) ? NULL : nexte;
			if (tbe) {
				if (tb) {
					textblock_append(tb,
						// e ? ", " : " and ");
						e ? ", " : " и ");
					textblock_append_textblock(tb, tbe);
					textblock_free(tbe);
				} else {
					tb = tbe;
				}
				++nadded;
			}
			continue;
		}

		if (!edesc) {
			e = (only_first) ? NULL : e->next;
			continue;
		}

		format_dice_string(&value, 1, sizeof(dice_string), dice_string);

		/* Check all the possible types of description format. */
		switch (base_descs[e->index].efinfo_flag) {
		case EFINFO_DICE:
			strnfmt(desc, sizeof(desc), edesc, dice_string);
			break;

		case EFINFO_HEAL:
			/* Healing sometimes has a minimum percentage. */
			{
				// char min_string[50];
				char min_string[100];

				if (value.m_bonus) {
					strnfmt(min_string, sizeof(min_string),
						// " (or %d%%, whichever is greater)",
						" (или %d%%, в зависимости чего больше)",
						value.m_bonus);
				} else {
					strnfmt(min_string, sizeof(min_string),
						"%s", "");
				}
				strnfmt(desc, sizeof(desc), edesc, dice_string,
					min_string);
			}
			break;

		case EFINFO_CONST:
			strnfmt(desc, sizeof(desc), edesc, value.base / 2);
			break;

		case EFINFO_FOOD:
			{
				const char *fed = e->subtype ?
					// (e->subtype == 1 ? "uses enough food value" : 
					(e->subtype == 1 ? "расходует достаточное количество пищи" : 
					 // "leaves you nourished") : "feeds you";
					 "делает вас сытым") : "питает вас";
				char turn_dice_string[20];

				format_dice_string(&value, z_info->food_value,
					sizeof(turn_dice_string),
					turn_dice_string);

				strnfmt(desc, sizeof(desc), edesc, fed,
					turn_dice_string, dice_string);
			}
			break;

		case EFINFO_CURE:
			strnfmt(desc, sizeof(desc), edesc,
				timed_effects[e->subtype].desc);
			break;

		case EFINFO_TIMED:
			strnfmt(desc, sizeof(desc), edesc,
				timed_effects[e->subtype].desc,
				dice_string);
			break;

		case EFINFO_STAT:
			{
				int stat = e->subtype;

				strnfmt(desc, sizeof(desc), edesc,
					lookup_obj_property(OBJ_PROPERTY_STAT, stat)->name);
			}
			break;

		case EFINFO_SEEN:
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].desc);
			break;

		case EFINFO_SUMM:
			strnfmt(desc, sizeof(desc), edesc,
				summon_desc(e->subtype));
			break;

		case EFINFO_TELE:
			/*
			 * Only currently used for the player, but can handle
			 * monsters.
			 */
			{
				// char dist[32];
				char dist[80];

				if (value.m_bonus) {
					strnfmt(dist, sizeof(dist),
						// "a level-dependent distance");
						"уровнезависимое расстояние");
				} else {
					strnfmt(dist, sizeof(dist),
						// "%d grids", value.base);
						"%d шагов", value.base);
				}
				strnfmt(desc, sizeof(desc), edesc,
					// (e->subtype) ? "a monster" : "you",
					(e->subtype) ? "монстра" : "вас",
					dist);
			}
			break;

		case EFINFO_QUAKE:
			strnfmt(desc, sizeof(desc), edesc, e->radius);
			break;

		case EFINFO_BALL:
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].player_desc,
				e->radius, dice_string);
			append_damage(desc, sizeof(desc), value, dev_skill_boost);
			break;

		case EFINFO_SPOT:
			{
				int i_radius = e->other ? e->other : e->radius;

				strnfmt(desc, sizeof(desc), edesc,
					projections[e->subtype].player_desc,
					e->radius, i_radius, dice_string);
				append_damage(desc, sizeof(desc), value, dev_skill_boost);
			}
			break;

		case EFINFO_BREATH:
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].player_desc, e->other,
				dice_string);
			append_damage(desc, sizeof(desc), value,
				e->index == EF_BREATH ? 0 : dev_skill_boost);
			break;

		case EFINFO_SHORT:
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].player_desc,
				e->radius +
					(e->other ? player->lev / e->other : 0),
				dice_string);
			break;

		case EFINFO_LASH:
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].lash_desc, e->subtype);
			break;

		case EFINFO_BOLT:
			/* Bolt that inflict status */
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].desc);
			break;

		case EFINFO_BOLTD:
			/* Bolts and beams that damage */
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].desc, dice_string);
			append_damage(desc, sizeof(desc), value, dev_skill_boost);
			break;

		case EFINFO_TOUCH:
			strnfmt(desc, sizeof(desc), edesc,
				projections[e->subtype].desc);
			break;

		case EFINFO_NONE:
			strnfmt(desc, sizeof(desc), "%s", edesc);
			break;

		default:
			strnfmt(desc, sizeof(desc), "%s", "");
			msg("Bad effect description passed to effect_info().  Please report this bug.");
			break;
		}

		e = (only_first) ? NULL : e->next;

		if (desc[0] != '\0') {
			if (tb) {
				if (e) {
					textblock_append(tb, ", ");
				} else {
					// textblock_append(tb, " and ");
					textblock_append(tb, " и ");
				}
			} else {
				tb = textblock_new();
				if (prefix) {
					textblock_append(tb, "%s", prefix);
				}
			}
			copy_to_textblock_with_coloring(tb, desc);

			++nadded;
		}
	}

	return tb;
}

/**
 * Fills a buffer with a short description, suitable for use a menu entry, of
 * an effect.
 * \param buf is the buffer to fill.
 * \param max is the maximum number of characters the buffer can hold.
 * \param e is the effect to describe.
 * \return the number of characters written to the buffer; will be zero if
 * the effect is invalid
 */
size_t effect_get_menu_name(char *buf, size_t max, const struct effect *e)
{
	const char *fmt;
	size_t len;

	if (!e || e->index <= EF_NONE || e->index >= EF_MAX) {
		return 0;
	}

	fmt = base_descs[e->index].menu_name;
	switch (base_descs[e->index].efinfo_flag) {
	case EFINFO_DICE:
	case EFINFO_HEAL:
	case EFINFO_CONST:
	case EFINFO_QUAKE:
	case EFINFO_NONE:
		len = strnfmt(buf, max, "%s", fmt);
		break;

	case EFINFO_FOOD:
		{
			const char *actstr;
			const char *actarg;
			int avg;

			switch (e->subtype) {
			case 0: /* INC_BY */
				actstr = "feed";
				actarg = "yourself";
				break;
			case 1: /* DEC_BY */
				actstr = "increase";
				actarg = "hunger";
				break;
			case 2: /* SET_TO */
				avg = (e->dice) ?
					dice_evaluate(e->dice, 1, AVERAGE, NULL) : 0;
				actstr = "become";
				if (avg > PY_FOOD_FULL) {
					actarg = "bloated";
				} else if (avg > PY_FOOD_HUNGRY) {
					actarg = "satisfied";
				} else {
					actarg = "hungry";
				}
				break;
			case 3: /* INC_TO */
				avg = (e->dice) ?
					dice_evaluate(e->dice, 1, AVERAGE, NULL): 0;
				actstr = "leave";
				if (avg > PY_FOOD_FULL) {
					actarg = "bloated";
				} else if (avg > PY_FOOD_HUNGRY) {
					actarg = "nourished";
				} else {
					actarg = "hungry";
				}
				break;
			default:
				actstr = NULL;
				actarg = NULL;
				break;
			}
			if (actstr && actarg) {
				len = strnfmt(buf, max, fmt, actstr, actarg);
			} else {
				len = strnfmt(buf, max, "%s", "");
			}
		}
		break;

	case EFINFO_CURE:
	case EFINFO_TIMED:
		len = strnfmt(buf, max, fmt, timed_effects[e->subtype].desc);
		break;

	case EFINFO_STAT:
		len = strnfmt(buf, max, fmt,
			lookup_obj_property(OBJ_PROPERTY_STAT,
			e->subtype)->name);
		break;

	case EFINFO_SEEN:
	case EFINFO_BOLT:
	case EFINFO_BOLTD:
	case EFINFO_TOUCH:
		len = strnfmt(buf, max, fmt, projections[e->subtype].desc);
		break;

	case EFINFO_SUMM:
		len = strnfmt(buf, max, fmt, summon_desc(e->subtype));
		break;

	case EFINFO_TELE:
		{
			random_value value = { 0, 0, 0, 0 };
			// char dist[32];
			char dist[80];
			int avg = 0;

			if (e->dice) {
				avg = dice_evaluate(e->dice, 1, AVERAGE,
					&value);
			}
			if (value.m_bonus) {
				// strnfmt(dist, sizeof(dist), "some distance");
				strnfmt(dist, sizeof(dist), "некоторое расстояние");
			} else {
				// strnfmt(dist, sizeof(dist), "%d grids", avg);
				strnfmt(dist, sizeof(dist), "%d шагов", avg);
			}
			len = strnfmt(buf, max, fmt,
				// (e->subtype) ? "other" : "you", dist);
				(e->subtype) ? "других" : "вас", dist);
		}
		break;

	case EFINFO_BALL:
	case EFINFO_SPOT:
	case EFINFO_BREATH:
	case EFINFO_SHORT:
		len = strnfmt(buf, max, fmt,
			projections[e->subtype].player_desc);
		break;

	case EFINFO_LASH:
		len = strnfmt(buf, max, fmt, projections[e->subtype].lash_desc);
		break;

	default:
		len = strnfmt(buf, max, "%s", "");
		msg("Bad effect description passed to effect_get_menu_name().  Please report this bug.");
		break;
	}

	return len;
}

/**
 * Returns a pointer to the next effect in the effect stack, skipping over
 * all the sub-effects from random or select effects
 */
struct effect *effect_next(struct effect *effect)
{
	if (effect->index == EF_RANDOM || effect->index == EF_SELECT) {
		struct effect *e = effect;
		int num_subeffects = MAX(0,
			dice_evaluate(effect->dice, 0, AVERAGE, NULL));
		// Skip all the sub-effects, plus one to advance beyond current
		for (int i = 0; e != NULL && i < num_subeffects + 1; i++) {
			e = e->next;
		}
		return e;
	} else {
		return effect->next;
	}
}

/**
 * Checks if the effect deals damage, by checking the effect's info string.
 * Random or select effects are considered to deal damage if any sub-effect
 * deals damage.
 */
bool effect_damages(const struct effect *effect)
{
	if (effect->index == EF_RANDOM || effect->index == EF_SELECT) {
		// Random or select effect
		struct effect *e = effect->next;
		int num_subeffects = dice_evaluate(effect->dice, 0, AVERAGE, NULL);

		// Check if any of the subeffects do damage
		for (int i = 0; e != NULL && i < num_subeffects; i++) {
			if (effect_damages(e)) {
				return true;
			}
			e = e->next;
		}
		return false;
	} else {
		// Not a random or select effect, check the info string for
		// damage
		return effect_info(effect) != NULL &&
			streq(effect_info(effect), "dam");
	}
}

/**
 * Calculates the average damage of the effect. Random effects and select
 * effects return an average of all sub-effect averages.
 *
 * \param effect is the effect to evaluate.
 * \param shared_dice is the dice set by a prior SET_VALUE effect.  Use
 * NULL if there wasn't a prior SET_VALUE effect to set the dice.
 */
int effect_avg_damage(const struct effect *effect, const dice_t *shared_dice)
{
	if (effect->index == EF_RANDOM || effect->index == EF_SELECT) {
		// Random or select effect, check the sub-effects to
		// accumulate damage
		int total = 0;
		struct effect *e = effect->next;
		int n_stated = dice_evaluate((shared_dice) ?
			shared_dice : effect->dice, 0, AVERAGE, NULL);
		int n_actual = 0;

		for (int i = 0; e != NULL && i < n_stated; i++) {
			total += effect_avg_damage(e, shared_dice);
			++n_actual;
			e = e->next;
		}
		// Return an average of the sub-effects' average damages
		return (n_actual > 0) ? total / n_actual : 0;
	} else if (effect_damages(effect)) {
		// Non-random effect, calculate the average damage
		return dice_evaluate((shared_dice) ?
			shared_dice : effect->dice, 0, AVERAGE, NULL);
	}
	return 0;
}

/**
 * Returns the projection of the effect, or an empty string if it has none.
 * Random or select effects only return a projection if all sub-effects have
 * the same projection.
 */
const char *effect_projection(const struct effect *effect)
{
	if (effect->index == EF_RANDOM || effect->index == EF_SELECT) {
		// Random or select effect
		int num_subeffects = dice_evaluate(effect->dice, 0, AVERAGE, NULL);
		struct effect *e;
		const char *subeffect_proj;

		// Check if all subeffects have the same projection, and if
		// not just give up on it
		if (num_subeffects <= 0 || !effect->next) {
			return "";
		}

		e = effect->next;
		subeffect_proj = effect_projection(e);
		for (int i = 0; e != NULL && i < num_subeffects; i++) {
			if (!streq(subeffect_proj, effect_projection(e))) {
				return "";
			}
			e = e->next;
		}

		return subeffect_proj;
	} else if (projections[effect->subtype].player_desc != NULL) {
		// Non-random effect, extract the projection if there is one
		switch (base_descs[effect->index].efinfo_flag) {
			case EFINFO_BALL:
			case EFINFO_BOLTD:
			case EFINFO_BREATH:
			case EFINFO_SHORT:
			case EFINFO_SPOT:
				return projections[effect->subtype].player_desc;
		}
	}

	return "";
}

/**
 * Help effect_summarize_properties() and summarize_cure():  add one element
 * to the linked list of object properties.
 */
static void add_to_summaries(struct effect_object_property **summaries,
		int idx, int reslevel_min, int reslevel_max,
		enum effect_object_property_kind kind)
{
	struct effect_object_property *prop = mem_alloc(sizeof(*prop));

	prop->next = *summaries;
	prop->idx = idx;
	prop->reslevel_min = reslevel_min;
	prop->reslevel_max = reslevel_max;
	prop->kind = kind;
	*summaries = prop;
}

/**
 * Help effect_summarize_properties():  update the summaries for an effect that
 * acts like a cure.
 * \param tmd Is the TMD_* index for the timed effect being cured.
 * \param summaries Is the pointer to the linked list of summaries.
 * \param unsummarized_count Is the count of unsummarized effects.
 */
static void summarize_cure(int tmd, struct effect_object_property **summaries,
		int *unsummarized_count)
{
	const struct timed_failure *f = timed_effects[tmd].fail;

	while (f) {
		if (f->code == TMD_FAIL_FLAG_OBJECT) {
			add_to_summaries(summaries, f->idx, 0, 0,
				EFPROP_CURE_FLAG);
		} else if (f->code == TMD_FAIL_FLAG_RESIST) {
			add_to_summaries(summaries, f->idx, -1, 0,
				EFPROP_CURE_RESIST);
		} else {
			++*unsummarized_count;
		}
		f = f->next;
	}
}

/**
 * Return a summary of the object properties that match up with the effects in
 * an effect chain.
 * \param ef Is the pointer to the first effect in the chain.
 * \param unsummarized_count If not NULL, *unsummarized_count will be set to
 * the count of effects in the chain that do something which can't be summarized
 * by an object property.
 * \return Return a pointer to a linked list of the object properties implied
 * by the effect chain.  When no longer needed, each element of that linked
 * list should be released with mem_free().
 */
struct effect_object_property *effect_summarize_properties(
		const struct effect *ef, int *unsummarized_count)
{
	int unsummarized = 0;
	struct effect_object_property *summaries = NULL;
	dice_t *remembered_dice = NULL;

	for (; ef; ef = ef->next) {
		int value_this;

		switch (ef->index) {
		case EF_RANDOM:
		case EF_SELECT:
			/*
			 * For random or select effects, summarize all of the
			 * subeffects since any of them is possible.  That's
			 * equivalent to simply skipping over the random or
			 * select effect and stepping one by one through what
			 * follows.
			 */
			break;

		case EF_SET_VALUE:
			/*
			 * Remember the value.  Does nothing that should be
			 * remembered in the summaries or unsummarized count.
			 */
			remembered_dice = ef->dice;
			break;

		case EF_CLEAR_VALUE:
			/*
			 * Forget the value.  Does nothing that should be
			 * remembered in the summaries or unsummarized count.
			 */
			remembered_dice = NULL;
			break;

		case EF_CURE:
			if (ef->subtype >= 0 && ef->subtype < TMD_MAX) {
				summarize_cure(ef->subtype, &summaries,
					&unsummarized);
			}
			break;

		case EF_TIMED_SET:
			value_this = (remembered_dice) ?
				dice_evaluate(remembered_dice, 0, MAXIMISE, NULL) :
				((ef->dice) ?
				dice_evaluate(ef->dice, 0, MAXIMISE, NULL) : 0);
			if (value_this <= 0 && ef->subtype >= 0 &&
					ef->subtype < TMD_MAX) {
				/* It's equivalent to a cure. */
				summarize_cure(ef->subtype, &summaries,
					&unsummarized);
				break;
			}
			/* Fall through. */

		case EF_TIMED_INC:
		case EF_TIMED_INC_NO_RES:
			value_this = (remembered_dice) ?
				dice_evaluate(remembered_dice, 0, MAXIMISE, NULL) :
				((ef->dice) ?
				dice_evaluate(ef->dice, 0, MAXIMISE, NULL) : 0);
			if (value_this > 0 && ef->subtype >= 0 &&
					ef->subtype < TMD_MAX) {
				bool summarized = false;
				const struct timed_failure *f;

				if (timed_effects[ef->subtype].oflag_dup !=
						OF_NONE) {
					add_to_summaries(&summaries,
						timed_effects[ef->subtype].oflag_dup,
						0, 0,
						timed_effects[ef->subtype].oflag_syn ?
						EFPROP_OBJECT_FLAG_EXACT : EFPROP_OBJECT_FLAG);
					summarized = true;
				}
				if (timed_effects[ef->subtype].temp_resist >= 0) {
					int rmin = -1, rmax = 1;

					f = timed_effects[ef->subtype].fail;
					while (f) {
						if (f->idx == timed_effects[ef->subtype].temp_resist) {
							if (f->code == TMD_FAIL_FLAG_RESIST) {
								rmax = MIN(rmax, 0);
							} else if (f->code == TMD_FAIL_FLAG_VULN) {
								rmin = MAX(rmin, 0);
							}
						}
						f = f->next;
					}
					add_to_summaries(&summaries,
						timed_effects[ef->subtype].temp_resist,
						rmin, rmax, EFPROP_RESIST);
					summarized = true;
				}
				f = timed_effects[ef->subtype].fail;
				while (f) {
					switch (f->code) {
					case TMD_FAIL_FLAG_OBJECT:
						add_to_summaries(&summaries,
							f->idx, 0, 0,
							EFPROP_CONFLICT_FLAG);
						summarized = true;
						break;

					case TMD_FAIL_FLAG_RESIST:
						if (f->idx != timed_effects[ef->subtype].temp_resist) {
							add_to_summaries(
								&summaries,
								f->idx, -1, 0,
								EFPROP_CONFLICT_RESIST);
							summarized = true;
						}
						break;

					case TMD_FAIL_FLAG_VULN:
						if (f->idx != timed_effects[ef->subtype].temp_resist) {
							add_to_summaries(
								&summaries,
								f->idx, 0, 3,
								EFPROP_CONFLICT_VULN);
							summarized = true;
						}
						break;

					default:
						/* Nothing special is needed. */
						break;
					}
					f = f->next;
				}
				if (timed_effects[ef->subtype].temp_brand >= 0) {
					add_to_summaries(&summaries,
						timed_effects[ef->subtype].temp_brand,
						0, 0, EFPROP_BRAND);
					summarized = true;
				}
				if (timed_effects[ef->subtype].temp_slay >= 0) {
					add_to_summaries(&summaries,
						timed_effects[ef->subtype].temp_slay,
						0, 0, EFPROP_SLAY);
					summarized = true;
				}
				if (!summarized) ++unsummarized;
			}
			break;

		case EF_TIMED_DEC:
			value_this = (remembered_dice) ?
				dice_evaluate(remembered_dice, 0, MAXIMISE, NULL) :
				((ef->dice) ?
				dice_evaluate(ef->dice, 0, MAXIMISE, NULL) : 0);
			/* If it decreases the duration, it's a partial cure. */
			if (value_this > 0) {
				summarize_cure(ef->subtype, &summaries,
					&unsummarized);
			}
			break;

		case EF_TELEPORT:
		case EF_TELEPORT_TO:
		case EF_TELEPORT_LEVEL:
			add_to_summaries(&summaries, OF_NO_TELEPORT,
				0, 0, EFPROP_CONFLICT_FLAG);
			break;

		/*
		 * There's other effects that have limited utility when the
		 * object already has some flags:
		 * DISABLE_TRAPS with OF_TRAP_IMMUNE is only good for unlocking
		 * DETECT_INVISIBLE with OF_SEE_INVISIBLE or OF_TELEPATHY
		 * RESTORE_x with OF_SUST_x
		 * RESTORE_EXP with OF_HOLD_LIFE
		 * For now, don't try to flag those.
		 */
		default:
			/*
			 * Everything else isn't related to an object property.
			 */
			++unsummarized;
			break;
		}
	}

	if (unsummarized_count) *unsummarized_count = unsummarized;
	return summaries;
}
