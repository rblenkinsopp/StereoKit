#pragma once

#include "../../stereokit.h"

namespace sk {

enum hand_finger_ {
	hand_finger_thumb = 0,
	hand_finger_index,
	hand_finger_middle,
	hand_finger_ring,
	hand_finger_pinky,
};
enum hand_joint_ {
	hand_joint_root = 0,
	hand_joint_knuckle_major,
	hand_joint_knuckle_mid,
	hand_joint_knuckle_minor,
	hand_joint_tip,
};
enum hand_system_ {
	hand_system_none = 0,
	hand_system_mouse,
	hand_system_oxr_controllers,
	hand_system_oxr_articulated,
	hand_system_mirage,
	hand_system_leap,
	hand_system_override
};

extern int32_t input_hand_pointer_id[handed_max];

hand_system_ input_hand_get_system();
void input_hand_refresh_system();

void input_hand_init    ();
void input_hand_shutdown();
void input_hand_update  ();
void input_hand_update_predicted();

void input_hand_update_meshes();
hand_joint_t *input_hand_get_pose_buffer(handed_ hand);
void input_hand_sim(handed_ handedness, const vec3 &hand_pos, const quat &orientation, bool tracked, bool trigger_pressed, bool grip_pressed);
void input_hand_state_update(handed_ handedness);
void input_hand_make_solid();

} // namespace sk