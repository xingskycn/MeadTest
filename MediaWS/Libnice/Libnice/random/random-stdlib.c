/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2006-2008 Collabora Ltd.
 *  Contact: Dafydd Harries
 * (C) 2006-2008 Nokia Corporation. All rights reserved.
 *  Contact: Kai Vehmanen
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Nice GLib ICE library.
 *
 * The Initial Developers of the Original Code are Collabora Ltd and Nokia
 * Corporation. All Rights Reserved.
 *
 * Contributors:
 *   Dafydd Harries, Collabora Ltd.
 *   Youness Alaoui, Collabora Ltd.
 *   Kai Vehmanen, Nokia
 *
 * Alternatively, the contents of this file may be used under the terms of the
 * the GNU Lesser General Public License Version 2.1 (the "LGPL"), in which
 * case the provisions of LGPL are applicable instead of those above. If you
 * wish to allow use of your version of this file only under the terms of the
 * LGPL and not to allow others to use your version of this file under the
 * MPL, indicate your decision by deleting the provisions above and replace
 * them with the notice and other provisions required by the LGPL. If you do
 * not delete the provisions above, a recipient may use your version of this
 * file under either the MPL or the LGPL.
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "random-stdlib.h"

static void
rng_seed (
  G_GNUC_UNUSED
  NiceRNG *rng, guint32 seed)
{
	// Just to do not complain about unused:
	(void)rng;
	srand(seed);
}
// Just one byte
static gchar randomByte(void)
{
	unsigned long long r = rand();
	gchar c = 256 * r / RAND_MAX;
	return c;
}
static void
rng_generate_bytes (
  G_GNUC_UNUSED
  NiceRNG *rng,
  guint len,
  gchar *buf)
{
  guint i;

  (void)rng;

  for (i = 0; i < len; i++)
    buf[i] = randomByte();
}

static guint
rng_generate_int (
  G_GNUC_UNUSED
  NiceRNG *rng,
  guint low,
  guint high)
{
	(void)rng;
	size_t guintSize = sizeof(guint);
	guint full;
	UInt8* fp = (UInt8*) &full;
	for (int i = 0; i<guintSize;i++)
	{
		fp[i] = randomByte();
	}
	
	guint len = high - low;
	if (len == 0) return low;
	return low + full % len;
}

static void
rng_free (NiceRNG *rng)
{
  free(rng);
}

NiceRNG *
nice_rng_glib_new (void)
{
  NiceRNG *ret;

	ret = malloc(sizeof(NiceRNG));
  ret->seed = rng_seed;
  ret->generate_bytes = rng_generate_bytes;
  ret->generate_int = rng_generate_int;
  ret->free = rng_free;
  return ret;
}

NiceRNG *
nice_rng_glib_new_predictable (void)
{
  NiceRNG *rng;

  rng = nice_rng_glib_new ();
  rng->seed (rng, 0);
  return rng;
}

