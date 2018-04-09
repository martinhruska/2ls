extern int __VERIFIER_nondet_int();
extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <stdlib.h>

typedef struct node {
    int val;
    struct node *next;
} Node;

int main() {
    Node *p, *list = malloc(sizeof(*list));
    list->next = NULL;
    list->val = 10;
    while (__VERIFIER_nondet_int()) {
        int x;
        __CPROVER_assume(x >= 10 && x <= 20);
        p = malloc(sizeof(*p));
        list->next = p;
        p->next = NULL;
        p->val = x;
        list = p;
    }

    while (1) {
        for (p = list; p!= NULL; p = p->next) {
            if (!(p->val <= 20 && p->val >= 10))
                __VERIFIER_error();
            if (p->val < 20) p->val++;
            else p->val /= 2;
        }
    }

}
