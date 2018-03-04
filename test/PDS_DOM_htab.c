#include <errno.h>
#include <munit.h>

#include <tinypds.h>

#define TINY_PDS_DOM_IMPL
#include <dom/tinypds_dom.h>

static const char* keys[] = {
    "Solar Lottery",
    "The Man Who Japed",
    "The World Jones Made",
    "The Cosmic Puppets",
    "Eye in the Sky",
    "Time Out of Joint",
    "Dr. Futurity",
    "Vulcan's Hammer",
    "The Man in the High Castle",
    "The Game-Players of Titan",
    "The Penultimate Truth",
    "The Simulacra",
    "The Unteleported Man",
    "Clans of the Alphane Moon",
    "The Three Stigmata of Palmer Eldritch",
    "Martian Time-Slip",
    "Dr. Bloodmoney or How We Got Along After the Bomb",
    "Now Wait for Last Year",
    "The Crack in Space",
    "The Ganymede Takeover",
    "Counter-Clock World",
    "The Zap Gun",
    "Do Androids Dream of Electric Sheep?",
    "Galactic Pot-Healer",
    "We Can Build You",
    "Ubik",
    "A Maze of Death",
    "Our Friends from Frolix 8",
    "Flow My Tears, the Policeman Said",
    "Deus Irae",
    "A Scanner Darkly"	
};

static void* htab_setup(const MunitParameter params[], void* user_data) {
    int ret;
    
    (void)params;
    (void)user_data;
    
    PDS_htab *tab = (PDS_htab*)malloc(sizeof(PDS_htab));
    munit_assert_not_null(tab);
    
    ret = PDS_htab_create(tab);
    munit_assert_int(ret, !=, 0);
    
    return tab;
}

static void htab_tear_down(void* fixture) {
    PDS_htab *tab = (PDS_htab*)fixture;
    PDS_htab_destroy(tab);
    free(tab);
}

MunitResult htab_add_test(const MunitParameter params[], void* fixture) {
    int ret;
    uint32_t i, count;
    uint32_t n = sizeof(keys) / sizeof(keys[0]);
    
    uint32_t items[31];
    
    PDS_htab *tab = (PDS_htab*)fixture;
    
    (void)params;
    
    for(i=0; i<n; i++) {
        items[i] = i;
        ret = PDS_htab_add(tab, keys[i], strlen(keys[i]), items[i]);
        munit_assert_int(ret, !=, 0);
    }
    for(i=0, count=0; i<tab->capacity; i++) {
        if(tab->buckets[i].key) {
            munit_assert_string_equal(tab->buckets[i].key, keys[tab->buckets[i].index]);
            count++;
        }
    }
    munit_assert_int(count, ==, n);
    return MUNIT_OK;
}

MunitResult htab_del_test(const MunitParameter params[], void* fixture) {
    uint32_t items[31];
    uint32_t index;
    uint32_t i, j;
    int ret;
    
    uint32_t items_to_delete[] = { 24, 0, 30, 8, 1, 14, 7, 20 };
    
    uint32_t n = sizeof(keys) / sizeof(keys[0]);
    uint32_t m = sizeof(items_to_delete) / sizeof(items_to_delete[0]);
    
    PDS_htab *tab = (PDS_htab*)fixture;
    
    (void)params;

    for(i=0; i<n; i++) {
        items[i] = i;
        ret = PDS_htab_add(tab, keys[i], strlen(keys[i]), items[i]);
        munit_assert_int(ret, !=, 0);
    }

    for(j=0; j<m; j++) {
        uint32_t k = items_to_delete[j];
        ret = PDS_htab_del(tab, keys[k], strlen(keys[k]));
        munit_assert_int(ret, !=, 0);

        ret = PDS_htab_get(tab, keys[k], strlen(keys[k]));
        munit_assert_int(ret, ==, UINT32_MAX);
    }

        
    for(index=0; index<n; index++) {
        ret = PDS_htab_get(tab, keys[index], strlen(keys[index]));
        for(j=0; j<m; j++) {
            if(items_to_delete[j] == index) {
                munit_assert_int(ret, ==, UINT32_MAX);
                break;
            }
        }
        if(j >= m) {
            munit_assert_int(ret, ==, index);
        }
    }

    return MUNIT_OK;
}

static MunitTest htab_tests[] = {
    { "htab add", htab_add_test, htab_setup, htab_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { "htab del", htab_del_test, htab_setup, htab_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite htab_suite = {
    "htab test suite", htab_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main (int argc, char* const* argv) {
    return munit_suite_main(&htab_suite, NULL, argc, argv);
}