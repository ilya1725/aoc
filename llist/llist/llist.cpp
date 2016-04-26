
#include <stdio.h>

// a0 -> a1 -> a2 -> a3
// b0 -> b1
// a0 -> b0 -> a1 -> b1 -> a2 -> a3

typedef struct elem{
    int data;
    elem * next;
} elem;

struct elem * interleave( elem * head_a, elem * head_b) {
    if (NULL == head_a && NULL == head_b){
        return NULL;
    }
    if (NULL == head_a) {
        return head_b;
    }
    if (NULL == head_b) {
        return head_a;
    }

    elem * head_r = head_a;
    elem * head_a_curr = head_a;
    elem * head_b_curr = head_b;
    bool list_flag = true;

    while(1) {
        if (list_flag){
            if (NULL != head_b_curr) {
                elem * head_a_t = head_a_curr->next;
                head_a_curr->next = head_b_curr;
                head_a_curr = head_a_t;
                list_flag = false;
            } else {
                head_a_curr = head_a_curr->next;
            }
        } else {
            if (NULL != head_a_curr) {
                elem * head_b_t = head_b_curr->next;
                head_b_curr->next = head_a_curr;
                head_b_curr = head_b_t;
                list_flag = true;
            } else {
                head_b_curr = head_b_curr->next;
            }
        }

        if (NULL == head_a_curr && NULL == head_b_curr){
            break;
        }
        //printf("%d %d\n", (head_a_curr == NULL ? 0 : head_a_curr->data), (head_b_curr == NULL ? 0 : head_b_curr->data));
    }

    return head_r;
}

int main (int argc, char **argv) {

    elem a[1];
    elem b[13];

    elem *ha = &a[0];
    elem *hb = &b[0];

    for (auto i=0;i<(sizeof(a)/sizeof(a[0]));i++) {
        a[i].data = i;
        if (i == (sizeof(a)/sizeof(a[0]))-1) {
            a[i].next = NULL;
        } else {
            a[i].next = &a[i+1];
        }
    }
    for (auto i=0;i<(sizeof(b)/sizeof(b[0]));i++) {
        b[i].data = i | 0xF000;
        if (i == (sizeof(b)/sizeof(b[0]))-1) {
            b[i].next = NULL;
        } else {
            b[i].next = &b[i+1];
        }
    }

    auto r = interleave(ha, hb);

    while (r != NULL){
        printf("%d->", r->data);
        r = r->next;
    }
    printf("\n");

    return 0;
}