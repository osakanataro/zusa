#include <curses.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAXCHARS 10
#define MAXPATS 10
#define MAXQUEUES 20

struct anim_parts_t {
	char *s[40];
};
typedef struct anim_parts_t AParts;

struct anim_patterns_t {
	int x, y;
	int dx, dy;
	AParts *ap;
};
typedef struct anim_patterns_t APatterns;

struct anim_messtr_t {
	int x, y;
	char s[256];
};
typedef struct anim_messtr_t MesStrs;

struct anim_queue_t {
	int count;
	int msec;
	int message_f;
	int x, y;
	APatterns pat[MAXPATS];
	MesStrs str;
};
typedef struct anim_queue_t AQueue;

static int rx, ry;


static AParts gikonekos[] = {
	/* ---- ひとつめ ---- */
	{
		"  ∧ ∧　三＝",
		" (ﾟДﾟ )　三二＝",
		"⊂＼⊂＼三二＝",
		"   ＠ ＠",
		NULL
	},

	{
		"  ∧ ∧　二ﾆ",
		" (ﾟ∀ﾟ ) 三二＝",
		"⊂＼⊂＼ﾉ三二＝",
		"      ＠ ＠",
		NULL
	},

	{
		"  ∧ ∧",
		" (ﾟ∀ﾟ )",
		"⊂＼⊂＼ﾉミ",
		"      し　しミﾐ",
		"            ミ",
		NULL
	},

	{
		"    ∩ ∩",
		"   ／　 ﾉ）ﾐﾐ",
		"⊂/⊂　/ ミﾐ",
		"(｡Д｡ ）ﾐ",
		" V　V",
		NULL
	},

	{
		"  ∩ ∩",
		"  | ~~ |＼ﾐﾐ",
		"⊂|　　|つ ミﾐ",
		"  (｡Д｡ ）ﾐ",
		"   Ｖ　V",
		NULL
	},

	{
		"∩ ∩彡彡",
		" ＼　＼ 彡彡",
		"   ＼つ＼つ彡",
		"     (｡Д｡）",
		"      |ﾉ　|ノ",
		NULL
	},

	{
		"        ﾉ    ﾉ    ﾉ",
		"    ﾉﾉﾉ  ﾉﾉﾉ  ﾉﾉﾉ",
		"ﾉﾉ    ﾉﾉ  ﾉﾉ",
		"     ⊂⌒~⊃｡Д｡）⊃",
		"           Ｖ  Ｖ",
		NULL
	},

	{
		"　　 　　　　　　　　　　　　　（⌒´",
		"⊂⌒~⊃｡Д｡）⊃≡≡(´⌒; ;;;",
		"                 (´⌒(´⌒ ;;;",
		NULL
	},

	{
		"　　　　　　　　　　　　　　　　　　　　(´´",
		"　　　　　　　　　　　　　　　　　　(´⌒(´",
		"⊂⌒~⊃｡Д｡）⊃≡≡＝(´⌒;;;(´⌒",
		"                (´⌒(´⌒;; (´⌒;;",
		NULL
	},

	{
		"⌒~⊃｡Д｡）⊃≡＝",
		NULL
	},

	{
		"~~⊃｡Д｡）⊃＝",
		NULL
	},

	{
		"~~⊃｡Д｡）⊃",
		NULL
	},

	/* ---- ふたつめ---- */
	{
		" ∧∧　三＝",
		"(ﾟДﾟ)　三二＝",
		"⊂＼⊂＼三二＝",
		"    ＠ ＠",
		NULL
	},

	{
		"  ∧∧",
		" (ﾟДﾟ)",
		"⊂＼⊂＼ﾉミ",
		"   し　しミﾐ",
		"         ミﾐ",
		NULL
	},

	{
		"   ∧∧　　　）＝､",
		"⊂(ﾟДﾟ⊂⌒｀つ三ミ",
		NULL
	},

	{
		"   ∧∧　　　）",
		"⊂(ﾟДﾟ⊂⌒｀つ",
		NULL

	},

	{
		"￣￣￣￣￣∨￣￣￣",
		"　　  　∧∧　　　）　　　　(´",
		"　　⊂(ﾟДﾟ⊂⌒｀つ≡(´⌒;;;　  ｽﾞ",
		"            　 (´⌒;;  ",
		NULL
	},

	{
		"￣￣￣￣￣∨￣￣￣",
		"　 ∧∧　　　）　　　　(´⌒(´",
		"⊂(ﾟДﾟ⊂⌒｀つ≡≡(´⌒;　　　　ｽﾞｻﾞ",
		"              　(´⌒(´⌒ ;;;      ",
		NULL
	},

	{
		"￣￣￣￣￣∨￣￣￣　　　　　　　(´´",
		"　 ∧∧　　　）　　　　　　(´⌒(´",
		"⊂(ﾟДﾟ⊂⌒｀つ≡≡≡(´⌒;;;(´⌒ ｽﾞｻﾞ-----ｯ",
		"          　 (´⌒(´⌒;; (´⌒;;  ",
		NULL
	},

};

static AParts rantata[] = {
	{
		"|",
		"|",
		"|)",
		"|",
		"|",
		NULL
	},

	{
		"|",
		"| ＼",
		"|Д｀)",
		"|⊂",
		"|",
		NULL
	},

	{
		"ﾀﾞﾚﾓｲﾅｲ..",
		NULL
	},

	{
		"ｵﾄﾞﾙﾅﾗｲﾏﾉｳﾁ",
		NULL
	},

	{
		"♪     Å",
		"  ♪ ／  ＼      ﾗﾝﾀ ﾗﾝﾀ",
		"  ヽ(´Д｀;)ﾉ    ﾗﾝﾀ ﾀﾝ",
		"    (へ    )       ﾗﾝﾀ ﾗﾝﾀ",
		"          >         ﾀﾝ",
		NULL
	},

	{
		"    ♪  Å",
		" ♪   ／  ＼      ﾗﾝﾀ ﾗﾝﾀ",
		"   ヽ(;´Д｀)ﾉ    ﾗﾝﾀ ﾀﾝ",
		"     (    へ)       ﾗﾝﾀ ﾗﾝﾀ",
		"      く             ﾀﾝ",
		NULL
	},

	{
		"        Å",
		"    _ ／  ＼",
		"   ( (;´Д｀)",
		"   」」/   ＼  ﾐﾗﾚﾀ...",
		NULL
	},
};

static AParts tipoff[] = {
	{
		"               ∧＿∧",
		"              （ ´Д｀）",
		"             ／,　　/",
		"           (ぃ９　　|",
		"           /　　　 /、",
		"          /　　　∧_二つ",
		"         /　　　/",
		"        /　　　 ＼",
		"       /　 /~＼　 ＼",
		"      /　 /    >　 ）",
		"    ／　ノ    /　／",
		"   /　／     / ./.",
		"  / ./     （　ヽ､",
		"（   _)      ＼__つ",
		NULL
	},

	{
		" (ﾟдﾟ)",
		"ﾟ(　 )－",
		" /  >",
		NULL

	},

	{
		"  ／￣￣￣￣￣￣",
		"＜",
		"  ＼＿＿＿＿＿＿",
		NULL
	},

	{
		"<",
		NULL
	},

	{
		"通報しますた",
		NULL
	},

	{
		"ｼﾏｽﾀ",
		NULL
	}
};

static AParts okottazou[] = {
	{
		"／￣￣￣￣￣＼",
		"|　・　Ｕ　　|",
		"| |ι　　　　|つ",
		"Ｕ｜｜￣￣｜｜",
		"   ￣      ￣",
		NULL
	},

	{
		"もうおこったぞう",
		NULL
	},

	{
		"どかーん！！",
		"(⌒⌒⌒)",
		"  ｜｜",
		NULL
	},
};

static AParts kusoproc[] = {
	{
		"  　　         ∧＿∧　　　　　　∧＿∧",
		"  　       _（  ´∀｀） 　　　（´∀｀　）",
		"init --> 三(⌒),　　　ノ⊃　　 （　　　　）  糞プロセスは",
		"          　￣/　/)　） 　　　　|　|　　|",
		"          .  〈＿）＼_）　　　（__（_＿_）",
		NULL
	},

	{
		"            ∧＿∧　　   .∧＿∧",
		"          （　　´∀）　（´∀｀　）",
		"init -->  ≡≡三三ニ⌒)          .）  立ち上げんなって",
		"           /　 /)　）￣  .|　|　　|",
		"         〈＿_）＿_）   （__（_＿_）",
		NULL
	},


	{
		"            ∧＿∧　 ,_  ∧＿∧",
		"         （ 　　´）ノ ）;)∀｀）",
		"init -->  /　　￣,ノ''　   　　）　　言ったろうが",
		"       Ｃ　　　/~　   /　/ 　 / ",
		"        / 　 /　〉  （__（＿_./",
		"        ＼__）＼）",
		NULL
	},

	{
		"                                ヽｌ /／",
		"            ∧＿∧(⌒)        ――★―――",
		"init --> （　　　　）/|ｌ    　／/ | ヽ　　ヴォケがーー！",
		"         （/　　　ノｌ|ｌｌ ／  /　|　ヽ",
		"         （Ｏ　　ノ彡''        /　 |",
		"         /　 ./ 〉",
		"         ＼__）_）",
		NULL
	}
};

static AParts mona8kick[] = {
	{
		"　　　　　　　　/ ）",
		" 　　　　　　 ./ /",
		" 　　　　　　 / /",
		"　　　　　　 / /",
		" 　　　　　./ /＿Λ　　　 　, -つ",
		" 　　　　　/ / ´Д｀）　.／__ノ",
		"　　　　　/　　　　＼　／ /　　    ⊂_ヽ、",
		"　　　　 .|　　　　へ／／　　　　　　.＼＼　Λ＿Λ",
		"　　　　  |　　　　レ /、   　 　　　　＼（　´Д｀）",
		"　　　　  |　　　　／.　　　　　　　　　.　>　　⌒ヽ",
		"　　　　　/　　　／　　　　　　　　　　　　/　　　へ＼",
		"　　　　 /　  ／　　　　　　　　　　　　　/　　 / 　＼＼",
		"　　　　/　 /　　　　　　　　　　　　　　レ　　ノ　 　ヽ_つ",
		"　　　／ ノ　　　　　　　　　　　　　　　/　 /",
		"　　_/ ／　　　　　　　　　　　　　　　 /　 /|",
		"　ノ ／　　　　　　　　　　　　　　　 （　（ 、",
		"⊂ -'　　　　　　　　　　　　　　　　　 |　|、＼",
		"    　　　　　　　　　　　　　　　　.　 |　/　＼ ⌒l",
		"    　　　　　　　　　　　　　　 　　　 |　|　　）/",
		"    　　　　　　　　　　　　　　　　　 ノ　）　 し'",
		"    　　　　　　　　　　　　　　 　　 (＿／",
		NULL
	},

	{
		"／￣￣￣￣￣￣￣",
		"| 吐けよ、core!",
		"＼　　　　　　　",
		"　￣|／￣￣￣￣",
		NULL
	},

	{
		"／￣￣￣",
		"|",
		"＼",
		"　￣∨￣￣￣￣",
		NULL
	}
};

static AParts kossori[] = {
	{
		"|",
		"|",
		"|",
		"|",
		"|",
		"|",
	},

	{
		"",
		"⌒彡　　　n=n++ ……",
		"冫、）",
		"` ／",
		" /",
		"/",
		NULL
	},

	{
		"",
		" 　サッ",
		"）彡",
		"",
		"",
		"",
		NULL
	}
};

static AParts unknown1[] = {
	{
		"ヽ￣￣ﾞ＼",
		"　＼　ヽ ＼",
		"  　ヽ　　ﾞ!",
		"　  /   　 |",
		"　／　 　 ,.ｲ              　　　(⌒;:)",
		"∠＿＿＿／　:|｀ヽ.、　　　　　(⌒　::;",
		" 　 　 ﾉ　　　 　'!　　 ＼-､( ⌒:: : : ..: j",
		"＿,.=''1　　　　　 ヽ,　 　...;::⌒￣⌒ヾ",
		" ヽ' 　i|　　　　: :::.....:: :::　;　 :::)",
		"　　　 !　　　　　　,,..:;::::;::（:: :;;:;:)",
		"　　　ノ.....:::;::::／;:/::::::;　:.....:ﾉ",
		NULL
	},

	{
		"　／￣￣￣￣￣￣￣",
		"＜",
		"  ＼＿＿＿＿＿",
		NULL
	}
};

static AParts pengiso[] = {
	{
		"　   ______________",
		"　  /　　　　　　　＼",
		"__ |　　・　　　・　 ＼___",
		"＼ |　　　Σ　　　   ／　/",
		"  ~|　/~~~~~~~~~＼      |",
		" 　|/　　　　　　 ＼.   |",
		" 　|　　　　　　　　　  |",
		"　　~~しつ~~~~しつ~~~~~~",
		NULL
	}
};

static AParts userhyoka[] = {
	{
		" 優良user　　　　　　普通　　　　　　クソuser",
		"┝━━━━━━━━━━┿━━━━━━━━━━┥",
		NULL
	},

	{
		"　　　88彡ミ8。　 /)",
		"　　8ノ/ノ＾＾ヾ8(i)))",
		"　　 |(| ∩ ∩||/ / 　　＜ココ!",
		"　　从ゝ__ ▽.从  /",
		"　　 /||＿、＿|| /",
		"　　/ （＿＿＿)",
		"　　＼(ミ＿,＿(",
		"　　　/. 　＿　＼",
		"　　／ ／　  ＼_.〉",
		"　／ ／　　　／ /",
		"(二_/　　　 （二）",
		NULL
	}
};

static AParts grounds[] = {
	{
		"石",
		NULL

	}
};


enum {
	GND_STONE = 0
};

enum {
	/* ---- ひとつめ ---- */
	GNEKO_DASH1 = 0,
	GNEKO_DASH2,
	GNEKO_JUMP1,
	GNEKO_ROT1,
	GNEKO_ROT2,
	GNEKO_ROT3,
	GNEKO_TDOWN1,
	GNEKO_SLIDE1,
	GNEKO_SLIDE2,
	GNEKO_SLIDE3,
	GNEKO_WABI1,
	GNEKO_WABI2,

	/* ---- ふたつめ ---- */
	GNEKO_DASH3,
	GNEKO_JUMP2,
	GNEKO_FLY1,
	GNEKO_TDOWN2,
	GNEKO_SLIDE4,
	GNEKO_SLIDE5,
	GNEKO_SLIDE6
};

enum {
	RANTATA_PEEP1 = 0,
	RANTATA_PEEP2,
	RANTATA_PEEP3,
	RANTATA_PEEP4,
	RANTATA_DANCE1,
	RANTATA_DANCE2,
	RANTATA_END
};

enum {
	TIPOFF_MONA1 = 0,
	TIPOFF_MONA2,
	TIPOFF_MESWIN1,
	TIPOFF_MESWIN2,
	TIPOFF_MES1,
	TIPOFF_MES2
};

enum {
	OKOTTA_ZOU  = 0,
	OKOTTA_MES1,
	OKOTTA_BANG
};

enum {
	KUSOPROC_SWING1  = 0,
	KUSOPROC_SWING2,
	KUSOPROC_HIT,
	KUSOPROC_STAR,
};

enum {
	MONA8KICK_BODY = 0,
	MONA8KICK_MWIN1,
	MONA8KICK_MWIN2
};

enum {
	KOSSORI_WALL = 0,
	KOSSORI_LOOK,
	KOSSORI_HIDE
};

enum {
	UNK1_SLIDE = 0,
	UNK1_MWIN
};

enum {
	PENGISO_BODY = 0,
	PENGISO_MES
};

enum {
	USERHYOKA_METER = 0,
	USERHYOKA_BODY
};

enum {
	ANIM1_PATS = 14,
	ANIM2_PATS = 8,
	ANIM3_PATS = 9,
	ANIM4_PATS = 4,
	ANIM5_PATS = 3,
	ANIM6_PATS = 4,
	ANIM7_PATS = 3,
	ANIM8_PATS = 3,
	ANIM9_PATS = 3,
	ANIM10_PATS = 2,
	ANIM11_PATS = 1,
};

static AQueue userhyoka_anim[] = {
	{
		30, 80000, 0, 0, 0,
		{
			{20, 1, 0, 0, &userhyoka[USERHYOKA_METER]},
			{1060, 3, 0, 0, &userhyoka[USERHYOKA_BODY]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},
};

static AQueue pengiso_anim[] = {
	{
		10, 80000, 0, 0, 0,
		{
			{10, 10, 0, 0, &pengiso[PENGISO_BODY]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 1, 0, 0,
		{
			{10, 10, 0, 0, &pengiso[PENGISO_BODY]},
			{0, 0, 0, 0, NULL}
		},
		{15, 8, "みんななかよくしてｸﾋﾟｰ"}
	},
};

static AQueue unkown1_anim[] = {
	{
		2, 80000, 0, 0, 0,
		{
			{35, 10, 0, 0, &unknown1[UNK1_SLIDE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		2, 80000, 0, 0, 0,
		{
			{15, 10, 0, 0, &unknown1[UNK1_SLIDE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 1, 0, 0,
		{
			{5, 10, 0, 0, &unknown1[UNK1_SLIDE]},
			{17, 12, 0, 0, &unknown1[UNK1_MWIN]},
			{0, 0, 0, 0, NULL}
		},
		{20, 13, "%p げっとずさー"}
	}
};

static AQueue kossori_anim[] = {
	{
		20, 80000, 0, 0, 0,
		{
			{0, 10, 0, 0, &kossori[KOSSORI_WALL]},
			{2, 10, 0, 0, &kossori[KOSSORI_LOOK]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},


	{
		5, 80000, 0, 0, 0,
		{
			{0, 10, 0, 0, &kossori[KOSSORI_WALL]},
			{2, 10, 0, 0, &kossori[KOSSORI_HIDE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 0, 0, 0,
		{
			{0, 10, 0, 0, &kossori[KOSSORI_WALL]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},
};

static AQueue mona8kick_anim[] = {
	{
		10, 80000, 0, 0, 0,
		{
			{2, 0, 0, 0, &mona8kick[MONA8KICK_BODY]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		10, 80000, 0, 0, 0,
		{
			{2, 0, 0, 0, &mona8kick[MONA8KICK_BODY]},
			{20, 0, 0, 0, &mona8kick[MONA8KICK_MWIN1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 1, 0, 0,
		{
			{2, 0, 0, 0, &mona8kick[MONA8KICK_BODY]},
			{20, 0, 0, 0, &mona8kick[MONA8KICK_MWIN1]},
			{36, 2, 0, 0, &mona8kick[MONA8KICK_MWIN2]},
			{0, 0, 0, 0, NULL}
		},
		{38, 3, "%p さんおひさしぶり"}
	},
};

static AQueue kusoproc_anim[] = {
	{
		20, 80000, 1, 0, 0,
		{
			{2, 10, 0, 0, &kusoproc[KUSOPROC_SWING1]},
			{0, 0, 0, 0, NULL}
		},
		{45, 14, "<-- %p"}
	},

	{
		20, 80000, 1, 0, 0,
		{
			{2, 10, 0, 0, &kusoproc[KUSOPROC_SWING2]},
			{0, 0, 0, 0, NULL}
		},
		{38, 14, "<-- %p"}
	},

	{
		20, 80000, 1, 0, 0,
		{
			{2, 10, 0, 0, &kusoproc[KUSOPROC_HIT]},
			{0, 0, 0, 0, NULL}
		},
		{34, 14, "<-- %p"}
	},

	{
		20, 80000, 1, 0, 0,
		{
			{2, 10, 0, 0, &kusoproc[KUSOPROC_STAR]},
			{0, 0, 0, 0, NULL}
		},
		{43, 10, "<-- %p"}
	}
};

static AQueue okottazou_anim[] = {
	{
		20, 80000, 0, 0, 0,
		{
			{20, 10, 0, 0, &okottazou[OKOTTA_ZOU]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		10, 80000, 0, 0, 0,
		{
			{20, 10, 0, 0, &okottazou[OKOTTA_ZOU]},
			{20, 15, 0, 0, &okottazou[OKOTTA_MES1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 0, 0, 0,
		{
			{20, 10, 0, 0, &okottazou[OKOTTA_ZOU]},
			{20, 15, 0, 0, &okottazou[OKOTTA_MES1]},
			{20, 7, 0, 0, &okottazou[OKOTTA_BANG]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	}
};

static AQueue tipoff_anim[] = {
	{
		10, 80000, 0, 0, 0,
		{
			{5, 10, 0, 0, &tipoff[TIPOFF_MONA1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		10, 80000, 0, 0, 0,
		{
			{5, 10, 0, 0, &tipoff[TIPOFF_MONA1]},
			{30, 21, 0, 0, &tipoff[TIPOFF_MONA2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		8, 80000, 0, 0, 0,
		{
			{5, 10, 0, 0, &tipoff[TIPOFF_MONA1]},
			{30, 21, 0, 0, &tipoff[TIPOFF_MONA2]},
			{32, 10, 0, 0, &tipoff[TIPOFF_MESWIN1]},
			{35, 11, 0, 0, &tipoff[TIPOFF_MES1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 0, 0, 0,
		{
			{5, 10, 0, 0, &tipoff[TIPOFF_MONA1]},
			{30, 21, 0, 0, &tipoff[TIPOFF_MONA2]},
			{32, 10, 0, 0, &tipoff[TIPOFF_MESWIN1]},
			{35, 11, 0, 0, &tipoff[TIPOFF_MES1]},
			{40, 21, 0, 0, &tipoff[TIPOFF_MESWIN2]},
			{41, 21, 0, 0, &tipoff[TIPOFF_MES2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	}
};

static AQueue gikoanim1[] = {
	{
		1, 80000, 0, 0, 0,
		{
			{66, 6, 0, 0, &gikonekos[GNEKO_DASH1]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		1, 40000, 0, 0, 0,
		{
			{56, 6, 0, 0, &gikonekos[GNEKO_DASH2]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		1, 50000, 0, 0, 0,
		{
			{50, 5, 0, 0, &gikonekos[GNEKO_JUMP1]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		1, 90000, 0, 0, 0,
		{
			{46, 4, 0, 0, &gikonekos[GNEKO_ROT1]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		1, 90000, 0, 0, 0,
		{
			{42, 4, 0, 0, &gikonekos[GNEKO_ROT2]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		1, 90000, 0, 0, 0,
		{
			{36, 4, 0, 0, &gikonekos[GNEKO_ROT3]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		2, 50000, 0, 0, 0,
		{
			{30, 6, 0, 0, &gikonekos[GNEKO_TDOWN1]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		3, 40000, 0, 0, 0,
		{
			{28, 8, 0, 0, &gikonekos[GNEKO_SLIDE1]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		3, 40000, 0, 0, 0,
		{
			{20, 7, 0, 0, &gikonekos[GNEKO_SLIDE2]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		6, 40000, 0, 0, 0,
		{
			{14, 7, 0, 0, &gikonekos[GNEKO_SLIDE2]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		8, 40000, 0, 0, 0,
		{
			{10, 9, 0, 0, &gikonekos[GNEKO_SLIDE3]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		10, 45000, 0, 0, 0,
		{
			{4, 9, 0, 0, &gikonekos[GNEKO_WABI1]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		20, 50000, 0, 0, 0,
		{
			{0, 9, 0, 0, &gikonekos[GNEKO_WABI2]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 1, 0, 0,
		{
			{0, 9, 0, 0, &gikonekos[GNEKO_WABI2]},
			{60, 9, 0, 0, &grounds[GND_STONE]},
			{0, 0, 0, 0, NULL}
		},
		{30, 8, "ゴルァ"}
	}
};


static AQueue gikoanim2[] = {
	{
		5, 20000, 0, 0, 0,
		{
			{66, 6, 0, 0, &gikonekos[GNEKO_DASH3]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}

	},

	{
		1, 10000, 0, 0, 0,
		{
			{56, 6, 0, 0, &gikonekos[GNEKO_JUMP2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		15, 10000, 0, 0, 0,
		{
			{50, 6, 0, 0, &gikonekos[GNEKO_FLY1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		5, 20000, 0, 0, 0,
		{
			{40, 6, 0, 0, &gikonekos[GNEKO_FLY1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		2, 30000, 0, 0, 0,
		{
			{32, 8, 0, 0, &gikonekos[GNEKO_TDOWN2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		5, 20000, 1, 0, 0,
		{
			{25, 7, 0, 0, &gikonekos[GNEKO_SLIDE4]},
			{0, 0, 0, 0, NULL}
		},
		{30, 6, "%p 番"}
	},

	{
		4, 10000, 1, 0, 0,
		{
			{15, 7, 0, 0, &gikonekos[GNEKO_SLIDE5]},
			{0, 0, 0, 0, NULL}
		},
		{15, 6, "%p 番ゲット"}
	},

	{
		30, 80000, 1, 0, 0,
		{
			{5, 7, 0, 0, &gikonekos[GNEKO_SLIDE6]},
			{0, 0, 0, 0, NULL}
		},
		{6, 6, "%p 番ゲットォォｫｫ！！"}
	},
};

static AQueue rantata_anim[] = {
	{
		30, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_PEEP1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}

	},

	{
		30, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_PEEP2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_PEEP2]},
			{10, 8, 0, 0, &rantata[RANTATA_PEEP3]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_PEEP2]},
			{10, 8, 0, 0, &rantata[RANTATA_PEEP3]},
			{26, 8, 0, 0, &rantata[RANTATA_PEEP4]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		15, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_DANCE1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		20, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_DANCE2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		15, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_DANCE1]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		20, 50000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_DANCE2]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},

	{
		30, 80000, 0, 0, 0,
		{
			{0, 6, 0, 0, &rantata[RANTATA_END]},
			{0, 0, 0, 0, NULL}
		},
		{0, 0, ""}
	},
};


void draw(int x, int y, APatterns *a);
void drawmes(int x, int y, MesStrs *m);
void usage();

void
usage()
{
	printf("\n\nusage: zusa -123456789abh\n");
	exit(1);
}

void
draw(int x, int y, APatterns *a)
{
	int i, yy;

	yy = a->y;

	if (a->x > 999) {
		if (rx == 0)
			rx = (rand() & RAND_MAX) % (a->x - 1000);

		a->x = rx;
	}

	if (a->y > 999) {
		if (ry == 0)
			ry = (rand() & RAND_MAX) % (a->x - 1000);

		a->y = rx;
	}

	for (i = 0; ((AParts *)(a->ap))->s[i] != NULL; i++) {
		move(y + yy++, x + a->x);
		addstr(((AParts *)(a->ap))->s[i]);
	}

	a->x += a->dx;
	a->y += a->dy;
}

void
drawmes(int x, int y, MesStrs *m)
{
	char buf[256], buf2[256];
	char *p;

	if (!(m->s))
		return;

	buf[0] = buf2[0] = '\0';
	p = strchr(m->s, '%');
	if (p) {
		strncat(buf, m->s, p - (m->s));

		switch (*(p + 1)) {
		case 'p':
			snprintf(buf2, 256, "%d", (int)getpid());
			break;
		case 'd':
			snprintf(buf2, 256, "%s", "hoge");
			break;
		default:
			break;
		}
		strncat(buf, buf2, 256);
		strncat(buf, p + 2, 256);
	} else {
		strncat(buf, m->s, 256);
	}

	if (m->x > 999) {
		if (rx == 0)
			rx = (rand() & RAND_MAX) % (m->x - 1000);

		m->x = rx;
	}

	if (m->y > 999) {
		if (ry == 0)
			ry = (rand() & RAND_MAX) % (m->x - 1000);

		m->y = ry;
	}
	move(y + m->y, x + m->x);
	addstr(buf);
}



int
main(int argc, char **argv)
{
	int i, j, n;
	int bx, by;
	AQueue queue[MAXQUEUES], *q;
	extern char *optarg;
	extern int optind;
	int ch;
	int rndm;

	srand((unsigned int)time(NULL));

	for (i = 0; i < MAXQUEUES; i++)
		queue[i].count = -1;

	q = gikoanim2;
	n = ANIM2_PATS;

	if( (ch = getopt(argc, argv, "123456789abh")) == -1){
		rndm=rand()%((2^15)-1);
		ch=rndm;
		
//		printf("**** %d ****\n",ch);
	}

	switch (ch) {
	case '1':
	case 1:
		q = gikoanim1;
		n = ANIM1_PATS;
		break;
	case '2':
	case 2:
		q = gikoanim2;
		n = ANIM2_PATS;
		break;
	case '3':
	case 3:
		q = rantata_anim;
		n = ANIM3_PATS;
		break;
	case '4':
	case 4:
		q = tipoff_anim;
		n = ANIM4_PATS;
		break;
	case '5':
	case 5:
		q = okottazou_anim;
		n = ANIM5_PATS;
		break;
	case '6':
	case 6:
		q = kusoproc_anim;
		n = ANIM6_PATS;
		break;
	case '7':
	case 7:
		q = mona8kick_anim;
		n = ANIM7_PATS;
		break;
	case '8':
	case 8:
		q = kossori_anim;
		n = ANIM8_PATS;
		break;
	case '9':
	case 9:
		q = unkown1_anim;
		n = ANIM9_PATS;
		break;
	case 'a':
	case 10:
		q = pengiso_anim;
		n = ANIM10_PATS;
		break;
	case 'b':
	case 11:
		q = userhyoka_anim;
		n = ANIM11_PATS;
		break;
	case 'h':
	case '?':
	/* fall-thru */
	default:
		usage();
		break;
	}

	argc -= optind;
	argv += optind;

	for (i = 0; i < n; i++) {
		queue[i] = q[i];
	}

	initscr();
	signal(SIGINT, SIG_IGN);
	noecho();
	leaveok(stdscr, TRUE);
	scrollok(stdscr, TRUE);

	bx = by = 0;
	while (1) {
		n = 0;
		rx = ry = 0;
		for (i = 0; i < MAXQUEUES; i++) {
			q = &queue[i];
			if (q->count < 0)
				continue;
			
			n++;
			while (q->count > 0) {
				for (j = 0; ((APatterns)q->pat[j]).ap != NULL; j++) {
					bx = ((bx == 0) && (q->x > 999)) ? (q->x - 1000) : (bx);
					by = ((by == 0) && (q->y > 999)) ? (q->y - 1000) : (by);
					draw(bx, by, &(q->pat[j]));
				}

				if (q->message_f)
					drawmes(bx, by, &(q->str));

				usleep(q->msec-3000);
				refresh();
				(q->count)--;
			}
			q->count = -1;
			clear();
		}

		if (n == 0)
			break;
	}

	endwin();
	return 0;
}
