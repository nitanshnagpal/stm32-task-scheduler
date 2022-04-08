#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <cstring>
#include "lwmem.h"  /* Using Lightweight dynamic memory manager (By - Tilen MAJERLE) for dynamic memory allocation */

/* Task Structure */
typedef struct task {
	void (*TaskFunc)(void); /* Task function */
	struct task *next;      /* Pointer to next Task Structure. It behaves like Linked List */
}task;
struct task *head_task_node; /* Head node of Linked List */

/**
 * \brief           Define region data for dynamic memory allocation of task structure
 */
uint8_t region1_data[1024];

/**
 * \brief           Define final regions
 */
static lwmem_region_t regions[] = {
    { region1_data, sizeof(region1_data) },
    /* Add more regions if needed */
};

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * @brief - Add task to the task list
 * @param - struct task *task_var : Task Structure
 * @return - -1 on failure, 0 on success
 */
int add_task(struct task *task_var);

/*
 * @brief - Delete task from task list
 * @param - Task index : Which task to delete i.e. 1, 2, 3, etc 
 * @return - -1 on failure, 0 on success
 */
int del_task(uint8_t task_index);

#endif