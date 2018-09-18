/* Configuration_AffineTransform.cpp
 *
 * Copyright (C) 1993-2018 David Weenink
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 djmw 20020315 GPL header
 */

#include "Configuration_AffineTransform.h"
#include "Configuration_and_Procrustes.h"
#include "Procrustes.h"
#include "SVD.h"

#undef your
#define your ((AffineTransform_Table) thy methods) ->

static void do_steps45 (constMAT w, MAT t, constMAT c, double *f) {
	// Step 4 || 10: If W'T has negative diagonal elements, multiply corresponding columns in T by -1.
	for (integer i = 1; i <= w.ncol; i ++) {
		double d = 0.0;
		for (integer k = 1; k <= w.ncol; k++) {
			d += w [k] [i] * t [k] [i];
		}
		if (d < 0.0) {
			for (integer k = 1; k <= w.ncol; k++) {
				t [k] [i] = -t [k] [i];
			}
		}
	}

	// Step 5 & 11: f = tr W'T (Diag (T'CT))^-1/2

	*f = 0.0;
	for (integer i = 1; i <= w.ncol; i ++) {
		longdouble d = 0.0, tct = 0.0;
		for (integer k = 1; k <= w.ncol; k ++) {
			d += w [k] [i] * t [k] [i];
			for (integer j = 1; j <= w.ncol; j ++) {
				tct += t [k] [i] * c [k] [j] * t [j] [i];
			}
		}
		if (tct > 0.0) {
			*f += d / sqrt (tct);
		}
	}
}

/*
	Using: Kiers & Groenen (1996), A monotonically convergent congruence algorithm for orthogona congruence rotation,
	Psychometrika (61), 375-389.
*/
void NUMmaximizeCongruence_inplace (MAT t, constMAT b, constMAT a, integer maximumNumberOfIterations, double tolerance) {
	Melder_assert (t.ncol == b.ncol && b.nrow == a.nrow && b.ncol == a.ncol);
	integer numberOfIterations = 0;

	if (b.ncol == 1) {
		t [1] [1] = 1.0;
		return;
	}
	integer nr = b.nrow, nc = b.ncol;
	autoMAT u = MATzero (nc, nc);
	autoVEC evec = VECzero (nc);
	autoSVD svd = SVD_create (nc, nc);

	// Steps 1 & 2: C = A'A and W = A'B

	autoMAT c = MATmul_tn (a, a);
	autoMAT w = MATmul_tn (a, b);
	double checkc = NUMsum (c.get());
	double checkw = NUMsum (w.get());
	
	Melder_require (checkc != 0.0 && checkw != 0.0, U"NUMmaximizeCongruence: the matrix should not be zero.");

	// Scale W by (diag(B'B))^-1/2

	for (integer j = 1; j <= nc; j ++) {
		longdouble scale = 0.0;
		for (integer k = 1; k <= nr; k ++) {
			scale += b [k] [j] * b [k] [j];
		}
		if (scale > 0.0) {
			scale = 1.0 / sqrt (scale);
		}
		for (integer i = 1; i <= nc; i++) {
			w [i] [j] *= scale;
		}
	}

	// Step 3: largest eigenvalue of C

	evec [1] = 1.0;
	double rho, f, f_old;
	NUMdominantEigenvector (c.get(), evec.get(), & rho, 1.0e-6);

	do_steps45 (w.get(), t, c.get(), & f);
	do {
		for (integer j = 1; j <= nc; j ++) {
			// Step 7.a

			longdouble p = 0.0;
			for (integer k = 1; k <= nc; k ++) {
				for (integer i = 1; i <= nc; i ++) {
					p += t [k] [j] * c [k] [i] * t [i] [j];
				}
			}

			// Step 7.b

			longdouble q = 0.0;
			for (integer k = 1; k <= nc; k ++) {
				q += w [k] [j] * t [k] [j];
			}

			// Step 7.c

			if (q == 0.0) {
				for (integer i = 1; i <= nc; i ++) {
					u [i] [j] = 0.0;
				}
			} else {
				longdouble ww = 0.0;
				for (integer k = 1; k <= nc; k ++) {
					ww += w [k] [j] * w [k] [j];
				}
				for (integer i = 1; i <= nc; i ++) {
					longdouble ct = 0.0;
					for (integer k = 1; k <= nc; k ++) {
						ct += c [i] [k] * t [k] [j];
					}
					u [i] [j] = (q * (ct - rho * t [i] [j]) / p - 2.0 * ww * t [i] [j] / q - w [i] [j]) / sqrt (p);
				}
			}
		}

		// Step 8

		SVD_svd_d (svd.get(), u.get());

		// Step 9

		for (integer i = 1; i <= nc; i ++) {
			for (integer j = 1; j <= nc; j ++) {
				t [i] [j] = 0.0;
				for (integer k = 1; k <= nc; k ++) {
					t [i] [j] -= svd -> u [i] [k] * svd -> v [j] [k];
				}
			}
		}

		numberOfIterations++;
		f_old = f;

		// Steps 10 & 11 equal steps 4 & 5

		do_steps45 (w.get(), t, c.get(), & f);

	} while (fabs (f_old - f) / f_old > tolerance && numberOfIterations < maximumNumberOfIterations);
}

autoAffineTransform Configurations_to_AffineTransform_congruence (Configuration me, Configuration thee, integer maximumNumberOfIterations, double tolerance) {
	try {
		// Use Procrustes transform to obtain starting configuration.
		// (We only need the transformation matrix T.)
		autoProcrustes p = Configurations_to_Procrustes (me, thee, false);
		Melder_assert (p -> dimension == my data.ncol);
		Melder_assert (p -> dimension == thy data.ncol);
		NUMmaximizeCongruence_inplace (p -> r.get(), my data.get(), thy data.get(), maximumNumberOfIterations, tolerance);
		autoAffineTransform at = AffineTransform_create (p -> dimension);
		matrixcopy_preallocated (at -> r.get(), p -> r.get());
		return at;
	} catch (MelderError) {
		Melder_throw (me, U": no congruence transformation created.");
	}
}

autoConfiguration Configuration_AffineTransform_to_Configuration (Configuration me, AffineTransform thee) {
	try {
		Melder_require (my numberOfColumns == thy dimension, U"The number of columns in the Configuration should equal the dimension of the transform.");
		
		autoConfiguration him = Data_copy (me);

		// Apply transformation YT

		thy v_transform (my data.at, my numberOfRows, his data.at);
		return him;
	} catch (MelderError) {
		Melder_throw (U"Configuration not created.");
	}
}

/* End of file Configuration_AffineTransform.cpp */
