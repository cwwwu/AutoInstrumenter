# AutoInstrumenter
LLVM/Clang-based tool for instrumenting C code

This Clang-based tool was written as a part of the research work I conducted while I was in grad school. The research I was working on pertained to determining efficient instrumentation schemes for runtime verification; you can find my research paper that I publishd [here](https://uwaterloo.ca/embedded-software-group/sites/ca.embedded-software-group/files/uploads/files/RV13-rv-hybrid.pdf). This tool was used to automatically instrument C code with additional code that would be used by a runtime verification monitor so that I could automate the majority of my experiments.

Currently, the code that is present in this repository was developed and tested with LLVM 3.2.

To build this tool, get a copy of the [LLVM 3.2 source code](http://llvm.org/releases/download.html#3.2). Then, overlay the contents of this repository in `LLVM_ROOT_SRC_DIR/tools/clang/tools`.
