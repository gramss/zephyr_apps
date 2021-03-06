#include <zephyr.h>
#include <device.h>
#include <stdio.h>
#include <sys/util.h>
#include <string.h>
#include <sys/printk.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <std_msgs/msg/int64.h>

#include <rmw_uros/options.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printk("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc);}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printk("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

void main(void)
{	
	rcl_init_options_t options = rcl_get_zero_initialized_init_options();

	RCCHECK(rcl_init_options_init(&options, rcl_get_default_allocator()))

	// Optional RMW configuration 
	rmw_init_options_t* rmw_options = rcl_init_options_get_rmw_init_options(&options);
	//choose USART Line 2
	rmw_uros_options_set_serial_device("2", rmw_options);
	RCCHECK(rmw_uros_options_set_client_key(0xDEADBEEF, rmw_options))

	rcl_context_t context = rcl_get_zero_initialized_context();
	RCCHECK(rcl_init(0, NULL, &options, &context))

	rcl_node_options_t node_ops = rcl_node_get_default_options();

	rcl_node_t node = rcl_get_zero_initialized_node();
	RCCHECK(rcl_node_init(&node, "zephyr_int64_publisher", "", &context, &node_ops))

	//publisher 1
	rcl_publisher_options_t publisher_ops = rcl_publisher_get_default_options();
	publisher_ops.qos.reliability = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
	rcl_publisher_t publisher = rcl_get_zero_initialized_publisher();
	RCCHECK(rcl_publisher_init(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int64), "zephyr_int64_publisher1", &publisher_ops))


	//publisher 2
	rcl_publisher_options_t publisher_ops2 = rcl_publisher_get_default_options();
	publisher_ops2.qos.reliability = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
	rcl_publisher_t publisher2 = rcl_get_zero_initialized_publisher();
	RCCHECK(rcl_publisher_init(&publisher2, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int64), "zephyr_int64_publisher2", &publisher_ops2))



	std_msgs__msg__Int64 msg;
	msg.data = 0;
	
	rcl_ret_t rc;
	rcl_ret_t rc2;

	do {
		rc = rcl_publish(&publisher, (const void*)&msg, NULL);
		rc2 = rcl_publish(&publisher2, (const void*)&msg, NULL);
		msg.data++;
		//k_sleep(1);
	} while (true);

	RCCHECK(rcl_publisher_fini(&publisher, &node))
	RCCHECK(rcl_publisher_fini(&publisher2, &node))

	RCCHECK(rcl_node_fini(&node))
}
