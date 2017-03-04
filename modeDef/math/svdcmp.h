/*********************************************************************
 * svd.h
 * Authored by Kris Hauser 2002-2003
 *
 * Interfaces to singular value decomposition routines.
 *
 * Copyright 2003, Regents of the University of California 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *********************************************************************/


//the following declares a function in Numerical Recipes in C
//DO NOT USE IT DIRECTLY!!!

//Given a matrix a[1..m][1..n], this routine computes its singular value decomposition, A =
//U.W.Vt. The matrix U[1..m][1..m] replaces a on output. The diagonal matrix of singular values W is output
//as a vector w[1..n]. The matrix V (not the transpose V T ) is output as v[1..n][1..n].

void svdcmp(double **a, int m, int n, double w[], double **v);


