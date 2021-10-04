#pragma once
#pragma warning(disable : 4996)
#ifdef __cplusplus

extern "C" {
#endif
	typedef struct List_t
	{
		int priority;
		int data;
		struct List_t* next;
	}List_t;
	typedef struct
	{
		List_t* start;
	}Queue_t;

	Queue_t* CreateQueue();
	void PushQueue(Queue_t* queue, int value, int priority);
	void DestroyQueue(Queue_t* queue);
	int IsEmpty(Queue_t* queue);
	int PopQueue(Queue_t* queue);

#ifdef __cplusplus
}
#endif
