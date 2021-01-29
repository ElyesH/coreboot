ifneq ($(NORTHBRIDGE_INTEL_I945),)
romstage-y += lpddr4.c ddr4.c ddr3.c ddr2.c ddr_common.c
ramstage-y += lpddr4.c ddr4.c ddr3.c ddr2.c ddr_common.c spd.c
else
romstage-y += ddr2.c ddr_common.c
ramstage-y += ddr2.c ddr_common.c spd.c
endif
