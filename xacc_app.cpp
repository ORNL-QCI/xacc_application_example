#include "xacc.hpp"

void tnqvm_simulation() {

 //Choose the desired quantum accelerator:
 auto qpu = xacc::getAccelerator("tnqvm", {std::make_pair("tnqvm-visitor", "exatn")});

 //Choose the desired quantum programming language:
 auto xasmCompiler = xacc::getCompiler("xasm");

 //Compile a quantum kernel into the quantum IR:
 auto ir = xasmCompiler->compile(
 R"(__qpu__ void ansatz(qbit q, double theta) {
    X(q[0]);
    Ry(q[1], theta);
    CX(q[1], q[0]);
    H(q[0]);
    H(q[1]);
    Measure(q[0]);
    Measure(q[1]);
 })", qpu);

 //Get the generated parameterized quantum circuit:
 auto circuit = ir->getComposite("ansatz");

 //Perform hybrid quantum/classical computation:
 auto angles = xacc::linspace(-3.1415, 3.1415, 20);
 for (auto & a: angles) {
  auto evaled = (*circuit)({a});
  auto qubits = xacc::qalloc(2);
  qpu->execute(qubits, evaled);
  auto exp_val = qubits->getExpectationValueZ();
  std::cout << "<X0X1>(" << a << ") = " << exp_val << "\n";
 }

 return;
}


int main(int argc, char** argv) {

 //Initialize the XACC runtime:
 xacc::Initialize(argc, argv);

 //Perform the quantum circuit simulation defined above:
 tnqvm_simulation();

 //Finalize the XACC runtime:
 xacc::Finalize();

 return 0;
}
