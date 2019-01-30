open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("FunctionDefinitionNode");

let make =
    (
      ~nodeID: nodeID,
      ~definition: definition,
      ~position: point,
      ~size: point,
      ~selectedNib: option(connectionNib)=?,
      ~emit: GraphActions.graphAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div
      className="node definition-node"
      style={ReactDOMRe.Style.make(
        ~left=pixels(position.x),
        ~top=pixels(position.y),
        ~width=pixels(size.x),
        ~height=pixels(size.y),
        (),
      )}>
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
           NodeConnection(nodeID),
           emit,
           selectedNib,
         )}
      </div>
      <div className="definition-body" />
      <div className="inputs">
        {SimpleNode.renderNibs(
           displayKeywordInputs(definition, "en"),
           "input internal",
           true,
           NodeConnection(nodeID),
           emit,
           selectedNib,
         )}
      </div>
    </div>;
  },
};
