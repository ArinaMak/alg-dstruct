#pragma once
#include "gtest/gtest.h"
extern "C" {
#include "pqueueinfo.h"
}
TEST(CreateQueueTest, Create_Queue_And_Check_Expect_True) {

	Queue_t* queue = NULL;
	queue = CreateQueue();
	EXPECT_TRUE(queue);
	EXPECT_TRUE(queue->start);
	EXPECT_FALSE(queue->start->data);
	EXPECT_FALSE(queue->start->priority);
	EXPECT_FALSE(queue->start->next);
	DestroyQueue(queue);
}
TEST(PushQueueTest, Add_Element_In_PushQueue_Val_2_Pri_3) {
	Queue_t* queue = NULL;
	queue = CreateQueue();
	PushQueue(queue, 2, 3);
	EXPECT_EQ(queue->start->data, 2);
	EXPECT_EQ(queue->start->priority, 3);
}
TEST(PushQueueTest, Add_Element_In_PushQueue_And_Check_Order) {
	Queue_t* queue = NULL;
	queue = CreateQueue();
	PushQueue(queue, 11, 1);
	PushQueue(queue, 33, 3);
	PushQueue(queue, 22, 2);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->next->priority, 2);
	EXPECT_EQ(queue->start->next->data, 22);
	EXPECT_EQ(queue->start->next->next->priority, 3);
	EXPECT_EQ(queue->start->next->next->data, 33);
	DestroyQueue(queue);
}

TEST(PushQueueTest, Add_Element_In_PushQueue_And_Check_Order_With_Different_Input_Sequence) {
	Queue_t* queue = NULL;
	queue = CreateQueue();
	PushQueue(queue, 22, 2);
	PushQueue(queue, 11, 1);
	PushQueue(queue, 33, 3);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->next->priority, 2);
	EXPECT_EQ(queue->start->next->data, 22);
	EXPECT_EQ(queue->start->next->next->priority, 3);
	EXPECT_EQ(queue->start->next->next->data, 33);
	DestroyQueue(queue);
}

TEST(PushQueueTest, Add_Element_In_PushQueue_And_Check_Order_With_Different_Input_Sequence_2) {
	Queue_t* queue = NULL;
	queue = CreateQueue();
	PushQueue(queue, 22, 2);
	PushQueue(queue, 11, 1);
	PushQueue(queue, 33, 1);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->next->priority, 1);
	EXPECT_EQ(queue->start->next->data, 33);
	EXPECT_EQ(queue->start->next->next->priority, 2);
	EXPECT_EQ(queue->start->next->next->data, 22);
	DestroyQueue(queue);
}
TEST(PopQueueTest, Create_Queue_And_Check_Pop) {
	Queue_t* queue = NULL;
	int element = 0;
	queue = CreateQueue();
	PushQueue(queue, 22, 2);
	PushQueue(queue, 11, 1);
	element = PopQueue(queue);
	EXPECT_EQ(element, 11);
}
int main() {
	testing::InitGoogleTest();
	RUN_ALL_TESTS();
	return 0;
}

