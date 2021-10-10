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
	free(queue->start);
	free(queue);
}
TEST(PushQueueTest, Add_Element_In_PushQueue1_Val_2_Pri_3) {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	queue->start = (List_t*)malloc(sizeof(List_t));
	if (!queue->start)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->data = 22;
	queue->start->priority = 2;
	queue->start->next = NULL;

	PushQueue(queue, 11, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->next->data, 22);
	EXPECT_EQ(queue->start->next->priority, 2);
	free(queue->start->next);
	free(queue->start);
	free(queue);
}
TEST(PushQueueTest, Add_Element_In_PushQueue_Val_2_Pri_3) {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	queue->start= (List_t*)malloc(sizeof(List_t));
	if (!queue->start)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->data = 0;
	queue->start->priority = 0;
	queue->start->next = NULL;

	PushQueue(queue, 2, 3);
	EXPECT_EQ(queue->start->data, 2);
	EXPECT_EQ(queue->start->priority, 3);
	free(queue->start);
	free(queue);
}
TEST(PushQueueTest, Add_Element_In_PushQueue_And_Check_Order) {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	queue->start = (List_t*)malloc(sizeof(List_t));
	if (!queue->start)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->data = 0;
	queue->start->priority = 0;
	queue->start->next = NULL;

	PushQueue(queue, 11, 1);
	PushQueue(queue, 33, 3);
	PushQueue(queue, 22, 2);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->next->priority, 2);
	EXPECT_EQ(queue->start->next->data, 22);
	EXPECT_EQ(queue->start->next->next->priority, 3);
	EXPECT_EQ(queue->start->next->next->data, 33);
	
	free(queue->start->next->next);
	free(queue->start->next);
	free(queue->start);
	free(queue);
}
TEST(PushQueueTest, Add_Element_In_PushQueue_And_Check_Order_With_Different_Input_Sequence) {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	queue->start = (List_t*)malloc(sizeof(List_t));
	if (!queue->start)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->data = 0;
	queue->start->priority = 0;
	queue->start->next = NULL;

	PushQueue(queue, 22, 2);
	PushQueue(queue, 11, 1);
	PushQueue(queue, 33, 3);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->next->priority, 2);
	EXPECT_EQ(queue->start->next->data, 22);
	EXPECT_EQ(queue->start->next->next->priority, 3);
	EXPECT_EQ(queue->start->next->next->data, 33);

	free(queue->start->next->next);
	free(queue->start->next);
	free(queue->start);
	free(queue);
}
TEST(PushQueueTest, Add_Element_In_PushQueue_And_Check_Order_With_Different_Input_Sequence_2) {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	queue->start = (List_t*)malloc(sizeof(List_t));
	if (!queue->start)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->data = 0;
	queue->start->priority = 0;
	queue->start->next = NULL;

	PushQueue(queue, 22, 2);
	PushQueue(queue, 11, 1);
	PushQueue(queue, 33, 1);
	EXPECT_EQ(queue->start->priority, 1);
	EXPECT_EQ(queue->start->data, 11);
	EXPECT_EQ(queue->start->next->priority, 1);
	EXPECT_EQ(queue->start->next->data, 33);
	EXPECT_EQ(queue->start->next->next->priority, 2);
	EXPECT_EQ(queue->start->next->next->data, 22);

	free(queue->start->next->next);
	free(queue->start->next);
	free(queue->start);
	free(queue);
}
TEST(PopQueueTest, Create_Queue_And_Check_Pop) {
	int element = 0;
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	if (!queue)
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	queue->start = (List_t*)malloc(sizeof(List_t));
	if (!queue->start)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->data = 11;
	queue->start->priority = 1;
	queue->start->next = (List_t*)malloc(sizeof(List_t));
	if (!queue->start->next)
	{
		printf("Memory allocation error\n");
		free(queue);
		exit(1);
	}
	queue->start->next->data = 22;
	queue->start->next->priority=2;
	queue->start->next->next = NULL;

	element = PopQueue(queue);
	EXPECT_EQ(element, 11);
	element = PopQueue(queue);
	EXPECT_EQ(element, 22);
}
int main() {
	testing::InitGoogleTest();
	RUN_ALL_TESTS();
	return 0;
}

