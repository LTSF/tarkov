#pragma once

#include <cstdint>
#include <vector>

#include "../structures/structures.hpp"

#include "../offsets/offsets.hpp"

#include "../../dependencies/mem/mem.hpp"

namespace engine::types
{
	enum class name_hash_t : std::uint64_t
	{
		fps_camera = 0x656D614320535046, // "FPS Came"
		game_world = 0x6C726f57656D6147,  // "GameWorl"
		optical_camera = 0x6974704f65736142 // "BaseOpti"
	};

	struct object_t
	{
		template< class type_t = std::uintptr_t >
		 type_t* as() const
		{
			return (type_t*)( this );
		}
	};

	struct class_object_t : public object_t
	{
		template< class type_t = std::uintptr_t >
		type_t get_object() const
		{
			return *reinterpret_cast<type_t*>(reinterpret_cast<std::uintptr_t>(this) + engine::offsets::class_object::object_reference);
		}
	};

	struct game_object_t : public object_t
	{
		bool is(name_hash_t hash) const
		{
			if (!this)
				return false;

			const auto nm = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(this) + engine::offsets::game_object::name);

			if (!nm)
				return false;

			return hash == *reinterpret_cast< name_hash_t *>(nm);
		}

		const char* get_name()
		{
			return *reinterpret_cast<const char**>(reinterpret_cast<std::uintptr_t>(this) + engine::offsets::game_object::name);
		}

		class_object_t* class_object() const;
	};

	struct base_object_t : public object_t
	{
		game_object_t* game_object() const;

		base_object_t* next_base_object() const;
	};

	struct transform_t : public object_t
	{
		 engine::structures::vector3_t get_position() const;
	};

	struct health_controller_t : public object_t
	{
		 
	};

	struct bone_array_t
	{
		transform_t transform;
	};

	struct profile_infromation_t : public object_t
	{

	};

	struct profile_t : public object_t
	{
		 profile_infromation_t get_profile_information() const;
	};

	class inertia_settings_t
	{
	public:
		char pad_0000[16]; //0x0000
		engine::structures::vector2_t exit_movement_state_speed_threshold; //0x0010
		engine::structures::vector2_t WalkInertia; //0x0018
		float FallThreshold; //0x0020
		engine::structures::vector2_t SpeedLimitAfterFallMin; //0x0024
		engine::structures::vector2_t SpeedLimitAfterFallMax; //0x002C
		engine::structures::vector2_t SpeedLimitDurationMin; //0x0034
		engine::structures::vector2_t SpeedLimitDurationMax; //0x003C
		engine::structures::vector2_t SpeedInertiaAfterJump; //0x0044
		float BaseJumpPenaltyDuration; //0x004C
		float DurationPower; //0x0050
		float BaseJumpPenalty; //0x0054
		float PenaltyPower; //0x0058
		engine::structures::vector2_t InertiaTiltCurveMin; //0x005C
		engine::structures::vector2_t InertiaTiltCurveMax; //0x0064
		engine::structures::vector2_t InertiaBackCoef; //0x006C
		engine::structures::vector2_t TiltInertiaMaxSpeed; //0x0074
		engine::structures::vector2_t TiltStartSideBackSpeed; //0x007C
		engine::structures::vector2_t TiltMaxSideBackSpeed; //0x0084
		engine::structures::vector2_t TiltAcceleration; //0x008C
		int32_t AverageRotationFrameSpan; //0x0094
		engine::structures::vector2_t SprintSpeedInertiaCurveMin; //0x0098
		engine::structures::vector2_t SprintSpeedInertiaCurveMax; //0x00A0
		engine::structures::vector2_t SprintBrakeInertia; //0x00A8
		engine::structures::vector2_t SprintTransitionMotionPreservation; //0x00B0
		engine::structures::vector3_t InertiaLimits; //0x00B8
		engine::structures::vector2_t WeaponFlipSpeed; //0x00C4
		float InertiaLimitsStep; //0x00CC
		engine::structures::vector2_t SprintAccelerationLimits; //0x00D0
		engine::structures::vector2_t PreSprintAccelerationLimits; //0x00D8
		engine::structures::vector2_t SideTime; //0x00E0
		engine::structures::vector2_t DiagonalTime; //0x00E8
		float MinDirectionBlendTime; //0x00F0
		float MoveTime; //0x00F4
		engine::structures::vector2_t MinMovementAccelerationRangeRight; //0x00F8
		engine::structures::vector2_t MaxMovementAccelerationRangeRight; //0x0100
	}; //Size: 0x0108

	class movement_context_t
	{
	public:
		char pad_0000[16]; //0x0000
		class player_t* player; //0x0010
		char pad_0018[480]; //0x0018
		float free_fall_time; //0x01F8
		char pad_01FC[4]; //0x01FC
		float N000004CD; //0x0200
		float pose_level; //0x0204
		char pad_0208[432]; //0x0208
		float N00000507; //0x03B8
		bool N00000512; //0x03BC
		bool grounded; //0x03BD
		char pad_03BE[74]; //0x03BE
	}; //Size: 0x0408

	class jump_t
	{
	public:
		char pad_0000[16]; //0x0000
		class movement_context_t* movement_context; //0x0010
		char pad_0018[56]; //0x0018
		void* destination; //0x0050
		char pad_0058[40]; //0x0058
	}; //Size: 0x0080

	class skinned_mesh_renderer_t
	{
	public:
		char pad_0000[8]; //0x0000
		class N0000076A** N000006B7; //0x0008
		class renderer* renderer; //0x0010
		char pad_0018[352]; //0x0018
	}; //Size: 0x0178

	class breath_effector_t
	{
	public:
		char pad_0000[160]; //0x0000
		int32_t is_aiming; //0x00A0
		float intensity; //0x00A4
		char pad_00A8[96]; //0x00A8
	}; //Size: 0x0108

	class motion_effector_t
	{
	public:
		char pad_0000[208]; //0x0000
		float intensity; //0x00D0
		char pad_00D4[116]; //0x00D4
	}; //Size: 0x0148

	class shot_effector_t
	{
	public:
		char pad_0000[64]; //0x0000
		engine::structures::vector2_t recoil_strength_xy; //0x0040
		engine::structures::vector2_t recoil_strength_z; //0x0048
		char pad_0050[184]; //0x0050
	}; //Size: 0x0108

	class procedural_weapon_animation_t
	{
	public:
		char pad_0000[40]; //0x0000
		class breath_effector_t* breath_effector; //0x0028
		char pad_0030[8]; //0x0030
		class motion_effector_t* motion_effector; //0x0038
		char pad_0040[8]; //0x0040
		class shot_effector_t* shot_effector; //0x0048
	}; //Size: 0x0050

	class physical_t
	{
	public:
		char pad_0000[56]; //0x0000
		class stamina_t* stamina; //0x0038
		char pad_0040[8]; //0x0040
		class oxygen_t* oxygen; //0x0048
		char pad_0050[100]; //0x0050
		float fall_damage_multiplier; //0x00B4
		float stamina_capacity; //0x00B8
		float stamina_restore_rate; //0x00BC
		char pad_00C0[204]; //0x00C0
	}; //Size: 0x018C

	class stamina_t
	{
	public:
		char pad_0000[72]; //0x0000
		float current; //0x0048
		char pad_004C[4]; //0x004C
	}; //Size: 0x0050

	class oxygen_t
	{
	public:
		char pad_0000[16]; //0x0000
		class total_capacity_t* total_capacity; //0x0010
		char pad_0018[48]; //0x0018
		float current; //0x0048
		char pad_004C[4]; //0x004C
	}; //Size: 0x0050

	class player_t
	{
	public:
		char pad_0000[40]; //0x0000
		class character_controller_t* character_controller; //0x0028
		char pad_0030[16]; //0x0030
		class movement_context_t* movement_context; //0x0040
		char pad_0048[96]; //0x0048
		class player_body_t* player_body; //0x00A8
		char pad_00B0[232]; //0x00B0
		class procedural_weapon_animation_t* procedural_weapon_animation; //0x0198
		char pad_01A0[816]; //0x01A0
		class physical_t* physical; //0x04D0
		char pad_04D8[8]; //0x04D8
		void* N000000F2; //0x04E0
		char pad_04E8[80]; //0x04E8
	}; //Size: 0x0538

	struct local_game_world_t : public object_t
	{
		 mem::vector< player_t* > get_players() const; /* Does not include local player */

		 player_t* get_local_player() const;
	};

	struct camera_t : public object_t
	{
		union view_matrix_t
		{
			struct
			{
				const float        _11, _12, _13, _14;
				const float        _21, _22, _23, _24;
				const float        _31, _32, _33, _34;
				const float        _41, _42, _43, _44;

			};

			const float m[4][4];
		};

		 view_matrix_t get_view_matrix() const;
	};

	struct game_object_manager_t : public object_t
	{
		 object_t* last_tagged_object() const;
		 object_t* first_tagged_object() const;

		 object_t* last_camera_tagged_object() const;
		 object_t* first_camera_tagged_object() const;

		 object_t* last_active_object() const;
		 object_t* first_active_object() const;

		 mem::vector< base_object_t* > get_tagged_objects() const;
		 mem::vector< base_object_t* > get_camera_objects() const;
		 mem::vector< base_object_t* > get_active_objects() const;

		 base_object_t* get_tagged_object_by_name(const name_hash_t name) const;
		 base_object_t* get_camera_object_by_name(const name_hash_t name) const;
		 base_object_t* get_active_object_by_name(const name_hash_t name) const;
	};
}