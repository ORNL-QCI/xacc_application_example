#include "exatn.hpp"

#include <vector>
#include <iostream>


void exatn_test()
{
 auto tensor = std::make_shared<exatn::Tensor>("Tensor",exatn::TensorShape{2,2,2,2,2});
 auto success = exatn::createTensor(tensor,exatn::TensorElementType::REAL64); assert(success);

 std::vector<double> tensor_data(32,1.0);
 success = exatn::initTensorData("Tensor",tensor_data); assert(success);

 success = exatn::destroyTensor("Tensor"); assert(success);

 exatn::sync();

 return;
}


void exatn_sycamore12_proto()
{
 using exatn::Tensor;
 using exatn::TensorShape;
 using exatn::TensorNetwork;
 using exatn::TensorElementType;

 exatn::resetRuntimeLoggingLevel(2); //debug

 const unsigned int num_qubits = 53;
 const unsigned int num_gates = 258; //total number of gates is 258
 std::vector<std::pair<unsigned int, unsigned int>> sycamore_12_cnot
 {
  {1,4},{3,7},{5,9},{6,13},{8,15},{10,17},{12,21},{14,23},{16,25},{18,27},
  {20,30},{22,32},{24,34},{26,36},{29,37},{31,39},{33,41},{35,43},{38,44},
  {40,46},{42,48},{45,49},{47,51},{50,52},{0,3},{2,6},{4,8},{7,14},{9,16},
  {11,20},{13,22},{15,24},{17,26},{19,29},{21,31},{23,33},{25,35},{30,38},
  {32,40},{34,42},{39,45},{41,47},{46,50},{0,1},{2,3},{4,5},{7,8},{9,10},
  {11,12},{13,14},{15,16},{17,18},{19,20},{21,22},{23,24},{25,26},{28,29},
  {30,31},{32,33},{34,35},{37,38},{39,40},{41,42},{44,45},{46,47},{49,50},
  {3,4},{6,7},{8,9},{12,13},{14,15},{16,17},{20,21},{22,23},{24,25},{26,27},
  {29,30},{31,32},{33,34},{35,36},{38,39},{40,41},{42,43},{45,46},{47,48},
  {50,51},{0,1},{2,3},{4,5},{7,8},{9,10},{11,12},{13,14},{15,16},{17,18},
  {19,20},{21,22},{23,24},{25,26},{28,29},{30,31},{32,33},{34,35},{37,38},
  {39,40},{41,42},{44,45},{46,47},{49,50},{3,4},{6,7},{8,9},{12,13},{14,15},
  {16,17},{20,21},{22,23},{24,25},{26,27},{29,30},{31,32},{33,34},{35,36},
  {38,39},{40,41},{42,43},{45,46},{47,48},{50,51},{1,4},{3,7},{5,9},{6,13},
  {8,15},{10,17},{12,21},{14,23},{16,25},{18,27},{20,30},{22,32},{24,34},
  {26,36},{29,37},{31,39},{33,41},{35,43},{38,44},{40,46},{42,48},{45,49},
  {47,51},{50,52},{0,3},{2,6},{4,8},{7,14},{9,16},{11,20},{13,22},{15,24},
  {17,26},{19,29},{21,31},{23,33},{25,35},{30,38},{32,40},{34,42},{39,45},
  {41,47},{46,50},{1,4},{3,7},{5,9},{6,13},{8,15},{10,17},{12,21},{14,23},
  {16,25},{18,27},{20,30},{22,32},{24,34},{26,36},{29,37},{31,39},{33,41},
  {35,43},{38,44},{40,46},{42,48},{45,49},{47,51},{50,52},{0,3},{2,6},{4,8},
  {7,14},{9,16},{11,20},{13,22},{15,24},{17,26},{19,29},{21,31},{23,33},
  {25,35},{30,38},{32,40},{34,42},{39,45},{41,47},{46,50},{0,1},{2,3},{4,5},
  {7,8},{9,10},{11,12},{13,14},{15,16},{17,18},{19,20},{21,22},{23,24},
  {25,26},{28,29},{30,31},{32,33},{34,35},{37,38},{39,40},{41,42},{44,45},
  {46,47},{49,50},{3,4},{6,7},{8,9},{12,13},{14,15},{16,17},{20,21},{22,23},
  {24,25},{26,27},{29,30},{31,32},{33,34},{35,36},{38,39},{40,41},{42,43},
  {45,46},{47,48},{50,51}
 };
 assert(num_gates <= sycamore_12_cnot.size());

 std::cout << "Building the circuit ... " << std::flush;

 TensorNetwork circuit("Sycamore12_CNOT");
 unsigned int tensor_counter = 0;

 //Left qubit tensors:
 unsigned int first_q_tensor = tensor_counter + 1;
 for(unsigned int i = 0; i < num_qubits; ++i){
  bool success = circuit.appendTensor(++tensor_counter,
                                      std::make_shared<Tensor>("Q"+std::to_string(i),TensorShape{2}),
                                      {});
  assert(success);
 }
 unsigned int last_q_tensor = tensor_counter;

 //CNOT gates:
 auto cnot = std::make_shared<Tensor>("CNOT",TensorShape{2,2,2,2});
 for(unsigned int i = 0; i < num_gates; ++i){
  bool success = circuit.appendTensorGate(++tensor_counter,
                                          cnot,
                                          {sycamore_12_cnot[i].first,sycamore_12_cnot[i].second});
  assert(success);
 }

 //Right qubit tensors:
 unsigned int first_p_tensor = tensor_counter + 1;
 for(unsigned int i = 0; i < num_qubits; ++i){
  bool success = circuit.appendTensor(++tensor_counter,
                                      std::make_shared<Tensor>("P"+std::to_string(i),TensorShape{2}),
                                      {{0,0}});
  assert(success);
 }
 unsigned int last_p_tensor = tensor_counter;
 std::cout << "Done\n" << std::flush;

 std::cout << "Simplifying the circuit ... " << std::flush;
 //Merge qubit tensors into adjacent CNOTs:
 for(unsigned int i = first_p_tensor; i <= last_p_tensor; ++i){
  const auto & tensor_legs = *(circuit.getTensorConnections(i));
  const auto other_tensor_id = tensor_legs[0].getTensorId();
  bool success = circuit.mergeTensors(other_tensor_id,i,++tensor_counter);
  assert(success);
 }
 for(unsigned int i = first_q_tensor; i <= last_q_tensor; ++i){
  const auto & tensor_legs = *(circuit.getTensorConnections(i));
  const auto other_tensor_id = tensor_legs[0].getTensorId();
  bool success = circuit.mergeTensors(other_tensor_id,i,++tensor_counter);
  assert(success);
 }
 std::cout << "Done\n" << std::flush;

 //Decompose all higher-than-rank-3 tensors:
 //circuit.decomposeTensors(); //optional
 circuit.printIt(); //debug

 //Generate the list of tensor operations for the circuit:
 std::cout << "Generating the list of tensor operations for the circuit ... " << std::flush;
 auto & operations = circuit.getOperationList("metis",true);
 std::cout << "Done\n" << std::flush;
 unsigned int max_rank = 0;
 std::cout << "Total FMA flop count = " << circuit.getFMAFlops()
           << ": Max intermdediate presence volume = " << circuit.getMaxIntermediatePresenceVolume()
           << ": Max intermdediate volume = " << circuit.getMaxIntermediateVolume(&max_rank)
           << ": Max intermdediate rank = " << max_rank << std::endl;

 std::cout << "Splitting some internal indices to reduce the size of intermediates ... " << std::flush;
 circuit.splitIndices(static_cast<std::size_t>(128UL*1024UL*1024UL));
 std::cout << "Done\n" << std::flush;
 circuit.printSplitIndexInfo();

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
 //exatn_test();
 exatn_sycamore12_proto();

 //Finalize the ExaTN runtime:
 exatn::finalize();
#ifdef MPI_ENABLED
 mpi_error = MPI_Finalize(); assert(mpi_error == MPI_SUCCESS);
#endif

 return 0;
}
