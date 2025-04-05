#include <stdio.h>
#include <string.h>
#define MAX_ADDR_LEN 64
#define MAX_TXS 5
#define MAX_ADDRS 5
#define MAX_STACK 100
#define MAX_VISITED 100
typedef struct Transaction {
    char tx_hash[MAX_ADDR_LEN];
    char from[MAX_ADDRS][MAX_ADDR_LEN];
    char to[MAX_ADDRS][MAX_ADDR_LEN];
    int from_count;
    int to_count;
} Transaction;
typedef struct TraceItem {
    char wallet[MAX_ADDR_LEN];
    int depth;
} TraceItem;
Transaction tx_list[MAX_TXS] = {
    {
        .tx_hash = "tx001",
        .from = {"Alice"},
        .to = {"Bob", "Charlie"},
        .from_count = 1,
        .to_count = 2
    },
    {
        .tx_hash = "tx002",
        .from = {"Bob"},
        .to = {"Dave"},
        .from_count = 1,
        .to_count = 1
    },
    {
        .tx_hash = "tx003",
        .from = {"Charlie"},
        .to = {"Eve"},
        .from_count = 1,
        .to_count = 1
    },
    {
        .tx_hash = "tx004",
        .from = {"Eve"},
        .to = {"Frank", "Grace"},
        .from_count = 1,
        .to_count = 2
    },
    {
        .tx_hash = "tx005",
        .from = {"Dave"},
        .to = {"Heidi"},
        .from_count = 1,
        .to_count = 1
    }
};
int tx_count = 5;
char visited[MAX_VISITED][MAX_ADDR_LEN];
int visited_count = 0;
int is_visited(const char* wallet) {
    for (int i = 0; i < visited_count; i++) {
        if (strcmp(visited[i], wallet) == 0) return 1;
    }
    return 0;
}
void add_visited(const char* wallet) {
    if (!is_visited(wallet)) {
        strcpy(visited[visited_count++], wallet);
    }
}
void trace_transaction(const char* start_wallet) {
    printf("\n🔍 Tracing from wallet: %s\n\n", start_wallet);
    TraceItem stack[MAX_STACK];
    int stack_top = 0;
    strcpy(stack[stack_top].wallet, start_wallet);
    stack[stack_top].depth = 0;
    stack_top++;
    while (stack_top > 0) {
        stack_top--;
        TraceItem current = stack[stack_top];
        if (is_visited(current.wallet)) continue;
        add_visited(current.wallet);
        int found = 0;
        for (int i = 0; i < tx_count; i++) {
            for (int j = 0; j < tx_list[i].from_count; j++) {
                if (strcmp(tx_list[i].from[j], current.wallet) == 0) {
                    found = 1;
                    for (int k = 0; k < current.depth; k++) printf("  ");
                    printf("→ %s (tx: %s)\n", current.wallet, tx_list[i].tx_hash);
                    if (tx_list[i].to_count > 1) {
                        for (int k = 0; k < current.depth + 1; k++) printf("  ");
                        printf("⚠ Mixer-like behavior detected (to %d addresses)\n", tx_list[i].to_count);
                    }
                    for (int k = 0; k < tx_list[i].to_count; k++) {
                        strcpy(stack[stack_top].wallet, tx_list[i].to[k]);
                        stack[stack_top].depth = current.depth + 1;
                        stack_top++;
                    }
                }
            }
        }
        if (!found) {
            for (int k = 0; k < current.depth; k++) printf("  ");
            printf("✔ Final receiver: %s\n", current.wallet);
        }
    }
}
int main() {
    const char* start_wallet = "Alice";
    trace_transaction(start_wallet);
    return 0;
}