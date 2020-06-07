int read_six_numbers(char *input, int *arr) {
    int res = sscanf(input, "%d %d %d %d %d %d", &arr[0], &arr[1], &arr[2], &arr[3], &arr[4], &arr[5]);
    if (res < 6) {
        explode();
    }
    return res;
}

int func4(int a /*edi*/, int b /*esi*/, int c /*edx*/) {
    int t1 /*eax*/ = c - b;
    int t2 /*ecx*/ = (unsigned)t1 >> 1;
    t1 = (t1 + t2) >> 1;
    t2 = t1 + b;
    if (t2 > a) {
        return 2 * func4(a, b, t2 - 1);
    } else {
        if (t2 >= a) return 0;
        return 2 * func4(a, t2 + 1, c);
    }
}

void phase_4(char *input /*rdi*/) {
    int a, b;
    int n = sscanf(input, "%d %d", &a, &b);
    if (n != 2) {
        explode();
    }
    if (a > 14) {
        explode();
    }
    // a <= 14
    int res = func4(a, 0, 14);
    if (res == 0 && b == 0) {
        return;
    } else {
        explode();
    }
}

char *global_s = "maduiersnfotvbyl";
// 0123456789abcdef
void phase_5(char *input) {
    if (string_length(input) != 6) {
        explode();
    }
    char s[6]; // [rsp+0x10, rsp+0x16)
    char ch = '\0'; // rsp+0x16
    for (int i /*eax*/ = 0; i < 6; i++) {
        s[i] = global_s[input[i] & 0xf];
    }
    if (string_not_equal(s, "flyers")) {
        explode();
    }
}

void phase_6(char *input /*rdi*/) {
    int arr[6]; // rsp

    typedef int value_t;

    typedef struct some {
        value_t val;
        struct some *next; // offset is 8
    } some_t;

    some_t *parr[6]; // rsp+0x20

    read_six_numbers(input, arr);

_40110e:
    for (int i = 0; i < 6; i++) {
        if ((unsigned)(arr[i] - 1) > 5u) {
            explode();
        }
        // arr[i] is in [1, 6]
        for (int j = i + 1; j < 6; j++) {
            if (arr[0] == arr[j]) {
                explode();
            }
        }
    }

_401153:
    for (int *p = arr; p != &arr[6]; p++) {
        *p = 7 - *p;
    }

_40116f:
    for (int i /*rsi/4*/ = 0; i < 6; i++) {
        parr[i] /*rdx*/ = (some_t *)0x6032d0;
        if (arr[i] /*ecx*/ > 1) {
            for (int j /*eax*/ = 1; j < arr[i]; j++) {
                parr[i] = parr[i]->next; // rdx = 0x8(rdx)
            }
        }
    }

_4011ab:
    some_t *last = parr[0] /*rbc*/; // rcx
    some_t **p = &parr[1]; // rax
    while (p != &arr[6] /*rsi*/) {
        last->next = *p /*rdx*/;
        last = *p;
        p++;
    }
    last->next = NULL;

_4011da:
    some_t *psome = parr[0]; // rbx
    for (int i /*ebp*/ = 5; i > 0; i--) {
        if (psome->val /*(rbx)*/ < psome->next->val /*eax*/) {
            explode();
        }
        psome = psome->next;
    }
}

typedef struct tree {
    int data;
    struct tree *left; // offset is 0x8
    struct tree *right; // offset is 0x10
} tree_t;

int fun7(tree_t *te /*rdi*/, int a /*esi*/) {
    if (te) {
        if (te->data /*edx*/ > a) {
            return 2 * fun7(te->left, a);
        } else {
            // te->data <= a
            if (te->data == a) {
                return 0;
            } else {
                return 2 * fun7(te->right, a) + 1;
            }
        }
    }
    return -1; // 0xffffffff
}

void secret_phase() {
    char *input = read_line();
    int res /*ebx*/ = strtol(input, 0, 10);
    if ((unsigned)(res - 1) /*eax*/ > 1000) {
        explode();
    }
    // res in [1, 1001]
    if (fun7(0x6030f0, res) != 2) {
        explode();
    }
    puts("Wow! You've defused the secret stage!");
    phase_defused();
}
