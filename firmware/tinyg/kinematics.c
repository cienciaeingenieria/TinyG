/*
 * kinematics.c - inverse kinematics routines
 * Part of TinyG project
 *
 * Copyright (c) 2010 - 2013 Alden S. Hart Jr.
 *
 * TinyG is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, 
 * or (at your option) any later version.
 *
 * TinyG is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with TinyG  If not, see <http://www.gnu.org/licenses/>.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "tinyg.h"
#include "config.h"
#include "canonical_machine.h"
#include "kinematics.h"

//static void _inverse_kinematics(float travel[], float joint[], float microseconds);

/*
 * ik_kinematics() - wrapper routine for inverse kinematics
 *
 *	Calls kinematics function(s). 
 *	Performs axis mapping & conversion of length units to steps (and deals with inhibited axes)
 *
 *	The reason steps are returned as floats (as opposed to, say, uint32_t) is to accommodate 
 *	fractional DDA steps. The DDA deals with fractional step values as fixed-point binary in 
 *	order to get the smoothest possible operation. Steps are passed to the move prep routine 
 *	as floats and converted to fixed-point binary during queue loading. See stepper.c for details.
 */

void ik_kinematics(float travel[], float steps[], float microseconds)
{
//	uint8_t i;
	float joint[AXES];

//	_inverse_kinematics(travel, joint, microseconds);// you can insert inverse kinematics transformations here
	memcpy(joint, travel, sizeof(float)*AXES);		 //...or just do a memcopy for cartesian machines

	// Map motors to axes and convert length units to steps
	// Most of the conversion math has already been done in during config in steps_per_unit()
	// which takes axis travel, step angle and microsteps into account.
	for (uint8_t axis=0; axis<AXES; axis++) {
		if (cfg.a[axis].axis_mode == AXIS_INHIBITED) { joint[axis] = 0;}
		if (cfg.m[MOTOR_1].motor_map == axis) { steps[MOTOR_1] = joint[axis] * cfg.m[MOTOR_1].steps_per_unit;}
		if (cfg.m[MOTOR_2].motor_map == axis) { steps[MOTOR_2] = joint[axis] * cfg.m[MOTOR_2].steps_per_unit;}
		if (cfg.m[MOTOR_3].motor_map == axis) { steps[MOTOR_3] = joint[axis] * cfg.m[MOTOR_3].steps_per_unit;}
		if (cfg.m[MOTOR_4].motor_map == axis) { steps[MOTOR_4] = joint[axis] * cfg.m[MOTOR_4].steps_per_unit;}
//		if (cfg.m[MOTOR_5].motor_map == axis) { steps[MOTOR_5] = joint[axis] * cfg.m[MOTOR_5].steps_per_unit;}
//		if (cfg.m[MOTOR_6].motor_map == axis) { steps[MOTOR_6] = joint[axis] * cfg.m[MOTOR_6].steps_per_unit;}

	// the above is a loop unrolled version of this:
	//	for (uint8_t motor=0; motor<MOTORS; motor++) {
	//		if (cfg.m[motor].motor_map == axis) { steps[motor] = joint[axis] * cfg.m[motor].steps_per_unit;}
	//	}
	}
}

/*
 * _inverse_kinematics() - inverse kinematics - example is for a cartesian machine
 *
 *	You can glue in inverse kinematics here, but be aware of time budget constrants.
 *	This function is run during the _exec() portion of the cycle and will therefore
 *	be run once per interpolation segment. The total time for the segment load, 
 *	including the inverse kinematics transformation cannot exceed the segment time, 
 *	and ideally should be no more than 25-50% of the segment time. Currently segments 
 *	run avery 5 ms, but this might be lowered. To profile this time look at the 
 *	time it takes to complete the mp_exec_move() function.
 */
/*
static void _inverse_kinematics(float travel[], float joint[], float microseconds)
{
	for (uint8_t i=0; i<AXES; i++) {
		joint[i] = travel[i];
	}	
}
*/

//############## UNIT TESTS ################

//#define __UNIT_TEST_KINEMATICS
#ifdef __UNIT_TESTS
#ifdef __UNIT_TEST_KINEMATICS

void _ik_test_inverse_kinematics(void);

void ik_unit_tests()
{
	_ik_test_inverse_kinematics();
}

void _ik_test_inverse_kinematics(void)
{
	return;
}

#endif
#endif
