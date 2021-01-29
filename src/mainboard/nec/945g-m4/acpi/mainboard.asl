/* SPDX-License-Identifier: GPL-2.0-or-later */

Device (SLPB)
{
	Name(_HID, EisaId("PNP0C0E"))

	// Wake
	Name(_PRW, Package(){0x1d, 0x04})
}

Device (PWRB)
{
	Name(_HID, EisaId("PNP0C0C"))
	Name (_UID, 0xAA)
	Name (_STA, 0x0B)

	// Wake
	Name(_PRW, Package(){0x1d, 0x04})
}
