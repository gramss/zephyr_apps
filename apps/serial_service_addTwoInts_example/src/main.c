#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include "example_interfaces/srv/add_two_ints.h"

#include <stdio.h>
#include <unistd.h>

#include <rmw_uros/options.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return 1;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

int main(int argc, const char * const * argv)
{
	//printf("starting main\n");
	rcl_init_options_t options = rcl_get_zero_initialized_init_options();
	RCCHECK(rcl_init_options_init(&options, rcl_get_default_allocator()))
	//printf("rcl init_options finished\n");

	// Optional RMW configuration 
	rmw_init_options_t* rmw_options = rcl_init_options_get_rmw_init_options(&options);
	//choose USART Line 2
	rmw_uros_options_set_serial_device("2", rmw_options);
	RCCHECK(rmw_uros_options_set_client_key(0xDEADBEEF, rmw_options))

	rcl_context_t context = rcl_get_zero_initialized_context();
	RCCHECK(rcl_init(argc, argv, &options, &context))
	//printf("rcl_init finished\n");

	rcl_node_options_t node_ops = rcl_node_get_default_options();
	rcl_node_t node = rcl_get_zero_initialized_node();
	RCCHECK(rcl_node_init(&node, "addtowints_server_rcl", "", &context, &node_ops))
	//printf("rcl_node_init finished\n");

	const char * service_name = "addtwoints";
	rcl_service_options_t service_op = rcl_service_get_default_options();
	rcl_service_t serv = rcl_get_zero_initialized_service();
	const rosidl_service_type_support_t * service_type_support = ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts);

	RCCHECK(rcl_service_init(&serv,&node, service_type_support, service_name, &service_op))
	//printf("rcl service init finished\n");

	rcl_wait_set_t wait_set = rcl_get_zero_initialized_wait_set();
	RCCHECK(rcl_wait_set_init(&wait_set, 0, 0, 0, 0, 1, 0, &context, rcl_get_default_allocator()))
	//printf("rcl_wait_set_init finished\n");

	do {
		RCSOFTCHECK(rcl_wait_set_clear(&wait_set))
		//printf("starting do\n");
		size_t index;
		RCSOFTCHECK(rcl_wait_set_add_service(&wait_set, &serv, &index))
		//printf("rcl_wait_set_add_service finished\n");
		rcl_wait(&wait_set, RCL_MS_TO_NS(1));
		//printf("rcl wait finished\n");
		for (size_t i = 0; i < wait_set.size_of_services; i++) {
			if (wait_set.services[i]) {   
				rmw_request_id_t req_id;
				example_interfaces__srv__AddTwoInts_Request req;
				example_interfaces__srv__AddTwoInts_Request__init(&req);
				RCSOFTCHECK(rcl_take_request(&serv,&req_id,&req))

				//printf("Service request value: %d + %d. Seq %d\n", (int)req.a, (int)req.b, (int) req_id.sequence_number);

				example_interfaces__srv__AddTwoInts_Response res;
				example_interfaces__srv__AddTwoInts_Response__init(&res);

				res.sum = req.a + req.b;

				RCSOFTCHECK(rcl_send_response(&serv,&req_id,&res))
			}
		}
	} while ( true );

  RCCHECK(rcl_service_fini(&serv,&node))
  RCCHECK(rcl_node_fini(&node))

  return 0;
}