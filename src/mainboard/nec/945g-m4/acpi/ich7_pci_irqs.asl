/* SPDX-License-Identifier: GPL-2.0-or-later */

/* This is board specific information: IRQ routing for the
 * 0:1e.0 PCI bridge of the ICH7
 */

If (PICM) {
	Return (Package() {
		Package() { 0x0001ffff, 0, 0, 0x12},
		Package() { 0x0001ffff, 1, 0, 0x13},
		Package() { 0x0001ffff, 2, 0, 0x14},
		Package() { 0x0001ffff, 3, 0, 0x15},

		Package() { 0x0002ffff, 0, 0, 0x13},
		Package() { 0x0002ffff, 1, 0, 0x14},
		Package() { 0x0002ffff, 2, 0, 0x15},
		Package() { 0x0002ffff, 3, 0, 0x16},

		Package() { 0x0003ffff, 0, 0, 0x13},
		Package() { 0x0003ffff, 1, 0, 0x12},
		Package() { 0x0003ffff, 2, 0, 0x15},
		Package() { 0x0003ffff, 3, 0, 0x16},

		Package() { 0x0004ffff, 0, 0, 0x12},
		Package() { 0x0004ffff, 1, 0, 0x17},
		Package() { 0x0004ffff, 2, 0, 0x10},
		Package() { 0x0004ffff, 3, 0, 0x14},

		Package() { 0x0005ffff, 0, 0, 0x11},
		Package() { 0x0005ffff, 1, 0, 0x14},
		Package() { 0x0005ffff, 2, 0, 0x15},
		Package() { 0x0005ffff, 3, 0, 0x16},

		Package() { 0x0008ffff, 0, 0, 0x14},
	})
} Else {
	Return (Package() {
		Package() { 0x0001ffff, 0, \_SB.PCI0.LPCB.LNKC, 0},
		Package() { 0x0001ffff, 1, \_SB.PCI0.LPCB.LNKD, 0},
		Package() { 0x0001ffff, 2, \_SB.PCI0.LPCB.LNKE, 0},
		Package() { 0x0001ffff, 3, \_SB.PCI0.LPCB.LNKF, 0},

		Package() { 0x0002ffff, 0, \_SB.PCI0.LPCB.LNKD, 0},
		Package() { 0x0002ffff, 1, \_SB.PCI0.LPCB.LNKE, 0},
		Package() { 0x0002ffff, 2, \_SB.PCI0.LPCB.LNKF, 0},
		Package() { 0x0002ffff, 3, \_SB.PCI0.LPCB.LNKG, 0},

		Package() { 0x0003ffff, 0, \_SB.PCI0.LPCB.LNKD, 0},
		Package() { 0x0003ffff, 1, \_SB.PCI0.LPCB.LNKC, 0},
		Package() { 0x0003ffff, 2, \_SB.PCI0.LPCB.LNKF, 0},
		Package() { 0x0003ffff, 3, \_SB.PCI0.LPCB.LNKG, 0},

		Package() { 0x0004ffff, 0, \_SB.PCI0.LPCB.LNKC, 0},
		Package() { 0x0004ffff, 1, \_SB.PCI0.LPCB.LNKH, 0},
		Package() { 0x0004ffff, 2, \_SB.PCI0.LPCB.LNKA, 0},
		Package() { 0x0004ffff, 3, \_SB.PCI0.LPCB.LNKE, 0},

		Package() { 0x0005ffff, 0, \_SB.PCI0.LPCB.LNKB, 0},
		Package() { 0x0005ffff, 1, \_SB.PCI0.LPCB.LNKE, 0},
		Package() { 0x0005ffff, 2, \_SB.PCI0.LPCB.LNKF, 0},
		Package() { 0x0005ffff, 3, \_SB.PCI0.LPCB.LNKG, 0},

		Package() { 0x0008ffff, 0, \_SB.PCI0.LPCB.LNKE, 0},
	})
}
