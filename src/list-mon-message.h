/**
 * \file list-mon-message.h
 * \brief List of monster message types
 *
 * id           - the message constant name
 * msg          - MSG_ type for the printed message
 * omit_subject - true to omit a monster name beforehand (see mon-msg.c)
 * text         - the message text
 */

/* id						MSG_GENERIC,	text */
// MON_MSG(NONE,				MSG_GENERIC,	false,	"[is|are] hurt.")
MON_MSG(NONE,				MSG_GENERIC,	false,	"[страдает|страдают].")
// MON_MSG(DIE,				MSG_KILL,		false,	"die[s].")
MON_MSG(DIE,				MSG_KILL,		false,	"[умирает|умирают].")
// MON_MSG(DESTROYED,			MSG_KILL,		false,	"[is|are] destroyed.")
MON_MSG(DESTROYED,			MSG_KILL,		false,	"уничтожен[|ы.]")							// +1-"" "а" "о"
// MON_MSG(RESIST_A_LOT,		MSG_GENERIC,	false,	"resist[s] a lot.")
MON_MSG(RESIST_A_LOT,		MSG_GENERIC,	false,	"упорно сопротивля[ется|ются].")
// MON_MSG(HIT_HARD,			MSG_GENERIC,	false,	"[is|are] hit hard.")
MON_MSG(HIT_HARD,			MSG_GENERIC,	false,	"сильно пострадал[|и.]")					// +1-"" "а" "о"
// MON_MSG(RESIST,				MSG_GENERIC,	false,	"resist[s].")
MON_MSG(RESIST,				MSG_GENERIC,	false,	"сопротивля[ется|ются].")
// MON_MSG(IMMUNE,				MSG_GENERIC,	false,	"[is|are] immune.")
MON_MSG(IMMUNE,				MSG_GENERIC,	false,	"неуязвим[|ы.]")							// +1-"" "а" "о"
// MON_MSG(RESIST_SOMEWHAT,	MSG_GENERIC,	false,	"resist[s] somewhat.")
MON_MSG(RESIST_SOMEWHAT,	MSG_GENERIC,	false,	"немного сопротивля[ется|ются].")
// MON_MSG(UNAFFECTED,			MSG_GENERIC,	false,	"[is|are] unaffected!")
MON_MSG(UNAFFECTED,			MSG_GENERIC,	false,	"не пострадал[|и!]")						// +1-"" "а" "о"
// MON_MSG(SPAWN,				MSG_GENERIC,	false,	"spawn[s]!")
MON_MSG(SPAWN,				MSG_GENERIC,	false,	"размножа[ется|ются]!")
// MON_MSG(HEALTHIER,			MSG_GENERIC,	false,	"look[s] healthier.")
MON_MSG(HEALTHIER,			MSG_GENERIC,	false,	"выглядит здоровым.")
// MON_MSG(FALL_ASLEEP,		MSG_GENERIC,	false,	"fall[s] asleep!")
MON_MSG(FALL_ASLEEP,		MSG_GENERIC,	false,	"засыпа[ет|ют]!")
// MON_MSG(WAKES_UP,			MSG_GENERIC,	false,	"wake[s] up.")
MON_MSG(WAKES_UP,			MSG_GENERIC,	false,	"просыпа[ется|ются].")
// MON_MSG(CRINGE_LIGHT,		MSG_GENERIC,	false,	"cringe[s] from the light!")
MON_MSG(CRINGE_LIGHT,		MSG_GENERIC,	false,	"корч[ится|атся] от света!")
// MON_MSG(SHRIVEL_LIGHT,		MSG_KILL,		false,	"shrivel[s] away in the light!")
MON_MSG(SHRIVEL_LIGHT,		MSG_KILL,		false,	"рассыпа[ется|ются] на свету!")
// MON_MSG(LOSE_SKIN,			MSG_GENERIC,	false,	"lose[s] some skin!")
MON_MSG(LOSE_SKIN,			MSG_GENERIC,	false,	"теря[ет|ют] немного кожи!")
// MON_MSG(DISSOLVE,			MSG_KILL,		false,	"dissolve[s]!")
MON_MSG(DISSOLVE,			MSG_KILL,		false,	"растворя[ется|ются]!")
// MON_MSG(CATCH_FIRE,			MSG_GENERIC,	false,	"catch[es] fire!")
MON_MSG(CATCH_FIRE,			MSG_GENERIC,	false,	"загора[ется|ются]!")
// MON_MSG(BADLY_FROZEN,		MSG_GENERIC,	false,	"[is|are] badly frozen.")
MON_MSG(BADLY_FROZEN,		MSG_GENERIC,	false,	"сильно замерза[ет|ют].")
// MON_MSG(SHUDDER,			MSG_GENERIC,	false,	"shudder[s].")
MON_MSG(SHUDDER,			MSG_GENERIC,	false,	"дрож[ит|ат].")
// MON_MSG(CHANGE,				MSG_GENERIC,	false,	"change[s]!")
MON_MSG(CHANGE,				MSG_GENERIC,	false,	"меня[ется|ются]!")
// MON_MSG(DISAPPEAR,			MSG_GENERIC,	false,	"disappear[s]!")
MON_MSG(DISAPPEAR,			MSG_GENERIC,	false,	"исчеза[ет|ют]!")
// MON_MSG(MORE_DAZED,			MSG_GENERIC,	false,	"[is|are] even more stunned.")
MON_MSG(MORE_DAZED,			MSG_GENERIC,	false,	"сильно ошеломлен[|ы.]")					// +1-"" "а" "о"
// MON_MSG(DAZED,				MSG_GENERIC,	false,	"[is|are] stunned.")
MON_MSG(DAZED,				MSG_GENERIC,	false,	"ошеломлен[|ы.]")							// +1-"" "а" "о"
// MON_MSG(NOT_DAZED,			MSG_GENERIC,	false,	"[is|are] no longer stunned.")
MON_MSG(NOT_DAZED,			MSG_GENERIC,	false,	"больше не ошеломлен[|ы.]")					// +1-"" "а" "о"
// MON_MSG(MORE_CONFUSED,		MSG_GENERIC,	false,	"look[s] more confused.")
MON_MSG(MORE_CONFUSED,		MSG_GENERIC,	false,	"выгляд[ит|ят] сильно растерянн[о|ыми].")	// -2-"" "" ""
// MON_MSG(CONFUSED,			MSG_GENERIC,	false,	"look[s] confused.")
MON_MSG(CONFUSED,			MSG_GENERIC,	false,	"выгляд[ит|ят] растерянн[о|ыми].")			// -2-"" "" ""
// MON_MSG(NOT_CONFUSED,		MSG_GENERIC,	false,	"[is|are] no longer confused.")
MON_MSG(NOT_CONFUSED,		MSG_GENERIC,	false,	"больше не растерян[|ы].")					// +1-"" "а" "о"
// MON_MSG(MORE_SLOWED,		MSG_GENERIC,	false,	"look[s] more slowed.")
MON_MSG(MORE_SLOWED,		MSG_GENERIC,	false,	"выгляд[ит|ят] сильно медлительн[о|ыми].")	// -2-"" "" ""
// MON_MSG(SLOWED,				MSG_GENERIC,	false,	"look[s] slowed.")
MON_MSG(SLOWED,				MSG_GENERIC,	false,	"выгляд[ит|ят] медлительн[о|ыми].")			// -2-"" "" ""
// MON_MSG(NOT_SLOWED	,		MSG_GENERIC,	false,	"speed[s] up.")
MON_MSG(NOT_SLOWED	,		MSG_GENERIC,	false,	"ускоря[ется|ются].")
// MON_MSG(MORE_HASTED,		MSG_GENERIC,	false,	"look[s] even faster!")
MON_MSG(MORE_HASTED,		MSG_GENERIC,	false,	"выгляд[ит|ят] более ускорено[о|ыми]!")		// -2-"" "" ""
// MON_MSG(HASTED,				MSG_GENERIC,	false,	"start[s] moving faster.")
MON_MSG(HASTED,				MSG_GENERIC,	false,	"начина[ет|ют] двигаться быстрее.")
// MON_MSG(NOT_HASTED,			MSG_GENERIC,	false,	"slow[s] down.")
MON_MSG(NOT_HASTED,			MSG_GENERIC,	false,	"замедля[ется|ются].")
// MON_MSG(MORE_AFRAID,		MSG_GENERIC,	false,	"look[s] more terrified!")
MON_MSG(MORE_AFRAID,		MSG_GENERIC,	false,	"выгляд[ит|ят] сильно испуганн[о|ыми]!")	// -2-"" "" ""
// MON_MSG(FLEE_IN_TERROR,		MSG_FLEE,		false,	"flee[s] in terror!")
MON_MSG(FLEE_IN_TERROR,		MSG_FLEE,		false,	"бе[жит|гут] в страхе!")
// MON_MSG(NOT_AFRAID,			MSG_GENERIC,	false,	"[is|are] no longer afraid.")
MON_MSG(NOT_AFRAID,			MSG_GENERIC,	false,	"больше не бо[ится|ятся].")
// MON_MSG(HELD,				MSG_GENERIC,    false,  "[is|are] frozen to the spot!")
MON_MSG(HELD,				MSG_GENERIC,    false,  "замира[ет|ют] на месте!")
// MON_MSG(NOT_HELD,			MSG_GENERIC,    false,  "can move again.")
MON_MSG(NOT_HELD,			MSG_GENERIC,    false,  "снова мо[жет|гут] двигаться.")
// MON_MSG(DISEN,				MSG_GENERIC,    false,  "seem[s] less magical!")
MON_MSG(DISEN,				MSG_GENERIC,    false,  "выгляд[ит|ят] менее волшебн[о|ыми]!")		// -2-"" "" ""
// MON_MSG(NOT_DISEN,			MSG_GENERIC,    false,  "seem[s] magical again.")
MON_MSG(NOT_DISEN,			MSG_GENERIC,    false,  "снова выгляд[ит|ят] волшебн[о|ыми].")		// -2-"" "" ""
// MON_MSG(COMMAND,			MSG_GENERIC,    false,  "falls under your spell!")
MON_MSG(COMMAND,			MSG_GENERIC,    false,  "попада[ет|ют] под ваши чары!")
// MON_MSG(NOT_COMMAND,		MSG_GENERIC,    false,  "is no longer under your control.")
MON_MSG(NOT_COMMAND,		MSG_GENERIC,    false,  "больше не под контролем.")
// MON_MSG(SHAPE_FAIL,			MSG_GENERIC,    false,  "shimmers for a moment.")
MON_MSG(SHAPE_FAIL,			MSG_GENERIC,    false,  "мерца[ет|ют] на мгновение.")
// MON_MSG(MORIA_DEATH,		MSG_KILL,		true,	"You hear [a|several] scream[|s] of agony!")
MON_MSG(MORIA_DEATH,		MSG_KILL,		true,	"Вы слышите[| несколько] крик[|ов] агонии!")
// MON_MSG(DISINTEGRATES,		MSG_KILL,		false,	"disintegrate[s]!")
MON_MSG(DISINTEGRATES,		MSG_KILL,		false,	"распада[ется|ются]!")
// MON_MSG(FREEZE_SHATTER,		MSG_KILL,		false,	"freeze[s] and shatter[s]!")
MON_MSG(FREEZE_SHATTER,		MSG_KILL,		false,	"замерза[ет|ют] и рассыпа[ется|ются]!")
// MON_MSG(MANA_DRAIN,			MSG_GENERIC,	false,	"lose[s] some mana!")
MON_MSG(MANA_DRAIN,			MSG_GENERIC,	false,	"теря[ет|ют] немного маны!")
// MON_MSG(BRIEF_PUZZLE,		MSG_GENERIC,	false,	"look[s] briefly puzzled.")
MON_MSG(BRIEF_PUZZLE,		MSG_GENERIC,	false,	"выгляд[ит|ят] озадаченн[о|ыми].")			// -2-"" "" ""
// MON_MSG(MAINTAIN_SHAPE,		MSG_GENERIC,	false,	"maintain[s] the same shape.")
MON_MSG(MAINTAIN_SHAPE,		MSG_GENERIC,	false,	"сохраня[ет|ют] прежний облик.")
// MON_MSG(UNHARMED,			MSG_GENERIC,	false,	"[is|are] unharmed.")
MON_MSG(UNHARMED,			MSG_GENERIC,	false,	"невредимым[|ы.]")							// +1-"" "а" "о"
// MON_MSG(APPEAR,			MSG_GENERIC,	false,	"appear[s]!")
MON_MSG(APPEAR,			MSG_GENERIC,	false,	"появля[ется|ются]!")
// MON_MSG(HIT_AND_RUN,		MSG_GENERIC,	true,	"There is a puff of smoke!")
MON_MSG(HIT_AND_RUN,		MSG_GENERIC,	true,	"Растворяется в облаке дыма!")
/* Dummy messages for monster pain - we use edit file info instead. */
MON_MSG(95,					MSG_GENERIC,	false,	"")
MON_MSG(75,					MSG_GENERIC,	false,	"")
MON_MSG(50,					MSG_GENERIC,	false,	"")
MON_MSG(35,					MSG_GENERIC,	false,	"")
MON_MSG(20,					MSG_GENERIC,	false,	"")
MON_MSG(10,					MSG_GENERIC,	false,	"")
MON_MSG(0,					MSG_GENERIC,	false,	"")
MON_MSG(MAX,				MSG_GENERIC,	false,	"")
