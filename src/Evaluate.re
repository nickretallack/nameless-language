[%%debugger.chrome];
open Definition;
open Helpers;

let getNumber = (value: value): float =>
  switch (value) {
  | PrimitiveValue(NumberValue(number)) => number
  | _ => raise(Not_found)
  };

let conditionalBranch = (inputs: list(unit => value)): list(value) => {
  let condition = Belt.List.getExn(inputs, 0);
  let thenBranch = Belt.List.getExn(inputs, 1);
  let elseBranch = Belt.List.getExn(inputs, 2);
  switch (condition()) {
  | DefinedValue({definitionID: "yes"}) => [thenBranch()]
  | DefinedValue({definitionID: "no"}) => [elseBranch()]
  | _ => raise(Not_found)
  };
};

// A function that takes two values as inputs and returns one value as an output.
let operator =
    (operation: (value, value) => value, inputs: list(unit => value))
    : list(value) => {
  let left = Belt.List.getExn(inputs, 0, ());
  let right = Belt.List.getExn(inputs, 1, ());
  [operation(left, right)];
};

let numericOperator =
    (operation: (float, float) => float, inputs: list(unit => value))
    : list(value) => {
  let left = Belt.List.getExn(inputs, 0, ());
  let right = Belt.List.getExn(inputs, 1, ());
  [
    PrimitiveValue(
      NumberValue(operation(getNumber(left), getNumber(right))),
    ),
  ];
};

let addNumbers = numericOperator((+.));
let subtractNumbers = numericOperator((-.));
let multiplyNumbers = numericOperator(( *. ));
let divideNumbers = numericOperator((/.));

let lessThan =
  operator((left: value, right: value) =>
    (
      DefinedValue({definitionID: left < right ? "yes" : "no", values: []}): value
    )
  );

let evaluateExternal =
    (name: string, outputIndex: int, inputs: list(unit => value)): value => {
  let externalFunction =
    switch (name) {
    | "+" => addNumbers
    | "-" => subtractNumbers
    | "*" => multiplyNumbers
    | "/" => divideNumbers
    | "<" => lessThan
    | "branch" => conditionalBranch
    | _ => raise(Not_found)
    };
  let outputs = externalFunction(inputs);
  Belt.List.getExn(outputs, outputIndex);
};

let rec evaluateConnection =
        (
          definitions: definitions,
          graphImplementation: graphImplementation,
          sink: connectionSide,
          graphLazyInputs: list(unit => value),
        )
        : value => {
  let source = Belt.Map.getExn(graphImplementation.connections, sink);
  switch (source.node) {
  | GraphConnection => PrimitiveValue(TextValue("Input!"))
  | NodeConnection(nodeID) =>
    let node = Belt.Map.String.getExn(graphImplementation.nodes, nodeID);
    switch (node.kind) {
    | ReferenceNode => PrimitiveValue(TextValue("Reference!"))
    | ListNode(_length) => PrimitiveValue(TextValue("List!"))
    | DefinedNode({kind, definitionID}) =>
      let nodeDefinition = Belt.Map.String.getExn(definitions, definitionID);
      switch (kind) {
      | ValueNode =>
        switch (nodeDefinition.implementation) {
        | ConstantImplementation(primitiveValue) =>
          PrimitiveValue(primitiveValue)
        | _ => PrimitiveValue(TextValue("Value?"))
        }
      | FunctionCallNode =>
        switch (nodeDefinition.implementation) {
        | ExternalImplementation({name}) =>
          switch (source.nib) {
          | NibConnection(outputID) =>
            let outputIndex =
              findIndexExn(nodeDefinition.display.outputOrdering, outputID);
            let lazyInputs =
              Belt.List.map(nodeDefinition.display.inputOrdering, (nibID, ()) =>
                evaluateConnection(
                  definitions,
                  graphImplementation,
                  {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
                  graphLazyInputs,
                )
              );
            evaluateExternal(name, outputIndex, lazyInputs);
          | _ => raise(Not_found)
          }
        | GraphImplementation(graphImplementation) =>
          switch (source.nib) {
          | NibConnection(outputID) =>
            let lazyInputs =
              Belt.List.map(nodeDefinition.display.inputOrdering, (nibID, ()) =>
                evaluateConnection(
                  definitions,
                  graphImplementation,
                  {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
                  graphLazyInputs,
                )
              );
            evaluateGraphOutput(
              definitions,
              graphImplementation,
              outputID,
              lazyInputs,
            );
          | _ => raise(Not_found)
          }
        | _ => PrimitiveValue(TextValue("Value?"))
        }
      | AccessorNode =>
        switch (nodeDefinition.implementation) {
        | RecordTypeImplementation(_) =>
          switch (source.nib) {
          | NibConnection(outputID) =>
            let value =
              evaluateConnection(
                definitions,
                graphImplementation,
                {node: NodeConnection(nodeID), nib: ValueConnection},
                graphLazyInputs,
              );
            switch (value) {
            | DefinedValue(definedValue) =>
              if (definedValue.definitionID != definitionID) {
                raise(Not_found);
              } else {
                let fieldIndex =
                  findIndexExn(
                    nodeDefinition.display.inputOrdering,
                    outputID,
                  );
                Belt.List.getExn(definedValue.values, fieldIndex, ());
              }
            | _ => raise(Not_found)
            };
          | _ => raise(Not_found)
          }
        | _ => PrimitiveValue(TextValue("Constructor?"))
        }
      | ConstructorNode =>
        switch (nodeDefinition.implementation) {
        | RecordTypeImplementation(_) =>
          DefinedValue({
            definitionID,
            values:
              Belt.List.map(nodeDefinition.display.inputOrdering, (nibID, ()) =>
                evaluateConnection(
                  definitions,
                  graphImplementation,
                  {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
                  graphLazyInputs,
                )
              ),
          })
        | _ => PrimitiveValue(TextValue("Accessor?"))
        }
      | _ => PrimitiveValue(TextValue("Defined node?"))
      };
    };
  };
}
and evaluateGraphOutput =
    (
      definitions: definitions,
      graphImplementation: graphImplementation,
      outputID: nibID,
      lazyInputs: list(unit => value),
    )
    : value => {
  evaluateConnection(
    definitions,
    graphImplementation,
    {node: GraphConnection, nib: NibConnection(outputID)},
    lazyInputs,
  );
};

type scope = {
  definitionID,
  sourceValues:
    Belt.Map.t(connectionSide, value, ConnectionComparator.identity),
};

type scopeID = string;
type stackFrame = {
  scopeID,
  explicitConnectionSide,
};

type execution = {
  scopes: Belt.Map.String.t(scope),
  stack: list(stackFrame),
};
