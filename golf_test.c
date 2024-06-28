#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minunit.h"
#include "golf.h"

char *test_constructors(void)
{
  mu_test("Visible", golf_card(GOLF_FACE_SIX, true) == 0b10110, MU_NO_CLEANUP);
  mu_test("Invisible", golf_card(GOLF_FACE_SIX, false) == 0b00110, MU_NO_CLEANUP);
  return NULL;
}

// This test validates that I can use the formatter for outputvalidation.
char *
test_make_and_format_hand(void)
{
  char actual[18];
  golfhand_t hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, false),
      golf_card(GOLF_FACE_TWO, false),
      golf_card(GOLF_FACE_THREE, false),
      golf_card(GOLF_FACE_FOUR, false),
      golf_card(GOLF_FACE_FIVE, true),
      golf_card(GOLF_FACE_SIX, true),
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD
  );
  golf_format_hand((char *) &actual, hand, false);
  mu_test("Non X-ray", strcmp(actual, "? ? ?\n? 5 6\n_ _ _") == 0, MU_NO_CLEANUP);
  golf_format_hand((char *) &actual, hand, true);
  mu_test("X-ray", strcmp(actual, "A 2 3\n4 5 6\n_ _ _") == 0, MU_NO_CLEANUP);
  return NULL;
}

char *
test_hand_row(void)
{
  char actual[18];
  golfhand_t hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_TWO, true),
      golf_card(GOLF_FACE_THREE, true),
      golf_card(GOLF_FACE_FOUR, true),
      golf_card(GOLF_FACE_FIVE, true),
      golf_card(GOLF_FACE_SIX, true),
      golf_card(GOLF_FACE_SEVEN, true),
      golf_card(GOLF_FACE_EIGHT, true),
      golf_card(GOLF_FACE_NINE, true)
  );
  golf_format_hand((char *) actual, golf_hand_row(hand, 0), true);
  mu_test("Row 1", strcmp(actual, "A 2 3\n_ _ _\n_ _ _") == 0, MU_NO_CLEANUP);
  golf_format_hand((char *) actual, golf_hand_row(hand, 1), true);
  mu_test("Row 2", strcmp(actual, "4 5 6\n_ _ _\n_ _ _") == 0, MU_NO_CLEANUP);
  golf_format_hand((char *) actual, golf_hand_row(hand, 2), true);
  mu_test("Row 3", strcmp(actual, "7 8 9\n_ _ _\n_ _ _") == 0, MU_NO_CLEANUP);

  return NULL;
}

char *
test_hand_col(void)
{
  char actual[18];
  golfhand_t hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_TWO, true),
      golf_card(GOLF_FACE_THREE, true),
      golf_card(GOLF_FACE_FOUR, true),
      golf_card(GOLF_FACE_FIVE, true),
      golf_card(GOLF_FACE_SIX, true),
      golf_card(GOLF_FACE_SEVEN, true),
      golf_card(GOLF_FACE_EIGHT, true),
      golf_card(GOLF_FACE_NINE, true)
  );
  golf_format_hand((char *) actual, golf_hand_col(hand, 0), true);
  mu_test("Col 1", strcmp(actual, "A 4 7\n_ _ _\n_ _ _") == 0, MU_NO_CLEANUP);
  golf_format_hand((char *) actual, golf_hand_col(hand, 1), true);
  mu_test("Col 2", strcmp(actual, "2 5 8\n_ _ _\n_ _ _") == 0, MU_NO_CLEANUP);
  golf_format_hand((char *) actual, golf_hand_col(hand, 2), true);
  mu_test("Col 3", strcmp(actual, "3 6 9\n_ _ _\n_ _ _") == 0, MU_NO_CLEANUP);

  return NULL;
}

char *
test_hand_elements(void)
{
  return NULL;
  golfhand_t hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_TWO, true),
      golf_card(GOLF_FACE_THREE, true),
      golf_card(GOLF_FACE_FOUR, true),
      golf_card(GOLF_FACE_FIVE, true),
      golf_card(GOLF_FACE_SIX, true),
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD
  );
  mu_test("A hand has 9 elements", golf_hand_elements(hand) == 9, MU_NO_CLEANUP);
  mu_test(
      "A segment has 3 elements",
      golf_hand_elements(golf_hand_row(hand, 2)) == 3,
      MU_NO_CLEANUP);
  mu_test("A card is one element", golf_card(GOLF_FACE_SIX, true), MU_NO_CLEANUP);
  return NULL;
}

char *
test_hand_uncleared_elements(void)
{
  golfhand_t hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_TWO, true),
      golf_card(GOLF_FACE_THREE, true),
      golf_card(GOLF_FACE_FOUR, true),
      golf_card(GOLF_FACE_FIVE, true),
      golf_card(GOLF_FACE_SIX, true),
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD);
  mu_test("Hand has 6 elements", golf_hand_uncleared_elements(hand) == 6, MU_NO_CLEANUP);
  mu_test(
      "Row 0 has 3 elements",
      golf_hand_uncleared_elements(golf_hand_row(hand, 0)) == 3,
      MU_NO_CLEANUP);
  mu_test(
      "Row 2 has 0 elements",
      golf_hand_uncleared_elements(golf_hand_row(hand, 2)) == 0,
      MU_NO_CLEANUP);
  mu_test(
      "Col 0 has 2 elements",
      golf_hand_uncleared_elements(golf_hand_col(hand, 0)) == 2,
      MU_NO_CLEANUP);
  return NULL;
}

char *
test_hand_segment_clears(void)
{
  // A A A
  // A 6 _
  // _ _ _
  // This doesn't make sense in the game but works for testing segments.
  golfhand_t hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_SIX, true),
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD);
  mu_test(
      "Row 0 clears",
      golf_segment_clears(golf_hand_row(hand, 0)),
      MU_NO_CLEANUP);
  mu_test(
      "Row 1 doesn't clear",
      !golf_segment_clears(golf_hand_row(hand, 1)), 
      MU_NO_CLEANUP);
  mu_test(
      "Row 2 clears",
      golf_segment_clears(golf_hand_row(hand, 2)),
      MU_NO_CLEANUP);
  mu_test(
      "Col 0 clears",
      golf_segment_clears(golf_hand_col(hand, 0)),
      MU_NO_CLEANUP);
  mu_test(
      "Col 1 doesn't clear",
      !golf_segment_clears(golf_hand_col(hand, 1)), 
      MU_NO_CLEANUP);
  mu_test(
      "Col 2 doesn't clear",
      !golf_segment_clears(golf_hand_col(hand, 2)),
      MU_NO_CLEANUP);
  // The same hand but hidden
   hand = golf_make_hand(
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_ACE, true),
      golf_card(GOLF_FACE_ACE, false),
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD,
      GOLF_REMOVED_CARD);
  mu_test(
      "Row 0 doesn't clear",
      !golf_segment_clears(golf_hand_row(hand, 0)),
      MU_NO_CLEANUP);
  
  return NULL;
}

char *
test_place_card(void)
{
  golfhand_t hand = golf_make_hand(
    golf_card(GOLF_FACE_ACE, true),
    golf_card(GOLF_FACE_ACE, true),
    golf_card(GOLF_FACE_ACE, true),
    golf_card(GOLF_FACE_TWO, true),
    golf_card(GOLF_FACE_THREE, true),
    golf_card(GOLF_FACE_THREE, true),
    golf_card(GOLF_FACE_TWO, true),
    golf_card(GOLF_FACE_FOUR, true),
    golf_card(GOLF_FACE_FIVE, true)
  );
  hand = golf_place_card(hand, MASK_BOT_MID, golf_card(GOLF_FACE_JACK, true));
  char actual[18];
  golf_format_hand(actual, hand, true);
  mu_test(
      "Jack is placed", 
      strcmp(actual, "A A A\n2 3 3\n2 J 5") == 0,
      MU_NO_CLEANUP);
  return NULL;
}

char *
test_clear_hand(void)
{
  // A A A
  // 2 3 3
  // 2 4 5
  golfhand_t hand = golf_make_hand(
    golf_card(GOLF_FACE_ACE, true),
    golf_card(GOLF_FACE_ACE, true),
    golf_card(GOLF_FACE_ACE, true),
    golf_card(GOLF_FACE_TWO, true),
    golf_card(GOLF_FACE_THREE, true),
    golf_card(GOLF_FACE_THREE, true),
    golf_card(GOLF_FACE_TWO, true),
    golf_card(GOLF_FACE_FOUR, true),
    golf_card(GOLF_FACE_FIVE, true)
  );
  hand = golf_clear_hand(hand);
  char actual[18];
  golf_format_hand(actual, hand, true);
  mu_test(
      "All but 4 5 clear",
      strcmp(actual, "_ _ _\n_ _ _\n_ 4 5") == 0,
      MU_NO_CLEANUP);
  return NULL;
}

char *
test_flip_card(void)
{
  golfhand_t hand = golf_make_hand(
    golf_card(GOLF_FACE_ACE, false),
    golf_card(GOLF_FACE_ACE, false),
    golf_card(GOLF_FACE_ACE, false),
    golf_card(GOLF_FACE_TWO, false),
    golf_card(GOLF_FACE_THREE, false),
    golf_card(GOLF_FACE_THREE, false),
    golf_card(GOLF_FACE_TWO, false),
    golf_card(GOLF_FACE_FOUR, false),
    golf_card(GOLF_FACE_FIVE, false)
  );
  hand = golf_flip_card(hand, MASK_BOT_LFT);
  char actual[18];
  golf_format_hand(actual, hand, false);
  mu_test(
      "Only one card is flipped",
      strcmp(actual, "? ? ?\n? ? ?\n2 ? ?") == 0,
      MU_NO_CLEANUP);
  return NULL;
}


void all_tests(void)
{
  mu_run_test(test_constructors);
  mu_run_test(test_make_and_format_hand);
  mu_run_test(test_hand_row);
  mu_run_test(test_hand_col);
  mu_run_test(test_hand_elements);
  mu_run_test(test_hand_uncleared_elements);
  mu_run_test(test_hand_segment_clears);
  mu_run_test(test_clear_hand);
  mu_run_test(test_place_card);
  mu_run_test(test_flip_card);
}

int main(void)
{
  mu_main(all_tests);
}
