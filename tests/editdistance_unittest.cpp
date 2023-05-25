
#include "../src/tools.h"
#include "gtest/gtest.h"
#include "table_fixture.cpp"

TEST(editDistance, editDistanceWithoutTable) {
    EXPECT_EQ(editDistance(L"", L""), 0.);
    EXPECT_EQ(editDistance(L"open", L""), 4.);
    EXPECT_EQ(editDistance(L"", L"river"), 5.);
    EXPECT_EQ(editDistance(L"elephant", L"eephlant"), 2);
    EXPECT_EQ(editDistance(L"sunday", L"monday"), 2.);
    EXPECT_EQ(editDistance(L"stack", L"steak"), 2.);
    EXPECT_EQ(editDistance(L"Cat", L"car"), 2.);
    EXPECT_EQ(editDistance(L"lake", L"lame"), 1.);
    EXPECT_EQ(editDistance(L"write", L"right"), 4.);
    EXPECT_EQ(editDistance(L"house", L"horses"), 2.);
    EXPECT_EQ(editDistance(L"chair", L"choir"), 1.);
    EXPECT_EQ(editDistance(L"rain", L"train"), 1.);
    EXPECT_EQ(editDistance(L"open", L"opne"), 1.);
    EXPECT_EQ(editDistance(L"chair", L"chiar"), 1.);
    EXPECT_EQ(editDistance(L"examples", L"examlpe"), 2.);
    EXPECT_EQ(editDistance(L"algorithm", L"algorihm"), 1.);
    EXPECT_EQ(editDistance(L"elephant", L"eelphant"), 1.);
    EXPECT_EQ(editDistance(L"mississippi", L"missippi"), 3.);
    EXPECT_EQ(editDistance(L"elephant", L"relevant"), 3.);
    EXPECT_EQ(editDistance(L"hypothesis", L"hypocrisy"), 5.);
    EXPECT_EQ(editDistance(L"phenomenon", L"penitentiary"), 8.);
    EXPECT_EQ(editDistance(L"exaggerate", L"extravagant"), 8.);
    EXPECT_EQ(editDistance(L"necessity", L"negotiable"), 8.);
    EXPECT_EQ(editDistance(L"sufficient", L"insufficien\u0163"), 3.);
    EXPECT_EQ(editDistance(L"ambiguous", L"a\u1e3fbivalence"), 8.);
    EXPECT_EQ(editDistance(L"\u13afccommodate", L"accomplish"), 7.);
    EXPECT_EQ(editDistance(L"disastrous", L"spontaneou\u015b"), 9.);
    EXPECT_EQ(editDistance(L"\u2130xponential", L"experimental"), 6.);
}

TEST_F(TableFixture, editDistanceWithTable){
    EXPECT_EQ(editDistance(L"aah", L"aah", *st_), 0.);
    EXPECT_EQ(editDistance(L"", L"", *st_), 0.);
    EXPECT_EQ(editDistance(L"Open", L"", *st_), 4.);
    EXPECT_EQ(editDistance(L"", L"River", *st_), 5.);

    // KEYBOARD MISTAKE
    EXPECT_EQ(editDistance(L"algorithm", L"algoritjm", *st_), .75);
    EXPECT_EQ(editDistance(L"synchronixe", L"synchronize", *st_), .75);
    EXPECT_EQ(editDistance(L"difficult", L"diffidult", *st_), .75);
    EXPECT_EQ(editDistance(L"resiluent", L"resilient", *st_), .75);
    EXPECT_EQ(editDistance(L"alliance", L"allisnce", *st_), .75);

    // VISUAL SIMILAR CHAR
    float test = editDistance(L"necessary", L"recessary", *st_);
    EXPECT_TRUE(0.84 < test and test < 0.86);
    test = editDistance(L"opporlunity", L"opportunity", *st_);
    EXPECT_TRUE(0.84 < test and test < 0.86);
    test = editDistance(L"catastrophe", L"catactrophe", *st_);
    EXPECT_TRUE(0.84 < test and test < 0.86);
    test = editDistance(L"relevont", L"relevant", *st_);
    EXPECT_TRUE(0.84 < test and test < 0.86);
    test = editDistance(L"renovate", L"renonate", *st_);
    EXPECT_TRUE(0.84 < test and test < 0.86);

    // SIMILAR CHARS
    EXPECT_EQ(editDistance(L"white", L"wh\u03cate", *st_), .5);
    EXPECT_EQ(editDistance(L"brilli\u00e5nt", L"brilliant", *st_), .5);
    EXPECT_EQ(editDistance(L"agile", L"agi\u0142e", *st_), .5);
    EXPECT_EQ(editDistance(L"ana\u1d89chy", L"anarchy", *st_), .5);
    EXPECT_EQ(editDistance(L"elephant", L"elephan\u03c4", *st_), .5);

    // CAPITAL
    EXPECT_EQ(editDistance(L"Frog", L"frog", *st_), .25);
    EXPECT_EQ(editDistance(L"water", L"Water", *st_), .25);
    EXPECT_EQ(editDistance(L"Horse", L"horse", *st_), .25);

    // DIFFERENT COMBINATION
    EXPECT_EQ(editDistance(L"Illustrious", L"illustriuos", *st_), 1.25);
    EXPECT_EQ(editDistance(L"anthropology", L"amthopology", *st_), 1.75);
    EXPECT_EQ(editDistance(L"Galaxy", L"ga\u0142axy", *st_), .75);
    test = editDistance(L"Whispre", L"Vhisper", *st_);
    EXPECT_TRUE(1.84 < test and test < 1.86);
    EXPECT_EQ(editDistance(L"Thunder", L"yhunlder", *st_), 2.);
    EXPECT_EQ(editDistance(L"Raimcoat", L"raincao\u03c4", *st_), 2.5);
    test = editDistance(L"Flrbfty", L"Firefly", *st_);
    EXPECT_TRUE(2.69 < test and test < 2.71);
    EXPECT_EQ(editDistance(L"Velvet", L"welv\u011bt", *st_), 1.5);
    test = editDistance(L"Castle", L"\u03c2asltc", *st_);
    EXPECT_TRUE(2.59 < test and test < 2.61);
    test = editDistance(L"Carousel", L"caraucel", *st_);
    EXPECT_TRUE(1.94 < test and test < 1.96);
    EXPECT_EQ(editDistance(L"Sail\u00edng", L"Ssiling", *st_), 1.25);
    EXPECT_EQ(editDistance(L"Luaghtr", L"Laughter", *st_), 2.);
    EXPECT_EQ(editDistance(L"Puzzle", L"\u1d88ussle", *st_), 2.25);
    test = editDistance(L"Mavshnalow", L"marshmallow", *st_);
    EXPECT_TRUE(2.84 < test and test < 2.86);
    EXPECT_EQ(editDistance(L"Serenade", L"s\u011br\u011bnad\u011b", *st_), 1.75);
    test = editDistance(L"wamdevlust", L"Wanderlust", *st_);
    EXPECT_TRUE(1.84 < test and test < 1.86);
    test = editDistance(L"Slardust", L"Star", *st_);
    EXPECT_TRUE(4.84 < test and test < 4.86);
    EXPECT_EQ(editDistance(L"Pr\u011bl\u011bvanr", L"Relevant", *st_), 3);
    test = editDistance(L"Accomplish", L"accamptish", *st_);
    EXPECT_TRUE(1.94 < test and test < 1.96);
    test = editDistance(L"Eccentric", L"essentr\u012bs", *st_);
    EXPECT_TRUE(3.29 < test and test < 3.31);
}
