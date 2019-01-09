open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("Node");

let make =
    (
      ~nodeID: nodeID,
      ~node: node,
      ~definitions: definitions,
      ~position: point,
      ~emit: GraphActions.graphAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let makeNode = (~outputs, ~inputs=[], ~name=?, ()) =>
      <SimpleNode nodeID position emit ?name inputs outputs />;

    switch (node) {
    | ReferenceNode =>
      makeNode(
        ~outputs=[{nib: ValueConnection, name: ""}],
        ~name="Reference",
        (),
      )
    | ListNode(length) =>
      makeNode(
        ~name="List",
        ~outputs=[{nib: ValueConnection, name: ""}],
        ~inputs=
          Belt.List.makeBy(length, index =>
            {nib: PositionalConnection(index), name: string_of_int(index)}
          ),
        (),
      )
    | DefinedNode({kind, definitionID}) =>
      let definition = Belt.Map.String.getExn(definitions, definitionID);
      let {inputs, outputs} = displayDefinedNode(definition, kind, "en");
      switch (kind) {
      | FunctionDefinitionNode =>
        <FunctionDefinitionNode nodeID definition position emit />
      | _ => makeNode(~name=getName(definition, "en"), ~inputs, ~outputs, ())
      };
    };
  },
};