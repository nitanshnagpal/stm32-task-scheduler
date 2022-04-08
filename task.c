

#include "task.h"


int add_task(struct task *task_var)
{
	/* Linked List insertion algorithm */
	
	if(totalTaks >= 100)
	{
		sprintf(buffer, "Can not have more than 100 tasks");
		return -1;
	}
	
	task_var->next = head_task_node;
	head_task_node = task_var;
	totalTaks++;
	
	return 0;
}



int del_task(uint8_t task_index)
{
	/* Linked List deletion algorithm */
	
	if(task_index < 0 || task_index > 100)
	{
		uint8_t len = sprintf(buffer, "Invalid task index");
		HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, 100);
		return -1;
	}
	
	struct task *task_var = head_task_node;
	struct task *task_del;
	task_index = totalTaks - task_index;
	
	if(task_index == 0)
	{
		head_task_node = task_var->next;
		lwmem_free(task_var);
		totalTaks--;
		return 0;
	}
	
	uint8_t i = 0;
	
	for(i=0;i<(task_index - 1);i++)
		task_var = task_var->next;
	
	task_del = task_var->next;
	task_var->next = task_del->next;
	lwmem_free(task_del); /* Free memory of task_del structure */
	totalTaks--;
	
	return 0;
}
