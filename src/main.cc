#include <llvmPy.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace llvmPy;
namespace cl = llvm::cl;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

cl::opt<string> Cmd(
        "c",
        cl::desc("Program passed as input"),
        cl::value_desc("cmd"));

cl::opt<bool> IsIR(
        "ir",
        cl::desc("Print resulting LLVM IR and exit"));

cl::opt<string> Filename(
        cl::Positional,
        cl::desc("Program file"));

int
main(int argc, char **argv)
{
    cl::ParseCommandLineOptions(argc, argv);

    std::vector<Token> tokens;
    std::vector<Stmt *> stmts;

    if (Cmd.getPosition() && Filename.getPosition()) {
        cerr << "Only one of cmd or filename may be specified." << endl;
        exit(1);
    } else if (Cmd.getPosition()) {
        std::stringstream ss(Cmd);
        Lexer lexer(ss);
        lexer.tokenize(tokens);
    } else if (Filename.getPosition()) {
        std::ifstream input;
        input.open(Filename, std::ios::in);
        Lexer lexer(input);
        lexer.tokenize(tokens);
    }

    Parser parser(tokens);
    parser.parse(stmts);

    RT rt;
    Compiler compiler(rt);
    Emitter em(compiler);

    RTModule &mod = *em.createModule("__main__", stmts);

    if (IsIR) {
        mod.getModule().print(llvm::outs(), nullptr);
    } else {
        cerr << "Only the --ir option is supported at this time." << endl;
        exit(1);
    }

    return 0;
}