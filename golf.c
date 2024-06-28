#include <stdlib.h>
#include "golf.h"

static uint8_t
shift_of_mask(golfhand_t const mask)
{
	switch (mask) {
		case MASK_TOP_LFT: return SHIFT_TOP_LFT; break;
		case MASK_TOP_MID: return SHIFT_TOP_MID; break;
    case MASK_TOP_RGT: return SHIFT_TOP_RGT; break;
    case MASK_MID_LFT: return SHIFT_MID_LFT; break;
    case MASK_MID_MID: return SHIFT_MID_MID; break;
    case MASK_MID_RGT: return SHIFT_MID_RGT; break;
    case MASK_BOT_LFT: return SHIFT_BOT_LFT; break;
    case MASK_BOT_MID: return SHIFT_BOT_MID; break;
    case MASK_BOT_RGT: return SHIFT_BOT_RGT; break;
	}
	return -1;
}

golfcard_t
golf_card(enum GolfFace const face, bool const visible)
{
	return (visible ? (golfcard_t) GOLF_CARD_VISIBLE : 0) | face;
}

int32_t
golf_card_points(golfcard_t const card)
{
	switch (card & GOLF_FACE_MASK) {
		case GOLF_FACE_NONE:  return 0; break;
		case GOLF_FACE_ACE:   return 1; break;
		case GOLF_FACE_TWO:   return 2; break;
		case GOLF_FACE_THREE: return 3; break;
		case GOLF_FACE_FOUR:  return 4; break;
		case GOLF_FACE_FIVE:  return -3; break;
		case GOLF_FACE_SIX:   return 6; break;
		case GOLF_FACE_SEVEN: return 7; break;
		case GOLF_FACE_EIGHT: return 8; break;
		case GOLF_FACE_NINE:  return 9; break;
		case GOLF_FACE_TEN:
		case GOLF_FACE_JACK:
		case GOLF_FACE_QUEEN:
		case GOLF_FACE_KING:  return 10; break;
		default: return -1; break;
	}
}

int32_t
golf_hand_points(golfhand_t hand)
{
	int32_t total = 0;
	while (hand) {
		total += golf_card_points(hand & GOLF_HAND_CARD_MASK);
		hand >>= SHIFTWIDTH;
	}
	return total;
}

golfhand_t
golf_hand_finish(golfhand_t hand)
{
	for (size_t i = 0; i < 9; ++i) {
		hand |= GOLF_CARD_VISIBLE << (SHIFTWIDTH * i);
	}
	return hand;
}

golfhand_t
golf_make_hand(
    golfcard_t const top_left,
		golfcard_t const top_mid,
		golfcard_t const top_right,
    golfcard_t const mid_left,
		golfcard_t const mid_mid,
		golfcard_t const mid_right,
    golfcard_t const bot_left,
		golfcard_t const bot_mid,
		golfcard_t const bot_right)
{
  return (
      (top_left  << SHIFT_TOP_LFT) |
      (top_mid   << SHIFT_TOP_MID) |
      (top_right << SHIFT_TOP_RGT) |
      (mid_left  << SHIFT_MID_LFT) |
      (mid_mid   << SHIFT_MID_MID) |
      (mid_right << SHIFT_MID_RGT) |
      (bot_left  << SHIFT_BOT_LFT) |
      (bot_mid   << SHIFT_BOT_MID) |
      (bot_right << SHIFT_BOT_RGT));
}

uint8_t
golf_hand_visible_elements(golfhand_t hand)
{
	uint8_t visible = 0;
	while (hand) {
		visible += !!(hand & GOLF_CARD_VISIBLE);
		hand >>= SHIFTWIDTH;
	}
	return visible;
}

golfsegment_t
golf_hand_row(golfhand_t const hand, uint8_t const row)
{
  switch (row) {
    case 0:
      return
				(hand & MASK_TOP_LFT)
				| (hand & MASK_TOP_MID)
				| (hand & MASK_TOP_RGT);
      break;
    case 1:
      return 
        (hand & MASK_MID_LFT) >> (SHIFT_MID_LFT - SHIFT_TOP_LFT)
        | (hand & MASK_MID_MID) >> (SHIFT_MID_MID - SHIFT_TOP_MID)
        | (hand & MASK_MID_RGT) >> (SHIFT_MID_RGT - SHIFT_TOP_RGT);
      break;
    case 2:
      return 
        (hand & MASK_BOT_LFT) >> (SHIFT_BOT_LFT - SHIFT_TOP_LFT)
        | (hand & MASK_BOT_MID) >> (SHIFT_BOT_MID - SHIFT_TOP_MID)
        | (hand & MASK_BOT_RGT) >> (SHIFT_BOT_RGT - SHIFT_TOP_RGT);
      break;
  }
  return -1;
}

golfsegment_t
golf_hand_col(golfhand_t const hand, uint8_t const col)
{
  switch (col) {
    case 0:
      return 
        (hand & MASK_TOP_LFT) >> (SHIFT_TOP_LFT - SHIFT_TOP_LFT)
        | (hand & MASK_MID_LFT) >> (SHIFT_MID_LFT - SHIFT_TOP_MID)
        | (hand & MASK_BOT_LFT) >> (SHIFT_BOT_LFT - SHIFT_TOP_RGT);
      break;
    case 1:
      return 
        (hand & MASK_TOP_MID) >> (SHIFT_TOP_MID - SHIFT_TOP_LFT)
        | (hand & MASK_MID_MID) >> (SHIFT_MID_MID - SHIFT_TOP_MID)
        | (hand & MASK_BOT_MID) >> (SHIFT_BOT_MID - SHIFT_TOP_RGT);
      break;
    case 2:
      return 
        (hand & MASK_TOP_RGT) >> (SHIFT_TOP_RGT - SHIFT_TOP_LFT)
        | (hand & MASK_MID_RGT) >> (SHIFT_MID_RGT - SHIFT_TOP_MID)
        | (hand & MASK_BOT_RGT) >> (SHIFT_BOT_RGT - SHIFT_TOP_RGT);
      break;
  }
  return -1;
}

uint8_t
golf_hand_elements(golfhand_t hand)
{
  uint8_t n = 0;
  while (hand) {
    n += !!(hand & GOLF_HAND_CARD_MASK);
    hand >>= SHIFTWIDTH;
  }
  return n;
}

uint8_t
golf_hand_uncleared_elements(golfhand_t hand)
{
  uint8_t n = 0;
  while (hand) {
    n += !!(hand & GOLF_FACE_MASK);
    hand >>= SHIFTWIDTH;
  }
  return n;
}

bool
golf_segment_clears(golfsegment_t const segment)
{
  // Single columns or rows don't clear along the short axis.
  if (golf_hand_uncleared_elements(segment) == 1) { return false; }

  golfcard_t left = (segment >> SHIFT_TOP_LFT) & GOLF_HAND_CARD_MASK;
  golfcard_t mid = (segment >> SHIFT_TOP_MID) & GOLF_HAND_CARD_MASK;
  golfcard_t right = (segment >> SHIFT_TOP_RGT) & GOLF_HAND_CARD_MASK;

  // We cannot clear if cards are face down. Removed cards are always "face up"
  if (
      !!(left & GOLF_CARD_VISIBLE) +
      !!(mid & GOLF_CARD_VISIBLE) +
      !!(right & GOLF_CARD_VISIBLE) < 3) {
    return false;
  }

  golfcard_t needed = left;
  if (needed == GOLF_REMOVED_CARD) { needed = mid; }
  if (needed == GOLF_REMOVED_CARD) { needed = right; }
  if (needed == GOLF_REMOVED_CARD) { return true; }

  // Cards need to be cleared or the needed one.
  return (
      (left == needed || left == GOLF_REMOVED_CARD)
      && (mid == needed || mid == GOLF_REMOVED_CARD)
      && (right == needed || right == GOLF_REMOVED_CARD)
  );
}



golfhand_t
_golf_clear_hand(golfhand_t hand)
{
  if (golf_segment_clears(golf_hand_row(hand, 0))) {
    hand = golf_place_card(hand, MASK_TOP_LFT, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_TOP_MID, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_TOP_RGT, GOLF_REMOVED_CARD);
  }
  if (golf_segment_clears(golf_hand_row(hand, 1))) {
    hand = golf_place_card(hand, MASK_MID_LFT, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_MID_MID, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_MID_RGT, GOLF_REMOVED_CARD);
  }
  if (golf_segment_clears(golf_hand_row(hand, 2))) {
    hand = golf_place_card(hand, MASK_BOT_LFT, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_BOT_MID, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_BOT_RGT, GOLF_REMOVED_CARD);
  }
  if (golf_segment_clears(golf_hand_col(hand, 0))) {
    hand = golf_place_card(hand, MASK_TOP_LFT, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_MID_LFT, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_BOT_LFT, GOLF_REMOVED_CARD);
  }
  if (golf_segment_clears(golf_hand_col(hand, 1))) {
    hand = golf_place_card(hand, MASK_TOP_MID, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_MID_MID, GOLF_REMOVED_CARD);
    hand = golf_place_card(hand, MASK_BOT_MID, GOLF_REMOVED_CARD);
  }
  if (golf_segment_clears(golf_hand_col(hand, 2))) {
		hand = golf_place_card(hand, MASK_TOP_RGT, GOLF_REMOVED_CARD);
		hand = golf_place_card(hand, MASK_MID_RGT, GOLF_REMOVED_CARD);
		hand = golf_place_card(hand, MASK_BOT_RGT, GOLF_REMOVED_CARD);
  }
  return hand;
}

golfhand_t
golf_clear_hand(golfhand_t hand)
{
	golfhand_t out = _golf_clear_hand(hand);
	while (out != hand) {
		hand = out;
		out = _golf_clear_hand(hand);
	}
	return out;
};

golfhand_t
golf_flip_card(golfhand_t const hand, golfhand_t const where)
{
  return hand | (GOLF_CARD_VISIBLE << shift_of_mask(where));
}

// Foo
// @param where The mask where to put the card
// @param which The card to place.
golfhand_t
golf_place_card(golfhand_t hand, golfhand_t where, golfcard_t which)
{
	hand &= ~where;
	return hand | (which << shift_of_mask(where));
}

char
golf_symbol(golfhand_t card, bool xray)
{
  if (!xray && !(card & GOLF_CARD_VISIBLE)) { return '?'; }
  switch (card & GOLF_FACE_MASK) {
    case GOLF_FACE_NONE:   return '_'; break;
    case GOLF_FACE_ACE:   return 'A'; break;
    case GOLF_FACE_TWO:   return '2'; break;
    case GOLF_FACE_THREE: return '3'; break;
    case GOLF_FACE_FOUR:  return '4'; break;
    case GOLF_FACE_FIVE:  return '5'; break;
    case GOLF_FACE_SIX:   return '6'; break;
    case GOLF_FACE_SEVEN: return '7'; break;
    case GOLF_FACE_EIGHT: return '8'; break;
    case GOLF_FACE_NINE:  return '9'; break;
    case GOLF_FACE_TEN:   return 'T'; break;
    case GOLF_FACE_JACK:  return 'J'; break;
    case GOLF_FACE_QUEEN: return 'Q'; break;
    case GOLF_FACE_KING:  return 'K'; break;
    default:              return 'X'; break;
  }
}

// Needs a char[18] at least.
void
golf_format_hand(char * s, golfhand_t hand, bool xray)
{
  sprintf(
      s,
      "%c %c %c\n%c %c %c\n%c %c %c",
      golf_symbol(hand >> SHIFT_TOP_LFT, xray),
      golf_symbol(hand >> SHIFT_TOP_MID, xray),
      golf_symbol(hand >> SHIFT_TOP_RGT, xray),
      golf_symbol(hand >> SHIFT_MID_LFT, xray),
      golf_symbol(hand >> SHIFT_MID_MID, xray),
      golf_symbol(hand >> SHIFT_MID_RGT, xray),
      golf_symbol(hand >> SHIFT_BOT_LFT, xray),
      golf_symbol(hand >> SHIFT_BOT_MID, xray),
      golf_symbol(hand >> SHIFT_BOT_RGT, xray));
}

struct GolfGame
GolfGame_new(size_t const players)
{
	struct GolfGame out = { players , {0,0,0,0,0,0,0,0,0,0}, 0 };
	out.dealer = 0;
	return out;
}

void
GolfRound_shuffle_deck(struct GolfRound * const round)
{
	while (round->discardsz) {
		round->deck[(round->decksz)++] = round->discard[--(round->discardsz)];
	}
	// Fisher Yates Shuffle??
	// Need to confirm, I think there were two loops.
	for (size_t i = 0; i < round->decksz; ++i) {
		long idx = random() % (i + 1);
		golfcard_t temp;
		temp = round->deck[i];
		round->deck[i] = round->deck[idx];
		round->deck[idx] = temp;
	}
}

golfcard_t
GolfRound_draw(struct GolfRound * const round, bool const visible)
{
	if (round->decksz == 0) { GolfRound_shuffle_deck(round); }
	golfcard_t card = round->deck[--(round->decksz)];
	return card | (visible ? GOLF_CARD_VISIBLE : 0);
}

void
GolfRound_init(
		struct GolfRound * const round,
		struct GolfGame const * const game)
{
	round->playersz = game->playersz;
	round->decksz = 0;
	round->whoseturn = (game->dealer + 1) % game->playersz;

	// Set up deck (via discard pile).
	for (enum GolfFace face = GOLF_FACE_ACE; face <= GOLF_FACE_KING ; face++) {
		for (size_t i = 0; i < 8; ++i) {
			round->discard[(round->discardsz)++] = golf_card(face, false);
		}
	}
	GolfRound_shuffle_deck(round);

	round->open = GolfRound_draw(round, true);

	for (size_t i = 0; i < round->playersz; ++i) {
		round->players[i] = golf_make_hand(
				GolfRound_draw(round, true),
				GolfRound_draw(round, true),
				GolfRound_draw(round, true),
				GolfRound_draw(round, false),
				GolfRound_draw(round, false),
				GolfRound_draw(round, false),
				GolfRound_draw(round, false),
				GolfRound_draw(round, false),
				GolfRound_draw(round, false));
	}
}

bool
GolfRound_is_finishing(struct GolfRound const * const round)
{
	for (size_t i = 0; i < round->playersz; ++i) {
		if (golf_hand_visible_elements(round->players[i]) == 9) {
			return true;
		}
	}
	return false;
}

bool
GolfRound_is_finished(struct GolfRound const * const round)
{
	for (size_t i = 0; i < round->playersz; ++i) {
		if (golf_hand_visible_elements(round->players[i]) != 9) {
			return false;
		}
	}
	return true;
}

void GolfGame_finish_round(
    struct GolfGame * const game,
    struct GolfRound const * const round)
{
	for (size_t i = 0; i < game->playersz; ++i) {
		game->scores[i] += golf_hand_points(round->players[i]);
	}
	game->dealer = (game->dealer + 1) % game->playersz;
}

bool
GolfGame_is_finished(struct GolfGame const * const game)
{
	for (size_t i = 0; i < game->playersz; ++i) {
		if (game->scores[i] >= 100) {return true;}
	}
	return false;
}

void
GolfRound_next(struct GolfRound * const round)
{
	round->whoseturn++;
	round->whoseturn %= round->playersz;
}

golf_actions_t
GolfRound_action(
    struct GolfRound * const round,
    golf_actions_t actions)
{
	golfhand_t hand = round->players[round->whoseturn];
	switch (actions) {
		case GOLF_ACTION_FLIP_DECK:
			GolfRound_flip_deck(round);
			return actions & ~GOLF_ACTION_FLIP_DECK;
			break;
		case GOLF_ACTION_NONE:
			return 
				GOLF_ACTION_FLIP_DECK
				| GOLF_ACTION_DECK_TOP_LFT | GOLF_ACTION_DECK_TOP_MID | GOLF_ACTION_DECK_TOP_RGT
				| GOLF_ACTION_DECK_MID_LFT | GOLF_ACTION_DECK_MID_MID | GOLF_ACTION_DECK_MID_RGT
				| GOLF_ACTION_DECK_BOT_LFT | GOLF_ACTION_DECK_BOT_MID | GOLF_ACTION_DECK_BOT_RGT
				| (golf_hand_card(hand, MASK_TOP_LFT) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_TOP_LFT : 0)
				| (golf_hand_card(hand, MASK_TOP_MID) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_TOP_MID : 0)
				| (golf_hand_card(hand, MASK_TOP_RGT) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_TOP_RGT : 0)
				| (golf_hand_card(hand, MASK_MID_LFT) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_MID_LFT : 0)
				| (golf_hand_card(hand, MASK_MID_MID) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_MID_MID : 0)
				| (golf_hand_card(hand, MASK_MID_RGT) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_MID_RGT : 0)
				| (golf_hand_card(hand, MASK_BOT_LFT) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_BOT_LFT : 0)
				| (golf_hand_card(hand, MASK_BOT_MID) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_BOT_MID : 0)
				| (golf_hand_card(hand, MASK_BOT_RGT) & GOLF_CARD_VISIBLE ? GOLF_ACTION_FLIP_BOT_RGT : 0)
				;
			break;
		case GOLF_ACTION_FLIP_TOP_LFT: GolfRound_flip_face_up(round, MASK_TOP_LFT); break;
		case GOLF_ACTION_FLIP_TOP_MID: GolfRound_flip_face_up(round, MASK_TOP_MID); break;
		case GOLF_ACTION_FLIP_TOP_RGT: GolfRound_flip_face_up(round, MASK_TOP_RGT); break;
		case GOLF_ACTION_FLIP_MID_LFT: GolfRound_flip_face_up(round, MASK_MID_LFT); break;
		case GOLF_ACTION_FLIP_MID_MID: GolfRound_flip_face_up(round, MASK_MID_MID); break;
		case GOLF_ACTION_FLIP_MID_RGT: GolfRound_flip_face_up(round, MASK_MID_RGT); break;
		case GOLF_ACTION_FLIP_BOT_LFT: GolfRound_flip_face_up(round, MASK_BOT_LFT); break;
		case GOLF_ACTION_FLIP_BOT_MID: GolfRound_flip_face_up(round, MASK_BOT_MID); break;
		case GOLF_ACTION_FLIP_BOT_RGT: GolfRound_flip_face_up(round, MASK_BOT_RGT); break;
		case GOLF_ACTION_DECK_TOP_LFT: GolfRound_flip_face_up(round, MASK_TOP_LFT); break;
		case GOLF_ACTION_DECK_TOP_MID: GolfRound_take_card(round, MASK_TOP_MID); break;
		case GOLF_ACTION_DECK_TOP_RGT: GolfRound_take_card(round, MASK_TOP_RGT); break;
		case GOLF_ACTION_DECK_MID_LFT: GolfRound_take_card(round, MASK_MID_LFT); break;
		case GOLF_ACTION_DECK_MID_MID: GolfRound_take_card(round, MASK_MID_MID); break;
		case GOLF_ACTION_DECK_MID_RGT: GolfRound_take_card(round, MASK_MID_RGT); break;
		case GOLF_ACTION_DECK_BOT_LFT: GolfRound_take_card(round, MASK_BOT_LFT); break;
		case GOLF_ACTION_DECK_BOT_MID: GolfRound_take_card(round, MASK_BOT_MID); break;
		case GOLF_ACTION_DECK_BOT_RGT: GolfRound_take_card(round, MASK_BOT_RGT); break;
	}
	return GOLF_ACTION_NONE;
}

void
GolfRound_flip_deck(struct GolfRound * const round)
{
	round->discard[(round->discardsz)++] = round->open;
	round->open = GolfRound_draw(round, true);
}

void
GolfRound_flip_face_up(
    struct GolfRound * const round,
    golfhand_t where)
{
	round->players[round->whoseturn] = golf_flip_card(round->players[round->whoseturn], where);
}

void
GolfRound_take_card(
    struct GolfRound * const round,
    golfhand_t where)
{
	GolfRound_flip_face_up(round, where);  // Just in case.
	golfcard_t temp = round->open;
	round->open = golf_hand_card(round->players[round->whoseturn], where);
	round->players[round->whoseturn] = golf_place_card(round->players[round->whoseturn], where, temp);
}

golfcard_t
golf_hand_card(golfhand_t const hand, golfcard_t const where)
{
	return (hand & where) >> shift_of_mask(where);
}
