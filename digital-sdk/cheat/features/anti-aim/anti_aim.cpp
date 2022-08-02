#include "anti_aim.h"
#include "../misc/log-system/logs.h"

void anti_aim::init()
{

	const bool should_run = !cant_work();

	if (!should_run)
		return;

	pitch();
	yaw();
	roll();
}

bool anti_aim::cant_work()
{	

	if (!g_cfg.m_anti_aim.m_enabled)
		return true;

	if (!ctx::packet_data::m_cmd)
		return true;

	if (!ctx::local()->is_alive())
		return true;

	if (ctx::local()->get_flags() & fl_frozen)
		return true;

	if (ctx::local()->get_move_type() == move_type_noclip
		|| ctx::local()->get_move_type() == move_type_ladder)
		return true;

	if (!ctx::local()->get_active_weapon())
		return true;

	if (ctx::packet_data::m_cmd->m_buttons & in_attack && ctx::local()->get_active_weapon()->get_idx() != weapon_revolver)
		return true;

	if ((ctx::packet_data::m_cmd->m_buttons & in_attack || ctx::packet_data::m_cmd->m_buttons & in_attack2) && ctx::local()->get_active_weapon()->is_knife())
		return true;

	if (ctx::packet_data::m_cmd->m_buttons & in_use)
		return true;

	if (ctx::local()->get_active_weapon()->get_throw_time())
		return true;

	return false;
}

void anti_aim::pitch()
{
	float modifier_value{};

	modifier_value = 85.f;

	ctx::packet_data::m_cmd->m_view_angles.x = modifier_value;
}

void anti_aim::yaw()
{
	ctx::packet_data::m_send_packet = ctx::packet_data::m_cmd->m_command % 2;

	ctx::packet_data::m_cmd->m_view_angles.y = math::normalize_yaw(ctx::packet_data::m_cmd->m_view_angles.y + 180.0f);

	const float desync_amount = 58.f;

	if (!ctx::packet_data::m_send_packet)
	{
		ctx::packet_data::m_cmd->m_view_angles.y += desync_amount;
	}
}

void anti_aim::roll()
{
	ctx::packet_data::m_cmd->m_view_angles.z = 0;

	const float desync_amount = 58.f;

	if (!ctx::packet_data::m_send_packet)
	{
		ctx::packet_data::m_cmd->m_view_angles.z += desync_amount;
	}
}