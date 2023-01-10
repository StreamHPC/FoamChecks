#include <clang/ASTMatchers/ASTMatchers.h>

#include <clang/Tooling/Transformer/RewriteRule.h>
#include <clang/Tooling/Transformer/Stencil.h>
#include <clang/Tooling/Transformer/Transformer.h>

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

#include <clang-tidy/utils/TransformerClangTidyCheck.h>

namespace tidy = clang::tidy;
namespace tf = clang::transformer;
namespace ast = clang::ast_matchers;

auto MakeMinMaxNamespaceRule() {
  const auto isUnqualifiedDeclRefExpr =
      ast::declRefExpr(ast::unless(ast::has(ast::nestedNameSpecifierLoc())));

  const auto isInFoamFunction =
      ast::hasAncestor(ast::functionDecl(ast::matchesName("^::Foam::.*")));

  const auto firstTwoParamsTypeIs = [](const char *Type) {
    return ast::allOf(
        ast::hasParameter(
            0, ast::hasType(ast::hasCanonicalType(ast::asString(Type)))),
        ast::hasParameter(
            1, ast::hasType(ast::hasCanonicalType(ast::asString(Type)))));
  };

  const auto isMinOrMax = ast::functionDecl(
      ast::anyOf(ast::hasName("::Foam::min"), ast::hasName("::Foam::max")),
      ast::parameterCountIs(2),
      ast::anyOf(firstTwoParamsTypeIs("const float"),
                 firstTwoParamsTypeIs("const double"),
                 firstTwoParamsTypeIs("const int")));

  return tf::makeRule(
      ast::callExpr(ast::callee(isMinOrMax),
                    ast::callee(ast::expr(ast::ignoringParenCasts(
                        isUnqualifiedDeclRefExpr.bind("callee")))),
                    ast::unless(isInFoamFunction)),
      tf::insertBefore(tf::node("callee"), tf::cat("Foam::")),
      tf::cat(tf::name("callee"),
              " should explicitly specify the Foam namespace"));
}

class MinMaxNamespaceCheck : public tidy::utils::TransformerClangTidyCheck {
public:
  MinMaxNamespaceCheck(llvm::StringRef Name, tidy::ClangTidyContext *Context)
      : TransformerClangTidyCheck(Name, Context) {
    setRule(MakeMinMaxNamespaceRule());
  }
};

class FoamModule : public tidy::ClangTidyModule {
public:
  void
  addCheckFactories(tidy::ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MinMaxNamespaceCheck>("foam-minmax-namespace");
  }
};

static tidy::ClangTidyModuleRegistry::Add<FoamModule> X("foam",
                                                        "Adds OpenFOAM checks");
