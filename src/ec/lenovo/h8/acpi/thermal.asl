/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/x86/acpi/statdef.asl>

Scope(\_TZ)
{
	Method(C2K, 1, NotSerialized)
	{
		Local0 = Arg0 * 10
		Local0 += 2732
		if (Local0 <= 2732) {
			Return (3000)
		}

		if (Local0 > 4012) {
			Return (3000)
		}
		Return (Local0)
	}

	ThermalZone(THM0)
	{
		/* Thermal zone polling frequency: 10 seconds */
		Name (_TZP, 100)

		/* Thermal sampling period for passive cooling: 10 seconds */
		Name (_TSP, 100)

		/* Coefficients for passive cooling */
		Name (_TC1, 0x02)
		Name (_TC2, 0x05)

/* Generated by acpigen */
External (\PPKG, MethodObj)

		/* Processors used for passive cooling */
		Method (_PSL, 0, Serialized)
		{
			Return (\PPKG ())
		}

		/* Get critical temperature in degree celsius */
		Method (GCRT, 0, NotSerialized) {
			Local0 = \TCRT
			if (Local0 > 0) {
				Return (Local0)
			}
			Return (127)
		}

		/* Get passive temperature in degree celsius */
		Method (GPSV, 0, NotSerialized) {
			Local0 = \TPSV
			if (Local0 > 0) {
				Return (Local0)
			}
			Return (95)
		}

		Method (_CRT, 0, NotSerialized) {
			Return (C2K (GCRT ()))
		}

		Method (_PSV, 0, NotSerialized) {
			Return (C2K (GPSV ()))
		}

		Method(_TMP) {
			Local0 = \_SB.PCI0.LPCB.EC.TMP0
			/* Avoid tripping alarm if invalid value reported */
			If (Local0 == 128) {
				Return (C2K(40))
			}
			Return (C2K(Local0))
		}

		Method (_AC0) {
			Local0 = GPSV ()

			/* Active fan 10 degree below passive threshold */
			Local0 -= 10

			If (\FLVL) {
				/* Turn of 5 degree below trip point */
				Local0 -= 5
			}

			Return (C2K (Local0))
		}

		Name (_AL0, Package () { FAN })

		PowerResource (FPWR, 0, 0)
		{
			/*
			 * WINDOWS BUG: Don't read from EmbeddedControl
			 * in PowerResources. Use system-memory instead!
			 */
			Method (_STA) {
				Return (\FLVL)
			}

			/*
			 * WINDOWS BUG: Don't write to FIELD elements located
			 * in another ACPI scope. Call a method that does it!
			 */
			Method (_ON) {
				\_SB.PCI0.LPCB.EC.FANE(1)
				\FLVL = 1
				Notify (\_TZ.THM0, NOTIFY_TZ_TRIPPTCHG)
			}

			Method (_OFF) {
				\_SB.PCI0.LPCB.EC.FANE(0)
				\FLVL = 0
				Notify (\_TZ.THM0, NOTIFY_TZ_TRIPPTCHG)
			}
		}

		Device (FAN)
		{
			Name (_HID, EISAID ("PNP0C0B"))
			Name (_PR0, Package () { FPWR })
		}
	}
#if CONFIG(H8_HAS_2ND_THERMAL_ZONE)
	ThermalZone(THM1)
	{
		/* Thermal zone polling frequency: 10 seconds */
		Name (_TZP, 100)

		/* Thermal sampling period for passive cooling: 10 seconds */
		Name (_TSP, 100)

		/* Coefficients for passive cooling */
		Name (_TC1, 0x02)
		Name (_TC2, 0x05)

		/* Processors used for passive cooling */
		Method (_PSL, 0, Serialized)
		{
			Return (\PPKG ())
		}

		Method (_CRT, 0, NotSerialized) {
			Return (C2K (99))
		}

		Method (_PSV, 0, NotSerialized) {
			Return (C2K (94))
		}

		Method(_TMP) {
			Local0 = \_SB.PCI0.LPCB.EC.TMP1
			/* Avoid tripping alarm if invalid value reported */
			If (Local0 == 128) {
				Return (C2K(40))
			}
			Return (C2K(Local0))
		}
	}
#endif
}
