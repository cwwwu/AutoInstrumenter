# AutoInstrumenter
LLVM/Clang-based tool for instrumenting C code

This Clang-based tool was written as a part of the research work I conducted while I was in grad school. This tool was used to automatically instrument C code with additional code that would be used by a runtime verification monitor.

Currently, the code that is present in this repository was developed and tested with LLVM 3.2.

To build this tool, get a copy of the LLVM 3.2 source code. Then, overlay the contents of this repository in LLVM_ROOT_SRC_DIR/tools/clang/tools.
