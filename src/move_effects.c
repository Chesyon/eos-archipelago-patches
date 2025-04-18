#include <pmdsky.h>
#include <cot.h>

#if CUSTOM_MOVE_EFFECTS

// Implements the "Body Press" move
// Based on https://github.com/Adex-8x/EoS-ASM-Effects/blob/main/moves/gen8/body_press.asm
// Deals damage based on the user's defense instead of attack stat
/*static bool MoveBodyPress(struct entity *user, struct entity *target, struct move *move)
{
  if (user->type == ENTITY_MONSTER)
  {
    struct monster *user_monster = (struct monster *)user->info;

    int old_attack = user_monster->offensive_stats[0];
    user_monster->offensive_stats[0] = user_monster->defensive_stats[0];

    bool dealt_damage = DealDamage(user, target, move, 0x100, ITEM_NOTHING);

    user_monster->offensive_stats[0] = old_attack;
    return dealt_damage;
  }
  return false;
}*/

// Called when using moves. Should return true if a custom effect was applied.
// This function is only called if the move doesn't fail due to a missing target
bool CustomApplyMoveEffect(
    move_effect_input *data, struct entity *user, struct entity *target, struct move *move)
{
  switch (data->move_id)
  {
  /*case MOVE_SCRATCH:
    // Replace move 260 (Scratch) with custom Body Press effect
    data->out_dealt_damage = MoveBodyPress(user, target, move);
    // Return true to signal that we've handled the effect
    return true;*/
  default:
    return false;
  }
}

#endif
