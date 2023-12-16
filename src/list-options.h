/**
 * \file list-options.h
 * \brief options
 *
 * Currently, if there are more than 21 of any option type, the later ones
 * will be ignored
 * Cheat options need to be followed by corresponding score options
 */

/* name                   description
type     normal */
OP(none,                  "",
SPECIAL, false)
// OP(rogue_like_commands,   "Use the roguelike command keyset",
OP(rogue_like_commands,   "Использовать roguelike команды",
INTERFACE, false)
// OP(use_sound,             "Use sound",
OP(use_sound,             "Включить звук",
INTERFACE, false)
// OP(show_damage,           "Show damage player deals to monsters",
OP(show_damage,           "Показывать урон нанесённый монстрам",
INTERFACE, false)
// OP(use_old_target,        "Use old target by default",
OP(use_old_target,        "Использовать старое целеуказание",
INTERFACE, false)
// OP(pickup_always,         "Always pickup items",
OP(pickup_always,         "Всегда подбирать предметы",
INTERFACE, false)
// OP(pickup_inven,          "Always pickup items matching inventory",
OP(pickup_inven,          "Всегда подбирать предметы имеющиеся в инвентаре",
INTERFACE, true)
// OP(show_flavors,          "Show flavors in object descriptions",
OP(show_flavors,          "Показывать вкусы(flavors) в описаниях объектов",
INTERFACE, false)
// OP(show_target,           "Highlight target with cursor",
OP(show_target,           "Выделить цель курсором",
INTERFACE, true)
// OP(highlight_player,      "Highlight player with cursor between turns",
OP(highlight_player,      "Выделить игрока курсором между ходами",
INTERFACE, false)
// OP(disturb_near,          "Disturb whenever viewable monster moves",
OP(disturb_near,          "Тревожить всякий раз, когда монстр движется",
INTERFACE, true)
// OP(solid_walls,           "Show walls as solid blocks",
OP(solid_walls,           "Показывать стены как сплошные блоки",
INTERFACE, false)
// OP(hybrid_walls,          "Show walls with shaded background",
OP(hybrid_walls,          "Показать стены с затененным фоном",
INTERFACE, false)
// OP(view_yellow_light,     "Color: Illuminate torchlight in yellow",
OP(view_yellow_light,     "Цвет: подсветить область света желтым",
INTERFACE, false)
// OP(animate_flicker,       "Color: Shimmer multi-colored things",
OP(animate_flicker,       "Цвет: Мерцающие разноцветные предметы",
INTERFACE, false)
// OP(center_player,         "Center map continuously",
OP(center_player,         "Центрировать карту постоянно",
INTERFACE, false)
// OP(purple_uniques,        "Color: Show unique monsters in purple",
OP(purple_uniques,        "Цвет: показывать уникальных монстров фиолетовым",
INTERFACE, false)
// OP(auto_more,             "Automatically clear '-more-' prompts",
OP(auto_more,             "Автоматически очищать подсказки '-ещё-'",
INTERFACE, false)
// OP(hp_changes_color,      "Color: Player color indicates % hit points",
OP(hp_changes_color,      "Цвет: цвет игрока указывает % от здоровья",
INTERFACE, true)
// OP(mouse_movement,        "Allow mouse clicks to move the player",
OP(mouse_movement,        "Разрешить перемещение игрока щелчками мыши",
INTERFACE, true)
// OP(notify_recharge,       "Notify on object recharge",
OP(notify_recharge,       "Уведомлять о перезарядке объекта",
INTERFACE, false)
// OP(effective_speed,       "Show effective speed as multiplier",
OP(effective_speed,       "Показывать скорость как множитель",
INTERFACE, false)
// OP(cheat_hear,            "Cheat: Peek into monster creation",
OP(cheat_hear,            "Чит: загляните в создание монстров",
CHEAT, false)
// OP(score_hear,            "Score: Peek into monster creation",
OP(score_hear,            "Счёт: загляните в создание монстров",
SCORE, false)
// OP(cheat_room,            "Cheat: Peek into dungeon creation",
OP(cheat_room,            "Чит: загляните в создание подземелий",
CHEAT, false)
// OP(score_room,            "Score: Peek into dungeon creation",
OP(score_room,            "Счёт: заглянуть в создание подземелий",
SCORE, false)
// OP(cheat_xtra,            "Cheat: Peek into something else",
OP(cheat_xtra,            "Чит: заглянуть во что-то еще",
CHEAT, false)
// OP(score_xtra,            "Score: Peek into something else",
OP(score_xtra,            "Счёт: заглянуть во что-то еще",
SCORE, false)
// OP(cheat_live,            "Cheat: Allow player to avoid death",
OP(cheat_live,            "Чит: Разрешить игроку избежать смерти",
CHEAT, false)
// OP(score_live,            "Score: Allow player to avoid death",
OP(score_live,            "Счёт: позволить игроку избежать смерти",
SCORE, false)
// OP(birth_randarts,        "Generate a new, random artifact set",
OP(birth_randarts,        "Сгенерировать новый, случайный набор артефактов",
BIRTH, false)
// OP(birth_connect_stairs,  "Generate connected stairs",
OP(birth_connect_stairs,  "Создавать соединенные лестницы",
BIRTH, true)
// OP(birth_force_descend,   "Force player descent (never make up stairs)",
OP(birth_force_descend,   "Принудительный спуск игрока (движение только вниз)",
BIRTH, false)
// OP(birth_no_recall,       "Word of Recall has no effect",
OP(birth_no_recall,       "Свиток Возвращения не имеет эффекта",
BIRTH, false)
// OP(birth_no_artifacts,    "Restrict creation of artifacts",
OP(birth_no_artifacts,    "Ограничить создание артефактов",
BIRTH, false)
// OP(birth_stacking,        "Stack objects on the floor",
OP(birth_stacking,        "Складывать предметы на пол",
BIRTH, true)
// OP(birth_lose_arts,       "Lose artifacts when leaving level",
OP(birth_lose_arts,       "Потеря артефактов при уходе с этажа",
BIRTH, false)
// OP(birth_feelings,        "Show level feelings",
OP(birth_feelings,        "Показать уровень чувств",
BIRTH, true)
// OP(birth_no_selling,      "Increase gold drops but disable selling",
OP(birth_no_selling,      "Увеличить количество золота,но запретить продажу",
BIRTH, true)
// OP(birth_start_kit,       "Start with a kit of useful gear",
OP(birth_start_kit,       "Начинать с комплектом полезного снаряжения",
BIRTH, true)
// OP(birth_ai_learn,        "Monsters learn from their mistakes",
OP(birth_ai_learn,        "Монстры учатся на своих ошибках",
BIRTH, true)
// OP(birth_know_runes,      "Know all runes on birth",
OP(birth_know_runes,      "Знать все руны при рождении",
BIRTH, false)
// OP(birth_know_flavors,    "Know all flavors on birth",
OP(birth_know_flavors,    "Знать все вкусы(flavors) при рождении",
BIRTH, false)
// OP(birth_levels_persist,  "Persistent levels (experimental)",
OP(birth_levels_persist,  "Постоянные уровни (эксперимент.)",
BIRTH, false)
// OP(birth_percent_damage,  "To-damage is a percentage of dice (experimental)",
OP(birth_percent_damage,  "Урон - процент от кубика (эксперимент.)",
BIRTH, false)

