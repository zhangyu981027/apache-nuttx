/****************************************************************************
 * arch/tricore/src/common/tricore_getintstack.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>

#include <nuttx/sched.h>

#include "tricore_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_UP
#  define g_idle_topstack __ustack
#  define g_intstackalloc ((uintptr_t)__istack - CONFIG_ARCH_INTERRUPTSTACK)
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifndef CONFIG_UP
extern uintptr_t __ustack0[];
extern uintptr_t __ustack1[];
extern uintptr_t __ustack2[];
extern uintptr_t __ustack3[];
extern uintptr_t __ustack4[];
extern uintptr_t __ustack5[];
extern uintptr_t __istack0[];
extern uintptr_t __istack1[];
extern uintptr_t __istack2[];
extern uintptr_t __istack3[];
extern uintptr_t __istack4[];
extern uintptr_t __istack5[];

static uintptr_t g_idle_topstack[CONFIG_NCPUS] =
{
  [0] = (uintptr_t)__ustack0,
#if CONFIG_NCPUS > 1
  [1] = (uintptr_t)__ustack1,
#if CONFIG_NCPUS > 2
  [2] = (uintptr_t)__ustack2,
#if CONFIG_NCPUS > 3
  [3] = (uintptr_t)__ustack3,
#if CONFIG_NCPUS > 4
  [4] = (uintptr_t)__ustack4,
#if CONFIG_NCPUS > 5
  [5] = (uintptr_t)__ustack5,
#endif
#endif
#endif
#endif
#endif
};

#if CONFIG_ARCH_INTERRUPTSTACK > 3
static uintptr_t g_int_topstack[CONFIG_NCPUS] =
{
  [0] = (uintptr_t)__istack0,
#if CONFIG_NCPUS > 1
  [1] = (uintptr_t)__istack1,
#if CONFIG_NCPUS > 2
  [2] = (uintptr_t)__istack2,
#if CONFIG_NCPUS > 3
  [3] = (uintptr_t)__istack3,
#if CONFIG_NCPUS > 4
  [4] = (uintptr_t)__istack4,
#if CONFIG_NCPUS > 5
  [5] = (uintptr_t)__istack5,
#endif
#endif
#endif
#endif
#endif
};
#endif
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_get_intstackbase
 ****************************************************************************/

#if CONFIG_ARCH_INTERRUPTSTACK > 3
uintptr_t up_get_intstackbase(int cpu)
{
#ifndef CONFIG_UP
  return g_int_topstack[cpu] - CONFIG_ARCH_INTERRUPTSTACK;
#else
  return g_intstackalloc;
#endif
}
#endif

/****************************************************************************
 * Name: up_cpu_idlestack
 ****************************************************************************/

int up_cpu_idlestack(int cpu, struct tcb_s *tcb, size_t stack_size)
{
#ifndef CONFIG_UP
  uintptr_t topstack = g_idle_topstack[cpu];
#else
  uintptr_t topstack = (uintptr_t)g_idle_topstack;
#endif

  tcb->stack_alloc_ptr = (void *)(topstack - stack_size);
  tcb->stack_base_ptr  = tcb->stack_alloc_ptr;
  tcb->adj_stack_size  = stack_size;
  return 0;
}
