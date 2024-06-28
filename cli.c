#include <string.h>
#include <stdio.h>
#include "golf.h"

void
print_board(struct GolfRound * round)
{
  char out[1000];
  size_t sz = 0;
  for (size_t row = 0; row < 3; row++) {
    for (size_t player = 0; player < round->playersz; ++player) {
      // To have uniform access.
      golfsegment_t segment = golf_hand_row(round->players[player], row);
      out[sz++] = golf_symbol(golf_hand_card(segment, MASK_TOP_LFT), false);
      out[sz++] = golf_symbol(golf_hand_card(segment, MASK_TOP_MID), false);
      out[sz++] = golf_symbol(golf_hand_card(segment, MASK_TOP_RGT), false);
      out[sz++] = ' ';
      out[sz++] = ' ';
      out[sz++] = ' ';
    }
    out[sz++] = '\n';
  }

  sz = stpcpy(out + sz, "Open: ") - out;
  out[sz++] = golf_symbol(round->open, true);
  out[sz++] = '\n';
  out[sz++] = '\0';
  printf("%s\n", out);
}

void
print_options(golf_actions_t const actions, size_t const player)
{
  printf("You are player %zu\nYou can:\n", player);
  golf_actions_t check = 1;
  while (check <= GOLF_ACTION_FLIP_DECK) {
    if (actions & check) {
      switch (check) {
        case GOLF_ACTION_FLIP_TOP_LFT: printf(" - 1 Flip top left\n"); break;
        case GOLF_ACTION_FLIP_TOP_MID: printf(" - 2 Flip top mid\n"); break;
        case GOLF_ACTION_FLIP_TOP_RGT: printf(" - 3 Flip top right\n"); break;
        case GOLF_ACTION_FLIP_MID_LFT: printf(" - 4 Flip mid left\n"); break;
        case GOLF_ACTION_FLIP_MID_MID: printf(" - 5 Flip mid mid\n"); break;
        case GOLF_ACTION_FLIP_MID_RGT: printf(" - 6 Flip mid right\n"); break;
        case GOLF_ACTION_FLIP_BOT_LFT: printf(" - 7 Flip bottom left\n"); break;
        case GOLF_ACTION_FLIP_BOT_MID: printf(" - 8 Flip bottom mid\n"); break;
        case GOLF_ACTION_FLIP_BOT_RGT: printf(" - 9 Flip bottom right\n"); break;
        case GOLF_ACTION_DECK_TOP_LFT: printf(" - 10 Swap open top left\n"); break;
        case GOLF_ACTION_DECK_TOP_MID: printf(" - 11 Swap open top mid\n"); break;
        case GOLF_ACTION_DECK_TOP_RGT: printf(" - 12 Swap open top right\n"); break;
        case GOLF_ACTION_DECK_MID_LFT: printf(" - 13 Swap open mid left\n"); break;
        case GOLF_ACTION_DECK_MID_MID: printf(" - 14 Swap open mid mid\n"); break;
        case GOLF_ACTION_DECK_MID_RGT: printf(" - 15 Swap open mid right\n"); break;
        case GOLF_ACTION_DECK_BOT_LFT: printf(" - 16 Swap open bottom left\n"); break;
        case GOLF_ACTION_DECK_BOT_MID: printf(" - 17 Swap open bottom mid\n"); break;
        case GOLF_ACTION_DECK_BOT_RGT: printf(" - 18 Swap open bottom right\n"); break;
        case GOLF_ACTION_FLIP_DECK: printf(" - 19 Flip the top card of the deck\n"); break;
      }
    }
    check <<= 1;
  }
}

int
main(void)
{
  struct GolfGame game = GolfGame_new(4);
  while (!GolfGame_is_finished(&game)) {
    struct GolfRound round;
    GolfRound_init(&round, &game);
    golf_actions_t available;
    while (!GolfRound_is_finished(&round)) {
      golf_actions_t choice = GOLF_ACTION_NONE;
      while (true) {
        available = GolfRound_action(&round, choice);
        if (!available) { break; }
        print_board(&round);
        print_options(available, round.whoseturn);
        printf("Action: ");
        scanf("%ud", &choice);
      }
      choice = GOLF_ACTION_NONE;
      

      round.players[round.whoseturn] = golf_clear_hand(round.players[round.whoseturn]);
    }
  }
  return 0;
}
