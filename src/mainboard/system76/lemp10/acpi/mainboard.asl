/* SPDX-License-Identifier: GPL-2.0-only */

#define EC_GPE_SCI 0x6E /* GPP_E15 */
#define EC_GPE_SWI 0x6E /* GPP_E20 */
#include <ec/system76/ec/acpi/ec.asl>

Scope (\_SB) {
	#include "sleep.asl"
}
