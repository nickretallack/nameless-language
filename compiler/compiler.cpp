// #include "../include/KaleidoscopeJIT.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Transforms/Scalar.h"
#include <cctype>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using std::to_string;
using json = nlohmann::json;

using namespace llvm;
// using namespace llvm::orc;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string, json> definitions;

static std::map<std::string, APInt> symbolValues;
static int currentSymbolValue = 0;
Value *SymbolValueGet(std::string id)
{
  if (symbolValues.find(id) != symbolValues.end())
  {
    symbolValues[id] = currentSymbolValue;
    currentSymbolValue++;
  }
  return ConstantInt::get(*TheContext, APInt(symbolValues[id]));
}

json findEntryPoint(json connections, int nibIndex)
{
  for (json::iterator it = connections.begin(); it != connections.end(); it++)
  {
    json connection = *it;
    json sink = connection["sink"];
    if (sink["node"]["type"] == "graph" && sink["nib"]["type"] == "nib" && sink["nib"]["index"] == nibIndex)
    {
      return connection["source"];
    }
  }
}

json findSource(json connections, int nodeIndex, int nibIndex)
{
  for (json::iterator it = connections.begin(); it != connections.end(); it++)
  {
    json connection = *it;
    json sink = connection["sink"];
    if (sink["node"]["type"] == "node" && sink["node"]["index"] == nodeIndex && sink["nib"]["type"] == "nib" && sink["nib"]["index"] == nibIndex)
    {
      return connection["source"];
    }
  }
}

Value *compileSource(json graph, json source);
Value *compileSink(json graph, int nodeIndex, int nibIndex)
{
  json source = findSource(graph["connections"], nodeIndex, nibIndex);
  return compileSource(graph, source);
}

Value *compileSource(json graph, json source)
{

  json sourceNode = source["node"];
  if (sourceNode["type"] == "graph")
  {
    std::cout << "Found a graph input\n";
    // graph connections: TODO
  }
  else
  {
    int nodeIndex = sourceNode["index"];

    json nodes = graph["nodes"];
    json node = nodes[nodeIndex];
    std::string kind = node["kind"];

    if (kind == "function call")
    {
      std::string contentID = node["contentID"];
      if (contentID == "dee1e378171f8de7a4882c442055f818fa248e4b030b7fa5d26e348f5c5fd496") // add
      {
        Value *left = compileSink(graph, nodeIndex, 0);
        Value *right = compileSink(graph, nodeIndex, 1);

        return Builder->CreateFAdd(left, right, "addtmp");
      }
      else if (contentID == "0646a9929c4e9c05197370bf3a6cbe8ce646922b81e630f3a51e60ebe6ee6ad6") // branch
      {
        Value *condition = compileSink(graph, nodeIndex, 0);
        Value *yes = SymbolValueGet("yes");
        condition = Builder->CreateFCmpONE(condition, ConstantInt::get(*TheContext, APInt(64, 0)), "branch-if");

        Function *TheFunction = Builder->GetInsertBlock()->getParent();

        // Create blocks for the then and else cases.  Insert the 'then' block at the
        // end of the function.
        BasicBlock *ThenBB =
            BasicBlock::Create(*TheContext, "then", TheFunction);
        BasicBlock *ElseBB = BasicBlock::Create(*TheContext, "else", TheFunction);
        BasicBlock *MergeBB = BasicBlock::Create(*TheContext, "ifcont", TheFunction);

        Builder->CreateCondBr(condition, ThenBB, ElseBB);

        Value *then = compileSink(graph, nodeIndex, 1);

        Builder->CreateBr(MergeBB);
        // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
        ThenBB = Builder->GetInsertBlock();

        Builder->SetInsertPoint(ElseBB);
        Value *otherwise = compileSink(graph, nodeIndex, 2);

        Builder->CreateBr(MergeBB);
        // codegen of 'Else' can change the current block, update ElseBB for the PHI.
        ElseBB = Builder->GetInsertBlock();

        TheFunction->insert(TheFunction->end(), MergeBB);
        Builder->SetInsertPoint(MergeBB);
        PHINode *PN =
            Builder->CreatePHI(Type::getDoubleTy(*TheContext), 2, "iftmp");

        PN->addIncoming(then, ThenBB);
        PN->addIncoming(otherwise, ElseBB);
        return PN;
      }
    }
    else if (kind == "value")
    {
      json definition = definitions[node["contentID"]];
      std::string type = definition["type"];
      if (type == "constant")
      {
        json value = definition["value"];
        if (value["type"] == "number")
        {
          double numberValue = value["value"];
          return ConstantFP::get(*TheContext, APFloat(numberValue));
        }
      }
      else if (type == "symbol")
      {
        return SymbolValueGet(definition["id"]);
      }
    }
  }
}

Value *compileGraph(json graph, int nibIndex)
{
  std::cout << graph["connections"] << '\n';
  std::cout << graph["nodes"] << '\n';

  json source = findEntryPoint(graph["connections"], nibIndex);
  return compileSource(graph, source);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Pass a JSON path\n";
    return 1;
  }

  std::ifstream f(argv[1]);
  json data = json::parse(f);
  json rawDefinitions = data["definitions"];

  for (json::iterator it = rawDefinitions.begin(); it != rawDefinitions.end(); ++it)
  {
    std::string canonicalString = it.value();
    definitions[it.key()] = json::parse(canonicalString);
  }

  json entryPoint = data["entryPoint"];
  int nibIndex = entryPoint["nibIndex"];
  json contentID = entryPoint["contentID"];
  json definition = definitions[contentID];
  std::string type = definition["type"];
  if (type != "graph")
  {
    std::cout << "non-graph\n";
    return 1;
  }

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("module", *TheContext);

  Builder = std::make_unique<IRBuilder<>>(*TheContext);

  std::cout << "compiling graph" << '\n';
  Value *value = compileGraph(definition, nibIndex);
  std::cout << "done\n";
  value->print(errs());
  std::cout << '\n';

  // TheJIT = ExitOnErr(KaleidoscopeJIT::Create());

  // // Open a new module.
}