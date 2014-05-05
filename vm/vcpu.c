#include <err.h>

#include <stdlib.h>

#include "vm.h"

struct vcpu {
	struct mem *mem;

	word_t regs[16];
};

#define regs_cl regs[12]
#define regs_fr regs[13]
#define regs_sp regs[14]
#define regs_ip regs[15]

struct vcpu *
vcpu_new(struct mem *mem)
{
	struct vcpu *vcpu;

	vcpu = calloc(1, sizeof(struct vcpu));
	if (vcpu == NULL)
		return NULL;
	vcpu->mem = mem;
	return vcpu;
}

void
vcpu_free(struct vcpu *vcpu)
{
	vcpu->mem = NULL;
	free(vcpu);
}

void
vcpu_run(struct vcpu *vcpu, vaddr_t entry)
{
	word_t insn;

	vcpu->regs_ip = entry;

	while (1) {
		insn = mem_fetch(vcpu->mem, vcpu->regs_ip);
		vcpu->regs_ip += WORDSIZE;

		switch (insn) {
		case 0x00000004: /* RET */
			if (vcpu->regs_sp == 0) {
				warnx("RET from vcpu_run");
				return;
			}
			break;
		default:
			warnx("illegal instruction word: %0*x",
			      WORDSIZE * 2, insn);
			return;
		}
	}
}
