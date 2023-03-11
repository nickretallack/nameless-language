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
#include "nlohmann/json.hpp"

using std::to_string;
using json = nlohmann::json;

using namespace llvm;
// using namespace llvm::orc;

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string, json> definitions;

// followConnection(json graph) {

// }

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
      if (node["contentID"] == "dee1e378171f8de7a4882c442055f818fa248e4b030b7fa5d26e348f5c5fd496")
      {
        // add
        Value *left = compileSink(graph, nodeIndex, 0);
        Value *right = compileSink(graph, nodeIndex, 1);

        return Builder->CreateFAdd(left, right, "addtmp");
      }
    }
    else if (kind == "value")
    {
      json definition = definitions[node["contentID"]];
      if (definition["type"] == "constant")
      {
        json value = definition["value"];
        if (value["type"] == "number")
        {
          double numberValue = value["value"];
          return ConstantFP::get(*TheContext, APFloat(numberValue));
        }
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

int main()
{
  std::string entryDefinitionID = "2abb71cd4f4bebdde86c7375314649fbf184424b4356879f39758b11edd12915";
  int entryNibIndex = 0;

  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("module", *TheContext);

  Builder = std::make_unique<IRBuilder<>>(*TheContext);

  std::ifstream f("published-samples/1+1.json");
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
    return 0;
  }

  std::cout << "compiling graph" << '\n';
  Value *value = compileGraph(definition, nibIndex);
  value->print(errs());
  std::cout << '\n';

  // std::cout << data.dump(4) << std::endl;
  // InitializeNativeTarget();
  // InitializeNativeTargetAsmPrinter();
  // InitializeNativeTargetAsmParser();

  // TheJIT = ExitOnErr(KaleidoscopeJIT::Create());

  // // Open a new module.
}