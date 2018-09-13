#include "CppUTest/TestHarness.h"

#include "tree_rotate.h"

enum {G = 1, P, X, A, B, C, D};

TEST_GROUP(TreeRotate)
{
	void setup() {
		tree_init();
	}

	void add_lchild(int u, int v) {
		PARENT(v) = u;
		LCHILD(u) = v;
	}

	void add_rchild(int u, int v) {
		PARENT(v) = u;
		RCHILD(u) = v;
	}
};

TEST(TreeRotate, ZigStep)
{
	add_lchild(P, X);		/*     P   */
	add_rchild(P, C);		/*    / \  */
					/*  (X)  C */
	add_lchild(X, A);		/*  /      */
	add_rchild(X, B);		/* A       */

	rotate_right(P);

	LONGS_EQUAL(A, LCHILD(X));	/*  (X)    */
	LONGS_EQUAL(P, RCHILD(X));	/*  / \    */
					/* A   P   */
	LONGS_EQUAL(B, LCHILD(P));	/*    / \  */
	LONGS_EQUAL(C, RCHILD(P));	/*   B   C */
}

TEST(TreeRotate, ZigZigStep)
{
	add_lchild(G, P);		/*           */
	add_rchild(G, D);		/*       G   */
					/*      / \  */
	add_lchild(P, X);		/*     P   D */
	add_rchild(P, C);		/*    / \    */
					/*  (X)  C   */
	add_lchild(X, A);		/*  / \      */
	add_rchild(X, B);		/* A   B     */

	rotate_right(G);
	rotate_right(P);

	LONGS_EQUAL(A, LCHILD(X));	/*           */
	LONGS_EQUAL(P, RCHILD(X));	/*  (X)      */
					/*  / \      */
	LONGS_EQUAL(B, LCHILD(P));	/* A   P     */
	LONGS_EQUAL(G, RCHILD(P));	/*    / \    */
					/*   B   G   */
	LONGS_EQUAL(C, LCHILD(G));	/*      / \  */
	LONGS_EQUAL(D, RCHILD(G));	/*     C   D */
}

TEST(TreeRotate, ZagZagStep)
{
	add_lchild(G, D);		/*           */
	add_rchild(G, P);		/*   G       */
					/*  / \      */
	add_lchild(P, C);		/* D   P     */
	add_rchild(P, X);		/*    / \    */
					/*   C  (X)  */
	add_lchild(X, B);		/*      / \  */
	add_rchild(X, A);		/*     B   A */

	rotate_left(G);
	rotate_left(P);

	LONGS_EQUAL(P, LCHILD(X));	/*           */
	LONGS_EQUAL(A, RCHILD(X));	/*      (X)  */
					/*      / \  */
	LONGS_EQUAL(G, LCHILD(P));	/*     P   A */
	LONGS_EQUAL(B, RCHILD(P));	/*    / \    */
					/*   G   B   */
	LONGS_EQUAL(D, LCHILD(G));	/*  / \      */
	LONGS_EQUAL(C, RCHILD(G));	/* D   C     */
}

TEST(TreeRotate, ZigZagStep)
{
	add_lchild(G, P);		/*         */
	add_rchild(G, D);		/*     G   */
					/*    / \  */
	add_lchild(P, A);		/*   P   D */
	add_rchild(P, X);		/*  / \    */
					/* A  (X)  */
	add_lchild(X, B);		/*    / \  */
	add_rchild(X, C);		/*   B   C */

	rotate_left(P);
	rotate_right(G);

	LONGS_EQUAL(P, LCHILD(X));	/*             */
	LONGS_EQUAL(G, RCHILD(X));	/*     (X)     */
					/*    /   \    */
	LONGS_EQUAL(A, LCHILD(P));	/*   P     G   */
	LONGS_EQUAL(B, RCHILD(P));	/*  / \   / \  */
					/* A   B C   D */
	LONGS_EQUAL(C, LCHILD(G));	/*             */
	LONGS_EQUAL(D, RCHILD(G));	/*             */
}

TEST(TreeRotate, ZagZigStep)
{
	add_lchild(G, D);		/*         */
	add_rchild(G, P);		/*   G     */
					/*  / \    */
	add_lchild(P, X);		/* D   P   */
	add_rchild(P, A);		/*    / \  */
					/*  (X)  A */
	add_lchild(X, C);		/*  / \    */
	add_rchild(X, B);		/* C   B   */

	rotate_right(P);
	rotate_left(G);

	LONGS_EQUAL(G, LCHILD(X));	/*             */
	LONGS_EQUAL(P, RCHILD(X));	/*     (X)     */
					/*    /   \    */
	LONGS_EQUAL(B, LCHILD(P));	/*   G     P   */
	LONGS_EQUAL(A, RCHILD(P));	/*  / \   / \  */
					/* D   C B   A */
	LONGS_EQUAL(D, LCHILD(G));	/*             */
	LONGS_EQUAL(C, RCHILD(G));	/*             */
}
