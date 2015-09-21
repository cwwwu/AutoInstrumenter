#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/FrontendAction.h"

#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CompilationDatabase.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Host.h"

#include "InsertBracesFrontendAction.h"
#include "InstrumentFrontendAction.h"

#include "InstrumentationMap.h"
#include "AutoInstrumentOpts.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace std;
using namespace clang::auto_instrument;

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
//static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
//static cl::extrahelp MoreHelp("\nMore help text...");

cl::opt<std::string> BuildPath(
  cl::Positional,
  cl::desc("[<build-path>]"));

cl::opt<std::string> InstrumentationScheme(
  cl::Positional,
  cl::desc("<instrumentation-scheme>"),
  cl::Required);

cl::opt<std::string> OutputMainFunctionMeta(
  cl::Positional,
  cl::desc("<main-function-meta>"),
  cl::Required);

cl::list<std::string> SourcePaths(
  cl::Positional,
  cl::desc("<source0> [... <sourceN>]"),
  cl::OneOrMore);

int main(int argc, const char **argv) {

  int result;

  llvm::OwningPtr<CompilationDatabase> Compilations(
        FixedCompilationDatabase::loadFromCommandLine(argc, argv));
  cl::ParseCommandLineOptions(argc, argv);

  if(!Compilations) { // Couldn't find a compilation DB from the command line
    string ErrorMessage;
    Compilations.reset(
      !BuildPath.empty() ?
        CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
        CompilationDatabase::autoDetectFromSource(SourcePaths[0], ErrorMessage)
      );

    if (!Compilations) // Still no compilation DB? - bail.
      llvm::report_fatal_error(ErrorMessage);
  }

  // Ensure that clang can parse the source files first
  llvm::errs() << "========== Checking Syntax of Source File(s) =========\n";
  ClangTool SyntaxTool(*Compilations, SourcePaths);
  if ( (result = SyntaxTool.run(
        newFrontendActionFactory<clang::SyntaxOnlyAction>())) ) {
    llvm::errs() << "Error parsing files\n";
    return result;
  }

  // Insert braces in source files to ensure that instrumented source compiles
  llvm::errs() << "========== Inserting Braces in Source File(s) =========\n";
  ClangTool InsertBracesTool(*Compilations, SourcePaths);
  result = InsertBracesTool.run(newFrontendActionFactory<InsertBracesFrontendAction>());

  llvm::errs() << "---------- Checking Syntax of Modified Source File(s) ---------\n";
  ClangTool AfterBracesSyntaxTool(*Compilations, SourcePaths);
  if ( (result = AfterBracesSyntaxTool.run(
        newFrontendActionFactory<clang::SyntaxOnlyAction>())) ) {
    llvm::errs() << "Error parsing files after inserting braces\n";
    return result;
  }

  llvm::errs() << "========== Auto-instrumenting Source File(s) =========\n";
  ClangTool InstrumentTool(*Compilations, SourcePaths);
  result = InstrumentTool.run(newFrontendActionFactory<InstrumentFrontendAction>());

  llvm::errs() << "---------- Checking Syntax of Instrumented Source File(s) ---------\n";
  ClangTool AfterInstrumentSyntaxTool(*Compilations, SourcePaths);
  if ( (result = AfterInstrumentSyntaxTool.run(
        newFrontendActionFactory<clang::SyntaxOnlyAction>())) ) {
    llvm::errs() << "Error parsing files after auto-instrumenting\n";
    return result;
  }
	return 0;
}
