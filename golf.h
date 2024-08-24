#ifndef GOLF_H
#define GOLF_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

enum GolfFace {
  GOLF_FACE_NONE  = 0,
  GOLF_FACE_ACE   = 1,
  GOLF_FACE_TWO   = 2,
  GOLF_FACE_THREE = 3,
  GOLF_FACE_FOUR  = 4,
  GOLF_FACE_FIVE  = 5,
  GOLF_FACE_SIX   = 6,
  GOLF_FACE_SEVEN = 7,
  GOLF_FACE_EIGHT = 8,
  GOLF_FACE_NINE  = 9,
  GOLF_FACE_TEN   = 10,
  GOLF_FACE_JACK  = 11,
  GOLF_FACE_QUEEN = 12,
  GOLF_FACE_KING  = 13, // 0b1101
};

#define DECKSIZE 104

#define GOLF_REMOVED_CARD 0b10000LL

// A card uses 5 bits:
// - the leftmost bit is a flag for the visibility.
// - The other four bits indicate the face.
#define GOLF_FACE_SIZE 4
#define GOLF_STATE_SIZE 1
#define GOLF_CARD_SIZE (GOLF_FACE_SIZE + GOLF_STATE_SIZE)


#define GOLF_FACE_MASK 0b01111LL
#define GOLF_HAND_CARD_MASK 0b11111LL
#define GOLF_CARD_VISIBLE 0b10000LL
#define GOLF_HAND_MASK 0b111111111111111111111111111111111111111111111LL
#define SHIFTWIDTH GOLF_CARD_SIZE

#define SHIFT_TOP_LFT (SHIFTWIDTH * 0)
#define SHIFT_TOP_MID (SHIFTWIDTH * 1)
#define SHIFT_TOP_RGT (SHIFTWIDTH * 2)
#define SHIFT_MID_LFT (SHIFTWIDTH * 3)
#define SHIFT_MID_MID (SHIFTWIDTH * 4)
#define SHIFT_MID_RGT (SHIFTWIDTH * 5)
#define SHIFT_BOT_LFT (SHIFTWIDTH * 6)
#define SHIFT_BOT_MID (SHIFTWIDTH * 7)
#define SHIFT_BOT_RGT (SHIFTWIDTH * 8)

#define MASK_TOP_LFT (GOLF_HAND_CARD_MASK << SHIFT_TOP_LFT)
#define MASK_TOP_MID (GOLF_HAND_CARD_MASK << SHIFT_TOP_MID)
#define MASK_TOP_RGT (GOLF_HAND_CARD_MASK << SHIFT_TOP_RGT)
#define MASK_MID_LFT (GOLF_HAND_CARD_MASK << SHIFT_MID_LFT)
#define MASK_MID_MID (GOLF_HAND_CARD_MASK << SHIFT_MID_MID)
#define MASK_MID_RGT (GOLF_HAND_CARD_MASK << SHIFT_MID_RGT)
#define MASK_BOT_LFT (GOLF_HAND_CARD_MASK << SHIFT_BOT_LFT)
#define MASK_BOT_MID (GOLF_HAND_CARD_MASK << SHIFT_BOT_MID)
#define MASK_BOT_RGT (GOLF_HAND_CARD_MASK << SHIFT_BOT_RGT)

// These are equivalent but named for clarity.

// Uses 5 bits.
typedef uint64_t golfcard_t;
// Uses 15 bits.
typedef uint64_t golfsegment_t;
// Uses 55 bits.
typedef uint64_t golfhand_t;

struct GolfRound {
  // The deck.
  golfcard_t deck[DECKSIZE];
  // How many cards are still in the deck.
  size_t decksz;
  golfcard_t discard[DECKSIZE];
  size_t discardsz;
  // The currently open card.
  golfcard_t open;

  // The "hand" of each player.
  golfhand_t players[10];
  // How many players the game has.
  size_t playersz;
  // Whose turn it is, must be in range [0, playersz].
  size_t whoseturn;
};

struct GolfGame {
  // How many players the game has.
  size_t playersz;
  int32_t scores[10];
  // Who the dealer is, must be in range [0, playersz].
  size_t dealer;
};

// Wrapper to create a new card.
golfcard_t
golf_card(enum GolfFace const face, bool const visible);

int32_t
golf_card_points(golfcard_t const card);

int32_t
golf_hand_points(golfhand_t hand);

// Wrapper to create a hand.
golfhand_t
golf_make_hand(
    golfcard_t const, golfcard_t const, golfcard_t const,
    golfcard_t const, golfcard_t const, golfcard_t const,
    golfcard_t const, golfcard_t const, golfcard_t const);

// Extracts a row from a hand into a single segment.
//
// The segment can be accessed like the top row of a board.
golfsegment_t
golf_hand_row(golfhand_t const hand, uint8_t const row);

// Extracts a column from a hand into a single row.
//
// The segment can be accessed like the top row of a board.
golfsegment_t
golf_hand_col(golfhand_t const hand, uint8_t const col);

golfcard_t
golf_hand_card(golfhand_t const hand, golfcard_t const where);


// Counts number of cards in hand. Should be 1, 3 or 9.
uint8_t
golf_hand_elements(golfhand_t hand);

// Number of uncleared cards in a row, column or hand.
uint8_t
golf_hand_uncleared_elements(golfhand_t hand);

uint8_t
golf_hand_visible_elements(golfhand_t hand);

golfhand_t
golf_hand_finish(golfhand_t const hand);


// Assumes three elements in top row.
bool
golf_segment_clears(golfsegment_t const segment);


golfhand_t
golf_clear_hand(golfhand_t hand);


golfhand_t
golf_flip_card(golfhand_t hand, golfhand_t where);

golfhand_t
golf_place_card(golfhand_t hand, golfhand_t where, golfcard_t which);

char
golf_symbol(golfhand_t card, bool xray);


// Needs a char[18] at least.
void
golf_format_hand(char * s, golfhand_t hand, bool xray);

struct GolfGame
GolfGame_new(size_t const players);

void
GolfRound_shuffle_deck(struct GolfRound * const round);

golfcard_t
GolfRound_draw(struct GolfRound * round, bool const visible);

// Use srandom before calling to seed the shuffle.
void
GolfRound_init(
    struct GolfRound * const round,
    struct GolfGame const * const game);

// Whether at least one player has all cards open.
bool
GolfRound_is_finishing(struct GolfRound const * const round);

// Whether all players have all cards open.
bool
GolfRound_is_finished(struct GolfRound const * const round);

void
GolfGame_finish_round(
    struct GolfGame * const game,
    struct GolfRound const * const round);

bool
GolfGame_is_finished(struct GolfGame const * const game);

void
GolfRound_next(struct GolfRound * const round);

typedef uint32_t golf_actions_t;
#define GOLF_ACTION_NONE         0
#define GOLF_ACTION_FLIP_TOP_LFT (1 << 0)
#define GOLF_ACTION_FLIP_TOP_MID (1 << 1)
#define GOLF_ACTION_FLIP_TOP_RGT (1 << 2)
#define GOLF_ACTION_FLIP_MID_LFT (1 << 3)
#define GOLF_ACTION_FLIP_MID_MID (1 << 4)
#define GOLF_ACTION_FLIP_MID_RGT (1 << 5)
#define GOLF_ACTION_FLIP_BOT_LFT (1 << 6)
#define GOLF_ACTION_FLIP_BOT_MID (1 << 7)
#define GOLF_ACTION_FLIP_BOT_RGT (1 << 8)
#define GOLF_ACTION_DECK_TOP_LFT (1 << 9)
#define GOLF_ACTION_DECK_TOP_MID (1 << 10)
#define GOLF_ACTION_DECK_TOP_RGT (1 << 11)
#define GOLF_ACTION_DECK_MID_LFT (1 << 12)
#define GOLF_ACTION_DECK_MID_MID (1 << 13)
#define GOLF_ACTION_DECK_MID_RGT (1 << 14)
#define GOLF_ACTION_DECK_BOT_LFT (1 << 15)
#define GOLF_ACTION_DECK_BOT_MID (1 << 16)
#define GOLF_ACTION_DECK_BOT_RGT (1 << 17)
#define GOLF_ACTION_FLIP_DECK    (1 << 18)

golf_actions_t
GolfRound_action(
    struct GolfRound * const round,
    golf_actions_t actions);

void
GolfRound_flip_deck(struct GolfRound * const round);

void
GolfRound_flip_face_up(
    struct GolfRound * const round,
    golfhand_t where);

void
GolfRound_take_card(
    struct GolfRound * const round,
    golfhand_t where);

#endif  // GOLF_H
