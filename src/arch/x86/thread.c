/* SPDX-License-Identifier: GPL-2.0-only */

#include <thread.h>

#if ENV_X86_64
#error COOP_MULTITASKING does not currently support x86_64
#endif

/* The stack frame looks like the following after a pushad instruction. */
struct pushad_regs {
	uint32_t edi; /* Offset 0x00 */
	uint32_t esi; /* Offset 0x04 */
	uint32_t ebp; /* Offset 0x08 */
	uint32_t esp; /* Offset 0x0c */
	uint32_t ebx; /* Offset 0x10 */
	uint32_t edx; /* Offset 0x14 */
	uint32_t ecx; /* Offset 0x18 */
	uint32_t eax; /* Offset 0x1c */
};

static inline uintptr_t push_stack(uintptr_t cur_stack, uintptr_t value)
{
	uintptr_t *addr;

	cur_stack -= sizeof(value);
	addr = (uintptr_t *)cur_stack;
	*addr = value;
	return cur_stack;
}

void arch_prepare_thread(struct thread *t,
			asmlinkage void (*thread_entry)(void *), void *arg)
{
	uintptr_t stack = t->stack_current;

	/* Imitate thread_entry(t) with return address of 0. thread_entry()
	 * is assumed to never return. */
	stack = push_stack(stack, (uintptr_t)arg);
	stack = push_stack(stack, (uintptr_t)0);
	stack = push_stack(stack, (uintptr_t)thread_entry);
	/* Make room for the registers. Ignore initial values. */
	stack -= sizeof(struct pushad_regs);

	t->stack_current = stack;
}
