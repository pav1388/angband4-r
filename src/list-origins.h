/**
 * \file list-origins.h
 * \brief List of object origins
 */
ORIGIN(NONE,		-1,	"")
// ORIGIN(FLOOR,		1,	"Found lying on the floor %s")
ORIGIN(FLOOR,		1,	"Найдено лежащим на полу %s")
// ORIGIN(CHEST,		1,	"Taken from a chest found %s")
ORIGIN(CHEST,		1,	"Взято из сундука найденного %s")
// ORIGIN(SPECIAL,		1,	"Found lying on the floor of a special room %s")
ORIGIN(SPECIAL,		1,	"Найдено лежащим на полу специальной комнаты %s")
// ORIGIN(PIT,			1,	"Found lying on the floor in a pit %s")
ORIGIN(PIT,			1,	"Найден лежащим на полу в яме %s")
// ORIGIN(VAULT,		1,	"Found lying on the floor in a vault %s")
ORIGIN(VAULT,		1,	"Найдено лежащим на полу в склепе %s")
// ORIGIN(LABYRINTH,	1,	"Found lying on the floor of a labyrinth %s")
ORIGIN(LABYRINTH,	1,	"Найдено лежащим на полу лабиринта %s")
// ORIGIN(CAVERN,		1,	"Found lying on the floor of a cavern %s")
ORIGIN(CAVERN,		1,	"Найдено лежащим на полу пещеры %s")
// ORIGIN(RUBBLE,		1,	"Found under some rubble %s")
ORIGIN(RUBBLE,		1,	"Найдено под завалами %s")
ORIGIN(MIXED,		-1,	"")                 /* stack with mixed origins */
// ORIGIN(DROP,		2,	"Dropped by %s %s") /* normal monster drops */
ORIGIN(DROP,		2,	"Брошено %s %s") /* normal monster drops */
// ORIGIN(DROP_SPECIAL,2,	"Dropped by %s %s") /* from monsters in special rooms */
ORIGIN(DROP_SPECIAL,2,	"Брошено %s %s") /* from monsters in special rooms */
// ORIGIN(DROP_PIT,	2,	"Dropped by %s %s") /* from monsters in pits/nests */
ORIGIN(DROP_PIT,	2,	"Брошено %s %s") /* from monsters in pits/nests */
// ORIGIN(DROP_VAULT,	2,	"Dropped by %s %s") /* from monsters in vaults */
ORIGIN(DROP_VAULT,	2,	"Брошено %s %s") /* from monsters in vaults */
ORIGIN(STATS,		-1,	"")  /* ^ only the above are considered by main-stats */
// ORIGIN(ACQUIRE,		1,	"Conjured forth by magic %s")
ORIGIN(ACQUIRE,		1,	"Создано магией %s")
// ORIGIN(STORE,		0,	"Bought from a store")
ORIGIN(STORE,		0,	"Куплено в магазине")
ORIGIN(STOLEN,		-1,	"")
// ORIGIN(BIRTH,		0,	"An inheritance from your family")
ORIGIN(BIRTH,		0,	"Наследство, полученное от вашей семьи.")
// ORIGIN(CHEAT,		0,	"Created by debug option")
ORIGIN(CHEAT,		0,	"Создано опциями отладки")
// ORIGIN(DROP_BREED,	2,	"Dropped by %s %s") /* from breeders */
ORIGIN(DROP_BREED,	2,	"Брошено %s %s") /* from breeders */
// ORIGIN(DROP_SUMMON,	2,	"Dropped by %s %s") /* from combat summons */
ORIGIN(DROP_SUMMON,	2,	"Брошено %s %s") /* from combat summons */
// ORIGIN(DROP_UNKNOWN,1,	"Dropped by an unknown monster %s")
ORIGIN(DROP_UNKNOWN,1,	"Брошено неизвестным монстром %s")
// ORIGIN(DROP_POLY,	2,	"Dropped by %s %s") /* from polymorphees */
ORIGIN(DROP_POLY,	2,	"Брошено %s %s") /* from polymorphees */
// ORIGIN(DROP_MIMIC,	2,	"Dropped by %s %s") /* from mimics */
ORIGIN(DROP_MIMIC,	2,	"Брошено %s %s") /* from mimics */
// ORIGIN(DROP_WIZARD,	2,	"Dropped by %s %s") /* from wizard mode summons */
ORIGIN(DROP_WIZARD,	2,	"Брошено %s %s") /* from wizard mode summons */
