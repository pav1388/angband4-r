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
MON_MSG(NONE,				MSG_GENERIC,	false,	"страда[ет|ют].")
MON_MSG(DIE,				MSG_KILL,		false,	"умира[ет|ют].")
MON_MSG(DESTROYED,			MSG_KILL,		false,	"уничтожен[|ы.]") // fix in mon-msg.c show_message()
MON_MSG(RESIST_A_LOT,		MSG_GENERIC,	false,	"упорно сопротивля[ется|ются].")
MON_MSG(HIT_HARD,			MSG_GENERIC,	false,	"сильно пострадал[|и.]") // fix ...
MON_MSG(RESIST,				MSG_GENERIC,	false,	"сопротивля[ется|ются].")
MON_MSG(IMMUNE,				MSG_GENERIC,	false,	"неуязвим[|ы.]") // fix ...
MON_MSG(RESIST_SOMEWHAT,	MSG_GENERIC,	false,	"немного сопротивля[ется|ются].")
MON_MSG(UNAFFECTED,			MSG_GENERIC,	false,	"не пострадал[|и!]") // fix ...
MON_MSG(SPAWN,				MSG_GENERIC,	false,	"размножа[ется|ются]!")
MON_MSG(HEALTHIER,			MSG_GENERIC,	false,	"выгляд[ит|ят] здоровым[|и].")
MON_MSG(FALL_ASLEEP,		MSG_GENERIC,	false,	"засыпа[ет|ют]!")
MON_MSG(WAKES_UP,			MSG_GENERIC,	false,	"просыпа[ется|ются].")
MON_MSG(CRINGE_LIGHT,		MSG_GENERIC,	false,	"корч[ится|атся] от света!")
MON_MSG(SHRIVEL_LIGHT,		MSG_KILL,		false,	"рассыпа[ется|ются] на свету!")
MON_MSG(LOSE_SKIN,			MSG_GENERIC,	false,	"теря[ет|ют] немного кожи!")
MON_MSG(DISSOLVE,			MSG_KILL,		false,	"растворя[ется|ются]!")
MON_MSG(CATCH_FIRE,			MSG_GENERIC,	false,	"загора[ется|ются]!")
MON_MSG(BADLY_FROZEN,		MSG_GENERIC,	false,	"сильно замерза[ет|ют].")
MON_MSG(SHUDDER,			MSG_GENERIC,	false,	"дрож[ит|ат].")
MON_MSG(CHANGE,				MSG_GENERIC,	false,	"меня[ется|ются]!")
MON_MSG(DISAPPEAR,			MSG_GENERIC,	false,	"исчеза[ет|ют]!")
MON_MSG(MORE_DAZED,			MSG_GENERIC,	false,	"сильно ошеломлен[|ы.]") // fix ...
MON_MSG(DAZED,				MSG_GENERIC,	false,	"ошеломлен[|ы.]") // fix ...
MON_MSG(NOT_DAZED,			MSG_GENERIC,	false,	"больше не ошеломлен[|ы.]") // fix ...
MON_MSG(MORE_CONFUSED,		MSG_GENERIC,	false,	"выгляд[ит|ят] сильно растерянн[|ыми.]") // fix ...
MON_MSG(CONFUSED,			MSG_GENERIC,	false,	"выгляд[ит|ят] растерянн[|ыми].") // fix ...
MON_MSG(NOT_CONFUSED,		MSG_GENERIC,	false,	"больше не растерян[|ы.]") // fix ...
MON_MSG(MORE_SLOWED,		MSG_GENERIC,	false,	"выгляд[ит|ят] сильно медлительн[|ыми.]") // fix ...
MON_MSG(SLOWED,				MSG_GENERIC,	false,	"выгляд[ит|ят] медлительн[|ыми.]") // fix ...
MON_MSG(NOT_SLOWED	,		MSG_GENERIC,	false,	"ускоря[ется|ются].")
MON_MSG(MORE_HASTED,		MSG_GENERIC,	false,	"выгляд[ит|ят] более ускорен[|ыми!]") // fix ...
MON_MSG(HASTED,				MSG_GENERIC,	false,	"начина[ет|ют] двигаться быстрее.")
MON_MSG(NOT_HASTED,			MSG_GENERIC,	false,	"замедля[ется|ются].")
MON_MSG(MORE_AFRAID,		MSG_GENERIC,	false,	"выгляд[ит|ят] сильно испуганн[|ыми!]") // fix ...
MON_MSG(FLEE_IN_TERROR,		MSG_FLEE,		false,	"бе[жит|гут] в страхе!")
MON_MSG(NOT_AFRAID,			MSG_GENERIC,	false,	"больше не бо[ится|ятся].")
MON_MSG(HELD,				MSG_GENERIC,    false,  "замира[ет|ют] на месте!")
MON_MSG(NOT_HELD,			MSG_GENERIC,    false,  "снова мо[жет|гут] двигаться.")
MON_MSG(DISEN,				MSG_GENERIC,    false,  "выгляд[ит|ят] менее волшебн[|ыми!]") // fix ...
MON_MSG(NOT_DISEN,			MSG_GENERIC,    false,  "снова выгляд[ит|ят] волшебн[|ыми.]") // fix ...
MON_MSG(COMMAND,			MSG_GENERIC,    false,  "попада[ет|ют] под ваши чары!")
MON_MSG(NOT_COMMAND,		MSG_GENERIC,    false,  "больше не под вашим контролем.")
MON_MSG(SHAPE_FAIL,			MSG_GENERIC,    false,  "мерца[ет|ют] на мгновение.")
MON_MSG(MORIA_DEATH,		MSG_KILL,		true,	"Вы слышите[| несколько] крик[|ов] агонии!")
MON_MSG(DISINTEGRATES,		MSG_KILL,		false,	"распада[ется|ются]!")
MON_MSG(FREEZE_SHATTER,		MSG_KILL,		false,	"замерза[ет|ют] и рассыпа[ется|ются]!")
MON_MSG(MANA_DRAIN,			MSG_GENERIC,	false,	"теря[ет|ют] немного маны!")
MON_MSG(BRIEF_PUZZLE,		MSG_GENERIC,	false,	"выгляд[ит|ят] озадаченн[|ыми.]") // fix ...
MON_MSG(MAINTAIN_SHAPE,		MSG_GENERIC,	false,	"сохраня[ет|ют] прежний облик.")
MON_MSG(UNHARMED,			MSG_GENERIC,	false,	"невредимым[|ы.]") // fix ...
MON_MSG(APPEAR,				MSG_GENERIC,	false,	"появля[ется|ются]!")
MON_MSG(HIT_AND_RUN,		MSG_GENERIC,	true,	"Растворяется в облаке дыма!")
MON_MSG(QUAKE_DEATH,		MSG_KILL,		false,	"поко[ится|ятся] под камнями!")
MON_MSG(QUAKE_HURT,			MSG_GENERIC,	false,	"во[ет|ют] от боли!")
/* Dummy messages for monster pain - we use edit file info instead. */
MON_MSG(95,					MSG_GENERIC,	false,	"")
MON_MSG(75,					MSG_GENERIC,	false,	"")
MON_MSG(50,					MSG_GENERIC,	false,	"")
MON_MSG(35,					MSG_GENERIC,	false,	"")
MON_MSG(20,					MSG_GENERIC,	false,	"")
MON_MSG(10,					MSG_GENERIC,	false,	"")
MON_MSG(0,					MSG_GENERIC,	false,	"")
MON_MSG(MAX,				MSG_GENERIC,	false,	"")
