/* SPDX-License-Identifier: GPL-2.0-or-later */

Device (SLPB)
{
	Name(_HID, EisaId("PNP0C0E"))

	// Wake
	Name(_PRW, Package(){0x1d, 0x04})
}
