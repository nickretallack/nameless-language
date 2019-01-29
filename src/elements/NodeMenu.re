open Definition;
open! AppActions;

/* type category =
   | ReferenceCategory
   | ListCategory
   | ConstantCategory
   | FunctionCategory
   | FunctionInterfaceCategory
   | RecordCategory; */

/* type state = {category}; */

/* type action =
   | SelectCategory(category); */

let component = ReasonReact.statelessComponent("NodeMenu");
let make =
    (
      ~definitions: definitions,
      ~nodes: nodes,
      ~nib: explicitConnectionSide,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  /* initialState: () => {category: ReferenceCategory},
     reducer: (action: action, state: state) =>
       switch (action) {
       | SelectCategory(category) => ReasonReact.Update({category: category})
       }, */
  render: self => {
    let scope =
      switch (nib.connectionSide.node) {
      | GraphConnection => GraphScope
      | NodeConnection(nodeID) =>
        let node = Belt.Map.String.getExn(nodes, nodeID);
        isFunctionDefinitionNode(node) ?
          NodeScope(nodeID) : node.scope;
      };

    <div>
      {nib.isSource ?
         ReasonReact.null :
         <a
           onClick={_event =>
             emit(
               AddNode({
                 node: {
                   kind: ReferenceNode,
                   scope,
                 },
                 explicitConnectionSide: nib,
                 connectionNib: ValueConnection,
               }),
             )
           }>
           {ReasonReact.string("Reference")}
         </a>}
    </div>;
  },
};
