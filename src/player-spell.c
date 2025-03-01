/**
 * \file player-spell.c
 * \brief Spell and prayer casting/praying
 *
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
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
#include "cave.h"
#include "cmd-core.h"
#include "effects.h"
#include "init.h"
#include "monster.h"
#include "obj-tval.h"
#include "obj-util.h"
#include "object.h"
#include "player-calcs.h"
#include "player-spell.h"
#include "player-timed.h"
#include "player-util.h"
#include "project.h"
#include "target.h"

/**
 * Used by get_spell_info() to pass information as it iterates through effects.
 */
struct spell_info_iteration_state {
	const struct effect *pre;
	char pre_special[40];
	random_value pre_rv;
	random_value shared_rv;
	bool have_shared;
};

/**
 * Stat Table (INT/WIS) -- Minimum failure rate (percentage)
 */
static const int adj_mag_fail[STAT_RANGE] =
{
	99	/* 3 */,
	99	/* 4 */,
	99	/* 5 */,
	99	/* 6 */,
	99	/* 7 */,
	50	/* 8 */,
	30	/* 9 */,
	20	/* 10 */,
	15	/* 11 */,
	12	/* 12 */,
	11	/* 13 */,
	10	/* 14 */,
	9	/* 15 */,
	8	/* 16 */,
	7	/* 17 */,
	6	/* 18/00-18/09 */,
	6	/* 18/10-18/19 */,
	5	/* 18/20-18/29 */,
	5	/* 18/30-18/39 */,
	5	/* 18/40-18/49 */,
	4	/* 18/50-18/59 */,
	4	/* 18/60-18/69 */,
	4	/* 18/70-18/79 */,
	4	/* 18/80-18/89 */,
	3	/* 18/90-18/99 */,
	3	/* 18/100-18/109 */,
	2	/* 18/110-18/119 */,
	2	/* 18/120-18/129 */,
	2	/* 18/130-18/139 */,
	2	/* 18/140-18/149 */,
	1	/* 18/150-18/159 */,
	1	/* 18/160-18/169 */,
	1	/* 18/170-18/179 */,
	1	/* 18/180-18/189 */,
	1	/* 18/190-18/199 */,
	0	/* 18/200-18/209 */,
	0	/* 18/210-18/219 */,
	0	/* 18/220+ */
};

/**
 * Stat Table (INT/WIS) -- failure rate adjustment
 */
static const int adj_mag_stat[STAT_RANGE] =
{
	-5	/* 3 */,
	-4	/* 4 */,
	-3	/* 5 */,
	-3	/* 6 */,
	-2	/* 7 */,
	-1	/* 8 */,
	 0	/* 9 */,
	 0	/* 10 */,
	 0	/* 11 */,
	 0	/* 12 */,
	 0	/* 13 */,
	 1	/* 14 */,
	 2	/* 15 */,
	 3	/* 16 */,
	 4	/* 17 */,
	 5	/* 18/00-18/09 */,
	 6	/* 18/10-18/19 */,
	 7	/* 18/20-18/29 */,
	 8	/* 18/30-18/39 */,
	 9	/* 18/40-18/49 */,
	10	/* 18/50-18/59 */,
	11	/* 18/60-18/69 */,
	12	/* 18/70-18/79 */,
	15	/* 18/80-18/89 */,
	18	/* 18/90-18/99 */,
	21	/* 18/100-18/109 */,
	24	/* 18/110-18/119 */,
	27	/* 18/120-18/129 */,
	30	/* 18/130-18/139 */,
	33	/* 18/140-18/149 */,
	36	/* 18/150-18/159 */,
	39	/* 18/160-18/169 */,
	42	/* 18/170-18/179 */,
	45	/* 18/180-18/189 */,
	48	/* 18/190-18/199 */,
	51	/* 18/200-18/209 */,
	54	/* 18/210-18/219 */,
	57	/* 18/220+ */
};

/**
 * Initialise player spells
 */
void player_spells_init(struct player *p)
{
	int i, num_spells = p->class->magic.total_spells;

	/* None */
	if (!num_spells) return;

	/* Allocate */
	p->spell_flags = mem_zalloc(num_spells * sizeof(uint8_t));
	p->spell_order = mem_zalloc(num_spells * sizeof(uint8_t));

	/* None of the spells have been learned yet */
	for (i = 0; i < num_spells; i++)
		p->spell_order[i] = 99;
}

/**
 * Free player spells
 */
void player_spells_free(struct player *p)
{
	mem_free(p->spell_flags);
	mem_free(p->spell_order);
}

/**
 * Make a list of the spell realms the player's class has books from
 */
struct magic_realm *class_magic_realms(const struct player_class *c, int *count)
{
	int i;
	struct magic_realm *r = mem_zalloc(sizeof(struct magic_realm));

	*count = 0;

	if (!c->magic.total_spells) {
		mem_free(r);
		return NULL;
	}

	for (i = 0; i < c->magic.num_books; i++) {
		struct magic_realm *r_test = r;
		struct class_book *book = &c->magic.books[i];
		bool found = false;

		/* Test for first realm */
		if (r->name == NULL) {
			memcpy(r, book->realm, sizeof(struct magic_realm));
			r->next = NULL;
			(*count)++;
			continue;
		}

		/* Test for already recorded */
		while (r_test) {
			if (streq(r_test->name, book->realm->name)) {
				found = true;
			}
			r_test = r_test->next;
		}
		if (found) continue;

		/* Add it */
		r_test = mem_zalloc(sizeof(struct magic_realm));
		memcpy(r_test, book->realm, sizeof(struct magic_realm));
		r_test->next = r;
		r = r_test;
		(*count)++;
	}

	return r;
}


/**
 * Get the spellbook structure from any object which is a book
 */
const struct class_book *object_kind_to_book(const struct object_kind *kind)
{
	struct player_class *class = classes;
	while (class) {
		int i;

		for (i = 0; i < class->magic.num_books; i++)
		if ((kind->tval == class->magic.books[i].tval) &&
			(kind->sval == class->magic.books[i].sval)) {
			return &class->magic.books[i];
		}
		class = class->next;
	}

	return NULL;
}

/**
 * Get the spellbook structure from an object which is a book the player can
 * cast from
 */
const struct class_book *player_object_to_book(const struct player *p,
		const struct object *obj)
{
	int i;

	for (i = 0; i < p->class->magic.num_books; i++)
		if ((obj->tval == p->class->magic.books[i].tval) &&
			(obj->sval == p->class->magic.books[i].sval))
			return &p->class->magic.books[i];

	return NULL;
}

const struct class_spell *spell_by_index(const struct player *p, int index)
{
	int book = 0, count = 0;
	const struct class_magic *magic = &p->class->magic;

	/* Check index validity */
	if (index < 0 || index >= magic->total_spells)
		return NULL;

	/* Find the book, count the spells in previous books */
	while (count + magic->books[book].num_spells - 1 < index)
		count += magic->books[book++].num_spells;

	/* Find the spell */
	return &magic->books[book].spells[index - count];
}

/**
 * Collect spells from a book into the spells[] array, allocating
 * appropriate memory.
 */
int spell_collect_from_book(const struct player *p, const struct object *obj,
		int **spells)
{
	const struct class_book *book = player_object_to_book(p, obj);
	int i, n_spells = 0;

	if (!book) {
		return n_spells;
	}

	/* Count the spells */
	for (i = 0; i < book->num_spells; i++)
		n_spells++;

	/* Allocate the array */
	*spells = mem_zalloc(n_spells * sizeof(*spells));

	/* Write the spells */
	for (i = 0; i < book->num_spells; i++)
		(*spells)[i] = book->spells[i].sidx;

	return n_spells;
}


/**
 * Return the number of castable spells in the spellbook 'obj'.
 */
int spell_book_count_spells(const struct player *p, const struct object *obj,
		bool (*tester)(const struct player *p, int spell))
{
	const struct class_book *book = player_object_to_book(p, obj);
	int i, n_spells = 0;

	if (!book) {
		return n_spells;
	}

	for (i = 0; i < book->num_spells; i++)
		if (tester(p, book->spells[i].sidx))
			n_spells++;

	return n_spells;
}


/**
 * True if at least one spell in spells[] is OK according to spell_test.
 */
bool spell_okay_list(const struct player *p,
		bool (*spell_test)(const struct player *p, int spell),
		const int spells[], int n_spells)
{
	int i;
	bool okay = false;

	for (i = 0; i < n_spells; i++)
		if (spell_test(p, spells[i]))
			okay = true;

	return okay;
}

/**
 * True if the spell is castable.
 */
bool spell_okay_to_cast(const struct player *p, int spell)
{
	return (p->spell_flags[spell] & PY_SPELL_LEARNED);
}

/**
 * True if the spell can be studied.
 */
bool spell_okay_to_study(const struct player *p, int spell_index)
{
	const struct class_spell *spell = spell_by_index(p, spell_index);
	return spell && spell->slevel <= p->lev
		&& !(p->spell_flags[spell_index] & PY_SPELL_LEARNED);
}

/**
 * True if the spell is browsable.
 */
bool spell_okay_to_browse(const struct player *p, int spell_index)
{
	const struct class_spell *spell = spell_by_index(p, spell_index);
	return spell && spell->slevel < 99;
}

/**
 * Spell failure adjustment by casting stat level
 */
static int fail_adjust(struct player *p, const struct class_spell *spell)
{
	int stat = spell->realm->stat;
	return adj_mag_stat[p->state.stat_ind[stat]];
}

/**
 * Spell minimum failure by casting stat level
 */
static int min_fail(struct player *p, const struct class_spell *spell)
{
	int stat = spell->realm->stat;
	return adj_mag_fail[p->state.stat_ind[stat]];
}

/**
 * Returns chance of failure for a spell
 */
int16_t spell_chance(int spell_index)
{
	int chance = 100, minfail;

	const struct class_spell *spell;

	/* Paranoia -- must be literate */
	if (!player->class->magic.total_spells) return chance;

	/* Get the spell */
	spell = spell_by_index(player, spell_index);
	if (!spell) return chance;

	/* Extract the base spell failure rate */
	chance = spell->sfail;

	/* Reduce failure rate by "effective" level adjustment */
	chance -= 3 * (player->lev - spell->slevel);

	/* Reduce failure rate by casting stat level adjustment */
	chance -= fail_adjust(player, spell);

	/* Not enough mana to cast */
	if (spell->smana > player->csp)
		chance += 5 * (spell->smana - player->csp);

	/* Get the minimum failure rate for the casting stat level */
	minfail = min_fail(player, spell);

	/* Non zero-fail characters never get better than 5 percent */
	if (!player_has(player, PF_ZERO_FAIL) && minfail < 5) {
		minfail = 5;
	}

	/* Necromancers are punished by being on lit squares */
	if (player_has(player, PF_UNLIGHT) && square_islit(cave, player->grid)) {
		chance += 25;
	}

	/* Fear makes spells harder (before minfail) */
	/* Note that spells that remove fear have a much lower fail rate than
	 * surrounding spells, to make sure this doesn't cause mega fail */
	if (player_of_has(player, OF_AFRAID)) chance += 20;

	/* Minimal and maximal failure rate */
	if (chance < minfail) chance = minfail;
	if (chance > 50) chance = 50;

	/* Stunning makes spells harder (after minfail) */
	if (player->timed[TMD_STUN] > 50) {
		chance += 25;
	} else if (player->timed[TMD_STUN]) {
		chance += 15;
	}

	/* Amnesia makes spells very difficult */
	if (player->timed[TMD_AMNESIA]) {
		chance = 50 + chance / 2;
	}

	/* Always a 5 percent chance of working */
	if (chance > 95) {
		chance = 95;
	}

	/* Return the chance */
	return (chance);
}


/**
 * Learn the specified spell.
 */
void spell_learn(int spell_index)
{
	int i;
	const struct class_spell *spell = spell_by_index(player, spell_index);

	/* Learn the spell */
	player->spell_flags[spell_index] |= PY_SPELL_LEARNED;

	/* Find the next open entry in "spell_order[]" */
	for (i = 0; i < player->class->magic.total_spells; i++)
		if (player->spell_order[i] == 99) break;

	/* Add the spell to the known list */
	player->spell_order[i] = spell_index;

	/* Mention the result */
	// msgt(MSG_STUDY, "You have learned the %s of %s.", spell->realm->spell_noun,
	// msgt(MSG_STUDY, "Вы выучили %s %s.", spell->realm->spell_noun,
	msgt(MSG_STUDY, "Вы выучили %s %s.", (streq(spell->realm->spell_noun, "spell") ? "заклинание" :
		streq(spell->realm->spell_noun, "prayer") ? "молитву" : 
		streq(spell->realm->spell_noun, "verse") ? "стих" : 
		streq(spell->realm->spell_noun, "ritual") ? "ритуал" : ""), 
		spell->name);

	/* One less spell available */
	player->upkeep->new_spells--;

	/* Message if needed */
	if (player->upkeep->new_spells) {
		// msg("You can learn %d more %s%s.", player->upkeep->new_spells,
			// spell->realm->spell_noun, PLURAL(player->upkeep->new_spells));
		char noun[25];
		if (streq(spell->realm->spell_noun, "spell"))
				strnfmt(noun, sizeof(noun), "заклинани%s", PLURAL_RU(player->upkeep->new_spells, "е", "я", "й"));
		else if (streq(spell->realm->spell_noun, "prayer"))
			strnfmt(noun, sizeof(noun), "молитв%s", PLURAL_RU(player->upkeep->new_spells, "у", "ы", ""));
		else if (streq(spell->realm->spell_noun, "verse"))
			strnfmt(noun, sizeof(noun), "стих%s", PLURAL_RU(player->upkeep->new_spells, "", "а", "ов"));
		else if (streq(spell->realm->spell_noun, "ritual"))
			strnfmt(noun, sizeof(noun), "ритуал%s", PLURAL_RU(player->upkeep->new_spells, "", "а", "ов"));
		
		msg("Вы можете выучить ещё %d %s.", player->upkeep->new_spells, noun);
	}
	/* Redraw Study Status */
	player->upkeep->redraw |= (PR_STUDY | PR_OBJECT);
}

static int beam_chance(void)
{
	int plev = player->lev;
	return (player_has(player, PF_BEAM) ? plev : (plev / 2));
}

/**
 * Cast the specified spell
 */
bool spell_cast(int spell_index, int dir, struct command *cmd)
{
	int chance;
	bool ident = false;
	int beam  = beam_chance();

	/* Get the spell */
	const struct class_spell *spell = spell_by_index(player, spell_index);

	/* Spell failure chance */
	chance = spell_chance(spell_index);

	/* Fail or succeed */
	if (randint0(100) < chance) {
		event_signal(EVENT_INPUT_FLUSH);
		// msg("You failed to concentrate hard enough!");
		msg("Вы не смогли достаточно сконцентрироваться!");
	} else {
		/* Cast the spell */
		if (!effect_do(spell->effect, source_player(), NULL, &ident, true, dir,
					   beam, 0, cmd)) {
			return false;
		}

		/* Reward COMBAT_REGEN with small HP recovery */
		if (player_has(player, PF_COMBAT_REGEN)) {
			convert_mana_to_hp(player, spell->smana << 16);
		}

		/* A spell was cast */
		sound(MSG_SPELL);

		if (!(player->spell_flags[spell_index] & PY_SPELL_WORKED)) {
			int e = spell->sexp;

			/* The spell worked */
			player->spell_flags[spell_index] |= PY_SPELL_WORKED;

			/* Gain experience */
			player_exp_gain(player, e * spell->slevel);

			/* Redraw object recall */
			player->upkeep->redraw |= (PR_OBJECT);
		}
	}

	/* Sufficient mana? */
	if (spell->smana <= player->csp) {
		/* Use some mana */
		player->csp -= spell->smana;
	} else {
		int oops = spell->smana - player->csp;

		/* No mana left */
		player->csp = 0;
		player->csp_frac = 0;

		/* Over-exert the player */
		player_over_exert(player, PY_EXERT_FAINT, 100, 5 * oops + 1);
		player_over_exert(player, PY_EXERT_CON, 50, 0);
	}

	/* Redraw mana */
	player->upkeep->redraw |= (PR_MANA);

	return true;
}


bool spell_needs_aim(int spell_index)
{
	const struct class_spell *spell = spell_by_index(player, spell_index);
	assert(spell);
	return effect_aim(spell->effect);
}

static size_t append_random_value_string(char *buffer, size_t size,
										 random_value *rv)
{
	size_t offset = 0;

	if (rv->base > 0) {
		offset += strnfmt(buffer + offset, size - offset, "%d", rv->base);

		if (rv->dice > 0 && rv->sides > 0) {
			offset += strnfmt(buffer + offset, size - offset, "+");
		}
	}

	if (rv->dice == 1 && rv->sides > 0) {
		// offset += strnfmt(buffer + offset, size - offset, "d%d", rv->sides);
		offset += strnfmt(buffer + offset, size - offset, "к%d", rv->sides);
	} else if (rv->dice > 1 && rv->sides > 0) {
		// offset += strnfmt(buffer + offset, size - offset, "%dd%d", rv->dice,
		offset += strnfmt(buffer + offset, size - offset, "%dк%d", rv->dice,
						  rv->sides);
	}

	return offset;
}

static void spell_effect_append_value_info(const struct effect *effect,
		char *p, size_t len, struct spell_info_iteration_state *ist)
{
	random_value rv = { 0, 0, 0, 0 };
	const char *type = NULL;
	char special[40] = "";
	size_t offset = strlen(p);

	if (effect->index == EF_CLEAR_VALUE) {
		ist->have_shared = false;
	} else if (effect->index == EF_SET_VALUE && effect->dice) {
		ist->have_shared = true;
		dice_roll(effect->dice, &ist->shared_rv);
	}

	type = effect_info(effect);
	if (type == NULL) return;

	if (effect->dice != NULL) {
		dice_roll(effect->dice, &rv);
	} else if (ist->have_shared) {
		rv = ist->shared_rv;
	}

	/* Handle some special cases where we want to append some additional info */
	switch (effect->index) {
		case EF_HEAL_HP:
			/* Append percentage only, as the fixed value is always displayed */
			if (rv.m_bonus) {
				strnfmt(special, sizeof(special), "/%d%%",
					rv.m_bonus);
			}
			break;
		case EF_TELEPORT:
			/* m_bonus means it's a weird random thing */
			if (rv.m_bonus) {
				my_strcpy(special, "random", sizeof(special));
			}
			break;
		case EF_SPHERE:
			/* Append radius */
			if (effect->radius) {
				int rad = effect->radius;
				strnfmt(special, sizeof(special), ", rad %d",
					rad);
			} else {
				my_strcpy(special, ", rad 2", sizeof(special));
			}
			break;
		case EF_BALL:
			/* Append radius */
			if (effect->radius) {
				int rad = effect->radius;
				if (effect->other) {
					rad += player->lev / effect->other;
				}
				strnfmt(special, sizeof(special), ", rad %d",
					rad);
			} else {
				my_strcpy(special, "rad 2", sizeof(special));
			}
			break;
		case EF_STRIKE:
			/* Append radius */
			if (effect->radius) {
				strnfmt(special, sizeof(special), ", rad %d",
					effect->radius);
			}
			break;
		case EF_SHORT_BEAM: {
			/* Append length of beam */
			int beam_len = effect->radius;
			if (effect->other) {
				beam_len += player->lev / effect->other;
				beam_len = MIN(beam_len, z_info->max_range);
			}
			strnfmt(special, sizeof(special), ", len %d", beam_len);
			break;
		}
		case EF_SWARM:
			/* Append number of projectiles. */
			strnfmt(special, sizeof(special), "x%d", rv.m_bonus);
			break;
	}

	/*
	 * Only display if have dice and it isn't redundant with the
	 * the previous one that was displayed.
	 */
	if ((rv.base > 0 || (rv.dice > 0 && rv.sides > 0))
			&& (!ist->pre
			|| ist->pre->index != effect->index
			|| !streq(special, ist->pre_special)
			|| ist->pre_rv.base != rv.base
			|| (((ist->pre_rv.dice > 0 && ist->pre_rv.sides > 0)
			|| (rv.dice > 0 && rv.sides > 0))
			&& (ist->pre_rv.dice != rv.dice
			|| ist->pre_rv.sides != rv.sides)))) {
		if (offset) {
			offset += strnfmt(p + offset, len - offset, ";");
		}

		offset += strnfmt(p + offset, len - offset, " %s ", type);
		offset += append_random_value_string(p + offset, len - offset, &rv);

		if (strlen(special) > 1) {
			strnfmt(p + offset, len - offset, "%s", special);
		}

		ist->pre = effect;
		my_strcpy(ist->pre_special, special, sizeof(ist->pre_special));
		ist->pre_rv = rv;
	}
}

void get_spell_info(int spell_index, char *p, size_t len)
{
	struct effect *effect = spell_by_index(player, spell_index)->effect;
	struct spell_info_iteration_state ist = {
		NULL, "", { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false };

	p[0] = '\0';

	while (effect) {
		spell_effect_append_value_info(effect, p, len, &ist);
		effect = effect->next;
	}
}
