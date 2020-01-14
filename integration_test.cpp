#include "xacc.hpp"

int main(int argc, char** argv) {
    xacc::Initialize(argc, argv);
    
    auto qpu = xacc::getAccelerator("tnqvm");//, {std::make_pair("tnqvm-visitor", "exatn")});

    auto xasmCompiler = xacc::getCompiler("xasm");
    auto ir = xasmCompiler->compile(R"(__qpu__ void ansatz(qbit q, double theta) {
      X(q[0]);
      Ry(q[1], theta);
      CX(q[1], q[0]);
      H(q[0]);
      H(q[1]);
      Measure(q[0]);
      Measure(q[1]);
    })", qpu);
    auto circuit = ir->getComposite("ansatz");

    auto angles = xacc::linspace(-3.1415, 3.1415, 20);

    for (auto& a : angles) {

        auto evaled = (*circuit)({a});

        auto qubits = xacc::qalloc(2);
        qpu->execute(qubits, evaled);

        auto exp_val = qubits->getExpectationValueZ();
        std::cout << "<X0X1>(" << a << ") = " << exp_val << "\n";
    }

    xacc::Finalize();

}
