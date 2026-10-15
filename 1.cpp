#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义候补订单结构体
typedef struct Order {
    int order_id;           // 订单号
    char name[50];          // 旅客姓名
    char id_card[30];       // 身份证号
    int tickets_needed;     // 该订单所需票数（基础版可默认每人1张）
    struct Order* next;     // 指向下一个订单的指针
} Order;

// 定义队列结构体
typedef struct {
    Order* front;           // 队头指针
    Order* rear;            // 队尾指针
    int size;               // 队列当前大小（排队人数）
} Queue;

// 初始化队列
Queue* initQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// 判断队列是否为空
int isEmpty(Queue* q) {
    return q->front == NULL;
}

// 入队操作（新增候补订单，加入队尾）
void enqueue(Queue* q, int id, char* name, char* id_card, int tickets) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->order_id = id;
    strcpy(newOrder->name, name);
    strcpy(newOrder->id_card, id_card);
    newOrder->tickets_needed = tickets;
    newOrder->next = NULL;

    if (isEmpty(q)) {
        q->front = newOrder;
        q->rear = newOrder;
    } else {
        q->rear->next = newOrder;
        q->rear = newOrder;
    }
    q->size++;
    printf("? 候补订单提交成功！订单号: %d，当前排队人数: %d\n", id, q->size);
}

// 余票发放处理逻辑
void processTickets(Queue* q, int k) {
    if (isEmpty(q)) {
        printf("?? 当前无候补订单，无需发放余票。\n");
        return;
    }

    printf("\n--- 开始发放余票，本次释放余票数: %d ---\n", k);
    int processed_count = 0;

    // 循环从队头开始处理，直到余票不足或队列为空
    while (!isEmpty(q) && k > 0) {
        Order* current = q->front;
        
        // 判断剩余余票是否满足当前订单所需票数
        if (k >= current->tickets_needed) {
            // 处理候补成功，出队
            printf("?? 候补成功！订单号: %d, 姓名: %s, 身份证: %s (分配 %d 张票)\n", 
                   current->order_id, current->name, current->id_card, current->tickets_needed);
            
            k -= current->tickets_needed; // 扣除余票

            // 执行出队操作
            q->front = q->front->next;
            if (q->front == NULL) {
                q->rear = NULL; // 队列空了
            }
            free(current); // 释放已出票订单的内存
            q->size--;
            processed_count++;
        } else {
            // 余票不足以满足当前队头订单的需求，停止发放，保留订单继续排队
            printf("?? 余票不足，当前排队第一位的订单需要 %d 张票，剩余余票 %d 张。停止本次发放。\n", 
                   current->tickets_needed, k);
            break;
        }
    }

    printf("--- 本次余票发放结束 ---\n");

    // 检查当前队列状态
    if (isEmpty(q)) {
        printf("当前候补队列已清空。\n");
    } else {
        printf("当前还有 %d 个订单在继续等待候补。\n", q->size);
    }
}

// 释放整个队列的内存
void freeQueue(Queue* q) {
    Order* current = q->front;
    while (current != NULL) {
        Order* temp = current;
        current = current->next;
        free(temp);
    }
    free(q);
}

int main() {
    Queue* waitlist = initQueue();
    int choice;
    int order_id_counter = 1001; // 模拟自动生成的订单号

    printf("====================================\n");
    printf(" 欢迎使用 12306 车票候补模拟系统\n");
    printf("====================================\n");

    while (1) {
        printf("\n请选择操作：\n");
        printf("1. 新增候补订单 (入队)\n");
        printf("2. 余票发放 (出票)\n");
        printf("3. 退出程序\n");
        printf("请输入选项 (1-3): ");
        
        if (scanf("%d", &choice) != 1) {
            // 清理输入缓冲区防止死循环
            while (getchar() != '\n');
            printf("输入无效，请输入数字！\n");
            continue;
        }

        if (choice == 1) {
            char name[50];
            char id_card[30];
            int tickets = 1; // 默认每位旅客需要1张票

            printf("请输入旅客姓名: ");
            scanf("%s", name);
            printf("请输入身份证号: ");
            scanf("%s", id_card);
            // 基础版可以不输入票数，如果需要多张票可在此处添加输入提示
            // printf("请输入所需票数: ");
            // scanf("%d", &tickets);

            enqueue(waitlist, order_id_counter++, name, id_card, tickets);

        } else if (choice == 2) {
            if (isEmpty(waitlist)) {
                printf("?? 当前没有候补订单，无需发放。\n");
            } else {
                int k;
                printf("请输入本次释放的余票数量k: ");
                if (scanf("%d", &k) != 1) {
                    while (getchar() != '\n');
                    printf("输入无效！\n");
                    continue;
                }
                if (k <= 0) {
                    printf("余票数量必须大于0！\n");
                } else {
                    processTickets(waitlist, k);
                }
            }

        } else if (choice == 3) {
            printf("感谢使用，程序退出。\n");
            // 补充要求3逻辑：退出前判断是否还有排队
            if (!isEmpty(waitlist)) {
                printf("当前还有 %d 位旅客在继续等待候补哦~\n", waitlist->size);
            }
            break;

        } else {
            printf("无效选项，请重新选择。\n");
        }
    }

    // 释放内存，避免内存泄漏
    freeQueue(waitlist);
    return 0;
}
