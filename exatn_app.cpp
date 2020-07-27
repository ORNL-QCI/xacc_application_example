#include "exatn.hpp"

#include <vector>

void exatn_simulation() {

 auto tensor = std::make_shared<exatn::Tensor>("Tensor",exatn::TensorShape{2,2,2,2,2});
 auto success = exatn::createTensor(tensor,exatn::TensorElementType::REAL64); assert(success);

 std::vector<double> tensor_data(32,1.0);
 success = exatn::initTensorData("Tensor",tensor_data); assert(success);

 success = exatn::destroyTensor("Tensor"); assert(success);

 exatn::sync();

 return;
}


int main(int argc, char** argv) {

 //Initialize the ExaTN runtime:
 exatn::ParamConf exatn_parameters;
 //Set the available CPU Host RAM size to be used by ExaTN:
 exatn_parameters.setParameter("host_memory_buffer_size",2L*1024L*1024L*1024L);
#ifdef MPI_ENABLED
 int thread_provided;
 int mpi_error = MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &thread_provided);
 assert(mpi_error == MPI_SUCCESS);
 assert(thread_provided == MPI_THREAD_MULTIPLE);
 exatn::initialize(exatn::MPICommProxy(MPI_COMM_WORLD),exatn_parameters,"lazy-dag-executor");
#else
 exatn::initialize(exatn_parameters,"lazy-dag-executor");
#endif

 //Perform the ExaTN simulation defined above:
 exatn_simulation();

 //Finalize the ExaTN runtime:
 exatn::finalize();
#ifdef MPI_ENABLED
 mpi_error = MPI_Finalize(); assert(mpi_error == MPI_SUCCESS);
#endif

 return 0;
}
