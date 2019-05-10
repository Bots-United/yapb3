//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Bot_tasks_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_TASKS_BOT_TASKS_DEFINITIONS_INCLUDED
#define BOT_TASKS_BOT_TASKS_DEFINITIONS_INCLUDED

#include <BotTasks/Task_DoubleJump.hpp>
#include <BotTasks/Schedule_PerformDoubleJump.hpp>

#include <BotTasks/Task_SelectTeam.hpp>
#include <BotTasks/Task_SelectModel.hpp>

#include <BotTasks/Task_AutoBuy.hpp>
#include <BotTasks/Task_AttackEnemy.hpp>
#include <BotTasks/Task_Camp.hpp>
#include <BotTasks/Task_Normal.hpp>
#include <BotTasks/Task_PlantBomb.hpp>
#include <BotTasks/Task_DefuseBomb.hpp>
#include <BotTasks/Task_RescueHostages.hpp>
#include <BotTasks/Task_SprayLogo.hpp>
#include <BotTasks/Task_FollowUser.hpp>
#include <BotTasks/Task_Idle.hpp>
#include <BotTasks/Task_MoveToPosition.hpp>

#endif	// ifndef BOT_TASKS_BOT_TASKS_DEFINITIONS_INCLUDED