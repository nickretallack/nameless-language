open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("FunctionDefinitionNode");

let make =
    (
      ~nodeID: nodeID,
      ~definition: definition,
      ~position: point,
      ~selectedNib: option(connectionNib)=?,
      ~emit: GraphActions.graphAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div className="node definition-node" style={positionStyle(position)}>
      <div className="output external">
        <Nib
          isSource=true
          isHighlighted=false
          connectionSide={node: NodeConnection(nodeID), nib: ValueConnection}
          emit
        />
      </div>
      <div className="outputs">
        {SimpleNode.renderNibs(
           displayKeywordOutputs(definition, "en"),
           "output internal",
           false,
           nodeID,
           emit,
           selectedNib,
         )}
      </div>
      <div className="definition-body"> {ReasonReact.string("todo")} </div>
      <div className="inputs">
        {SimpleNode.renderNibs(
           displayKeywordInputs(definition, "en"),
           "input internal",
           true,
           nodeID,
           emit,
           selectedNib,
         )}
      </div>
    </div>;
  },
};
