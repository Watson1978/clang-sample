// http://d.hatena.ne.jp/fjnl/20110311/1299798998
#include <llvm/Support/Host.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclGroup.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/FrontendOptions.h>
#include <clang/Parse/ParseAST.h>

struct SampleConsumer : clang::ASTConsumer {
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef group) {
        for (auto it = group.begin(); it != group.end(); ++it) {
            if (auto const* fn = llvm::dyn_cast<clang::FunctionDecl>(*it)) {
                check(*fn);
            }
        }
        return true;
    }

private:
    void check(clang::FunctionDecl const& fn) const {
        if (fn.isMain()) {
            if (fn.param_size() > 0) {
                llvm::outs() << "main has some arguments.\n";
            } else {
                llvm::outs() << "main doesn't have any arguments.\n";
            }
        }
    }
};

int main(int argc, char** argv)
{
    if (argc <= 1) {
        llvm::errs() << "sample [input file]\n";
        return 1;
    }

    clang::CompilerInstance CI;
    clang::LangOptions Opts;

    CI.createDiagnostics();
    CI.getInvocation().setLangDefaults(Opts, clang::IK_ObjCXX);
    clang::CompilerInvocation::CreateFromArgs(
        CI.getInvocation(),
        argv + 1, argv + argc,
        CI.getDiagnostics()
    );

    CI.setTarget(
        clang::TargetInfo::CreateTargetInfo(
            CI.getDiagnostics(),
            CI.getInvocation().TargetOpts
            )
    );

    CI.createFileManager();
    CI.createSourceManager(CI.getFileManager());
    CI.getDiagnosticClient().BeginSourceFile(CI.getLangOpts(), nullptr);
    CI.createPreprocessor(clang::TU_Complete);
    CI.createASTContext();
    CI.setASTConsumer(llvm::make_unique<SampleConsumer>());

    auto const n = CI.getFrontendOpts().Inputs.size();
    if (n == 0) {
        llvm::errs() << "No input file.\n";
        return 1;
    } else if (n > 1) {
        llvm::errs() << "There are too many inputs.\n";
        return 1;
    }

    CI.InitializeSourceManager(
        CI.getFrontendOpts().Inputs[0]
    );

    clang::ParseAST(
        CI.getPreprocessor(),
        &CI.getASTConsumer(),
        CI.getASTContext()
    );

    return 0;
}
