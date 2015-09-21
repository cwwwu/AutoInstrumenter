#ifndef INSERTBRACESFRONTENDACTION_H
#define INSERTBRACESFRONTENDACTION_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "InsertBracesASTConsumer.h"

using namespace clang;
using namespace llvm;

class InsertBracesFrontendAction : public ASTFrontendAction {
 public:
  virtual ASTConsumer *CreateASTConsumer(CompilerInstance &Compiler, StringRef InFile);

 private:
  Rewriter rewriter;
};

#endif