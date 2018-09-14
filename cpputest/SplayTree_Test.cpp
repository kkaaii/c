#include "CppUTest/TestHarness.h"

#include "SplayTree.h"

enum {G = 1, P, X, A, B, C, D};

TEST_GROUP(SplayTree)
{
	void setup() {
		SplayTree_Init();
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

TEST(SplayTree, ZigStep)
{
	add_lchild(P, X);		/*     P   */
	add_rchild(P, C);		/*    / \  */
					/*  (X)  C */
	add_lchild(X, A);		/*  / \    */
	add_rchild(X, B);		/* A   B   */

	//SplayTree_RotateRight(P);
        SplayTree_Splay(X);

	LONGS_EQUAL(A, LCHILD(X));	/*  (X)    */
	LONGS_EQUAL(P, RCHILD(X));	/*  / \    */
					/* A   P   */
	LONGS_EQUAL(B, LCHILD(P));	/*    / \  */
	LONGS_EQUAL(C, RCHILD(P));	/*   B   C */
}

TEST(SplayTree, ZagStep)
{
	add_lchild(P, C);		/*   P     */
	add_rchild(P, X);		/*  / \    */
					/* C  (X)  */
	add_lchild(X, B);		/*    / \  */
	add_rchild(X, A);		/*   B   A */

	//SplayTree_RotateLeft(P);
        SplayTree_Splay(X);

	LONGS_EQUAL(P, LCHILD(X));	/*    (X)  */
	LONGS_EQUAL(A, RCHILD(X));	/*    / \  */
					/*   P   A */
	LONGS_EQUAL(C, LCHILD(P));	/*  / \    */
	LONGS_EQUAL(B, RCHILD(P));	/* C   B   */
}

TEST(SplayTree, ZigZigStep)
{
	add_lchild(G, P);		/*           */
	add_rchild(G, D);		/*       G   */
					/*      / \  */
	add_lchild(P, X);		/*     P   D */
	add_rchild(P, C);		/*    / \    */
					/*  (X)  C   */
	add_lchild(X, A);		/*  / \      */
	add_rchild(X, B);		/* A   B     */

	//SplayTree_RotateRight(G);
	//SplayTree_RotateRight(P);
        SplayTree_Splay(X);

	LONGS_EQUAL(A, LCHILD(X));	/*           */
	LONGS_EQUAL(P, RCHILD(X));	/*  (X)      */
					/*  / \      */
	LONGS_EQUAL(B, LCHILD(P));	/* A   P     */
	LONGS_EQUAL(G, RCHILD(P));	/*    / \    */
					/*   B   G   */
	LONGS_EQUAL(C, LCHILD(G));	/*      / \  */
	LONGS_EQUAL(D, RCHILD(G));	/*     C   D */
}

TEST(SplayTree, ZagZagStep)
{
	add_lchild(G, D);		/*           */
	add_rchild(G, P);		/*   G       */
					/*  / \      */
	add_lchild(P, C);		/* D   P     */
	add_rchild(P, X);		/*    / \    */
					/*   C  (X)  */
	add_lchild(X, B);		/*      / \  */
	add_rchild(X, A);		/*     B   A */

	//SplayTree_RotateLeft(G);
	//SplayTree_RotateLeft(P);
        SplayTree_Splay(X);

	LONGS_EQUAL(P, LCHILD(X));	/*           */
	LONGS_EQUAL(A, RCHILD(X));	/*      (X)  */
					/*      / \  */
	LONGS_EQUAL(G, LCHILD(P));	/*     P   A */
	LONGS_EQUAL(B, RCHILD(P));	/*    / \    */
					/*   G   B   */
	LONGS_EQUAL(D, LCHILD(G));	/*  / \      */
	LONGS_EQUAL(C, RCHILD(G));	/* D   C     */
}

TEST(SplayTree, ZigZagStep)
{
	add_lchild(G, P);		/*         */
	add_rchild(G, D);		/*     G   */
					/*    / \  */
	add_lchild(P, A);		/*   P   D */
	add_rchild(P, X);		/*  / \    */
					/* A  (X)  */
	add_lchild(X, B);		/*    / \  */
	add_rchild(X, C);		/*   B   C */

	//SplayTree_RotateLeft(P);
	//SplayTree_RotateRight(G);
        SplayTree_Splay(X);

	LONGS_EQUAL(P, LCHILD(X));	/*             */
	LONGS_EQUAL(G, RCHILD(X));	/*     (X)     */
					/*    /   \    */
	LONGS_EQUAL(A, LCHILD(P));	/*   P     G   */
	LONGS_EQUAL(B, RCHILD(P));	/*  / \   / \  */
					/* A   B C   D */
	LONGS_EQUAL(C, LCHILD(G));	/*             */
	LONGS_EQUAL(D, RCHILD(G));	/*             */
}

TEST(SplayTree, ZagZigStep)
{
	add_lchild(G, D);		/*         */
	add_rchild(G, P);		/*   G     */
					/*  / \    */
	add_lchild(P, X);		/* D   P   */
	add_rchild(P, A);		/*    / \  */
					/*  (X)  A */
	add_lchild(X, C);		/*  / \    */
	add_rchild(X, B);		/* C   B   */

	//SplayTree_RotateRight(P);
	//SplayTree_RotateLeft(G);
        SplayTree_Splay(X);

	LONGS_EQUAL(G, LCHILD(X));	/*             */
	LONGS_EQUAL(P, RCHILD(X));	/*     (X)     */
					/*    /   \    */
	LONGS_EQUAL(B, LCHILD(P));	/*   G     P   */
	LONGS_EQUAL(A, RCHILD(P));	/*  / \   / \  */
					/* D   C B   A */
	LONGS_EQUAL(D, LCHILD(G));	/*             */
	LONGS_EQUAL(C, RCHILD(G));	/*             */
}
