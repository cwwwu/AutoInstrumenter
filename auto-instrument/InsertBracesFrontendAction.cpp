#include "InsertBracesFrontendAction.h"

ASTConsumer *InsertBracesFrontendAction::CreateASTConsumer(
    CompilerInstance &Compiler, StringRef InFile) {
  
  ASTContext *ctx = &Compiler.getASTContext();
  rewriter = Rewriter(ctx->getSourceManager(), ctx->getLangOpts());
  return new InsertBracesASTConsumer(rewriter);
}
