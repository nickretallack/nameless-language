open Definition;
open Helpers;

let getNumber = (value: value): float =>
  switch (value) {
  | PrimitiveValue(primitiveValue) =>
    switch (primitiveValue) {
    | NumberValue(number) => number
    | _ => raise(Not_found)
    }
  | _ => raise(Not_found)
  };

let addNumbers = (inputs: list(unit => value)): list(value) => {
  let [left, right] = Belt.List.map(inputs, input => input());
  [PrimitiveValue(NumberValue(getNumber(left) +. getNumber(right)))];
};

let evaluateExternal =
    (name: string, outputIndex: int, inputs: list(unit => value)) => {
  let externalFunction =
    switch (name) {
    | "+" => addNumbers
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
        )
        : value => {
  let source = Belt.Map.getExn(graphImplementation.connections, sink);
  switch (source.node) {
  | GraphConnection => PrimitiveValue(TextValue("Input!"))
  | NodeConnection(nodeID) =>
    let node = Belt.Map.String.getExn(graphImplementation.nodes, nodeID);
    switch (node) {
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
              findByIndexExn(nodeDefinition.display.outputOrdering, nibID =>
                nibID == outputID
              );
            let lazyInputs =
              Belt.List.map(nodeDefinition.display.inputOrdering, (nibID, ()) =>
                evaluateConnection(
                  definitions,
                  graphImplementation,
                  {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
                )
              );
            evaluateExternal(name, outputIndex, lazyInputs);
          | _ => raise(Not_found)
          }
        | _ => PrimitiveValue(TextValue("Value?"))
        }

      | _ => PrimitiveValue(TextValue("Defined node?"))
      };
    };
  };
};
let evaluateGraphOutput =
    (
      definitions: definitions,
      graphImplementation: graphImplementation,
      outputID: nibID,
    )
    : value => {
  evaluateConnection(
    definitions,
    graphImplementation,
    {node: GraphConnection, nib: NibConnection(outputID)},
  );
};
