/*
 * Copyright (c) Alexander Bluhm <bluhm@genua.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	uint64_t mem;
	uint32_t eax, ebx, ecx, edx;

	mem = 0x0123456789abcdefLL;
	eax = 0x11111111;
	ebx = 0x22222222;
	ecx = 0x33333333;
	edx = 0x44444444;

	printf("mem %.16llx, edx:eax %.8x%8x, ecx:ebx %.8x%.8x\n",
	    mem, edx, eax, ecx, ebx);
	printf("cmpxchg8b mem, mem != edx:eax\n");
	asm volatile (
		"lock	cmpxchg8b %0"
		: "+m" (mem), "+a" (eax), "+b" (ebx), "+c" (ecx), "+d" (edx));
	printf("mem %.16llx, edx:eax %.8x%8x, ecx:ebx %.8x%.8x\n",
	    mem, edx, eax, ecx, ebx);

	if (mem != 0x0123456789abcdefLL) {
		printf("expected mem 0x0123456789abcdef\n");
		return 1;
	}
	if (edx != 0x01234567 || eax != 0x89abcdef) {
		printf("expected edx:eax 0x0123456789abcdef\n");
		return 1;
	}
	if (ecx != 0x33333333 || ebx != 0x22222222) {
		printf("expected ecx:ebx 0x3333333322222222\n");
		return 1;
	}

	printf("cmpxchg8b mem, mem == edx:eax\n");
	asm volatile (
		"lock cmpxchg8b %0"
		: "+m" (mem), "+a" (eax), "+b" (ebx), "+c" (ecx), "+d" (edx));
	printf("mem %.16llx, edx:eax %.8x%8x, ecx:ebx %.8x%.8x\n",
	    mem, edx, eax, ecx, ebx);

	if (mem != 0x3333333322222222LL) {
		printf("expected mem 0x3333333322222222\n");
		return 1;
	}
	if (edx != 0x01234567 || eax != 0x89abcdef) {
		printf("expected edx:eax 0x0123456789abcdef\n");
		return 1;
	}
	if (ecx != 0x33333333 || ebx != 0x22222222) {
		printf("expected ecx:ebx 0x3333333322222222\n");
		return 1;
	}

	return 0;
}
